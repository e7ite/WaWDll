#include "stdafx.h"
#include "aimbot.h"

bool Aimbot::ExecuteAimbot()
{
    if (InGame())
    {
        int target = GetAimbotTarget();
        if (target != -1)
        {
            // Get entity tag position in world 
            GameData::AimTarget_GetTagPos(0, &GameData::cg_entitiesArray[target],
                GameData::SL_FindString("j_head"), this->targetAngles);

            // Convert from 3D vector to angles and subtract the delta angles
            GameData::vectoangles(this->targetAngles - GameData::cgameGlob->refdef.vieworg,
                this->targetAngles);
            this->targetAngles -= GameData::cgameGlob->predictedPlayerState.delta_angles;

            return true;
        }
    }
    return false;
}

bool Aimbot::ValidTarget(GameData::centity_s *target)
{
    // Check if same entity index as me
    if (target->nextState.number == GameData::cgameGlob->clientNum)
        return 0;

    // Check if they have a valid object map
    WORD handle = GameData::clientObjMap[target->nextState.number];
    if (!handle)
        return 0;

    // Check for correct flags and not ragdoll
    return target->alive & 2
       && target->nextState.lerp.eFlags == 16
       && !target->pose.isRagdoll && !target->pose.ragdollHandle;
}

int Aimbot::GetAimbotTarget() const
{
    float enemyPos[3];
    int index = -1;
    float closestDistance = 999999999999.0f;
    unsigned short id = GameData::SL_FindString("j_head");
    float *myPos = GameData::cgameGlob->predictedPlayerState.origin;

    // Loop through all entities and find zombie entities
    for (int i = 0; i < 1024; i++)
    {
        GameData::centity_s *cent = &GameData::cg_entitiesArray[i];
        if (ValidTarget(cent)
           && GameData::AimTarget_GetTagPos(0, cent, id, enemyPos))
        {
            // Find closest visible zombie and get their index
            if (GameData::AimTarget_IsTargetVisible(cent, id))
            {
                if (float distance = Distance3D(myPos, enemyPos);
                    distance < closestDistance)
                {
                    index = i;
                    closestDistance = distance;
                }
            }
        }
    }

    return index;
}

void Aimbot::SetAngles() const
{
    *(vec3_t*)GameData::clientActive->viewangles = this->targetAngles;
}

void Aimbot::RemoveSpread(GameData::playerState_s *ps, GameData::usercmd_s *cmd) const
{
    float minSpread, maxSpread, finalSpread, range;
    vec3_t viewOrg, viewAxis[3], spreadEnd, spreadDir, spreadFix;
    float cgSpread = GameData::cgameGlob->aimSpreadScale / 255.0f;
    GameData::WeaponDef *weap = GameData::BG_GetWeaponDef(ps->weapon);

    // Get my view origin
    if (!CG_GetPlayerViewOrigin(0, ps, viewOrg))
        return;

    // Convert my angles into vectors
    if (GameData::cgameGlob->renderingThirdPerson)
    {
        GameData::AngleVectors(
            GameData::cgameGlob->clients[GameData::cgameGlob->clientNum].playerAngles,
            viewAxis[0], viewAxis[1], viewAxis[2]);
    }
    else
    {
        float tmp[3] = { GameData::cgameGlob->gunPitch,GameData::cgameGlob->gunYaw, 0.0f };
        GameData::AngleVectors(tmp, viewAxis[0], viewAxis[1], viewAxis[2]);
    }

    // Get current weapon spread
    GameData::BG_GetSpreadForWeapon(ps, weap, &minSpread, &maxSpread);
    finalSpread = ps->fWeaponPosFrac == 1.0f ? weap->fAdsSpread : minSpread;
    finalSpread = (maxSpread - finalSpread) * cgSpread + finalSpread;

    // Get current weapon max range
    range = weap->weapType == 3 ? weap->fMinDamageRange : 8192.0f;

    // Get the final position of the bullet 
    GameData::CG_BulletEndPos(ps->commandTime, finalSpread, viewOrg, spreadEnd, spreadDir,
        viewAxis[0], viewAxis[1], viewAxis[2], range);

    // Convert the bullet position into an angle
    GameData::vectoangles(spreadDir, spreadFix);

    // Correct my current angles to counteract the spread
    cmd->angles[0] += AngleToShort(GameData::cgameGlob->gunPitch - spreadFix[0]);
    cmd->angles[1] += AngleToShort(GameData::cgameGlob->gunYaw - spreadFix[1]);
}

void Aimbot::FixMovement(GameData::usercmd_s *cmd, float currentAngle, float oldAngle,
    float oldForwardmove, float oldRightmove) const
{
    float deltaView = currentAngle - oldAngle, f1, f2;

    if (oldAngle < 0.0f)
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