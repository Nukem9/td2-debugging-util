#pragma once

class GameLauncherInterfaceV004
{
private:
	uint32_t m_LaunchedProcessId = 0;
	HANDLE m_LaunchedProcessHandle = nullptr;

public:
	constexpr static const char *INTERFACE_NAME = "GameLauncherInterfaceV004";

	using ProgressUpdateCallbackPfn = bool(__stdcall *)(int ProgressPercent, void *UserData);
	using ProgressFinishedCallbackPfn = void(__stdcall *)(int Status, const char *StatusMessage, void *UserData);

	virtual void Destroy()
	{
		if (m_LaunchedProcessHandle)
			CloseHandle(m_LaunchedProcessHandle);

		delete this;
	}

	virtual UNIMPLEMENTED_VOID(StartGameA);

	virtual void StartGameW(
		const wchar_t *ExecutablePath,
		const wchar_t *LaunchParameters,
		const wchar_t *WorkingDirectory,
		ProgressUpdateCallbackPfn ProgressUpdateCallback,
		ProgressFinishedCallbackPfn ProgressFinishedCallback,
		void *UserData)
	{
		PROCESS_INFORMATION pi = {};

		STARTUPINFOW si = {};
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOWNORMAL;

		std::wstring launchParamsCopy = LaunchParameters ? LaunchParameters : L"";

		if (WorkingDirectory && wcslen(WorkingDirectory) <= 0)
			WorkingDirectory = nullptr;

		if (!CreateProcessW(
			ExecutablePath,
			launchParamsCopy.empty() ? nullptr : launchParamsCopy.data(),
			nullptr,
			nullptr,
			false,
			CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED,
			nullptr,
			WorkingDirectory,
			&si,
			&pi))
		{
			ProgressFinishedCallback(4, "Generic error", UserData);
		}
		else
		{
			m_LaunchedProcessId = pi.dwProcessId;
			m_LaunchedProcessHandle = pi.hProcess;
			ResumeThread(pi.hThread);
			CloseHandle(pi.hThread);

			ProgressUpdateCallback(1, UserData);
			ProgressFinishedCallback(8, "Connecting", UserData);
			ProgressUpdateCallback(50, UserData);
			ProgressFinishedCallback(10, "Authenticating", UserData);
			ProgressUpdateCallback(100, UserData);
			ProgressFinishedCallback(0, "Done", UserData);
		}
	}

	virtual uint32_t GetGameProcessId()
	{
		return m_LaunchedProcessId;
	}

	virtual HANDLE OpenGameProcess()
	{
		HANDLE newHandle = nullptr;

		if (m_LaunchedProcessHandle)
			DuplicateHandle(GetCurrentProcess(), m_LaunchedProcessHandle, GetCurrentProcess(), &newHandle, 0, false, DUPLICATE_SAME_ACCESS);

		return newHandle;
	}

	virtual void InitializeLocale(const char *LanguageCode, const char *CountryCode)
	{
		// We don't care about locale
	}

	virtual UNIMPLEMENTED_VOID(VFunc06); // Virtual destructor?
};
