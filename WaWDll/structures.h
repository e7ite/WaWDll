#pragma once

using QWORD = unsigned __int64;

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
    int contentIndex;                               //0x00
    float bias;                                     //0x04
    int realTime;                                   //0x08
    int frameTime;                                  //0x0C
    float cursorPos[2];                             //0x10
    int isCursorVisible;                            //0x18
    int screenDimensions[2];                        //0x1C
    float screenAspect;                             //0x24
    float fps;                                      //0x28
}; //Size = 0x2C

struct Font_s
{
    const char *fontName;                           //0x00
    int pixelHeight;                                //0x04
    int glyphCount;                                 //0x08
    struct Material *material;                      //0x0C
    struct Material *glowMaterial;                  //0x10
    struct Glyph *glyphs;                           //0x14
}; //Size = 0x18

struct ScreenPlacement
{
    float scaleVirtualToReal[2];                    //0x00
    float scaleVirtualToFull[2];                    //0x08
    float scaleRealToVirtual[2];                    //0x10
    float virtualViewableMin[2];                    //0x18
    float virtualViewableMax[2];                    //0x20
    float virtualTweakableMin[2];                   //0x28
    float virtualTweakableMax[2];                   //0x30
    float realViewportBase[2];                      //0x38
    float realViewportSize[2];                      //0x40
    float realViewportMid[2];                       //0x48
    float realViewableMin[2];                       //0x50
    float realViewableMax[2];                       //0x58
    float realTweakableMin[2];                      //0x60
    float realTweakableMax[2];                      //0x68
    float subScreen[2];                             //0x70
}; //Size = 0x78

struct KeyState
{
    int down;                                       //0x00
    int repeats;                                    //0x04
    const char *binding;                            //0x08
    const char *binding2;                           //0x0C
}; //Size = 0x10

struct LerpEntityState
{
    int eFlags;                                     //0x000
    int eType;                                      //0x004
    char pad00[0x84];                               //0x008
}; //Size = 0x8C

struct entityState_s
{
    int number;                                     //0x000
    LerpEntityState lerp;                           //0x004
    char pad00[0x1C];                               //0x090
    int clientNum;                                  //0x0AC
    char pad01[0x68];                               //0x0B0
}; //Size = 0x118

struct playerState_s
{
    int commandTime;                                //0x0000
    int pm_type;                                    //0x0004
    int bobCycle;                                   //0x0008
    int pm_flags;                                   //0x000C
    int weapFlags;                                  //0x0010
    int otherFlags;                                 //0x0014
    int pm_time;                                    //0x0018
    int loopSoundFade;                              //0x001C
    float origin[3];                                //0x0020
    float velocity[3];                              //0x002C
    char pad00[0x44];                               //0x0038
    float delta_angles[3];                          //0x007C
    char pad01[0x70];                               //0x0088
    int clientNum;                                  //0x00F8
    int offHandIndex;                               //0x00FC
    int renderOptions;                              //0x0100
    int weapon;                                     //0x0104
    char pad02[0x8];                                //0x0108
    float fWeaponPosFrac;                           //0x0110
    char pad03[0x4];                                //0x0114
    int spreadOverride;                             //0x0118
    int spreadOverrideState;                        //0x011C
    int viewmodelIndex;                             //0x0120
    float viewangles[3];                            //0x0124
    char pad04[0x34];                               //0x0130
    int health;                                     //0x0164
    char pad05[0x4];                                //0x0168
    int maxHealth;                                  //0x016C
    char pad06[0x7A4];                              //0x0170
    float aimSpreadScale;                           //0x0914
    char pad07[0x1794];                             //0x0918
}; //Size = 0x20AC

struct cpose_t
{
    unsigned __int16 lightningHandle;               //0x00
    char eType;                                     //0x02
    char eTypeUnion;                                //0x03
    char localClientNum;                            //0x04
    char isRagdoll;                                 //0x05
    char pad00[0x2];                                //0x06
    int ragdollHandle;                              //0x08
    char pad[0x18];                                 //0x0C
    float origin[3];                                //0x24
    float angles[3];                                //0x30    
    char pad01[0x8];                                //0x3C
}; //Size = 0x44

