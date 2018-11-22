
// jmgarcia 20090623 
#define CD40_CODIGO_ETM
#include "general.h"
// FIN jmgarcia 

#include "constMP.h"
#include "iir_filt.h"

/**********************************************************
	Coeficientes utilizados en los distintos filtros IIR
	presentes en los distintos módulos del programa
***********************************************************/
	
/**********************************************************
	 Coeficientes utilizados en los filtros IIR
	Paso Bajo de 2700 Hz: 53 dB por encima de 2850 Hz
***********************************************************/
	
/*float a_2700[8] = {1.0,1.85934212428291,3.02698850752025,2.28381422851313,
1.65173686891619,0.40125031527237,0.15347222997504,-0.07371532981298};

float b_2700[8] = {0.12566457643333,0.67371934391856,1.71422323080154,
2.63783732118003,2.63783732118002,1.71422323080154,0.67371934391856,0.12566457643333};*/
/*float a_2700[8] = { 1.0, 1.66624132709443, 2.67107522221292, 1.78989851103904, 1.31991765589155, 
		0.22366198241709, 0.12026250478124, -0.08006464331154};
float b_2700[8] = {0.09800022592670, 0.55278385443445, 1.44731396296507, 2.25739823673615, 
		2.25739823673615, 1.44731396296507, 0.55278385443445, 0.09800022592670};

float a_dc[2] ={1,-0.9950};
float b_dc[2] ={0.9975,-0.9975};
*/
/*
float a_intr_diez[4] = {1,-0.03106752855872,0.45102412855423,-0.03457122308103};
float b_intr_diez[4] = {0.18918740226113,0.50350528619611,
                        0.50350528619611,0.18918740226113};
*/
/*
float a_intr_diez[7] = {1,-1.85981254424800,2.63605852746709,-2.21920420909552,
                        1.32324927146226,-0.48336444632362,0.08926362536804};
                 
float b_diez[7] = {0.01262196256944,0.05272582211228,0.10846802805516,
                    0.13576790322231,0.10846802805516,0.05272582211228,
                    0.01262196256944};

float b_interp[7] = {0.02524392513888,0.10545164422456,0.21693605611031,
                    0.27153580644462,0.21693605611031,0.10545164422456,
                    0.02524392513888};
*/
/*float a_intr_diez[8] = {1,-2.03883788508502,3.32390334032592,-3.32711292579761,
                        2.52456239262939,-1.30789476120299,0.44940628574003,
                        -0.07882004727485};
                 
float b_diez[8] = {0.00983331133625,0.04083525111274,0.09083186602824,
                   0.13110277119021,0.13110277119022,0.09083186602823,
                   0.04083525111275,0.00983331133624};

float b_interp[8] = {0.01966662267249,0.08167050222549,0.18166373205647,
                     0.26220554238043,0.26220554238044,0.18166373205646,
                     0.08167050222549,0.01966662267249};

float a_2280[3] = {1,0.30540053795516,0.49};
float b_2280[3] = {0.73767884440798,0.32151746914917,0.73620422439801}; 
*/

float a_2400_2600[5] = { 1, 1.17503154093632F, 1.51851361314671F, 0.69667620062114F, 0.35153041F };
float b_2400_2600[5] = { 0.62408059284571F, 0.95140398220683F, 1.59517705154305F, 0.94950212564640F, 0.62158801246218F };

/*
float a_425[3] = {1,-1.79513148828707,0.9025};
float b_425[3] = {0.95159408705196,-1.79634555076317,0.94969185047194}; 

float a_BL[5] = {1,-3.72310201994836,5.22061468065816,
                 -3.26675018682663,0.76949343155911};
float b_BL[5] = {0.1590228525e-4,0.6360914099e-4,0.9541371149e-4,
                 0.6360914099e-4,0.1590228525e-4};
*/
float bpa1020[5] = { 0.350938F, -1.308118F, 1.918113F, -1.308118F, 0.350938F };
float apa1020[5] = { 1,-1.832149F,1.761368F,-0.753482F,0.199491F };

float bpa300[5] = { 0.715753F,-2.847056F,4.262657F,-2.847056F,0.715753F };
float apa300[5] = { 1,-3.415572F,4.447481F,-2.613473F,0.586546F };

float a_DTMF[8] = { 1,-4.35359811693147F,8.70418559754473F,-10.05352174933416F, 
					7.15907006136459F,-3.06070340266376F,0.69071603019502F,-0.04441395854338F };
float b_DTMF[8] = { 0.30312020363648F,-2.00158742443694F,5.77874686264492F,-9.44964996757024F,
					9.44964996757024F,-5.77874686264492F,2.00158742443694F,-0.30312020363648F };


/**********************************************************
	Variables utilizadas para almacenar las condiciones
	iniciales de la siguiente trama
***********************************************************/

// float in_d[7]={0,0,0,0,0,0,0};
// float out_d[7]={0,0,0,0,0,0,0};

// jmgarcia 20090714
#if 0
float inTR_d[NUMCANAL][7];// Cuidadin
float outTR_d[NUMCANAL][7];// Cuidadin
#endif
// Los NUMCANAL primeros arrays son para audio de entrada ( if->host, y los demas para el
// de salida  (host->if).
float inTR_d[(NUMCANAL)*2][7];
float outTR_d[(NUMCANAL)*2][7];
// FIN jmgarcia


/**********************************************************
	Buffer auxiliar utilizado en tiempo real para almacenar
	el audio filtrado que se va a transmitir a la línea
***********************************************************/

float iirBuf_Aux[TAMTRAMA];

#ifdef _FALSE_
/**********************************************************

	Paso alto utilizado para eliminar la componente
	continua introducida por los codecs. Aunque las
	condiciones iniciales utilizadas son nulas, existe
	la posiblilidad de que no sea así.
	
***********************************************************/
void init_iirdc(int L,int NC,int *dirx_delay,float *y_delay,float *base_x,int ci_init)
{
  int i;
  
  for(i=0;i<NC;i++)
  {
    switch (ci_init)
    {
      case CINULAS:
        dirx_delay[i] = (int)&base_x[i];
        base_x[i] = 0;
        break;

      case CINONULAS:
        dirx_delay[i] = (int)&base_x[i*L];
        break;
      
      default:
        break;
    }    
    y_delay[i] = 0;
  }
}

float iir_dc(int L,float *x,float *y,int Dirx_delay,float *y_delay)
{
  int i,j;
  float *x_delay = (float *)Dirx_delay;
  
  y[0] = b_dc[0]*x[0] + b_dc[1]*(*x_delay) - a_dc[1]*(*y_delay);
  for(i=1,j=0;i<L;i++,j++)
    y[i] = b_dc[0]*x[i] + b_dc[1]*x[j] - a_dc[1]*y[j];

  *y_delay = y[j];
  return x[j];
}

