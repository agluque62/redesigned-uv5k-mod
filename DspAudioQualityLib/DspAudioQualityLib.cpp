// DspAudioQualityLib.cpp : Define las funciones exportadas de la aplicación DLL.
//

#include "stdafx.h"

#include <math.h>

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

	__declspec(dllexport) void DAQRoutineTest(int routine) {
		ChannelProcess::RoutineTest(routine);
	}

	__declspec(dllexport) void DAQTwiddleGeneratorTest() {
	
		int n = 512;
		float w[512 * 2];

		int i, j, k;
		const float PI = 3.14159265358979323846;

		for (j = 1, k = 0; j < n >> 2; j = j << 2)
		{
			for (i = 0; i < n >> 2; i += j)
			{
				w[k + 5] = sin(6.0 * PI * i / n);
				w[k + 4] = cos(6.0 * PI * i / n);

				w[k + 3] = sin(4.0 * PI * i / n);
				w[k + 2] = cos(4.0 * PI * i / n);

				w[k + 1] = sin(2.0 * PI * i / n);
				w[k + 0] = cos(2.0 * PI * i / n);

				k += 6;
			}
		}
	}

#ifdef __cplusplus
}
#endif // __cplusplus

