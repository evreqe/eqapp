#pragma once

#include "common.h"

import eqapp;

const char* g_DLLName = "eqapp_dll.dll";

HMODULE g_Module = NULL;

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

EXTERN_DLL_EXPORT DWORD __stdcall Inject();
EXTERN_DLL_EXPORT DWORD __stdcall Eject();

BOOL __stdcall DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved);