/**********************************************************

	Paso bajo: diezmado de las muestras de entrada
***********************************************************/
void iir_diezm(float *x, float *y, float *x_d, float *y_d, int numY)
{
  int i,j;
  float y_diez[TAMTRAMA*2];

  y_diez[0] = b_diez[0]*x[0] + b_diez[1]*x_d[6] + b_diez[2]*x_d[5] + b_diez[3]*x_d[4] +
         b_diez[4]*x_d[3] + b_diez[5]*x_d[2] + b_diez[6]*x_d[1] + b_diez[7]*x_d[0] -  
         a_intr_diez[1]*y_d[6] - a_intr_diez[2]*y_d[5] - a_intr_diez[3]*y_d[4] - a_intr_diez[4]*y_d[3] -
         a_intr_diez[5]*y_d[2] - a_intr_diez[6]*y_d[1] - a_intr_diez[7]*y_d[0];

  y_diez[1] = b_diez[0]*x[1] + b_diez[1]*x[0] + b_diez[2]*x_d[6] + b_diez[3]*x_d[5] +
         b_diez[4]*x_d[4] + b_diez[5]*x_d[3] + b_diez[6]*x_d[2] + b_diez[7]*x_d[1] -  
         a_intr_diez[1]*y_diez[0] - a_intr_diez[2]*y_d[6] - a_intr_diez[3]*y_d[5] - a_intr_diez[4]*y_d[4] -
         a_intr_diez[5]*y_d[3] - a_intr_diez[6]*y_d[2] - a_intr_diez[7]*y_d[1];

  y_diez[2] = b_diez[0]*x[2] + b_diez[1]*x[1] + b_diez[2]*x[0] + b_diez[3]*x_d[6] +
         b_diez[4]*x_d[5] + b_diez[5]*x_d[4] + b_diez[6]*x_d[3] + b_diez[7]*x_d[2] -  
         a_intr_diez[1]*y_diez[1] - a_intr_diez[2]*y_diez[0] - a_intr_diez[3]*y_d[6] - a_intr_diez[4]*y_d[5] -
         a_intr_diez[5]*y_d[4] - a_intr_diez[6]*y_d[3] - a_intr_diez[7]*y_d[2];

  y_diez[3] = b_diez[0]*x[3] + b_diez[1]*x[2] + b_diez[2]*x[1] + b_diez[3]*x[0] +
         b_diez[4]*x_d[6] + b_diez[5]*x_d[5] + b_diez[6]*x_d[4] + b_diez[7]*x_d[3] -  
         a_intr_diez[1]*y_diez[2] - a_intr_diez[2]*y_diez[1] - a_intr_diez[3]*y_diez[0] - a_intr_diez[4]*y_d[6] -
         a_intr_diez[5]*y_d[5] - a_intr_diez[6]*y_d[4] - a_intr_diez[7]*y_d[3];

  y_diez[4] = b_diez[0]*x[4] + b_diez[1]*x[3] + b_diez[2]*x[2] + b_diez[3]*x[1] +
         b_diez[4]*x[0] + b_diez[5]*x_d[6] + b_diez[6]*x_d[5] + b_diez[7]*x_d[4] -  
         a_intr_diez[1]*y_diez[3] - a_intr_diez[2]*y_diez[2] - a_intr_diez[3]*y_diez[1] - a_intr_diez[4]*y_diez[0] -
         a_intr_diez[5]*y_d[6] - a_intr_diez[6]*y_d[5] - a_intr_diez[7]*y_d[4];

  y_diez[5] = b_diez[0]*x[5] + b_diez[1]*x[4] + b_diez[2]*x[3] + b_diez[3]*x[2] +
         b_diez[4]*x[1] + b_diez[5]*x[0] + b_diez[6]*x_d[6] + b_diez[7]*x_d[5] -  
         a_intr_diez[1]*y_diez[4] - a_intr_diez[2]*y_diez[3] - a_intr_diez[3]*y_diez[2] - a_intr_diez[4]*y_diez[1] -
         a_intr_diez[5]*y_diez[0] - a_intr_diez[6]*y_d[6] - a_intr_diez[7]*y_d[5];

  y_diez[6] = b_diez[0]*x[6] + b_diez[1]*x[5] + b_diez[2]*x[4] + b_diez[3]*x[3] +
         b_diez[4]*x[2] + b_diez[5]*x[1] + b_diez[6]*x[0] + b_diez[7]*x_d[6] -  
         a_intr_diez[1]*y_diez[5] - a_intr_diez[2]*y_diez[4] - a_intr_diez[3]*y_diez[3] - a_intr_diez[4]*y_diez[2] -
         a_intr_diez[5]*y_diez[1] - a_intr_diez[6]*y_diez[0] - a_intr_diez[7]*y_d[6];

  for (i=7;i<numY*2;i++)
    y_diez[i] = b_diez[0]*x[i] + b_diez[1]*x[i-1] + b_diez[2]*x[i-2] + b_diez[3]*x[i-3] +
         b_diez[4]*x[i-4] + b_diez[5]*x[i-5] + b_diez[6]*x[i-6] + b_diez[7]*x[i-7] -  
         a_intr_diez[1]*y_diez[i-1] - a_intr_diez[2]*y_diez[i-2] - a_intr_diez[3]*y_diez[i-3] -
         a_intr_diez[4]*y_diez[i-4] - a_intr_diez[5]*y_diez[i-5] - a_intr_diez[6]*y_diez[i-6] -
         a_intr_diez[7]*y_diez[i-7];
  

  x_d[0] = x[2*numY-7];
  x_d[1] = x[2*numY-6];
  x_d[2] = x[2*numY-5];
  x_d[3] = x[2*numY-4];
  x_d[4] = x[2*numY-3];
  x_d[5] = x[2*numY-2];
  x_d[6] = x[2*numY-1];

  y_d[0] = y_diez[2*numY-7];
  y_d[1] = y_diez[2*numY-6];
  y_d[2] = y_diez[2*numY-5];
  y_d[3] = y_diez[2*numY-4];
  y_d[4] = y_diez[2*numY-3];
  y_d[5] = y_diez[2*numY-2];
  y_d[6] = y_diez[2*numY-1];

  for (i=0,j=1;i<numY;i++,j+=2)
    y[i] = y_diez[j];


/*  
  y_diez[0] = b_diez[0]*x[0] + b_diez[1]*x_d[5] + b_diez[2]*x_d[4] + b_diez[3]*x_d[3]
            + b_diez[4]*x_d[2] + b_diez[5]*x_d[1] + b_diez[6]*x_d[0]
            - a_intr_diez[1]*y_d[5] - a_intr_diez[2]*y_d[4] - a_intr_diez[3]*y_d[3]
            - a_intr_diez[4]*y_d[2] - a_intr_diez[5]*y_d[1] - a_intr_diez[6]*y_d[0];
  
  y_diez[1] = b_diez[0]*x[1] + b_diez[1]*x[0] + b_diez[2]*x_d[5] + b_diez[3]*x_d[4]
            + b_diez[4]*x_d[3] + b_diez[5]*x_d[2] + b_diez[6]*x_d[1]
            - a_intr_diez[1]*y_diez[0] - a_intr_diez[2]*y_d[5] - a_intr_diez[3]*y_d[4]
            - a_intr_diez[4]*y_d[3] - a_intr_diez[5]*y_d[2] - a_intr_diez[6]*y_d[1];
  
  y_diez[2] = b_diez[0]*x[2] + b_diez[1]*x[1] + b_diez[2]*x[0] + b_diez[3]*x_d[5]
            + b_diez[4]*x_d[4] + b_diez[5]*x_d[3] + b_diez[6]*x_d[2]
            - a_intr_diez[1]*y_diez[1] - a_intr_diez[2]*y_diez[0] - a_intr_diez[3]*y_d[5]
            - a_intr_diez[4]*y_d[4] - a_intr_diez[5]*y_d[3] - a_intr_diez[6]*y_d[2];
  
  y_diez[3] = b_diez[0]*x[3] + b_diez[1]*x[2] + b_diez[2]*x[1] + b_diez[3]*x[0]
            + b_diez[4]*x_d[5] + b_diez[5]*x_d[4] + b_diez[6]*x_d[3]
            - a_intr_diez[1]*y_diez[2] - a_intr_diez[2]*y_diez[1] - a_intr_diez[3]*y_diez[0]
            - a_intr_diez[4]*y_d[5] - a_intr_diez[5]*y_d[4] - a_intr_diez[6]*y_d[3];
  
  y_diez[4] = b_diez[0]*x[4] + b_diez[1]*x[3] + b_diez[2]*x[2] + b_diez[3]*x[1]
            + b_diez[4]*x[0] + b_diez[5]*x_d[5] + b_diez[6]*x_d[4]
            - a_intr_diez[1]*y_diez[3] - a_intr_diez[2]*y_diez[2] - a_intr_diez[3]*y_diez[1]
            - a_intr_diez[4]*y_diez[0] - a_intr_diez[5]*y_d[5] - a_intr_diez[6]*y_d[4];
  
  y_diez[5] = b_diez[0]*x[5] + b_diez[1]*x[4] + b_diez[2]*x[3] + b_diez[3]*x[2]
            + b_diez[4]*x[1] + b_diez[5]*x[0] + b_diez[6]*x_d[5]
            - a_intr_diez[1]*y_diez[4] - a_intr_diez[2]*y_diez[3] - a_intr_diez[3]*y_diez[2]
            - a_intr_diez[4]*y_diez[1] - a_intr_diez[5]*y_diez[0] - a_intr_diez[6]*y_d[5];
  
  
  for (i=6;i<numY*2;i++)
  {
    y_diez[i] = b_diez[0]*x[i] + b_diez[1]*x[i-1] + b_diez[2]*x[i-2]
              + b_diez[3]*x[i-3] + b_diez[4]*x[i-4] + b_diez[5]*x[i-5]
              + b_diez[6]*x[i-6]
              - a_intr_diez[1]*y_diez[i-1] - a_intr_diez[2]*y_diez[i-2]
              - a_intr_diez[3]*y_diez[i-3] - a_intr_diez[4]*y_diez[i-4]
              - a_intr_diez[5]*y_diez[i-5] - a_intr_diez[6]*y_diez[i-6];
  }
  
  x_d[0] = x[2*numY-6];
  x_d[1] = x[2*numY-5];
  x_d[2] = x[2*numY-4];
  x_d[3] = x[2*numY-3];
  x_d[4] = x[2*numY-2];
  x_d[5] = x[2*numY-1];

  y_d[0] = y_diez[2*numY-6];
  y_d[1] = y_diez[2*numY-5];
  y_d[2] = y_diez[2*numY-4];
  y_d[3] = y_diez[2*numY-3];
  y_d[4] = y_diez[2*numY-2];
  y_d[5] = y_diez[2*numY-1];

  for (i=0,j=1;i<numY;i++,j+=2)
    y[i] = y_diez[j];
*/
/*
  float y_diez[TAMTRAMA];
  
  y[0] = b_intr_diez[0]*x[0] + b_intr_diez[1]*x_d[2] + b_intr_diez[2]*x_d[1] + b_intr_diez[3]*x_d[0]
                   - a_intr_diez[1]*y_d[2] - a_intr_diez[2]*y_d[1] - a_intr_diez[3]*y_d[0];
  
  y_diez[0] = b_intr_diez[0]*x[1] + b_intr_diez[1]*x[0] + b_intr_diez[2]*x_d[2] + b_intr_diez[3]*x_d[1]
                   - a_intr_diez[1]*y[0] - a_intr_diez[2]*y_d[2] - a_intr_diez[3]*y_d[1];
  
  y[1] = b_intr_diez[0]*x[2] + b_intr_diez[1]*x[1] + b_intr_diez[2]*x[0] + b_intr_diez[3]*x_d[1]
                   - a_intr_diez[1]*y_diez[0] - a_intr_diez[2]*y[0] - a_intr_diez[3]*y_d[2];
  
  y_diez[1] = b_intr_diez[0]*x[3] + b_intr_diez[1]*x[2] + b_intr_diez[2]*x[1] + b_intr_diez[3]*x[0]
                   - a_intr_diez[1]*y[1] - a_intr_diez[2]*y_diez[0] - a_intr_diez[3]*y[0];

  for (j=2,i=4;j<numY;i++,j++)
  {
    y[j] = b_intr_diez[0]*x[i] + b_intr_diez[1]*x[i-1] + b_intr_diez[2]*x[i-2] + b_intr_diez[3]*x[i-3]
                   - a_intr_diez[1]*y_diez[j-1] - a_intr_diez[2]*y[j-1] - a_intr_diez[3]*y_diez[j-2];

    y_diez[j] = b_intr_diez[0]*x[i] + b_intr_diez[1]*x[i-1] + b_intr_diez[2]*x[i-2] + b_intr_diez[3]*x[i-3]
                     - a_intr_diez[1]*y[j] - a_intr_diez[2]*y_diez[j-1] - a_intr_diez[3]*y[j-1];
  }
  
  x_d[0] = x[2*numY-3];
  x_d[1] = x[2*numY-2];
  x_d[2] = x[2*numY-1];

  y_d[0] = y_diez[numY-2];
  y_d[1] = y[numY-1];
  y_d[2] = y_diez[numY-1];
*/
}


