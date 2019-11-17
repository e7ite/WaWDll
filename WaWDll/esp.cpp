#include "stdafx.h"

float Distance3D(vec3_t c1, vec3_t c2)
{
    float dx = c2.x - c1.x;
    float dy = c2.y - c1.y;
    float dz = c2.z - c1.z;

    return sqrtf(dx * dx + dy * dy + dz * dz);
}

void DrawBorderBox(float head[2], float foot[2], const float *color)
{
    float boxHeight = (foot[1] - head[1]);
    float boxWidth = boxHeight / 2.2f;
    float x = (foot[0] - (boxWidth / 2));
    float y = foot[1] - boxHeight;
    void *material = Material_RegisterHandle("white", 0);

    CG_DrawRotatedPicPhysical(scrPlace,
        x, y, boxWidth, 1, 0, color, material); //top
    CG_DrawRotatedPicPhysical(scrPlace,
        x, foot[1], boxWidth, 1, 0, color, material); //bottom
    CG_DrawRotatedPicPhysical(scrPlace,
        x, y, 1, boxHeight, 0, color, material); //left
    CG_DrawRotatedPicPhysical(scrPlace, 
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
            if (ValidTarget(&cg_entitiesArray[i]))
            {
                const float *color = Colors::blue;
                AimTarget_GetTagPos(0, 
                    &cg_entitiesArray[i],
                    SL_FindString("j_head"), head);
                foot = cg_entitiesArray[i].pose.origin;

                WorldPosToScreenPos(0, head, headScreen);
                WorldPosToScreenPos(0, foot, feetScreen);

                if (AimTarget_IsTargetVisible(
                        &cg_entitiesArray[i],
                        SL_FindString("j_head")))
                    color = Colors::red;

                DrawBorderBox(headScreen, feetScreen, color);
            }
        }
    }
} 
