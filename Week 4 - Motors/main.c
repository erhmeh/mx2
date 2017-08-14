/* 
 * File:   main.c
 * Author: Robert Stephenson
 *
 * MX2 Week 4 Skeleton
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

Motor motor;

void main()
{
    ////////////////////////////////////////////////////////////////////////////
    //INIT
    ////////////////////////////////////////////////////////////////////////////
    //Init MXK pins
    MXK_Init();

    //Init LCD
    if (MXK_BlockSwitchTo(eMXK_HMI)) {
        LCD_Init();
        
        Console_Init();
        if (MXK_Release())
            MXK_Dequeue();
    }
    
    //Init SSD
    if (MXK_BlockSwitchTo(eMXK_HMI))
    {
        HMI_Init();
        
        if (MXK_Release())
            MXK_Dequeue();
    }
    
    //Init stepper motor
    //Task 1 - We must first initialise the motor module for use
    //- Define any required motor variables here (keep reading first)
    if (MXK_BlockSwitchTo(eMXK_Motor)) {
        //- Open up Motor.c, find the initialisation function and execute it here
        //Hint: See Config.h for module addresses
        Motor_Init(&motor, MXK_MOTOR);
        Motor_Move(&motor, 100);
    if (MXK_Release())
                MXK_Dequeue();
    }
    
    //Init interrupts
    //Task 2 - The motor module requires use of Timer 0 interrupts to step the motor regularly
    //- Switch on all necessary interrupt options inside the PIC to allow Timer 0 and clear any relevant flags
    //Hint: Look at global, peripheral and timer based interrupts in the PIC datasheet.
    ////////////////////////////////////////////////////////////////////////////
    ISR_Enable();
    
    ////////////////////////////////////////////////////////////////////////////
    //MAIN LOOP
    ////////////////////////////////////////////////////////////////////////////
    //Main program loop
    loop()
    {
        //HMI work
        if (MXK_BlockSwitchTo(eMXK_HMI)) {
            
            //Write data to LCD
            printf("%c", ENDOFTEXT);
            Colour myColours[] = {RED, WHITE, BLUE, GREEN, CYAN};
            int i;
            char myName[] = "Motor";
            for(i = 0; i < 5; i++){
                Console_SetForecolour(myColours[i]);
                printf("%c", myName[i]);
                Console_Render();
            }
            Console_SetForecolour(WHITE);
            printf(" Information\n");
            Console_Render();
            
            //Task 4 - We would like to display stepper information to the user
            //- Define and display a 'steps remaining' variable
            //Hint: The mDelta variable may be useful here
            
            //Task 5 (Advanced) - We would like to give the user control over the motor
            HMI_Poll();
            UINT8 upState = HMIBoard.mUp.mGetState();
            //- Define and display a 'target number of steps'
            //- Allow the user to increase or decrease the 'target number of steps' with the up and down buttons
            //- Allow the user to begin stepping by pressing the left or right button
            
            //Write to SSD
            HMI_SetSegments("2017");
            HMI_Render();
            
            if (MXK_Release())
            MXK_Dequeue();
        }
        
        //Actuate stepper motor
        if (MXK_BlockSwitchTo(eMXK_Motor)) {
            //Task 3 - Time to actuate the stepper motor.  Look inside the Motor.c file for the
            //appropriate functions.
            //- Set the motor speed
            //- Activate the motor
            //Hint: Be careful not to call the function repeatedly within the main loop.  You may need
            //to use the mDelta variable to prevent this.
            Motor_Speed(&motor, 256);
            Motor_Continious(&motor, 1);
            
            if (MXK_Release())
            MXK_Dequeue();
        }
    }
    ////////////////////////////////////////////////////////////////////////////
}