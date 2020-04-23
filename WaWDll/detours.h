#pragma once

#include "structures.h"
#include "menu.h"
#include "aimbot.h"

namespace GameData
{
    enum DetourAddresses : DWORD
    {
        Win_GetEvent_a                      = 0x5FEC60,
        UI_Refresh_a                        = 0x5B6810,
        TopLevelExceptionFilter_a           = 0x5FF510,
        AimTarget_GetTagPos_0_a             = 0x4039C0,
        Menu_PaintAll_a                     = 0x5CA9A2,
        Menu_HandleMouseMove_a              = 0x5C9B10,
        CL_WritePacket_a                    = 0x63EA80,
        CL_KeyEvent_a                       = 0x4780F0,
        CG_PredictPlayerStateInternal_a     = 0x665C30,
        CG_DrawNightVisionOverlay_a         = 0x43D230,
        CG_Draw2DInternal_a                 = 0x4388A0,
        CL_SendCmd_a                        = 0x478D20,
        CL_CreateCmd_a                      = 0x63E936,
        CL_CreateNewCommands_a              = 0x63E994,
        CL_MouseMove_a                      = 0x63DE70,
        IN_MouseEvent_a                     = 0x5FA5F0,
        VM_Notify_a                         = 0x698670,
        CG_DamageFeedback_a                 = 0x455370,
        Com_Printf_a                        = 0x59A2C0,
    };

    extern void __usercall* Menu_PaintAll;
    extern LONG(__stdcall* TopLevelExceptionFilter)(
        struct _EXCEPTION_POINTERS* ExceptionInfo);
    extern void(*CL_SendCmd)();
    extern void(__cdecl* CL_WritePacket)();
    extern void(__fastcall* CG_DrawNightVisionOverlay)(int localClientNum);
    extern void __usercall* AimTarget_GetTagPos_0;
    extern int(__cdecl* Menu_HandleMouseMove)(ScreenPlacement* scrPlace, void* menu);
    extern void(__cdecl* CG_Draw2DInternal)();
    extern void(__cdecl* UI_Refresh)(int localClientNum);
    extern void(__cdecl* CL_KeyEvent)(int localClientNum, int value,
        int down, unsigned int time);
    extern struct sysEvent_t*(__cdecl* Win_GetEvent)(sysEvent_t* result, int unk);
    extern void __usercall* Cbuf_AddTextHook;
    extern void(__cdecl* CG_PredictPlayerState_Internal)(int localClientNum);
    extern void __usercall* CL_CreateCmd;
    extern void(__cdecl* CL_CreateNewCommands)();
    extern void(__cdecl* IN_MouseEvent)(int mstate);
    extern void __usercall* VM_Notify;
    extern void __usercall* CG_DamageFeedback;
    extern int(*Com_Printf)(int channel, const char* format, ...);
 }

void DetourFunction(DWORD targetFunction, DWORD detourFunction);
void DetourRemove(DWORD targetFunction, DWORD detourFunction);
void RemoveDetour(QWORD bytes);
void InsertDetour(LPVOID targetFunction, LPVOID detourFunction);

void Menu_PaintAllDetourInvoke(GameData::UiContext* dc);
void Menu_PaintAllDetour(GameData::UiContext* dc);
void Cmd_ExecuteSingleCommandDetour(int localClientNum, int controllerIndex,
    const char* text);
void TopLevelExceptionFilterDetour(struct _EXCEPTION_POINTERS* ExceptionInfo);
void CL_WritePacketDetour();
void CL_SendCmdDetour();
void CG_DrawNightVisionOverlayDetour(int localClientNum);
void AimTarget_GetTagPos_0DetourInvoke(GameData::centity_s* cent,
    unsigned __int16 bone, float* out);
int Menu_HandleMouseMoveDetour(GameData::ScreenPlacement* scrPlace, void* item);
void CG_Draw2DInternalDetour();
void UI_RefreshDetour(int localClientNum);
void CL_KeyEventDetour(int localClientNum, int key, int down, int time);
void Cbuf_AddTextDetourInvoke(const char* text, int localClientNum);
bool Cbuf_AddTextDetour(const char* text, int localClientNum);
void CG_PredictPlayerState_InternalDetour(int localClientNum);
void CL_CreateNewCommandsDetourInvoke();
void CL_CreateNewCommandsDetour();
void IN_MouseEventDetour(int mstate);
void VM_NotifyDetourInvoke(GameData::scriptInstance_t inst, int notifyListOwnerId,
    int stringValue, GameData::VariableValue* top);
void VM_NotifyDetour(GameData::scriptInstance_t inst, int notifyListOwnerId,
    int stringValue, GameData::VariableValue* top);
void CG_DamageFeedbackDetourInvoke(int localClientNum, int yawByte,
    int pitchByte, int damage);
bool CG_DamageFeedbackDetour(int localClientNum, int yawByte, int pitchByte,
    int damage);
int Com_PrintfDetour(int channel, const char* format, ...);

