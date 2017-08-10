/* 
 * File:   main.c
 * Author: Robert Stephenson
 *
 * MX2: MXK Skeleton
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

void main()
{
    //Init MXK Pins
    MXK_Init();

    //Init HMI
    if (MXK_BlockSwitchTo(eMXK_HMI)) {

        if (MXK_Release())
            MXK_Dequeue();
    }
    
    //Init stepper
    Motor Stepper;
    if (MXK_BlockSwitchTo(eMXK_Motor)) {

        if (MXK_Release())
                MXK_Dequeue();
    }

    //Init interrupts
    //
    //
    
    loop()
    {
        //Read IR sensor
        //
        //
        
        //HMI code
        if (MXK_BlockSwitchTo(eMXK_HMI)) {

            if (MXK_Release())
            MXK_Dequeue();
        }

        //Stepper code
        if (MXK_BlockSwitchTo(eMXK_Motor)) {

            if (MXK_Release())
            MXK_Dequeue();
        }
    }
}