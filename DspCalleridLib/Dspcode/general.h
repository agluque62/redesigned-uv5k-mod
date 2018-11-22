/******************************************************************************

   Nombre: GENERAL.H
   Descripcion: Definiciones generales.

   $Revision: 1.2 $
   $Date: 2010/03/02 11:59:12 $

   Fecha ultima modificacion: 08/10/2010     

   Notas:

******************************************************************************/

// El siguiente comentario no es interpretado como comentario por Dynamic C...

/*** BeginHeader  */

#ifndef __INCLUIDO_GENERAL__
#define __INCLUIDO_GENERAL__

//--------------------------------------------------------
//  Definicion de la plataforma origen
//--------------------------------------------------------

#define __DOS_WINDOWS__        // DOS o Windows.
//#define __HOST_LINUX__         // Linux.

//--------------------------------------------------------
//  Definicion de la plataforma destino
//--------------------------------------------------------

//#define __PLATAFORMA_8051__          // 8051/52 bajo S.O. de Page.
//#define __PLATAFORMA_PC_DOS__        // PC compatible bajo DOS.
// #define __PLATAFORMA_TMS320C6712__   // C6712.
#define __PLATAFORMA_TMS320C6747__   // C6747.
//#define __PLATAFORMA_RABBIT_DC__     // Procesador Rabbit 2000 con Dynamic C.
//#define __PLATAFORMA_LINUX_X86__     // Linux sobre PC x86.
//#define __PLATAFORMA_LINUX_PPC___   // Linux sobre PowerPC.
//#define __PLATAFORMA_LINUX_PPC8XX___   // Linux sobre PowerPC 8xx.

   
#ifdef __PLATAFORMA_TMS320C6712__   // C6712.
#define PLATAFORMA_TI_C6000
#endif
#ifdef __PLATAFORMA_TMS320C6747__   // C6747.
#define PLATAFORMA_TI_C6000
#endif


#ifdef __PLATAFORMA_LINUX_X86__
   #define __PLATAFORMA_LINUX__     
#endif
#ifdef __PLATAFORMA_LINUX_PPC8XX__
   #define __PLATAFORMA_LINUX__     
   #define __PLATAFORMA_LINUX_PPC__     
#endif

  
#ifdef __PLATAFORMA_8051__
   #ifdef __HOST_LINUX__
      //#include "../include/pragmas.h"
      #include "pragmas.h"
   #else
      #include "..\include\pragmas.h"
   #endif   
   #ifdef __GENERAR_ASSEMBLER__
      #pragma SRC
   #endif
#endif


//--------------------------------------------------------
//  Definicion del ancho de palabra
//--------------------------------------------------------

#ifdef __PLATAFORMA_8051__
   #define __INT_16__
#endif
#ifdef __PLATAFORMA_PC_DOS__
   #define __INT_16__
#endif
#ifdef PLATAFORMA_TI_C6000
   #define __INT_32__
#endif
#ifdef __PLATAFORMA_RABBIT_DC__
   #define __INT_16__
#endif
#ifdef __PLATAFORMA_LINUX_X86__
   #define __INT_32__
#endif
#ifdef __PLATAFORMA_LINUX_PPC__
   #define __INT_32__
#endif
   

//--------------------------------------------------------
//  Definicion de Endianess
//--------------------------------------------------------

#ifdef __PLATAFORMA_8051__
   #define __BIG_ENDIAN__
#endif
#ifdef __PLATAFORMA_PC_DOS__
   #define __LITTLE_ENDIAN__
#endif
#ifdef PLATAFORMA_TI_C6000
   #define __LITTLE_ENDIAN__
#endif
#ifdef __PLATAFORMA_RABBIT_DC__
   #define __LITTLE_ENDIAN__
#endif
#ifdef __PLATAFORMA_LINUX_X86__
   #define __LITTLE_ENDIAN__
#endif
#ifdef __PLATAFORMA_LINUX_PPC__
   #define __BIG_ENDIAN__
#endif


//--------------------------------------------------------
//  Definiciones de registros
//--------------------------------------------------------

#ifdef __PLATAFORMA_8051__
   //#include <reg52.h>
   #include <reg320.h>
   
   #define SCON SCON0
   #define SBUF SBUF0
#endif


//--------------------------------------------------------
//  Definiciones para compilacion multiplataforma.
//--------------------------------------------------------

/*
 * Definiciones para switches multiplataforma
 * 
 * El compilador para DSPs de Texas Instruments la caga
 * en los switches, que solo funcionan si son a unsigned int.
 */
 
#ifdef PLATAFORMA_TI_C6000
   #define ByteSwitch(PARAM)     switch( (LongWord)(PARAM) & 0x000000FF )
   #define WordSwitch(PARAM)     switch( (LongWord)(PARAM) & 0x0000FFFF )
   #define LongWordSwitch(PARAM) switch( (PARAM) )
#endif

#ifdef  __PLATAFORMA_8051__
   #define ByteSwitch(PARAM)     switch( (PARAM) )
   #define WordSwitch(PARAM)     switch( (PARAM) )
   #define LongWordSwitch(PARAM) switch( (PARAM) )
#endif

#ifdef __PLATAFORMA_LINUX__
   #define ByteSwitch(PARAM)     switch( (LongWord)(PARAM) & 0x000000FF )
   #define WordSwitch(PARAM)     switch( (LongWord)(PARAM) & 0x0000FFFF )
   #define LongWordSwitch(PARAM) switch( (PARAM) )