/**********************************************************

	Paso alto: interpolación de las muestras de salida
	
***********************************************************/
void iir_interp(float *x, float *y, float *x_d, float *y_d, int numY)
{
  int i,j;
  float x_interp[TAMTRAMA*2];

  for (i=0,j=0;i<numY;i++)
  {
    x_interp[j++] = x[i];
    x_interp[j++] = 0;
  }

  y[0] = b_interp[0]*x_interp[0] + b_interp[1]*x_d[6] + b_interp[2]*x_d[5] + b_interp[3]*x_d[4] +
         b_interp[4]*x_d[3] + b_interp[5]*x_d[2] + b_interp[6]*x_d[1] + b_interp[7]*x_d[0] -  
         a_intr_diez[1]*y_d[6] - a_intr_diez[2]*y_d[5] - a_intr_diez[3]*y_d[4] - a_intr_diez[4]*y_d[3] -
         a_intr_diez[5]*y_d[2] - a_intr_diez[6]*y_d[1] - a_intr_diez[7]*y_d[0];

  y[1] = b_interp[0]*x_interp[1] + b_interp[1]*x_interp[0] + b_interp[2]*x_d[6] + b_interp[3]*x_d[5] +
         b_interp[4]*x_d[4] + b_interp[5]*x_d[3] + b_interp[6]*x_d[2] + b_interp[7]*x_d[1] -  
         a_intr_diez[1]*y[0] - a_intr_diez[2]*y_d[6] - a_intr_diez[3]*y_d[5] - a_intr_diez[4]*y_d[4] -
         a_intr_diez[5]*y_d[3] - a_intr_diez[6]*y_d[2] - a_intr_diez[7]*y_d[1];

  y[2] = b_interp[0]*x_interp[2] + b_interp[1]*x_interp[1] + b_interp[2]*x_interp[0] + b_interp[3]*x_d[6] +
         b_interp[4]*x_d[5] + b_interp[5]*x_d[4] + b_interp[6]*x_d[3] + b_interp[7]*x_d[2] -  
         a_intr_diez[1]*y[1] - a_intr_diez[2]*y[0] - a_intr_diez[3]*y_d[6] - a_intr_diez[4]*y_d[5] -
         a_intr_diez[5]*y_d[4] - a_intr_diez[6]*y_d[3] - a_intr_diez[7]*y_d[2];

  y[3] = b_interp[0]*x_interp[3] + b_interp[1]*x_interp[2] + b_interp[2]*x_interp[1] + b_interp[3]*x_interp[0] +
         b_interp[4]*x_d[6] + b_interp[5]*x_d[5] + b_interp[6]*x_d[4] + b_interp[7]*x_d[3] -  
         a_intr_diez[1]*y[2] - a_intr_diez[2]*y[1] - a_intr_diez[3]*y[0] - a_intr_diez[4]*y_d[6] -
         a_intr_diez[5]*y_d[5] - a_intr_diez[6]*y_d[4] - a_intr_diez[7]*y_d[3];

  y[4] = b_interp[0]*x_interp[4] + b_interp[1]*x_interp[3] + b_interp[2]*x_interp[2] + b_interp[3]*x_interp[1] +
         b_interp[4]*x_interp[0] + b_interp[5]*x_d[6] + b_interp[6]*x_d[5] + b_interp[7]*x_d[4] -  
         a_intr_diez[1]*y[3] - a_intr_diez[2]*y[2] - a_intr_diez[3]*y[1] - a_intr_diez[4]*y[0] -
         a_intr_diez[5]*y_d[6] - a_intr_diez[6]*y_d[5] - a_intr_diez[7]*y_d[4];

  y[5] = b_interp[0]*x_interp[5] + b_interp[1]*x_interp[4] + b_interp[2]*x_interp[3] + b_interp[3]*x_interp[2] +
         b_interp[4]*x_interp[1] + b_interp[5]*x_interp[0] + b_interp[6]*x_d[6] + b_interp[7]*x_d[5] -  
         a_intr_diez[1]*y[4] - a_intr_diez[2]*y[3] - a_intr_diez[3]*y[2] - a_intr_diez[4]*y[1] -
         a_intr_diez[5]*y[0] - a_intr_diez[6]*y_d[6] - a_intr_diez[7]*y_d[5];

  y[6] = b_interp[0]*x_interp[6] + b_interp[1]*x_interp[5] + b_interp[2]*x_interp[4] + b_interp[3]*x_interp[3] +
         b_interp[4]*x_interp[2] + b_interp[5]*x_interp[1] + b_interp[6]*x_interp[0] + b_interp[7]*x_d[6] -  
         a_intr_diez[1]*y[5] - a_intr_diez[2]*y[4] - a_intr_diez[3]*y[3] - a_intr_diez[4]*y[2] -
         a_intr_diez[5]*y[1] - a_intr_diez[6]*y[0] - a_intr_diez[7]*y_d[6];

  for (i=7;i<numY*2;i++)
    y[i] = b_interp[0]*x_interp[i] + b_interp[1]*x_interp[i-1] + b_interp[2]*x_interp[i-2] + b_interp[3]*x_interp[i-3] +
         b_interp[4]*x_interp[i-4] + b_interp[5]*x_interp[i-5] + b_interp[6]*x_interp[i-6] + b_interp[7]*x_interp[i-7] -  
         a_intr_diez[1]*y[i-1] - a_intr_diez[2]*y[i-2] - a_intr_diez[3]*y[i-3] - a_intr_diez[4]*y[i-4] -
         a_intr_diez[5]*y[i-5] - a_intr_diez[6]*y[i-6] - a_intr_diez[7]*y[i-7];
  

  x_d[0] = x_interp[2*numY-7];
  x_d[1] = x_interp[2*numY-6];
  x_d[2] = x_interp[2*numY-5];
  x_d[3] = x_interp[2*numY-4];
  x_d[4] = x_interp[2*numY-3];
  x_d[5] = x_interp[2*numY-2];
  x_d[6] = x_interp[2*numY-1];

  y_d[0] = y[2*numY-7];
  y_d[1] = y[2*numY-6];
  y_d[2] = y[2*numY-5];
  y_d[3] = y[2*numY-4];
  y_d[4] = y[2*numY-3];
  y_d[5] = y[2*numY-2];
  y_d[6] = y[2*numY-1];


/*
  y[0] = b_interp[0]*x_interp[0] + b_interp[1]*x_d[5] + b_interp[2]*x_d[4] + b_interp[3]*x_d[3]
            + b_interp[4]*x_d[2] + b_interp[5]*x_d[1] + b_interp[6]*x_d[0]
            - a_intr_diez[1]*y_d[5] - a_intr_diez[2]*y_d[4] - a_intr_diez[3]*y_d[3]
            - a_intr_diez[4]*y_d[2] - a_intr_diez[5]*y_d[1] - a_intr_diez[6]*y_d[0];
  
  y[1] = b_interp[0]*x_interp[1] + b_interp[1]*x_interp[0] + b_interp[2]*x_d[5] + b_interp[3]*x_d[4]
            + b_interp[4]*x_d[3] + b_interp[5]*x_d[2] + b_interp[6]*x_d[1]
            - a_intr_diez[1]*y[0] - a_intr_diez[2]*y_d[5] - a_intr_diez[3]*y_d[4]
            - a_intr_diez[4]*y_d[3] - a_intr_diez[5]*y_d[2] - a_intr_diez[6]*y_d[1];
  
  y[2] = b_interp[0]*x_interp[2] + b_interp[1]*x_interp[1] + b_interp[2]*x_interp[0] + b_interp[3]*x_d[5]
            + b_interp[4]*x_d[4] + b_interp[5]*x_d[3] + b_interp[6]*x_d[2]
            - a_intr_diez[1]*y[1] - a_intr_diez[2]*y[0] - a_intr_diez[3]*y_d[5]
            - a_intr_diez[4]*y_d[4] - a_intr_diez[5]*y_d[3] - a_intr_diez[6]*y_d[2];
  
  y[3] = b_interp[0]*x_interp[3] + b_interp[1]*x_interp[2] + b_interp[2]*x_interp[1] + b_interp[3]*x_interp[0]
            + b_interp[4]*x_d[5] + b_interp[5]*x_d[4] + b_interp[6]*x_d[3]
            - a_intr_diez[1]*y[2] - a_intr_diez[2]*y[1] - a_intr_diez[3]*y[0]
            - a_intr_diez[4]*y_d[5] - a_intr_diez[5]*y_d[4] - a_intr_diez[6]*y_d[3];
  
  y[4] = b_interp[0]*x_interp[4] + b_interp[1]*x_interp[3] + b_interp[2]*x_interp[2] + b_interp[3]*x_interp[1]
            + b_interp[4]*x_interp[0] + b_interp[5]*x_d[5] + b_interp[6]*x_d[4]
            - a_intr_diez[1]*y[3] - a_intr_diez[2]*y[2] - a_intr_diez[3]*y[1]
            - a_intr_diez[4]*y[0] - a_intr_diez[5]*y_d[5] - a_intr_diez[6]*y_d[4];
  
  y[5] = b_interp[0]*x_interp[5] + b_interp[1]*x_interp[4] + b_interp[2]*x_interp[3] + b_interp[3]*x_interp[2]
            + b_interp[4]*x_interp[1] + b_interp[5]*x_interp[0] + b_interp[6]*x_d[5]
            - a_intr_diez[1]*y[4] - a_intr_diez[2]*y[3] - a_intr_diez[3]*y[2]
            - a_intr_diez[4]*y[1] - a_intr_diez[5]*y[0] - a_intr_diez[6]*y_d[5];
  
  
  for (i=6;i<numY*2;i++)
  {
    y[i] = b_interp[0]*x_interp[i] + b_interp[1]*x_interp[i-1] + b_interp[2]*x_interp[i-2]
              + b_interp[3]*x_interp[i-3] + b_interp[4]*x_interp[i-4]
              + b_interp[5]*x_interp[i-5] + b_interp[6]*x_interp[i-6]
              - a_intr_diez[1]*y[i-1] - a_intr_diez[2]*y[i-2]
              - a_intr_diez[3]*y[i-3] - a_intr_diez[4]*y[i-4]
              - a_intr_diez[5]*y[i-5] - a_intr_diez[6]*y[i-6];
  }
  
  x_d[0] = x_interp[2*numY-6];
  x_d[1] = x_interp[2*numY-5];
  x_d[2] = x_interp[2*numY-4];
  x_d[3] = x_interp[2*numY-3];
  x_d[4] = x_interp[2*numY-2];
  x_d[5] = x_interp[2*numY-1];

  y_d[0] = y[2*numY-6];
  y_d[1] = y[2*numY-5];
  y_d[2] = y[2*numY-4];
  y_d[3] = y[2*numY-3];
  y_d[4] = y[2*numY-2];
  y_d[5] = y[2*numY-1];
*/
/*  
  y[0] = b_interp[0]*x_interp[0] + b_interp[1]*x_d[2] + b_interp[2]*x_d[1] + b_interp[3]*x_d[0]
                   - a_intr_diez[1]*y_d[2] - a_intr_diez[2]*y_d[1] - a_intr_diez[3]*y_d[0];
  
  y[1] = b_interp[0]*x_interp[1] + b_interp[1]*x_interp[0] + b_interp[2]*x_d[2] + b_interp[3]*x_d[1]
                   - a_intr_diez[1]*y[0] - a_intr_diez[2]*y_d[2] - a_intr_diez[3]*y_d[1];
  
  y[2] = b_interp[0]*x_interp[2] + b_interp[1]*x_interp[1] + b_interp[2]*x_interp[0] + b_interp[3]*x_d[2]
                   - a_intr_diez[1]*y[1] - a_intr_diez[2]*y[0] - a_intr_diez[3]*y_d[2];
  
  for (i=3;i<numY*2;i++)
    y[i] = b_interp[0]*x_interp[i] + b_interp[1]*x_interp[i-1] + b_interp[2]*x_interp[i-2] + b_interp[3]*x_interp[i-3]
                   - a_intr_diez[1]*y[i-1] - a_intr_diez[2]*y[i-2] - a_intr_diez[3]*y[i-3];
  
  x_d[0] = x_interp[2*numY-3];
  x_d[1] = x_interp[2*numY-2];
  x_d[2] = x_interp[2*numY-1];

  y_d[0] = y[2*numY-3];
  y_d[1] = y[2*numY-2];
  y_d[2] = y[2*numY-1];
*/  

/*
  y[0] = b_intr_diez[0]*x[0] + b_intr_diez[1]*x_d[2] + b_intr_diez[2]*x_d[1] + b_intr_diez[3]*x_d[0]
                   - a_intr_diez[1]*y_d[2] - a_intr_diez[2]*y_d[1] - a_intr_diez[3]*y_d[0];
  
  y[1] = b_intr_diez[0]*0 + b_intr_diez[1]*x[0] + b_intr_diez[2]*x_d[2] + b_intr_diez[3]*x_d[1]
                   - a_intr_diez[1]*y[0] - a_intr_diez[2]*y_d[2] - a_intr_diez[3]*y_d[1];
  
  y[2] = b_intr_diez[0]*x[1] + b_intr_diez[1]*0 + b_intr_diez[2]*x[0] + b_intr_diez[3]*x_d[1]
                   - a_intr_diez[1]*y[1] - a_intr_diez[2]*y[0] - a_intr_diez[3]*y_d[2];
  
  y[3] = b_intr_diez[0]*0 + b_intr_diez[1]*x[1] + b_intr_diez[2]*0 + b_intr_diez[3]*x[0]
                   - a_intr_diez[1]*y[2] - a_intr_diez[2]*y[1] - a_intr_diez[3]*y[0];

  for (j=4,i=2;i<numY;i++,j++)
  {
    y[j] = b_intr_diez[0]*x[i] + b_intr_diez[1]*0 + b_intr_diez[2]*x[i-1] + b_intr_diez[3]*0
                   - a_intr_diez[1]*y[j-1] - a_intr_diez[2]*y[j-2] - a_intr_diez[3]*y[j-3];
    j++;
    
    y[j] = b_intr_diez[0]*0 + b_intr_diez[1]*x[i] + b_intr_diez[2]*0 + b_intr_diez[3]*x[i-1]
                   - a_intr_diez[1]*y[j-1] - a_intr_diez[2]*y[j-2] - a_intr_diez[3]*y[j-3];
  }
  
  x_d[0] = 0;
  x_d[1] = x[numY-1];
  x_d[2] = 0;

  y_d[0] = y[2*numY-3];
  y_d[1] = y[2*numY-2];
  y_d[2] = y[2*numY-1];
*/
}

