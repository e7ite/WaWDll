#include "stdafx.h"

UiContext *dc                       = (UiContext*)0x208E920;
ScreenPlacement *scrPlace           = (ScreenPlacement*)0x957360;
KeyState *keys                      = (KeyState*)0x951C44;
centity_s *cg_entitiesArray         = (centity_s*)0x35D39F0;
cg_s *cgameGlob                     = (cg_s*)0x34732B8;
clientActive_t *clientActive        = (clientActive_t*)0x3058528;
WORD *clientObjMap                  = (WORD*)0x1FE58C8;
BYTE *objBuf                        = (BYTE*)0x1F978C8;
HWND *hwnd                          = (HWND*)0x22C1BE4;
scrVmPub_t *gScrVmPub               = (scrVmPub_t*)0x3BD4700;
gentity_s *g_entities               = (gentity_s*)0x176C6F0;
WeaponDef **bg_weaponVariantDefs    = (WeaponDef**)0x8F6770;
cgs_t *cgs                          = (cgs_t*)0x3466578;
actor_s *actors                     = (actor_s*)0x176C874;
int *cl_connectionState             = (int*)0x305842C;

std::vector<QWORD> GameData::detours;
std::map<const char*, dvar_s*> GameData::dvars;
bool GameData::dvarsInitialized;
bool GameData::sndsInitialized;
GameData::Font GameData::normalFont       = { 1, "fonts/normalFont" };
int(__stdcall *GameData::MessageBoxA)(HWND hWnd, LPCSTR lpText,
    LPCSTR lpCaption, UINT uType)
= *(int(__stdcall**)(HWND, LPCSTR, LPCSTR, UINT))MessageBoxA_a;
DWORD(__stdcall *GameData::timeGetTime)() = *(DWORD(__stdcall**)())timeGetTime_a;

Colors::Color Colors::white               = { 255, 255, 255, 255 };
Colors::Color Colors::black               = {   0,   0,   0, 255 };
Colors::Color Colors::red                 = { 255,   0,   0, 255 };
Colors::Color Colors::green               = {   0, 255,   0, 255 };
Colors::Color Colors::blue                = {   0,   0, 255, 255 };
Colors::Color Colors::transparentBlack    = {   0,   0,   0, 100 };

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
_iobuf*(__cdecl *FileWrapper_Open)(const char *filename, const char *mode)
    = (_iobuf*(__cdecl*)(const char*, const char*))FileWrapper_Open_a;
void(__cdecl *Com_Error)(int code, const char *fmt, ...)
    = (void(__cdecl*)(int, const char*,...))Com_Error_a;
snd_buffer*(__cdecl *SND_FindBuffer)(const char *filename, unsigned int offset)
    = (snd_buffer*(__cdecl*)(const char*, unsigned int))SND_FindBuffer_a;

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

bool GameData::InsertDvar(const char *dvarName, dvar_s *dvar)
{
    dvar_s *pdvar = dvar ? dvar : Dvar_FindVar(dvarName);
    if (!pdvar)
        return false;

    dvars.insert(std::pair<const char*, dvar_s*>(dvarName, pdvar));
    return true;
}

bool InGame()
{
    return GameData::dvarsInitialized &&
        GameData::dvars["cl_ingame"]->current.enabled
        && *cl_connectionState >= 9;
}

void Cbuf_AddText(const char *cmd)
{
    DWORD addr = Cbuf_AddText_a;
    __asm
    {
        mov             ecx, 0
        mov             eax, cmd
        call            addr
    }
}

void CG_DrawRotatedPicPhysical(ScreenPlacement *scrPlace, float x, float y,
    float width, float height, float angle, const float *color, void *material)
{
    DWORD addr = CG_DrawRotatedPicPhysical_a;
    __asm
    {
        push            material
        push            color
        sub             esp, 10h
        fld             x
        fstp            [esp]
        fld             y
        fstp            [esp + 4]
        fld             width
        fstp            [esp + 8]
        fld             height
        fstp            [esp + 12]
        movss           xmm0, angle
        mov             edx, scrPlace
        call            addr
        add             esp, 18h
    }
}

int Key_StringToKeynum(const char *name)
{
    DWORD addr = Key_StringToKeynum_a;
    DWORD funcRet;
    __asm
    {
        mov             edi, name
        call            addr
        mov             funcRet, eax
    }
    return funcRet;
}

bool Key_IsDown(const char *bind)
{
    for (__int16 i = 0; i < 256; i++)
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
        mov         edi, dobj
        movzx       ecx, tagname
        push        pos
        push        cent
        call        CG_DObjGetWorldTagPos
        add         esp, 8
        mov         funcRet, al
    }

    return funcRet;
}

