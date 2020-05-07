#pragma once

#include "structures.h"
#include "menu.h"

struct Aimbot
{
    // The angles needed to be set to my player to aim at target
    vec3_t      targetAngles;
    // The index in the cg_entitiesArray structure to access this target
    int         target;
    // Refereence to the menu variable; used to tell if aimbot should be used
    OptionData &enableAimbot;
    // Refereence to the menu variable; used to trigger aimbot if set
    OptionData &aimKey;
    // Refereence to the menu variable; used to tell if the aimbot should fire the 
    // gun automatically when a target it found
    OptionData &autoShoot;

    Aimbot() : 
        target(-1),
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
     * @brief Returns if the target specified in arg is visible and targetable 
     * @param target A pointer to game structure of the target.
     * @return If the player is able to be targeted
    **/
    static bool ValidTarget(GameData::centity_s *target);

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
     * @param oldForwardMove The last forward move specified in cmd movement after
     *
     * For some reason, after a player uses silent aimbot through passing in the 
     * custom angles, the player's movement isn't tracked correctly anymore. This
     * function solves this issue, but I shamefully admit I do not understand the math
     * in how this works. Credit to Mr.Hankey in UnKnoWnCheaTs for this routine:
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
     * off a post off UnKnoWnCheaTs as well. Credits to kokole:
     * https://www.unknowncheats.me/forum/call-of-duty-black-ops/99319-perfect-spread.html
    **/
    void RemoveSpread(GameData::playerState_s *ps, GameData::usercmd_s *cmd) const;
};
