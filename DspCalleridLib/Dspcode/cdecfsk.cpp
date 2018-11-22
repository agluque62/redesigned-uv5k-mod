/******************************************************************************

   Nombre: CDECFSK.CPP
   Descripción: Definición de la clase CDecFSK. Detector del callerID
   
   $Revision: 1.1 $
   $Date: 2018/02/03 11:16:23 $
   
   Fecha ultima modificacion: 03/02/2018 
   
*******************************************************************************/

#define COMPILANDO_CDECFSK

#include "general.h"
#include "constMP.h"
#include "detTonos.h"
#include "iir_filt.h"
#include "cdecfsk.hpp"

#define _TI_ENHANCED_MATH_H 1
#include <math.h>



//-----------------------------------------------------------
//  Definiciones privadas de la clase
//-----------------------------------------------------------

/* Valores utilizados en la detección de los tonos en el canal */
#define NADA			0
#define INICIO			1
#define CUERPO			2
#define FINAL			3
#define TONO425			4

/*
 * Portao desde el CD-30.
 */
 
/*
#define CCDTMF_MAGN   2.2e6
#define CCDTMF_TWIST  5.01
#define CCDTMF_TOT_1  6.16
#define CCDTMF_TOT_2  20.0
#define CCDTMF_UMBRAL 1.3e5
*/

//-----------------------------------------------------------
//  Macros privadas de la clase
//-----------------------------------------------------------

// No tiene
  

//-----------------------------------------------------------
//  Tipos de datos privados de la clase
//-----------------------------------------------------------
 
// No tiene.


//-----------------------------------------------------------
//  Datos privados comunes a todos los objetos de la clase
//-----------------------------------------------------------

/*
 *  Coeficientes de Goertzel para cada una  de las 8 frecuencias que componen  los
 *  DTMFs, más la  frecuencia de 425 Hz  que no tengo ni Pidea de que  pinta aquí.
 *  Esto viene del CD-30 y por ignorancia respeto esos 425 Hz de telefonía.
 *  Ya me enteraré (o no) pa qué.
 */
 
/*
static const float afCoeficientes[9] = {

    1.88961209,  //    425   Hz
    1.70773781,  //    697   Hz
    1.64528104,  //    770   Hz
    1.56868698,  //    852   Hz
    1.47820457,  //    941   Hz
    1.16410402,  //   1209   Hz
    0.99637021,  //   1336   Hz
    0.79861839,  //   1477   Hz
    0.56853271	 //   1633   Hz
};
*/


/*
 *  Tabla de dígitos para el algoritmo de detección.
 *
 *          1209    1336    1447    1633 
 *                                      
 *  697     1       2       3       A(12)
 *                                       
 *  770     4       5       6       B(13)
 *                                       
 *  852     7       8       9       C(14)
 *                                       
 *  941     *(10)   0       #(11)   D(15)
 */

/*
static const int aiDigitosDtmf[16] = {

    1,      2,      3,      12,
    4,      5,      6,      13,
    7,      8,      9,      14,
    10,     0,     11,      15 
};
*/

//-----------------------------------------------------------
//  Servicios públicos de la clase
//-----------------------------------------------------------


/*
 * Funcion: CDecFsk::TomaMuestras.
 * Proposito: Detecta CallerID en un chorizo de muestras DE LONGITUD FIJA.
 * Parámetros ->
 *   pfTrama: Buffer con una trama de tamaño fijo igual a CCFSK_TAMTRAMA muestras.
 *
 * Retorna: 0 ->  No hay detección
 *          1 ->  ID Caller detectado.
 */
 