unsigned __int16 SL_FindString(const char *tagname)
{
    unsigned int len = strlen(tagname) + 1;
    DWORD addr = SL_FindString_a;
    __asm
    {
        mov         eax, 0
        push        len
        push        tagname
        call        addr
        add         esp, 8
    }
}

bool WorldPosToScreenPos(int localClientNum, const float *world, float pos[2])
{
    DWORD addr = WorldPosToScreenPos_a;
    bool funcRet;
    __asm
    {
        mov         ecx, localClientNum
        mov         eax, world
        mov         esi, pos
        call        addr
        mov         funcRet, al
    }
    return funcRet;
}

void vectoangles(const float *vec, float *pos)
{
    DWORD addr = vectoangles_a;
    __asm
    {
        mov         edi, pos
        mov         esi, vec
        call        addr
    }
}

void CL_GetUserCmd(int cmdNum, usercmd_s *cmd)
{
    __asm
    {
        mov         esi, cmdNum
        and         esi, 7Fh
        lea         eax, ds:0[esi * 8]
        sub         eax, esi
        lea         esi, ds:30FD700h[eax * 8]   ;source
        mov         edi, cmd                    ;destination
        mov         ecx, 0Eh                    ;amount of dwords to copy
        rep         movsd                       ;move dwords esi to edi,
                                                ;ecx times
    }
}

usercmd_s* CL_GetUserCmd(int cmdNum)
{
    usercmd_s *funcRet;
    __asm
    {
        mov         esi, cmdNum
        and         esi, 7Fh
        lea         eax, ds:0[esi * 8]
        sub         eax, esi
        lea         esi, ds:30FD700h[eax * 8]
        mov         funcRet, esi
    }
    return funcRet;
}

void CL_DrawTextPhysical(const char *text, int maxChars,
    void *font, float x, float y, float xScale, float yScale,
    float rotation, const float *color, int style)
{
    __asm
    {
        push        style 
        sub         esp, 14h
        fld         rotation
        fstp        [esp + 10h]
        fld         yScale
        fstp        [esp + 0Ch]
        fld         xScale
        fstp        [esp + 08h]
        fld         y
        fstp        [esp + 04h]
        fld         x
        fstp        [esp]
        push        font
        push        maxChars
        push        text
        mov         ecx, color
        call        CL_DrawTextPhysicalInternal
        add         esp, 24h
    }
}

void ScrPlace_ApplyRect(ScreenPlacement *scrPlace, 
    float *x, float *y, float *w, float *h, int horzAlign, int vertAlign)
{
    DWORD addr = ScrPlace_ApplyRect_a;
    __asm
    {
        mov         edx, x
        mov         ecx, w
        mov         edi, y
        mov         esi, h
        push        vertAlign
        push        horzAlign
        push        scrPlace
        call        addr
        add         esp, 0Ch
    }
}

void UI_DrawText(ScreenPlacement *scrPlace, const char *text,
    int maxChars, void *font, float x, float y, float scale, 
    float angle, const float *color, int style)
{
    DWORD addr = UI_DrawText_a;
    __asm
    {
        mov         eax, 0
        mov         ecx, 0
        push        style
        push        color
        push        scale
        push        y
        push        x
        push        font
        push        maxChars
        push        text
        push        scrPlace
        call        addr
        add         esp, 24h
    }
}

Font_s* UI_GetFontHandle(ScreenPlacement *scrPlace, int fontEnum)
{
    DWORD addr = UI_GetFontHandle_a;
    Font_s *funcRet;
    __asm
    {
        mov         ecx, scrPlace
        mov         eax, fontEnum
        fldz
        sub         esp, 4
        fstp        [esp]
        call        addr
        add         esp, 4
        mov         funcRet, eax
    }
    return funcRet;
}

float UI_TextWidth(const char *text, int maxChars,
    Font_s *font, float scale)
{
    float funcRet;
    __asm
    {
        push        maxChars
        push        text
        mov         eax, font
        movss       xmm0, scale
        call        UI_TextWidthInternal
        add         esp, 8
        cvtsi2ss    xmm0, eax
        movss       funcRet, xmm0
    }
    return funcRet;
}

float R_TextWidth(const char *text, int maxChars, Font_s *font)
{
    DWORD addr = R_TextWidth_a;
    float funcRet;
    __asm
    {
        mov         eax, text
        push        font
        push        maxChars
        call        addr
        add         esp, 8
        cvtsi2ss    xmm0, eax
        movss       funcRet, xmm0
    }
    return funcRet;
}

int Sys_Milliseconds()
{
    return GameData::timeGetTime() - *(int*)0x22BEC34;
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
        mov         esi, ps
        mov         edx, minSpread
        mov         ecx, maxSpread
        push        weap
        call        addr
        add         esp, 4
    }
}