struct centity_s
{
    cpose_t pose;                                   //0x000
    LerpEntityState lerp;                           //0x044
    entityState_s nextState;                        //0x0D0
    char pad00[0xE8];                               //0x1E8
    int alive;                                      //0x2D0
}; //Size = 0x2D4

struct refdef_s
{
    unsigned int x;                                 //0x0000
    unsigned int y;                                 //0x0004
    unsigned int width;                             //0x0008
    unsigned int height;                            //0x000C
    float tanHalfFovX;                              //0x0010
    float tanHalfFovY;                              //0x0014
    float fov_x;                                    //0x0018
    float vieworg[3];                               //0x001C
    float yaw;                                      //0x0028
    float viewaxis[3][3];                           //0x002C
    int time;                                       //0x0050
    float zNear;                                    //0x0054
    float zFar;                                     //0x0058
    char pad00[0x4288];                             //0x005C
}; //Size = 0x42E4

struct clientInfo_t
{
    int infoValid;                                  //0x000
    int nextValid;                                  //0x004
    int clientNum;                                  //0x008
    char name[0x20];                                //0x00C
    int team;                                       //0x02C
    int oldteam;                                    //0x030
    int ffateam;                                    //0x034
    char pad00[0x20];                               //0x038
    char model[0x40];                               //0x058
    char attachModelNames[0x6][0x40];               //0x098
    char attachTagNames[0x6][0x40];                 //0x218
    char pad01[0x68];                               //0x398
    float playerAngles[3];                          //0x400
    char pad02[0x188];                              //0x40C
}; //Size = 0x594

struct snapshot_s
{
    int snapFlags;                                  //0x00000
    int ping;                                       //0x00004
    int serverTime;                                 //0x00008
    playerState_s ps;                               //0x0000C
    int numEntities;                                //0x020B8
    int numClients;                                 //0x020BC
    char pad00[0xC40C];                             //0x020C0
    entityState_s entities[0x200];                  //0x0E4CC
}; //Size = 0x250C0

struct cg_s
{
    int clientNum;                                  //0x000000
    char pad00[0x20];                               //0x000004
    snapshot_s *snap;                               //0x000024
    snapshot_s *nextSnap;                           //0x000028
    char pad01[0xAAC50];                            //0x00002C
    int time;                                       //0x0AAC7C
    int oldTime;                                    //0x0AAC80
    int physicsTime;                                //0x0AAC84
    int mapRestart;                                 //0x0AAC88
    char pad02[0x4];                                //0x0AAC8C
    int renderingThirdPerson;                       //0x0AAC90
    char pad03[0x4];                                //0x0AAC94
    playerState_s predictedPlayerState;             //0x0AAC98
    char pad04[0x33C];                              //0x0ACD44
    refdef_s refdef;                                //0x0AD080
    char pad05[0x6F68];                             //0x0B1364
    int weaponSelect;                               //0x0B82CC
    char pad06[0xD4];                               //0x0B82D0
    float gunPitch;                                 //0x0B83A4
    float gunYaw;                                   //0x0B83A8
    char pad07[0x8C7E4];                            //0x0B83AC
    clientInfo_t clients[0x12];                     //0x144B90
    char pad08[0x10548];                            //0x14AFF8
    float aimSpreadScale;                           //0x15B540
    char pad09[0x1];                                //0x15B544
}; //Size = 0x15DAB8

struct cgs_t
{
    int viewX;                                      //0x0000
    int viewY;                                      //0x0004
    int viewWidth;                                  //0x0008
    int viewHeight;                                 //0x000C
    float viewAspect;                               //0x0010
    int serverCommandSequence;                      //0x0014
    int processedSnapshotNum;                       //0x0018
    int localServer;                                //0x001C
    char gametype[0x20];                            //0x0020
    char szHostName[0x100];                         //0x0040
    int maxclients;                                 //0x0140
    char pad00[0x4];                                //0x0144
    char mapname[0x40];                             //0x0148
    char pad01[0xCBA8];                             //0x0188
}; //Size = 0xCD30