/**********************************************************

	Notch utilizado en los protocolos de LCE y R2. Para
	eliminar la componente de 2280Hz del audio transmitido
	y para eliminar la misma componente del audio recibido
	que es direccionado	al handset del equipo.
	
***********************************************************/
void iir_2280(float *x, float *y, float *b, float *a, float *x_d, float *y_d, int numY)
{
  int i;
  
  y[0] = b[0]*x[0] + b[1]*x_d[1] + b[2]*x_d[0] - a[1]*y_d[1] - a[2]*y_d[0];
  y[1] = b[0]*x[1] + b[1]*x[0] + b[2]*x_d[1] - a[1]*y[0] - a[2]*y_d[1];
  
  for (i=2;i<numY;i++)
    y[i] = b[0]*x[i] + b[1]*x[i-1] + b[2]*x[i-2] - a[1]*y[i-1] - a[2]*y[i-2];
  
  x_d[0] = x[numY-2];
  x_d[1] = x[numY-1];
  y_d[0] = y[numY-2];
  y_d[1] = y[numY-1];

}

/**********************************************************

	Notch utilizado en el protocolo N5. Para eliminar las
	componentes de 2400Hz y 2600Hz del audio transmitido
	y para eliminar las mismas componentes del audio
	recibido que es direccionado al handset del equipo.
	
***********************************************************/
void iir_2400_2600(float *x, float *y, float *b, float *a, float *x_d, float *y_d, int numY)
{
  int i;
  
  y[0] = b[0]*x[0] + b[1]*x_d[3] + b[2]*x_d[2] + b[3]*x_d[1] + b[4]*x_d[0] 
                   - a[1]*y_d[3] - a[2]*y_d[2] - a[3]*y_d[1] - a[4]*y_d[0];
  y[1] = b[0]*x[1] + b[1]*x[0] + b[2]*x_d[3] + b[3]*x_d[2] + b[4]*x_d[1]
                   - a[1]*y[0] - a[2]*y_d[3] - a[3]*y_d[2] - a[4]*y_d[1];
  y[2] = b[0]*x[2] + b[1]*x[1] + b[2]*x[0] + b[3]*x_d[3] + b[4]*x_d[2]
                   - a[1]*y[1] - a[2]*y[0] - a[3]*y_d[3] - a[4]*y_d[2];
  y[3] = b[0]*x[3] + b[1]*x[2] + b[2]*x[1] + b[3]*x[0] + b[4]*x_d[3]
                   - a[1]*y[2] - a[2]*y[1] - a[3]*y[0] - a[4]*y_d[3];
  
  for (i=4;i<numY;i++)
    y[i] = b[0]*x[i] + b[1]*x[i-1] + b[2]*x[i-2] + b[3]*x[i-3] + b[4]*x[i-4]
                     - a[1]*y[i-1] - a[2]*y[i-2] - a[3]*y[i-3] - a[4]*y[i-4];
  
  x_d[0] = x[numY-4];
  x_d[1] = x[numY-3];
  x_d[2] = x[numY-2];
  x_d[3] = x[numY-1];

  y_d[0] = y[numY-4];
  y_d[1] = y[numY-3];
  y_d[2] = y[numY-2];
  y_d[3] = y[numY-1];

}

