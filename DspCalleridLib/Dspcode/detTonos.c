
// jmgarcia 20090623
#define CD40_CODIGO_ETM
#include "general.h"
// FIN jmgarcia

#include <math.h>
#include "constMP.h"
#include "espectro.h"
#include "detTonos.h"
/*
extern int Circuito;
extern int SCV_DS_R2[NUMCTO];

// Variables definidas en espectro.c		
extern float spectAdq_aux[2*MAXTAMFFT];
extern float spectReal_aux[2*TAMFFT];

// Variable global utilizada en VAD_ETM
unsigned short Niv[NUMCANAL];	
unsigned short hangover[NUMCANAL];
*/
/******************************************************************************
*                                                                             *
*				CONSTANTES DE DETECCIÓN                       *
*	                                                                      *
******************************************************************************/

float cteMFR5[6] = {1.7052803287082F, 1.5208119312001F, 1.2988960966604F,
                   1.0449971294319F, 0.7653668647302F, 0.4668907277118F};
                   
float cteMFL5[2] = {-0.6180339887499F, -0.9079809994791F};

//float cteMF_SCVDS[2] = {1.9021130325903, 1.7820130483767};

short pesosN5[6] = {0, 1, 2, 4, 7, 12};

float cte425 = 1.8896120929338F;

/*
float cteLC = -0.4362864827931;                   

float cteFWR2[5] = {0.9358596285211, 0.7653668647302, 0.5880806504646,
                    0.4055745907130, 0.2194686221821};

float cteBWR2[5] = {1.250485312671, 1.391825593185, 1.5208119312,
                    1.636299434850, 1.737263028876};

short pesosFWR2[5] = {0, 1, 2, 4, 7};

short pesosBWR2[5] = {10, 11, 12, 14, 17};

float cteLR2 = -0.4362864827931;                   

float cteIDTMF[4] = {1.7077378093489, 1.6452810360417, 1.5686869838668,
                     1.4782045682402};

float cteSDTMF[4] = {1.1641040230273, 0.9963702106790, 0.7986183892266,
                     0.5685327067752};

int identifDTMF[4][4] = {1, 2, 3, 13, 4, 5, 6, 14, 7, 8, 9, 15, 11, 10, 12, 16};

float cte2062 = -0.0973508889912;
float cte1017 = 1.3952058170413;

float nivelR_LC[5] = {0.0517,0.0638,0.0843,0.1256,0.2503};
float nivelA_LC[5] = {13.58,8.69,4.889,2.173,0.543};


float cteTX_TMTS[3] = {-1.3378720571399606422, -1.386174725091271739553, -1.43296919998097126336};
float cteRX_TMTS[3] = {-1.5238681586868769720, -1.565759411192603173351, -1.60594707796893424555};

float UmbralMedidor[NUMCTO];
float	cte_IW = 1.41421356237316336891534932238871;//2*cos(CTE_TONO*1000);
*/
/*
float coef_p[4] = {-0.35643685337497, 2.48671582825015, -5.85630752976033, 4.67819878406612};
*/
/*
float coef_p[5] = {0.18513617903508, -1.65673084567682, 5.78405226721129, -9.42412481200803, 6.06539524878868};
*/
/*
float coef_p[2] = {-0.4375, 1.1375};
*/

float cte1300 = 1.0449971294319F;

// jmgarcia 20090623

float	cte_IW = 1.41421356237316336891534932238871F;//2*cos(CTE_TONO*1000);

#define abs(X) ((0>(X))?(-X):(X))

// FIN jmgarcia


/******************************************************************************

   energia
                                                                 
   FUNCION:
   
   Calcula la energía de las muestras contenidas en la trama.										         
                                                                 
   ENTRADAS:                                                     

   float *trama: puntero a la secuencia temporal		   
   short longitud: número de muestras por trama           
                                                                
   SALIDAS:                                                  
   
   float *nivel: valor de energía de la trama                       
                                                            
   LLAMADA DESDE C:						   					

   void energia(float *trama,int longitud,float *nivel)
                                                                 
******************************************************************************/

