#pragma once

namespace GameData
{
    struct playerState_s;

#pragma pack(push, 1)
    struct WeaponDef
    {
        const char      *szInternalName;                // 0x000
        const char      *szOverlayName;                 // 0x004
        char             pad00[0x13C];                  // 0x008
        int              weapType;                      // 0x144
        int              weapClass;                     // 0x148
        int              penetrateType;                 // 0x14C
        int              impactType;                    // 0x150
        int              inventoryType;                 // 0x154
        int              fireType;                      // 0x158
        int              clipType;                      // 0x15C
        int              itemIndex;                     // 0x160
        char             pad01[0x13C];                  // 0x164
        struct Material *reticleSlide;                  // 0x2A0
        char             pad02[0x134];                  // 0x2A4
        struct Material *hudIcon;                       // 0x3D8
        int              hudIconRatio;                  // 0x3DC
        struct Material *ammoCounterIcon;               // 0x3E0
        int              ammoCounterIconRatio;          // 0x3E4
        char             pad03[0x24];                   // 0x3E8
        int              shotCount;                     // 0x40C
        char             pad04[0x420];                  // 0x410
        float            fAdsSpread;                    // 0x830
        char             pad05[0xEC];                   // 0x834
        float            fMinDamageRange;               // 0x920
    }; // Size = 0x924
#pragma pack(pop)

    extern WeaponDef **bg_weaponVariantDefs;

    enum
    {
        RandomBulletDir_a                   = 0x4E54C0,
        BG_GetSpreadForWeapon_a             = 0x41DB20,
    };

    extern void (__cdecl *RandomBulletDir)(int randSeed, float *x, float *y);

    WeaponDef *BG_GetWeaponDef(int weapon);
    void __usercall BG_GetSpreadForWeapon(playerState_s *ps, WeaponDef *weap, float *minSpread,
        float *maxSpread);
}