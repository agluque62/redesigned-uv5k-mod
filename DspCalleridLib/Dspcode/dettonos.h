

/* Se usan dos constantes distintas para comprobar la variaci�n de la envolvente:
	- Una se utiliza cuando el nivel de la se�al es bajo (SILENCIO) o alto (TONO)
	- La otra cuando el nivel de la se�al est� decreciendo (BAJADA). Su menor
	valor (->pendiente m�s suave) permite que la amortiguaci�n de la se�al al
	finalizar un tono no confunda al detector y que no detecte por tanto un nuevo
	tono que en realidad no existe.                                              */

#define ALFA			0.99F
#define ALFA_B			0.9F
#define	ALFA_MEDIDOR	0.95F	//para detectar niveles de hasta -35 dBm tras una multifrecuencia de -10 dBm


/* Definici�n de los distintos estados posibles por los que puede pasar el
   aut�mata encargado de la detecci�n de se�al en funci�n de los cambios
   de nivel en la l�nea                                                   */
#define SILENCIO	0
#define TONO		1
#define BAJADA		2


/* �stos son los umbrales utilizados en la detecci�n de se�al en funci�n de
   las variaciones de la envolvente:
   . UMBRAL1 es un valor absoluto, y al sobrepasarlo se considera que hay
     se�al distinta de ruido
   . UMBRAL2 es relativo al m�ximo, y marca el fin de la se�al
   . UMBRAL2_B se utiliza con el protocolo de LC, para detectar antes la
     finalizaci�n de la se�al                                              */
#define UMBRAL1	    2625//JJA PRUEBAS 5e6 // jmgarcia 20090706 2625/*2e5*/
#define UMBRAL1_B   2e5/*5e5*/
#define UMBRAL2	    0.7
#define UMBRAL2_B	0.851
#define UMBRAL2_D	0.877521
#define UMBRAL2_BL	0.4475	/* 15 Hz */

/* Constante utilizada en la detecci�n de multifrecuencia para evaluar
   la diferencia de nivel existente entre ambos tonos                 */
#define ctetwist    5.01e0


/* Constantes de conversi�n de niveles */
#define CTE_N5			117.3498 /*117.5018*/	/* 20*log10(N=80)+CTE */
#define CTE_LC			112.9128 /*113.0648*/	/* 20*log10(N=48)+CTE */
#define CTE_R2_DECO		121.4322 /*121.5842*/	/* 20*log10(N=128)+CTE */
#define CTE_R2_OPT		115.6789 /*115.8309*/	/* 20*log10(N=66)+CTE */
#define CTE_DTMF_DECO	121.4322 /*121.5842*/	/* 20*log10(N=128)+CTE */
#define CTE_DTMF_TR		115.6789 /*115.8309*/	/* 20*log10(N=66)+CTE */
#define CTE_DTMF64		115.4116

#define CTE_TM			124.9540 /*124.9540*/	/* 20*log10(N=90)+CTE */ /* Alcazar 124.9540 */
	
/*********** TELEMANDO-TELESE�ALIZACI�N ***************************/


/* Frecuencias de Transmisores */
#define Tono_2933		22 // Tx Reserva
#define Tono_2975		23 // Tx Autom�tico, PTT
#define Tono_3017		24 // Tx Principal


/* Frecuencias de Receptores */
#define Tono_3103		25 // Rx Reserva
#define Tono_3145		26 // Rx Autom�tico, SQ
#define Tono_3187		27 // Rx Principal

/**************** Protocolo TMTS *****************************************/

#define TRANSMISORES		1
#define RECEPTORES		0

/* Al realizar la decodificaci�n en BL, a pesar de que se calcula el espectro
   con un ancho de banda de 4000Hz, s�lo se estudian los primeros 205 puntos
   correspondientes a los 200Hz que se ven en t real                         */
#define MAXPUNTOS_BL		205
#define VEFNOMINALx10   	7.74597
#define CTE_DA_BLxSQRT2		0.0032541785 /*0.00322723535*/	/*junio*/

