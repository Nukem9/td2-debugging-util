#include <Windows.h>
#include <detours/detours.h>
#include <SoftPub.h>
#include "util.h"

PEXCEPTION_POINTERS LastExceptionPointers = nullptr;
PNT_TIB LastThreadTib = nullptr;
HANDLE WaitForExceptionEvent = CreateEventA(nullptr, false, false, nullptr);

DWORD WINAPI DebuggerHelperThread(LPVOID Parameter)
{
	WaitForSingleObject(WaitForExceptionEvent, INFINITE);

	//
	// Under normal circumstances we wouldn't need insane hacks to suspend a thread and continue its execution
	// on yet another thread. However, VMProtect uses ThreadHideFromDebugger which can't be trivially bypassed
	// on Windows 10+. At least not without a kernel driver.
	//
	// So instead of patching all ThreadHideFromDebugger checks, we suspend the main thread via an exception,
	// "copy" its context, and resume it on a new thread. Suddenly breakpoints work again.
	//
	auto currentThreadTib = reinterpret_cast<PNT_TIB>(_readgsbase_u64());
	currentThreadTib->StackBase = LastThreadTib->StackBase;
	currentThreadTib->StackLimit = LastThreadTib->StackLimit;
	currentThreadTib->ArbitraryUserPointer = LastThreadTib->ArbitraryUserPointer;

	LastExceptionPointers->ContextRecord->Rip += 1;

	RtlRestoreContext(LastExceptionPointers->ContextRecord, nullptr);
	return 0;
}

LONG WINAPI VectoredHandler(PEXCEPTION_POINTERS ExceptionInfo)
{
	switch (ExceptionInfo->ExceptionRecord->ExceptionCode)
	{
	case STATUS_BREAKPOINT:
	{
		if (!LastExceptionPointers)
		{
			// Windows throws STATUS_STACK_OVERFLOW if we don't commit the required stack space
			// before switching to a new thread. Try 512KB up front.
			CommitStackPages(512 * 1024);

			LastExceptionPointers = ExceptionInfo;
			LastThreadTib = reinterpret_cast<PNT_TIB>(_readgsbase_u64());

			SetEvent(WaitForExceptionEvent);
			Sleep(INFINITE);
		}
	}
	break;
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

decltype(&WinVerifyTrust) OldWinVerifyTrust = WinVerifyTrust;

LONG WINAPI HookedWinVerifyTrust(HWND hwnd, GUID *pgActionID, LPVOID pWVTData)
{
	// Called after WinMain but before EAC libraries are loaded
	static bool once = []()
	{
		__debugbreak();
		RaiseHardError(L"Attach debugger now", L"Info");

		return false;
	}();

	// Return zero to bypass all signature checks
	return 0;
}

decltype(&GetSystemTimeAsFileTime) OldGetSystemTimeAsFileTime = GetSystemTimeAsFileTime;

void WINAPI HookedGetSystemTimeAsFileTime(LPFILETIME lpSystemTimeAsFileTime)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(reinterpret_cast<void **>(&OldGetSystemTimeAsFileTime), HookedGetSystemTimeAsFileTime);
	DetourTransactionCommit();

	GetSystemTimeAsFileTime(lpSystemTimeAsFileTime);

	// Called immediately after executable unpacking but before WinMain (_security_init_cookie)
	// RaiseHardError(L"Attach debugger now", L"Info");
}

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		AddVectoredExceptionHandler(1, VectoredHandler);
		CloseHandle(CreateThread(nullptr, 0, DebuggerHelperThread, nullptr, 0, nullptr));

		DetourSetIgnoreTooSmall(true);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(reinterpret_cast<void **>(&OldWinVerifyTrust), HookedWinVerifyTrust);
		DetourAttach(reinterpret_cast<void **>(&OldGetSystemTimeAsFileTime), HookedGetSystemTimeAsFileTime);
		DetourTransactionCommit();
	}

	return TRUE;
}
