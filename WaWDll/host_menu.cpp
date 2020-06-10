#include "stdafx.hpp"
#include "host_menu.hpp"
#include "weapon.hpp"

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

    ScriptObject::VariableValueWrapper ScriptObject::operator[](const char *fieldName)
    {
        VariableValue index;
        const char *err = "Variable is not a pointer";
        if (this->stackVal.type == VAR_POINTER)
        {
            for (DWORD id = FindFirstSibling(SCRIPTINSTANCE_SERVER, this->stackVal.u.stringValue);
                id;
                id = FindNextSibling(SCRIPTINSTANCE_SERVER, id))
            {
                if (!strcmp(fieldName, SL_ConvertToString(GetVariableName(SCRIPTINSTANCE_SERVER, id))))
                {
                    index.type = VAR_STRING;
                    index.u.stringValue = GetVariableName(SCRIPTINSTANCE_SERVER, id);
                    return Scr_EvalArray(SCRIPTINSTANCE_SERVER, &this->stackVal, &index);
                }
            }
            err = "Variable not found";
        }
        index.type = VAR_UNDEFINED;
        index.u.intValue = 0;
        Com_Error(0, err);
        return index;
    }

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

    unsigned int FindVariable(scriptInstance_t inst, unsigned int parentId, unsigned int name)
    {
        WORD *gScVarGlob = (WORD *)0x3974700;
        return static_cast<DWORD>(
            *(WORD *)(
                (int)gScVarGlob + ((FindVariableIndexInternal(inst,
                    name, (parentId + name) % 0xFFFD + 1) + inst * 0x16000) << 4)
            )
        );
    }

    unsigned int FindArrayVariable(scriptInstance_t inst, unsigned parentId, int intValue)
    {
        unsigned int result;
        DWORD addr = FindArrayVariable_a;
        __asm
        {
            mov         eax, parentId
            mov         ecx, intValue
            push        inst
            call        addr
            add         esp, 4
            mov         result, eax
        }
        return result;
    }

    unsigned int FindObject(scriptInstance_t inst, unsigned int id)
    {
        WORD *gScVarGlob = (WORD *)0x3974704;
        return *(unsigned int *)((int)gScVarGlob + ((id + inst * 0x16000) << 4));
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

    unsigned int FindFirstSibling(scriptInstance_t inst, unsigned int id)
    {
        return *(WORD *)(0x391471E + ((inst * 0x16000 + id) << 4));
    }

    unsigned int FindNextSibling(scriptInstance_t inst, unsigned int id)
    {
        unsigned int list = 0x3974700 + 0x160000 * inst;
        unsigned int nextSibling = *(WORD *)(list + (id << 4) + 0xE);
        return nextSibling ? *(WORD *)(list + (nextSibling << 4)) : 0;
    }

    unsigned int GetVariableKeyObject(scriptInstance_t inst, unsigned int id)
    {
        WORD *gsvgVariableList = (WORD *)0x3974700;
        int *gScrVarGlobClient = (int *)0x3974708;
        DWORD index = inst * 0x16000;

        DWORD index2 =
            (DWORD)(*(WORD *)((DWORD)gsvgVariableList + ((id + index) << 4)));

        return ((*(int *)(
            (DWORD)gScrVarGlobClient + ((index2 + index) << 4)
            )) >> 8) - 0x10000;
    }

    unsigned int GetVariableName(scriptInstance_t inst, unsigned int id)
    {
        return *(unsigned int *)(((id + inst * 0x16000) << 4) + 0x3974708) >> 8;
    }

    const char *SL_ConvertToString(int stringValue)
    {
        const char **gScrMemTreePub = (const char **)0x3702390;
        return *gScrMemTreePub + ((stringValue * 2 + stringValue) * 4) + 4;
    }

    unsigned short __usercall SL_FindString(scriptInstance_t inst, const char *tagname)
    {
        unsigned short result;
        unsigned int len = strlen(tagname) + 1;
        DWORD addr = SL_FindString_a;
        __asm
        {
            mov         eax, inst
            push        len
            push        tagname
            call        addr
            add         esp, 8
            mov         result, ax
        }
        return result;
    }

    unsigned int Scr_GetSelf(scriptInstance_t inst, unsigned int threadId)
    {
        unsigned short *gsvgVariableList = (unsigned short *)0x3914716;
        int index = (threadId + inst * 0x16000) << 4;

        return static_cast<unsigned int>(
            *(decltype(gsvgVariableList))((int)gsvgVariableList + index)
        );
    }

    void __usercall Scr_AddFloat(scriptInstance_t inst, float value)
    {
        DWORD addr = Scr_AddFloat_a;
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

    void __usercall Scr_AddInt(scriptInstance_t inst, int value)
    {
        DWORD addr = Scr_AddInt_a;
        __asm
        {
            mov         eax, inst
            push        value
            call        addr
        }
    }

    void __usercall Scr_AddString(scriptInstance_t inst, const char *string)
    {
        DWORD addr = Scr_AddString_a;
        __asm
        {
            mov         eax, inst
            push        string
            call        addr
            add         esp, 4
        }
    }

    void __usercall Scr_AddVector(scriptInstance_t inst, const float *value)
    {
        DWORD addr = Scr_AddVector_a;
        __asm
        {
            mov         eax, inst
            push        value
            call        addr
            add         esp, 4
        }
    }

    void __usercall Scr_AddObject(scriptInstance_t inst, unsigned int id)
    {
        DWORD addr = Scr_AddObject_a;
        __asm
        {
            mov         eax, inst
            mov         esi, id
            call        addr
        }
    }

    void Scr_AddEntity(scriptInstance_t inst, gentity_s *ent)
    {
        Scr_AddObject(inst, Scr_GetEntityId(inst, ent->s.number, 0, 0));
    }

    unsigned int __usercall Scr_GetEntityId(scriptInstance_t inst, int entnum, int classnum, short clientNum)
    {
        int result;
        DWORD addr = Scr_GetEntityId_a;
        __asm
        {
            mov         eax, entnum
            push        clientNum
            push        classnum
            push        entnum
            call        addr
            add         esp, 0Ch
            mov         result, eax
        }
        return result;
    }

    void __usercall Scr_GetVector(scriptInstance_t inst, float *vectorValue, unsigned int index)
    {
        DWORD addr = Scr_GetVector_a;
        __asm
        {
            mov         eax, inst
            mov         ecx, vectorValue
            push        index
            call        addr
            add         esp, 4
        }
    }

    scr_vartypes_t __usercall Scr_GetType(scriptInstance_t inst, unsigned int index)
    {
        scr_vartypes_t result;
        DWORD addr = Scr_GetType_a;
        __asm
        {
            mov         eax, inst
            mov         ecx, index
            call        addr
            mov         result, eax
        }
        return result;
    }

    unsigned int __usercall Scr_GetVariableFieldIndex(scriptInstance_t inst, unsigned int name, unsigned int parentId)
    {
        int result;
        DWORD addr = Scr_GetVariableFieldIndex_a;
        __asm
        {
            mov         eax, inst
            mov         esi, name
            push        parentId
            call        addr
            add         esp, 4
            mov         result, eax
        }
        return *(WORD *)(0x3974700 + ((inst * 0x16000 + result) << 4));
    }

    VariableValue Scr_EvalArray(scriptInstance_t inst, VariableValue *value, VariableValue *index)
    {
        VariableValue result;
        VariableValueInternal *entryValue = (VariableValueInternal *)(0x3974700 
            + ((inst * 0x16000 + Scr_FindArrayIndex(inst, value->u.stringValue, index)) << 4));
        result.type = (scr_vartypes_t)(entryValue->status & 0x1F);
        result.u = entryValue->u;
        return result;
    }

    unsigned int __usercall Scr_FindArrayIndex(scriptInstance_t inst, unsigned int parentId, VariableValue *index)
    {
        const char *err = "Invalid array index type";
        switch (index->type)
        {
            case VAR_INTEGER:
                if (index->u.intValue + 0x7EA002 <= 0xFEA001)
                    return FindArrayVariable(inst, parentId, index->u.intValue);
                err = "Array index out of bounds";
            break;
            case VAR_STRING:
                return FindVariable(inst, parentId, index->u.stringValue);
        }
        Com_Error(0, err);
        return 0;
    }

    unsigned int __usercall Scr_FindField(scriptInstance_t inst, const char *name, int *pType)
    {
        unsigned int result;
        DWORD addr = Scr_FindField_a;
        __asm
        {
            mov         eax, inst
            push        pType
            push        name
            call        addr
            add         esp, 8
            mov         result, eax
        }
        return result;
    }

    int __usercall Scr_GetPointerType(scriptInstance_t inst, unsigned int index)
    {
        int result;
        DWORD addr = Scr_GetPointerType_a;
        __asm
        {
            mov         eax, inst
            mov         ecx, index
            call        addr
            mov         result, eax
        }
        return result;
    }

    VariableUnion Scr_GetObject(scriptInstance_t inst)
    {
        VariableUnion result;
        DWORD addr = Scr_GetObject_a;
        __asm
        {
            mov         eax, inst
            call        addr
            mov         result, eax
        }
        return result;
    }

    void (__cdecl *__usercall Scr_GetMethod(const char **pName, int *pType))(scr_entref_t entref)
    {
        void (__cdecl *result)(scr_entref_t);
        DWORD addr = Scr_GetMethod_a;
        __asm
        {
            mov         edi, pType
            mov         esi, pName
            call        addr
            mov         result, eax
        }
        return result;
    }

    void Scr_SetParameters(scriptInstance_t inst, int argCount)
    {
        gScrVmPub[inst].inparamcount = argCount;
        gScrVmPub[inst].outparamcount = argCount;
    }

    void Scr_ClearOutParams(scriptInstance_t inst, int argCount)
    {
        for (int i = argCount; i > 0; i--)
        {
            ((void (__cdecl *)(scriptInstance_t, VariableValue *))0x67EB70)(
                inst, 
                gScrVmPub[inst].top--);
            gScrVmPub[inst].outparamcount--;
        }

        gScrVmPub[inst].inparamcount = gScrVmPub[inst].outparamcount;
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

    void (__cdecl *GScr_MagicBulletInternal)() = (void (__cdecl *)())GScr_MagicBullet_a;
    void GScr_MagicBullet(int client, const char *weapon)
    {
        gclient_s *ent = g_entities[client].client;

        vec3_t origin = Scr_GetTagOrigin(client, "tag_eye");
        origin.x -= 20;
        origin.x -= 20;
        origin.z -= 15;
        
        Scr_AddVector(SCRIPTINSTANCE_SERVER, 
            Scr_BulletTrace(
                Scr_AnglesToForward(Scr_GetPlayerAngles(cgameGlob->clientNum)) * 25.0f, 
                Scr_AnglesToForward(Scr_GetPlayerAngles(cgameGlob->clientNum)) * 9999.0f,
                0, 
                &g_entities[client])["position"]);
        Scr_AddVector(SCRIPTINSTANCE_SERVER, Scr_AnglesToForward(Scr_GetPlayerAngles(cgameGlob->clientNum)) * 25.0f);
        Scr_AddString(SCRIPTINSTANCE_SERVER, weapon);
        Scr_SetParameters(SCRIPTINSTANCE_SERVER, 3);
        GScr_MagicBulletInternal();
        Scr_ClearOutParams(SCRIPTINSTANCE_SERVER, 4);
    }

    void (__cdecl *Scr_BulletTraceInternal)() = (void (__cdecl *)())Scr_BulletTrace_a;
    ScriptObject Scr_BulletTrace(const float *start, const float *end, unsigned int mask, gentity_s *ent)
    {
        Scr_AddEntity(SCRIPTINSTANCE_SERVER, ent);
        Scr_AddInt(SCRIPTINSTANCE_SERVER, mask);
        Scr_AddVector(SCRIPTINSTANCE_SERVER, end);
        Scr_AddVector(SCRIPTINSTANCE_SERVER, start);
        Scr_SetParameters(SCRIPTINSTANCE_SERVER, 4);
        Scr_BulletTraceInternal();
        ScriptObject obj(SCRIPTINSTANCE_SERVER, gScrVmPub[SCRIPTINSTANCE_SERVER].top);
        Scr_ClearOutParams(SCRIPTINSTANCE_SERVER, 5);
        return obj;
    }

    void (__cdecl *Scr_GetTagOriginInternal)(scr_entref_t entref) 
        = (void (__cdecl *)(scr_entref_t))Scr_GetTagOrigin_a;
    vec3_t Scr_GetTagOrigin(int client, const char *tagName)
    {
        scr_entref_t entref;
        entref.classnum = 0;
        entref.client = 0;
        entref.entnum = client;
        Scr_AddString(SCRIPTINSTANCE_SERVER, tagName);
        Scr_SetParameters(SCRIPTINSTANCE_SERVER, 1);
        Scr_GetTagOriginInternal(entref);
        vec3_t vec = gScrVmPub[SCRIPTINSTANCE_SERVER].top->u.vectorValue;
        Scr_ClearOutParams(SCRIPTINSTANCE_SERVER, 2);
        return vec;
    }

    void (__cdecl *Scr_GetPlayerAnglesInternal)(scr_entref_t entref)
        = (void (__cdecl *)(scr_entref_t))Scr_GetPlayerAngles_a;
    vec3_t Scr_GetPlayerAngles(int client)
    {
        scr_entref_t entref;
        entref.classnum = 0;
        entref.client = 0;
        entref.entnum = client;
        Scr_GetPlayerAnglesInternal(entref);
        vec3_t vec = gScrVmPub[SCRIPTINSTANCE_SERVER].top->u.vectorValue;
        Scr_ClearOutParams(SCRIPTINSTANCE_SERVER, 1);
        return vec;
    }

    void (__cdecl *Scr_AnglesToForwardInternal)() = (void (__cdecl *)())Scr_AnglesToForward_a;
    vec3_t Scr_AnglesToForward(const float *angles)
    {
        Scr_AddVector(SCRIPTINSTANCE_SERVER, angles);
        Scr_SetParameters(SCRIPTINSTANCE_SERVER, 1);
        Scr_AnglesToForwardInternal();
        vec3_t vec = gScrVmPub[SCRIPTINSTANCE_SERVER].top->u.vectorValue;
        Scr_ClearOutParams(SCRIPTINSTANCE_SERVER, 2);
        return vec;
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
            int functype;
            GameData::scriptInstance_t newInst;

            // GScr_MagicBullet(cgameGlob->clientNum, /*ptrs41_zombie*/"panzerschrek");
            
            vec3_t vec = Scr_GetPlayerAngles(cgameGlob->clientNum);
            for (int i = 0; i < 3; i++)
                std::cout << vec[i] << std::endl;

            // Get the GSC script function/method and pass the arguments
            void *script = GetFunctionOrMethod("anglestoforward", &functype, &newInst);

            switch (functype)
            {
                case 0:
                    std::cout << "MEthod " << newInst << std::endl;
                    CopyAddressToClipboard(script);
                break;
                case 1:
                    std::cout << "Function " << newInst << std::endl;
                    CopyAddressToClipboard(script);
                break;
                default:
                    std::cout << "Not found\n";
                break;
            }
        }

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

void *GetFunctionOrMethod(const char *name, int *funcType, GameData::scriptInstance_t *instType)
{
    int type;
    void *result = nullptr;
    for (int i = GameData::SCRIPTINSTANCE_SERVER; i < GameData::SCRIPT_INSTANCE_MAX; i++)
    {
        *instType = (GameData::scriptInstance_t)i;
        // Discarding result of name change as this is overwriting the copy of the func name
        if ((result = GameData::GetFunction((GameData::scriptInstance_t)i, &name, &type)))
        {
            *funcType = 1;
            return result;
        }
        else if ((result = GameData::GetMethod((GameData::scriptInstance_t)i, &name, &type)))
        {
            *funcType = 0;
            return result;
        }
    }
    *funcType = -1;
    return result;
}