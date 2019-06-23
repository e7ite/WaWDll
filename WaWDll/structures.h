#pragma once

#include <Windows.h>
#include <detours.h>
#include <iostream>
#include <cstdio>
#include <functional>
#include <string>
#include <sstream>
#include <map>
#include <vector>

struct vec3_t
{
	union
	{
		struct { float x, y, z; };
		struct { float pitch, yaw, roll; };
	};

	vec3_t(float x = 0.0f, float y = 0.0f, float z = 0.0f)
		: x(x), y(y), z(z) {}
	vec3_t(float *vec) { memcpy(this, vec, 0xC); }
	vec3_t(const vec3_t &vec) { memcpy(this, &vec, 0xC); }

	operator float*() { return (float*)this; }

	vec3_t operator+(const vec3_t &vec) const;
	vec3_t operator+(float vec[3]) const;
	vec3_t operator-(const vec3_t &vec) const;
	vec3_t operator-(float vec[3]) const;
	vec3_t& operator+=(const vec3_t &vec);
	vec3_t& operator+=(float vec[3]);
	vec3_t& operator-=(const vec3_t &vec);
	vec3_t& operator-=(float vec[3]);
};

struct UiContext
{
	int contentIndex;						//0x00
	float bias;								//0x04
	int realTime;							//0x08
	int frameTime;							//0x0C
	float cursorPos[2];						//0x10
	int isCursorVisible;					//0x18
	int screenDimensions[2];				//0x1C
	float screenAspect;						//0x24
	float fps;								//0x28
}; //Size = 0x2C

struct Font_s
{
	const char *fontName;					//0x00
	int pixelHeight;						//0x04
	int glyphCount;							//0x08
	struct Material *material;				//0x0C
	struct Material *glowMaterial;			//0x10
	struct Glyph *glyphs;					//0x14
}; //Size = 0x18

struct ScreenPlacement
{
	float scaleVirtualToReal[2];			//0x00
	float scaleVirtualToFull[2];			//0x08
	float scaleRealToVirtual[2];			//0x10
	float virtualViewableMin[2];			//0x18
	float virtualViewableMax[2];			//0x20
	float virtualTweakableMin[2];			//0x28
	float virtualTweakableMax[2];			//0x30
	float realViewportBase[2];				//0x38
	float realViewportSize[2];				//0x40
	float realViewportMid[2];				//0x48
	float realViewableMin[2];				//0x50
	float realViewableMax[2];				//0x58
	float realTweakableMin[2];				//0x60
	float realTweakableMax[2];				//0x68
	float subScreen[2];						//0x70
}; //Size = 0x78

struct KeyState
{
	int down;								//0x00
	int repeats;							//0x04
	const char *binding;					//0x08
	const char *binding2;					//0x0C
}; //Size = 0x10

struct LerpEntityState
{
	int eFlags;								//0x000
	int eType;								//0x004
	char pad00[0x84];						//0x008
}; //Size = 0x8C

struct entityState_s
{
	int number;								//0x000
	LerpEntityState lerp;					//0x004
	char pad00[0x88];						//0x090
}; //Size = 0x118

struct playerState_s
{
	int commandTime;						//0x0000
	int pm_type;							//0x0004
	int bobCycle;							//0x0008
	int pm_flags;							//0x000C
	int weapFlags;							//0x0010
	int otherFlags;							//0x0014
	int pm_time;							//0x0018
	int loopSoundFade;						//0x001C
	float origin[3];						//0x0020
	float velocity[3];						//0x002C
	char pad00[0x44];						//0x0038
	float delta_angles[3];					//0x007C
	char pad01[0x70];						//0x0088
	int clientNum;							//0x00F8
	int offHandIndex;						//0x00FC
	int renderOptions;						//0x0100
	int weapon;								//0x0104
	char pad02[0x8];						//0x0108
	float fWeaponPosFrac;					//0x0110
	char pad03[0x4];						//0x0114
	int spreadOverride;						//0x0118
	int spreadOverrideState;				//0x011C
	int viewmodelIndex;						//0x0120
	float viewangles[3];					//0x0124
	char pad04[0x34];						//0x0130
	int health;								//0x0164
	char pad05[0x4];						//0x0168
	int maxHealth;							//0x016C
	char pad06[0x7A4];						//0x0170
	float aimSpreadScale;					//0x0914
	char pad07[0x1794];						//0x0918
}; //Size = 0x20AC