void energia(float *trama,int longitud,float *nivel)
{
	short i;

    *nivel = trama[0]*trama[0] + trama[1]*trama[1];

    for (i=2;i<longitud;i++) *nivel = *nivel + trama[i]*trama[i]; 			
} 

/******************************************************************************

   detecta_nivel
                                                                 
   FUNCION:
   
   Calcula el estado en el que se encuentra la trama de señal, y si
   se produce un cambio de nivel indica en que muestra ha sido. Válido
   para los protocolos N5, R2, y BL.										         
                                                                 
   ENTRADAS:                                                     

   float *trama: puntero a la secuencia temporal		   
   int longitud: número de muestras por trama           
   float *energia_ant: energia de la ultima muestra		   
   int *estado: estado de la trama anterior  		 
   float *nivel_max: nivel maximo del ESTADO TONO		 
                                                             
   SALIDAS:                                                  
   
   int *cambio: número de muestra en la que se produce el cambio de estado.                       
                                                            
   LLAMADA DESDE C:						   					

   void detecta_nivel(float *trama,int longitud,float *energia_ant,
                      int *estado,float *nivel_max,int *cambio)
                                                                 
******************************************************************************/

void detecta_nivel(float *trama,int longitud,float *energia_ant,int *estado,
                   float *nivel_max,int *cambio)
 {
	register int cont1;
	register float energ;

	if (*estado!=TONO) *nivel_max = 0.0;

	for (cont1 =0;cont1<longitud;)
	{
		energ = ALFA* *energia_ant + (1 - ALFA)* trama[cont1]*trama[cont1];

		switch(*estado)
		{
			/*---- LA SEÑAL ESTA A NIVEL BAJO ----*/
			case SILENCIO:
				if(energ > UMBRAL1)   /* pasamos el umbral de subida */
				{
					*estado = TONO;
					*cambio = cont1;
					*nivel_max = 0.0;
					break;
				}
				break;

			/*---- LA SEÑAL ESTA A NIVEL ALTO ----*/
			case TONO:
				if(energ > *nivel_max)  /* actualizamos nivel maximo */
				{
					*nivel_max = energ;
					break;
				}
				if(energ < UMBRAL2* *nivel_max)  /* pasamos umbral de bajada */
				{
					*estado = BAJADA;
					*cambio = cont1;
					break;
				}
				break;

			/*---- LA SEÑAL ESTA BAJANDO DE NIVEL ----*/
			case BAJADA:
				if(energ < UMBRAL1)     /* llegamos a bajo nivel */
				{
					*estado = SILENCIO;
					break;
				}
				if(energ > *energia_ant)  /* la señal vuelve a subir antes de llegar abajo */
				{
					*estado = TONO;
					*cambio = cont1;
					*nivel_max = 0.0;
					break;
				}
				break;

		}/*switch*/
		*energia_ant = energ;
		energ = 0.0;
		cont1++;
	}/*for*/
	
  } /* fin de la rutina */


void detecta_nivel2H(float *trama,int longitud,float *energia_ant,int *estado,
                   float *nivel_max,int *cambio)
 {
	register int cont1;
	register float energ;

	if (*estado!=TONO) *nivel_max = 0.0;

	for (cont1 =0;cont1<longitud;)
	{
		energ = ALFA* *energia_ant + (1 - ALFA)* trama[cont1]*trama[cont1];

		switch(*estado)
		{
			/*---- LA SEÑAL ESTA A NIVEL BAJO ----*/
			case SILENCIO:
				if(energ > UMBRAL1_B)   /* pasamos el umbral de subida */
				{
					*estado = TONO;
					*cambio = cont1;
					*nivel_max = 0.0;
					break;
				}
				break;

			/*---- LA SEÑAL ESTA A NIVEL ALTO ----*/
			case TONO:
				if(energ > *nivel_max)  /* actualizamos nivel maximo */
				{
					*nivel_max = energ;
					break;
				}
				if(energ < UMBRAL2* *nivel_max)  /* pasamos umbral de bajada */
				{
					*estado = BAJADA;
					*cambio = cont1;
					break;
				}
				break;

			/*---- LA SEÑAL ESTA BAJANDO DE NIVEL ----*/
			case BAJADA:
				if(energ < UMBRAL1_B)     /* llegamos a bajo nivel */
				{
					*estado = SILENCIO;
					break;
				}
				if(energ > *energia_ant)  /* la señal vuelve a subir antes de llegar abajo */
				{
					*estado = TONO;
					*cambio = cont1;
					*nivel_max = 0.0;
					break;
				}
				break;

		}/*switch*/
		*energia_ant = energ;
		energ = 0.0;
		cont1++;
	}/*for*/
	
  } /* fin de la rutina */


