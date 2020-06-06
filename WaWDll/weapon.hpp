#pragma once

namespace GameData
{
    struct playerState_s;

    enum weapClass_t
    {
        WEAPCLASS_RIFLE                        = 0x0,
        WEAPCLASS_MG                           = 0x1,
        WEAPCLASS_SMG                          = 0x2,
        WEAPCLASS_SPREAD                       = 0x3,
        WEAPCLASS_PISTOL                       = 0x4,
        WEAPCLASS_GRENADE                      = 0x5,
        WEAPCLASS_ROCKETLAUNCHER               = 0x6,
        WEAPCLASS_TURRET                       = 0x7,
        WEAPCLASS_NON_PLAYER                   = 0x8,
        WEAPCLASS_GAS                          = 0x9,
        WEAPCLASS_ITEM                         = 0xA,
        WEAPCLASS_MELEE                        = 0xB,
        WEAPCLASS_KILLSTREAK_ALT_STORED_WEAPON = 0xC,
        WEAPCLASS_NUM                          = 0xD,
    };

    enum weapType_t
    {
        WEAPTYPE_BULLET                        = 0x0,
        WEAPTYPE_GRENADE                       = 0x1,
        WEAPTYPE_PROJECTILE                    = 0x2,
        WEAPTYPE_BINOCULARS                    = 0x3,
        WEAPTYPE_GAS                           = 0x4,
        WEAPTYPE_BOMB                          = 0x5,
        WEAPTYPE_MINE                          = 0x6,
        WEAPTYPE_MELEE                         = 0x7,
        WEAPTYPE_NUM                           = 0x8,
    };

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
    }; // Current Size = 0x924
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