int CDecFsk::TomaMuestras( float *pfTrama )
{
	int i;
	int nmuestras_h;	// muestras que quedan para las 128
	int nmuestras_e;	// muestras que sobran despues de tener las 128
	short procesa=0;	// flag para procesar
	int _ret=0;
	
	nmuestras_h=CDECFSK_TAMDECO_FSK - ibuffer;
	nmuestras_e=CDECFSK_TAMTRAMA - nmuestras_h;

	// Se supone que vienen 48 muestras y necesitamos 128 para procesar
	if (nmuestras_h > CDECFSK_TAMTRAMA)
	{
		// Metemos las muestras y nos vamos
		for (i=0; i<CDECFSK_TAMTRAMA; i++)
			buffer[ibuffer++]=pfTrama[i];
	}
	else
	{
		// Metemos las que quepan y procesamos
		for (i=0; i<nmuestras_h; i++)
			buffer[ibuffer++]=pfTrama[i];

		procesa=1;
	}
	
	if (procesa)	
	{	
		ibuffer=0;
		_ret=get_codigosFSK(buffer);    
	}

	// Metemos en el buffer las muestras que queden, si es que quedan
	if (nmuestras_e > 0)
	{
		for (i=0; i<nmuestras_e; i++)
			buffer[ibuffer++]=pfTrama[nmuestras_h+i];
	}
    return _ret; 
}  



////////////////////////////////////////////////////////////////////////////////////






/**************************************************************************//**
	\brief Decodificación de adquisiciones de DTMF
	\file decoDTMF.c
	\ingroup deco
*//***************************************************************************/

/*
#include "constMP.h"
#include "espectro.h"
#include "deco.h"
#include "detTonos.h"
#include "filtros.h"
// #include "dspf_sp_fir_gen.h"
#define _TI_ENHANCED_MATH_H 1
#include <math.h>
*/

/**********************************************************************
	Variables externas declaradas en mainN5.c
**********************************************************************/
//extern short bufout[NUMCANAL][LONG_SPECT2]; /* Resultado de la decodificación de las muestras */
//extern float bufin[LONG_SPECT];	/* Muestras temporales leidas pasadas a float */
//extern int niveles[NUMCANAL][NumNivelesMax];/* Niveles de las frecuencias de los dígitos */

/**********************************************************************
	Variables externas declaradas en EmularN5.c
**********************************************************************/
//extern float EnergAnt[NUMCANAL];			/* Energía de la trama anterior */
//extern float NivelMax[NUMCANAL];			/* Nivel máximo de la trama */
//extern int EstNivel[NUMCANAL];				/* Estado de la rutina de cambio de nivel */
//extern int EstNivelAnt[NUMCANAL];

/**********************************************************************
	Variables externas declaradas en decoN5.c
**********************************************************************/
//extern short situacion[NUMCANAL];
//extern int tipotono[NUMCANAL];
//extern int contador[NUMCANAL];	        		/* Contador auxiliar */
//extern unsigned short tramas_silencio[NUMCANAL];/* # tramas de silencio */
//extern unsigned short long_silencio[NUMCANAL]; 	/* msegundos de silencio */
//extern unsigned short tramas_tono[NUMCANAL];    /* # tramas de tono */
//extern unsigned short long_tono[NUMCANAL];     	/* msegundos de tono */
//extern short detectar_tono[NUMCANAL];  			/* Indica que detecte en linea */
//extern int nn[NUMCANAL];						/* Contador para el array de niveles */
/*extern short lim_deco[NUMCANAL];*/                /* Número máximo de niveles */
/*======================================================================*/
//float trama_antDTMF[TAMDECO_DTMF*2];
//int CambioDTMF[NUMCANAL];

extern float a_DTMF[8];
extern float b_DTMF[8]; 
//extern float out_d[7];
extern float bpa1020[5];
extern float apa1020[5];

float bpb880[19]={-0.00422326F,-0.0161835F,-0.0320799F,	//coeficientes en orden inverso: de b18 a b0
				  -0.0443273F,-0.0358977F,0.00518616F,0.0797520F,0.169184F,0.242905F,0.271370F,0.242905F,
				   0.169184F,0.0797520F,0.00518616F,-0.0358977F,-0.0443273F,-0.0320799F,-0.0161835F,-0.00422326F};

/*======================================================================*/
void CDecFsk::Inicia(int iRecurso,CComeventos *poComevent)
{
	iRec=iRecurso;
	poComeventos=poComevent;
}