/******************************************************************************

   dettono_425
                                                                 
   FUNCION:
   
   Decide si en una trama hay un tono de 425 Hz en base a la relacion que haya
   entre la energia de la señal a esa frecuencia y la energia total de la trama.
                                                                 
   ENTRADAS:                                                     

   int longitud: número de muestras en la trama
   float *trama: puntero a las muestras temporales
   float *etono: valor de la energia total de la trama
   float nivel: umbral para comprobar la relación entre la energía total
                y la energía de un tono monofrecuencia
   float umb_abs: umbral absoluto de detección
                                                                
   SALIDAS:                                                  
   
   float *etono: puntero a la energia del tono de señalización de 425Hz
   float *umb_trigger: energía máxima detectada en la frecuencia de
                       señalización de línea utilizada para el trigger
                       de nivel de señal
   La función devuelve además mediante un entero el valor del tono detectado.
   . 425Hz:       1
   . desconocido: 0
   
   LLAMADA DESDE C:						   					

   int dettono_425(int longitud,float *trama,float *etono,float nivel,
                     float umb_abs,float *umb_trigger)
                                                                 
******************************************************************************/
int dettono_425(int longitud,float *trama,float *etono,float nivel,
                  float *umb_trigger)
{
	short i;       /* Contador para la rutina */
	float s[3];
	float esenal = etono[0];

    /* Calculo la energia en 425Hz */	
    	s[0]=trama[0];    	    
	s[1]=trama[1]+cte425*trama[0];
	for (i=2;i<longitud;i++)
    	{
	  	s[2]=trama[i]+cte425*s[1]-s[0];
      		s[0]=s[1];	
      		s[1]=s[2];  		
    	}			/* end del for que recorre la trama */
    	etono[1]=s[1]*s[1]-cte425*s[1]*s[0]+s[0]*s[0]; 
	
	*umb_trigger=etono[1];		

    /* Pruebas para ver si hay o no una señal de 425Hz */
    if ( etono[1]*nivel>esenal ) return (18);
		
	/* NO HAY TEST CON UMBRAL ABSOLUTO, SE HACE FUERA DE LA FUNCIÓN */
	
	return (0);
    
}  /* end de la rutina de deteccion de 425Hz */

