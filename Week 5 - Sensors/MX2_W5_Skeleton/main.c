/* 
 * File:   main.c
 * Author: Robert Stephenson
 *
 * MX2 Week 5 Skeleton
 */
#include <xc.h>
#include "ProcessorConfig.h"
#include "ISR.h"
#include "MXK.h"
#include "Config.h"
#include "Functions.h"

#include "Colours.h"
#include "Console.h"
#include <stdio.h>
#include "Motor.h"
#include "LCD.h"
#include "LED.h"
#include "HMI.h"

#define _XTAL_FREQ 12000000

void main()
{
    ////////////////////////////////////////////////////////////////////////////
    //INIT
    ////////////////////////////////////////////////////////////////////////////
    MXK_Init();
    
    //Init ADC//
    //Task 1//
    //- Set the correct port directions (which port/pin are you using?)
    //- Disable ADC related interrupts (we don't want these stopping our program)
    //- Clear ADC interrupt flag
    
    ADSHR = 1;
    //- Set relevant input channels to analog
    ADSHR = 0;
    
    //- Set voltage references to Vdd and Vss (5V and 0V respectively)
    //- Make sure the ADC is idle
    
    //- Set our results up to be right justified
    //- Disable calibration
    //- Set acquisition time to 20 TAD
    //- Set conversion clock rate to FRC
    
    //- Switch the ADC on

    if (MXK_BlockSwitchTo(eMXK_HMI)) {
        LCD_Init();
        Console_Init();
        HMI_Init();
        if (MXK_Release())
            MXK_Dequeue();
    }
    
    INTCONbits.GIE	= 0;
    ////////////////////////////////////////////////////////////////////////////
    
    
    ////////////////////////////////////////////////////////////////////////////
    //MAIN LOOP
    ////////////////////////////////////////////////////////////////////////////
    //Main program loop
    UINT16 ADCReading = 0;
    int count = 0;
    loop()
    {
        //ADC//
        if(count <=0){
            __delay_ms(2);
            //Task 2//
            //- Activate the ADC
            //- Wait for the ADC to complete

            //- Obtain the results
            //- Combine the results into a single value (ADCReading) and display
            count = 50;
        }
        count--;
            

        if (MXK_BlockSwitchTo(eMXK_HMI)) {
            printf("%c", ENDOFTEXT);
            Console_SetForecolour(WHITE);
            printf("Analog to Digital\n\n");
            //Task 3//
            //- Display the raw result
            //- Display the result as a voltage
            //- Display the result as an approximate distance
            printf("ADRES: %u\n", ADCReading);
            Console_Render();

            HMI_SetNumber(ADCReading);
            HMI_Render();
            
            if (MXK_Release())
            MXK_Dequeue();
        }
    }
    ////////////////////////////////////////////////////////////////////////////
}