/*========== Inicializa variables para la decodificación ===============*/
void CDecFsk::init_decodificacionDTMF(void)
 {
	short i;
	
	//for(i=0;i<NumNivelesMax;i++)
	//	niveles[Canal][i]=0;

	for (i=0;i<7/*2*/;i++)
	{
		inTR_d[i]=0;
		outTR_d[i]=0;
	}
	
	EnergAnt = 0.0;
	EstNivel = 0;
	situacion=0;
	tipotono = 0;
	contador=0;
	tramas_silencio = 0;
	long_silencio = 0;
	tramas_tono = 0;
	long_tono = 0;
	detectar_tono = 0;
	//nn = 0;
	/*lim_deco[Canal] = MAXDIGDTMFx2;*/
	tonoprogresion=-2;
	umbral425=0.0385F;
	init_decodificadorFSK();
 }

/************************************************************************

	Rutina de inicialización del decodificador FSK

************************************************************************/
void CDecFsk::init_decodificadorFSK()
{
	int i;

	for(i=0;i<CDECFSK_TAMDECO_FSK;i++)
	{
		buff_auxFSK[i]=0;
		buff_auxFSK1[i]=0;
	}
	
	for(i=0;i<(CDECFSK_TAMDECO_FSK+18);i++)
		buff_auxFSK2[i]=0;

	for(i=0;i<4;i++)
	{
		xpa_d[i]=0;
		ypa_d[i]=0;
	}
	
	for(i=0;i<18;i++)
		xpb_d[i]=0;
	
	for(i=0;i<6;i++)
		tramaFSK_ant[i]=0;

	x90_d[0]=0;
	x90_d[1]=0;
	pos_tramaFSK=20;
	bit_actual=-1;
	contStart=0;
	argmax=0;
	posStart=0;

	for(i=0;i<35;i++)
		bytesFSK[i]=0;

	num_bytesFSK=0;
	decodificandoFSK=0;
	anterior1300=0;

	ibuffer=0;
}
/************************************************************************
	
	Desfasador 90º

	float *x: Puntero a las muestras de entrada
	float *y: Puntero a las muestras de salida
	float *x_d: Puntero a las muestras de entrada retrasadas (muestra anterior)
	int numY: Número de muestras de entrada
	
*************************************************************************/
void CDecFsk::desfase_90(float *x, float *y, float *x_d, int numY)
{
	int i;
	
	y[0]=x_d[1]+0.3081F*x_d[0];
	y[1]=x[0]+0.3081F*x_d[1];
	
	for (i=2;i<numY;i++)
		y[i]=x[i-1]+0.3081F*x[i-2];
	
	x_d[0]=x[numY-2];
	x_d[1]=x[numY-1];
}