void AngleVectors(const float *angles, float *forward, float *right, float *up)
{
    DWORD addr = AngleVectors_a;
    __asm
    {
        mov         edx, angles
        mov         edi, right
        mov         esi, forward
        push        up
        call        addr
        add         esp, 4
    }
}

void DrawSketchPicGun(ScreenPlacement *scrPlace, rectDef_s *rect,
    const float *color, Material *material, int ratio)
{
    DWORD addr = DrawSketchPicGun_a;
    __asm
    {
        mov         eax, rect
        push        ratio
        push        material
        push        color
        push        scrPlace
        call        addr
        add         esp, 10h
    }
}

bool CG_GetPlayerViewOrigin(int localClientNum, playerState_s *ps, float origin[3])
{
    DWORD addr = CG_GetPlayerViewOrigin_a;
    bool funcRet;
    __asm
    {
        mov         eax, localClientNum
        push        origin
        push        ps
        call        addr
        add         esp, 8
        mov         funcRet, al
    }
    return funcRet;
}

float __libm_sse2_tan(float x)
{
    DWORD addr = __libm_sse2_tan_a;
    float funcRet;
    __asm
    {
        movss       xmm0, x
        call        addr
        movss       funcRet, xmm0
    }
    return funcRet;
}

void Vec3Normalize(float *x)
{
    DWORD addr = Vec3Normalize_a;
    __asm
    {
        mov         esi, x
        push        addr
        ret
    }
}

void UI_DrawRect(ScreenPlacement *scrPlace, float x, float y, float width,
    float height, int horzAlign, int vertAlign, float thickness, const float *color)
{
    DWORD addr = UI_DrawRect_a;
    __asm
    {
        mov         eax, color
        mov         ecx, horzAlign
        sub         esp, 4
        fld         thickness
        fstp        dword ptr [esp]
        push        vertAlign
        sub         esp, 10h
        fld         height
        fstp        dword ptr [esp + 0Ch]
        fld         width
        fstp        dword ptr [esp + 8]
        fld         y
        fstp        dword ptr [esp + 4]
        fld         x
        fstp        dword ptr [esp]
        push        scrPlace
        call        addr
        add         esp, 1Ch
    }
}

void UI_FillRect(ScreenPlacement *scrPlace, float x, float y, float width,
    float height, int horzAlign, int vertAlign, const float *color)
{
    DWORD addr = UI_FillRect_a;
    __asm
    {
        sub         esp, 1Ch
        mov         ecx, scrPlace
        mov         edx, color
        mov         dword ptr [esp + 18h], edx
        mov         edx, vertAlign
        mov         dword ptr [esp + 14h], edx
        mov         edx, horzAlign
        mov         dword ptr [esp + 10h], edx
        fld         height
        fstp        dword ptr [esp + 0Ch]
        fld         width
        fstp        dword ptr [esp + 8]
        fld         y
        fstp        dword ptr [esp + 4]
        fld         x
        fstp        dword ptr [esp]
        call        addr
        add         esp, 1Ch
    }
}

bool AimTarget_IsTargetVisible(centity_s *cent, unsigned __int16 bone)
{
    DWORD addr = AimTarget_IsTargetVisible_a;
    bool funcRet;
    __asm
    {
        movzx       eax, bone
        push        cent
        call        addr
        mov         funcRet, al
        add         esp, 4
    }
    return funcRet;
}

bool IN_IsForegroundWindow()
{
    return *(bool*)(0x229A0D4);
}

void speex_error(const char *arg)
{
    __asm
    {
        mov         eax, arg
        push        6C1CE0h
        ret
    }
}

const char* SL_ConvertToString(int stringValue)
{
    const char **gScrMemTreePub = (const char**)0x3702390;
    return *gScrMemTreePub + ((stringValue * 2 + stringValue) * 4) + 4;
}

unsigned int FindVariableIndexInternal(scriptInstance_t inst, unsigned int name,
    unsigned int index)
{
    DWORD addr = FindVariableIndexInternal_a;
    unsigned int funcRet;
    __asm
    {
        mov         eax, inst
        push        index
        push        name
        call        addr
        add         esp, 8
        mov         funcRet, eax
    }
    return funcRet;
}

unsigned int FindVariable(scriptInstance_t inst,
    unsigned int parentId, unsigned int unsignedValue)
{
    WORD *gScVarGlob = (WORD*)0x3974700;
    return static_cast<DWORD>(
        *(WORD*)(  
            (int)gScVarGlob + (
                (FindVariableIndexInternal(inst,
                    unsignedValue, (parentId + unsignedValue) % 0xFFFD + 1)
                + inst * 0x16000) << 4 
            )
        )
    );
}

unsigned int FindObject(scriptInstance_t inst, unsigned int id)
{
    WORD *gScVarGlob = (WORD*)0x3974704;
    return *(unsigned int*)((int)gScVarGlob + ((id + inst * 0x16000) << 4));
}

