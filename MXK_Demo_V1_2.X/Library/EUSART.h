/* 
 * File:   EUSART.h
 * Author: David Ledger
 *
 * Created on 3 April 2017, 10:31 AM
 */
#ifndef EUSART_H
#define	EUSART_H
#include <xc.h>
#include "Types.h"
#include "Config.h"

#define EUSART_RECIEVE_ISR_CONDITION     (PIR1bits.RC1IF && PIE1bits.RC1IE)
#define EUSART_TRANSMIT_ISR_CONDITION    (PIR1bits.TX1IF && PIE1bits.TX1IE)

#define TXSTA       TXSTA1bits
#define RCSTA       RCSTA1bits
#define BAUDCON     BAUDCON1
#define BIT_BRG16   TXSTA.BRG16
#define BIT_BRGH    TXSTA.BRGH
#define BIT_SYNC    TXSTA.SYNC

typedef enum
{
    eEUSART_Sync = 1,
    eEUSART_Async = 0
}   eEUSART_Mode;
typedef enum
{
    eEUSART_Master = 1,
    eEUSART_Slave = 0
}   eEUSART_Role;
typedef enum
{
    eEUSART_9Bit = 1,
    eEUSART_8Bit = 0
}   eEUSART_TXLength;

void EUSART_Init(UINT32 pBaudrate, eEUSART_Role pRole, eEUSART_Mode pMode, eEUSART_TXLength pLength);
bool EUSART_Busy();
bool EUSART_Send(str pInput, UINT16 pLength);

#ifdef EUSARTISR
#define EUSART_Poll()
#else
void EUSART_Poll();
#endif

#ifdef EUSARTISR
void EUSART_RECIEVE_ISR();
void EUSART_TRANSMIT_ISR();
#endif

#endif	/* EUSART_H */