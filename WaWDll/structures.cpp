#include "structures.h"

UiContext *dc = (UiContext*)0x208E920;
ScreenPlacement *scrPlace = (ScreenPlacement*)0x957360;
KeyState *keys = (KeyState*)0x951C44;
centity_s *cg_entitiesArray = (centity_s*)0x35D39F0;
cg_s *cgameGlob = (cg_s*)0x34732B8;
clientActive_t *clientActive = (clientActive_t*)0x3058528;
WORD *clientObjMap = (WORD*)0x1FE58C8;
BYTE *objBuf = (BYTE*)0x1F978C8;
WeaponDef **bg_weaponVariantDefs = (WeaponDef**)0x8F6770;
cgs_t *cgs = (cgs_t*)0x3466578;
actor_s *actors = (actor_s*)0x176C874;

void*(__cdecl *R_RegisterFont)(const char *font, __int32 imageTrac)
	= (void*(*)(const char*, __int32))R_RegisterFont_a;
void*(__cdecl *Material_RegisterHandle)(const char *materialName, 
	__int32 imageTrac)
	= (void*(*)(const char*, __int32))Material_RegisterHandle_a;
void(__cdecl *CG_DrawRotatedPic)(ScreenPlacement *scrPlace, float x, float y,
	float width, float height, float angle, const float *color, void *material)
	= (void(*)(ScreenPlacement*, float, float,
	float, float, float, const float*, void*))CG_DrawRotatedPic_a;
void(__cdecl *Cmd_ExecuteSingleCommand)(__int32 localClientNum,
	__int32 controllerIndex, const char *text)
	= (void(*)(__int32, __int32, const char*))Cmd_ExecuteSingleCommmand_a;
dvar_s*(__cdecl *Dvar_FindVar)(const char *dvarName)
	= (dvar_s*(*)(const char*))Dvar_FindVar_a;
void(__cdecl *CL_DrawTextPhysicalInternal)(const char *text, int maxChars,
	void *font, float x, float y, float xScale, float yScale, float rotation,
	int style) = (void(__cdecl*)(const char*, int,
	void*, float, float, float, float, float, int))CL_DrawTextPhysical_a;
__int32(__cdecl *UI_TextWidthInternal)(const char *text, __int32 maxChars,
	void *font, float scale)
	= (__int32(*)(const char*, __int32, void*, float))UI_TextWidth_a;
void(__cdecl *CG_GameMessage)(int localClientNum, const char *msg, int length)
	= (void(__cdecl*)(int, const char*, int))CG_GameMessage_a;
int(__cdecl *CG_GetPlayerWeapon)(playerState_s *ps, int localClientNum)
	= (int(__cdecl*)(playerState_s*, int))CG_GetPlayerWeapon_a;

vec3_t vec3_t::operator+(const vec3_t &vec) const
{
	return vec3_t(this->x + vec.x, this->y + vec.y, this->z + vec.z);
}

vec3_t vec3_t::operator+(float vec[3]) const
{
	return vec3_t(this->x + vec[0], this->y + vec[1], this->z + vec[2]);
}

vec3_t vec3_t::operator-(const vec3_t &vec) const
{
	return vec3_t(this->x - vec.x, this->y - vec.y, this->z - vec.z);
}

vec3_t vec3_t::operator-(float vec[3]) const
{
	return vec3_t(this->x - vec[0], this->y - vec[1], this->z - vec[2]);
}

vec3_t& vec3_t::operator+=(const vec3_t &vec)
{
	this->x += vec.x;
	this->y += vec.y;
	this->z += vec.z;

	return *this;
}

vec3_t& vec3_t::operator+=(float vec[3])
{
	this->x += vec[0];
	this->y += vec[1];
	this->z += vec[2];

	return *this;
}

vec3_t& vec3_t::operator-=(const vec3_t &vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	this->z -= vec.z;

	return *this;
}

vec3_t& vec3_t::operator-=(float vec[3])
{
	this->x -= vec[0];
	this->y -= vec[1];
	this->z -= vec[2];

	return *this;
}