/************************************************************************
	
	void decodificadorFSK(float *x_in, int longitud, int Canal)
	
	Lee señal analógica (positiva para los "1" y negativa para los "0") y
	determina los bits, que son juntados y almacenados en bytesFSK[ ]
*************************************************************************/
void CDecFsk::decodificadorFSK(float *x_in, int longitud)
{
	short i;
	short bytedecodificado=0;
	short num_resultadosFSK=num_bytesFSK;
	
	while(pos_tramaFSK<longitud)
	{
		switch(bit_actual)
		{
			case -1:					// Buscando el final de la señal de marca
				if(x_in[pos_tramaFSK]<0)
					bit_actual=0;
				pos_tramaFSK++;
			break;
			case 0:						//START
				//La posición de START se decide como arg max(r[n-7]-r[n])
				if((pos_tramaFSK<7)&&((tramaFSK_ant[pos_tramaFSK]-x_in[pos_tramaFSK])>argmax))
				{
					argmax=tramaFSK_ant[pos_tramaFSK]-x_in[pos_tramaFSK];
					posStart=contStart;
				}
				else if((x_in[pos_tramaFSK-7]-x_in[pos_tramaFSK])>argmax)
				{
					argmax=x_in[pos_tramaFSK-7]-x_in[pos_tramaFSK];
					posStart=contStart;
				}
				pos_tramaFSK++;
				contStart++;
				if(contStart==6){		//Buscamos en las 6 muestras siguientes al cambio de signo
					contStart=0;
					argmax=0;
					pos_tramaFSK += posStart +1; //siguiente bit a +7 desde el START (ya se ha sumado 6 buscando el arg max)
					posStart=0;
					bit_actual++;
				}
			break;
			case 9:						//STOP
				if(x_in[pos_tramaFSK]<0)
				{
					for(i=0;i<8;i++)		//Juntamos los bits en una unica variable
						bytedecodificado = bytedecodificado | (bits_decodificados[i]<<i);
					
					bytesFSK[num_bytesFSK] = bytedecodificado;	
					num_bytesFSK++;
						
					bit_actual=0;
					bytedecodificado = 0;
				}
				pos_tramaFSK++;
			break;
			default:			//Bits 1-8
				if (x_in[pos_tramaFSK]<0)
					bits_decodificados[bit_actual-1] = 0;
				else
					bits_decodificados[bit_actual-1] = 1;
				pos_tramaFSK += (short)(roundf(6.667F*(bit_actual + 1)) - roundf(6.667F*bit_actual));	//Desplazamiento al siguiente bit
				bit_actual++;
			break;
		}
	}
	pos_tramaFSK -= longitud; //Preparamos la trama siguiente
	for(i=0;i<6;i++)
		tramaFSK_ant[i]=x_in[longitud-7+i];
		
	if (num_bytesFSK > num_resultadosFSK) //Hemos decodificado nuevos bytes
	{
		if (bytesFSK[0]!=ESTABLECIMIENTO) //Si no tenemos el byte de establecimiento, reiniciamos
		{
			init_decodificadorFSK();
		}
		
	}
}

/***********************************************************************
	
	void tramasFSK(float *x_in, int longitud, int Canal)
	
	Decodifica las tramas en FSK con la información de identificación del llamante
	en telefonía
************************************************************************/
void CDecFsk::tramasFSK(float *x_in, int longitud)
{
	short i;
	short numsubtramasFSK=1;
	int tamsubtramaFSK=longitud;
	float niv=0;
	int det1300;
	
	iir(&x_in[0],&buff_auxFSK[0],&bpa1020[0],&apa1020[0],&xpa_d[0],&ypa_d[0],4,longitud);
	
	//Convertimos señal modulada en FSK en señal positiva para los "1" y negativa para los "0"
	desfase_90(&buff_auxFSK[0],&buff_auxFSK2[18],&x90_d[0],longitud);
	
	for(i=0;i<longitud;i++)
	{
		buff_auxFSK2[18+i] *= buff_auxFSK[i];
		if (i<18)
			buff_auxFSK2[i]=xpb_d[i];		//Condiciones iniciales del filtro paso bajo
	}
	
	// DSPF_sp_fir_gen(&buff_auxFSK2[0],&bpb880[0],&buff_auxFSK1[0],19,longitud);
	fir(&buff_auxFSK2[0],&bpb880[0],&buff_auxFSK1[0],19,longitud);
	
	for(i=0;i<18;i++)					//Actualizamos condiciones iniciales
		xpb_d[i]=buff_auxFSK2[longitud+i];
	
	if (longitud==CDECFSK_TAMDECO_FSK)
	{
		numsubtramasFSK=2;				//Usamos 2 subtramas de 64 muestras para detectar mejor la marca
		tamsubtramaFSK=CDECFSK_TAMDECO_FSK2;
	}

	for(i=0;i<numsubtramasFSK;i++)			//Cuando detectamos dos subtramas con la señal de marca decodificamos
	{
		det1300=detecta_1300(tamsubtramaFSK,&buff_auxFSK[tamsubtramaFSK*i],&niv);
		
		if((!decodificandoFSK) && det1300)
			if(anterior1300)
				decodificandoFSK=1;
			else
				anterior1300 = 1;
		else
			anterior1300 = 0;
		
		if (decodificandoFSK)
			decodificadorFSK(&buff_auxFSK1[tamsubtramaFSK*i],tamsubtramaFSK);
	}
}

