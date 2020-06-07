#include "stdafx.hpp"
#include "weapon.hpp"

namespace GameData
{
    WeaponDef **bg_weaponVariantDefs = (WeaponDef **)0x8F6770;

    void (__cdecl *RandomBulletDir)(int randSeed, float *x, float *y)
        = (void (__cdecl *)(int, float *, float *))RandomBulletDir_a;

    WeaponDef *BG_GetWeaponDef(int weapon)
    {
        return bg_weaponVariantDefs[weapon];
    }

    void BG_GetSpreadForWeapon(struct playerState_s *ps, WeaponDef *weap, 
        float *minSpread, float *maxSpread)
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
}
