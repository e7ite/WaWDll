#include "stdafx.h"
#include "detours.h"
#include "esp.h"

namespace GameData
{
    void __usercall* Menu_PaintAll = (void __usercall*)Menu_PaintAll_a;
    void(*CL_SendCmd)() = (void(*)())CL_SendCmd_a;
    LONG(__stdcall* TopLevelExceptionFilter)(struct _EXCEPTION_POINTERS* ExceptionInfo)
        = (LONG(__stdcall*)(_EXCEPTION_POINTERS*))TopLevelExceptionFilter_a;
    void(__cdecl* CL_WritePacket)() = (void(__cdecl*)())CL_WritePacket_a;
    void(__fastcall* CG_DrawNightVisionOverlay)(int localClientNum)
        = (void(__fastcall*)(int))CG_DrawNightVisionOverlay_a;
    void __usercall* AimTarget_GetTagPos_0 = (void __usercall*)AimTarget_GetTagPos_0_a;
    int(__cdecl* Menu_HandleMouseMove)(GameData::ScreenPlacement* scrPlace, void* menu)
        = (int(__cdecl*)(ScreenPlacement*, void*))Menu_HandleMouseMove_a;
    void(__cdecl* CG_Draw2DInternal)() = (void(__cdecl*)())CG_Draw2DInternal_a;
    void(__cdecl* UI_Refresh)(int localClientNum) = (void(__cdecl*)(int))UI_Refresh_a;
    void(__cdecl* CL_KeyEvent)(int localClientNum, int value, int down,
        unsigned int time) = (void(*)(int, int, int, unsigned int))CL_KeyEvent_a;
    sysEvent_t*(__cdecl* Win_GetEvent)(sysEvent_t* result, int unk)
        = (sysEvent_t*(*)(sysEvent_t*, int))Win_GetEvent_a;
    void __usercall* Cbuf_AddTextHook = (void __usercall*)Cbuf_AddText_a;
    void(__cdecl* CG_PredictPlayerState_Internal)(int localClientNum)
        = (void(__cdecl*)(int))CG_PredictPlayerStateInternal_a;
    void __usercall* CL_CreateCmd = (void __usercall*)CL_CreateCmd_a;
    void(__cdecl* CL_CreateNewCommands)()
        = (void(__cdecl*)())CL_CreateNewCommands_a;
    void(__cdecl* IN_MouseEvent)(int mstate) = (void(__cdecl*)(int))IN_MouseEvent_a;
    void __usercall* VM_Notify = (void __usercall*)VM_Notify_a;
    void __usercall* CG_DamageFeedback = (void __usercall*)CG_DamageFeedback_a;
    int(*Com_Printf)(int channel, const char* format, ...)
        = (int(*)(int, const char*, ...))Com_Printf_a;
}

void DetourFunction(DWORD targetFunction, DWORD detourFunction)
{
    /*Initiate Detour Transcation API*/
    DetourTransactionBegin();

    /*Enlists Current Thread in Transaction to Appropriately Update
      Instruction Pointers for That Thread*/
    DetourUpdateThread(GetCurrentThread());

    /*Allocates the Detour for the Target Function*/
    DetourAttach(reinterpret_cast<PVOID*>(targetFunction),
        reinterpret_cast<PVOID*>(detourFunction));

    /*Overwrites the first instruction in the target function to jmp
      to Detour before returning to target function to restore program flow*/
    DetourTransactionCommit();
}

void DetourRemove(DWORD targetFunction, DWORD detourFunction)
{
    /*Initiate Detour Transcation API*/
    DetourTransactionBegin();

    /*Enlists Current Thread in Transaction to Appropriately Update
      Instruction Pointers for That Thread*/
    DetourUpdateThread(GetCurrentThread());

    /*Deallocates the Detour for the Target Function*/
    DetourDetach(reinterpret_cast<PVOID*>(targetFunction),
        reinterpret_cast<PVOID*>(detourFunction));

    /*Restores overwritten instructions of Target Function
      and restores Target Function Pointer to point to original
      function*/
    DetourTransactionCommit();
}

