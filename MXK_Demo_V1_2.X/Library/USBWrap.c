#include "USBWrap.h"
#include <stdio.h>
#include <string.h>

#include "Config.h"
#include <xc.h>
#include "usb.h"

#include "usb_device_cdc.h"
#include "Events.h"

char  USBInputBuffer[CDC_DATA_IN_EP_SIZE];
char  USBOutputBuffer[CDC_DATA_OUT_EP_SIZE];

void USB_Tasks();
void USB_Init()
{    
    line_coding.bCharFormat = 0;
    line_coding.bDataBits   = 8;
    line_coding.bParityType = 0;
    line_coding.dwDTERate   = 9600;

    USBDeviceInit();
    USBDeviceAttach();
}

void USB_Poll()
{
    #if defined(USB_POLLING)
	// Interrupt or polling method.  If using polling, must call
	// this function periodically.  This function will take care
	// of processing and responding to SETUP transactions
	// (such as during the enumeration process when you first
	// plug in).  USB hosts require that USB devices should accept
	// and process SETUP packets in a timely fashion.  Therefore,
	// when using polling, this function should be called
	// regularly (such as once every 1.8ms or faster** [see
	// in-line code comments in usb_device.c for explanation when
	// "or faster" applies])  In most cases, the USBDeviceTasks()
	// function does not take very long to execute (ex: <100
	// instruction cycles) before it returns.
	USBDeviceTasks();
    #endif

    /* If the USB device isn't configured yet, we can't really do anything
     * else since we don't have a host to talk to.  So jump back to the
     * top of the while loop. */
    if( USBGetDeviceState() < CONFIGURED_STATE )
	return;

    /* If we are currently suspended, then we need to see if we need to
     * issue a remote wakeup.  In either case, we shouldn't process any
     * keyboard commands since we aren't currently communicating to the host
     * thus just continue back to the start of the while loop. */
    if( USBIsDeviceSuspended()== true )
	return;

    //Application specific tasks
    USB_Tasks();
}

void USB_Tasks()
{
    UINT8 Recieved;
    /* Check to see if there is a transmission in progress, if there isn't, then
     * we can see about performing an echo response to data received.
     */
    if( USBUSARTIsTxTrfReady() == true)
    {

	Recieved = getsUSBUSART(USBInputBuffer, CDC_DATA_IN_EP_SIZE);
	if (Recieved > 0)
	{
#ifdef USB_ECHO
	    putUSBUSART(USBInputBuffer,  Recieved);
#else
	    Recieved = getsUSBUSART(USBInputBuffer, CDC_DATA_IN_EP_SIZE);
#endif
	    Event_USBRecieve(Recieved);
	}
    }    
    CDCTxService();
}

inline void USB_ISR()
{
    #if defined(USB_INTERRUPT)
        USBDeviceTasks();
    #endif
}

void USB_Print(cstr pInput)
{
    UINT16 len = strlen(pInput);
    putUSBUSART(pInput, len);
}