struct usercmd_s
{
    int serverTime;                                 //0x00
    int button_bits;                                //0x04
    int angles[3];                                  //0x08
    char weapon;                                    //0x14
    char offhandIndex;                              //0x15
    char forwardmove;                               //0x16
    char rightmove;                                 //0x17
    char pad00[0x20];                               //0x18
};    //Size = 0x38

struct clientActive_t
{
    int ads;                                        //0x000000
    char pad00[0xC];                                //0x000004
    playerState_s ps;                               //0x000010
    char pad01[0x2305C];                            //0x0020BC
    int parseEntitiesNum;                           //0x025118
    char pad02[0x64];                               //0x02511C
    float cgameOrigin[3];                           //0x025180
    float cgameVelocity[3];                         //0x02518C
    int cgameBobCycle;                              //0x025198
    int cgameMovementDir;                           //0x02519C
    char pad03[0x4];                                //0x0251A0        
    int cgamePredictedDataServerTime;               //0x0251A4
    float viewangles[3];                            //0x0251A8
    char pad04[0x80024];                            //0x0251B4
    usercmd_s cmds[0x80];                           //0x0A51D8
    int cmdNumber;                                  //0x0A6DD8
    char pad05[0x81254];                            //0x0A6DDC
    entityState_s parseEntities[0x800];             //0x128030
}; //Size = 0x1B4030

struct rectDef_s
{
    float x;                                        //0x00
    float y;                                        //0x04
    float w;                                        //0x08
    float h;                                        //0x0C
    int horzAlign;                                  //0x10
    int vertAlign;                                  //0x14
}; //Size = 0x18

struct WeaponDef
{
    const char *szInternalName;                     //0x000
    const char *szOverlayName;                      //0x004
    char pad00[0x13C];                              //0x008
    int weapType;                                   //0x144
    int weapClass;                                  //0x148
    int penetrateType;                              //0x14C
    int impactType;                                 //0x150
    int inventoryType;                              //0x154
    int fireType;                                   //0x158
    int clipType;                                   //0x15C
    int itemIndex;                                  //0x160
    char pad01[0x13C];                              //0x164
    struct Material *reticleSlide;                  //0x2A0
    char pad02[0x134];                              //0x2A4
    struct Material *hudIcon;                       //0x3D8
    int hudIconRatio;                               //0x3DC
    struct Material *ammoCounterIcon;               //0x3E0
    int ammoCounterIconRatio;                       //0x3E4
    char pad03[0x24];                               //0x3E8
    int shotCount;                                  //0x40C
    char pad04[0x420];                              //0x410
    float fAdsSpread;                               //0x830
    char pad05[0xEC];                               //0x834
    float fMinDamageRange;                          //0x920
}; //Size = 0x924

union DvarValue
{
    bool enabled;                                   //0x01
    int integer;                                    //0x04
    unsigned int unsignedInt;                       //0x04
    __int64 integer64;                              //0x08
    unsigned __int64 unsignedInt64;                 //0x08
    float value;                                    //0x04
    float vector[4];                                //0x10
    const char *string;                             //0x04
    char color[4];                                  //0x04
}; //Size = 0x10

struct dvar_s
{
    const char *name;                               //0x00
    const char *description;                        //0x04
    unsigned int flags;                             //0x08
    char pad00[0x4];                                //0x0C
    DvarValue current;                              //0x10
    DvarValue latched;                              //0x20
    DvarValue reset;                                //0x30
    DvarValue saved;                                //0x40
    char pad01[0x8];                                //0x50
    dvar_s *next;                                   //0x58
}; //Size = 0x5C

enum scriptInstance_t : int
{
    SCRIPTINSTANCE_SERVER = 0x0,
    SCRIPTINSTANCE_CLIENT = 0x1,
    SCRIPT_INSTANCE_MAX = 0x2,
};

struct VariableValue
{
    union VariableUnion
    {
        int intValue;
        float floatValue;
        unsigned int stringValue;
        const float *vectorValue;
        const char *codePosValue;
        unsigned int pointerValue;
        struct VariableStackBuffer *stackValue;
        unsigned int entityOffset;
    };
    VariableUnion u;                                //0x00
    int type;                                       //0x04
}; //Size = 0x08

