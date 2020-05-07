#include "stdafx.h"
#include "math.h"

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