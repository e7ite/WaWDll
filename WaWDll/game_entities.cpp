#include "stdafx.hpp"
#include "game_entities.hpp"

namespace GameData
{
    centity_s      *cg_entitiesArray = (centity_s *)0x35D39F0;
    cg_s           *cgameGlob        = (cg_s *)0x34732B8;
    WORD           *clientObjMap     = (WORD *)0x1FE58C8;
    BYTE           *objBuf           = (BYTE *)0x1F978C8;
    gentity_s      *g_entities       = (gentity_s *)0x176C6F0;
    cgs_t          *cgs              = (cgs_t *)0x3466578;
    clientActive_t *clientActive     = (clientActive_t *)0x3058528;

    int (__cdecl *CG_GetPlayerWeapon)(playerState_s *ps, int localClientNum)
        = (int (__cdecl *)(playerState_s *, int))CG_GetPlayerWeapon_a;

    void CL_GetUserCmd(int cmdNum, usercmd_s *cmd)
    {
        __asm
        {
            mov         esi, cmdNum
            and         esi, 7Fh
            lea         eax, ds:0[esi * 8]
            sub         eax, esi
            lea         esi, ds:30FD700h[eax * 8]   ; source
            mov         edi, cmd                    ; destination
            mov         ecx, 0Eh                    ; amount of dwords to copy
            rep movsd                               ; move dwords esi to edi,
                                                    ; ecx times
        }
    }

    usercmd_s *__usercall CL_GetUserCmd(int cmdNum)
    {
        usercmd_s *result;
        __asm
        {
            mov         esi, cmdNum
            and         esi, 7Fh
            lea         eax, ds:0[esi * 8]
            sub         eax, esi
            lea         esi, ds:30FD700h[eax * 8]
            mov         result, esi
        }
        return result;
    }

    actor_s *Actor_FirstActor(int teamFlags)
    {
        actor_s *result;
        DWORD addr = Actor_FirstActor_a;
        __asm
        {
            mov         esi, teamFlags
            call        addr
            mov         result, eax
        }
        return result;
    }

    actor_s *Actor_NextActor(actor_s *pPrevActor, int teamFlags)
    {
        actor_s *result;
        DWORD addr = Actor_NextActor_a;
        __asm
        {
            mov         eax, pPrevActor
            mov         edi, teamFlags
            call        addr
            mov         result, eax
        }
        return result;
    }

    bool __usercall CG_GetPlayerViewOrigin(int localClientNum, playerState_s *ps, float origin[3])
    {
        DWORD addr = CG_GetPlayerViewOrigin_a;
        bool result;
        __asm
        {
            mov         eax, localClientNum
            push        origin
            push        ps
            call        addr
            add         esp, 8
            mov         result, al
        }
        return result;
    }
}

bool ValidTarget(GameData::actor_s *target)
{
    // Check if AI's level is marked as dead
    return target->eState[target->stateLevel] != GameData::AIS_DEATH;
}