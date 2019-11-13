#include "detours.h"

BOOL APIENTRY DllMain(HMODULE H, DWORD Reason, LPVOID P)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		AllocConsole();
		SetConsoleTitle("WaW Hack");
        FILE *f;
		freopen_s(&f, "CONOUT$", "w", stdout);

		InsertDetour(&Menu_PaintAll, Menu_PaintAllDetourInvoke);
		InsertDetour(&R_EndFrame, R_EndFrameDetour);
		InsertDetour(&Cmd_ExecuteSingleCommand, Cmd_ExecuteSingleCommandDetour);
		InsertDetour(&TopLevelExceptionFilter, TopLevelExceptionFilterDetour);
		InsertDetour(&CL_WritePacket, CL_WritePacketDetour);
		InsertDetour(&AimTarget_GetTagPos_0, AimTarget_GetTagPos_0DetourInvoke);
		InsertDetour(&Menu_HandleMouseMove, Menu_HandleMouseMoveDetour);
		InsertDetour(&Cbuf_AddTextHook, Cbuf_AddTextDetourInvoke);
		InsertDetour(&CG_PredictPlayerState_Internal, 
			CG_PredictPlayerState_InternalDetour);
		InsertDetour(&CL_CreateNewCommands, CL_CreateNewCommandsDetourInvoke);
		InsertDetour(&IN_MouseEvent, IN_MouseEventDetour);
		InsertDetour(&VM_Notify, VM_NotifyDetourInvoke);
        InsertDetour(&CG_DamageFeedback, CG_DamageFeedbackDetourInvoke);
        InsertDetour(&Com_Printf, Com_PrintfDetour);
		break;
	case DLL_PROCESS_DETACH:
		FreeConsole();

		for (const auto &i : GameData::detours)
			RemoveDetour(i);
		break;
	}

	return TRUE;
} 