#pragma once

#include "structures.hpp"
#include "aimbot.hpp"

// TODO: Implement ESP singleton class

/**
 * @brief Iterates through game entities and draws a bordered box on zombies
**/
void RenderESP();

/**
 * @brief Draws a bordered box around on a player/zombie
 * @param The head position in 2D space of the player
 * @param The foot position in 2D space of the player
 * @param The color of the box to draw
**/
void DrawBorderBox(float head[2], float foot[2], const float *color);
