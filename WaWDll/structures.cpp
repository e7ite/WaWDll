#include "stdafx.h"
#include "structures.h"

namespace GameData
{
    UiContext       *dc                               = (UiContext*)0x208E920;
    ScreenPlacement *scrPlace                         = (ScreenPlacement*)0x957360;
    KeyState        *keys                             = (KeyState*)0x951C44;
    centity_s       *cg_entitiesArray                 = (centity_s*)0x35D39F0;
    cg_s            *cgameGlob                        = (cg_s*)0x34732B8;
    clientActive_t  *clientActive                     = (clientActive_t*)0x3058528;
    WORD            *clientObjMap                     = (WORD*)0x1FE58C8;
    BYTE            *objBuf                           = (BYTE*)0x1F978C8;
    HWND            *hwnd                             = (HWND*)0x22C1BE4;
    scrVmPub_t      *gScrVmPub                        = (scrVmPub_t*)0x3BD4700;
    gentity_s       *g_entities                       = (gentity_s*)0x176C6F0;
    WeaponDef      **bg_weaponVariantDefs             = (WeaponDef**)0x8F6770;
    snd_voice_t     *snd_voicesArray                  = (snd_voice_t*)0x4DC32EC;
    cgs_t           *cgs                              = (cgs_t*)0x3466578;
    actor_s         *actors                           = (actor_s*)0x176C874;
    int             *cl_connectionState               = (int*)0x305842C;

    int(__stdcall *MessageBoxA)(HWND hWnd, LPCSTR lpText,
        LPCSTR lpCaption, UINT uType)
        = *(int(__stdcall**)(HWND, LPCSTR, LPCSTR, UINT))MessageBoxA_a;
    DWORD(__stdcall *timeGetTime)() = *(DWORD(__stdcall**)())timeGetTime_a;
    void(__stdcall *InitializeCriticalSection)(LPCRITICAL_SECTION lpCriticalSection)
        = *(void(__stdcall**)(LPCRITICAL_SECTION))InitializeCriticalSection_a;
    void(__stdcall *EnterCriticalSection)(LPCRITICAL_SECTION lpCriticalSection)
        = *(void(__stdcall**)(LPCRITICAL_SECTION))EnterCriticalSection_a;
    void(__stdcall *LeaveCriticalSection)(LPCRITICAL_SECTION lpCriticalSection)
        = *(void(__stdcall**)(LPCRITICAL_SECTION))LeaveCriticalSection_a;

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
    FILE*(__cdecl *FileWrapper_Open)(const char *filename, const char *mode)
        = (FILE*(__cdecl*)(const char*, const char*))FileWrapper_Open_a;
    void(__cdecl *Com_Error)(int code, const char *fmt, ...)
        = (void(__cdecl*)(int, const char*, ...))Com_Error_a;
    snd_buffer*(__cdecl *Snd_FindBuffer)(const char *filename, unsigned int offset)
        = (snd_buffer*(__cdecl*)(const char*, unsigned int))Snd_FindBuffer_a;
    char*(__cdecl *va)(const char *fmt, ...) 
        = (char*(__cdecl*)(const char*, ...))va_a;
    SndBank*(__cdecl *SND_FindAliasBank)(const char *name)
        = (SndBank*(__cdecl*)(const char*))SND_FindAliasBank_a;