void RemoveDetour(QWORD bytes)
{
    DetourRemove((bytes&  ((QWORD)UINT_MAX << 32)) >> 32,
        (bytes&  UINT_MAX));
}

void InsertDetour(LPVOID targetFunction, LPVOID detourFunction)
{
    detours.push_back(((QWORD)targetFunction << 32) | (QWORD)detourFunction);
    DetourFunction((DWORD)targetFunction, (DWORD)detourFunction);
}

void __declspec(naked) Menu_PaintAllDetourInvoke(GameData::UiContext* dc)
{
    __asm
    {
        push        esi
        call        Menu_PaintAllDetour
        add         esp, 4
        pop         edi
        pop         ebp
        pop         ebx
        add         esp, 410h
        ret
    }
}

void Menu_PaintAllDetour(GameData::UiContext* dc)
{
    Menu& menu = Menu::Instance();
    GameData::EnterCriticalSection(&menu.critSection);

    if (!dvarsInitialized)
    {
        if (InsertDvar("cl_ingame")
            && InsertDvar("cg_fov")
            && InsertDvar("perk_weapSpreadMultiplier")
            && InsertDvar("sv_cheats")
            && InsertDvar("player_sustainAmmo"))
        {
            dvarsInitialized = true;
            menu.GetOptionData(MISC_MENU, "FOV").data.integer = 65;
        }
        else
            GameData::Com_Error(0, "Dvars failed to load\n");
    }

    if (GameData::IN_IsForegroundWindow())
        menu.MonitorKeys();

    if (menu.open)
        menu.Execute();

    RenderESP();

    GameData::LeaveCriticalSection(&menu.critSection);
}

void Cmd_ExecuteSingleCommandDetour(int localClientNum, int controllerIndex, const char* text)
{
    return GameData::Cmd_ExecuteSingleCommand(localClientNum, controllerIndex, text);
}

void TopLevelExceptionFilterDetour(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
    PDWORD_PTR currESP =
        (PDWORD_PTR)ExceptionInfo->ContextRecord->Esp;
    PDWORD_PTR currEIP =
        (PDWORD_PTR)ExceptionInfo->ContextRecord->Eip;

    printf("EAX: %p\n", (void*)ExceptionInfo->ContextRecord->Eax);
    printf("EBX: %p\n", (void*)ExceptionInfo->ContextRecord->Ebx);
    printf("ECX: %p\n", (void*)ExceptionInfo->ContextRecord->Ecx);
    printf("EDX: %p\n", (void*)ExceptionInfo->ContextRecord->Edx);
    printf("EDI: %p\n", (void*)ExceptionInfo->ContextRecord->Edi);
    printf("ESI: %p\n", (void*)ExceptionInfo->ContextRecord->Esi);
    printf("EBP: %p\n", (void*)ExceptionInfo->ContextRecord->Ebp);
    printf("ESP: %p\n", (void*)currESP);
    printf("EIP: %p\n", (void*)currEIP);

    printf("\nSTACK VIEW:\n");
    for (int i = 0; i < 8; i++)
    {
        if (i)
            ++currESP;
        printf("%p: %p ", (void*)currESP, (void*)*currESP);
        for (int j = 0; j < 3; j++)
            printf("%p ", (void*)*(++currESP));
        printf("\n");
    }

    if (CopyTextToClipboard(GameData::va("%p", currEIP)))
        printf("\nInstruction pointer copied to clipboard\n");
    else
        printf("\nProblem copying instruction pointer to clipboard\n");
}

void CL_WritePacketDetour()
{
    GameData::CL_WritePacket();
}

void CL_SendCmdDetour()
{
    GameData::CL_SendCmd();
}

void CG_DrawNightVisionOverlayDetour(int localClientNum)
{
    GameData::CG_DrawNightVisionOverlay(localClientNum);
}

