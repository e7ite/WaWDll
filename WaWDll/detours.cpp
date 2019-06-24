#include "detours.h"

__usercall Menu_PaintAll = (__usercall)Menu_PaintAll_a;
void(__cdecl *R_EndFrame)() = (void(*)())R_EndFrame_a;
void(*CL_SendCmd)() = (void(*)())CL_SendCmd_a;
LONG(__stdcall *TopLevelExceptionFilter)(
	struct _EXCEPTION_POINTERS *ExceptionInfo)
	= (LONG(__stdcall*)(_EXCEPTION_POINTERS*))TopLevelExceptionFilter_a;
void(__cdecl *CL_WritePacket)() = (void(__cdecl*)())CL_WritePacket_a;
void(__fastcall *CG_DrawNightVisionOverlay)(int localClientNum)
	= (void(__fastcall*)(int))CG_DrawNightVisionOverlay_a;
__usercall AimTarget_GetTagPos_0 = (__usercall)AimTarget_GetTagPos_0_a;
int(__cdecl *Menu_HandleMouseMove)(ScreenPlacement *scrPlace, void *menu)
	= (int(__cdecl*)(ScreenPlacement*, void*))Menu_HandleMouseMove_a;
void(__cdecl *CG_Draw2DInternal)() = (void(__cdecl*)())CG_Draw2DInternal_a;
void(__cdecl *UI_Refresh)(int localClientNum) = (void(__cdecl*)(int))UI_Refresh_a;
void(__cdecl *CL_KeyEvent)(__int32 localClientNum, __int32 value, __int32 down,
	unsigned __int32 time)
	= (void(*)(__int32, __int32, __int32, unsigned __int32))CL_KeyEvent_a;
sysEvent_t*(__cdecl *Win_GetEvent)(sysEvent_t *result, __int32 unk)
	= (sysEvent_t*(*)(sysEvent_t*, __int32))Win_GetEvent_a;
__usercall Cbuf_AddText = (__usercall)Cbuf_AddText_a;
void(__cdecl *CG_PredictPlayerState_Internal)(int localClientNum)
	= (void(__cdecl*)(int))CG_PredictPlayerStateInternal_a;
__usercall CL_CreateCmd = (__usercall)CL_CreateCmd_a;
void(__thiscall *CL_CreateNewCommands)()
	= (void(__thiscall*)())CL_CreateNewCommands_a;

void DetourFunction(DWORD targetFunction, DWORD detourFunction)
{
	/*Initiate Detour Transcation API*/
	DetourTransactionBegin();

	/*Enlists Current Thread in Transaction to Appropriately Update
	  Instruction Pointers for That Thread*/
	DetourUpdateThread(GetCurrentThread()),

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
	DetourUpdateThread(GetCurrentThread()),

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
	DetourRemove((bytes & ((QWORD)UINT_MAX << 32)) >> 32,
		(bytes & UINT_MAX));
}

void InsertDetour(QWORD targetFunction, QWORD detourFunction)
{
	GameData::detours.push_back((targetFunction << 32) | detourFunction);
	DetourFunction(static_cast<DWORD>(targetFunction),
		static_cast<DWORD>(detourFunction));
}

void __declspec(naked) Menu_PaintAllStub(UiContext *dc)
{
	__asm
	{
		push		esi
		call		Menu_PaintAllDetour
		add			esp, 4
		pop			edi
		pop			ebp
		pop			ebx
		add			esp, 410h
		ret
	}
}

void Menu_PaintAllDetour(UiContext *dc)
{
	ExecuteMenu();
	RenderESP();
}

void R_EndFrameDetour()
{
	WriteBytes(0x46A87E, Variables::noRecoil ? "\xEB" : "\x74", 1);
	GameData::dvarGlob["cg_fov"]->current.value = (float)Variables::fov;

	R_EndFrame();
}

void Cmd_ExecuteSingleCommandDetour(int localClientNum, int controllerIndex, const char *text)
{
	return Cmd_ExecuteSingleCommand(localClientNum, controllerIndex, text);
}

