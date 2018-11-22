/******************************************************************************

   Nombre: CDECFSK.HPP
   Descripción: Declaración de la clase CDecFsk. Detector de callerID
   
   $Revision: 1.1 $
   $Date: 2009/04/24 11:16:23 $
   
   Fecha ultima modificacion: 11/03/2009  
   
   Notas: Este bicho funciona con tamaño fijo de tramas de 48 muestras
          para una frecuencia de muestreo de 8 Kmuestras / s.
   
*******************************************************************************/

#ifndef INCLUIDO_CDECFSK_PUNTOHACHE
#define INCLUIDO_CDECFSK_PUNTOHACHE

#include "ccomevent.hpp"

//-----------------------------------------------------------
//  Definiciones públicas del módulo
//-----------------------------------------------------------

#define ESTABLECIMIENTO	0x80
#define	FECHAHORA		0x01
#define IDLLAMANTE		0x02


/*
 * Longitud fija de trama con la que funciona esto.
 */

#define CDECFSK_TAMTRAMA     	48
#define CDECFSK_TAMDECO_FSK		128
#define CDECFSK_TAMDECO_FSK2	64


//-----------------------------------------------------------
//  Declaración de la clase CDecFsk
//-----------------------------------------------------------


class CDecFsk   
{
	private:
		int iRec; // Hace falta para acceder al batiburrillo de variables globales heredadas del ETM
		class CComeventos *poComeventos; // Usado solo para depurar

		float x90_d[2];
		float xpa_d[4];
		float ypa_d[4];
		float xpb_d[18];

		short pos_tramaFSK;
		short contStart;
		short bit_actual;
		short bits_decodificados[8];
		float argmax;
		short posStart;
		float tramaFSK_ant[6];
		short bytesFSK[35];
		short num_bytesFSK;
		short decodificandoFSK;
		short anterior1300;

		float buff_auxFSK[CDECFSK_TAMDECO_FSK];
		float buff_auxFSK1[CDECFSK_TAMDECO_FSK];
		float buff_auxFSK2[CDECFSK_TAMDECO_FSK+18];
		
		short tonoprogresion; 		// si es >0 hay un tono de progresión en la línea
		float umbral425;

		float buffer[CDECFSK_TAMDECO_FSK];
		int ibuffer;

		// Variables heredadas del batiburrillo de variables desperdigadas del ETM (las metemos aqui por lo menos para tenerlas contenidas)
		// Muchas de estas sobran pero de momento las dejamos por si las moscas.

		float out_d[7];

		float inTR_d[7];
		float outTR_d[7];

		float EnergAnt;			/* Energía de la trama anterior */
		float NivelMax;			/* Nivel máximo de la trama */
		int EstNivel;				/* Estado de la rutina de cambio de nivel */
		int EstNivelAnt;
		short situacion;
		int tipotono;
		int contador;	        		/* Contador auxiliar */
		unsigned short tramas_silencio;/* # tramas de silencio */
		unsigned short long_silencio; 	/* msegundos de silencio */
		unsigned short tramas_tono;    /* # tramas de tono */
		unsigned short long_tono;     	/* msegundos de tono */
		short detectar_tono;  			/* Indica que detecte en linea */
		//int nn[NUMCANAL];						/* Contador para el array de niveles */

		//float trama_antDTMF[TAMDECO_DTMF*2];
		//int CambioDTMF;


   public:
	  unsigned char idcaller[35];	// de momento ponemos 35

	  void Inicia(int iRecurso,CComeventos *poComevent);

	  void init_decodificacionDTMF(void);
	  void init_decodificadorFSK(void);
	  void desfase_90(float *x, float *y, float *x_d, int numY);
	  void decodificadorFSK(float *x_in, int longitud);
	  void tramasFSK(float *x_in, int longitud);
	  int  get_codigosFSK(float *x_in);

      int TomaMuestras( float* );
};   


 
#endif