/**********************************************************

	Utilizado para eliminar el tono de 
	invitación a marcar generado por la centralita
	Se utiliza en la decodificación de DTMF para poder
	seguir las variaciones de nivel de los dígitos MF
	que van superpuestos a dicho tono.
	
***********************************************************/
void iir_425(float *x, float *y, float *b, float *a, float *x_d, float *y_d, int numY)
{
  int i;
  
  y[0] = b[0]*x[0] + b[1]*x_d[1] + b[2]*x_d[0] - a[1]*y_d[1] - a[2]*y_d[0];
  y[1] = b[0]*x[1] + b[1]*x[0] + b[2]*x_d[1] - a[1]*y[0] - a[2]*y_d[1];
  
  for (i=2;i<numY;i++)
    y[i] = b[0]*x[i] + b[1]*x[i-1] + b[2]*x[i-2] - a[1]*y[i-1] - a[2]*y[i-2];
  
  x_d[0] = x[numY-2];
  x_d[1] = x[numY-1];
  y_d[0] = y[numY-2];
  y_d[1] = y[numY-1];

}


/**********************************************************

	Paso Alto utilizado para eliminar el tono de 
	invitación a marcar generado por la centralita
	y el elevado ruido de BF que hay en las líneas
	Se utiliza en la decodificación de DTMF para poder
	seguir las variaciones de nivel de los dígitos MF
	que van superpuestos a dicho tono.
	
***********************************************************/
void iir_DTMF(float *x, float *y, float *b, float *a, float *x_d, float *y_d, int numY)
{
  int i,j;
  
  y[0] = b[0]*x[0] + b[1]*x_d[6] + b[2]*x_d[5] + b[3]*x_d[4] +
         b[4]*x_d[3] + b[5]*x_d[2] + b[6]*x_d[1] + b[7]*x_d[0] -  
         a[1]*y_d[6] - a[2]*y_d[5] - a[3]*y_d[4] - a[4]*y_d[3] -
         a[5]*y_d[2] - a[6]*y_d[1] - a[7]*y_d[0];

  y[1] = b[0]*x[1] + b[1]*x[0] + b[2]*x_d[6] + b[3]*x_d[5] +
         b[4]*x_d[4] + b[5]*x_d[3] + b[6]*x_d[2] + b[7]*x_d[1] -  
         a[1]*y[0] - a[2]*y_d[6] - a[3]*y_d[5] - a[4]*y_d[4] -
         a[5]*y_d[3] - a[6]*y_d[2] - a[7]*y_d[1];

  y[2] = b[0]*x[2] + b[1]*x[1] + b[2]*x[0] + b[3]*x_d[6] +
         b[4]*x_d[5] + b[5]*x_d[4] + b[6]*x_d[3] + b[7]*x_d[2] -  
         a[1]*y[1] - a[2]*y[0] - a[3]*y_d[6] - a[4]*y_d[5] -
         a[5]*y_d[4] - a[6]*y_d[3] - a[7]*y_d[2];

  y[3] = b[0]*x[3] + b[1]*x[2] + b[2]*x[1] + b[3]*x[0] +
         b[4]*x_d[6] + b[5]*x_d[5] + b[6]*x_d[4] + b[7]*x_d[3] -  
         a[1]*y[2] - a[2]*y[1] - a[3]*y[0] - a[4]*y_d[6] -
         a[5]*y_d[5] - a[6]*y_d[4] - a[7]*y_d[3];

  y[4] = b[0]*x[4] + b[1]*x[3] + b[2]*x[2] + b[3]*x[1] +
         b[4]*x[0] + b[5]*x_d[6] + b[6]*x_d[5] + b[7]*x_d[4] -  
         a[1]*y[3] - a[2]*y[2] - a[3]*y[1] - a[4]*y[0] -
         a[5]*y_d[6] - a[6]*y_d[5] - a[7]*y_d[4];

  y[5] = b[0]*x[5] + b[1]*x[4] + b[2]*x[3] + b[3]*x[2] +
         b[4]*x[1] + b[5]*x[0] + b[6]*x_d[6] + b[7]*x_d[5] -  
         a[1]*y[4] - a[2]*y[3] - a[3]*y[2] - a[4]*y[1] -
         a[5]*y[0] - a[6]*y_d[6] - a[7]*y_d[5];

  y[6] = b[0]*x[6] + b[1]*x[5] + b[2]*x[4] + b[3]*x[3] +
         b[4]*x[2] + b[5]*x[1] + b[6]*x[0] + b[7]*x_d[6] -  
         a[1]*y[5] - a[2]*y[4] - a[3]*y[3] - a[4]*y[2] -
         a[5]*y[1] - a[6]*y[0] - a[7]*y_d[6];

  for (i=7;i<numY;i++)
    y[i] = b[0]*x[i] + b[1]*x[i-1] + b[2]*x[i-2] + b[3]*x[i-3] +
         b[4]*x[i-4] + b[5]*x[i-5] + b[6]*x[i-6] + b[7]*x[i-7] -  
         a[1]*y[i-1] - a[2]*y[i-2] - a[3]*y[i-3] - a[4]*y[i-4] -
         a[5]*y[i-5] - a[6]*y[i-6] - a[7]*y[i-7];
  

  for (i=0,j=7;i<7;i++,j--)
  {
    x_d[i] = x[numY-j];  
    y_d[i] = y[numY-j];
  }
/*
  x_d[0] = x[numY-7];
  x_d[1] = x[numY-6];
  x_d[2] = x[numY-5];
  x_d[3] = x[numY-4];
  x_d[4] = x[numY-3];
  x_d[5] = x[numY-2];
  x_d[6] = x[numY-1];

  y_d[0] = y[numY-7];
  y_d[1] = y[numY-6];
  y_d[2] = y[numY-5];
  y_d[3] = y[numY-4];
  y_d[4] = y[numY-3];
  y_d[5] = y[numY-2];
  y_d[6] = y[numY-1];
*/
}



