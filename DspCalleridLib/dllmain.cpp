// dllmain.cpp : Define el punto de entrada de la aplicación DLL.
#include "stdafx.h"

#include "Dspcode/cdecfsk.hpp"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	CDecFsk decFsk;

	__declspec(dllexport) void DCLStart() {
		decFsk.Inicia(0, NULL);
		decFsk.init_decodificacionDTMF();
	}

	__declspec(dllexport) bool DCLDecode(float *pMuestras, unsigned char *idcaller) {
		int cidLength = decFsk.TomaMuestras(pMuestras);
		if (cidLength > 0) {
			memcpy(idcaller, decFsk.idcaller, cidLength);
			idcaller[cidLength] = 0;
			decFsk.init_decodificacionDTMF();
			return true;
		}
		return false;
	}

#ifdef __cplusplus
}
#endif // __cplusplus

