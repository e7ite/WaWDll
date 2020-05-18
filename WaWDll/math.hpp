#pragma once

namespace GameData
{
    enum 
    {
        __libm_sse2_tan_a                   = 0x7E0892,
        CG_WorldPosToScreenPos_a            = 0x44CD30,
        vectoangles_a                       = 0x5DF960,
        AngleVectors_a                      = 0x5E3150,
        Vec3Normalize_a                     = 0x4037C0,
    };

     float __libm_sse2_tan(float x);
     bool CG_WorldPosToScreenPos(int localClientNum, const float *world, float pos[2]);
     void vectoangles(const float *vec, float *pos);
     void AngleVectors(const float *angles, float *forward, float *right, float *up);
     void Vec3Normalize(float *x);
}

/**
 * @brief Converts an angle in degrees to radians
 * @param The angle in degrees
 * @return The same angle in radians
**/
float DegreesToRadians(float deg);

/**
 * @brief Returns pi using Intel x87 FPU instructions
 * @return PI 
**/
float pi();

/**
 * @brief Converts from an angle in float data type to an angle in short data type
 * @param The angle to convert to a short
 * @return The same angle in a short data type
**/
int AngleToShort(float x);

/**
 * @brief Converts from an angle in short data type into an angle in float data type
 * @param The angle to convert to a float
 * @return The same angle in a float data type
**/
float ShortToAngle(int x);

/**
 * @brief Calculates the distance between two points in space
 * @param c1 A point in the game space
 * @param c2 A point in the game space
 * @return the distance between the two
**/
float Distance3D(const float *c1, const float *c2);

