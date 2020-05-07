#include "stdafx.h"
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

            InsertDetour(&GameData::Menu_PaintAll, Menu_PaintAllDetourInvoke);
            InsertDetour(&GameData::Cmd_ExecuteSingleCommand, Cmd_ExecuteSingleCommandDetour);
            InsertDetour(&GameData::TopLevelExceptionFilter, TopLevelExceptionFilterDetour);
            InsertDetour(&GameData::CL_WritePacket, CL_WritePacketDetour);
            InsertDetour(&GameData::AimTarget_GetTagPos_0, AimTarget_GetTagPos_0DetourInvoke);
            InsertDetour(&GameData::Menu_HandleMouseMove, Menu_HandleMouseMoveDetour);
            InsertDetour(&GameData::Cbuf_AddTextHook, Cbuf_AddTextDetourInvoke);
            InsertDetour(&GameData::CG_PredictPlayerState_Internal, CG_PredictPlayerState_InternalDetour);
            InsertDetour(&GameData::CL_CreateNewCommands, CL_CreateNewCommandsDetourInvoke);
            InsertDetour(&GameData::IN_MouseEvent, IN_MouseEventDetour);
            InsertDetour(&GameData::VM_Notify, VM_NotifyDetourInvoke);
            InsertDetour(&GameData::CG_DamageFeedback, CG_DamageFeedbackDetourInvoke);
            InsertDetour(&GameData::Com_Printf, Com_PrintfDetour);
            break;
        case DLL_PROCESS_DETACH:
            FreeConsole();

            for (const auto &i : detours)
                RemoveDetour(i);
            detours.clear();
        break;
    }

    return TRUE;
} 
