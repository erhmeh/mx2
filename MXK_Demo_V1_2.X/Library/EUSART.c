#include "EUSART.h"

void EUSART_Init(UINT32 pBaudrate, eEUSART_Role pRole, eEUSART_Mode pMode, eEUSART_TXLength pLength)
{
    //Add user code here
}

bool EUSART_Busy()
{
    //Returns true when the EUSART is in the middle of
    // transaction.
    return false;
}

bool EUSART_Send(str pInput, UINT16 pLength)
{
    //Add user code here
    //User to a FIFO and kick-start the interrupt process
    //Alternatively you can poll to see when the transaction is complete
    return true;
}
#ifdef EUSARTISR
void EUSART_RECIEVE_ISR()
{
    //Trigger "event" here when data received
    
}
void EUSART_TRANSMIT_ISR()
{
    //Triggered when data is finished transmitting
    
}
#else
void EUSART_Poll()
{
    //This should be implemented if interrupts are not used
    //you must transmit bytes from the FIFO
    
    
    // and 
    //Recieve bytes when they are avalible and add them to the RX fifo, you can
    // trigger an event here
    
}
#endif