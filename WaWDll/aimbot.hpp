#pragma once

#include "menu.hpp"

namespace GameData
{
    enum
    {
        AimTarget_GetTagPos_0_a             = 0x4039C0,
        CL_CreateNewCommands_a              = 0x63E994,
    };

    extern void __usercall *AimTarget_GetTagPos_0;
    void AimTarget_GetTagPos_0DetourInvoke(centity_s *cent, unsigned short bone, float *out);
        
    extern void (__cdecl *CL_CreateNewCommands)();
    void CL_CreateNewCommandsDetourInvoke();
    void CL_CreateNewCommandsDetour();
}

struct Aimbot
{
    // The angles needed to be set to my player to aim at target
    vec3_t      targetAngles;
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