unsigned int Scr_GetSelf(scriptInstance_t inst, unsigned int threadId)
{
    unsigned short *gsvg_variableList = (unsigned short*)0x3914716;
    int index = (threadId + inst * 0x16000) << 4;

    return static_cast<unsigned int>(
        *(decltype(gsvg_variableList))(
            (int)gsvg_variableList + index
        )
    );
}

unsigned int FindLastSibling(scriptInstance_t inst, unsigned int parentId)
{
    DWORD addr = FindLastSibling_a;
    DWORD funcRet;
    __asm
    {
        mov         edx, parentId
        mov         esi, inst
        call        addr
        mov         funcRet, eax
    }
    return funcRet;
}

int GetVariableKeyObject(scriptInstance_t inst, unsigned int id)
{
    WORD *gsvg_variableList = (WORD*)0x3974700;
    int *gScrVarGlob = (int*)0x3974708;
    int index = inst * 0x16000;

    unsigned int index2 = 
        static_cast<unsigned int>(
            *(WORD*)((int)gsvg_variableList + ((id + index) << 4))
    );

    return ((*(int*)((int)gScrVarGlob + ((index2 + index) << 4))) >> 8) - 0x10000;
}

void Scr_SetParameters(int count)
{
    gScrVmPub[SCRIPTINSTANCE_CLIENT].top->u.intValue = count;
}

void Scr_AddFloat(float value)
{
    DWORD addr = Scr_AddFloat_a;
    scriptInstance_t inst = SCRIPTINSTANCE_CLIENT;
    __asm
    {
        mov         eax, inst
        sub         esp, 4
        movss       xmm0, value
        movss       [esp], xmm0
        call        addr
        add         esp, 4
    }
}

void Scr_AddInt(int value)
{
    DWORD addr = Scr_AddInt_a;
    __asm
    {
        mov         eax, value
        call        addr
    }
}

void Scr_AddString(const char *string)
{
    DWORD addr = Scr_AddString_a;
    scriptInstance_t inst = SCRIPTINSTANCE_CLIENT;
    __asm
    {
        mov         eax, inst
        push        string
        call        addr
        add         esp, 4
    }
}

void Scr_AddVector(const float *value)
{
    DWORD addr = Scr_AddVector_a;
    scriptInstance_t inst = SCRIPTINSTANCE_CLIENT;
    __asm
    {
        mov         eax, inst
        push        value
        call        addr
        add         esp, 4
    }
}

int FS_WriteFile(const char *filename, const void *buffer, int limit)
{
    DWORD addr = FS_WriteFile_a;
    int status;
    __asm
    {
        mov         eax, filename
        push        limit
        push        buffer
        call        addr
        add         esp, 8
        mov         status, eax
    }
    return status;
}

int FS_ReadFile(const char *qpath, void **buffer)
{
    DWORD addr = FS_ReadFile_a;
    int status;
    __asm
    {
        mov         eax, qpath
        push        buffer
        call        addr
        add         esp, 4
        mov         status, eax
    }
    return status;
}

XAsset DB_FindXAsset(XAssetType type)
{
    XAssetHeader **DB_XAssetPool = (XAssetHeader**)0x8DC828;
    int *g_poolSize = (int*)0x8DC5D0;
    const char*(__cdecl **DB_XAssetGetNameHandler)(XAssetHeader *header)
        = (const char*(__cdecl**)(XAssetHeader*))0x8DCAF8;

    XAsset xasset;
    xasset.header = DB_XAssetPool[type];
    xasset.type = type;
    xasset.assets = g_poolSize[type];
    xasset.name = DB_XAssetGetNameHandler[type](xasset.header);
    return xasset;
}

snd_alias_list_t* SND_FindAlias(int localClientNum, const char *name)
{
    DWORD addr = SND_FindAlias_a;
    snd_alias_list_t *funcRet;
    __asm
    {
        mov         esi, name
        push        localClientNum
        call        addr
        add         esp, 4
        mov         funcRet, eax
    }
    return funcRet;
}

_iobuf* FS_FileOpenReadBinary(const char *filename)
{
    return FileWrapper_Open(filename, "rb");
}

void UI_PlaySound(int context, const char *aliasname)
{
    DWORD addr = UI_PlaySound_a;
    __asm
    {
        mov         eax, aliasname
        push        context
        call        addr
        add         esp, 4
    }
}

int SND_Play(const char *alias, int entIndex, float volume)
{
    DWORD addr = SND_Play_a;
    int funcRet;
    __asm
    {
        mov         eax, alias
        sub         esp, 8
        fld         volume
        fstp        [esp + 4]
        mov         ecx, entIndex
        mov         [esp], ecx
        call        addr
        add         esp, 8
        mov         funcRet, eax
    }
    return funcRet;
}