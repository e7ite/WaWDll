#include "stdafx.hpp"
#include "nonhost_menu.hpp"

// Forward declarations for detours
namespace GameData
{
    enum scriptInstance_t : int;

    extern void __usercall *AimTarget_GetTagPos_0;
    void AimTarget_GetTagPos_0DetourInvoke(centity_s *cent,
        unsigned short bone, float *out);

    extern void (__cdecl *CL_CreateNewCommands)();
    void CL_CreateNewCommandsDetourInvoke();
    void CL_CreateNewCommandsDetour();

    extern void __usercall *VM_Notify;
    void VM_NotifyDetourInvoke(scriptInstance_t inst,
        int notifyListOwnerId, int stringValue, struct VariableValue *top);
    void VM_NotifyDetour(scriptInstance_t inst, int notifyListOwnerId,
        int stringValue, struct VariableValue *top);

    extern void __usercall *CG_DamageFeedback;
    void CG_DamageFeedbackDetourInvoke(int localClientNum, int yawByte,
        int pitchByte, int damage);
    bool CG_DamageFeedbackDetour(int localClientNum, int yawByte, int pitchByte,
        int damage);
}

struct DetourData
{
    PVOID *targetFunction;
    PVOID *detourFunction;

    DetourData(PVOID *targetFunction, PVOID *detourFunction)
        : targetFunction(targetFunction), detourFunction(detourFunction) {}
};

std::vector<DetourData> detours;

void DetourFunction(PVOID *targetFunction, PVOID *detourFunction)
{
    // Initiate Detour Transcation API
    DetourTransactionBegin();

    // Enlists Current Thread in Transaction to Appropriately Update
    // Instruction Pointers for That Thread
    DetourUpdateThread(GetCurrentThread());

    // Allocates the Detour for the Target Function
    DetourAttach(targetFunction, detourFunction);

    // Overwrites the first instruction in the target function to jmp
    // to Detour before returning to target function to restore program flow
    DetourTransactionCommit();
}

void DetourRemove(PVOID *targetFunction, PVOID *detourFunction)
{
    // Initiate Detour Transcation API 
    DetourTransactionBegin();

    // Enlists Current Thread in Transaction to Appropriately Update
    // Instruction Pointers for That Thread
    DetourUpdateThread(GetCurrentThread());

    // Deallocates the Detour for the Target Function
    DetourDetach(targetFunction, detourFunction);

    // Restores overwritten instructions of Target Function
    // and restores Target Function Pointer to point to original
    // function
    DetourTransactionCommit();
}

void InsertDetour(LPVOID targetFunction, LPVOID detourFunction)
{
    detours.push_back(DetourData((PVOID *)targetFunction, (PVOID *)detourFunction));
    DetourFunction((PVOID *)targetFunction, (PVOID *)detourFunction);
}

BOOL APIENTRY DllMain(HMODULE H, DWORD Reason, LPVOID P)
{
    switch (Reason)
    {
        case DLL_PROCESS_ATTACH:
        {
            // Create a debug console, set its title and redirect stdout to it
            if (!AllocConsole())
                GameData::Com_Error(0, "AllocConsole() %s", FormatError(GetLastError()).c_str());
            if (!SetConsoleTitle("WaW Hack"))
                GameData::Com_Error(0, "SetConsoleTitle(): %s", FormatError(GetLastError()).c_str());
            FILE *f;
            char errDesc[0x40] = { 0 };
            if (errno_t err = freopen_s(&f, "CONOUT$", "w", stdout))
                GameData::Com_Error(0, "freopen_s(stdout) %s", strerror_s<0x40>(errDesc, err));
            if (errno_t err = freopen_s(&f, "CONOUT$", "w", stderr))
                GameData::Com_Error(0, "freopen_s(stderr) %s", strerror_s<0x40>(errDesc, err));

            // Detour all the functions designated
            InsertDetour(&GameData::Menu_PaintAll, GameData::Menu_PaintAllDetourInvoke);
            InsertDetour(&GameData::TopLevelExceptionFilter, GameData::TopLevelExceptionFilterDetour);
            InsertDetour(&GameData::AimTarget_GetTagPos_0, GameData::AimTarget_GetTagPos_0DetourInvoke);
            InsertDetour(&GameData::Menu_HandleMouseMove, GameData::Menu_HandleMouseMoveDetour);
            InsertDetour(&GameData::Cbuf_AddTextHook, GameData::Cbuf_AddTextDetourInvoke);
            InsertDetour(&GameData::CL_CreateNewCommands, GameData::CL_CreateNewCommandsDetourInvoke);
            InsertDetour(&GameData::IN_MouseEvent, GameData::IN_MouseEventDetour);
            InsertDetour(&GameData::VM_Notify, GameData::VM_NotifyDetourInvoke);
            InsertDetour(&GameData::CG_DamageFeedback, GameData::CG_DamageFeedbackDetourInvoke);
            InsertDetour(&GameData::Com_Printf, GameData::Com_PrintfDetour);
        }
        break;
        case DLL_PROCESS_DETACH:
        {
            // Free the allocated console and closes stdout stream
            if (!FreeConsole())
                GameData::Com_Error(0, FormatError(GetLastError()).c_str());
            fclose(stdout);
            fclose(stderr);

            // Remove all the detours used
            for (auto i : detours)
                DetourRemove(i.targetFunction, i.detourFunction);
            detours.clear();
        }
        break;
    }

    return TRUE;
} 
