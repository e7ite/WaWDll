#pragma once

#include "stdafx.hpp"
#include "math.hpp"

namespace GameData
{
    enum ai_state_t : DWORD
    {
        AIS_INVALID = 0x0,
        AIS_KEEPCURRENT = 0x0,
        AIS_EXPOSED = 0x1,
        AIS_TURRET = 0x2,
        AIS_GRENADE_RESPONSE = 0x3,
        AIS_BADPLACE_FLEE = 0x4,
        AIS_COVERARRIVAL = 0x5,
        AIS_DEATH = 0x6,
        AIS_DEFAULT = 0x1,
        AIS_SETABLE_FIRST = 0x1,
        AIS_SETABLE_LAST = 0x6,
        AIS_PAIN = 0x7,
        AIS_REACT = 0x8,
        AIS_SCRIPTEDANIM = 0x9,
        AIS_CUSTOMANIM = 0xA,
        AIS_NEGOTIATION = 0xB,
        AIS_PUSHABLE_FIRST = 0x7,
        AIS_PUSHABLE_LAST = 0xB,
        AIS_COUNT = 0xC,
    };

#pragma pack(push, 1)
    struct hudelem_s
    {
        int   type;                                     // 0x00
        float x;                                        // 0x04
        float y;                                        // 0x08
        float z;                                        // 0x0C
        int   targetEntNum;                             // 0x10
        char  pad00[0x10];                              // 0x14
        int   font;                                     // 0x24
        int   alignOrg;                                 // 0x28
        int   alignScreen;                              // 0x2C
        int   rgba;                                     // 0x30
        char  pad01[0x10];                              // 0x34
        int   width;                                    // 0x44
        int   height;                                   // 0x48
        int   materialIndex;                            // 0x4C
        char  pad02[0x4];                               // 0x50
        int   fromAlignOrg;                             // 0x54
        int   fromAlignScreen;                          // 0x58
        int   fromwidth;                                // 0x5C
        int   fromheight;                               // 0x60
        char  pad03[0x8];                               // 0x64
        int   fromX;                                    // 0x6C
        int   fromY;                                    // 0x70
        char  pad04[0x8];                               // 0x74
        int   time;                                     // 0x7C
        int   duration;                                 // 0x80
        char  pad05[0x4];                               // 0x84
        int   text;                                     // 0x88
        char  pad06[0x8];                               // 0x8C
        int   fxBirthTime;                              // 0x94
        int   fxLetterTime;                             // 0x98
        int   fxDecayStartTime;                         // 0x9C
        int   fxDecayDuration;                          // 0xA0
        int   soundID;                                  // 0xA4
        char  pad07[0x4];                               // 0xA8
    }; // Size = 0xAC

    struct game_hudelem_s
    {
        hudelem_s elem;                                 // 0x00
        int clientNum;                                  // 0xAC
        int team;                                       // 0xB0
        int archived;                                   // 0xB4
    }; // Size = 0xB8

    struct scr_entref_t
    {
        unsigned short classnum;                        // 0x00
        unsigned short entnum;                          // 0x02
        unsigned short client;                          // 0x04
    }; // Size = 0x06

    struct LerpEntityState
    {
        int  eFlags;                                    // 0x000
        int  eType;                                     // 0x004
        char pad00[0x84];                               // 0x008
    }; // Size = 0x8C

    struct entityState_s
    {
        int             number;                         // 0x000
        LerpEntityState lerp;                           // 0x004
        char            pad00[0x1C];                    // 0x090
        int             clientNum;                      // 0x0AC
        char            pad01[0x30];                    // 0x0B0
        int             weapon;                         // 0x0E0
        char            pad02[0x34];                    // 0x0E4
    }; // Size = 0x118

