#include "GameLauncherInterface.h"
#include "GameClientInterface.h"

extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(Cerberus_BeginFrame);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(Cerberus_EndFrame);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(Cerberus_GameRoundEnd);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(Cerberus_GameRoundStart);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(Cerberus_PlayerDespawn);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(Cerberus_PlayerDowned);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(Cerberus_PlayerRevive);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(Cerberus_PlayerSpawn);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(Cerberus_PlayerTakeDamage);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(Cerberus_PlayerTick);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(Cerberus_PlayerUseWeapon);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(ClientAuth_ClientWriteChallengeResponse);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(ClientAuth_Destroy);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(ClientAuth_Initialize);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(CreateClientAuth);

#ifdef _M_IX86
#pragma comment(linker, "/export:CreateGameClient=_CreateGameClient@4")
#endif

extern "C" __declspec(dllexport) void *__stdcall CreateGameClient(const char *InterfaceName)
{
	if (!InterfaceName || _stricmp(InterfaceName, GameClientInterfaceV012::INTERFACE_NAME) != 0)
		return nullptr;

	return new GameClientInterfaceV012();
}

#ifdef _M_IX86
#pragma comment(linker, "/export:CreateGameLauncher=_CreateGameLauncher@4")
#endif

extern "C" __declspec(dllexport) void *__stdcall CreateGameLauncher(const char *InterfaceName)
{
	if (!InterfaceName || _stricmp(InterfaceName, GameLauncherInterfaceV004::INTERFACE_NAME) != 0)
		return nullptr;

	return new GameLauncherInterfaceV004();
}

extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(CreateThirdPartyLauncher);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClientP2P_BeginSession);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClientP2P_Cerberus);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClientP2P_EndSession);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClientP2P_InitLocalization);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClientP2P_PollForMessageToPeer);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClientP2P_PollStatus);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClientP2P_ReceiveMessageFromPeer);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClientP2P_RegisterPeer);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClientP2P_ResetState);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClientP2P_SetLogCallback);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClientP2P_SetMaxAllowedMessageLength);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClientP2P_UnregisterPeer);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClientP2P_UpdatePlatformUserAuthTicket);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClient_ConnectionReset);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClient_Destroy);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClient_Initialize);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClient_NetProtect);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClient_PollStatus);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClient_PopNetworkMessage);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClient_PushNetworkMessage);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClient_SetMaxAllowedMessageLength);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClient_SetPlatformUserAuthTicket);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameClient_ValidateServerHost);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameLauncher_Destroy);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameLauncher_GetGameProcessId);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameLauncher_OpenGameProcess);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameLauncher_StartGameA);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(GameLauncher_StartGameW);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(NetProtectClient_GetProtectMessageOutputLength);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(NetProtectClient_ProtectMessage);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(NetProtectClient_UnprotectMessage);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(ThirdPartyLauncher_Destroy);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(ThirdPartyLauncher_Initialize);
extern "C" __declspec(dllexport) UNIMPLEMENTED_VOID(ThirdPartyLauncher_SetServer);
