#include <Windows.h>
#include <detours/detours.h>
#include <SoftPub.h>
#include <stdio.h>
#include "util.h"

PEXCEPTION_POINTERS LastExceptionPointers = nullptr;
HANDLE WaitForExceptionEvent = CreateEventA(nullptr, false, false, nullptr);

DWORD WINAPI DebuggerHelperThread(LPVOID Parameter)
{
	using NtRaiseExceptionPfn = LONG(NTAPI *)(PEXCEPTION_RECORD ExceptionRecord, PCONTEXT ThreadContext, BOOLEAN HandleException);
	auto NtRaiseException = reinterpret_cast<NtRaiseExceptionPfn>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtRaiseException"));

	WaitForSingleObject(WaitForExceptionEvent, INFINITE);

	//
	// Under normal circumstances we wouldn't need insane hacks to suspend a thread and continue its execution
	// on yet another thread. However, VMProtect uses ThreadHideFromDebugger which can't be trivially bypassed
	// on Windows 10+. At least not without a kernel driver.
	//
	// So instead of patching all ThreadHideFromDebugger checks, we suspend the main thread via an exception,
	// "copy" its context, and resume it on a new thread. Suddenly breakpoints work again.
	//
	// TODO: Stack limits are broken (TEB fixup required)
	// TODO: We're depending on the debugger to raise STATUS_BREAKPOINT in a VEH
	//
	LastExceptionPointers->ContextRecord->Rip += 1;
	NtRaiseException(LastExceptionPointers->ExceptionRecord, LastExceptionPointers->ContextRecord, TRUE);

	// Never gets here with a debugger attached
	return 0;
}

LONG WINAPI VectoredHandler(PEXCEPTION_POINTERS ExceptionInfo)
{
	switch (ExceptionInfo->ExceptionRecord->ExceptionCode)
	{
	case 0x40010006:
	case 0x406D1388:
		break;

	case STATUS_BREAKPOINT:
		LastExceptionPointers = ExceptionInfo;
		SetEvent(WaitForExceptionEvent);

		Sleep(INFINITE);
		break;

	default:
	{
		wchar_t message[512] = {};
		swprintf_s(message, L"Exception 0x%X at 0x%llX", ExceptionInfo->ExceptionRecord->ExceptionCode, ExceptionInfo->ContextRecord->Rip);

		// RaiseHardError(message, L"Info");
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
		RaiseHardError(L"Attach debugger now", L"Info");
		return false;
	}();

	// Zero indicates success
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
	// RaiseHardError(L"Attach debugger", L"Info");
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