    struct playerState_s
    {
        int       commandTime;                          // 0x0000
        int       pm_type;                              // 0x0004
        int       bobCycle;                             // 0x0008
        int       pm_flags;                             // 0x000C
        int       weapFlags;                            // 0x0010
        int       otherFlags;                           // 0x0014
        int       pm_time;                              // 0x0018
        int       loopSoundFade;                        // 0x001C
        float     origin[3];                            // 0x0020
        float     velocity[3];                          // 0x002C
        char      pad00[0x44];                          // 0x0038
        float     delta_angles[3];                      // 0x007C
        char      pad01[0x70];                          // 0x0088
        int       clientNum;                            // 0x00F8
        int       offHandIndex;                         // 0x00FC
        int       renderOptions;                        // 0x0100
        int       weapon;                               // 0x0104
        char      pad02[0x8];                           // 0x0108
        float     fWeaponPosFrac;                       // 0x0110
        char      pad03[0x4];                           // 0x0114
        int       spreadOverride;                       // 0x0118
        int       spreadOverrideState;                  // 0x011C
        int       viewmodelIndex;                       // 0x0120
        float     viewangles[3];                        // 0x0124
        char      pad04[0x34];                          // 0x0130
        int       health;                               // 0x0164
        char      pad05[0x4];                           // 0x0168
        int       maxHealth;                            // 0x016C
        char      pad06[0x7A4];                         // 0x0170
        float     aimSpreadScale;                       // 0x0914
        char      pad07[0x2BC];                         // 0x0918
        hudelem_s current[0x1F];                        // 0x0BD4
        char      pad08[0x4];                           // 0x20A8
    }; // Size = 0x20AC

    struct cpose_t
    {
        unsigned __int16 lightningHandle;               // 0x00
        char             eType;                         // 0x02
        char             eTypeUnion;                    // 0x03
        char             localClientNum;                // 0x04
        char             isRagdoll;                     // 0x05
        char             pad00[0x2];                    // 0x06
        int              ragdollHandle;                 // 0x08
        char             pad01[0x18];                   // 0x0C
        float            origin[3];                     // 0x24
        float            angles[3];                     // 0x30    
        char             pad02[0x8];                    // 0x3C
    }; // Size = 0x44

    struct centity_s
    {
        cpose_t         pose;                           // 0x000
        LerpEntityState lerp;                           // 0x044
        entityState_s   nextState;                      // 0x0D0
        char            pad00[0xE8];                    // 0x1E8
        int             alive;                          // 0x2D0
    }; // Size = 0x2D4

    struct refdef_s
    {
        unsigned int x;                                 // 0x0000
        unsigned int y;                                 // 0x0004
        unsigned int width;                             // 0x0008
        unsigned int height;                            // 0x000C
        float        tanHalfFovX;                       // 0x0010
        float        tanHalfFovY;                       // 0x0014
        float        fov_x;                             // 0x0018
        float        vieworg[3];                        // 0x001C
        float        yaw;                               // 0x0028
        float        viewaxis[3][3];                    // 0x002C
        int          time;                              // 0x0050
        float        zNear;                             // 0x0054
        float        zFar;                              // 0x0058
        char         pad00[0x4288];                     // 0x005C
    }; // Size = 0x42E4

    struct clientInfo_t
    {
        int   infoValid;                                // 0x000
        int   nextValid;                                // 0x004
        int   clientNum;                                // 0x008
        char  name[0x20];                               // 0x00C
        int   team;                                     // 0x02C
        int   oldteam;                                  // 0x030
        int   ffateam;                                  // 0x034
        char  pad00[0x20];                              // 0x038
        char  model[0x40];                              // 0x058
        char  attachModelNames[0x6][0x40];              // 0x098
        char  attachTagNames[0x6][0x40];                // 0x218
        char  pad01[0x68];                              // 0x398
        float playerAngles[3];                          // 0x400
        char  pad02[0x188];                             // 0x40C
    }; // Size = 0x594

    struct snapshot_s
    {
        int           snapFlags;                        // 0x00000
        int           ping;                             // 0x00004
        int           serverTime;                       // 0x00008
        playerState_s ps;                               // 0x0000C
        int           numEntities;                      // 0x020B8
        int           numClients;                       // 0x020BC
        char          pad00[0xC40C];                    // 0x020C0
        entityState_s entities[0x200];                  // 0x0E4CC
    }; // Size = 0x250C0

    struct cg_s
    {
        int           clientNum;                        // 0x000000
        char          pad00[0x20];                      // 0x000004
        snapshot_s   *snap;                             // 0x000024
        snapshot_s   *nextSnap;                         // 0x000028
        char          pad01[0xAAC50];                   // 0x00002C
        int           time;                             // 0x0AAC7C
        int           oldTime;                          // 0x0AAC80
        int           physicsTime;                      // 0x0AAC84
        int           mapRestart;                       // 0x0AAC88
        char          pad02[0x4];                       // 0x0AAC8C
        int           renderingThirdPerson;             // 0x0AAC90
        char          pad03[0x4];                       // 0x0AAC94
        playerState_s predictedPlayerState;             // 0x0AAC98
        char          pad04[0x33C];                     // 0x0ACD44
        refdef_s      refdef;                           // 0x0AD080
        char          pad05[0x6F0C];                    // 0x0B1364
        int           lastClipFlashTime;                // 0x0B8270
        char          pad06[0x58];                      // 0x0B8274
        int           weaponSelect;                     // 0x0B82CC
        char          pad07[0xD4];                      // 0x0B82D0
        float         gunPitch;                         // 0x0B83A4
        float         gunYaw;                           // 0x0B83A8
        char          pad08[0x8C7E4];                   // 0x0B83AC
        clientInfo_t  clients[0x12];                    // 0x144B90
        char          pad09[0x10548];                   // 0x14AFF8
        float         aimSpreadScale;                   // 0x15B540
        char          pad10[0x2574];                    // 0x15B544
    }; // Size = 0x15DAB8

