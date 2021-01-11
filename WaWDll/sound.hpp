#pragma once

#include "stdafx.hpp"

namespace GameData
{
#pragma pack(push, 1)
    union SoundFileRef
    {
        struct PrimedSound
        {
            const char *directory;                          // 0x00
            const char *buffer;                             // 0x04
            unsigned int size;                              // 0x08
        } *primeSnd;                                    // 0x04
        struct LoadedSound
        {
            char pad00[0x4];                                // 0x00
            const char *directory;                          // 0x04
            const char *name;                               // 0x08               
        } loadSnd;                                      // 0x0C
    }; // Size = 0x0C

    struct SoundFile
    {
        char pad00[0x4];                                // 0x00
        SoundFileRef file;                              // 0x04
    }; // Size = 0x90

    struct snd_alias_t
    {
        const char *name;                               // 0x00
        unsigned int id;                                // 0x04
        char pad00[0x4];                                // 0x08
        const char **secondaryname;                     // 0x0C
        char pad01[0x4];                                // 0x10
        SoundFile *soundFile;                           // 0x14
        int sequence;                                   // 0x18
        char pad02[0x1C];                               // 0x1C
        float distReverbMax;                            // 0x38
        float probability;                              // 0x3C
        char pad03[0x44];                               // 0x40
        int flags;                                      // 0x84
        char dryCurve;                                  // 0x88
        char pad04[0x1F];                               // 0x89
        char flags2;                                    // 0xA8
        char pad05[0xF];                                // 0xA9
    }; // Size = 0xB8

    struct SndStartAliasInfo
    {
        snd_alias_t *alias;                             // 0x00
    };

    struct SndBank
    {
        const char *name;                               // 0x00
        snd_alias_t *head;                              // 0x04
        unsigned int aliasCount;                        // 0x08
    }; // Size = 0x0C

    enum XAssetType
    {
        ASSET_TYPE_SOUND = 0x9,
    };

    union XAssetHeader
    {
        template <typename T, int N>
        struct XAssetPool
        {
            T *freeHead;
            T entries[N];
        };
        XAssetPool<SndBank, 0x3E80> sound;
    };

    struct XAsset
    {
        const char *name;                               // 0x00
        XAssetType type;                                // 0x04
        int assets;                                     // 0x08
        XAssetHeader *header;                           // 0x0C
    }; // Size = 0x10

    struct snd_buffer
    {
        char *data;                                     // 0x000
        int reference_count;                            // 0x004
        char filename[0x104];                           // 0x008
        unsigned int offset_in_file;                    // 0x10C
        unsigned int data_size;                         // 0x110
        unsigned int file_size;                         // 0x114
    };

    struct snd_asset
    {
        char pad00[0xC];                                // 0x000
        unsigned int header_size;                       // 0x00C
        unsigned int block_size;                        // 0x010
        unsigned int buffer_size;                       // 0x014
        char pad01[0x8];                                // 0x018
        char data[0x830];                               // 0x020
    }; // Size = 0x850

    struct snd_voice_t
    {
        int loadingState;                               // 0x000
        int srcChannelCount;                            // 0x004
        int baserate;                                   // 0x008
        int endtime;                                    // 0x00C
        int totalMsec;                                  // 0x010
        int handle;                                     // 0x014
        int entity_update;                              // 0x018
        char pad00[0x1C];                               // 0x01C
        int notify_context;                             // 0x038
        char pad01[0x20];                               // 0x03C
        snd_alias_t *aliases[2];                        // 0x05C
        char pad02[0x128];                              // 0x064
    }; // Size = 0x18C
#pragma pack(pop)

    extern snd_voice_t *snd_voicesArray;

    enum
    {
        DB_FindXAssetEntry_a   = 0x48D760,
        FS_WriteFile_a         = 0x5DC050,
        FS_ReadFile_a          = 0x5DBFB0,
        SND_FindAlias_a        = 0x63B560,
        FileWrapper_Open_a     = 0x7AC7B1,
        UI_PlaySound_a         = 0x5CAC10,
        SND_Play_a             = 0x6B2530,
        Snd_FindBuffer_a       = 0x6BE4D0,
        SND_FindAliasBank_a    = 0x5E5670,
        Snd_StreamGetRequest_a = 0x6BE1A0,
        Snd_LoadBuffer_a       = 0x6BE530,
        FS_FreeFile_a          = 0x5E4580,
    };

    // File system
    XAsset DB_FindXAsset(XAssetType type);
    int __usercall FS_WriteFile(const char *filename, const void *buffer, int size);
    int __usercall FS_ReadFile(const char *qpath, void **buffer);
    void __usercall FS_FreeFile(void *buffer);

    // Sound
    extern snd_buffer *(__cdecl *Snd_FindBuffer)(const char *filename, unsigned int offset);
    extern SndBank *(__cdecl *SND_FindAliasBank)(const char *name);
    extern FILE *(__cdecl *FileWrapper_Open)(const char *filename, const char *mode);

    snd_alias_t *__usercall SND_PickSoundAlias(const char *name, int objectId);
    int SND_HashToIndex(const int hash);
    void __usercall UI_PlaySound(int context, const char *aliasname);
    int __usercall SND_Play(const char *alias, int entIndex, float volume);
    snd_voice_t *Snd_GetVoice(int i);
    void *SND_GetGroup(snd_alias_t *alias);
}
