
#include <math.h>

#include "processor.h"
//#include "signaldumper.h"
#include "dsp_windows.h"
//#include "crecurso.hpp"

#define FS (8000.0f)

/*
static int f_200Hz = (int) (200.0/8000*FFT_SIZE);
static int f_3000Hz = (int) (3000.0/8000*FFT_SIZE);
*/

static int CEPS_TONE_IDX_MAX = (int) (FS/80);
static int CEPS_TONE_IDX_MIN = (int) (FS/300);
static int CEPS_TONE_IDX_COUNT = -1;


/*
 * Generates inplace abs of the given block of complex signal
 */
static void abs_complex(complex_num * v, int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		v[i].real = sqrt(v[i].real*v[i].real + v[i].imaginary*v[i].imaginary);
		v[i].imaginary = 0;
	}
}

/*
 * Generates inplace log(abs(v[i))) being v a complex signal
 */
static void abs_log_complex(complex_num * v, int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		float real = sqrt(v[i].real*v[i].real + v[i].imaginary*v[i].imaginary);
		v[i].real = real==0 ? 0 : log(real);
		v[i].imaginary = 0;
	}
}


/*
 * Generates the mean of given real signal block.
 * No check is done on imaginary part
 */
static complex_num mean_real(complex_num * v, int n)
{
	complex_num r = {0,0};
	int i;

	for ( i = 0; i < n; i++)
		r.real += v[i].real;

	if (n > 0)
		r.real /= n;

	return r;
}

static complex_num signalPower(complex_num * v, int n)
{
	complex_num r = {0,0};
	int i;

	for ( i = 0; i < n; i++)
		r.real += v[i].real*v[i].real;

	return r;
}

/*
 * Computes total energy of given real signal as sum(v[i]^2)
 * No check is done on imaginary part
 */
/*static complex_num energy_real(complex_num * v, int n)
{
	complex_num r = {0,0};
	int i;

	for ( i = 0; i< n; i++)
		r.real += v[i].real*v[i].real;

	return r;
}
*/

/*
 * Computes a value representing some kind of singal's tonality
 * absFft: absolute value of FFT of the block to study
 */
/*
static complex_num tonality(complex_num * absFft)
{
	int i;
	complex_num r = {0,0};

	complex_num mean = mean_real(&absFft[f_200Hz], f_3000Hz-f_200Hz);
	complex_num energy = energy_real(absFft, PROCESSOR_BLOCK_SIZE);

	for ( i = f_200Hz; i < f_3000Hz; i++)
	{
		float tmp = (absFft[i].real - mean.real);
		r.real += tmp*tmp;
	}

	if (energy.real > 0)
		r.real /= energy.real;
	else
		r.real = 0;

	return r;
}
*/

static int max_real(complex_num * v, int length)
{
	int i;
	int maxIdx = 0;
	float maxReal = v[0].real;

	for (i = 0; i < length; i++)
	{
		if (v[i].real > maxReal)
		{
			maxReal = v[i].real;
			maxIdx = i;
		}
	}
	return maxIdx;
}

static float abs_float(float f)
{
	return f>0?f:-f;
}

static void logBlockSignal(complex_num val, int fileId)
{
    {
    	int i;
    	for (i = 0; i < (PROCESSOR_BLOCK_SIZE - PROCESSOR_BLOCK_OVERLAP); i++)
    		logSignal(&val,1,fileId);
    }
}

static void process_internal (processor_data_t * data)
{
    int cepsFreqIdx;
	complex_num cepsFreq;
    complex_num cepsFreqSt;

    complex_num cepsFreqSnr;

    complex_num quality;

	complex_num * in = data->cBlock;
	complex_num y_fft[PROCESSOR_BLOCK_SIZE];
	complex_num y_ceps[PROCESSOR_BLOCK_SIZE];

	complex_num power;

	power = signalPower(in,PROCESSOR_BLOCK_SIZE);
	data->fDebug=power.real;

	/* windowing of input data*/
	window(in,PROCESSOR_HANN_WINDOW,PROCESSOR_BLOCK_SIZE);

	//JJA logSignal(in,PROCESSOR_BLOCK_SIZE,0);

	/* fft of windowed signal */
	fft(in,y_fft);
	//fft();
	abs_log_complex(y_fft,PROCESSOR_BLOCK_SIZE);
	//JJA logSignal(y_fft,PROCESSOR_BLOCK_SIZE,1);

	/* cepstrum of windows signal */
	ifft(y_fft,y_ceps);
	abs_complex(y_ceps,PROCESSOR_BLOCK_SIZE);
	//JJA logSignal(y_ceps,PROCESSOR_BLOCK_SIZE,2);
	//JJA logSignal(&y_ceps[CEPS_TONE_IDX_MIN],CEPS_TONE_IDX_COUNT,3);

	/* Extraction of frequency as maximum of cepstrum on interest band */
	{
		cepsFreqIdx = CEPS_TONE_IDX_MIN + max_real(&y_ceps[CEPS_TONE_IDX_MIN],CEPS_TONE_IDX_COUNT);
		cepsFreq.real = FS/cepsFreqIdx;
		cepsFreq.imaginary = 0;
	}
	//JJA logBlockSignal(cepsFreq,4);


	/* Extraction of a measure of the variation of frequency between blocks */
	{
		cepsFreqSt.real = 0.7f*(1-abs_float((cepsFreq.real-data->lastCepsFreq.real)/220.0f))+(0.3f*data->lastCepsFreqSt.real);
		data->lastCepsFreqSt.real = cepsFreqSt.real;
		data->lastCepsFreq.real = cepsFreq.real;
	}
	//JJA logBlockSignal(cepsFreqSt,5);


	/* cepsSNR as mean hold around frequency minus mean of all band */
	{
		complex_num ceps_signal = mean_real(&y_ceps[cepsFreqIdx-1],3);
		complex_num ceps_noise = mean_real(&y_ceps[CEPS_TONE_IDX_MIN],CEPS_TONE_IDX_COUNT);
		cepsFreqSnr.real = ceps_signal.real - ceps_noise.real;
		cepsFreqSnr.imaginary = 0;
	}
	//JJA logBlockSignal(cepsFreqSnr,6);

	quality.imaginary = 0;

	/* generation of current quality value as a fixed and filtered tonality measure */
	/* quality.real = cTonality.real*15.0f-1.5f; */

	/* calculus of quality as the product of cepsSNR and frequency variation */
	quality.real = 35 * cepsFreqSnr.real * cepsFreqSt.real - 1;

	// if (power.real < 5e-4)  // AGL
	if (power.real < 5000)	//JJA
		quality.real = 0;

    if (quality.real != quality.real) /* NAN will not happen with real signals, since it is never all zeros */
    	quality.real = data->quality.real;
    else if (quality.real > 5)
    	quality.real = 5;
    else if (quality.real < 0)
    	quality.real = 0;

    if (quality.real > data->quality.real)
    	//data->quality.real = quality.real*0.1f + data->quality.real*0.9f;
    	data->quality.real = quality.real*0.25f + data->quality.real*0.75f;
    	//data->quality.real = quality.real*0.50f + data->quality.real*0.50f;
    else
    	data->quality.real = quality.real*0.005f + data->quality.real*0.995f;
   	//JJA logBlockSignal(data->quality,13);

   	//JJA logBlockSignal(power,15);

	data->fproc=1;	// JJA
}


