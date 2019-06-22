#include "esp.h"

float Distance3D(vec3_t c1, vec3_t c2)
{
	float dx = c2.x - c1.x;
	float dy = c2.y - c1.y;
	float dz = c2.z - c1.z;

	return sqrtf((float)((dx * dx) + (dy * dy) + (dz * dz)));
}

void DrawBorderBox(float head[2], float foot[2], const float *color)
{
	float boxHeight = (foot[1] - head[1]), boxWidth = boxHeight / 2.2f;

	CG_DrawRotatedPicPhysical(scrPlace,
		(foot[0] - (boxWidth / 2)), (foot[1] - boxHeight), boxWidth, 1, 0,
		color, Material_RegisterHandle("white", 0)); //top
	CG_DrawRotatedPicPhysical(scrPlace,
		(foot[0] - (boxWidth / 2)), foot[1], boxWidth, 1, 0, color,
		Material_RegisterHandle("white", 0)); //bottom
	CG_DrawRotatedPicPhysical(scrPlace,
		(foot[0] - (boxWidth / 2)), (foot[1] - boxHeight),
		1, boxHeight, 0, color, Material_RegisterHandle("white", 0)); //left
	CG_DrawRotatedPicPhysical(scrPlace, 
		(foot[0] + (boxWidth / 2)), (foot[1] - boxHeight), 1,
		boxHeight, 0, color, Material_RegisterHandle("white", 0)); //right
}

void RenderESP()
{
	if (GameData::dvarGlob["cl_ingame"]->current.enabled)
	{
		vec3_t head, foot;
		float headScreen[2], feetScreen[2];
		const float *color = Colors::blue;
		float closest = static_cast<float>(INT_MAX);

		for (__int32 i = 0; i < 1024; ++i)
		{
			if (ValidTarget(&cg_entitiesArray[i]))
			{
				AimTarget_GetTagPos(0, 
					&cg_entitiesArray[i],
					SL_FindString("j_head"), head);
				foot = cg_entitiesArray[i].pose.origin;

				WorldPosToScreenPos(0, head, headScreen);
				WorldPosToScreenPos(0, foot, feetScreen);

				CG_DrawRotatedPicPhysical(scrPlace, headScreen[0], headScreen[1],
					20, 20, 0, Colors::white, Material_RegisterHandle("white", 0));

				if (AimTarget_IsTargetVisible(
					&cg_entitiesArray[i],
					SL_FindString("j_head")))
					color = Colors::red;

				DrawBorderBox(headScreen, feetScreen, color);
			}
		}
	}
}