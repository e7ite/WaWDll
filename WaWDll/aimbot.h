#pragma once

#include "structures.h"
#include "esp.h"

namespace Aimbot
{
	extern vec3_t targetAngles;
	extern bool gotTarget;
}

bool ExecuteAimbot();
bool ValidTarget(centity_s *target);
int GetAimbotTarget();
void SetAngles(const vec3_t& angles);
bool AimTarget_IsTargetVisible(centity_s *cent, unsigned __int16 bone);
bool AimTarget_IsTargetVisible(int targetEntNum, unsigned __int16 bone);
void FixMovement(usercmd_s *cmd, float currentAngle, float oldAngle, 
	float oldForwardmove, float oldRightmove);
float DegreesToRadians(float deg);
float pi();
int AngleToShort(float x);
float ShortToAngle(int x);
void RemoveSpread(playerState_s *ps, usercmd_s *cmd);
void CG_BulletEndPos(int commandTime, float spread, float *start, float *end,
	float *dir, const float *forwardDir, const float *rightDir, const float *upDir,
	float maxRange);