#include "stdafx.hpp"
#include "sound.hpp"

namespace GameData
{
    snd_voice_t *snd_voicesArray = (snd_voice_t *)0x4DC32EC;

    snd_buffer *(__cdecl *Snd_FindBuffer)(const char *filename, unsigned int offset)
        = (snd_buffer *(__cdecl *)(const char *, unsigned int))Snd_FindBuffer_a;
    SndBank *(__cdecl *SND_FindAliasBank)(const char *name)
        = (SndBank *(__cdecl *)(const char *))SND_FindAliasBank_a;
    FILE *(__cdecl *FileWrapper_Open)(const char *filename, const char *mode)
        = (FILE *(__cdecl *)(const char *, const char *))FileWrapper_Open_a;

    snd_voice_t *Snd_GetVoice(int i)
    {
        return ((int *)0x4DCB4DC)[i] ? &snd_voicesArray[i] : nullptr;
    }
    
    int SND_HashToIndex(int hash)
    {
        return (hash &= 0xFF) * 3;
    }

    void *SND_GetGroup(snd_alias_t *alias)
    {
        return *(void **)(0x4DC2EE8 + (((alias->flags >> 0x16) & 0x3F) << 4));
    }
    
    int __usercall FS_WriteFile(const char *filename, const void *buffer, int limit)
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

    int __usercall FS_ReadFile(const char *qpath, void **buffer)
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

    snd_alias_t *__usercall SND_PickSoundAlias(const char *name, int objectId)
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

    void __usercall UI_PlaySound(int context, const char *aliasname)
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

    int __usercall SND_Play(const char *alias, int entIndex, float volume)
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

    void __usercall FS_FreeFile(void *buffer)
    {
        DWORD addr = FS_FreeFile_a;
        __asm
        {
            mov         esi, buffer
            call        addr
        }
    }
}
