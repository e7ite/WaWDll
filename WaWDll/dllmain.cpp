#include "stdafx.hpp"
#include "detours.hpp"

BOOL APIENTRY DllMain(HMODULE H, DWORD Reason, LPVOID P)
{
    switch (Reason)
    {
        case DLL_PROCESS_ATTACH:
        {
            if (!AllocConsole())
                GameData::Com_Error(0, FormatError(GetLastError()).c_str());
            if (!SetConsoleTitle("WaW Hack"))
                GameData::Com_Error(0, FormatError(GetLastError()).c_str());
            FILE *f;
            char errDesc[0x40] = { 0 };
            if (errno_t err = freopen_s(&f, "CONOUT$", "w", stdout))
                GameData::Com_Error(0, "Failed to open stdout stream! %s", strerror_s<0x40>(errDesc, err));

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
            std::stringstream errs;
            bool errorOccurred = false;
            if (!FreeConsole())
            {
                errorOccurred = true;
                errs << FormatError(GetLastError()) << std::endl;
            }
            if (fclose(stdout) == EOF)
            {
                errorOccurred = true;
                errs << FormatError(GetLastError()) << std::endl;
            }

            for (const auto &i : detours)
                RemoveDetour(i);
            detours.clear();

            if (errorOccurred)
                GameData::Com_Error(0, errs.str().c_str());
        }
        break;
    }

    return TRUE;
} 
