#include "detours.h"

BOOL APIENTRY DllMain(HMODULE H, DWORD Reason, LPVOID P)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		AllocConsole();
		SetConsoleTitle("WaW Hack");
		freopen("CONOUT$", "w", stdout);

		InsertDetour(&Menu_PaintAll, Menu_PaintAllStub);
		InsertDetour(&R_EndFrame, R_EndFrameDetour);
		InsertDetour(&Cmd_ExecuteSingleCommand, Cmd_ExecuteSingleCommandDetour);
		InsertDetour(&TopLevelExceptionFilter, TopLevelExceptionFilterDetour);
		InsertDetour(&CL_WritePacket, CL_WritePacketDetour);
		InsertDetour(&AimTarget_GetTagPos_0, AimTarget_GetTagPos_0Stub);
		InsertDetour(&Menu_HandleMouseMove, Menu_HandleMouseMoveDetour);
		InsertDetour(&Cbuf_AddTextHook, Cbuf_AddTextStub);
		InsertDetour(&CG_PredictPlayerState_Internal, 
			CG_PredictPlayerState_InternalDetour);
		InsertDetour(&CL_CreateNewCommands, CL_CreateNewCommandsStub);
		break;
	case DLL_PROCESS_DETACH:
		FreeConsole();

		for (const auto &i : GameData::detours)
			RemoveDetour(i);
		break;
	}

	return TRUE;
} 