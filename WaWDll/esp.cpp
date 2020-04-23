#include "stdafx.h"
#include "esp.h"

void DrawBorderBox(float head[2], float foot[2], const float *color)
{
    float boxHeight = (foot[1] - head[1]);
    float boxWidth = boxHeight / 2.2f;
    float x = (foot[0] - (boxWidth / 2));
    float y = foot[1] - boxHeight;
    void* material = GameData::Material_RegisterHandle("white", 0);

    GameData::CG_DrawRotatedPicPhysical(GameData::scrPlace,
        x, y, boxWidth, 1, 0, color, material); //top
    GameData::CG_DrawRotatedPicPhysical(GameData::scrPlace,
        x, foot[1], boxWidth, 1, 0, color, material); //bottom
    GameData::CG_DrawRotatedPicPhysical(GameData::scrPlace,
        x, y, 1, boxHeight, 0, color, material); //left
    GameData::CG_DrawRotatedPicPhysical(GameData::scrPlace,
        foot[0] + boxWidth / 2, y, 1, boxHeight, 0, color, material); //right
}

void RenderESP()
{
    if (InGame())
    {
        vec3_t head, foot;
        float headScreen[2], feetScreen[2];
        float closest = static_cast<float>(INT_MAX);

        for (int i = 0; i < 1024; ++i)
        {
            if (Aimbot::ValidTarget(&GameData::cg_entitiesArray[i]))
            {
                const float* color = Colors::blue;
                GameData::AimTarget_GetTagPos(0, 
                   &GameData::cg_entitiesArray[i],
                    GameData::SL_FindString("j_head"), head);
                foot = GameData::cg_entitiesArray[i].pose.origin;

                GameData::CG_WorldPosToScreenPos(0, head, headScreen);
                GameData::CG_WorldPosToScreenPos(0, foot, feetScreen);

                if (GameData::AimTarget_IsTargetVisible(
                       &GameData::cg_entitiesArray[i],
                    GameData::SL_FindString("j_head")))
                    color = Colors::red;

                DrawBorderBox(headScreen, feetScreen, color);
            }
        }
    }
} 
