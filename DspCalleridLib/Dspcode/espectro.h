/*#define TARJETA*/

/* Constantes utilizadas en las funciones de cálculo del espectro */
#define LT         		256
#define LT2             128
#define LT4				64

#define PI              3.141592653589793
#define FREQ_MUEST		8000.0

#define RADIX           4

#define TAMFFT          64
#define LINDX           16

#define MAXTAMFFT       4096
#define MAXLINDX        64

#define LONG_SPECT		8192
#define LONG_SPECT2		4096
#define LONG_SPECT4		2048
                                 
#define UMBRAL_dB       -50
#define UMBRAL_dBx10    -500
/*
#define UMBRAL_dBV      -10
*/

#ifdef TARJETA
 /* 20*log10(2*128) */
 #define LOGLT           48.1648
 #define LOGLTx10        481.648
 #define CTE             0
 #define CTEx10          0
#else
 /* 20*log10(2*128)+CTE */
 #define LOGLT           127.4528/*127.453*/	/*127.6048*/
 #define LOGLTx10        1274.528
// jmgarcia 20090714  #define CTE             79.288		/*79.44*/	
 #define CTEx10          792.88
#endif

/* 20*log10(2*128)+CTEBL */
#define LOGLTBL          129.681406	/* 129.754*/ /*junio*/			/*129.81*/	/*129.7328*/	/*129.32178*/
#define LOGLTBLx10       1296.81406	/* 1297.54*/ /*junio*/
/* Se le suman 40 para equiparar en pantalla los dos espectros */
#define CTEBL            81.5166067 /* 81.589*/ /*junio*/				/*81.645*/	/*81.568*/		/*81.15698*/
#define CTEBLx10         815.166067 /* 815.89*/ /*junio*/


#define LONG_AUXBL		((LONG_SPECT2<<16)&0x1FFF0000) + MAXTAMFFT

void ajusta_nivel2280(float *niveles);
void cfftr4_dif(float* x, float* w, short n);
void digitrev_index(unsigned char *index, int n, int radix);
void digitrev(float *x, unsigned char *index, int n, int radix);
void init_fft(int N, float * coefW, float *coefA, float *coefB, unsigned char *indx);
void fft_real(int N, float * coefW, float *coefA, float *coefB, unsigned char *indx, float *x, short *y);
void fft_realBL(int N, float * coefW, float *coefA, float *coefB, unsigned char *indx, float *x, short *y);
void split1(int N, float *X, float *A, float *B, float *G);
