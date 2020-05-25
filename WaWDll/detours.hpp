#pragma once

#include "structures.hpp"
#include "aimbot.hpp"

namespace GameData
{
    enum
    {
        UI_Refresh_a                        = 0x5B6810,
        Menu_HandleMouseMove_a              = 0x5C9B10,
        CL_WritePacket_a                    = 0x63EA80,
        CL_KeyEvent_a                       = 0x4780F0,
        CG_PredictPlayerStateInternal_a     = 0x665C30,
        CG_DrawNightVisionOverlay_a         = 0x43D230,
        CG_Draw2DInternal_a                 = 0x4388A0,
        CL_SendCmd_a                        = 0x478D20,
        CL_CreateCmd_a                      = 0x63E936,
        CL_MouseMove_a                      = 0x63DE70,
        IN_MouseEvent_a                     = 0x5FA5F0,
        VM_Notify_a                         = 0x698670,
        CG_DamageFeedback_a                 = 0x455370,
        Com_Printf_a                        = 0x59A2C0,
    };

    extern LONG (__stdcall *TopLevelExceptionFilter)(struct _EXCEPTION_POINTERS *ExceptionInfo);
    void TopLevelExceptionFilterDetour(struct _EXCEPTION_POINTERS *ExceptionInfo);
    
    extern void (__cdecl *CL_WritePacket)();
    void CL_WritePacketDetour();

    extern void (__cdecl *CL_SendCmd)();
    void CL_SendCmdDetour();
    
    extern void (__fastcall *CG_DrawNightVisionOverlay)(int localClientNum);
    void CG_DrawNightVisionOverlayDetour(int localClientNum);
    
    extern int (__cdecl *Menu_HandleMouseMove)(ScreenPlacement *scrPlace, void *menu);
    int Menu_HandleMouseMoveDetour(GameData::ScreenPlacement *scrPlace, void *item);

    extern void (__cdecl *CG_Draw2DInternal)();
    void CG_Draw2DInternalDetour();
    
    extern void (__cdecl *UI_Refresh)(int localClientNum);
    void UI_RefreshDetour(int localClientNum);
    
    extern void (__cdecl *CL_KeyEvent)(int localClientNum, int value,
        int down, unsigned int time);
    void CL_KeyEventDetour(int localClientNum, int key, int down, int time);
    
    extern void __usercall *Cbuf_AddTextHook;
    void Cbuf_AddTextDetourInvoke(const char *text, int localClientNum);    
    bool Cbuf_AddTextDetour(const char *text, int localClientNum);
    
    extern void (__cdecl *CG_PredictPlayerState_Internal)(int localClientNum);
    void CG_PredictPlayerState_InternalDetour(int localClientNum);
    
    extern void (__cdecl *IN_MouseEvent)(int mstate);
    void IN_MouseEventDetour(int mstate);
    
    extern void __usercall *VM_Notify;
    void VM_NotifyDetourInvoke(scriptInstance_t inst, int notifyListOwnerId,
        int stringValue, VariableValue *top);
    void VM_NotifyDetour(scriptInstance_t inst, int notifyListOwnerId,
        int stringValue, VariableValue *top);
    
    extern void __usercall *CG_DamageFeedback;
    void CG_DamageFeedbackDetourInvoke(int localClientNum, int yawByte,
        int pitchByte, int damage);
    bool CG_DamageFeedbackDetour(int localClientNum, int yawByte, int pitchByte,
        int damage);
    
    extern int( __cdecl *Com_Printf)(int channel, const char *format, ...);
    int Com_PrintfDetour(int channel, const char *format, ...);
 }

void DetourFunction(DWORD targetFunction, DWORD detourFunction);
void DetourRemove(DWORD targetFunction, DWORD detourFunction);
void RemoveDetour(QWORD bytes);
void InsertDetour(LPVOID targetFunction, LPVOID detourFunction);