void __declspec(naked) AimTarget_GetTagPos_0DetourInvoke(GameData::centity_s* cent,
    unsigned short bone, float* out)
{
    __asm
    {
        mov         eax, [esp + 4]
        push        eax
        push        esi
        push        ecx
        push        0
        call        GameData::AimTarget_GetTagPos
        add         esp, 10h
        ret
    }
}

int Menu_HandleMouseMoveDetour(GameData::ScreenPlacement* scrPlace, void* item)
{
    Menu& menu = Menu::Instance();
    GameData::EnterCriticalSection(&menu.critSection);

    if (!menu.open)
    { 
        GameData::LeaveCriticalSection(&menu.critSection);
        return GameData::Menu_HandleMouseMove(scrPlace, item);
    }
    
    GameData::LeaveCriticalSection(&menu.critSection);
    return 0;
}

void CG_Draw2DInternalDetour()
{
    return GameData::CG_Draw2DInternal();
}

void UI_RefreshDetour(int localClientNum)
{
    return GameData::UI_Refresh(localClientNum);
}

void CL_KeyEventDetour(int localClientNum, int key, int down, int time)
{
    Menu& menu = Menu::Instance();

    GameData::EnterCriticalSection(&menu.critSection);

    OptionData& aimKey = menu.GetOptionData(AIMBOT_MENU, "Aim Key");
    OptionData& autoShoot = menu.GetOptionData(AIMBOT_MENU, "Auto Shoot");

    if (InGame() && GameData::keys[key].binding
        && !*(int*)0x208E938
        && (aimKey.data.integer != 1 || !strcmp(GameData::keys[key].binding, "+attack"))
        && autoShoot.data.boolean)
        return;

    GameData::LeaveCriticalSection(&menu.critSection);

    return GameData::CL_KeyEvent(localClientNum, key, down, time);
}

void __declspec(naked) Cbuf_AddTextDetourInvoke(const char* text,
    int localClientNum)
{
    __asm
    {
        push        eax
        push        ecx
        push        ecx
        push        eax
        call        Cbuf_AddTextDetour
        add         esp, 8h
        cmp         al, 0
        pop         ecx
        pop         eax
        jz          LABEL_1
        push        ebp
        push        esi
        push        edi
        push        22990F8h
        push        594208h
        ret
LABEL_1:
        pop         edx
        jmp         edx
    }
}

bool Cbuf_AddTextDetour(const char* text, int localClientNum)
{
    return true;
}

void CG_PredictPlayerState_InternalDetour(int localClientNum)
{
    GameData::CG_PredictPlayerState_Internal(localClientNum);
}

void __declspec(naked) CL_CreateNewCommandsDetourInvoke()
{
    __asm
    {
        pushad
        call        CL_CreateNewCommandsDetour
        popad
        pop         edi
        pop         esi
        pop         ebx
        add         esp, 78h
        ret
    }
}

void CL_CreateNewCommandsDetour()
{
    Menu& menu = Menu::Instance();
    Aimbot& aimbot = Aimbot::Instance();

    GameData::EnterCriticalSection(&menu.critSection);

    GameData::usercmd_s* ncmd = &GameData::clientActive->cmds[GameData::clientActive->cmdNumber + 1 & 0x7F];
    GameData::usercmd_s* ccmd = &GameData::clientActive->cmds[GameData::clientActive->cmdNumber & 0x7F];
    GameData::usercmd_s* ocmd = &GameData::clientActive->cmds[GameData::clientActive->cmdNumber - 1 & 0x7F];

    ocmd->serverTime++;

    bool aimbotRun = false;
    bool isShooting = GameData::Key_IsDown("+attack");
    if (aimbot.enableAimbot.data.boolean)
    {
        if ((aimbot.aimKey.data.integer == 1 && isShooting)
            || (aimbot.aimKey.data.integer == 2 && GameData::Key_IsDown("+speed_throw"))
            || !aimbot.aimKey.data.integer)
        {
            aimbotRun = aimbot.ExecuteAimbot();
            if (aimbotRun)
                aimbot.SetAngles();
        }
    }

    if (aimbot.autoShoot.data.boolean && (aimbotRun || isShooting))
    {
        ccmd->button_bits &= ~1;
        ocmd->button_bits |= 1;
    }

    GameData::LeaveCriticalSection(&menu.critSection);
}

