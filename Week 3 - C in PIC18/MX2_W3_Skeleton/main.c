/* 
 * File:   main.c
 * Author: Robert Stephenson
 *
 * MX2 Week 3 Skeleton
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
    ////////////////////////////////////////////////////////////////////////////
    //INIT
    ////////////////////////////////////////////////////////////////////////////
    //Init MXK Pins
    MXK_Init();

    //Init LCD
    if (MXK_BlockSwitchTo(eMXK_HMI)) {
        //Task 1 - Place LCD initialisation code here

        if (MXK_Release())
            MXK_Dequeue();
    }
    
    //Init SSD
    if (MXK_BlockSwitchTo(eMXK_HMI))
    {
        //Task 2 - Place SSD initialisation code here
        
        if (MXK_Release())
            MXK_Dequeue();
    }
    ////////////////////////////////////////////////////////////////////////////
    
    
    ////////////////////////////////////////////////////////////////////////////
    //MAIN LOOP
    ////////////////////////////////////////////////////////////////////////////
    //Main program loop
    loop()
    {
        //Write data to LCD
        if (MXK_BlockSwitchTo(eMXK_HMI)) {
            //Task 1 - Write LCD code here
            
            if (MXK_Release())
            MXK_Dequeue();
        }
        
        //Write data to SSD
        if (MXK_BlockSwitchTo(eMXK_HMI)) {
            //Task 2 - Write SSD code here
            
            if (MXK_Release())
            MXK_Dequeue();
        }
        
        //Read button states
        if (MXK_BlockSwitchTo(eMXK_HMI)) {
            //Task 3 - Handle button information here
            //Hint 1: HMIBoard.mDown.mGetState() will return the 
            //state of the Down button (0 or 1))
            //Hint 2: HMI_Poll() needs to be called to update the button states
            //with their physical state
            
            if (MXK_Release())
            MXK_Dequeue();
        }
    }
    ////////////////////////////////////////////////////////////////////////////
}