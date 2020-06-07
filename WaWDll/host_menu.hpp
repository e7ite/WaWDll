#pragma once

#include "game_entities.hpp"
#include "nonhost_menu.hpp"

namespace GameData
{
    void __usercall LookAtKiller(gentity_s *attacker, gentity_s *inflictor, gentity_s *self);
    void CG_BulletEndPos(int commandTime, float spread, float *start, float *end,
    float *dir, const float *forwardDir, const float *rightDir, const float *upDir,
    float maxRange);

    enum scriptInstance_t : int
    {
        SCRIPTINSTANCE_SERVER = 0x0,
        SCRIPTINSTANCE_CLIENT = 0x1,
        SCRIPT_INSTANCE_MAX   = 0x2,
    };

    enum scr_vartypes_t
    {
        VAR_UNDEFINED         = 0x0,
        VAR_BEGIN_REF         = 0x1,
        VAR_POINTER           = 0x1,
        VAR_STRING            = 0x2,
        VAR_ISTRING           = 0x3,
        VAR_VECTOR            = 0x4,
        VAR_END_REF           = 0x5,
        VAR_FLOAT             = 0x5,
        VAR_INTEGER           = 0x6,
        VAR_CODEPOS           = 0x7,
        VAR_PRECODEPOS        = 0x8,
        VAR_FUNCTION          = 0x9,
        VAR_STACK             = 0xA,
        VAR_ANIMATION         = 0xB,
        VAR_DEVELOPER_CODEPOS = 0xC,
        VAR_THREAD            = 0xD,
        VAR_NOTIFY_THREAD     = 0xE,
        VAR_TIME_THREAD       = 0xF,
        VAR_CHILD_THREAD      = 0x10,
        VAR_OBJECT            = 0x11,
        VAR_DEAD_ENTITY       = 0x12,
        VAR_ENTITY            = 0x13,
        VAR_ARRAY             = 0x14,
        VAR_DEAD_THREAD       = 0x15,
        VAR_COUNT             = 0x16,
        VAR_THREAD_LIST       = 0x17,
        VAR_ENDON_LIST        = 0x18,
    };

#pragma pack(push, 1)
    struct scr_entref_t
    {
        unsigned short classnum;                        // 0x00
        unsigned short entnum;                          // 0x02
        unsigned short client;                          // 0x04
    }; // Size = 0x06

    struct VariableValue
    {
        union VariableUnion
        {
            int                         intValue;           // 0x04
            float                       floatValue;         // 0x04
            unsigned int                stringValue;        // 0x04
            const float                *vectorValue;        // 0x04
            const char                 *codePosValue;       // 0x04
            unsigned int                pointerValue;       // 0x04
            struct VariableStackBuffer *stackValue;         // 0x04
            unsigned int                entityOffset;       // 0x04
        };
        VariableUnion u;                                // 0x00
        int           type;                             // 0x04
    }; // Size = 0x08

    struct function_stack_t
    {
        const char    *pos;                             // 0x00
        unsigned int   localId;                         // 0x04
        unsigned int   localVarCount;                   // 0x08
        VariableValue *top;                             // 0x0C
        VariableValue *startTop;                        // 0x10
    }; // Size = 0x14

    struct function_frame_t
    {
        function_stack_t fs;                            // 0x00
        int              topType;                       // 0x14
    }; // Size = 0x18

    struct scrVmPub_t
    {
        unsigned int     *localVars;                    // 0x0000
        VariableValue    *maxstack;                     // 0x0004
        int               function_count;               // 0x0008
        function_frame_t *function_frame;               // 0x000C
        VariableValue    *top;                          // 0x0010
        bool              debugCode;                    // 0x0014
        bool              abort_on_error;               // 0x0015
        bool              terminal_error;               // 0x0016
        char              pad00[0x1];                   // 0x0017
        unsigned int      inparamcount;                 // 0x0018
        unsigned int      outparamcount;                // 0x001C
        function_frame_t  function_frame_start[0x20];   // 0x0020
        VariableValue     stack[0x800];                 // 0x0320
    }; // Size = 0x4320
#pragma pack(pop)

    extern scrVmPub_t *gScrVmPub;