#endif



/*
 * ELIMINADO. QUE SEA CADA UNO DE LOS MoDULOS MULTIPLATAFORMA EL QUE DESDEFINA
 * LAS PALABRAS QUE PROCEDAN.
 * small:
 * code:
 * data:
 * idata:
 * bit:
 * xdata: Si se compila para 8051, estas constantes NO DEBEN ESTAR definidas.
 * reentrant: Si se compila para 8051, estas constantes NO DEBEN ESTAR definidas.
 *            Si se compila para otras plataformas que no reconozcan las palabras
 *            clave code, idata, xdata, etc, se pueden definir estas constantes y
 *            asi aprovechar el modulo para otras plataformas. El valor de la
 *           constante siempre debe ser vacio.
 *

#ifndef __PLATAFORMA_RABBIT_DC__
   #ifndef __PLATAFORMA_8051__
      #define small
      #define code
      #define data
      #define idata
      #define reentrant
      #define xdata
      #define bit unsigned char
   #endif
#endif */

#ifdef PLATAFORMA_TI_C6000
   #define NULL 0
#endif

//--------------------------------------------------------
//  Definiciones generales
//--------------------------------------------------------

#ifdef __PLATAFORMA_8051__

   #define XMEMBUF 9000            // Memoria para bufferes circulares
   #define XMEMPRO 1024            // Memoria para la ram protegida 
   
   #define NO_SALVA_BITS   0       // Parametros creacion de procesos
   #define SALVA_BITS      1
   
   #define ESPERA          1       // Parametros para lee y escribe
   #define NOESPERA        0

#endif


//--------------------------------------------------------
//  Macros generales
//--------------------------------------------------------

// No tiene
 
 
//--------------------------------------------------------
//  Tipos de datos generales
//--------------------------------------------------------


#ifdef __INT_16__
   typedef unsigned int Word;
   typedef unsigned long int LongWord;
#endif

#ifdef __INT_32__
   typedef unsigned short Word;
   typedef unsigned int LongWord;
#endif

// Dynamic C no tiene campos de bits.
#ifndef __PLATAFORMA_RABBIT_DC__

   #ifndef __PLATAFORMA_8051__
      #define TipoCampoBits unsigned int
   #else
      #define TipoCampoBits unsigned char
   #endif

   struct Bits
      {
      TipoCampoBits ucBit0 : 1;
      TipoCampoBits ucBit1 : 1;
      TipoCampoBits ucBit2 : 1;
      TipoCampoBits ucBit3 : 1;
      TipoCampoBits ucBit4 : 1;
      TipoCampoBits ucBit5 : 1;
      TipoCampoBits ucBit6 : 1;
      TipoCampoBits ucBit7 : 1;
      };
 
   union ByteField
      {
      unsigned char ucByte;
      struct Bits sBits;
      };
#ifndef STD_
   union Byte
      {
      unsigned char ucByte;
      struct Bits sBits;
      };
#endif
#else

   union Byte
      {
      unsigned char ucByte;
      };

#endif


#ifdef __BIG_ENDIAN__
   struct Bytes
      {
      union ByteField uHigh;
      union ByteField uLow;
      };
#else
   struct Bytes
      {
      union ByteField uLow;
      union ByteField uHigh;
      };
#endif


union WordField
   {
   Word wWord;
   struct Bytes sBytes;
   };

#ifndef __PLATAFORMA_8051__
#ifndef __PLATAFORMA_RABBIT_DC__
#ifndef __PLATAFORMA_LINUX__
#ifndef __PLATAFORMA_PC_DOS__
#ifndef __cplusplus
   union Word
      {
      Word wWord;
      struct Bytes sBytes;
      };
#endif
#endif
#endif
#endif
#endif


#ifdef __BIG_ENDIAN__
   struct Words
      {
      union WordField uHigh;
      union WordField uLow;
      };
#else
   struct Words
      {
      union WordField uLow;
      union WordField uHigh;
      };
#endif

union LongWordField
   {
   LongWord lwLongWord;
   struct Words sWords;
   };

#ifndef __PLATAFORMA_8051__
#ifndef __PLATAFORMA_RABBIT_DC__
#ifndef __PLATAFORMA_LINUX__
#ifndef __PLATAFORMA_PC_DOS__
#ifndef __cplusplus
   union LongWord
      {
      LongWord lwLongWord;
      struct Words sWords;
      };
#endif
#endif
#endif
#endif
#endif


#define WORD Word
#define DWORD LongWord


/*
 * Estructura: FechaHora.
 * Descripcion: Estructura de datos que almadena una fecha/hora.
 * Campos ->
 */

struct FechaHora
   {
   unsigned char ucDia;
   unsigned char ucMes;
   Word wAno;
   unsigned char ucHoras;
   unsigned char ucMinutos;
   unsigned char ucSegundos;
   };  
   
   
//--------------------------------------------------------------------
//  Incluye definiciones especificas de la aplicacion final.
//--------------------------------------------------------------------

#ifndef __PLATAFORMA_RABBIT_DC__

#include "appdefs.h"

#endif


#endif  // Incluido General.

// El siguiente comentario no es interpretado como comentarop por Dynamic C...
/*** EndHeader */


/***************************************************************************

    $Log: general.h,v $
    Revision 1.2  2010/03/02 11:59:12  jmgarcia

    Solo cambian comentarios


***************************************************************************/
