/******************************************************************************

   Nombre: APPDEFS.H
   Descripcion: Definiciones específicas de la aplicación final.

   $Revision: 1.5 $
   $Date: 2010/03/02 11:32:30 $

   Fecha ultima modificacion: 06/10/2010           
      
******************************************************************************/


//--------------------------------------------------------------
//  Definiciónes de la versión hardware
// 
// Sólo una de las siguientes constantes debe estar definida.
//--------------------------------------------------------------

#define PRUEBAS_OFFLINE_ATSR2

#define __VERSION_HARDWARE_521ED0__

#define CHIP_6747   // Para la CSL del Code Composer

/*
#ifndef __VERSION_HARDWARE_XXXX__
   #ifndef __VERSION_HARDWARE_XXXX__
      #error VERSION HARDWARE NO DEFINIDA 
   #endif
#endif

#ifdef __VERSION_HARDWARE_XXXX__
   #ifdef __VERSION_HARDWARE_XXXX__
      #error DEFINIDA MAS DE UNA VERSION HARDWARE
   #endif
#endif
*/


//-------------------------------------------------------------------------
//  Definiciones generales del DSP de las pasareillas CD40
//-------------------------------------------------------------------------

/**
 * \brief Maximo de recursos en el sistema
 */
 #define MAX_RECURSOS  16
/*#ifdef CD40_COMPILANDO_DSP_CGW

#define MAX_RECURSOS  32

#else  // DSP de la DSW

#define MAX_RECURSOS  12

#endif*/


/**
 * \brief Primer ID de canal de audio asignado al HPI
 */
 
#define PRIMER_CANAL_HPI   64



/**
 * \name  Codigos estaticos de RTP payload para audio.
 *        Ver Pag 33 del RFC 3551 de Julio de 2003.
 * @{
 */
 
#define  RTP_PT_PCMU           0  // G.711 u law
#define  RTP_PT_GSM            3
#define  RTP_PT_G723           4
#define  RTP_PT_DVI4_8K        5
#define  RTP_PT_DVI4_16K       6
#define  RTP_PT_LPC            7
#define  RTP_PT_PCMA           8  // G.711 A law
#define  RTP_PT_G722           9
#define  RTP_PT_L16_1C        10
#define  RTP_PT_L16_2C        11
#define  RTP_PT_QCELP         12  
#define  RTP_PT_CN            13
#define  RTP_PT_MPA           14
#define  RTP_PT_G728          15
#define  RTP_PT_DVI4_11P25K   16
#define  RTP_PT_DVI4_22P5K    17
#define  RTP_PT_G729          18 

//@}


//-------------------------------------------------------------------------
//  Definiciones para el codigo heredao del CD-30 y el ETM
//-------------------------------------------------------------------------

#define NUMCANAL  MAX_RECURSOS  


/* Tamano en muestras de las tramas de muestras del DSP                     */
#define TAMTRAMA                        48      /* Tamano de cada TS        */

/* Tamano de una subtrama. Se utiliza cuando se quiere una resolucion menor */
/* de una trama, por ejemplo, a la hora de generar senales                  */
#define TAMSUBTRAMA                     8

/* Duracion en mseg de las tramas de muetras con la que trabaja el DSP      */
/* No es lo mismo que una trama o TS de la senal PCM                        */
#define MSEGTRAMA                       6



//-------------------------------------------------------------------------
//  Definiciones para el codigo heredao del CD-30
//-------------------------------------------------------------------------

#ifdef CD40_CODIGO_CD30

#include "cd30_rec.h"


/* Retardo que introducen las rutinas de comunicaciones en cualquier        */
/* protocolo de comunicaciones del tipo TX=RX+IntervaloTemp                 */
#define RETTRAMA                        12

#define RETBAJADA   2   /* Retardo que introduce detecta_nivel, para        */
                        /* detectar una caida en el nivel de senal          */
/**/
#define RETMARE     0 // 5 // 11

#endif

//-------------------------------------------------------------------------
//  Definiciones para el codigo heredao del ETM
//-------------------------------------------------------------------------

#ifdef CD40_CODIGO_ETM

#define NUMCTO  MAX_RECURSOS

typedef unsigned char byte;

#endif

//--------------------------------------------------------
//  Definiciones de incidencias
//--------------------------------------------------------

/*
 * Constantes __INCIDENCIAS_NIVEL_xx__: Si esta constante esta definida, se activa la
 *                                      compilacon del codigo de generacion de incidencias de nivel xx.
 */
 
#ifndef __NOAPP__  // El NoApp no lleva incidencias.
#ifndef APLICACION_BOOT

#ifndef PRUEBAS_OFFLINE_ATSR2
	//#define USAR_DSPBIOS_LOG
	#define __INCIDENCIAS_NIVEL_0__  // Incidencias de errores SW.
	#define __INCIDENCIAS_NIVEL_1__  // Incidencias de funcionamiento (rebosamiento de colas, etc...).
	#define __INCIDENCIAS_NIVEL_2__  // Incidencias de depuracion.
	#define __INCIDENCIAS_NIVEL_3__  // Incidencias TEMPORALES de depuracion para aislar a las demas.
	#include "inci.h"
#endif
#endif
#endif

//--------------------------------------------------------
//  Desactiva algunas palabras clave de otras plataformas.
//--------------------------------------------------------

#define idata
#define xdata
#define small
#define code


//--------------------------------------------------------
//  Definiciones generales
//--------------------------------------------------------

/*
 * Codecs soportados
 */
 
 


//--------------------------------------------------------
//  Macros generales
//--------------------------------------------------------

// No tiene
 
 
//--------------------------------------------------------
//  Tipos de datos generales
//--------------------------------------------------------

// No tiene