void CG_DrawRotatedPicPhysical(ScreenPlacement *scrPlace, float x, float y,
	float width, float height, float angle, const float *color, void *material)
{
	DWORD addr = CG_DrawRotatedPicPhysical_a;
	__asm
	{
		push		material
		push		color
		sub			esp, 10h
		fld			x
		fstp		[esp]
		fld			y
		fstp		[esp + 4]
		fld			width
		fstp		[esp + 8]
		fld			height
		fstp		[esp + 12]
		movss		xmm0, angle
		mov			edx, scrPlace
		call		addr
		add			esp, 18h
	}
}

__int32 Key_StringToKeynum(const char *name)
{
	DWORD addr = Key_StringToKeynum_a;
	DWORD funcRet;

	__asm
	{
		mov			edi, name
		call		addr
		mov			funcRet, eax
	}

	return funcRet;
}

bool Key_IsDown(const char *bind)
{
	for (__int16 i = 0; i < 256; ++i)
		if (keys[i].binding)
			if (!strcmp(keys[i].binding, bind))
				if (keys[i].down)
					return true;

	return false;
}

float R_NormalizedTextScale(Font_s *font, float scale)
{
	return scale * 48.0f / (float)font->pixelHeight;
}

float UI_TextHeight(Font_s *font, float scale)
{
	return static_cast<float>(floor(font->pixelHeight 
		* R_NormalizedTextScale(font, scale) + 0.5));
}

float R_TextHeight(Font_s *font)
{
	return static_cast<float>(font->pixelHeight);
}

__int32 AimTarget_GetTagPos(__int32 localClientNum, centity_s *cent,
	unsigned __int16 tagname, float *pos)
{
	DWORD CG_DObjGetWorldTagPos = CG_DObjGetWorldTagPos_a;
	bool funcRet;

	WORD handle = clientObjMap[cent->nextState.number + 0x880 * localClientNum];
	if (!handle)
		return 0;

	DWORD dobj = (DWORD)objBuf + 0x68 * handle;
	if (!dobj)
		return 0;
	
	__asm
	{
		mov			edi, dobj
		movzx		ecx, tagname
		push		pos
		push		cent
		call		CG_DObjGetWorldTagPos
		add			esp, 8
		mov			funcRet, al
	}

	return funcRet;
}

unsigned __int16 SL_FindString(const char *tagname)
{
	unsigned __int32 len = strlen(tagname) + 1;
	DWORD addr = SL_FindString_a;

	__asm
	{
		mov			eax, 0
		push		len
		push		tagname
		call		addr
		add			esp, 8
	}
}

bool WorldPosToScreenPos(int localClientNum, const float *world, float pos[2])
{
	DWORD addr = WorldPosToScreenPos_a;
	bool funcRet;

	__asm
	{
		mov			ecx, localClientNum
		mov			eax, world
		mov			esi, pos
		call		addr
		mov			funcRet, al
	}

	return funcRet;
}

void vectoangles(const float *vec, float *pos)
{
	DWORD addr = vectoangles_a;
	__asm
	{
		mov			edi, pos
		mov			esi, vec
		call		addr
	}
}

bool AimTarget_IsTargetVisible(int targetEntNum, unsigned __int16 bone)
{
	vec3_t target;
	trace_t results;
	float fraction;
	DWORD mask	                  = 0x280F001;
	float *vec3_origin			  = (float*)0x816F68;
	float *playerEyePos			  = (float*)&cgameGlob->refdef.vieworg;
	int passEntNum				  = cgameGlob->predictedPlayerState.clientNum;
	DWORD CG_TraceCapsule		  = CG_TraceCapsule_a;
	DWORD FX_ClientVisibilityTest = FX_ClientVisibilityTest_a;

	if (!AimTarget_GetTagPos(0, &cg_entitiesArray[targetEntNum], bone, target))
		return false;

	__asm
	{
		mov			eax, vec3_origin
		lea			edi, results
		push		0
		push		0
		push		0
		push		0
		push		0
		push		mask
		push		passEntNum
		lea			ebx, target
		push		ebx
		mov			ebx, playerEyePos
		push		ebx
		push		vec3_origin
		call		CG_TraceCapsule
		add			esp, 28h
		lea			edi, playerEyePos
		push		edi
		lea			ecx, results
		call		FX_ClientVisibilityTest
		add			esp, 4
		movss		fraction, xmm0
	}

	return fraction > 0.000099999997;
}

