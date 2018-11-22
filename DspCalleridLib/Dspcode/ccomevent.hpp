/***************************************************************************

    Nombre: CCOMEVENT.HPP
    Descripcion: Declaracion de la clase CComeventos, un consumidor de
                 eventos para el DSP del CD40.
   
    $Revision: 1.4 $
    $Date: 2009/08/07 10:11:05 $
   
    Fecha ultima modificacion: 15/07/2009                                            
   
    Notas: 
                
   
***************************************************************************/

#ifndef INCLUIDO_COMEVENT_PUNTOHACHE
#define INCLUIDO_COMEVENT_PUNTOHACHE


//--------------------------------------------------
//  Definiciones publicas de la clase CComeventos
//--------------------------------------------------

/*
 * Retorno de funciones.
 */
 
#define CCEV_OK                      0
#define CCEV_ERROR                  -1




//--------------------------------------------------
//  Declaracion de la clase CComeventos
//--------------------------------------------------

class CComeventos
{
    public:

        CComeventos();
        
        int EvtVadTx( int, int, int );       
        int EvtVadRx( int, int, int );       
        int EvtLcen( int, int );        
        int EvtDigitoDtmf( int, int, int );  
		int EvtCallerId( int, int, unsigned char *);
        int EvtR2N5( int, int );
        int EvtCalRx( int, int );       
        int EvtBucle( int, int, int, int, int );
		int EvtDebug( int, int, int, int, int, int );               
                        
};  
    

#endif







