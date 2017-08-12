/* 
 * File:   main.c
 * Author: User
 *
 * Created on 13 September 2016, 12:03 PM
 */

////////////////////////////////////////////////////////////////////////////////
#include <xc.h>
#include "ProcessorConfig.h"
#include "ISR.h"
#include "MXK.h"
#include "FIFO.h"
#include "Config.h"
#include "Functions.h"
#include "FPGA.h"

void main()
{
    ////////////////////////////////////////////////////////////////////////////
    //Configures the input and output states of the PIC ports
    MXK_Init();

    //////////////////////////////////////////////////////////////////////////// 
    //Initialize all selected modules
#ifdef BUTTONTEST
    FunctInitButton ();
#endif
#ifdef ADCTEST
    FunctInitADC    ();
#endif
#ifdef LCDTEST
    FunctInitLCD    ();
#endif
#if defined(SEVENSEGTEST) || defined(HMIBUTTONTEST)
    FunctInitHMI    ();
#endif
#ifdef IMUTEST
    FunctInitIMU    ();
#endif
#ifdef USBTEST
    FunctInitUSB    ();
#endif  
#ifdef MOTORTEST
    FunctInitMotor  ();
#endif
#ifdef USARTTEST
    FunctInitEUSART  ();
#endif
#ifdef ASMMODE
    FunctInitASM();
#endif
    ////////////////////////////////////////////////////////////////////////////
    //ADD USER INITIALIZATION CODE HERE
    // ---->
    // <----
    ////////////////////////////////////////////////////////////////////////////
    //Enable global interrupts, this is required for HMI, Motor and optionally
    // the LCD libraries
    ISR_Enable();
    loop()
    {
	////////////////////////////////////////////////////////////////////////////
	//ADD USER RUNTIME CODE HERE
	// ---->
	
	// <----
	////////////////////////////////////////////////////////////////////////////
	//Run code from all selected modules
#ifdef EUSARTTEST
	FunctEUSART();
#endif
#ifdef ADCTEST
	FunctADC();
#endif
#ifdef ASMMODE
	FunctASM();
#endif
#ifdef USBTEST
	FunctUSB();
#endif
#ifdef BUTTONTEST
	FunctButton();
#endif
#ifdef HMITEST
    #ifndef ASMMODE
	FunctHMI();	
    #endif
#endif
#ifdef IMUTEST		
	FunctIMU();
#endif
#ifdef MOTORTEST	
	FunctMotor(); 
#endif
    }
}