///******************************************************************************
//*                                                                             *
//*				PROTOCOLO N5                                                  *
//*	                                                                          *
//******************************************************************************/
//
///******************************************************************************
//
//   detecta_slN5
//                                                                 
//   FUNCION:
//   
//   Analiza la trama de muestras temporales y busca en ella los tonos de
//   señalización de línea del protocolo N5. Para que la búsqueda sea
//   positiva, se deben pasar unos test de señal
//                                                                 
//   ENTRADAS:                                                     
//
//   int longitud: número de muestras en la trama
//   float *trama: puntero a las muestras temporales
//   float *etono: puntero a la energia total de la trama
//   float nivel: umbral para comprobar la relación entre la energía total
//                y la energía de un tono monofrecuencia
//   float umb_abs: umbral absoluto de detección
//   float umb_rel: umbral relativo usado en multifrecuencia para una frecuencia
//   float sumb_rel: umbral relativo usado en multifrecuencia para la suma de energías
//   			 de las dos frecuencias
//                                                                
//   SALIDAS:                                                  
//   
//   float *etono: puntero a las energias de los tonos de señalización
//   float *umb_trigger: energía máxima detectada en las frecuencias de
//                       señalización de línea utilizada para el trigger
//                       de nivel de señal
//   La función devuelve además mediante un entero el valor del tono detectado.
//   . 2400+2600:   11
//   . 2400Hz:      12
//   . 2600Hz:      13
//   . desconocido: 0
//   
//   LLAMADA DESDE C:						   					
//
//   int detecta_slN5(int longitud,float *trama,float *etono,float nivel,
//                    float umb_abs,float umb_rel,float sumb_rel,float *umb_trigger)
//
//                                                                 
//******************************************************************************/
//
//int detecta_slN5(int longitud,float *trama,float *etono,float nivel,
//                 float umb_abs,float umb_rel,float sumb_rel,float *umb_trigger)
//{
//	short j,i;       /* Contadores para la rutina */
//	short indice1;
//	float s[3];
//	float emax1,emax2;
//	float esenal = etono[2];
//	float ganancia=0.02856;
//	float umbral_relativo=7.18;
//	float sumbral_relativo=22.9;
//		
//
//    	/* Calculo la energia de las dos frecuencias */	
//    
//    	for (j=0;j<2;j++)
//	{
//	    	s[0]=trama[0];    	    
//	    	s[1]=trama[1]+cteMFL5[j]*trama[0];
//	    	for (i=2;i<longitud;i++)
//     		{
//		  s[2]=trama[i]+cteMFL5[j]*s[1]-s[0];
//	      	  s[0]=s[1];	
//	      	  s[1]=s[2];  		
//	     	}			/* end del for que recorre la trama */
//        	etono[j]=s[1]*s[1]-cteMFL5[j]*s[1]*s[0]+s[0]*s[0]; 
//	}
//	
//    	/* de las energias de los dos tonos el mayor en emax 1 y en otro en emax2 */
//    	
//    	emax1 = etono[0];
//	indice1 = 0;    	
//    	if(etono[1]>emax1)
//	{
//	  emax2 = emax1;
//	  emax1 = etono[1];
//	  indice1 = 1;
//	}
//	else
//	  emax2 = etono[1];
//	
//    	/* Pruebas para ver si hay o no una señal de linea N5 (2400 ó 2600)*/
//    	if ( emax1*ganancia>esenal )
//    	{
//	      *umb_trigger=emax1;
//	      if (emax1<umb_abs) 
//	      			return (0);
//	      else 
//	      			return (12+indice1);
//	}
//        
//    // /* Pruebas para ver si hay o no una señal de linea MF N5 (2400 + 2600)*/
//    // if(!SCV_DS_R2[Circuito]){  //para evitar problemas con la deteccion de 2400+2600 en SCV_DS
//		// /* Twist */
//		// if (emax2*ctetwist< emax1) return (0);	
//		// /* Tono-total */
//		// if (emax2<umbral_relativo*esenal) return (0);
//	// }
//	/* Tono-total */
//    if ((emax1<umbral_relativo*esenal)||(emax1+emax2<sumbral_relativo*esenal))
//		return (0);	
//    
//    *umb_trigger=emax2;
//    
//    /* Magnitud */
//    if (emax2<umb_abs) return (0);
//	
//	/* Sí hay MF de 2400+2600 */
//	return (11);
//	
//}  /* end de la rutina de deteccion de señales de línea N5 */
//
///******************************************************************************
//
//   detecta_registradorN5
//                                                                 
//   FUNCION:
//   
//   Analiza la trama de muestras temporales y busca en ella los tonos de
//   señalización de registrador del protocolo N5. Para que la búsqueda sea
//   positiva, se deben pasar unos test de señal
//                                                                 
//   ENTRADAS:                                                     
//
//   int longitud: número de muestras en la trama
//   float *trama: puntero a las muestras temporales
//   float *etono: puntero a la energia total de la trama
//   float umb_abs: umbral absoluto de detección
//   float umb_rel: umbral relativo usado en multifrecuencia para una frecuencia
//   float sumb_rel: umbral relativo usado en multifrecuencia para la
//                   suma de energías de las dos frecuencias
//                                                                
//   SALIDAS:                                                  
//   
//   float *etono: puntero a las energias de los tonos de señalización
//   float *umb_trigger: energía máxima detectada en las frecuencias de
//                       señalización de línea utilizada para el trigger
//                       de nivel de señal
//   La función devuelve además mediante un entero el valor del tono detectado.
//   . dígitos del 0 al 9: 10, 1, 2, ...,9
//   . 14, 15
//   . desconocido: 0
//   
//   LLAMADA DESDE C:						   					
//
//   int detecta_registradorN5(int longitud,float *trama,float *etono,float umb_abs,
//                          float umb_rel,float sumb_rel,float *umb_trigger)
//
//                                                                 
//******************************************************************************/
//
//int detecta_registradorN5(int longitud,float *trama,float *etono,float umb_abs,
//                          float umb_rel,float sumb_rel,float *umb_trigger)
//{
//	short j,i;       /* Contadores para la rutina */
//	short digito;
//	short indice1,indice2;
//	float s[3];
//	float emax1,emax2;
//	float esenal = etono[8];
//	int longit=longitud;
//	float umbral_absoluto=umb_abs;
//
//    /* Calculo la energia de las seis frecuencias */	
//    
//    indice1=indice2=0;
//	emax2=0.0;
//
//	for (j=0;j<6;j++)
//	{
//	    s[0]=trama[0];    	    
//	    s[1]=trama[1]+cteMFR5[j]*trama[0];
//	    for (i=2;i<longit;i++)
//     	{
//	      s[2]=trama[i]+cteMFR5[j]*s[1]-s[0];
//      	  s[0]=s[1];	
//      	  s[1]=s[2];  		
//     	}			/* end del for que recorre la trama */
//        etono[j]=s[1]*s[1]-cteMFR5[j]*s[1]*s[0]+s[0]*s[0]; 
//	}
//	
//    emax1=etono[0];	
//    for (j=1;j<6;j++)
//	{
//		if(etono[j]>emax1)
//		{
//			emax2=emax1;
//			emax1=etono[j];
//			indice2=indice1;
//			indice1=j;
//		}
//		else
//			if(etono[j]>emax2)
//			{
//				emax2=etono[j];
//				indice2=j;
//			}
//	}
//
//    /* Pruebas para ver si hay o no una señal de registrador N5 */
//    /* Twist */
//	if (emax2*ctetwist< emax1) return (0);	
//	
//	/* Tono-total */
//    if ((emax1<umb_rel*esenal)||(emax2<umb_rel*esenal)||(emax1+emax2<sumb_rel*esenal))
//		return (0);	
//
//	*umb_trigger=emax2;
//	
//	/* Magnitud */
//    if (emax2<umbral_absoluto) return (0);
//	
//	/* Cálculo del dígito */
//	digito = pesosN5[indice1]+pesosN5[indice2];
//	if (digito==11)	digito = 10;
//	else if (digito==19) digito = 15;
//
//	return (digito);  /* Si hay, retorna el simbolo apropiado */
//
//}  /* end de la rutina de deteccion de señales de registrador N5 */
//
///******************************************************************************
//
//   get_nivelesN5
//                                                                 
//   FUNCION:
//   
//   Realiza el cálculo de los niveles de las frecuencias que componen un dígito.
//   Los niveles se dan en dBm con una resolución 10 veces mayor.
//                                                                 
//   ENTRADAS:                                                     
//
//   int digito:  valor del digito detectado
//   float cte:   constante utilizada en la conversión de energía a nivel
//   float *ener: puntero al array donde se almacenan las distintas energías
//                                                                
//   SALIDAS:                                                  
//   
//   int *n1: nivel para la primera frecuencia del dígito
//   int *n2: nivel para la segunda frecuencia del dígito
//   
//   LLAMADA DESDE C:						   					
//
//   void get_nivelesN5(int digito,float cte,float *ener,int *n1,int *n2)
//                                                                 
//******************************************************************************/
//
///* 9/05/02
//   Se amplia la función para dar tambien los niveles de
//   .la señalización de línea
//   .resto de las señales de registrador (KP,ST)
//*/
//   
//void get_nivelesN5(int digito,float cte,float *ener,int *n1,int *n2)
//{
//  float ener1;
//  float ener2;
//  
//  switch( digito )
//  {
//  	case 1:
//	  ener1 = ener[0];
//	  ener2 = ener[1];
//	  break;
//	case 2:
//	  ener1 = ener[0];
//	  ener2 = ener[2];
//	  break;
//	case 3:
//	  ener1 = ener[1];
//	  ener2 = ener[2];
//	  break;
//	case 4:
//	  ener1 = ener[0];
//	  ener2 = ener[3];
//	  break;
//	case 5:
//	  ener1 = ener[1];
//	  ener2 = ener[3];
//	  break;
//	case 6:
//	  ener1 = ener[2];
//	  ener2 = ener[3];
//	  break;
//	case 7:
//	  ener1 = ener[0];
//	  ener2 = ener[4];
//	  break;
//	case 8:
//	  ener1 = ener[1];
//	  ener2 = ener[4];
//	  break;
//	case 9:
//	  ener1 = ener[2];
//	  ener2 = ener[4];
//	  break;
//	case 10:
//	  ener1 = ener[3];
//	  ener2 = ener[4];
//	  break;
///* Códigos adicionales */
//  	case 11:
//	  ener1 = ener[6];
//	  ener2 = ener[7];
//	  break;
//	case 12:
//	  ener1 = ener[6];
//	  ener2 = 0;
//	  break;
//	case 13:
//	  ener1 = ener[7];
//	  ener2 = 0;
//	  break;
//	case 14:
//	  ener1 = ener[2];
//	  ener2 = ener[5];
//	  break;
//	case 15:
//	  ener1 = ener[4];
//	  ener2 = ener[5];
//	  break;
//	default:
//	  break;
//
//  } /* end del switch digito */
//  
//  /* se realiza la conversion de energia a nivel */
//  if(ener1!=0.0)
//  {
//    ener1=10*log10(ener1) - cte;
//	*n1=_spint(ener1);
//  }
//  else	*n1 = UMBRAL_dB;
//
//  if(ener2!=0.0)
//  {
//    ener2=10*log10(ener2) - cte;
//	*n2=_spint(ener2);
//  }
//  else	*n2 = UMBRAL_dB;
//
//} /* fin de la función obtiene niveles */
//
///******************************************************************************
//
//   detecta5
//                                                                 
//   FUNCION:
//   
//   Se le pasa una trama, y esta función indica si en ella hay cualquiera de
//   las frecuencias de la norma N5. Además suministra los niveles de las
//   frecuencias de los dígitos presentes en dicha trama, si los hubiere
//                                                                 
//   ENTRADAS:                                                     
//
//   float *trama: puntero a la secuencia temporal
//   int muestras: número de muestras por trama
//   int tipo: tipo de decodificación a realizar
//   int *nivel1: umbral absoluto de detección
//                                                                
//   SALIDAS:                                                  
//   int *nivel1: nivel de la primera frecuencia
//   int *nivel2: nivel de la segunda frecuencia, o bien nivel utilizado
//                  para el trigger de nivel
//   La función devuelve mediante un valor entero un código correspondiente
//   al tono detectado
//   
//   LLAMADA DESDE C:						   					
//
//   int decodificaN5(float *trama,int muestras,int tipo,
//                    int *nivel1,int *nivel2)
//                                                                 
//******************************************************************************/
//
////int decodificaN5(float *trama,int muestras,int tipo,int *nivel1,int *nivel2)
////{
////  /* Todas las variables están inicializadas para un tamaño de trama de 80 muestras */
////  float ener[9];
////  float cte=CTE_N5;
////  float gain=0.02856;
////  float	*trama_deco;
////  int	num_muestras;
////  int	tip;
////  /* CALCULO DE GAIN (N=80,10ms)						*/
////  /* Máximo: 40									*/
////  /* Primer cero: 100Hz								*/
////  /* Segundo máximo: 150Hz							*/
////  /* Caida (goer0000.m)								*/
////  /* Se utiliza GAIN=Es/Et=0.02856, Et/Es=35.01, ^f=+-20Hz			*/
////  
////  float ttrlevl;
////  float nrgcoefp=7.18;
////  
////  /* Se pone 7.18=19.35/4.873. Significa suponer que la	*/
////  /* diferencia entre los dos tonos es de 3.873 ó 5.9dB	*/
////  
////  float nrgcoefs=22.9;
////  /* El resultado, en teoría, es como el de un tono (22.9,+-40Hz)*/
//// 
////  float umbtrg;
////  int result;
////  trama_deco=trama;
////  num_muestras=muestras;
////  tip=tipo;
////  energia(trama_deco,num_muestras,&ener[8]); // Energia de las muestras de decodificación
////
////  switch (tip)
////  {
////	case 0: /* Deteccion de todos los tipos de tonos en el estado DECODIFICA*/
////		ttrlevl=56e6;	/* -40dBm */
////		
////		result=detecta_slN5(muestras,trama,&ener[6],gain,ttrlevl,nrgcoefp,nrgcoefs,&umbtrg);
////		if (result) break;
////		
////		result=detecta_registradorN5(muestras,trama,ener,ttrlevl,nrgcoefp,nrgcoefs,&umbtrg);
////		break;
////		
////	case 1: /* Detección de todos los tipos de tonos con modo emulador */
////		ttrlevl=(float)(*nivel1)*1e6;
////		
////		result=detecta_slN5(muestras,trama,&ener[6],gain,ttrlevl,nrgcoefp,nrgcoefs,&umbtrg);
////		if (result) break;
////		result=detecta_registradorN5(muestras,trama,ener,ttrlevl,nrgcoefp,nrgcoefs,&umbtrg);
////		
////		break;
////
////	case 2: /* Deteccion de todos los tipos de tonos en trigger-monitor*/
////		ttrlevl=(float)(*nivel1)*1e6;
////		result=detecta_slN5(muestras,trama,&ener[6],gain,ttrlevl,nrgcoefp,nrgcoefs,&umbtrg);
////		if (result) break;
////		
////		result=detecta_registradorN5(muestras,trama,ener,ttrlevl,nrgcoefp,nrgcoefs,&umbtrg);
////		break;
////
////	case 3: /* Deteccion de señalización de línea con modo emulador en sesión pruebas */
////		ttrlevl=(float)(*nivel1)*1e6;
////		result=detecta_slN5(muestras,trama,&ener[6],gain,ttrlevl,nrgcoefp,nrgcoefs,&umbtrg);
////		if (result==12) result=0; /* Para no detectar la señal de toma */
////		break;
////
////	case 4: /* Deteccion de señalización de línea con modo emulador */
////		ttrlevl=(float)(*nivel1)*1e6;
////		
////		result=detecta_slN5(num_muestras,trama_deco,&ener[6],gain,ttrlevl,nrgcoefp,nrgcoefs,&umbtrg);
////		break;
////	// case 5: /* Detección de todos los tipos de tonos en SCV_DS */
////		// ttrlevl=(float)(*nivel1)*1e6;
////		
////		// result=detecta_sl_SCV_DS(muestras,trama,&ener[6],gain,ttrlevl,nrgcoefp,nrgcoefs,&umbtrg);
////		// if (result) break;
////		// result=detecta_registradorN5(muestras,trama,ener,ttrlevl,nrgcoefp,nrgcoefs,&umbtrg);
////		
////		// break;
////
////	default:
////		break;
////  } /* end del switch */
////
////  if(tip==0) get_nivelesN5(result,cte,ener,nivel1,nivel2);
////  else if((tip==1)||(tip==3)||(tip==4)) 
////  {
////    *nivel2=_spint(umbtrg/1e6);
////  }
////  
////  return result;
////
////}  /* end de la rutina de deteccion de tonos */ 


