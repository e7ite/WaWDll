#pragma once

#include "nonhost_menu.hpp"
#include "host_menu.hpp"
#include "weapon.hpp"
#include "game_entities.hpp"

namespace GameData
{
    enum scriptInstance_t;

    enum
    {
        CG_DObjGetWorldTagPos_a     = 0x443030,
        AimTarget_IsTargetVisible_a = 0x403CA0,
        AimTarget_GetTagPos_0_a     = 0x4039C0,
        CL_CreateNewCommands_a      = 0x63E994,
        CG_DamageFeedback_a         = 0x455370,
    };

    // Miscellaneous
    unsigned short SL_FindString(scriptInstance_t inst, const char *tagname);
    const char *SL_ConvertToString(int stringValue);

    // Aim assist functions 
    int __usercall AimTarget_GetTagPos(int localClientNum, centity_s *cent,
        unsigned short tagname, float *pos);
    bool __usercall AimTarget_IsTargetVisible(centity_s *cent, unsigned short bone);
    void __usercall LookAtKiller(gentity_s *attacker, gentity_s *inflictor, gentity_s *self);
    void CG_BulletEndPos(int commandTime, float spread, float *start, float *end,
        float *dir, const float *forwardDir, const float *rightDir, const float *upDir,
        float maxRange);

    // Detour to disable the game's native tag position function to avoid game ending 
    // error. When running on custom zombie maps and getting their tag position
    extern void __usercall *AimTarget_GetTagPos_0;
    void AimTarget_GetTagPos_0DetourInvoke(centity_s *cent, unsigned short bone, float *out);
        
    // Detour where aimbot will be ran
    extern void (__cdecl *CL_CreateNewCommands)();
    void CL_CreateNewCommandsDetourInvoke();
    void CL_CreateNewCommandsDetour();

    // Used to disable player damage flinch
    extern void __usercall *CG_DamageFeedback;
    void CG_DamageFeedbackDetourInvoke(int localClientNum, int yawByte,
        int pitchByte, int damage);
    bool CG_DamageFeedbackDetour(int localClientNum, int yawByte, int pitchByte,
        int damage);
}

struct Aimbot
{
    // The angles needed to be set to my player to aim at target
    vec3_t targetAngles;
    // Refereence to the menu variable; used to tell if aimbot should be used
    OptionData &enableAimbot;
    // Refereence to the menu variable; used to trigger aimbot if set
    OptionData &aimKey;
    // Refereence to the menu variable; used to tell if the aimbot should fire the 
    // gun automatically when a target it found
    OptionData &autoShoot;

    Aimbot() :
        enableAimbot(Menu::Instance().GetOptionData(AIMBOT_MENU, "Enable Aimbot")),
        aimKey(Menu::Instance().GetOptionData(AIMBOT_MENU, "Aim Key")),
        autoShoot(Menu::Instance().GetOptionData(AIMBOT_MENU, "Auto Shoot")) {}

    static Aimbot &Instance() { static Aimbot aimbot; return aimbot; }

    /**
     * @brief Finds a viable target, calculates the angles to aim at them correctly,
     *        and sets my player angles to them.
    **/
    bool ExecuteAimbot();

    /**
     * @brief Gets the closest visible player and returns their index
     * @return The target's index which can be used in GameData::cg_entitiesArray
    **/
    int GetAimbotTarget() const;

    /**
     * @brief Sets my player aimbot angles to the calculated angles from 
     *        Aimbot::targetAngles
    **/
    void SetAngles() const;

    /**
     * @brief Fixes movement when using silent aimbot
     * @param cmd The command which the custom silent angles are passed in
     * @param currentAngle The yaw after aimbot is set
     * @param oldAngle The yaw before aimbot is set
     * @param oldForwardMove The last forward move specified in cmd movement before
     * @param oldRightMove The last right move specified in cmd movement before
     *
     * For some reason, after a player uses silent aimbot through passing in the 
     * custom angles, the player's movement isn't tracked correctly anymore. This
     * method solves this issue, but I shamefully admit I do not understand the math
     * in how this works. Credit to Mr.Hankey in UnKnoWnCheaTs for this method:
     * https://www.unknowncheats.me/forum/counterstrike-global-offensive/150904-le-spinbot.html
    **/
    void FixMovement(GameData::usercmd_s *cmd, float currentAngle, float oldAngle,
        float oldForwardmove, float oldRightmove) const;

    /**
     * @brief Removes any random bullet spread when hipfiring a weapon
     * @param ps My player's playerState_s structure
     * @param cmd My player's usercmd_s structure
     *
     * This also involves some math I am not too familiar with. This code is based
     * off the game's DrawBulletImpacts function and a post off UnKnoWnCheaTs by kokole:
     * https://www.unknowncheats.me/forum/call-of-duty-black-ops/99319-perfect-spread.html
    **/
    void RemoveSpread(GameData::playerState_s *ps, GameData::usercmd_s *cmd) const;
};