struct cpose_t
{
	unsigned __int16 lightningHandle;		//0x00
	char eType;								//0x02
	char eTypeUnion;						//0x03
	char localClientNum;					//0x04
	char isRagdoll;							//0x05
	char pad00[0x2];						//0x06
	int ragdollHandle;						//0x08
	char pad[0x18];							//0x0C
	float origin[3];						//0x24
	float angles[3];						//0x30	
	char pad01[0x8];						//0x3C
}; //Size = 0x44

struct centity_s
{
	cpose_t pose;							//0x000
	LerpEntityState lerp;					//0x044
	entityState_s nextState;				//0x0D0
	char pad00[0xE8];						//0x1E8
	int alive;								//0x2D0
}; //Size = 0x2D4

struct refdef_s
{
	unsigned int x;							//0x0000
	unsigned int y;							//0x0004
	unsigned int width;						//0x0008
	unsigned int height;					//0x000C
	float tanHalfFovX;						//0x0010
	float tanHalfFovY;						//0x0014
	float fov_x;							//0x0018
	float vieworg[3];						//0x001C
	float yaw;								//0x0028
	float viewaxis[3][3];					//0x002C
	int time;								//0x0050
	float zNear;							//0x0054
	float zFar;								//0x0058
	char pad00[0x4288];						//0x005C
}; //Size = 0x42E4

struct clientInfo_t
{
	int infoValid;							//0x000
	int nextValid;							//0x004
	int clientNum;							//0x008
	char name[0x20];						//0x00C
	int team;								//0x02C
	int oldteam;							//0x030
	int ffateam;							//0x034
	char pad00[0x1C];						//0x038
	char model[0x40];						//0x054
	char attachModelNames[6][0x40];			//0x094
	char attachTagNames[6][0x40];			//0x214
	char pad01[0x6C];						//0x394
	float playerAngles[3];					//0x400
	char pad02[0x188];						//0x40C
}; //Size = 0x594

struct snapshot_s
{
	int snapFlags;							//0x00000
	int ping;								//0x00004
	int serverTime;							//0x00008
	playerState_s ps;						//0x0000C
	int numEntities;						//0x020B8
	int numClients;							//0x020BC
	char pad00[0xC40C];						//0x020C0
	entityState_s entities[0x200];			//0x0E4CC
}; //Size = 0x250C0

struct cg_s
{
	int clientNum;							//0x000000
	char pad00[0x20];						//0x000004
	snapshot_s *snap;						//0x000024
	snapshot_s *nextSnap;					//0x000028
	char pad01[0xAAC50];					//0x00002C
	int time;								//0x0AAC7C
	int oldTime;							//0x0AAC80
	int physicsTime;						//0x0AAC84
	int mapRestart;							//0x0AAC88
	char pad02[0x4];						//0x0AAC8C
	int renderingThirdPerson;				//0x0AAC90
	char pad03[0x4];						//0x0AAC94
	playerState_s predictedPlayerState;		//0x0AAC98
	char pad04[0x33C];						//0x0ACD44
	refdef_s refdef;						//0x0AD080
	char pad05[0x6F68];						//0x0B1364
	int weaponSelect;						//0x0B82CC
	char pad06[0xD4];						//0x0B82D0
	float gunPitch;							//0x0B83A4
	float gunYaw;							//0x0B83A8
	char pad07[0x8C7E4];					//0x0B83AC
	clientInfo_t clients[0x12];				//0x144B90
	char pad08[0x10548];					//0x14AFF8
	float aimSpreadScale;					//0x15B540
	char pad09[0x1];						//0x15B544
}; //Size = 0x15DAB8

struct cgs_t
{
	int viewX;								//0x0000
	int viewY;								//0x0004
	int viewWidth;							//0x0008
	int viewHeight;							//0x000C
	float viewAspect;						//0x0010
	int serverCommandSequence;				//0x0014
	int processedSnapshotNum;				//0x0018
	int localServer;						//0x001C
	char gametype[0x20];					//0x0020
	char szHostName[0x100];					//0x0040
	int maxclients;							//0x0140
	char pad00[0x4];						//0x0144
	char mapname[0x40];						//0x0148
	char pad01[0xCBA8];						//0x0188
}; //Size = 0xCD30

struct usercmd_s
{
	int serverTime;							//0x00
	int button_bits;						//0x04
	int angles[3];							//0x08
	char weapon;							//0x14
	char offhandIndex;						//0x15
	char forwardmove;						//0x16
	char rightmove;							//0x17
	char pad00[0x20];						//0x18
};	//Size = 0x38

