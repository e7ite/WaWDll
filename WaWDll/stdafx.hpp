// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

// Windows Library
#include <windows.h>
#include <detours/detours.h>
#ifdef _DEBUG
#include <ShlObj_core.h>
#endif // _DEBUG

// Standard Library 
#include <vector>
#include <functional>
#include <sstream>
#include <unordered_map>
#ifdef _DEBUG
#include <iostream>
#include <filesystem>
#include <fstream>
#endif // _DEBUG

// Use when game function is using non-standard calling convention caused by 
// compiler optimization. Caller cleans the stack frame
// Detours using __usercall must have target function invocation and return manually written.
// Microsoft detours know how to correctly invoke a function with a standard 
// calling convention such as __cdecl, __fastcall, etc. All standard calling connvention
// detours must do is invoke the function pointer afterward to resume program flow.
#define __usercall

// Use when game function is using non-standard calling convention caused by 
// compiler optimization. Callee cleans the stack frame
// Detours using __userpurge must have target function invocation and return manually written.
// Microsoft detours know how to correctly invoke a function with a standard 
// calling convention such as __cdecl, __fastcall, etc. All standard calling connvention
// detours must do is invoke the function pointer afterward to resume program flow.
#define __userpurge

// Nasty test code that runs once. Only use this when for testing and make sure
// to remove later!
// Credit to StackOverflow answer for the preprocessor hack for generating a random
// variable name 
#ifdef _DEBUG
#define PP_CAT_II(p, res) res
#define PP_CAT_I(a, b) PP_CAT_II(~, a ## b)
#define PP_CAT(a,b) PP_CAT_I(a, b)
#define UNIQUE_NAME(base) PP_CAT(base, __COUNTER__)

#define TESTIMPL(code, name) \
static bool name = false;    \
if (!name)                   \
{                            \
    code                     \
    name = true;             \
}
#define TEST(code) TESTIMPL(code, UNIQUE_NAME(a))
#endif