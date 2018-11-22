/*#define SAVEDATA*/ 
#include "def_fs.h"

/********************************************************************
	Constantes generales utilizadas en las distintas funciones
	del equipo
********************************************************************/

#define VERDADERO       	1
#define FALSO			0 

/* Asignación de protocolos */
#define  R2             	0
#define  N5             	1
#define  LC             	2
#define  DTMF           	3
#define  BL             	4
#define	 TM			5 	
#define	 TS			6 	

#define  TR2            	8
#define  TN5            	9

#define  MRL            	10
#define  GRL            	11

/* Número de recursos en el sistema */
// jmgarcia 20090604
#if 0
// FIN jmgarcia
#define NUMCANAL		4
#define NUMCTO         		2
// jmgarcia 20090604
#endif
// FIN jmgarcia
#define NUMCODECS       	6

/* Constantes utilizadas en las funciones de entrada/salida del sistema */
#define BLOCK_SIZEx4    	1152
#define BLOCK_SIZEx2    	576

#define DATA_SIZE       	2
#define TAMTRAMA		48
#define BLOCK_SIZE      	288
#define NUM_OF_BLOCKS   	2

/* Constantes utilizadas en las funciones de manejo de tramas de entrada/salida */
#define TAMTRAMAx2		96
#define TAMTRAMAx3		144
#define TAMTRAMAx4		192
#define NBYTESPP        	1152
#define NBYTESP         	576

/* Retrasos asociados a los codecs y a la entrada/salida de muestras */
#ifdef CONFIR_FS16K
	#define DELAY_INOUTx4   472	/* 4B*(2*48+3+1+18) */
#endif
#ifdef SINFIR_FS8K
	#define DELAY_INOUTx4   396	/* 4B*(2*48+3) */
#endif

/* Tamaños de trama utilizados en la detección de los tonos por protocolo */
#define TAMDECO_R2		128
#define TAMDECOPT_R2    	66
#define TAMDECO_N5		80
#define TAMDECO_LC		48
#define TAMDECO_BL		200
#define TAMDECO_DTMF    	128
#define TAMDECO_DTMF2		64
#define TAMDECOTR_DTMF  	66

#define TAMDECO_TM		192 /* Alcazar */

/* Número de muestras que hay que restar a la longitud de los tonos
   debido a las pendientes de bajada (por protocolo)                */
#define AJUSTE_N5		35

 #ifdef CONFIR_FS16K
#define AJUSTE_trLC		13
  #endif
#define AJUSTE_LC		16
#define AJUSTE_R2		35
#define AJUSTE_DTMF		35
#define AJUSTE_I_BL		8		/* entre 5 y 16 muestras */
#define AJUSTE_F_BL		55		/* entre 30 y 80 muestras */

#define AJUSTE_TM		35		/* Alcazar */

/*
#define AJUSTE_BL		35
*/
/* Constantes asociadas a las variables de configuración como terminal */
#define NumDigMax		17

/* Algunas longitudes asociadas a los arrays de comunicación PC/DSP */
#define NumTiemposMax   	20
#define NumNivelesMax   	42	/* En N5: (T+KP+17*D+ST+L)*2 */
#define NumTouts		7

/* Constantes utilizadas en las funciones de emulación de los protocolos
   ya sea en la generación o en la detección de señales                 */

#define TAMSUBTRAMA		8		/* 8 muestras equivalen a 1 ms			*/
#define MSEGTRAMA		6
#define MSEG2TRAMAS		12

// jmgarcia 20090710
// Aumento en diez milis este tiempo sin saber mu bien como va esto.
//#define RETTRAMA		12		/* Retardo de las rutinas de in/out
/*                                   				Se redondea a 12ms aunque son 12.375 */
#define RETTRAMA		22		/* Retardo de las rutinas de in/out
                                   				Se redondea a 12ms aunque son 12.375 */
// FIN jmgarcia

/* Tamaño de la Tabla utilizada en todas las funcíones de emulación para
   configurar los tiempos operativos del sistema						*/
#define TAMTABLA		77 // JJA 75
#define ENTRADAS		2


#define UMBRAL_AUDIO_ANALISIS 	1305 //(-23 dBm)


/************************************************	
PROTOCOLO DE EVALUACIOO DE LINEAS ANALOGICAS	*
************************************************/
#define  O22			7
#define NumDigMax_O22		5

#define NUM_MAX_MEDIDAS		64
#define BASICA			0
#define EXHAUSTIVA		1
#define NUM_FREC_BASICO		3
#define	NUM_CODIGOS		18	//los 16 de la norma O22 de Evaluacion + 2 de la norma 011 de ATS Nº5
