#pragma once


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

