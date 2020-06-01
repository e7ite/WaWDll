#pragma once

#include "game_entities.hpp"

namespace GameData
{
    enum scriptInstance_t : int
    {
        SCRIPTINSTANCE_SERVER = 0x0,
        SCRIPTINSTANCE_CLIENT = 0x1,
        SCRIPT_INSTANCE_MAX   = 0x2,
    };

#pragma pack(push, 1)
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

    enum
    {
        FindVariableIndexInternal_a         = 0x68BC20,
        FindLastSibling_a                   = 0x68BCA0,
        Scr_AddFloat_a                      = 0x69A670,
        Scr_AddInt_a                        = 0x42A2B0,
        Scr_AddString_a                     = 0x69A7E0,
        Scr_AddVector_a                     = 0x69A940,
        Scr_GetFunction_a                   = 0x66EA30,
        CScr_GetFunction_a                  = 0x5676F0,
        CScr_GetFunctionProjectSpecific_a   = 0x52F0B0,
        VM_Notify_a                         = 0x698670,
    };
    
    // GSC Script
    extern void (__cdecl *(__cdecl *Scr_GetFunction)(const char **pName, int *type))();
    extern void (__cdecl *(__cdecl *CScr_GetFunction)(const char **pName))();
    extern void (__cdecl *(__cdecl *CScr_GetFunctionProjectSpecific)(const char **pName, int *type))();

    const char *SL_ConvertToString(int stringValue);
    void Scr_SetParameters(int count);
    void Scr_AddFloat(float value);
    void Scr_AddInt(int value);
    void Scr_AddString(const char *string);
    void Scr_AddVector(const float *value);
    void (__cdecl *GetFunction(scriptInstance_t inst, const char **pName, int *type))();
    unsigned int FindVariable(scriptInstance_t inst,
        unsigned int parentId, unsigned int unsignedValue);
    unsigned int FindObject(scriptInstance_t inst, unsigned int id);
    unsigned int Scr_GetSelf(scriptInstance_t inst, unsigned int threadId);
    unsigned int FindLastSibling(scriptInstance_t inst, unsigned int id);
    int GetVariableKeyObject(scriptInstance_t inst, unsigned int id);


    extern void __usercall *VM_Notify;
    void VM_NotifyDetourInvoke(scriptInstance_t inst, int notifyListOwnerId,
        int stringValue, VariableValue *top);
    void VM_NotifyDetour(scriptInstance_t inst, int notifyListOwnerId,
        int stringValue, VariableValue *top);
}