    snd_voice_t *Snd_GetVoice(int i)
    {
        return ((int*)0x4DCB4DC)[i] ? &snd_voicesArray[i] : nullptr;
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
            fstp            [esp + 0Ch]
            movss           xmm0, angle
            mov             edx, scrPlace
            call            addr
            add             esp, 18h
        }
    }

    int Key_StringToKeynum(const char *name)
    {
        DWORD result;
        DWORD addr = Key_StringToKeynum_a;
        __asm
        {
            mov             edi, name
            call            addr
            mov             result, eax
        }
        return result;
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
        return static_cast<float>(
            floor(font->pixelHeight * R_NormalizedTextScale(font, scale) + 0.5));
    }

    float R_TextHeight(Font_s *font)
    {
        return static_cast<float>(font->pixelHeight);
    }

    int AimTarget_GetTagPos(int localClientNum, centity_s *cent,
        unsigned __int16 tagname, float *pos)
    {
        bool result;
        DWORD CG_DObjGetWorldTagPos = CG_DObjGetWorldTagPos_a;

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
            mov         result, al
        }

        return result;
    }

    unsigned short SL_FindString(const char *tagname)
    {
        unsigned short result;
        unsigned int len = strlen(tagname) + 1;
        DWORD addr = SL_FindString_a;
        __asm
        {
            mov         eax, 0
            push        len
            push        tagname
            call        addr
            add         esp, 8
            mov         result, ax
        }
        return result;
    }

    bool CG_WorldPosToScreenPos(int localClientNum, const float *world, float pos[2])
    {
        bool result;
        DWORD addr = CG_WorldPosToScreenPos_a;
        __asm
        {
            mov         ecx, localClientNum
            mov         eax, world
            mov         esi, pos
            call        addr
            mov         result, al
        }
        return result;
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
            lea         esi, ds:30FD700h[eax * 8]   ; source
            mov         edi, cmd                    ; destination
            mov         ecx, 0Eh                    ; amount of dwords to copy
            rep movsd                               ; move dwords esi to edi,
                                                    ; ecx times
        }
    }

    usercmd_s *CL_GetUserCmd(int cmdNum)
    {
        usercmd_s *result;
        __asm
        {
            mov         esi, cmdNum
            and         esi, 7Fh
            lea         eax, ds:0[esi * 8]
            sub         eax, esi
            lea         esi, ds:30FD700h[eax * 8]
            mov         result, esi
        }
        return result;
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

    Font_s *UI_GetFontHandle(ScreenPlacement *scrPlace, int fontEnum)
    {
        DWORD addr = UI_GetFontHandle_a;
        Font_s *result;
        __asm
        {
            mov         ecx, scrPlace
            mov         eax, fontEnum
            fldz
            sub         esp, 4
            fstp        [esp]
            call        addr
            add         esp, 4
            mov         result, eax
        }
        return result;
    }

    float UI_TextWidth(const char *text, int maxChars,
        Font_s *font, float scale)
    {
        float result;
        __asm
        {
            push        maxChars
            push        text
            mov         eax, font
            movss       xmm0, scale
            call        UI_TextWidthInternal
            add         esp, 8
            cvtsi2ss    xmm0, eax
            movss       result, xmm0
        }
        return result;
    }

    float R_TextWidth(const char *text, int maxChars, Font_s *font)
    {
        DWORD addr = R_TextWidth_a;
        float result;
        __asm
        {
            mov         eax, text
            push        font
            push        maxChars
            call        addr
            add         esp, 8
            cvtsi2ss    xmm0, eax
            movss       result, xmm0
        }
        return result;
    }

    unsigned int Sys_Milliseconds()
    {
        return GameData::timeGetTime() - *(int*)0x22BEC34;
    }

    WeaponDef *BG_GetWeaponDef(int weapon)
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
        bool result;
        __asm
        {
            mov         eax, localClientNum
            push        origin
            push        ps
            call        addr
            add         esp, 8
            mov         result, al
        }
        return result;
    }

    float __libm_sse2_tan(float x)
    {
        DWORD addr = __libm_sse2_tan_a;
        float result;
        __asm
        {
            movss       xmm0, x
            call        addr
            movss       result, xmm0
        }
        return result;
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
            fstp        dword ptr[esp]
            push        vertAlign
            sub         esp, 10h
            fld         height
            fstp        dword ptr[esp + 0Ch]
            fld         width
            fstp        dword ptr[esp + 8]
            fld         y
            fstp        dword ptr[esp + 4]
            fld         x
            fstp        dword ptr[esp]
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
            mov         dword ptr[esp + 18h], edx
            mov         edx, vertAlign
            mov         dword ptr[esp + 14h], edx
            mov         edx, horzAlign
            mov         dword ptr[esp + 10h], edx
            fld         height
            fstp        dword ptr[esp + 0Ch]
            fld         width
            fstp        dword ptr[esp + 8]
            fld         y
            fstp        dword ptr[esp + 4]
            fld         x
            fstp        dword ptr[esp]
            call        addr
            add         esp, 1Ch
        }
    }

    bool AimTarget_IsTargetVisible(centity_s *cent, unsigned short bone)
    {
        bool result;
        DWORD addr = AimTarget_IsTargetVisible_a;
        __asm
        {
            movzx       eax, bone
            push        cent
            call        addr
            mov         result, al
            add         esp, 4
        }
        return result;
    }

    bool IN_IsForegroundWindow()
    {
        return *(bool*)(0x229A0D4);
    }

    const char *SL_ConvertToString(int stringValue)
    {
        const char **gScrMemTreePub = (const char**)0x3702390;
        return *gScrMemTreePub + ((stringValue * 2 + stringValue) * 4) + 4;
    }

    unsigned int FindVariableIndexInternal(scriptInstance_t inst, unsigned int name,
        unsigned int index)
    {
        unsigned int result;
        DWORD addr = FindVariableIndexInternal_a;
        __asm
        {
            mov         eax, inst
            push        index
            push        name
            call        addr
            add         esp, 8
            mov         result, eax
        }
        return result;
    }

    unsigned int FindVariable(scriptInstance_t inst,
        unsigned int parentId, unsigned int unsignedValue)
    {
        WORD *gScVarGlob = (WORD*)0x3974700;
        return static_cast<DWORD>(
            *(WORD*)(
                (int)gScVarGlob + ((FindVariableIndexInternal(inst,
                    unsignedValue, (parentId + unsignedValue) % 0xFFFD + 1)
                    + inst * 0x16000) << 4)
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
        unsigned short *gsvgVariableList = (unsigned short*)0x3914716;
        int index = (threadId + inst * 0x16000) << 4;

        return static_cast<unsigned int>(
                *(decltype(gsvgVariableList))((int)gsvgVariableList + index)
            );
    }

    unsigned int FindLastSibling(scriptInstance_t inst, unsigned int parentId)
    {
        DWORD result;
        DWORD addr = FindLastSibling_a;
        __asm
        {
            mov         edx, parentId
            mov         esi, inst
            call        addr
            mov         result, eax
        }
        return result;
    }

    int GetVariableKeyObject(scriptInstance_t inst, unsigned int id)
    {
        WORD *gsvgcVariableList = (WORD*)0x3974700;
        int *gScrVarGlobClient = (int*)0x3974708;
        DWORD index = inst * 0x16000;

        DWORD index2 =
            (DWORD)(*(WORD*)((DWORD)gsvgcVariableList + ((id + index) << 4)));

        return ((*(int*)(
            (DWORD)gScrVarGlobClient + ((index2 + index) << 4)
            )) >> 8) - 0x10000;
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
        int result;
        DWORD addr = FS_WriteFile_a;
        __asm
        {
            mov         eax, filename
            push        limit
            push        buffer
            call        addr
            add         esp, 8
            mov         result, eax
        }
        return result;
    }

    int FS_ReadFile(const char *qpath, void **buffer)
    {
        int result;
        DWORD addr = FS_ReadFile_a;
        __asm
        {
            mov         eax, qpath
            push        buffer
            call        addr
            add         esp, 4
            mov         result, eax
        }
        return result;
    }

    XAsset DB_FindXAsset(XAssetType type)
    {
        XAssetHeader **DB_XAssetPool = (XAssetHeader**)0x8DC828;
        int(__cdecl **DB_GetXAssetSizeHandler)() = (int(__cdecl**)())0x8DCC18;
        const char **g_assetNames = (const char**)0x8DCA68;
        int *g_poolSize = (int*)0x8DC5D0;

        XAsset xasset;
        xasset.header = DB_XAssetPool[type];
        xasset.type = type;
        xasset.assets = g_poolSize[type];
        xasset.name = g_assetNames[type];
        return xasset;
    }

    snd_alias_t *SND_PickSoundAlias(const char *name, int objectId)
    {
        DWORD addr = SND_FindAlias_a;
        snd_alias_t *funcRet;
        __asm
        {
            mov         esi, name
            push        objectId
            call        addr
            add         esp, 4
            mov         funcRet, eax
        }
        return funcRet;
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
        int result;
        DWORD addr = SND_Play_a;
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
            mov         result, eax
        }
        return result;
    }

    void FS_FreeFile(void *buffer)
    {
        DWORD addr = FS_FreeMem_a;
        __asm
        {
            mov         esi, buffer
            call        addr
        }
    }

    int SND_HashToIndex(int hash)
    {
        return (hash &= 0xFF) * 3;
    }

    void *SND_GetGroup(snd_alias_t *alias)
    {
        return *(void**)(0x4DC2EE8 + (((alias->flags >> 0x16) & 0x3F) << 4));
    }

    void CG_BulletEndPos(int commandTime, float spread, float *start, float *end,
        float *dir, const float *forwardDir, const float *rightDir, const float *upDir,
        float maxRange)
    {
        float right, up;
        float aimOffset = __libm_sse2_tan(DegreesToRadians(spread)) * maxRange;

        RandomBulletDir(commandTime, &right, &up);
        right *= aimOffset;
        up *= aimOffset;

        end[0] = maxRange * forwardDir[0] + start[0];
        end[1] = maxRange * forwardDir[1] + start[1];
        end[2] = maxRange * forwardDir[2] + start[2];

        end[0] = right * rightDir[0] + end[0];
        end[1] = right * rightDir[1] + end[1];
        end[2] = right * rightDir[2] + end[2];

        end[0] = up * upDir[0] + end[0];
        end[1] = up * upDir[1] + end[1];
        end[2] = up * upDir[2] + end[2];

        if (dir)
        {
            dir[0] = end[0] - start[0];
            dir[1] = end[1] - start[1];
            dir[2] = end[2] - start[2];
            Vec3Normalize(dir);
        }
    }
}

