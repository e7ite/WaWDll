#include "stdafx.hpp"
#include "host_menu.hpp"

namespace GameData
{
    scrVmPub_t *gScrVmPub = (scrVmPub_t *)0x3BD4700;

    void (__cdecl *(__cdecl *Scr_GetFunction)(const char **pName, int *pType))()
        = (void (__cdecl *(__cdecl *)(const char **, int *))())Scr_GetFunction_a;
    void (__cdecl *(__cdecl *CScr_GetFunction)(const char **pName))()
        = (void (__cdecl *(__cdecl *)(const char **))())CScr_GetFunction_a;
    void (__cdecl *(__cdecl *CScr_GetFunctionProjectSpecific)(const char **pName, int *pType))()
        = (void (__cdecl *(__cdecl *)(const char **, int *))())CScr_GetFunctionProjectSpecific_a;
    void (__cdecl *(__cdecl *CScr_GetMethod)(const char **pName, int *pType))(scr_entref_t entref)
        = (void (__cdecl *(__cdecl *)(const char **, int *))(scr_entref_t))CScr_GetMethod_a;

    unsigned int __usercall FindVariableIndexInternal(scriptInstance_t inst, unsigned int name,
        unsigned int index)
    {
        unsigned int result;
        DWORD addr = FindVariableIndexInternal_a;
        __asm
        {
            mov         eax, inst
            push        index
            push        name
            call        addr
            add         esp, 8
            mov         result, eax
        }
        return result;
    }

    unsigned int FindVariable(scriptInstance_t inst,
        unsigned int parentId, unsigned int unsignedValue)
    {
        WORD *gScVarGlob = (WORD *)0x3974700;
        return static_cast<DWORD>(
            *(WORD *)(
                (int)gScVarGlob + ((FindVariableIndexInternal(inst,
                    unsignedValue, (parentId + unsignedValue) % 0xFFFD + 1)
                    + inst * 0x16000) << 4)
                )
            );
    }

    unsigned int FindObject(scriptInstance_t inst, unsigned int id)
    {
        WORD *gScVarGlob = (WORD *)0x3974704;
        return *(unsigned int *)((int)gScVarGlob + ((id + inst * 0x16000) << 4));
    }

    unsigned int Scr_GetSelf(scriptInstance_t inst, unsigned int threadId)
    {
        unsigned short *gsvgVariableList = (unsigned short *)0x3914716;
        int index = (threadId + inst * 0x16000) << 4;

        return static_cast<unsigned int>(
            *(decltype(gsvgVariableList))((int)gsvgVariableList + index)
        );
    }

    unsigned int __usercall FindLastSibling(scriptInstance_t inst, unsigned int parentId)
    {
        DWORD result;
        DWORD addr = FindLastSibling_a;
        __asm
        {
            mov         edx, parentId
            mov         esi, inst
            call        addr
            mov         result, eax
        }
        return result;
    }

    int GetVariableKeyObject(scriptInstance_t inst, unsigned int id)
    {
        WORD *gsvgcVariableList = (WORD *)0x3974700;
        int *gScrVarGlobClient = (int *)0x3974708;
        DWORD index = inst * 0x16000;

        DWORD index2 =
            (DWORD)(*(WORD *)((DWORD)gsvgcVariableList + ((id + index) << 4)));

        return ((*(int *)(
            (DWORD)gScrVarGlobClient + ((index2 + index) << 4)
            )) >> 8) - 0x10000;
    }

    void *GetFunctionOrMethod(const char *name, int *funcType)
    {
        int type;
        void *result = nullptr;
        for (int i = SCRIPTINSTANCE_SERVER; i < SCRIPT_INSTANCE_MAX; i++)
        {
            // Discarding result of name change as overwriting the copy of the func name
            if ((result = GetFunction((scriptInstance_t)i, &name, &type)))
            {
                *funcType = 1;
                return result;
            }
            else if ((result = GetMethod((scriptInstance_t)i, &name, &type)))
            {
                *funcType = 0;
                return result;
            }
        }
        *funcType = -1;
        return result;
    }

    void __usercall Scr_AddFloat(float value)
    {
        DWORD addr = Scr_AddFloat_a;
        scriptInstance_t inst = SCRIPTINSTANCE_CLIENT;
        __asm
        {
            mov         eax, inst
            sub         esp, 4
            movss       xmm0, value
            movss       [esp], xmm0
            call        addr
            add         esp, 4
        }
    }

    void __usercall Scr_AddInt(int value)
    {
        DWORD addr = Scr_AddInt_a;
        __asm
        {
            mov         eax, value
            call        addr
        }
    }