struct function_stack_t
{
    const char *pos;                                //0x00
    unsigned int localId;                           //0x04
    unsigned int localVarCount;                     //0x08
    VariableValue *top;                             //0x0C
    VariableValue *startTop;                        //0x10
}; //Size = 0x14

struct function_frame_t
{
    function_stack_t fs;                            //0x00
    int topType;                                    //0x14
}; //Size = 0x18

struct scrVmPub_t
{
    unsigned int *localVars;                        //0x0000
    VariableValue *maxstack;                        //0x0004
    int function_count;                             //0x0008
    function_frame_t *function_frame;               //0x000C
    VariableValue *top;                             //0x0010
    bool debugCode;                                 //0x0014
    bool abort_on_error;                            //0x0015
    bool terminal_error;                            //0x0016
    char pad00[0x1];                                //0x0017
    unsigned int inparamcount;                      //0x0018
    unsigned int outparamcount;                     //0x001C
    function_frame_t function_frame_start[0x20];    //0x0020
    VariableValue stack[0x800];                     //0x0320
}; //Size = 0x4320

struct EntHandle
{
    unsigned short number;                          //0x00
    unsigned short infoIndex;                       //0x02
}; //Size = 0x4

struct entityShared_t
{
    char linked;                                    //0x00
    char bmodel;                                    //0x01
    unsigned short svFlags;                         //0x02
    int clientMask;                                 //0x04
    int broadcastTime;                              //0x08
    char pad00[0x8];                                //0x0C
    float mins[3];                                  //0x10
    float maxs[3];                                  //0x1C
    int contents;                                   //0x28
    float absmin[3];                                //0x2C
    float absmax[3];                                //0x38
    float currentOrigin[3];                         //0x44
    float currentAngles[3];                         //0x50
    EntHandle ownerNum;                             //0x5C
    int eventTime;                                  //0x60
    char pad01[0x4];                                //0x64
}; //Size = 0x68

struct clientState_s
{

};

struct clientSession_t
{
    int sessionState;                               //0x000
    int forceSpectatorClient;                       //0x004
    int killcamEntity;                              //0x008
    int killcamTargetEntity;                        //0x00C
    int archiveTime;                                //0x010
    unsigned int scriptPersId;                      //0x014
    char pad00[0x1C];                               //0x018
    int connected;                                  //0x034
    usercmd_s cmd;                                  //0x038
    usercmd_s oldcmd;                               //0x070
    int localClient;                                //0x0A8
    int predictedItemPickup;                        //0x0AC
    char newnetname[0x20];                          //0x0B0
    int maxHealth;                                  //0x0D0
    char pad01[0x10];                               //0x0D4
    float moveSpeedScaleMultiplier;                 //0x0E4
    int viewmodelIndex;                             //0x0E8
    int noSpectate;                                 //0x0EC
    int teamInfo;                                   //0x0F0
    clientState_s cs;                               //0x100
}; //Size = 0x18C

//TODO: Resume mapping gentity_s at 018EF1E4
struct gclient_s
{
    playerState_s ps;                               //0x0000
    clientSession_t sess;                           //0x20AC
    int spectatorClient;                            //0x2238
    int flags;                                      //0x223C
}; //Size = 0x2348

struct actor_s
{
    struct gentity_s *gent;
    struct sentient_s *sentient;
}; //Size = 0x378

struct gentity_s
{
    entityState_s s;                                //0x000
    entityShared_t r;                               //0x118
    gclient_s *client;                              //0x180
    actor_s *actor;                                 //0x184
    struct sentient_s *sentient;                    //0x188
    char pad00[0xC];                                //0x18C
    unsigned short model;                           //0x198
    char pad01[0x142];                              //0x19A
    unsigned short attachModelNames[0x13];          //0x2DC
}; //Size = 0x378

union SoundFile
{
    struct PrimedSound
    {
        const char *directory;                          //0x00
        char *buffer;                                   //0x04
        unsigned int size;                              //0x08
    };
    struct LoadedSound
    {
        char pad00[0x4];                                //0x00
        const char *directory;                          //0x04
        const char *name;                               //0x08               
    };
    PrimedSound *primeSnd;                          //0x04
    LoadedSound loadSnd;                            //0x0C
}; //Size = 0x0C

