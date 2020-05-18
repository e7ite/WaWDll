#include "stdafx.hpp"
#include "esp.hpp"

void DrawBorderBox(float head[2], float foot[2], const float *color)
{
    float boxHeight = (foot[1] - head[1]);
    float boxWidth = boxHeight / 2.2f;
    float x = (foot[0] - (boxWidth / 2));
    float y = foot[1] - boxHeight;
    void *material = GameData::Material_RegisterHandle("white", 0);

    // Draw the top line
    GameData::CG_DrawRotatedPicPhysical(GameData::scrPlace,
        x, y, boxWidth, 1, 0, color, material);
    // Draw the bottom line
    GameData::CG_DrawRotatedPicPhysical(GameData::scrPlace,
        x, foot[1], boxWidth, 1, 0, color, material);
    // Draw the left line
    GameData::CG_DrawRotatedPicPhysical(GameData::scrPlace,
        x, y, 1, boxHeight, 0, color, material);
    // Draw the right line
    GameData::CG_DrawRotatedPicPhysical(GameData::scrPlace,
        foot[0] + boxWidth / 2, y, 1, boxHeight, 0, color, material); 
}

void RenderESP()
{
    if (InGame())
    {
        vec3_t head, foot;
        float headScreen[2], feetScreen[2];
        float closest = static_cast<float>(INT_MAX);
        unsigned int id = GameData::SL_FindString("j_head");

        // Loop through all entities and find zombie entities
        for (int i = 0; i < 1024; ++i)
        {
            // Check if a zombie entity
            if (Aimbot::ValidTarget(&GameData::cg_entitiesArray[i]))
            {
                const float *color = Colors::blue;

                // Get the zombie head position
                GameData::AimTarget_GetTagPos(0, 
                   &GameData::cg_entitiesArray[i],
                    GameData::SL_FindString("j_head"), head);

                // Get the zombie foot position
                foot = GameData::cg_entitiesArray[i].pose.origin;

                // Convert the 3D positions to a 2D screen position
                GameData::CG_WorldPosToScreenPos(0, head, headScreen);
                GameData::CG_WorldPosToScreenPos(0, foot, feetScreen);

                // If the zombie head is visible, change the color to red
                if (GameData::AimTarget_IsTargetVisible(
                       &GameData::cg_entitiesArray[i], id))
                    color = Colors::red;

                // Draw the box around the zombie
                DrawBorderBox(headScreen, feetScreen, color);
            }
        }
    }
} 