    struct cgs_t
    {
        int   viewX;                                    // 0x0000
        int   viewY;                                    // 0x0004
        int   viewWidth;                                // 0x0008
        int   viewHeight;                               // 0x000C
        float viewAspect;                               // 0x0010
        int   serverCommandSequence;                    // 0x0014
        int   processedSnapshotNum;                     // 0x0018
        int   localServer;                              // 0x001C
        char  gametype[0x20];                           // 0x0020
        char  szHostName[0x100];                        // 0x0040
        int   maxclients;                               // 0x0140
        char  pad00[0x4];                               // 0x0144
        char  mapname[0x40];                            // 0x0148
        char  pad01[0xCBA8];                            // 0x0188
    }; // Size = 0xCD30

    struct usercmd_s
    {
        int  serverTime;                                // 0x00
        int  button_bits;                               // 0x04
        int  angles[3];                                 // 0x08
        char weapon;                                    // 0x14
        char offhandIndex;                              // 0x15
        char forwardmove;                               // 0x16
        char rightmove;                                 // 0x17
        char pad00[0x20];                               // 0x18
    }; // Size = 0x38

    struct clientActive_t
    {
        int           ads;                              // 0x000000
        char          pad00[0xC];                       // 0x000004
        playerState_s ps;                               // 0x000010
        char          pad01[0x2305C];                   // 0x0020BC
        int           parseEntitiesNum;                 // 0x025118
        char          pad02[0x64];                      // 0x02511C
        float         cgameOrigin[3];                   // 0x025180
        float         cgameVelocity[3];                 // 0x02518C
        int           cgameBobCycle;                    // 0x025198
        int           cgameMovementDir;                 // 0x02519C
        char          pad03[0x4];                       // 0x0251A0        
        int           cgamePredictedDataServerTime;     // 0x0251A4
        float         viewangles[3];                    // 0x0251A8
        char          pad04[0x80024];                   // 0x0251B4
        usercmd_s     cmds[0x80];                       // 0x0A51D8
        int           cmdNumber;                        // 0x0A6DD8
        char          pad05[0x81254];                   // 0x0A6DDC
        entityState_s parseEntities[0x800];             // 0x128030
    }; // Size = 0x1B4030

    extern clientActive_t *clientActive;

    struct EntHandle
    {
        unsigned short number;                          // 0x00
        unsigned short infoIndex;                       // 0x02
    }; // Size = 0x4

    struct entityShared_t
    {
        char           linked;                          // 0x00
        char           bmodel;                          // 0x01
        unsigned short svFlags;                         // 0x02
        int            clientMask;                      // 0x04
        int            broadcastTime;                   // 0x08
        char           pad00[0x8];                      // 0x0C
        float          mins[3];                         // 0x14
        float          maxs[3];                         // 0x20
        int            contents;                        // 0x2C
        float          absmin[3];                       // 0x30
        float          absmax[3];                       // 0x3C
        float          currentOrigin[3];                // 0x48
        float          currentAngles[3];                // 0x54
        EntHandle      ownerNum;                        // 0x60
        int            eventTime;                       // 0x64
    }; // Size = 0x68

    struct clientState_s
    {
        int clientIndex;                                // 0x000
        int team;                                       // 0x004
        int modelIndex;                                 // 0x008
        char pad00[0x44];                               // 0x00C
        char name[0x20];                                // 0x050
        char pad01[0x24];                               // 0x070
    }; // Size = 0x94