struct snd_alias_t
{
    char pad00[0x4];                                //0x00
    SoundFile file;                                 //0x04
    char pad01[0x4];                                //0x10
    char nameBuf[0x40];                             //0x50
}; //Size = 0x90

struct snd_alias_list_t
{
    const char *name;                               //0x00
    unsigned int id;                                //0x04
    char pad00[0xC];                                //0x08
    snd_alias_t *head;                              //0x14
}; //Size = 0x18

struct SndBank
{
    const char *name;                               //0x00
    snd_alias_list_t *alias;                        //0x04
    unsigned int aliasCount;                        //0x08
}; //Size = 0x0C

enum XAssetType
{
    ASSET_TYPE_SOUND = 0x9,
};

union XAssetHeader
{
    template <typename T, int N>
    struct XAssetPool
    {
        T *freeHead;
        T entries[N];
    };
    XAssetPool<SndBank, 0x3E80> sound;
};

struct XAsset
{
    const char *name;                               //0x00
    XAssetType type;                                //0x04
    int assets;                                     //0x08
    XAssetHeader *header;                           //0x0C
}; //Size = 0x10

struct snd_buffer
{
    char *data;                                     //0x000
    int reference_count;                            //0x004
    char filename[0x104];                           //0x008
    unsigned int offset_in_file;                    //0x10C
    unsigned int data_size;                         //0x110
    unsigned int file_size;                         //0x114
};

struct snd_asset
{
    char pad00[0xC];                                //0x000
    unsigned int header_size;                       //0x00C
    unsigned int block_size;                        //0x010
    unsigned int buffer_size;                       //0x014
    char pad01[0x8];                                //0x018
    char data[0x830];                               //0x020
}; //Size = 0x850

struct snd_stream
{
    char filename[0x104];                           //0x000
    snd_asset header;                               //0x104
    char pad00[0x4];                                //0x954
};

struct snd_stream_request
{
    char filename[0x104];                           //0x000
    unsigned int start_of_read;                     //0x104
    int need;                                       //0x108
    snd_buffer *buffer;                             //0x10C
}; //Size = 0x110

extern UiContext *dc;
extern ScreenPlacement *scrPlace;
extern KeyState *keys;
extern playerState_s *ps;
extern centity_s *cg_entitiesArray;
extern cg_s *cgameGlob;
extern clientActive_t *clientActive;
extern WORD *clientObjMap;
extern BYTE *objBuf;
extern int *cl_connectionState;
extern HWND *hwnd;
extern scrVmPub_t *gScrVmPub;
extern gentity_s *g_entities;