struct clientActive_t
{
	int ads;								//0x000000
	char pad00[0xC];						//0x000004
	playerState_s ps;						//0x000010
	char pad01[0x2305C];					//0x0020BC
	int parseEntitiesNum;					//0x025118
	char pad02[0x64];						//0x02511C
	float cgameOrigin[3];					//0x025180
	float cgameVelocity[3];					//0x02518C
	int cgameBobCycle;						//0x025198
	int cgameMovementDir;					//0x02519C
	char pad03[0x4];						//0x0251A0		
	int cgamePredictedDataServerTime;		//0x0251A4
	float viewangles[3];					//0x0251A8
	char pad04[0x80024];					//0x0251B4
	usercmd_s cmds[0x80];					//0x0A51D8
	int cmdNumber;							//0x0A6DD8
	char pad05[0x81254];					//0x0A6DDC
	entityState_s parseEntities[0x800];		//0x128030
}; //Size = 0x1B4030

struct actor_s
{
	struct gentity_s *gent;
	struct sentient_s *sentient;
};	//Size = 0x378

struct trace_t
{
	
};

struct rectDef_s
{
	float x;								//0x00
	float y;								//0x04
	float w;								//0x08
	float h;								//0x0C
	int horzAlign;							//0x10
	int vertAlign;							//0x14
}; //Size = 0x18

struct WeaponDef
{
	const char *szInternalName;				//0x000
	const char *szOverlayName;				//0x004
	char pad00[0x13C];						//0x008
	int weapType;							//0x144
	int weapClass;							//0x148
	int penetrateType;						//0x14C
	int impactType;							//0x150
	int inventoryType;						//0x154
	int fireType;							//0x158
	int clipType;							//0x15C
	int itemIndex;							//0x160
	char pad01[0x13C];						//0x164
	struct Material *reticleSlide;			//0x2A0
	char pad02[0x134];						//0x2A4
	struct Material *hudIcon;				//0x3D8
	int hudIconRatio;						//0x3DC
	struct Material *ammoCounterIcon;		//0x3E0
	int	ammoCounterIconRatio;				//0x3E4
	char pad03[0x24];						//0x3E8
	int shotCount;							//0x40C
	char pad04[0x420];						//0x410
	float fAdsSpread;						//0x830
	char pad05[0xEC];						//0x834
	float fMinDamageRange;					//0x920
};

union DvarValue
{
	bool enabled;							//0x01
	int integer;							//0x04
	unsigned int unsignedInt;				//0x04
	__int64 integer64;						//0x08
	unsigned __int64 unsignedInt64;			//0x08
	float value;							//0x04
	float vector[4];						//0x10
	const char *string;						//0x04
	char color[4];							//0x04
}; //Size = 0x10

struct dvar_s
{
	const char *name;						//0x00
	const char *description;				//0x04
	unsigned int flags;						//0x08
	char pad00[0x4];						//0x0C
	DvarValue current;						//0x10
	DvarValue latched;						//0x20
	DvarValue reset;						//0x30
	DvarValue saved;						//0x40
	char pad01[0x8];						//0x50
	dvar_s *next;							//0x58
}; //Size = 0x5C

extern UiContext *dc;
extern ScreenPlacement *scrPlace;
extern KeyState *keys;
extern playerState_s *ps;
extern centity_s *cg_entitiesArray;
extern cg_s *cgameGlob;
extern clientActive_t *clientActive;
extern WORD *clientObjMap;
extern BYTE *objBuf;

enum FuncAddresses : DWORD
{
	R_RegisterFont_a					= 0x6E8D84,
	Material_RegisterHandle_a			= 0x6E9C00,
	CG_DrawRotatedPic_a					= 0x43E570,
	Cmd_ExecuteSingleCommmand_a			= 0x594ED0,
	Dvar_FindVar_a						= 0x5EDE30,
	CL_DrawTextPhysical_a				= 0x6F5F10,
	UI_TextWidth_a						= 0x5B5EC0,
	CG_GameMessage_a					= 0x661910,
	CG_GetPlayerWeapon_a				= 0x46BF20,
	WorldPosToScreenPos_a				= 0x44CD30,
	vectoangles_a						= 0x5DF960,
	UI_GetFontHandle_a					= 0x5B6400,
	UI_DrawText_a						= 0x5B5FB0,
	SL_FindString_a						= 0x68DA90,
	ScrPlace_ApplyRect_a				= 0x47A450,
	R_TextWidth_a						= 0x6E8DA0,
	R_EndFrame_a						= 0x6F6A80,
	CG_DObjGetWorldTagPos_a				= 0x443030,
	FX_ClientVisibilityTest_a			= 0x4AF130,
	Key_StringToKeynum_a				= 0x477540,
	CG_TraceCapsule_a					= 0x46D5B0,
	CG_DrawRotatedPicPhysical_a			= 0x43E3C0,
	AimTarget_IsTargetVisible_a			= 0x403CA0,
	BG_GetSpreadForWeapon_a				= 0x41DB20,
	AngleVectors_a						= 0x5E3150,
	DrawSketchPicGun_a					= 0x42CC30,
	CG_GetPlayerViewOrigin_a			= 0x468A00,
	__libm_sse2_tan_a					= 0x7E0892,
	RandomBulletDir_a					= 0x4E54C0,
	Vec3Normalize_a						= 0x4037C0,
};