/**********************************************************

	Filtro de diezmado usado con BL para realizar el
	cálculo del espectro en tiempo real. La relación
	de diezmado es de 20/1 (se pasa de un ancho de banda
	de 4000Hz a 200Hz)
	
***********************************************************/
void iir_BL(float *x, float *y, float *b, float *a, float *x_d, float *y_d, int numY)
{
  int i;
  
  y[0] = b[0]*x[0] + b[1]*x_d[3] + b[2]*x_d[2] + b[3]*x_d[1] + b[4]*x_d[0] 
                   - a[1]*y_d[3] - a[2]*y_d[2] - a[3]*y_d[1] - a[4]*y_d[0];
  y[1] = b[0]*x[1] + b[1]*x[0] + b[2]*x_d[3] + b[3]*x_d[2] + b[4]*x_d[1]
                   - a[1]*y[0] - a[2]*y_d[3] - a[3]*y_d[2] - a[4]*y_d[1];
  y[2] = b[0]*x[2] + b[1]*x[1] + b[2]*x[0] + b[3]*x_d[3] + b[4]*x_d[2]
                   - a[1]*y[1] - a[2]*y[0] - a[3]*y_d[3] - a[4]*y_d[2];
  y[3] = b[0]*x[3] + b[1]*x[2] + b[2]*x[1] + b[3]*x[0] + b[4]*x_d[3]
                   - a[1]*y[2] - a[2]*y[1] - a[3]*y[0] - a[4]*y_d[3];
  
  for (i=4;i<numY;i++)
    y[i] = b[0]*x[i] + b[1]*x[i-1] + b[2]*x[i-2] + b[3]*x[i-3] + b[4]*x[i-4]
                     - a[1]*y[i-1] - a[2]*y[i-2] - a[3]*y[i-3] - a[4]*y[i-4];
  
  x_d[0] = x[numY-4];
  x_d[1] = x[numY-3];
  x_d[2] = x[numY-2];
  x_d[3] = x[numY-1];

  y_d[0] = y[numY-4];
  y_d[1] = y[numY-3];
  y_d[2] = y[numY-2];
  y_d[3] = y[numY-1];

}


