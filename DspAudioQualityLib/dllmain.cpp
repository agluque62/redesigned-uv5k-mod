// dllmain.cpp : Define el punto de entrada de la aplicación DLL.
#include "stdafx.h"
#include "ChannelProcess.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		ChannelProcess::Init();
		break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
		break;

    case DLL_PROCESS_DETACH:
		ChannelProcess::Dispose();
        break;
    }
    return TRUE;
}

