#pragma once

#include "DspCode/processor.h"

#define CHANNELS_MAX_NUMBER		32

class ChannelProcess
{
public:
	ChannelProcess();
	~ChannelProcess();
public:
	void ProccessData(float *pfData, int count);
	int GetQualityValue();
private:
	processor_data pdata;

public:
	static void Init() {
		for (int i = 0; i < CHANNELS_MAX_NUMBER; i++) {
			instances[i] = NULL;
		}
	}
	static void Dispose() {
		for (int i = 0; i < CHANNELS_MAX_NUMBER; i++) {
			if (instances[i] != NULL) {
				delete instances[i];
				instances[i] = NULL;
			}
		}
	}
	static int GetNewInstance() {
		for (int i = 0; i < CHANNELS_MAX_NUMBER; i++) {
			if (instances[i] == NULL) {
				instances[i] = new ChannelProcess();
				return i;
			}
		}
		return -1;
	}
	static void ReleaseInstance(int instance) {
		if (instance >= 0 && instance < CHANNELS_MAX_NUMBER) {
			if (instances[instance] != NULL) {
				delete instances[instance];
				instances[instance] = NULL;
			}
		}
	}
	static ChannelProcess *GetInstance(int instance) {
		if (instance >= 0 && instance < CHANNELS_MAX_NUMBER) {
			return instances[instance];
		}	
		return NULL;		
	}

	static ChannelProcess *instances[CHANNELS_MAX_NUMBER];	
};

