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
void(__cdecl *CL_KeyEvent)(int localClientNum, int value, int down,
	unsigned int time) = (void(*)(int, int, int, unsigned int))CL_KeyEvent_a;
sysEvent_t*(__cdecl *Win_GetEvent)(sysEvent_t *result, int unk)
	= (sysEvent_t*(*)(sysEvent_t*, int))Win_GetEvent_a;
__usercall Cbuf_AddTextHook = (__usercall)Cbuf_AddText_a;
void(__cdecl *CG_PredictPlayerState_Internal)(int localClientNum)
	= (void(__cdecl*)(int))CG_PredictPlayerStateInternal_a;
__usercall CL_CreateCmd = (__usercall)CL_CreateCmd_a;
void(__cdecl *CL_CreateNewCommands)()
	= (void(__cdecl*)())CL_CreateNewCommands_a;
void(__cdecl *IN_MouseEvent)(int mstate) = (void(__cdecl*)(int))IN_MouseEvent_a;
__usercall VM_Notify = (__usercall)VM_Notify_a;

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
	DetourRemove((bytes & ((QWORD)UINT_MAX << 32)) >> 32,
		(bytes & UINT_MAX));
}

void InsertDetour(LPVOID targetFunction, LPVOID detourFunction)
{
	GameData::detours.push_back(
		((QWORD)targetFunction << 32) | (QWORD)detourFunction);
	DetourFunction((DWORD)targetFunction, (DWORD)detourFunction);
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
	if (!GameData::initialized)
	{
		Menu::Build();

		if (GameData::InsertDvar("cl_ingame")
			&& GameData::InsertDvar("cg_fov")
			&& GameData::InsertDvar("perk_weapSpreadMultiplier")
			&& GameData::InsertDvar("sv_cheats")
			&& GameData::InsertDvar("player_sustainAmmo")
			&& GameData::InsertDvar("cl_bypassMouseInput"))
			GameData::initialized = true;
		else
		{
			GameData::MessageBoxA(*hwnd, "Error reading dvars",
				"WaW DLL by E7ite", 0x10);
			speex_error("Error reading dvars");
		}
	}

	if (IN_IsForegroundWindow())
		Menu::MonitorKeys();

	if (Menu::open)
		Menu::Execute();

	RenderESP();
}

void R_EndFrameDetour()
{
	unsigned const char steadyAimBytes[] = { 0x83, 0xFF, 0x02, 0x75, 0x15 };

	WriteBytes(0x46A87E, Variables::noRecoil ? "\xEB" : "\x74", 1);
	WriteBytes(0x41DB2B, Variables::steadyAim ? "\x90\x90\x90\x90\x90" 
		: reinterpret_cast<const char*>(steadyAimBytes), 5);

	if (GameData::initialized)
	{
		GameData::dvars["cg_fov"]->current.value = static_cast<float>(Variables::fov);
		GameData::dvars["sv_cheats"]->current.enabled = Variables::cheatsEnabled;
		GameData::dvars["player_sustainAmmo"]->current.enabled = Variables::infAmmo;
	}

	R_EndFrame();
}

void Cmd_ExecuteSingleCommandDetour(int localClientNum, int controllerIndex, const char *text)
{
	return Cmd_ExecuteSingleCommand(localClientNum, controllerIndex, text);
}

void TopLevelExceptionFilterDetour(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	PDWORD_PTR currESP 
		= reinterpret_cast<PDWORD_PTR>(ExceptionInfo->ContextRecord->Esp);

	printf("EAX: 0x%x\n", ExceptionInfo->ContextRecord->Eax);
	printf("EBX: 0x%x\n", ExceptionInfo->ContextRecord->Ebx);
	printf("ECX: 0x%x\n", ExceptionInfo->ContextRecord->Ecx);
	printf("EDX: 0x%x\n", ExceptionInfo->ContextRecord->Edx);
	printf("EDI: 0x%x\n", ExceptionInfo->ContextRecord->Edi);
	printf("ESI: 0x%x\n", ExceptionInfo->ContextRecord->Esi);
	printf("EBP: 0x%x\n", ExceptionInfo->ContextRecord->Ebp);
	printf("EIP: 0x%x\n", ExceptionInfo->ContextRecord->Eip);

	printf("\nSTACK VIEW:\n");
	for (int i = 0; i < 8; i++)
	{
		if (i)
			++currESP;

		printf("0x%x: %8x ", (unsigned int)currESP, *currESP);
		for (int j = 0; j < 3; j++)
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
		&& (Variables::aimKey != 1 || !strcmp(keys[key].binding, "+attack"))
		&& Variables::autoShoot)
		return;

	return CL_KeyEvent(localClientNum, key, down, time);
}

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
        push        594208h
		ret
LABEL_1:
		pop			edx
		jmp			edx
	}
}

bool Cbuf_AddTextDetour(const char *text, int localClientNum)
{
	return true;
}

void CG_PredictPlayerState_InternalDetour(int localClientNum)
{
	CG_PredictPlayerState_Internal(localClientNum);
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

	ocmd->serverTime++;

	bool aimbotRun = false;
	bool isShooting = Key_IsDown("+attack");
	if (Variables::enableAimbot)
	{
		if ((Variables::aimKey == 1 && isShooting)
			|| (Variables::aimKey == 2 && Key_IsDown("+speed_throw"))
			|| !Variables::aimKey)
		{
			aimbotRun = ExecuteAimbot();
			if (aimbotRun)
				SetAngles(Aimbot::targetAngles);
		}
	}

	if (Variables::autoShoot && (aimbotRun || isShooting))
	{
		ccmd->button_bits &= ~1;
		ocmd->button_bits |= 1;
	}
}

void IN_MouseEventDetour(int mstate)
{
	if (!Menu::open)
		return IN_MouseEvent(mstate);
}

void __declspec(naked) VM_NotifyStub(int inst, int notifyListOwnerId, 
	int stringValue, VariableValue *top)
{
	__asm
	{
        push        ebp
        mov         ebp, esp
		push        [ebp + 10h]
		push        [ebp + 0Ch]
		push        [ebp + 8]
		push		eax
		call		VM_NotifyDetour
		pop			eax
		add			esp, 0Ch
		mov			ebp, esp
		and			esp, 0FFFFFFF8h
		push		00698676h
		ret
	}
}

void VM_NotifyDetour(int inst, int notifyListOwnerId, int stringValue,
	VariableValue *top)
{
    DWORD notifyListId = FindVariable(inst, notifyListOwnerId, 0x15FFE);
    LPCSTR notifyString = SL_ConvertToString(stringValue);



    if (!strcmp(notifyString, "spawned_player"))
        printf("born\n");
	if (!strcmp(notifyString, "weapon_fired"))
		printf("shooting weapon\n");

    printf("Scr_GetSelf: %x\n", Scr_GetSelf(inst, notifyListOwnerId));
}
