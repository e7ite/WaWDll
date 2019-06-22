#include "aimbot.h"

vec3_t Aimbot::targetAngles;
bool Aimbot::gotTarget;

bool ExecuteAimbot()
{
	if (GameData::dvarGlob["cl_ingame"]->current.enabled)
	{
		int target = GetAimbotTarget();
		if (target != -1)
		{
			AimTarget_GetTagPos(0, &cg_entitiesArray[target],
				SL_FindString("j_head"), Aimbot::targetAngles);

			vectoangles(Aimbot::targetAngles - cgameGlob->refdef.vieworg, 
				Aimbot::targetAngles);

			Aimbot::targetAngles -= cgameGlob->predictedPlayerState.delta_angles;

			return true;
		}
	}
	
	return false;
}

bool ValidTarget(centity_s *target)
{
	if (target->nextState.number == cgameGlob->clientNum)
		return 0;

	WORD handle = clientObjMap[target->nextState.number];
	if (!handle)
		return 0;

	DWORD dobj = (DWORD)objBuf + 0x68 * handle;
	if (!dobj)
		return 0;

	return target->alive & 2
		&& target->nextState.lerp.eFlags == 16
		&& !target->pose.isRagdoll && !target->pose.ragdollHandle;
}

bool AimTarget_IsTargetVisible(centity_s *cent, unsigned __int16 bone)
{
	DWORD addr = AimTarget_IsTargetVisible_a;
	bool funcRet;

	__asm
	{
		movzx		eax, bone
		push		cent
		call		addr
		mov			funcRet, al
		add			esp, 4
	}

	return funcRet;
}

int GetAimbotTarget()
{
	int index = -1;
	float vec[3];
	float closestDistance = static_cast<float>(INT_MAX);

	for (__int32 i = 0; i < 1024; ++i)
	{
		if (ValidTarget(&cg_entitiesArray[i])
			&& AimTarget_GetTagPos(0, 
				&cg_entitiesArray[i],
				SL_FindString("j_head"), vec))
		{
			if (AimTarget_IsTargetVisible(
				&cg_entitiesArray[i],
				SL_FindString("j_head")))
			{
				float distance = Distance3D(
					cgameGlob->predictedPlayerState.origin, vec);
				if (distance < closestDistance)
				{
					index = i;
					closestDistance = distance;
				}
			}
		}
	}

	return index;
}

void SetAngles(const vec3_t& angles)
{
	clientActive->viewangles[0] = angles.pitch;
	clientActive->viewangles[1] = angles.yaw;
	clientActive->viewangles[2] = angles.roll;
}

void FixMovement(usercmd_s *cmd, float currentAngle, float oldAngle, 
	float oldForwardmove, float oldRightmove)
{
	float deltaView = currentAngle - oldAngle, f1, f2;

	if (oldAngle < 0.f)
		f1 = 360.0f + oldAngle;
	else
		f1 = oldAngle;

	if (currentAngle < 0.0f)
		f2 = 360.0f + currentAngle;
	else
		f2 = currentAngle;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);

	deltaView = 360.0f - deltaView;

	cmd->forwardmove = static_cast<char>(
		cosf(DegreesToRadians(deltaView)) * oldForwardmove 
		+ cosf(DegreesToRadians(deltaView + 90.f)) * oldRightmove
	);
	cmd->rightmove = static_cast<char>(
		sinf(DegreesToRadians(deltaView)) * oldForwardmove 
		+ sinf(DegreesToRadians(deltaView + 90.f)) * oldRightmove
	);
}

float DegreesToRadians(float deg)
{
	return deg * (pi() / 180.f);
}

float pi()
{
	float funcRet;

	__asm
	{
		fldpi
		fstp			funcRet
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
