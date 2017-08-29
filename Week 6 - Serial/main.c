/* 
 * File:   main.c
 * Author: Robert Stephenson
 *
 * MX2: MXK W6 Skeleton
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

    //Task 1 - Initialise the EUART (refer to datasheet section 20.0)
    //
    //Set correct pin directions (refer to schematic)  Hint: TX=OUT and RX=IN
    //Set correct number of transmission bits (refer to iRobot user manual)
    //Enable transmission
    //Decide on transmission mode
    //Disable sync break characters
    
    //Enable the serial port
    //Set correct number of reception bits (same as transmission)
    //Enable continuous reception
    //Disable any framing or overrun errors
    
    //Disable data inversion
    //Idle state is high
    //Disable monitoring of the RX pin for wake up
    //Disable automatic baud detection
    
    //Task 2 - Set the baud rate (bits per second)
    //
    //Check the iRobot user guide for the required baud rate and note it down
    //Use the formulas in the datasheet (table 20-1) to calculate the required value for SPBRG
    //Ensure that BRGH and BRG16 are set according to the formula you chose to use
    //Calculate the baud rate error using the formulas in the datasheet (example 20-1)
    
    loop()
    {
        //Task 3 - Transmit commands to the iRobot
        //
        //Use the same conditional looping technique that was used last week to
        //poll the TRMT bit (shows the status of the transmission shift register)
        //If the shift register is empty then assign a value to be transmitted
        //Transmission will now take place automatically
        
        //Task 4 - Receive data from the iRobot
        //
        //Use the same conditional looping technique above to check the receive flag
        //If you are expecting data to be returned then you must wait for it
        //Once data is received you can read it from the receiver and store it
        //Clear any relevant flags as the final step
        
        //HMI code
        if (MXK_BlockSwitchTo(eMXK_HMI)) {

            if (MXK_Release())
            MXK_Dequeue();
        }
    }
}