// Decodifica codigos FSK para el ID caller (para tramas de 128 muestras)
int CDecFsk::get_codigosFSK(float *x_in)
 {
	int Cambio = 0;
	//int Result;
	//int nivel1, nivel2;
	int i/*,j*/;
	float ener[3];
	short tono425;
	float bufaux[CDECFSK_TAMDECO_FSK];
	int _ret=0;

	tramasFSK(x_in,CDECFSK_TAMDECO_FSK);
		
	for(i=0;i<2;i++)						//Tonos de señalización 425Hz
	{
		energia(&x_in[i*TAMDECO_DTMF2],TAMDECO_DTMF2,&ener[0]);
		tono425=(dettono_425(TAMDECO_DTMF2,&x_in[i*TAMDECO_DTMF2],&ener[0],umbral425,&ener[2]))
				&&(ener[1]>34.766e6);			//Umbral absoluto en -40dBm
		if(tono425)
			tonoprogresion++;
		else
			tonoprogresion--;
		
		if (tonoprogresion<-2) tonoprogresion=-2;	//Con dos tramas consecutivas pasamos por cero
		if (tonoprogresion>2) tonoprogresion=2;
		
		if(!tonoprogresion)			
		{
			if(!tono425)			//Final del tono de 425Hz
			{
				tonoprogresion=-2;
				long_tono+=tramas_tono*8;		//Escribimos 
				//bufout[Canal][contador[Canal]++]=tipotono[Canal];
				//bufout[Canal][contador[Canal]++]=long_tono[Canal];
				umbral425=0.0385F; 					//cambiamos el umbral a 1/26
				tramas_silencio=0;
				EnergAnt = 0.0;
				EstNivel = 0;
				tipotono = 0;
				tramas_tono=0;
				detectar_tono=0;
				long_silencio= (1-i)*8;	//si la segunda subtrama sin detectar tono es i=0 rellenamos con 8ms de silencio
			}
			else					//Inicio del tono de 425Hz
			{
				if((long_silencio)||(long_tono)||(tramas_silencio)||(tramas_tono))	//Si empezamos directamente los 6s con tono de 425 aqui no entramos
				{
					long_silencio-= 8*(1-i)+2;	//suponemos el cambio 2ms antes de la primera detección
					long_tono-= 8*(1-i)+2;
					//resto_decodificaDTMF(Canal); 		//Escribimos lo que tuviésemos
					//JJA contador--;					//Sobreescribir el "fin de códigos"
					long_tono=2;
				}
				tonoprogresion=2;
				umbral425=0.1F; 					//cambiamos el umbral a 1/10
				//if (nn[Canal]<NumNivelesMax)
				//	niveles[Canal][nn[Canal]++]=_spint(10*log10(ener[1])-CTE_DTMF64);
				tipotono = 18;
				tramas_tono=0;
				situacion=TONO425;
			}
		}
		
		if (tonoprogresion>0)
			tramas_tono++;
	}

	iir(x_in,bufaux,b_DTMF,a_DTMF,inTR_d,outTR_d,7,TAMDECO_DTMF);	                
	
	if(tonoprogresion>0)
		return -1;							//Estamos dentro de un tono de 425Hz, no miramos nada más
	
	EstNivelAnt = EstNivel;
	
	/* ver el nivel de la trama que le pasamos, para ver si hay tono */
	detecta_nivel2H(bufaux,TAMDECO_DTMF,&EnergAnt,&EstNivel,&NivelMax,&Cambio);

	if (EstNivel!=EstNivelAnt)	/* segun estado hacer una u otra funcion */
	 {
		if((EstNivel==0) && (EstNivelAnt==2)) situacion = NADA;   /* silencio */
		if((EstNivel==0) && (EstNivelAnt==1)) situacion = FINAL;  /* fin tono */
		if (EstNivel==1)	 situacion = INICIO; 	/* inicio tono */
		if((EstNivel==2) && (EstNivelAnt==1)) situacion = FINAL;  /* fin tono */
		if((EstNivel==2) && (EstNivelAnt==0)) situacion = NADA;   /* silencio */
	 }
	else
	 {
		if((EstNivel==0)||(EstNivel==2)) situacion = NADA;  /* silencio */
		if(EstNivel==1)	situacion = CUERPO;        /* tono */
	 }
	
	switch (situacion)
	 {
		case NADA:					/* no se detecta nada en la linea */
			tramas_silencio++;
			break;
		case INICIO:				/* se detecta un cambio de nivel(subida) */
			Cambio=(Cambio+0x0004)>>3;		/* pasar a entero redondeando */
			long_silencio=long_silencio + 16*tramas_silencio + Cambio;
			long_tono=16 - Cambio;
			/* escribir en buffer el silencio y su duración hasta inicio de tono */
			//bufout[Canal][contador[Canal]++]=(short)tipotono[Canal];
			//bufout[Canal][contador[Canal]++]=long_silencio[Canal];
			/* se inicializan las variables utilizadas en la detección del silencio */
			tramas_silencio=0;
			detectar_tono=1;
			break;
		case CUERPO:				/* estamos dentro de un tono */
			tramas_tono++;
			/*
			if (detectar_tono[Canal]==1)
			  detectar_tono[Canal]=2;
            else if (detectar_tono[Canal]==2)
              detectar_tono[Canal]=3;
			*/
			if (detectar_tono != 0)
			  detectar_tono++;
			break;
		case FINAL:					/* fin de tono */
			Cambio=(Cambio-AJUSTE_DTMF+0x0004)>>3;		/* pasar a entero redondeando */
			long_tono=long_tono + 16*tramas_tono + Cambio;
			long_silencio=16 - Cambio;
			/* escribir en buffer el tipo de tono y su duración */
			if (tipotono==0) tipotono=17;		//Tono desconocido
			if ((bytesFSK[2]==IDLLAMANTE)||(bytesFSK[12]==IDLLAMANTE))
				tipotono=19;							//Información de id.llamante en FSK
			
			//bufout[Canal][contador[Canal]++]=(short)tipotono[Canal];
			//bufout[Canal][contador[Canal]++]=long_tono[Canal];
			
			if(tipotono==19)						//Pasamos los dígitos id.llamante
			{
				if (bytesFSK[2]==IDLLAMANTE)
					for(i=0;i<bytesFSK[3];i++)
						if(bytesFSK[4+i]&0x30) //Solo si es un  dígito
						{	
							idcaller[contador++]=(unsigned char)(bytesFSK[4+i]&0x0F);
							_ret++;
						}
				if (bytesFSK[12]==IDLLAMANTE)	
					for(i=0;i<bytesFSK[13];i++)
						if(bytesFSK[14+i]&0x30) //Solo si es un  dígito
						{
							idcaller[contador++]=(unsigned char)(bytesFSK[14+i]&0x0F);
							_ret++;
						}
				init_decodificadorFSK();				//Reiniciamos decodificador
			}
			/* se inicializan las variables utilizadas en la detección del tono*/
			tipotono=0;
			tramas_tono=0;
			detectar_tono=0;
			break;
	 }	/*  end del switch*/
	 
	/*
	if (detectar_tono&&(situacion==CUERPO))
	{
	  Result=decodificaDTMF(bufaux,TAMDECO_DTMF,3,&nivel1,&nivel2);
		
	  switch (detectar_tono)
	  {
		case 0: case 1: case 2:
			break;
		case 3:
			if (Result)		
			 {
				tipotono=Result;
				detectar_tono=0;
				
				if ((tipotono)&&(nn[Canal]<NumNivelesMax))
				 {
					niveles[Canal][nn[Canal]++]=nivel1;
					niveles[Canal][nn[Canal]++]=nivel2;
				 }
			 }
			 else
			 {
			 	for (i=0;i<TAMDECO_DTMF;i++) trama_antDTMF[i]=bufaux[i];
			 }
			 break;	
		
		default:
		  if (Result)
		  {
			for (i=0,j=TAMDECO_DTMF; i<TAMDECO_DTMF; i++,j++) trama_antDTMF[j]=bufin[i];
			
			CambioDTMF[Canal]=detectaDTMFant(trama_antDTMF,TAMDECO_DTMF);

			// Primero hay que escribir en el buffer la duracion de lo que haya antes
			long_tono[Canal]=long_tono[Canal] + 16*(tramas_tono[Canal]-2) + CambioDTMF[Canal];

			// escribir en buffer el tipo de tono y su duración 
			if (tipotono[Canal]==0) tipotono[Canal]=17;
			bufout[Canal][contador[Canal]++]=tipotono[Canal];
			bufout[Canal][contador[Canal]++]=long_tono[Canal];

			// Ahora se inicializan las variables utilizadas en la detección
			 //  de la multifrecuencia y que sirven para contabilizar su duración
			tipotono[Canal]=Result;
			tramas_tono[Canal]=1;
            long_tono[Canal]=16-CambioDTMF[Canal];
			detectar_tono[Canal]=0;
			if ((tipotono[Canal])&&(nn[Canal]<NumNivelesMax))
		    {
				niveles[Canal][nn[Canal]++]=nivel1;
				niveles[Canal][nn[Canal]++]=nivel2;
			}

	      }
          else
	      {
	       for (i=0;i<TAMDECO_DTMF;i++) trama_antDTMF[i]=bufin[i];
	      }
	      break;

		}	// end switch (detectar_tono[Canal])
	}
*/

 return _ret;
 }	/* end get_codigosDTMF */