void TopLevelExceptionFilterDetour(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	PDWORD_PTR currESP = (PDWORD_PTR)ExceptionInfo->ContextRecord->Esp;

	printf("EAX: 0x%x\n", ExceptionInfo->ContextRecord->Eax);
	printf("EBX: 0x%x\n", ExceptionInfo->ContextRecord->Ebx);
	printf("ECX: 0x%x\n", ExceptionInfo->ContextRecord->Ecx);
	printf("EDX: 0x%x\n", ExceptionInfo->ContextRecord->Edx);
	printf("EDI: 0x%x\n", ExceptionInfo->ContextRecord->Edi);
	printf("ESI: 0x%x\n", ExceptionInfo->ContextRecord->Esi);
	printf("EBP: 0x%x\n", ExceptionInfo->ContextRecord->Ebp);
	printf("EIP: 0x%x\n", ExceptionInfo->ContextRecord->Eip);

	printf("\nSTACK VIEW:\n");
	for (int i = 0; i < 8; ++i)
	{
		if (i)
			++currESP;

		printf("0x%x: %8x ", (unsigned int)currESP, *currESP);
		for (int j = 0; j < 3; ++j)
			printf("%8x ", *(++currESP));
		printf("\n");
	}
}

void CL_WritePacketDetour()
{

	CL_WritePacket();
}

void CL_SendCmdDetour()
{
	if (!InGame())
		return CL_SendCmd();

	if (Key_IsDown("+attack"));
	//{
	//	ocmd->button_bits &= ~1;
	//	ocmd->button_bits |= 1;
	//}

	CL_SendCmd();
}

void CG_DrawNightVisionOverlayDetour(int localClientNum)
{
	CG_DrawNightVisionOverlay(localClientNum);
}

void __declspec(naked) AimTarget_GetTagPos_0Stub(centity_s *cent,
	unsigned __int16 bone, float *out)
{
	__asm
	{
		mov			eax, [esp + 4]
		push		eax
		push		esi
		push		ecx
		push		0
		call		AimTarget_GetTagPos
		add			esp, 10h
		ret
	}
}

int Menu_HandleMouseMoveDetour(ScreenPlacement *scrPlace, void *item)
{
	if (!Menu::open)
		return Menu_HandleMouseMove(scrPlace, item);

	return 0;
}

void CG_Draw2DInternalDetour()
{
	return CG_Draw2DInternal();
}

void UI_RefreshDetour(int localClientNum)
{
	return UI_Refresh(localClientNum);
}

void CL_KeyEventDetour(int localClientNum, int key, int down, int time)
{
	if (InGame() && keys[key].binding 
		&& !*(int*)0x208E938
		&& !strcmp(keys[key].binding, "+attack") && Variables::autoShoot)
		return;

	return CL_KeyEvent(localClientNum, key, down, time);
}

DWORD Cbuf_AddTextRet = Cbuf_AddTextRet_a;
void __declspec(naked) Cbuf_AddTextStub(const char *text, int localClientNum)
{
	__asm
	{
		push		eax
		push		ecx
		push		ecx
		push		eax
		call		Cbuf_AddTextDetour
		add			esp, 8h
		cmp			al, 0
		pop			ecx
		pop			eax
		jz			LABEL_1
		push		ebp
		push		esi
		push		edi
		push		22990F8h
		jmp			Cbuf_AddTextRet
LABEL_1:
		pop			edx
		jmp			edx
	}
}

bool Cbuf_AddTextDetour(const char *text, int localClientNum)
{
	if (InGame()
		&& strstr(text, "attack"))
		return false;

	return true;
}

void CG_PredictPlayerState_InternalDetour(int localClientNum)
{
	CG_PredictPlayerState_Internal(localClientNum);

	return ;
}

void __declspec(naked) CL_CreateNewCommandsStub()
{
	__asm
	{
		pushad
		call		CL_CreateNewCommandsDetour
		popad
		pop			edi
		pop			esi
		pop			ebx
		add			esp, 78h
		ret
	}
}

void CL_CreateNewCommandsDetour()
{
	usercmd_s *ncmd = &clientActive->cmds[clientActive->cmdNumber + 1 & 0x7F],
		*ccmd = &clientActive->cmds[clientActive->cmdNumber & 0x7F],
		*ocmd = &clientActive->cmds[clientActive->cmdNumber - 1 & 0x7F];

	//memcpy(ocmd, ccmd, sizeof usercmd_s);
	ocmd->serverTime++;

	if (Variables::enableAimbot)
		if (Key_IsDown("+attack"))
			if (ExecuteAimbot())
			{
				//SetAngles(Aimbot::targetAngles);
				ccmd->angles[0] = AngleToShort(Aimbot::targetAngles[0]);
				ccmd->angles[1] = AngleToShort(Aimbot::targetAngles[1]);
				RemoveSpread(&cgameGlob->predictedPlayerState, ocmd);
			}

	if (Key_IsDown("+attack"))
	{
		ccmd->button_bits &= ~1;
		ocmd->button_bits |= 1;
	}
}