/******************************************************************************

   detnivel_1000
                                                                 
   FUNCION:
   
   Decide si en una trama hay un tono de 1000 Hz en base a la relacion que haya
   entre la energia de la señal a esa frecuencia y la energia total de la trama.
                                                                 
   ENTRADAS:                                                     

   int longitud: número de muestras en la trama
   float *trama: puntero a las muestras temporales
   float nivel:  umbral para comprobar la relación entre la energía total
                y la energía de un tono monofrecuencia                                                                
   SALIDAS:                                                  
  
   La función devuelve además mediante un entero el valor del tono detectado.
   . 1000Hz:      1
   . desconocido: 0
   
   LLAMADA DESDE C:						   					

   int detnivel_1000(int longitud,float *trama)
                                                                 
******************************************************************************/

int detnivel_1000(int longitud,float *trama)
{
	short i;       /* Contador para la rutina */
	float s[3];
	float etono,esenal;
	
	energia(trama,longitud,&esenal); // Se asigna la Energia total de la trama

    /* Calculo la energia en 1000Hz */	
    s[0]=trama[0];    	    
	s[1]=trama[1]+cte_IW*trama[0];
	for (i=2;i<longitud;i++)
    {
		s[2]=trama[i]+cte_IW*s[1]-s[0];
    	s[0]=s[1];	
    	s[1]=s[2];  		
    }	
    	
    etono=s[1]*s[1]-cte_IW*s[1]*s[0]+s[0]*s[0]; 
	
	// *umb_trigger=etono;	// Energia máxima detectada en la frecuencia de señalización de 1000Hz	

	if ( etono*0.0556>esenal ) 
		return (1);
	
	return (0);
}

