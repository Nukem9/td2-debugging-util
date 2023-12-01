#pragma once

class GameClientInterfaceV012
{
public:
	constexpr static const char *INTERFACE_NAME = "GameClientInterfaceV012";
	
	using ProgressUpdateCallbackPfn = bool(__stdcall *)(int ProgressPercent, void *UserData);
	using ProgressFinishedCallbackPfn = void(__stdcall *)(int Status, const char *StatusMessage, void *UserData);

	virtual void Initialize(ProgressUpdateCallbackPfn ProgressUpdateCallback, ProgressFinishedCallbackPfn ProgressFinishedCallback, void *UserData)
	{
		// Ignored. Gets called on startup.
	}

	virtual void Destroy()
	{
		delete this;
	}

	virtual void ConnectionReset()
	{
		// Ignored. Gets called in the main menu.
	}

	virtual bool PopNetworkMessage(void *Buffer, size_t BufferSize)
	{
		return false;
	}

	virtual void PushNetworkMessage(void *Buffer, size_t BufferSize)
	{
		// Discarded
	}

	virtual void PollStatus(void *, void *, void *)
	{
		// Callbacks ignored
	}

	virtual UNIMPLEMENTED_VOID(ValidateServerHost);
	virtual UNIMPLEMENTED_VOID(VFunc07);
	virtual UNIMPLEMENTED_VOID(VFunc08);
	virtual UNIMPLEMENTED_VOID(VFunc09);

	virtual void *NetProtect()
	{
		return nullptr;
	}

	virtual UNIMPLEMENTED_VOID(VFunc11);
	virtual UNIMPLEMENTED_VOID(VFunc12);
	virtual UNIMPLEMENTED_VOID(VFunc13);
	virtual UNIMPLEMENTED_VOID(VFunc14);
	virtual UNIMPLEMENTED_VOID(VFunc15);
	virtual UNIMPLEMENTED_VOID(VFunc16);
	virtual UNIMPLEMENTED_VOID(VFunc17);
	virtual UNIMPLEMENTED_VOID(VFunc18);
	virtual UNIMPLEMENTED_VOID(VFunc19);
};
