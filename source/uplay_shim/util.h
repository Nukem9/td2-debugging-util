#pragma once

#include <Windows.h>

static void RaiseHardError(const wchar_t *Message, const wchar_t *Title)
{
	// Use NtRaiseHardError to prevent MessageBoxA interference
	// https://chromium.googlesource.com/chromium/chromium/+/master/chrome/app/hard_error_handler_win.cc
	typedef LONG(
		WINAPI *
		NtRaiseHardErrorPfn)(LONG ServiceCode, ULONG ParamCount, ULONG Undocumented, PVOID Parameters, UINT ErrorMode, PULONG Response);

	typedef struct _UNICODE_STRING
	{
		USHORT Length;
		USHORT MaximumLength;
		_Field_size_bytes_part_opt_(MaximumLength, Length) PWCH Buffer;
	} UNICODE_STRING;

	const UNICODE_STRING title {
		.Length = static_cast<USHORT>(wcslen(Title) * sizeof(wchar_t)),
		.MaximumLength = static_cast<USHORT>(wcslen(Title) * sizeof(wchar_t)),
		.Buffer = const_cast<wchar_t *>(Title),
	};

	const UNICODE_STRING text {
		.Length = static_cast<USHORT>(wcslen(Message) * sizeof(wchar_t)),
		.MaximumLength = static_cast<USHORT>(wcslen(Message) * sizeof(wchar_t)),
		.Buffer = const_cast<wchar_t *>(Message),
	};

	ULONG_PTR args[] = { (ULONG_PTR)&text, (ULONG_PTR)&title, MB_OK };
	ULONG response = 0;

	auto NtRaiseHardError = reinterpret_cast<NtRaiseHardErrorPfn>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtRaiseHardError"));
	NtRaiseHardError(0x50000018, 3, 3, args, 2, &response);
}
