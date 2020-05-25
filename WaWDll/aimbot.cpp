#include "stdafx.hpp"
#include "aimbot.hpp"

namespace GameData
{
    void BG_GetSpreadForWeapon(playerState_s *ps, WeaponDef *weap, float *minSpread,
        float *maxSpread)
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

    void CG_BulletEndPos(int commandTime, float spread, float *start, float *end,
        float *dir, const float *forwardDir, const float *rightDir, const float *upDir,
        float maxRange)
    {
        float right, up;
        float aimOffset = __libm_sse2_tan(DegreesToRadians(spread)) * maxRange;

        RandomBulletDir(commandTime, &right, &up);
        right *= aimOffset;
        up *= aimOffset;

        end[0] = maxRange * forwardDir[0] + start[0];
        end[1] = maxRange * forwardDir[1] + start[1];
        end[2] = maxRange * forwardDir[2] + start[2];

        end[0] = right * rightDir[0] + end[0];
        end[1] = right * rightDir[1] + end[1];
        end[2] = right * rightDir[2] + end[2];

        end[0] = up * upDir[0] + end[0];
        end[1] = up * upDir[1] + end[1];
        end[2] = up * upDir[2] + end[2];

        if (dir)
        {
            dir[0] = end[0] - start[0];
            dir[1] = end[1] - start[1];
            dir[2] = end[2] - start[2];
            Vec3Normalize(dir);
        }
    }

    // Do not let the game run its usual tag position function as it has error checks which
    // close the game. 
    void __usercall *AimTarget_GetTagPos_0 = (void __usercall *)AimTarget_GetTagPos_0_a;
    void __declspec(naked) AimTarget_GetTagPos_0DetourInvoke(centity_s *cent,
        unsigned short bone, float *out)
    {
        __asm
        {
            mov         eax, [esp + 4]
            push        eax
            push        esi
            push        ecx
            push        0
            call        AimTarget_GetTagPos
            add         esp, 10h
            ret
        }
    }

    // Aimbot is performed on this detour
    void (__cdecl *CL_CreateNewCommands)() = (void (__cdecl *)())CL_CreateNewCommands_a;
    void __declspec(naked) CL_CreateNewCommandsDetourInvoke()
    {
        __asm
        {
            pushad
            call        CL_CreateNewCommandsDetour
            popad
            pop         edi
            pop         esi
            pop         ebx
            add         esp, 78h
            ret
        }
    }
    void CL_CreateNewCommandsDetour()
    {
        Menu &menu = Menu::Instance();
        Aimbot &aimbot = Aimbot::Instance();
        GameData::EnterCriticalSection(&menu.critSection);

        GameData::usercmd_s *ncmd = &GameData::clientActive->cmds[GameData::clientActive->cmdNumber + 1 & 0x7F];
        GameData::usercmd_s *ccmd = &GameData::clientActive->cmds[GameData::clientActive->cmdNumber & 0x7F];
        GameData::usercmd_s *ocmd = &GameData::clientActive->cmds[GameData::clientActive->cmdNumber - 1 & 0x7F];

        ocmd->serverTime++;

        bool aimbotRun = false;
        bool isShooting = GameData::Key_IsDown("+attack");
        if (aimbot.enableAimbot.data.boolean)
        {
            if ((aimbot.aimKey.data.integer == 1 && isShooting)
                || (aimbot.aimKey.data.integer == 2 && GameData::Key_IsDown("+speed_throw"))
                || !aimbot.aimKey.data.integer)
            {
                aimbotRun = aimbot.ExecuteAimbot();
                if (aimbotRun)
                    aimbot.SetAngles();
            }
        }

        if (aimbot.autoShoot.data.boolean && (aimbotRun || isShooting))
        {
            ccmd->button_bits &= ~1;
            ocmd->button_bits |= 1;
        }

        GameData::LeaveCriticalSection(&menu.critSection);
    }

}

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
    *(vec3_t *)GameData::clientActive->viewangles = this->targetAngles;
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