bool dvarsInitialized;
std::vector<QWORD> detours;
std::unordered_map<const char*, GameData::dvar_s*> dvars;

Fonts::Font Fonts::normalFont = { 1, "fonts/normalFont" };

Colors::Color Colors::white            = { 255.0f, 255.0f, 255.0f, 255.0f };
Colors::Color Colors::black            = {   0.0f,   0.0f,   0.0f, 255.0f };
Colors::Color Colors::red              = { 255.0f,   0.0f,   0.0f, 255.0f };
Colors::Color Colors::green            = {   0.0f, 255.0f,   0.0f, 255.0f };
Colors::Color Colors::blue             = {   0.0f,   0.0f, 255.0f, 255.0f };
Colors::Color Colors::transparentBlack = {   0.0f,   0.0f,   0.0f, 100.0f };

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

vec3_t &vec3_t::operator+=(const vec3_t &vec)
{
    this->x += vec.x;
    this->y += vec.y;
    this->z += vec.z;
    return *this;
}

vec3_t &vec3_t::operator+=(float vec[3])
{
    this->x += vec[0];
    this->y += vec[1];
    this->z += vec[2];
    return *this;
}

vec3_t &vec3_t::operator-=(const vec3_t &vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
    this->z -= vec.z;
    return *this;
}

