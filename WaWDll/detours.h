#pragma once

#include "menu.h"
#include "aimbot.h"
#include "esp.h"

using __usercall = void*;

void DetourFunction(DWORD targetFunction, DWORD detourFunction);
void DetourRemove(DWORD targetFunction, DWORD detourFunction);
void RemoveDetour(QWORD bytes);
void InsertDetour(QWORD targetFunction, QWORD detourFunction);

enum DetourAddresses : DWORD
{
	Win_GetEvent_a						= 0x5FEC60,
	UI_Refresh_a						= 0x5B6810,
	TopLevelExceptionFilter_a			= 0x5FF510,
	AimTarget_GetTagPos_0_a				= 0x4039C0,
	Menu_PaintAll_a						= 0x5CA9A2,
	Menu_HandleMouseMove_a				= 0x5C9B10,
	CL_WritePacket_a					= 0x63EA80,
	CL_KeyEvent_a						= 0x4780F0,
	CG_PredictPlayerStateInternal_a		= 0x665C30,
	CG_DrawNightVisionOverlay_a			= 0x43D230,
	CG_Draw2DInternal_a					= 0x4388A0,
	Cbuf_AddText_a						= 0x594200,
	Cbuf_AddTextRet_a					= 0x594208,
	CL_SendCmd_a						= 0x478D20,
	CL_CreateCmd_a						= 0x63E936,
};

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
extern __usercall CL_CreateCmd;

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
void CL_CreateCmdStub(int localClientNum, usercmd_s *cmd);
void CL_CreateCmdDetour(int localClientNum, usercmd_s *cmd);