#ifdef __cplusplus
extern "C" {
#endif
void detecta_nivel2H(float *trama,int longitud,float *energia_ant,int *estado,float *nivel_max,int *cambio);
int dettono_425(int longitud,float *trama,float *etono,float nivel,float *umb_trigger);
//#ifdef __cplusplus
//}
//#endif

void energia(float *trama,int longitud,float *nivel);
void detecta_nivel(float *trama,int longitud,float *energia_ant,int *estado,float *nivel_max,int *cambio);
void detecta_nivel_medidor(float *trama,int longitud,float *energia_ant,int *estado,float *nivel_max,int *cambio,short Canal);
int detecta_sl_SCV_DS(int longitud,float *trama,float *etono,float nivel,float umb_abs,float umb_rel,float sumb_rel,float *umb_trigger);
int detecta_slN5(int longitud,float *trama,float *etono,float nivel,float umb_abs,float umb_rel,float sumb_rel,float *umb_trigger);
int detecta_registradorN5(int longitud,float *trama,float *etono,float umb_abs,float umb_rel,float sumb_rel,float *umb_trigger);
void get_nivelesN5(int digito,float cte,float *ener,int *n1,int *n2);
int decodificaN5(float *trama,int muestras,int tipo,int *nivel1,int *nivel2);

void detecta_nivelLC(float *trama,int longitud,float *energia_ant,int *estado,float *nivel_max,int *cambio);
void detecta_trLC(float *trama,int longitud,float *energia_ant,int *estado,float *nivel_max,int *cambio);
int detnivel_2280(int longitud,float *trama,float *etono,float nivel,float *umb_trigger);
void get_nivelLC(int digito,float cte,float *ener,int *n1);
int detectaLC(float *trama,int muestras,int tipo,int *nivel1,int *nivel2);
int detectaLCant(float *trama,int muestras);

int goertzelfw(int longitud,float *trama,float *etono,float umb_abs,float umb_rel,float sumb_rel,float *umb_trigger);
int goertzelbw(int longitud,float *trama,float *etono,float umb_abs,float umb_rel,float sumb_rel,float *umb_trigger);
void get_nivelesR2(int digito,float cte,float *ener,int *n1,int *n2);
int decodificaR2(float *trama,int muestras,int tipo,int *nivel1,int *nivel2);

void detecta_nivelBL(float *trama,int longitud,float *energia_ant,int *estado,float *nivel_max,int *cambio);

int grtzeldtmf(int longitud,float *trama,float *etono,float umb_abs,float umb_rel,float sumb_rel,float *umb_trigger);

void get_nivelesDTMF(int digito,float cte,float *ener,int *n1,int *n2);
int decodificaDTMF(float *trama,int muestras,short tipo,int *nivel1,int *nivel2);

int decodifica_BL(int N,float *coefW,float *coefA,float *coefB,unsigned char *indx,float *x,int *nivel1);
/*
void Obtiene_Veficaces(int *freq, float *x, int *nivel1);
*/
int Obtiene_Veficaces(float *x, int *nivel1);

void max_calibracion(float *coefW,float *coefA,float *coefB,unsigned char *indx,float *x,float *y,float *niv);

void get_nivelTMTS(int digito,float cte,float *ener, int *n1);
int detecta_Tonos_TX_TMTS(int longitud,float *trama,float nivel, float umb_abs,float *umb_trigger);
int detecta_Tonos_RX_TMTS(int longitud,float *trama,float nivel, float umb_abs,float *umb_trigger);
int decodificaTMTS(float *trama,int muestras,int tipo,int *nivel1,int *nivel2, short TipoRadio);
unsigned short VAD_ETM(int longitud, float *trama, int umbr_audio, unsigned short Canal);
int detnivel_IW(int longitud,float *trama,float *etono,int *nivel);
int detnivel_2062(int longitud,float *trama);
int det2062PTT(float *trama);
int detecta_nivelPTT(float *trama,float *energia_ant);
int detecta_1300(int longitud,float *trama, float *umb_trigger);
int detnivel_1017(int longitud,float *trama);
int goertzel(float *trama, float cteG, float gain, int longitud, float *umb_trigger);
int multifrecuencia(float *trama, float *cteG1, float *cteG2);
int detnivel_1000(int longitud,float *trama);

#ifdef __cplusplus
}
#endif