/****************************************************************

	Paso Bajo utilizado para eliminar tonos superiores
	a 2700Hz:
    
       Audio(2700Hz)+Sel Tx+Sel Rx --> [ PB ] --> Audio
    
    ENTRADAS:
      float *x: puntero a las muestras de entrada
      float *b: coeficientes del numerador
      float *a: coeficientes del denominador
      float *x_d: puntero a las muestras de entrada retrasadas
                  (trama anterior)
      float *y_d: puntero a las muestras filtradas retrasadas
                  (trama anterior)
      int numY: número de muestras de entrada, debe ser >7
    
    ENTRADAS:
      float *y: puntero a las muestras ya filtradas
      float *x_d: puntero a las muestras de entrada retrasadas
                  (últimas de la trama actual)
      float *y_d: puntero a las muestras filtradas retrasadas
                  (últimas de la trama actual)

    LLAMADA:
    
    iir_2700(inF,inF_filtrada,b_2700,a_2700,in_d,out_d,TAMTRAMA)
    
*****************************************************************/


void iir_paso_bajo2700(float *x, float *y, float *b, float *a, float *x_d, float *y_d, int numY)
{
  int i=0, j=0;
  
  //float y_paso_bajo_2700[TAMTRAMA*2]; 
  
  
  
  y[0] = b[0]*x[0] + b[1]*x_d[6] + b[2]*x_d[5] + b[3]*x_d[4] +
         b[4]*x_d[3] + b[5]*x_d[2] + b[6]*x_d[1] + b[7]*x_d[0] -  
         a[1]*y_d[6] - a[2]*y_d[5] - a[3]*y_d[4] - a[4]*y_d[3] -
         a[5]*y_d[2] - a[6]*y_d[1] - a[7]*y_d[0];

  y[1] = b[0]*x[1] + b[1]*x[0] + b[2]*x_d[6] + b[3]*x_d[5] +
         b[4]*x_d[4] + b[5]*x_d[3] + b[6]*x_d[2] + b[7]*x_d[1] -  
         a[1]*y[0] - a[2]*y_d[6] - a[3]*y_d[5] - a[4]*y_d[4] -
         a[5]*y_d[3] - a[6]*y_d[2] - a[7]*y_d[1];

  y[2] = b[0]*x[2] + b[1]*x[1] + b[2]*x[0] + b[3]*x_d[6] +
         b[4]*x_d[5] + b[5]*x_d[4] + b[6]*x_d[3] + b[7]*x_d[2] -  
         a[1]*y[1] - a[2]*y[0] - a[3]*y_d[6] - a[4]*y_d[5] -
         a[5]*y_d[4] - a[6]*y_d[3] - a[7]*y_d[2];

  y[3] = b[0]*x[3] + b[1]*x[2] + b[2]*x[1] + b[3]*x[0] +
         b[4]*x_d[6] + b[5]*x_d[5] + b[6]*x_d[4] + b[7]*x_d[3] -  
         a[1]*y[2] - a[2]*y[1] - a[3]*y[0] - a[4]*y_d[6] -
         a[5]*y_d[5] - a[6]*y_d[4] - a[7]*y_d[3];

  y[4] = b[0]*x[4] + b[1]*x[3] + b[2]*x[2] + b[3]*x[1] +
         b[4]*x[0] + b[5]*x_d[6] + b[6]*x_d[5] + b[7]*x_d[4] -  
         a[1]*y[3] - a[2]*y[2] - a[3]*y[1] - a[4]*y[0] -
         a[5]*y_d[6] - a[6]*y_d[5] - a[7]*y_d[4];

  y[5] = b[0]*x[5] + b[1]*x[4] + b[2]*x[3] + b[3]*x[2] +
         b[4]*x[1] + b[5]*x[0] + b[6]*x_d[6] + b[7]*x_d[5] -  
         a[1]*y[4] - a[2]*y[3] - a[3]*y[2] - a[4]*y[1] -
         a[5]*y[0] - a[6]*y_d[6] - a[7]*y_d[5];

  y[6] = b[0]*x[6] + b[1]*x[5] + b[2]*x[4] + b[3]*x[3] +
         b[4]*x[2] + b[5]*x[1] + b[6]*x[0] + b[7]*x_d[6] -  
         a[1]*y[5] - a[2]*y[4] - a[3]*y[3] - a[4]*y[2] -
         a[5]*y[1] - a[6]*y[0] - a[7]*y_d[6];

  
  for (i=7;i<numY;i++)  
  {
    y[i] = b[0]*x[i] + b[1]*x[i-1] + b[2]*x[i-2] + b[3]*x[i-3] +
         b[4]*x[i-4] + b[5]*x[i-5] + b[6]*x[i-6] + b[7]*x[i-7] -  
         a[1]*y[i-1] - a[2]*y[i-2] - a[3]*y[i-3] - a[4]*y[i-4] -
         a[5]*y[i-5] - a[6]*y[i-6] - a[7]*y[i-7];         
  }  
  
 	for (i=0,j=7;i<7;i++,j--)
	{
	    x_d[i] = x[numY-j];  
	    y_d[i] = y[numY-j];
	}

 	//for (i=0,j=1;i<numY;i++,j+=2)
   // 	y[i] = y_paso_bajo_2700[j];
 

}

