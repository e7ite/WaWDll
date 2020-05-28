#include "stdafx.hpp"
#include "detours.hpp"
#include "esp.hpp"

namespace GameData
{
    void (__cdecl *CL_SendCmd)() = (void (*)())CL_SendCmd_a;
    void CL_SendCmdDetour()
    {
        GameData::CL_SendCmd();
    }

    void (__cdecl *CL_WritePacket)() = (void (__cdecl *)())CL_WritePacket_a;
    void CL_WritePacketDetour()
    {
        GameData::CL_WritePacket();
    }

    void (__fastcall *CG_DrawNightVisionOverlay)(int localClientNum)
        = (void (__fastcall*)(int))CG_DrawNightVisionOverlay_a;
    void CG_DrawNightVisionOverlayDetour(int localClientNum)
    {
        GameData::CG_DrawNightVisionOverlay(localClientNum);
    }

    int (__cdecl *Menu_HandleMouseMove)(ScreenPlacement *scrPlace, void *menu)
        = (int (__cdecl *)(ScreenPlacement *, void *))Menu_HandleMouseMove_a;
    int Menu_HandleMouseMoveDetour(ScreenPlacement *scrPlace, void *item)
    {
        Menu &menu = Menu::Instance();
        GameData::EnterCriticalSection(&menu.critSection);

        if (!menu.open)
        { 
            GameData::LeaveCriticalSection(&menu.critSection);
            return GameData::Menu_HandleMouseMove(scrPlace, item);
        }
    
        GameData::LeaveCriticalSection(&menu.critSection);
        return 0;
    }

    void (__cdecl *CG_Draw2DInternal)() = (void (__cdecl *)())CG_Draw2DInternal_a;
    void CG_Draw2DInternalDetour()
    {
        return GameData::CG_Draw2DInternal();
    }

    void (__cdecl *UI_Refresh)(int localClientNum) = (void (__cdecl *)(int))UI_Refresh_a;
    void UI_RefreshDetour(int localClientNum)
    {
        return GameData::UI_Refresh(localClientNum);
    }

    void (__cdecl *CL_KeyEvent)(int localClientNum, int value, int down,
        unsigned int time) = (void(*)(int, int, int, unsigned int))CL_KeyEvent_a;
    void CL_KeyEventDetour(int localClientNum, int key, int down, int time)
    {
        Menu &menu = Menu::Instance();
        GameData::EnterCriticalSection(&menu.critSection);

        OptionData &aimKey = menu.GetOptionData(AIMBOT_MENU, "Aim Key");
        OptionData &autoShoot = menu.GetOptionData(AIMBOT_MENU, "Auto Shoot");

        if (InGame() && GameData::keys[key].binding
            && !*(int *)0x208E938
            && (aimKey.data.integer != 1 || !strcmp(GameData::keys[key].binding, "+attack"))
            && autoShoot.data.boolean)
            return;

        GameData::LeaveCriticalSection(&menu.critSection);
        return GameData::CL_KeyEvent(localClientNum, key, down, time);
    }