void CL_GetUserCmd(int cmdNum, usercmd_s *cmd)
{
	__asm
	{
		mov			esi, cmdNum
		and			esi, 7Fh
		lea			eax, ds:0[esi * 8]
		sub			eax, esi
		lea			esi, ds:30FD700h[eax * 8]	;source
		mov			edi, cmd					;destination
		mov			ecx, 0Eh					;amount of dwords to copy
		rep	movsd								;move dwords esi to edi,
												;ecx times
	}
}

usercmd_s* CL_GetUserCmd(int cmdNum)
{
	DWORD funcRet;

	__asm
	{
		mov			esi, cmdNum
		and			esi, 7Fh
		lea			eax, ds:0[esi * 8]
		sub			eax, esi
		lea			esi, ds:30FD700h[eax * 8]
		mov			funcRet, esi
	}

	return (usercmd_s*)funcRet;
}

void CL_DrawTextPhysical(const char *text, int maxChars,
	void *font, float x, float y, float xScale, float yScale,
	float rotation, const float *color, int style)
{
	__asm
	{
		push		style 
		sub			esp, 14h
		fld			rotation
		fstp		[esp + 10h]
		fld			yScale
		fstp		[esp + 0Ch]
		fld			xScale
		fstp		[esp + 08h]
		fld			y
		fstp		[esp + 04h]
		fld			x
		fstp		[esp]
		push		font
		push		maxChars
		push		text
		mov			ecx, color
		call		CL_DrawTextPhysicalInternal
		add			esp, 24h
	}
}

void ScrPlace_ApplyRect(ScreenPlacement *scrPlace, 
	float *x, float *y, float *w, float *h, int horzAlign, int vertAlign)
{
	DWORD addr = ScrPlace_ApplyRect_a;
	__asm
	{
		mov			edx, x
		mov			ecx, w
		mov			edi, y
		mov			esi, h
		push		vertAlign
		push		horzAlign
		push		scrPlace
		call		addr
		add			esp, 0Ch
	}
}

void UI_DrawText(ScreenPlacement *scrPlace, const char *text,
	__int32 maxChars, void *font, float x, float y, float scale, 
	float angle, const float *color, __int32 style)
{
	DWORD addr = UI_DrawText_a;
	__asm
	{
		mov			eax, 0
		mov			ecx, 0
		push		style
		push		color
		push		scale
		push		y
		push		x
		push		font
		push		maxChars
		push		text
		push		scrPlace
		call		addr
		add			esp, 24h
	}
}

Font_s* UI_GetFontHandle(ScreenPlacement *scrPlace, int fontEnum)
{
	DWORD addr = UI_GetFontHandle_a;
	Font_s *funcRet;

	__asm
	{
		mov			ecx, scrPlace
		mov			eax, fontEnum
		fldz
		sub			esp, 4
		fstp		[esp]
		call		addr
		add			esp, 4
		mov			funcRet, eax
	}

	return funcRet;
}

float UI_TextWidth(const char *text, __int32 maxChars,
	Font_s *font, float scale)
{
	float funcRet;

	__asm
	{
		push			maxChars
		push			text
		mov				eax, font
		movss			xmm0, scale
		call			UI_TextWidthInternal
		add				esp, 8h
		cvtsi2ss		xmm0, eax
		movss			funcRet, xmm0
	}

	return funcRet;
}

int R_TextWidth(const char *text, int maxChars, Font_s *font)
{
	DWORD addr = R_TextWidth_a;
	int funcRet;

	__asm
	{
		mov				eax, text
		push			font
		push			maxChars
		call			addr
		add				esp, 8h
		mov				funcRet, eax
	}

	return funcRet;
}

int Sys_Milliseconds()
{
	return timeGetTime() - *(int*)0x22BEC34;
}

WeaponDef* BG_GetWeaponDef(int weapon)
{
	return bg_weaponVariantDefs[weapon];
}