vec3_t &vec3_t::operator-=(float vec[3])
{
    this->x -= vec[0];
    this->y -= vec[1];
    this->z -= vec[2];
    return *this;
}

bool InsertDvar(const char *dvarName, GameData::dvar_s *dvar)
{
    GameData::dvar_s *pdvar = dvar ? dvar : GameData::Dvar_FindVar(dvarName);
    if (!pdvar)
        return false;

    dvars.insert(std::pair<const char*, GameData::dvar_s*>(dvarName, pdvar));
    return true;
}

bool InGame()
{
    return dvarsInitialized &&
        dvars.at("cl_ingame")->current.enabled
        && *GameData::cl_connectionState >= 9;
}

bool CopyTextToClipboard(const std::string &text)
{
    HGLOBAL hg;
    bool state = false;
    size_t len = text.size();

    if (!OpenClipboard(*GameData::hwnd))
        return false;
    if (!EmptyClipboard())
        return false;

    hg = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
    if (!hg)
    {
        state = false;
        goto end;
    }
    memcpy(static_cast<LPSTR>(GlobalLock(hg)), text.c_str(), len + 1);
    GlobalUnlock(hg);
    state = SetClipboardData(CF_TEXT, hg);

end:
    CloseClipboard();
    GlobalFree(hg);
    return state;
}

float Distance3D(vec3_t c1, vec3_t c2)
{
    float dx = c2.x - c1.x;
    float dy = c2.y - c1.y;
    float dz = c2.z - c1.z;

    return sqrtf(dx * dx + dy * dy + dz  *dz);
}