extern void*(__cdecl *R_RegisterFont)(const char *font, __int32 imageTrac);
extern void*(__cdecl *Material_RegisterHandle)(const char *materialName,
	__int32 imageTrac);
extern void(__cdecl *CG_DrawRotatedPic)(ScreenPlacement *scrPlace, float x, float y,
	float width, float height, float angle, const float *color, void *material);
extern void(__cdecl *Cmd_ExecuteSingleCommand)(__int32 localClientNum,
	__int32 controllerIndex, const char *text);
extern dvar_s*(__cdecl *Dvar_FindVar)(const char *dvarName);
extern void(__cdecl *CL_DrawTextPhysicalInternal)(const char *text, int maxChars,
	void *font, float x, float y, float xScale, float yScale, float rotation,
	int style);
extern __int32(__cdecl *UI_TextWidthInternal)(const char *text, __int32 maxChars,
	void *font, float scale);
extern void(__cdecl *CG_GameMessage)(int localClientNum, const char *msg, int length);
extern int(__cdecl *CG_GetPlayerWeapon)(playerState_s *ps, int localClientNum);
extern void(__cdecl *RandomBulletDir)(int randSeed, float *x, float *y);

void CG_DrawRotatedPicPhysical(ScreenPlacement *scrPlace, float x, float y,
	float width, float height, float angle, const float *color, void *material);
__int32 Key_StringToKeynum(const char *name);
bool Key_IsDown(const char *bind);
float R_NormalizedTextScale(Font_s *font, float scale);
float UI_TextHeight(Font_s *font, float scale);
float R_TextHeight(Font_s *font);
__int32 AimTarget_GetTagPos(__int32 localClientNum, centity_s *cent,
	unsigned __int16 tagname, float *pos);
unsigned __int16 SL_FindString(const char *tagname);
bool WorldPosToScreenPos(int localClientNum, const float *world, float pos[2]);
void vectoangles(const float *vec, float *pos);
bool AimTarget_IsTargetVisible(int targetEntNum, unsigned __int16 bone);
void CL_GetUserCmd(int cmdNum, usercmd_s *cmd);
usercmd_s* CL_GetUserCmd(int cmdNum);
void CL_DrawTextPhysical(const char *text, int maxChars,
	void *font, float x, float y, float xScale, float yScale,
	float rotation, const float *color, int style);
void ScrPlace_ApplyRect(ScreenPlacement *scrPlace,
	float *x, float *y, float *w, float *h, int horzAlign, int vertAlign);
void UI_DrawText(ScreenPlacement *scrPlace, const char *text,
	__int32 maxChars, void *font, float x, float y, float scale,
	float angle, const float *color, __int32 style);
Font_s* UI_GetFontHandle(ScreenPlacement *scrPlace, int fontEnum);
float UI_TextWidth(const char *text, __int32 maxChars,
	Font_s *font, float scale);
int R_TextWidth(const char *text, int maxChars, Font_s *font);
int Sys_Milliseconds();
void InsertDvar(const char *dvarName);
WeaponDef* BG_GetWeaponDef(int weapon);
void BG_GetSpreadForWeapon(playerState_s *ps, WeaponDef *weap, float *minSpread,
	float *maxSpread);
void AngleVectors(const float *angles, float *forward, float *right, float *up);
void DrawSketchPicGun(ScreenPlacement *scrPlace, rectDef_s *rect,
	const float *color, struct Material *material, int ratio);
void CG_GetPlayerViewOrigin(int localClientNum, playerState_s *ps, float out[3]);
float __libm_sse2_tan(float x);
void Vec3Normalize(float *x);