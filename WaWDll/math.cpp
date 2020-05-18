#include "stdafx.hpp"
#include "math.hpp"

namespace GameData
{
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
}

float DegreesToRadians(float deg)
{
    return deg * pi() / 180.0f;
}

float pi()
{
    float funcRet;
    __asm
    {
        fldpi
        fstp            funcRet
    }
    return funcRet;
}

int AngleToShort(float x)
{
    return ((int)(x * 65536 / 360) & 65535);
}

float ShortToAngle(int x)
{
    return (x * (360.0f / 65536));
}

float Distance3D(const float *c1, const float *c2)
{
    float dx = c2[0] - c1[0];
    float dy = c2[1] - c1[1];
    float dz = c2[2] - c1[2];

    return sqrtf(dx * dx + dy * dy + dz * dz);
}