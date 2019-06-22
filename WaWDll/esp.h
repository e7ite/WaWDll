#pragma once

#include "structures.h"
#include "menu.h"
#include "aimbot.h"

float Distance3D(vec3_t c1, vec3_t c2);
void RenderESP();
void DrawBorderBox(float head[2], float foot[2], const float *color);