    enum // Addresses
    {
        FindVariableIndexInternal_a       = 0x68BC20,
        FindLastSibling_a                 = 0x68BCA0,
        Scr_AddFloat_a                    = 0x69A670,
        Scr_AddInt_a                      = 0x69A610,
        Scr_AddString_a                   = 0x69A7E0,
        Scr_AddVector_a                   = 0x69A940,
        Scr_AddObject_a                   = 0x69A770,
        Scr_GetEntityId_a                 = 0x692520,
        Scr_GetPointerType_a              = 0x69A580,
        Scr_GetObject_a                   = 0x69A460,
        Scr_GetVariableFieldIndex_a       = 0x6902F0,
        Scr_GetVector_a                   = 0x69A220,
        Scr_FindField_a                   = 0x693250,
        Scr_ClearOutParams_a              = 0x693DA0,
        Scr_GetFunction_a                 = 0x66EA30,
        CScr_GetFunction_a                = 0x5676F0,
        CScr_GetFunctionProjectSpecific_a = 0x52F0B0,
        Scr_GetMethod_a                   = 0x530630,
        CScr_GetMethod_a                  = 0x671110,
        VM_Notify_a                       = 0x698670,
        GScr_MagicBullet_a                = 0x51AFF0,
        Scr_BulletTrace_a                 = 0x51FB40,
    };
    
    // GSC Script
    extern void (__cdecl *(__cdecl *Scr_GetFunction)(const char **pName, int *pType))();
    extern void (__cdecl *(__cdecl *CScr_GetFunction)(const char **pName))();
    extern void (__cdecl *(__cdecl *CScr_GetFunctionProjectSpecific)(const char **pName, int *pType))();
    extern void (__cdecl *(__cdecl *CScr_GetMethod)(const char **pName, int *pType))(scr_entref_t entref);

    const char *SL_ConvertToString(int stringValue);
    void __usercall Scr_AddFloat(scriptInstance_t inst, float value);
    void __usercall Scr_AddInt(scriptInstance_t inst, int value);
    void __usercall Scr_AddString(scriptInstance_t inst, const char *string);
    void __usercall Scr_AddVector(scriptInstance_t inst, const float *value);
    void __usercall Scr_AddObject(scriptInstance_t inst, unsigned int id);
    void Scr_AddEntity(scriptInstance_t inst, gentity_s *ent);
    void Scr_SetParameters(scriptInstance_t inst);
    void __usercall Scr_ClearOutParams(scriptInstance_t inst);
    unsigned int __usercall Scr_GetEntityId(scriptInstance_t inst, int entnum, int classnum, short clientNum);
    void __usercall Scr_GetVector(scriptInstance_t inst, float *vectorValue, unsigned int index);
    unsigned int __usercall Scr_GetVariableFieldIndex(scriptInstance_t inst, unsigned int name, unsigned int parentId);
    unsigned int __usercall Scr_FindField(scriptInstance_t inst, const char *name, int *pType);
    int __usercall Scr_GetPointerType(scriptInstance_t inst, unsigned int index);
    VariableValue::VariableUnion __usercall Scr_GetObject(scriptInstance_t inst);
    void (__cdecl *__usercall Scr_GetMethod(const char **pName, int *pType))(scr_entref_t entref);
    void (__cdecl *GetFunction(scriptInstance_t inst, const char **pName, int *pType))();
    void (__cdecl *GetMethod(scriptInstance_t inst, const char **pName, int *pType))(scr_entref_t entref);
    unsigned int FindVariable(scriptInstance_t inst, unsigned int parentId, unsigned int name);
    unsigned int FindObject(scriptInstance_t inst, unsigned int id);
    unsigned int FindLastSibling(scriptInstance_t inst, unsigned int id);
    unsigned int GetVariableKeyObject(scriptInstance_t inst, unsigned int id);
    unsigned int Scr_GetSelf(scriptInstance_t inst, unsigned int threadId);
    
    // GSC Script Functions/Methods
    extern void (__cdecl *GScr_MagicBulletInternal)();
    void GScr_MagicBullet(int client, const char *weapon);
    extern int (__cdecl *Scr_BulletTraceInternal)();
    int Scr_BulletTrace(float *start, float *end, unsigned int mask, gentity_s *ent);

    // Used to monitor player events
    extern void __usercall *VM_Notify;
    void VM_NotifyDetourInvoke(scriptInstance_t inst, int notifyListOwnerId,
        int stringValue, VariableValue *top);
    void VM_NotifyDetour(scriptInstance_t inst, int notifyListOwnerId,
        int stringValue, VariableValue *top);
}

/**
 * @brief Searches for a GSC script function/method and returns a pointer to it
 * @param name The name of the function to look for
 * @param funcType Fills in if the return is a GSC function (1) or a GSC method (0)
 * @param instType Fills in if the return is a client function or server function
 * @return Pointer to the script function. Must be casted to the respective type
 * 
 * If the return is a method the type to cast will be a void (__cdecl *)()
 * Else if the return is a function, cast the cast will be a void (__cdecl *)(scr_entref_t)
 * Else if the searched for script is not found, it will return nullptr
**/
void *GetFunctionOrMethod(const char *name, int *funcType, GameData::scriptInstance_t *instType);