    void __usercall *Cbuf_AddTextHook = (void __usercall *)Cbuf_AddText_a;
    void __declspec(naked) Cbuf_AddTextDetourInvoke(const char *text,
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
    bool Cbuf_AddTextDetour(const char *text, int localClientNum)
    {
        return true;
    }

    void (__cdecl *CG_PredictPlayerState_Internal)(int localClientNum)
        = (void (__cdecl *)(int))CG_PredictPlayerStateInternal_a;
    void CG_PredictPlayerState_InternalDetour(int localClientNum)
    {
        GameData::CG_PredictPlayerState_Internal(localClientNum);
}

    void (__cdecl *IN_MouseEvent)(int mstate) = (void (__cdecl *)(int))IN_MouseEvent_a;
    void IN_MouseEventDetour(int mstate)
    {
        Menu &menu = Menu::Instance();
        GameData::EnterCriticalSection(&menu.critSection);

        if (!menu.open)
        {
            GameData::LeaveCriticalSection(&menu.critSection);
            return GameData::IN_MouseEvent(mstate);
        }

        GameData::LeaveCriticalSection(&menu.critSection);
    }

    void __usercall *VM_Notify = (void __usercall *)VM_Notify_a;
    void __declspec(naked) VM_NotifyDetourInvoke(scriptInstance_t inst,
        int notifyListOwnerId, int stringValue, VariableValue *top)
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
    void VM_NotifyDetour(scriptInstance_t inst, int notifyListOwnerId,
        int stringValue, VariableValue *top)
    {
        LPCSTR notifyString = SL_ConvertToString(stringValue);
        if (!strcmp(notifyString, "spawned_player"))
            std::cout << "born" << std::endl;
        if (!strcmp(notifyString, "weapon_fired"))
            UI_PlaySound(0, "grenade_bounce_glass");

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
        int self = Scr_GetSelf(inst, notifyListOwnerId);
    }

    void __usercall *CG_DamageFeedback = (void __usercall *)CG_DamageFeedback_a;
    void __declspec(naked) CG_DamageFeedbackDetourInvoke(int localClientNum,
        int yawByte, int pitchByte, int damage)
    {
        __asm
        {
            push        ecx
            push        ebp
            mov         ebp, esp
            push        [ebp + 0Ch] ; damage
            push        eax         ; pitchByte
            push        edx         ; yawByte
            push        [ebp + 8]
            call        CG_DamageFeedbackDetour
            add         esp, 10h
            pop         ebp
            pop         ecx
            test        al, al
            jnz         CONTINUE_FLOW
            ret
    CONTINUE_FLOW:
            sub         esp, 28h
            mov         ecx, dword ptr[8F435Ch]
            push        00455379h
            ret
        }
    }
    bool CG_DamageFeedbackDetour(int localClientNum, int yawByte, int pitchByte,
        int damage)
    {
        Menu &menu = Menu::Instance();
        GameData::EnterCriticalSection(&menu.critSection);

        bool result = !Menu::Instance().GetOptionData(MISC_MENU, "No Flinch").data.boolean;
    
        GameData::LeaveCriticalSection(&menu.critSection);
        return result;
}

    int (__cdecl *Com_Printf)(int channel, const char *format, ...)
        = (int (__cdecl *)(int, const char *, ...))Com_Printf_a;
    int Com_PrintfDetour(int channel, const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);

        char printBuffer[1024];
        vsnprintf(printBuffer, 1024, format, ap);
        printf("%s\n", printBuffer);

        va_end(ap);
        return 0;
}
}

void DetourFunction(DWORD targetFunction, DWORD detourFunction)
{
    // Initiate Detour Transcation API
    DetourTransactionBegin();

    // Enlists Current Thread in Transaction to Appropriately Update
    // Instruction Pointers for That Thread
    DetourUpdateThread(GetCurrentThread());

    // Allocates the Detour for the Target Function
    DetourAttach(reinterpret_cast<PVOID *>(targetFunction),
        reinterpret_cast<PVOID *>(detourFunction));

    // Overwrites the first instruction in the target function to jmp
    // to Detour before returning to target function to restore program flow
    DetourTransactionCommit();
}

void DetourRemove(DWORD targetFunction, DWORD detourFunction)
{
    // Initiate Detour Transcation API 
    DetourTransactionBegin();

    // Enlists Current Thread in Transaction to Appropriately Update
    // Instruction Pointers for That Thread
    DetourUpdateThread(GetCurrentThread());

    // Deallocates the Detour for the Target Function
    DetourDetach(reinterpret_cast<PVOID *>(targetFunction),
        reinterpret_cast<PVOID *>(detourFunction));

    // Restores overwritten instructions of Target Function
    // and restores Target Function Pointer to point to original
    // function
    DetourTransactionCommit();
}

void RemoveDetour(QWORD bytes)
{
    DetourRemove((bytes & ((QWORD)UINT_MAX << 32)) >> 32,
        (bytes & UINT_MAX));
}

void InsertDetour(LPVOID targetFunction, LPVOID detourFunction)
{
    detours.push_back(((QWORD)targetFunction << 32) | (QWORD)detourFunction);
    DetourFunction((DWORD)targetFunction, (DWORD)detourFunction);
}