enum FuncAddresses : DWORD
{
    R_RegisterFont_a                    = 0x6E8D84,
    Material_RegisterHandle_a           = 0x6E9C00,
    CG_DrawRotatedPic_a                 = 0x43E570,
    Cmd_ExecuteSingleCommmand_a         = 0x594ED0,
    Dvar_FindVar_a                      = 0x5EDE30,
    CL_DrawTextPhysical_a               = 0x6F5F10,
    UI_TextWidth_a                      = 0x5B5EC0,
    CG_GameMessage_a                    = 0x661910,
    CG_GetPlayerWeapon_a                = 0x46BF20,
    WorldPosToScreenPos_a               = 0x44CD30,
    vectoangles_a                       = 0x5DF960,
    UI_GetFontHandle_a                  = 0x5B6400,
    UI_DrawText_a                       = 0x5B5FB0,
    SL_FindString_a                     = 0x68DA90,
    ScrPlace_ApplyRect_a                = 0x47A450,
    R_TextWidth_a                       = 0x6E8DA0,
    R_EndFrame_a                        = 0x6F6A80,
    CG_DObjGetWorldTagPos_a             = 0x443030,
    FX_ClientVisibilityTest_a           = 0x4AF130,
    Key_StringToKeynum_a                = 0x477540,
    CG_TraceCapsule_a                   = 0x46D5B0,
    CG_DrawRotatedPicPhysical_a         = 0x43E3C0,
    AimTarget_IsTargetVisible_a         = 0x403CA0,
    BG_GetSpreadForWeapon_a             = 0x41DB20,
    AngleVectors_a                      = 0x5E3150,
    DrawSketchPicGun_a                  = 0x42CC30,
    CG_GetPlayerViewOrigin_a            = 0x468A00,
    __libm_sse2_tan_a                   = 0x7E0892,
    RandomBulletDir_a                   = 0x4E54C0,
    Vec3Normalize_a                     = 0x4037C0,
    UI_DrawRect_a                       = 0x5B5BD0,
    UI_FillRect_a                       = 0x5B08E0,
    MessageBoxA_a                       = 0x7EB33C,
    timeGetTime_a                       = 0x7EB39C,
    Cbuf_AddText_a                      = 0x594200,
    FindVariableIndexInternal_a         = 0x68BC20,
    FindLastSibling_a                   = 0x68BCA0,
    Scr_AddFloat_a                      = 0x69A670,
    Scr_AddInt_a                        = 0x42A2B0,
    Scr_AddString_a                     = 0x69A7E0,
    Scr_AddVector_a                     = 0x69A940,
    DB_FindXAssetEntry_a                = 0x48D760,
    FS_WriteFile_a                      = 0x5DC050,
    FS_ReadFile_a                       = 0x5DBFB0,
    SND_FindAlias_a                     = 0x63B560,
    FileWrapper_Open_a                  = 0x7AC7B1,
    UI_PlaySound_a                      = 0x5CAC10,
    SND_Play_a                          = 0x6B2530,
    Com_Error_a                         = 0x59AC50,
    Snd_FindBuffer_a                    = 0x6BE4D0,
    Snd_StreamGetRequest_a              = 0x6BE1A0,
    va_a                                = 0x5F6D80,
    FS_FreeMem_a                        = 0x5E4580,
    Snd_LoadBuffer_a                    = 0x6BE530,
};

namespace Colors
{
    struct Color
    {
        float r, g, b, a;

        Color() : r(0.0F), g(0.0f), b(0.0f), a(1.0f) {}
        Color(float r, float g, float b, float a)
            : r(r / 255.0f), g(g / 255.0f), b(b / 255.0f), a(a / 255.0f) {}

        operator float*() { return reinterpret_cast<float*>(this); }
    };

    extern Color white;
    extern Color black;
    extern Color red;
    extern Color green;
    extern Color blue;
    extern Color transparentBlack;
}

namespace GameData
{
    struct Font
    {
        int index;
        const char *dir;
    };

    extern Font normalFont;
    extern std::vector<QWORD> detours;
    extern std::map<const char*, dvar_s*> dvars;
    extern bool dvarsInitialized;
    extern bool sndsInitialized;

    extern int(__stdcall *MessageBoxA)(HWND hWnd, LPCSTR lpText,
        LPCSTR lpCaption, UINT uType);
    extern DWORD(__stdcall *timeGetTime)();

    bool InsertDvar(const char *dvarName, dvar_s *dvar = nullptr);
}

//Game Function Pointers
extern Font_s*(__cdecl *R_RegisterFont)(const char *font, int imageTrac);
extern void*(__cdecl *Material_RegisterHandle)(const char *materialName,
    int imageTrac);
extern void(__cdecl *CG_DrawRotatedPic)(ScreenPlacement *scrPlace, float x, float y,
    float width, float height, float angle, const float *color, void *material);
extern void(__cdecl *Cmd_ExecuteSingleCommand)(int localClientNum,
    int controllerIndex, const char *text);
extern dvar_s*(__cdecl *Dvar_FindVar)(const char *dvarName);
extern void(__cdecl *CL_DrawTextPhysicalInternal)(const char *text, int maxChars,
    void *font, float x, float y, float xScale, float yScale, float rotation,
    int style);
extern int(__cdecl *UI_TextWidthInternal)(const char *text, int maxChars,
    void *font, float scale);
extern void(__cdecl *CG_GameMessage)(int localClientNum, const char *msg, int length);
extern int(__cdecl *CG_GetPlayerWeapon)(playerState_s *ps, int localClientNum);
extern void(__cdecl *RandomBulletDir)(int randSeed, float *x, float *y);
extern _iobuf*(__cdecl *FileWrapper_Open)(const char *filename, const char *mode);
extern void(__cdecl *Com_Error)(int code, const char *fmt, ...);
extern snd_buffer*(__cdecl *Snd_FindBuffer)(const char *filename, unsigned int offset);
extern char* (__cdecl *va)(const char *fmt, ...);

