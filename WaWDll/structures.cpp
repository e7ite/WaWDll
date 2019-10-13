#include "structures.h"

UiContext *dc					 = (UiContext*)0x208E920;
ScreenPlacement *scrPlace		 = (ScreenPlacement*)0x957360;
KeyState *keys					 = (KeyState*)0x951C44;
centity_s *cg_entitiesArray		 = (centity_s*)0x35D39F0;
cg_s *cgameGlob					 = (cg_s*)0x34732B8;
clientActive_t *clientActive	 = (clientActive_t*)0x3058528;
WORD *clientObjMap				 = (WORD*)0x1FE58C8;
BYTE *objBuf					 = (BYTE*)0x1F978C8;
WeaponDef **bg_weaponVariantDefs = (WeaponDef**)0x8F6770;
cgs_t *cgs						 = (cgs_t*)0x3466578;
actor_s *actors					 = (actor_s*)0x176C874;
int cl_connectionState			 = 0x305842C;

std::vector<QWORD> GameData::detours;
std::map<const char*, dvar_s*> GameData::dvars;
GameData::Font GameData::normalFont	   = { 1,    "fonts/normalFont" };

Colors::Color Colors::white			   = { 255, 255, 255, 255 };
Colors::Color Colors::black			   = {   0,   0,   0, 255 };
Colors::Color Colors::red			   = { 255,   0,   0, 255 };
Colors::Color Colors::green			   = {   0, 255,   0, 255 };
Colors::Color Colors::blue			   = {   0,   0, 255, 255 };
Colors::Color Colors::transparentBlack = {   0,   0,   0, 100 };


Font_s*(__cdecl *R_RegisterFont)(const char *font, int imageTrac)
	= (Font_s*(*)(const char*, int))R_RegisterFont_a;
void*(__cdecl *Material_RegisterHandle)(const char *materialName, 
	int imageTrac)
	= (void*(*)(const char*, int))Material_RegisterHandle_a;
void(__cdecl *CG_DrawRotatedPic)(ScreenPlacement *scrPlace, float x, float y,
	float width, float height, float angle, const float *color, void *material)
	= (void(*)(ScreenPlacement*, float, float,
	float, float, float, const float*, void*))CG_DrawRotatedPic_a;
void(__cdecl *Cmd_ExecuteSingleCommand)(int localClientNum,
	int controllerIndex, const char *text)
	= (void(*)(int, int, const char*))Cmd_ExecuteSingleCommmand_a;
dvar_s*(__cdecl *Dvar_FindVar)(const char *dvarName)
	= (dvar_s*(*)(const char*))Dvar_FindVar_a;
void(__cdecl *CL_DrawTextPhysicalInternal)(const char *text, int maxChars,
	void *font, float x, float y, float xScale, float yScale, float rotation,
	int style) = (void(__cdecl*)(const char*, int,
	void*, float, float, float, float, float, int))CL_DrawTextPhysical_a;
int(__cdecl *UI_TextWidthInternal)(const char *text, int maxChars,
	void *font, float scale)
	= (int(*)(const char*, int, void*, float))UI_TextWidth_a;
void(__cdecl *CG_GameMessage)(int localClientNum, const char *msg, int length)
	= (void(__cdecl*)(int, const char*, int))CG_GameMessage_a;
int(__cdecl *CG_GetPlayerWeapon)(playerState_s *ps, int localClientNum)
	= (int(__cdecl*)(playerState_s*, int))CG_GetPlayerWeapon_a;
void(__cdecl *RandomBulletDir)(int randSeed, float *x, float *y)
	= (void(__cdecl*)(int, float*, float*))RandomBulletDir_a;

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

int Key_StringToKeynum(const char *name)
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

int AimTarget_GetTagPos(int localClientNum, centity_s *cent,
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
	unsigned int len = strlen(tagname) + 1;
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
	int maxChars, void *font, float x, float y, float scale, 
	float angle, const float *color, int style)
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