    struct clientSession_t
    {
        int           sessionState;                     // 0x000
        int           forceSpectatorClient;             // 0x004
        int           killcamEntity;                    // 0x008
        int           killcamTargetEntity;              // 0x00C
        int           archiveTime;                      // 0x010
        unsigned int  scriptPersId;                     // 0x014
        char          pad00[0x1C];                      // 0x018
        int           connected;                        // 0x034
        usercmd_s     cmd;                              // 0x038
        usercmd_s     oldcmd;                           // 0x070
        int           localClient;                      // 0x0A8
        int           predictedItemPickup;              // 0x0AC
        char          newnetname[0x20];                 // 0x0B0
        int           maxHealth;                        // 0x0D0
        char          pad01[0x10];                      // 0x0D4
        float         moveSpeedScaleMultiplier;         // 0x0E4
        int           viewmodelIndex;                   // 0x0E8
        int           noSpectate;                       // 0x0EC
        int           teamInfo;                         // 0x0F0
        clientState_s cs;                               // 0x0F4
        int           psOffsetTime;                     // 0x188
    }; // Size = 0x18C

    // TODO: Resume mapping gclient_s at 018EF2A4. This is after gclient_s::flags
    struct gclient_s
    {
        playerState_s   ps;                             // 0x0000
        clientSession_t sess;                           // 0x20AC
        int             spectatorClient;                // 0x2238
        int             flags;                          // 0x223C
        char            pad00[0xC];                     // 0x2240
        int             button_bits;                    // 0x224C
        int             oldbutton_bits;                 // 0x2250
        char            pad01[0x8];                     // 0x2254
        float           fGunPitch;                      // 0x225C
        float           fGunYaw;                        // 0x2260
        char            pad02[0xE4];                    // 0x2264
    }; // Size = 0x2348

    struct actor_s
    {
        struct gentity_s  *gent;                        // 0x000
        struct sentient_s *sentient;                    // 0x004
        int                species;                     // 0x008
        char               pad00[0xB80];                // 0x00C
        ai_state_t         eState[5];                   // 0xB8C
        int                eSubState[5];                // 0xBA0
        unsigned int       stateLevel;                  // 0xBB4
        int                iStateTIme;                  // 0xBB8
        int                preThinkTIme;                // 0xBBC
        struct ai_transition_cmd_t
        {
            int eTransition;                                // 0x00
            int eState;                                     // 0x04
        } StateTransitions[0xB];                        // 0xBC0
        int                transitionCount;             // 0xC18
        int                eSimulatedState[5];          // 0xC1C
        int                simulatedStateLevel;         // 0xC30
        char               pad01[0x136];                // 0xC34
        unsigned short     damageWeapon;                // 0xD6A


        void (__fastcall *GetStateFunction())(actor_s *, int)
        {
            return ((void (__fastcall ***)(actor_s *, int))0x8DC3C8)
                [this->species][this->eState[0] * 8 - this->eState[0]];
        }
    }; // Current Size = 0xD6C

    struct gentity_s
    {
        entityState_s         s;                        // 0x000
        entityShared_t        r;                        // 0x118
        gclient_s            *client;                   // 0x180
        actor_s              *actor;                    // 0x184
        struct sentient_s    *sentient;                 // 0x188
        struct scr_vehicle_s *scr_vehicle;              // 0x18C
        char                  pad00[0x8];               // 0x190 
        unsigned short        model;                    // 0x198
        char                  pad01[0x1DE];             // 0x19A
    }; // Size = 0x378
#pragma pack(pop)

    extern centity_s *cg_entitiesArray;
    extern cg_s      *cgameGlob;
    extern gentity_s *g_entities;
    extern cgs_t     *cgs;
    extern WORD      *clientObjMap;
    extern BYTE      *objBuf;
   
    enum
    {
        CG_GetPlayerViewOrigin_a            = 0x468A00,
        CG_GetPlayerWeapon_a                = 0x46BF20,
        LookAtKiller_a                      = 0x4F5450,
        Actor_FirstActor_a                  = 0x4B5850,
        Actor_NextActor_a                   = 0x4B58A0,
    };

    // Actors
    actor_s *Actor_FirstActor(int teamFlags);
    actor_s *Actor_NextActor(actor_s *pPrevActor, int teamFlags);

    // Player data
    extern int (__cdecl *CG_GetPlayerWeapon)(playerState_s *ps, int localClientNum);

    void __usercall CL_GetUserCmd(int cmdNum, usercmd_s *cmd);
    usercmd_s *__usercall CL_GetUserCmd(int cmdNum);
    bool __usercall CG_GetPlayerViewOrigin(int localClientNum, playerState_s *ps, float out[3]);
}

/**
 * @brief Returns if the target specified in arg is visible and targetable 
 * @param target A pointer to game structure of the target.
 * @return If the player is able to be targeted
**/
bool ValidTarget(GameData::actor_s *target);
