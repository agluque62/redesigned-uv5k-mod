// DspAudioQualityLib.cpp : Define las funciones exportadas de la aplicación DLL.
//

#include "stdafx.h"

#include "ChannelProcess.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	__declspec(dllexport) int DAQOpenInstance() {
		return ChannelProcess::GetNewInstance();
	}

	__declspec(dllexport) void DAQSetData(int instance, int count, float *pfData) {
		ChannelProcess *p = ChannelProcess::GetInstance(instance);
		if (p != NULL) {
			p->ProccessData(pfData, count);
		}
	}

	__declspec(dllexport) int DAQGetQuality(int instance) {
		ChannelProcess *p = ChannelProcess::GetInstance(instance);
		if (p != NULL) {
			return p->GetQualityValue();
		}
		return -1;
	}

	__declspec(dllexport) float DAQGetSampleMax(int instance) {
		ChannelProcess *p = ChannelProcess::GetInstance(instance);
		if (p != NULL) {
			return p->GetSampleMax();
		}
		return 0;
	}

	__declspec(dllexport) void DAQCloseInstance(int instance) {
		ChannelProcess::ReleaseInstance(instance);
	}

#ifdef __cplusplus
}
#endif // __cplusplus

