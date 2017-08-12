/* 
 * File:   USB.h
 * Author: David Ledger
 *             Wrapper for the microchip CDC USB stack
 * Created on 21 March 2017, 9:39 AM
 */

#ifndef USB_WRAP_H
#define	USB_WRAP_H
#include "Types.h"
#include "FIFO.h"
#include "usb_config.h"

#define USB_ISR_CONDITION   (PIR2bits.USBIF && PIE2bits.USBIE)

extern char  USBInputBuffer[CDC_DATA_IN_EP_SIZE];
extern char  USBOutputBuffer[CDC_DATA_OUT_EP_SIZE];

void USB_Init();
void USB_Poll();
void USB_Print(cstr pInput);

inline void USB_ISR();
#endif	/* USB_H */

