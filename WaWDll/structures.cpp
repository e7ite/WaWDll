#include "stdafx.hpp"
#include "structures.hpp"

namespace GameData
{
    centity_s       *cg_entitiesArray                 = (centity_s *)0x35D39F0;
    cg_s            *cgameGlob                        = (cg_s *)0x34732B8;
    clientActive_t  *clientActive                     = (clientActive_t *)0x3058528;
    WORD            *clientObjMap                     = (WORD *)0x1FE58C8;
    BYTE            *objBuf                           = (BYTE *)0x1F978C8;
    HWND            *hwnd                             = (HWND *)0x22C1BE4;
    scrVmPub_t      *gScrVmPub                        = (scrVmPub_t *)0x3BD4700;
    gentity_s       *g_entities                       = (gentity_s *)0x176C6F0;
    WeaponDef      **bg_weaponVariantDefs             = (WeaponDef **)0x8F6770;
    snd_voice_t     *snd_voicesArray                  = (snd_voice_t *)0x4DC32EC;
    cgs_t           *cgs                              = (cgs_t *)0x3466578;
    actor_s         *actors                           = (actor_s *)0x176C874;
    int             *cl_connectionState               = (int *)0x305842C;

    int (__stdcall *MessageBoxA)(HWND hWnd, LPCSTR lpText,
        LPCSTR lpCaption, UINT uType)
        = *(int (__stdcall **)(HWND, LPCSTR, LPCSTR, UINT))MessageBoxA_a;
    DWORD (__stdcall *timeGetTime)() = *(DWORD (__stdcall **)())timeGetTime_a;
    void (__stdcall *InitializeCriticalSection)(LPCRITICAL_SECTION lpCriticalSection)
        = *(void (__stdcall **)(LPCRITICAL_SECTION))InitializeCriticalSection_a;
    void (__stdcall *EnterCriticalSection)(LPCRITICAL_SECTION lpCriticalSection)
        = *(void (__stdcall **)(LPCRITICAL_SECTION))EnterCriticalSection_a;
    void (__stdcall *LeaveCriticalSection)(LPCRITICAL_SECTION lpCriticalSection)
        = *(void (__stdcall **)(LPCRITICAL_SECTION))LeaveCriticalSection_a;

    void (__cdecl *Cmd_ExecuteSingleCommand)(int localClientNum,
        int controllerIndex, const char *text)
        = (void(*)(int, int, const char *))Cmd_ExecuteSingleCommmand_a;
    dvar_s *(__cdecl *Dvar_FindVar)(const char *dvarName)
        = (dvar_s *(*)(const char *))Dvar_FindVar_a;
    int (__cdecl *CG_GetPlayerWeapon)(playerState_s *ps, int localClientNum)
        = (int (__cdecl *)(playerState_s *, int))CG_GetPlayerWeapon_a;
    void (__cdecl *RandomBulletDir)(int randSeed, float *x, float *y)
        = (void (__cdecl *)(int, float *, float *))RandomBulletDir_a;
    FILE *(__cdecl *FileWrapper_Open)(const char *filename, const char *mode)
        = (FILE *(__cdecl *)(const char *, const char *))FileWrapper_Open_a;
    void (__cdecl *Com_Error)(int code, const char *fmt, ...)
        = (void (__cdecl *)(int, const char *, ...))Com_Error_a;
    snd_buffer *(__cdecl *Snd_FindBuffer)(const char *filename, unsigned int offset)
        = (snd_buffer *(__cdecl *)(const char *, unsigned int))Snd_FindBuffer_a;
    char *(__cdecl *va)(const char *fmt, ...) 
        = (char *(__cdecl *)(const char *, ...))va_a;
    SndBank *(__cdecl *SND_FindAliasBank)(const char *name)
        = (SndBank *(__cdecl *)(const char *))SND_FindAliasBank_a;

    snd_voice_t *Snd_GetVoice(int i)
    {
        return ((int *)0x4DCB4DC)[i] ? &snd_voicesArray[i] : nullptr;
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

    unsigned int Sys_Milliseconds()
    {
        return GameData::timeGetTime() - *(int *)0x22BEC34;
    }

    WeaponDef *BG_GetWeaponDef(int weapon)
    {
        return bg_weaponVariantDefs[weapon];
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
        return *(bool *)(0x229A0D4);
    }

    const char *SL_ConvertToString(int stringValue)
    {
        const char **gScrMemTreePub = (const char **)0x3702390;
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
        WORD *gScVarGlob = (WORD *)0x3974700;
        return static_cast<DWORD>(
            *(WORD *)(
                (int)gScVarGlob + ((FindVariableIndexInternal(inst,
                    unsignedValue, (parentId + unsignedValue) % 0xFFFD + 1)
                    + inst * 0x16000) << 4)
                )
            );
    }

    unsigned int FindObject(scriptInstance_t inst, unsigned int id)
    {
        WORD *gScVarGlob = (WORD *)0x3974704;
        return *(unsigned int *)((int)gScVarGlob + ((id + inst * 0x16000) << 4));
    }

    unsigned int Scr_GetSelf(scriptInstance_t inst, unsigned int threadId)
    {
        unsigned short *gsvgVariableList = (unsigned short *)0x3914716;
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
        WORD *gsvgcVariableList = (WORD *)0x3974700;
        int *gScrVarGlobClient = (int *)0x3974708;
        DWORD index = inst * 0x16000;

        DWORD index2 =
            (DWORD)(*(WORD *)((DWORD)gsvgcVariableList + ((id + index) << 4)));

        return ((*(int *)(
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
        XAssetHeader **DB_XAssetPool = (XAssetHeader **)0x8DC828;
        int (__cdecl **DB_GetXAssetSizeHandler)() = (int (__cdecl **)())0x8DCC18;
        const char **g_assetNames = (const char **)0x8DCA68;
        int *g_poolSize = (int *)0x8DC5D0;

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
        return *(void **)(0x4DC2EE8 + (((alias->flags >> 0x16) & 0x3F) << 4));
    }
}

std::vector<QWORD> detours;
std::unordered_map<const char *, GameData::dvar_s *> dvars;

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

    dvars.insert(std::pair<const char *, GameData::dvar_s *>(dvarName, pdvar));
    return true;
}

bool InGame()
{
    return dvars.at("cl_ingame")->current.enabled
        && *GameData::cl_connectionState >= 9;
}

bool CopyTextToClipboard(const std::string &text)
{
    if (!OpenClipboard(*GameData::hwnd))
        return false;
    if (!EmptyClipboard())
        return false;

    size_t len = text.size() + 1;
    bool state = false;
    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, len);
    if (hg)
    {
        memcpy(static_cast<LPSTR>(GlobalLock(hg)), text.c_str(), len);
        GlobalUnlock(hg);
        state = SetClipboardData(CF_TEXT, hg);
    }

    CloseClipboard();
    GlobalFree(hg);
    return state;
}