//Custom Functions
bool InGame();
bool CopyTextToClipboard(const std::string &text);

//Rebuilt Game Functions
void Cbuf_AddText(const char *cmd);
void CG_DrawRotatedPicPhysical(ScreenPlacement *scrPlace, float x, float y,
    float width, float height, float angle, const float *color, void *material);
int Key_StringToKeynum(const char *name);
bool Key_IsDown(const char *bind);
float R_NormalizedTextScale(Font_s *font, float scale);
float UI_TextHeight(Font_s *font, float scale);
float R_TextHeight(Font_s *font);
int AimTarget_GetTagPos(int localClientNum, centity_s *cent,
    unsigned __int16 tagname, float *pos);
unsigned __int16 SL_FindString(const char *tagname);
bool WorldPosToScreenPos(int localClientNum, const float *world, float pos[2]);
void vectoangles(const float *vec, float *pos);
void CL_GetUserCmd(int cmdNum, usercmd_s *cmd);
usercmd_s* CL_GetUserCmd(int cmdNum);
void CL_DrawTextPhysical(const char *text, int maxChars,
    void *font, float x, float y, float xScale, float yScale,
    float rotation, const float *color, int style);
void ScrPlace_ApplyRect(ScreenPlacement *scrPlace,
    float *x, float *y, float *w, float *h, int horzAlign, int vertAlign);
void UI_DrawText(ScreenPlacement *scrPlace, const char *text,
    int maxChars, void *font, float x, float y, float scale,
    float angle, const float *color, int style);
Font_s* UI_GetFontHandle(ScreenPlacement *scrPlace, int fontEnum);
float UI_TextWidth(const char *text, int maxChars,
    Font_s *font, float scale);
float R_TextWidth(const char *text, int maxChars, Font_s *font);
int Sys_Milliseconds();
WeaponDef* BG_GetWeaponDef(int weapon);
void BG_GetSpreadForWeapon(playerState_s *ps, WeaponDef *weap, float *minSpread,
    float *maxSpread);
void AngleVectors(const float *angles, float *forward, float *right, float *up);
void DrawSketchPicGun(ScreenPlacement *scrPlace, rectDef_s *rect,
    const float *color, struct Material *material, int ratio);
bool CG_GetPlayerViewOrigin(int localClientNum, playerState_s *ps, float out[3]);
float __libm_sse2_tan(float x);
void Vec3Normalize(float *x);
void UI_FillRect(ScreenPlacement *scrPlace, float x, float y, float width,
    float height, int horzAlign, int veryAlign, const float *color);
void UI_DrawRect(ScreenPlacement *scrPlace, float x, float y, float width,
    float height, int horzAlign, int vertAlign, float size, const float *color); 
bool AimTarget_IsTargetVisible(centity_s *cent, unsigned __int16 bone);
bool IN_IsForegroundWindow();
const char* SL_ConvertToString(int stringValue);
unsigned int FindVariable(scriptInstance_t inst,
    unsigned int parentId, unsigned int unsignedValue);
unsigned int FindObject(scriptInstance_t inst, unsigned int id);
unsigned int Scr_GetSelf(scriptInstance_t inst, unsigned int threadId);
unsigned int FindLastSibling(scriptInstance_t inst, unsigned int id);
int GetVariableKeyObject(scriptInstance_t inst, unsigned int id);
void Scr_SetParameters(int count);
void Scr_AddFloat(float value);
void Scr_AddInt(int value);
void Scr_AddString(const char *string);
void Scr_AddVector(const float *value);
int FS_WriteFile(const char *filename, const void *buffer, int size);
int FS_ReadFile(const char *qpath, void **buffer);
XAsset DB_FindXAsset(XAssetType type);
snd_alias_list_t* SND_FindAlias(int localClientNum, const char *name);
void UI_PlaySound(int context, const char *aliasname);
int SND_Play(const char *alias, int entIndex, float volume);
void FS_FreeFile(void *buffer);
void Snd_StreamGetRequest(snd_stream *s, snd_stream_request *r);