/*======================================================================*/

/*========= Final de la rutina de gestion de la decodificación  ========*/

/*
void resto_decodificaDTMF(int Canal)
 {	// Escribimos lo que se nos ha quedado en el tintero
	switch(situacion[Canal])
	 {
		case NADA:
			long_silencio[Canal] = long_silencio[Canal] + 16*tramas_silencio[Canal];
			bufout[Canal][contador[Canal]++]=(short)tipotono[Canal];
			bufout[Canal][contador[Canal]++]=long_silencio[Canal];
			break;
		case INICIO:
			if (tipotono[Canal]==0) tipotono[Canal]=17;
			bufout[Canal][contador[Canal]++]=(short)tipotono[Canal];
			bufout[Canal][contador[Canal]++]=long_tono[Canal];
			break;
		case CUERPO:
			long_tono[Canal] = long_tono[Canal] + 16*tramas_tono[Canal];
			if (tipotono[Canal]==0) tipotono[Canal]=17;
			bufout[Canal][contador[Canal]++]=(short)tipotono[Canal];
			bufout[Canal][contador[Canal]++]=long_tono[Canal];
			break;
		case FINAL:
			bufout[Canal][contador[Canal]++]=(short)tipotono[Canal];
			bufout[Canal][contador[Canal]++]=long_silencio[Canal];
			break;
		case TONO425:
			long_tono[Canal] = long_tono[Canal] + 8*tramas_tono[Canal];
			bufout[Canal][contador[Canal]++]=(short)tipotono[Canal];
			bufout[Canal][contador[Canal]++]=long_tono[Canal];
			break;
	 }//fin del switch situación

	bufout[Canal][contador[Canal]++]=0xFFFF;	// clave que determina el fin de los códigos

 }	// fin de resto_decodifica
 */

/*============== fin del resto de la decodificación ====================*/

/*
int detectaDTMFant(float *trama,int muestras)
{
  // Todas las variables están inicializadas para un tamaño de trama de 128 muestras
  float ener[10];
  float ttrlevl = 38e6;   // -40dBm
  float nrgcoefp = 12.00;
  float nrgcoefs = 34.00;

  float umbtrg;
  int result;
  int contDTMF=0;
  int inicioR;


  do
  {
    inicioR=8*(contDTMF+1);
    
    energia(trama+inicioR,muestras,&ener[8]);
    result = grtzeldtmf(muestras,trama+inicioR,ener,ttrlevl,nrgcoefp,nrgcoefs,&umbtrg);

    if (result) return(contDTMF+1);
    contDTMF=contDTMF+1;
  }while(contDTMF<15); 
  
  return(contDTMF+1); 

}
*/