/******************************************************************************

   detecta_1300
                                                                 
   FUNCION:
   
   Decide si en una trama hay un tono de 1300 Hz en base a la relacion que haya
   entre la energia de la señal a esa frecuencia y la energia total de la trama.
                                                                 
   ENTRADAS:                                                     

   int longitud: número de muestras en la trama
   float *trama: puntero a las muestras temporales
   float nivel:  umbral para comprobar la relación entre la energía total
                y la energía de un tono monofrecuencia                                                                
   SALIDAS:                                                  
    La función devuelve además mediante un entero el valor del tono detectado.
   . 1300Hz:      1
   . desconocido: 0
  float *umb_trigger: energía máxima detectada en la frecuencia de
                       señalización
					   
   LLAMADA DESDE C:						   					

   int detecta_1300(int longitud,float *trama)
                                                  
******************************************************************************/
int detecta_1300(int longitud,float *trama, float *umb_trigger)
{
	short i;       /* Contador para la rutina */
	float s[3];
	float etono,esenal;
	
	energia(trama,longitud,&esenal); // Se asigna la Energia total de la trama

    	/* Calculo la energia en 1300Hz */	
    	s[0]=trama[0];    	    
	s[1]=trama[1]+cte1300*trama[0];
	for (i=2;i<longitud;i++)
    	{
		s[2]=trama[i]+cte1300*s[1]-s[0];
      		s[0]=s[1];	
      		s[1]=s[2];  		
    	}			/* end del for que recorre la trama */
    	
    	etono=s[1]*s[1]-cte1300*s[1]*s[0]+s[0]*s[0]; 
	
	*umb_trigger=etono;	// Energia máxima detectada en la frecuencia de señalización de 1300 Hz	
    	
    	/* Pruebas para ver si hay o no una señal de 1300Hz */
    	switch (longitud)
		{
			case TAMDECO_DTMF2: case TAMDECOTR_DTMF:
				if ( etono*0.0332>esenal ) return (1); //para 64 y 66 muestras
				break;
			case TAMTRAMA:
				if ( etono*0.048>esenal ) return (1); //para 48 muestras
				break;
		}
		
	return (0);
}