/****************************************************************

	Paso Alto eliptico orden 4 utilizado para eliminar:
	-tonos inferiores a 1020Hz en detección FSK en telefonía
	-ruido baja frecuencia por el micro del handset (fc=300Hz)
        
    ENTRADAS:
      float *x: puntero a las muestras de entrada
      float *b: coeficientes del numerador
      float *a: coeficientes del denominador
      float *x_d: puntero a las muestras de entrada retrasadas
                  (trama anterior)
      float *y_d: puntero a las muestras filtradas retrasadas
                  (trama anterior)
      int numY: número de muestras de entrada, debe ser >4
    
    ENTRADAS:
      float *y: puntero a las muestras ya filtradas
      float *x_d: puntero a las muestras de entrada retrasadas
                  (últimas de la trama actual)
      float *y_d: puntero a las muestras filtradas retrasadas
                  (últimas de la trama actual)

    LLAMADA:
    
    iir_paso_alto(inF,inF_filtrada,b_1020,a_1020,in_d,out_d,TAMTRAMA)
    iir_paso_alto(inF,inF_filtrada,b_300,a_300,in_d,out_d,TAMTRAMA)
*****************************************************************/

void iir_paso_alto(float *x, float *y, float *b, float *a, float *x_d, float *y_d, int numY)
{
  int i=0, j=0;
  
  y[0] = b[0]*x[0] + b[1]*x_d[3] + b[2]*x_d[2] + b[3]*x_d[1] + b[4]*x_d[0] -  
         a[1]*y_d[3] - a[2]*y_d[2] - a[3]*y_d[1] - a[4]*y_d[0];

  y[1] = b[0]*x[1] + b[1]*x[0] + b[2]*x_d[3] + b[3]*x_d[2] + b[4]*x_d[1] -  
         a[1]*y[0] - a[2]*y_d[3] - a[3]*y_d[2] - a[4]*y_d[1];

  y[2] = b[0]*x[2] + b[1]*x[1] + b[2]*x[0] + b[3]*x_d[3] + b[4]*x_d[2] -  
         a[1]*y[1] - a[2]*y[0] - a[3]*y_d[3] - a[4]*y_d[2];

  y[3] = b[0]*x[3] + b[1]*x[2] + b[2]*x[1] + b[3]*x[0] + b[4]*x_d[3] -  
         a[1]*y[2] - a[2]*y[1] - a[3]*y[0] - a[4]*y_d[3];

  for (i=4;i<numY;i++)  
  {
    y[i] = b[0]*x[i] + b[1]*x[i-1] + b[2]*x[i-2] + b[3]*x[i-3] + b[4]*x[i-4] -  
         a[1]*y[i-1] - a[2]*y[i-2] - a[3]*y[i-3] - a[4]*y[i-4];
  }  
  
 	for (i=0,j=4;i<4;i++,j--)
	{
	    x_d[i] = x[numY-j];  
	    y_d[i] = y[numY-j];
	}
}
#endif

/**
 * \brief		Implementación de un filtro IIR
 * \param x		puntero a las muestras de entrada
 * \param y 	puntero a las muestras ya filtradas
 * \param x_d	puntero a las muestras de entrada retrasadas (trama anterior)
 * \param y_d	puntero a las muestras filtradas retrasadas (trama anterior)
 * \param b		coeficientes del numerador
 * \param a		coeficientes del denominador
 * \param N		orden del filtro
 * \param numY	número de muestras de salida
 
 IIR de orden 2 utilizado para:
 - Notch a 2280Hz en los protocolos de LCE y R2
 - Notch tono de invitación a marcar. Decodificación DTMF
 
 IIR de orden 4 que usamos para:
 - Notch a 2400Hz y 2600Hz utilizado en el protocolo N5.
 - Filtro de diezmado usado con BL para calcular el espectro en tiempo real. La relación de diezmado es de 20/1
 (se pasa de un ancho de banda de 4000Hz a 200Hz)
 - Paso alto eliptico a 1020Hz para detección FSK en telefonía
 - Paso alto (fc=300Hz) para eliminar ruido baja frecuencia por el micro del handset
 
 IIR de orden 7:
 - Paso Alto utilizado para eliminar el tono de invitación a marcar generado por la centralita
 y el elevado ruido de BF que hay en las líneas.	Se utiliza en la decodificación de DTMF para poder
 seguir las variaciones de nivel de los dígitos MF que van superpuestos a dicho tono.	
 - Paso Bajo utilizado para eliminar tonos superiores a 2700Hz
 - Filtros diezmado e interpolación de la entrada/salida
 */
void iir(float *x, float *y, float *b, float *a, float *x_d, float *y_d, int N, int numY)
{
	int i,j;
	float *X, *Y;

	y[0] = b[0]*x[0];
	for (j = 1;j<=N;j++)
		y[0] += b[j]*x_d[N-j] - a[j]*y_d[N-j];
		 
	for (i=1;i<numY;i++)
	{
		y[i] = b[0]*x[i];
		X=&x[i-1];
		Y=&y[i-1];
		for (j = 1;j<=N;j++)
		{
			y[i] += b[j] * *X-- - a[j]* *Y--;
			if (i==j){
				X=&x_d[N-1];
				Y=&y_d[N-1];
			}
		}
	}
	for (i=0,j=N;i<N;i++,j--)
	{
		x_d[i] = x[numY-j];  
		y_d[i] = y[numY-j];
	}
}


/**
 * \brief		Implementación de un filtro FIR
 * \param x		puntero a las muestras de entrada
 * \param y 	puntero a las muestras ya filtradas
 * \param h		coeficientes del filtro
 * \param numH	número de coeficientes
 * \param numY	número de muestras de salida
 */
void fir(float *x, float *h, float *y, int numH, int numY)
{
   int i, j;
   float sum;

   for(j=0; j < numY; j++)
   {
      sum = 0;
      for(i=0; i < numH; i++)
      {
		sum += x[i+j] * h[i];
      }
      y[j] = sum;
   }
}
