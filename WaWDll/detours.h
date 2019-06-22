#pragma once

#include "menu.h"
#include "aimbot.h"
#include "esp.h"

using __usercall = void*;

void DetourFunction(DWORD targetFunction, DWORD detourFunction);
void DetourRemove(DWORD targetFunction, DWORD detourFunction);
void RemoveDetour(QWORD bytes);
void InsertDetour(QWORD targetFunction, QWORD detourFunction);

extern __usercall Menu_PaintAll;
extern void(__cdecl *R_EndFrame)();
extern LONG(__stdcall *TopLevelExceptionFilter)
	(struct _EXCEPTION_POINTERS *ExceptionInfo);
extern void(*CL_SendCmd)();
extern void(__cdecl *CL_WritePacket)();
extern void(__fastcall *CG_DrawNightVisionOverlay)(int localClientNum);
extern __usercall AimTarget_GetTagPos_0;
extern int(__cdecl *Menu_HandleMouseMove)
	(ScreenPlacement *scrPlace, void *menu);
extern void(__cdecl *CG_Draw2DInternal)();
extern void(__cdecl *UI_Refresh)(int localClientNum);
extern void(__cdecl *CL_KeyEvent)(__int32 localClientNum, __int32 value,
	__int32 down, unsigned __int32 time);
extern struct sysEvent_t*(__cdecl *Win_GetEvent)(sysEvent_t *result, __int32 unk);
extern __usercall Cbuf_AddText;
extern void(__cdecl *CG_PredictPlayerState_Internal)(int localClientNum);

void Menu_PaintAllStub(UiContext *dc);
void Menu_PaintAllDetour(UiContext *dc);
void R_EndFrameDetour();
void Cmd_ExecuteSingleCommandDetour(int localClientNum, int controllerIndex, const char *text);
void TopLevelExceptionFilterDetour(struct _EXCEPTION_POINTERS *ExceptionInfo);
void CL_WritePacketDetour();
void CL_SendCmdDetour();
void CG_DrawNightVisionOverlayDetour(int localClientNum);
void AimTarget_GetTagPos_0Stub(centity_s *cent,
	unsigned __int16 bone, float *out);
int Menu_HandleMouseMoveDetour(ScreenPlacement *scrPlace, void *item);
void CG_Draw2DInternalDetour();
void UI_RefreshDetour(int localClientNum);
void CL_KeyEventDetour(int localClientNum, int key, int down, int time);
void Cbuf_AddTextStub(const char *text, int localClientNum);
bool Cbuf_AddTextDetour(const char *text, int localClientNum);
void CG_PredictPlayerState_InternalDetour(int localClientNum);