void IN_MouseEventDetour(int mstate)
{
    Menu& menu = Menu::Instance();
    GameData::EnterCriticalSection(&menu.critSection);

    if (!menu.open)
    {
        GameData::LeaveCriticalSection(&menu.critSection);
        return GameData::IN_MouseEvent(mstate);
    }

    GameData::LeaveCriticalSection(&menu.critSection);
}

void __declspec(naked) VM_NotifyDetourInvoke(GameData::scriptInstance_t inst,
    int notifyListOwnerId, int stringValue, GameData::VariableValue* top)
{
    __asm
    {
        push        ebp
        mov         ebp, esp
        push        [ebp + 10h]
        push        [ebp + 0Ch]
        push        [ebp + 8]
        push        eax
        call        VM_NotifyDetour
        pop         eax
        add         esp, 0Ch
        mov         ebp, esp
        and         esp, 0FFFFFFF8h
        push        00698676h
        ret
    }
}

void VM_NotifyDetour(GameData::scriptInstance_t inst, int notifyListOwnerId,
    int stringValue, GameData::VariableValue* top)
{
    LPCSTR notifyString = GameData::SL_ConvertToString(stringValue);
    if (!strcmp(notifyString, "spawned_player"))
        printf("born\n");
    if (!strcmp(notifyString, "weapon_fired"))
        GameData::UI_PlaySound(0, "grenade_bounce_glass");

    //DWORD notifyListId = FindVariable(inst, notifyListOwnerId, 0x15FFE);
    //if (!notifyListId)
    //    return;
    //notifyListId = FindObject(inst, notifyListId);

    //DWORD notifyNameListId = FindVariable(inst, notifyListId, stringValue);
    //if (!notifyNameListId)
    //    return;
    //notifyNameListId = FindObject(inst, notifyNameListId);

    //DWORD notifyListIndex = FindLastSibling(inst, notifyNameListId);
    //if (!notifyListIndex)
    //    return;

    //DWORD threadId = GetVariableKeyObject(inst, notifyListIndex);
    int self = GameData::Scr_GetSelf(inst, notifyListOwnerId);
}

void __declspec(naked) CG_DamageFeedbackDetourInvoke(int localClientNum,
    int yawByte, int pitchByte, int damage)
{
    __asm
    {
        push        ecx
        push        ebp
        mov         ebp, esp
        push        [ebp + 0Ch]; damage
        push        eax; pitchByte
        push        edx; yawByte
        push        [ebp + 8]
        call        CG_DamageFeedbackDetour
        add         esp, 10h
        pop         ebp
        pop         ecx
        test        al, al
        jnz         CONTINUE_FLOW
        ret
        CONTINUE_FLOW :
        sub         esp, 28h
        mov         ecx, dword ptr[8F435Ch]
        push        00455379h
        ret
    }
}

bool CG_DamageFeedbackDetour(int localClientNum, int yawByte, int pitchByte,
    int damage)
{
    Menu& menu = Menu::Instance();
    GameData::EnterCriticalSection(&menu.critSection);

    bool result = !Menu::Instance().GetOptionData(MISC_MENU, "No Flinch").data.boolean;
    
    GameData::LeaveCriticalSection(&menu.critSection);

    return result;
}

int Com_PrintfDetour(int channel, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);

    char printBuffer[1024];
    vsnprintf(printBuffer, 1024, format, ap);
    printf("%s\n", printBuffer);

    va_end(ap);
    return 0;
}