float UI_TextWidth(const char *text, int maxChars,
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
		add				esp, 8
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
		add				esp, 8
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

void BG_GetSpreadForWeapon(playerState_s *ps, WeaponDef *weap, float *minSpread,
	float *maxSpread)
{
	DWORD addr = BG_GetSpreadForWeapon_a;
	__asm
	{
		mov			esi, ps
		mov			edx, minSpread
		mov			ecx, maxSpread
		push		weap
		call		addr
		add			esp, 4
	}
}

void AngleVectors(const float *angles, float *forward, float *right, float *up)
{
	DWORD addr = AngleVectors_a;
	__asm
	{
		mov			edx, angles
		mov			edi, right
		mov			esi, forward
		push		up
		call		addr
		add			esp, 4
	}
}

void DrawSketchPicGun(ScreenPlacement *scrPlace, rectDef_s *rect,
	const float *color, Material *material, int ratio)
{
	DWORD addr = DrawSketchPicGun_a;
	__asm
	{
		mov			eax, rect
		push		ratio
		push		material
		push		color
		push		scrPlace
		call		addr
		add			esp, 10h
	}
}

bool CG_GetPlayerViewOrigin(int localClientNum, playerState_s *ps, float out[3])
{
	DWORD addr = CG_GetPlayerViewOrigin_a;
	bool funcRet;
	__asm
	{
		mov			eax, localClientNum
		push		out
		push		ps
		call		addr
		add			esp, 8
		mov			funcRet, al
	}
	return funcRet;
}

float __libm_sse2_tan(float x)
{
	DWORD addr = __libm_sse2_tan_a;
	float funcRet;
	__asm
	{
		movss		xmm0, x
		call		addr
		movss		funcRet, xmm0
	}
	return funcRet;
}

void Vec3Normalize(float *x)
{
	DWORD addr = Vec3Normalize_a;
	__asm
	{
		mov			esi, x
		mov			edx, addr
		call		edx
	}
}

void UI_DrawRect(ScreenPlacement *scrPlace, float x, float y, float width,
	float height, int horzAlign, int vertAlign, float thickness, const float *color)
{
	DWORD addr = UI_DrawRect_a;
	__asm
	{
		mov			eax, color
		mov			ecx, horzAlign
		sub			esp, 4
		fld			thickness
		fstp		dword ptr [esp]
		push		vertAlign
		sub			esp, 10h
		fld			height
		fstp		dword ptr [esp + 0Ch]
		fld			width
		fstp		dword ptr [esp + 8]
		fld			y
		fstp		dword ptr [esp + 4]
		fld			x
		fstp		dword ptr [esp]
		push		scrPlace
		call		addr
		add			esp, 1Ch
	}
}

void UI_FillRect(ScreenPlacement *scrPlace, float x, float y, float width,
	float height, int horzAlign, int vertAlign, const float *color)
{
	DWORD addr = UI_FillRect_a;
	__asm
	{
		sub			esp, 1Ch
		mov			ecx, scrPlace
		mov			edx, color
		mov			dword ptr [esp + 18h], edx
		mov			edx, vertAlign
		mov			dword ptr [esp + 14h], edx
		mov			edx, horzAlign
		mov			dword ptr [esp + 10h], edx
		fld			height
		fstp		dword ptr [esp + 0Ch]
		fld			width
		fstp		dword ptr [esp + 8]
		fld			y
		fstp		dword ptr [esp + 4]
		fld			x
		fstp		dword ptr [esp]
		call		addr
		add			esp, 1Ch
	}
}

bool InGame()
{
	return GameData::dvars["cl_ingame"]->current.enabled && *(int*)cl_connectionState >= 9;
}

void InsertDvar(const char *dvarName)
{
	GameData::dvars.insert(
		std::pair<const char*, dvar_s*>(dvarName, Dvar_FindVar(dvarName))
	);
}