#include "stdafx.hpp"
#include "detours.hpp"

BOOL APIENTRY DllMain(HMODULE H, DWORD Reason, LPVOID P)
{
    switch (Reason)
    {
        case DLL_PROCESS_ATTACH:
        {
            // Create a debug console, set its title and redirect stdout to it
            if (!AllocConsole())
                GameData::Com_Error(0, FormatError(GetLastError()).c_str());
            if (!SetConsoleTitle("WaW Hack"))
                GameData::Com_Error(0, FormatError(GetLastError()).c_str());
            FILE *f;
            char errDesc[0x40] = { 0 };
            if (errno_t err = freopen_s(&f, "CONOUT$", "w", stdout))
                GameData::Com_Error(0, "Failed to open stdout stream! %s", strerror_s<0x40>(errDesc, err));

            // Detour all the functions designated
            InsertDetour(&GameData::Menu_PaintAll, GameData::Menu_PaintAllDetourInvoke);
            InsertDetour(&GameData::TopLevelExceptionFilter, GameData::TopLevelExceptionFilterDetour);
            InsertDetour(&GameData::CL_WritePacket, GameData::CL_WritePacketDetour);
            InsertDetour(&GameData::AimTarget_GetTagPos_0, GameData::AimTarget_GetTagPos_0DetourInvoke);
            InsertDetour(&GameData::Menu_HandleMouseMove, GameData::Menu_HandleMouseMoveDetour);
            InsertDetour(&GameData::Cbuf_AddTextHook, GameData::Cbuf_AddTextDetourInvoke);
            InsertDetour(&GameData::CG_PredictPlayerState_Internal, GameData::CG_PredictPlayerState_InternalDetour);
            InsertDetour(&GameData::CL_CreateNewCommands, GameData::CL_CreateNewCommandsDetourInvoke);
            InsertDetour(&GameData::IN_MouseEvent, GameData::IN_MouseEventDetour);
            InsertDetour(&GameData::VM_Notify, GameData::VM_NotifyDetourInvoke);
            InsertDetour(&GameData::CG_DamageFeedback, GameData::CG_DamageFeedbackDetourInvoke);
            //InsertDetour(&GameData::Com_Printf, GameData::Com_PrintfDetour);
        }
        break;
        case DLL_PROCESS_DETACH:
        {
            // Free the allocated console
            if (!FreeConsole())
                GameData::Com_Error(0, FormatError(GetLastError()).c_str());

            // Remove all the detours used
            for (auto i : detours)
                DetourRemove(i.targetFunction, i.detourFunction);
            detours.clear();
        }
        break;
    }

    return TRUE;
} 
