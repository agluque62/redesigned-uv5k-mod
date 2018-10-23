#include "stdafx.h"
#include "ChannelProcess.h"

/** */
ChannelProcess *ChannelProcess::instances[CHANNELS_MAX_NUMBER];

ChannelProcess::ChannelProcess()
{
	processor_init(&pdata, 0);
}

ChannelProcess::~ChannelProcess()
{
}

void ChannelProcess::ProccessData(float *pfDadta, int count) 
{
	process(&pdata, pfDadta, count);
}

int ChannelProcess::GetQualityValue() 
{
	return quality_indicator(&pdata);
}

void ChannelProcess::RoutineTest(int routine) 
{
	routine_test(routine);
}