    void __usercall Scr_AddString(const char *string)
    {
        DWORD addr = Scr_AddString_a;
        scriptInstance_t inst = SCRIPTINSTANCE_CLIENT;
        __asm
        {
            mov         eax, inst
            push        string
            call        addr
            add         esp, 4
        }
    }

    void __usercall Scr_AddVector(const float *value)
    {
        DWORD addr = Scr_AddVector_a;
        scriptInstance_t inst = SCRIPTINSTANCE_CLIENT;
        __asm
        {
            mov         eax, inst
            push        value
            call        addr
            add         esp, 4
        }
    }

    void (__cdecl *__usercall Scr_GetMethod(const char **pName, int *pType))(scr_entref_t entref)
    {
        DWORD addr = Scr_GetMethod_a;
        void (__cdecl *result)(scr_entref_t);
        __asm
        {
            mov         edi, pName
            mov         esi, pType
            call        addr
            mov         result, eax
        }
        return result;
    }

    void (__cdecl *GetFunction(scriptInstance_t inst, const char **pName, int *pType))()
    {
        if (inst)
        {
            return Scr_GetFunction(pName, pType);
        }
        else
        {
            *pType = 0;
            void (__cdecl *result)() = CScr_GetFunction(pName);
            if (!result)
                result = CScr_GetFunctionProjectSpecific(pName, pType);
            return result;
        }
    }

    void (__cdecl *GetMethod(scriptInstance_t inst, const char **pName, int *pType))(scr_entref_t entref)
    {
        if (inst)
            return CScr_GetMethod(pName, pType);
        return Scr_GetMethod(pName, pType);
    }

    void __usercall *VM_Notify = (void __usercall *)VM_Notify_a;
    void __declspec(naked) VM_NotifyDetourInvoke(scriptInstance_t inst,
        int notifyListOwnerId, int stringValue, VariableValue *top)
    {
        __asm
        {
            push        ebp
            mov         ebp, esp
            push        [ebp + 10h]
            push        [ebp + 0Ch]
            push        [ebp + 8]
            push        eax
            call        VM_NotifyDetour
            pop         eax
            add         esp, 0Ch
            mov         ebp, esp
            and         esp, 0FFFFFFF8h
            push        00698676h
            ret
        }
    }
    void VM_NotifyDetour(scriptInstance_t inst, int notifyListOwnerId,
        int stringValue, VariableValue *top)
    {
        LPCSTR notifyString = SL_ConvertToString(stringValue);
        if (!strcmp(notifyString, "spawned_player"))
            std::cout << "born" << std::endl;
        if (!strcmp(notifyString, "weapon_fired"))
        {
            /*for (int i = 0; i < 1024; i++)
                if (ValidTarget(&GameData::cg_entitiesArray[i]))
                    LookAtKiller(
                        GameData::g_entities + i,
                        GameData::g_entities + i,
                        GameData::g_entities + GameData::cgameGlob->clientNum);*/
            for (actor_s *actor = Actor_FirstActor(-1);
                actor;
                actor = Actor_NextActor(actor, -1))
            {
                std::cout << "New\n\n";
                std::cout << "Species: " << actor->species
                    << " State: " << actor->eState[actor->stateLevel] << std::endl;
                std::cout << std::hex << actor->GetStateFunctionTable() << std::endl;
            }
        }

        TEST(
            const char *str = "magicbullet";
            int type;
            CopyAddressToClipboard(GetFunctionOrMethod(str, &type));
        );

        //TEST(
        //    for (int i = 0; i < 0x60C; i += 0xC)
        //        std::cout << *(const char **)(0x8CF678 + i) << std::endl;
        //    for (int i = 0; i < 0x48; i += 0xC)
        //        std::cout << *(const char **)(0x839F80 + i) << std::endl;
        //    for (int i = 0; i < 0x1008; i += 0xC)
        //        std::cout << *(const char **)(0x8DABE0 + i) << std::endl;
        //);

        //DWORD notifyListId = FindVariable(inst, notifyListOwnerId, 0x15FFE);
        //if (!notifyListId)
        //    return;
        //notifyListId = FindObject(inst, notifyListId);

        //DWORD notifyNameListId = FindVariable(inst, notifyListId, stringValue);
        //if (!notifyNameListId)
        //    return;
        //notifyNameListId = FindObject(inst, notifyNameListId);

        //DWORD notifyListIndex = FindLastSibling(inst, notifyNameListId);
        //if (!notifyListIndex)
        //    return;

        //DWORD threadId = GetVariableKeyObject(inst, notifyListIndex);
        int self = Scr_GetSelf(inst, notifyListOwnerId);
    }
}