int quality_indicator(processor_data_t * data)
{
	//JJA return (int) data->quality.real;
	return (int) (data->quality.real*10.0f + 0.5f); // JJA (x10 y redondeo para tener valores de 0 a 50)
}

float quality_indicator_float(processor_data_t * data)
{
	return data->quality.real;
}

void processor_init(processor_data_t * data, int instanceid)
{
	CEPS_TONE_IDX_COUNT = CEPS_TONE_IDX_MAX - CEPS_TONE_IDX_MIN;

	fft_init();

	data->initiated = 1;

	data->cBlockPos = 0;
	data->cBlock = data->cBlock0;
	data->instance_id = instanceid;

	data->quality.real = 0;
	data->quality.imaginary = 0;

	data->lastCepsFreq.real = 0;
	data->lastCepsFreq.imaginary = 0;
	data->lastCepsFreqSt.real = 0;
	data->lastCepsFreqSt.imaginary = 0;

	data->fproc=0;	// JJA

	data->cnt1 = data->cnt2 = 0;

}

int process(processor_data_t * data, float * v, int count)
{

	int _ret=0;

	data->cnt1++;
	
	complex_num * nextBlock = (data->cBlock == data->cBlock0)?data->cBlock1:data->cBlock0;

	if (!data->initiated)
		return -1;

	if (data->instance_id > 0)
	{
		data->instance_id--;
		return 1;
	}

	/* generation of blocks of PROCESSOR_BLOCK_SIZE that are going to be processed */
	int i;
	for (i = 0; i < count; i++, data->cBlockPos++)
	{
		float abs_sample = abs_float(v[i]);
		if (abs_sample > data->sample_max) data->sample_max = abs_sample;

		data->cBlock[data->cBlockPos].real = v[i];
		data->cBlock[data->cBlockPos].imaginary = 0;

		if (data->cBlockPos == (PROCESSOR_BLOCK_SIZE-1))
		{
			int j,k;

			for (j = 0; j < PROCESSOR_BLOCK_OVERLAP; j++)
			{
				k = j+(PROCESSOR_BLOCK_SIZE-PROCESSOR_BLOCK_OVERLAP);

				nextBlock[j].real = data->cBlock[k].real;
				nextBlock[j].imaginary = data->cBlock[k].imaginary;
			}
			
			//if (iContProcess_BSS > 0)
			//{
			//	process_internal(data);
			//	iContProcess_BSS--;
			//}
			//else
   //				_ret=-2; // No procesamos el BSS para distribuir la carga (ya se hara despues si se puede)

			data->cnt2++;
			process_internal(data);

			data->cBlock = nextBlock;
			nextBlock = (data->cBlock == data->cBlock0)?data->cBlock1:data->cBlock0;
			data->cBlockPos = PROCESSOR_BLOCK_OVERLAP-1;
		}
	}

	return _ret;
}

/* AGL para el benchmark de las rutinas */
complex_num test_vector[PROCESSOR_BLOCK_SIZE];
complex_num test_vector_out[PROCESSOR_BLOCK_SIZE];
void routine_test(int routine)
{
	switch (routine)
	{
	case 0:			// Power
		signalPower(test_vector, PROCESSOR_BLOCK_SIZE);
		break;
	case 1:			// Windowing
		window(test_vector, PROCESSOR_HANN_WINDOW, PROCESSOR_BLOCK_SIZE);
		break;
	case 2:			// FFT
		fft(test_vector, test_vector_out);
		break;
	case 3:			// LOG Complex
		abs_log_complex(test_vector, PROCESSOR_BLOCK_SIZE);
		break;
	case 4:			// IFFT
		ifft(test_vector, test_vector_out);
		break;
	case 5:			// ABS Complex
		abs_complex(test_vector, PROCESSOR_BLOCK_SIZE);
		break;

	case 6:			// Inicializar FFT
		fft_init();
		break;

	default:
		break;
	}
}

