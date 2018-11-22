#define CINULAS     0
#define CINONULAS   1

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/*
void init_iirdc(int L,int NC,int *dirx_delay,float *y_delay,float *base_x,int ci_init);
float iir_dc(int L,float *x,float *y,int Dirx_delay,float *y_delay);

void iir_diezm(float *x, float *y, float *x_d, float *y_d, int numY);

void iir_interp(float *x, float *y, float *x_d, float *y_d, int numY);

void iir_2280(float *x, float *y, float *b, float *a, float *x_d, float *y_d, int numY);

void iir_2400_2600(float *x, float *y, float *b, float *a, float *x_d, float *y_d, int numY);

void iir_425(float *x, float *y, float *b, float *a, float *x_d, float *y_d, int numY);

void iir_DTMF(float *x, float *y, float *b, float *a, float *x_d, float *y_d, int numY);

void iir_BL(float *x, float *y, float *b, float *a, float *x_d, float *y_d, int numY);

void iir_paso_bajo2700(float *x, float *y, float *b, float *a, float *x_d, float *y_d, int numY);

void iir_paso_alto(float *x, float *y, float *b, float *a, float *x_d, float *y_d, int numY);
*/

	#define iir_2400_2600(x,y,b,a,x_d,y_d,n) iir(x,y,b,a,x_d,y_d,4,n)
	#define iir_paso_alto(x,y,b,a,x_d,y_d,n) iir(x,y,b,a,x_d,y_d,4,n)

	extern float bpa1020[5];
	extern float apa1020[5];
	extern float a_DTMF[8];
	extern float b_DTMF[8];

	void iir(float *x, float *y, float *b, float *a, float *x_d, float *y_d, int N, int numY);
	void fir(float *x, float *h, float *y, int numH, int numY);


#ifdef __cplusplus
}
#endif // __cplusplus
