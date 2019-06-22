#include "detours.h"

BOOL APIENTRY DllMain(HMODULE H, DWORD Reason, LPVOID P)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		AllocConsole();
		SetConsoleTitle("WaW Hack");
		freopen("CONOUT$", "w", stdout);

		InsertDvar("cl_ingame");
		InsertDvar("cg_fov");
		InsertDvar("perk_weapSpreadMultiplier");

		Variables::enableAimbot = true;
		Variables::autoShoot = true;
		Variables::noRecoil = true;

		std::cout << std::hex << CL_GetUserCmd(0) << std::endl;
		std::cout << offsetof(cpose_t, origin) << std::endl;

		BuildMenu();

		InsertDetour((QWORD)&Menu_PaintAll, (QWORD)Menu_PaintAllStub);
		InsertDetour((QWORD)&R_EndFrame, (QWORD)R_EndFrameDetour);
		InsertDetour((QWORD)&Cmd_ExecuteSingleCommand,
			(QWORD)Cmd_ExecuteSingleCommandDetour);
		InsertDetour((QWORD)&TopLevelExceptionFilter,
			(QWORD)TopLevelExceptionFilterDetour);
		InsertDetour((QWORD)&CL_WritePacket, (QWORD)CL_WritePacketDetour);
		InsertDetour((QWORD)&AimTarget_GetTagPos_0,
			(QWORD)AimTarget_GetTagPos_0Stub);
		InsertDetour((QWORD)&Menu_HandleMouseMove,
			(QWORD)Menu_HandleMouseMoveDetour);
		InsertDetour((QWORD)&Cbuf_AddText, (QWORD)Cbuf_AddTextStub);
		InsertDetour((QWORD)&CG_PredictPlayerState_Internal, 
			(QWORD)CG_PredictPlayerState_InternalDetour);
		break;

	case DLL_PROCESS_DETACH:
		FreeConsole();

		for (const auto &i : GameData::detours)
			RemoveDetour(i);
		break;
	}

	return TRUE;
}