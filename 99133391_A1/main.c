/* 
 * File:   main.c
 * Author: Jamin Early
 *
 * MX2: Assignment 1
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

//Prototype functions
void initialise();
void displaySID();
void displayText();
int getMode();
void displayMode();
void continuousMode();
void stepMode();
void positionMode();
void nullMode();
void setMode();

/*Initialisation of components*/
void initialise() {
    //Initialise MXK Pins
    MXK_Init();

    //Initialise HMI
    if (MXK_BlockSwitchTo(eMXK_HMI)) {

        LCD_Init();
        Console_Init();
        HMI_Init();
        if (MXK_Release())
            MXK_Dequeue();
    }

    //Initialise stepper
    //    Motor Stepper;
    //    if (MXK_BlockSwitchTo(eMXK_Motor)) {
    //
    //        if (MXK_Release())
    //                MXK_Dequeue();
    //    }

    //Init interrupts
    //
    //
}

/*The displayStaticText function is called to display my name and SID*/
void displaySID() {
    Console_SetForecolour(GREEN);
    printf("Jamin Early 99133391\n");
}

/*Displays text to console*/
void displayText() {
    //Clear console for use
    printf("%c", ENDOFTEXT);

    //Display Name and SID
    displaySID();

    //Display the selected mode
    displayMode();

    //Render the console
    Console_Render();
}

/*The getMode function returns the current mode as defined by the dip switches. 
 * The modes are defined as follows:
 * 0-Undefined
 * 1-Continuous Mode
 * 2-Step Mode
 * 3-Position Mode
 * 
 * The displayMode function prints the current mode to the console.
 * 
 * The setMode function calls the corresponding function depending on the what the getMode function returns.
 */
int getMode() {
    HMI_Poll();
    int dipState = HMIBoard.mDIP.mGetState();
    switch (dipState) {
        case 2:
            return 3;
        case 4:
            return 2;
        case 8:
            return 1;
        default:
            return 0;
    }
}
void displayMode() {
    Console_SetForecolour(RED);
    int mode = getMode();
    printf("Current Mode:\n");
    switch (mode) {
        case 3:
            printf(" Position\n\n");
            break;
        case 2:
            printf(" Step\n\n");
            break;
        case 1:
            printf(" Continuous\n\n");
            break;
        default:
            printf(" Undefined\n\n");
            break;
    }
}
/*Function for when continuous mode is selected*/
void continuousMode() {
    //Current mode
    int mode = getMode();
    //Default speed is stationary
    int speed = 0;

    //Default direction is clockwise. 0 is Clockwise | 1 is Anti-clockwise
    int direction = 0;

    //Remain in loop while set in continuous mode.
    while (mode == 1) {

        //Up button increases speed up to a maximum value of 10 
        if ((int) HMIBoard.mUp.mGetState() == 1 && (int) HMIBoard.mDown.mGetState() == 0 & speed <= 9) {
            while((int) HMIBoard.mUp.mGetState() == 1){}
            speed++;
        }
        //Down button decreases speed to a minimum value of 0
        if ((int) HMIBoard.mDown.mGetState() == 1 && (int) HMIBoard.mUp.mGetState() == 0 & speed > 0) {
            while((int) HMIBoard.mDown.mGetState() == 1){}
            speed--;
        }
        //Left button sets mode to Anti-clockwise
        if ((int) HMIBoard.mLeft.mGetState() == 1 && (int) HMIBoard.mRight.mGetState() == 0) {
            direction = 1;
        }
        //Right button sets mode to clockwise
        if ((int) HMIBoard.mRight.mGetState() == 1 && (int) HMIBoard.mLeft.mGetState() == 0) {
            direction = 0;
        }
        displayText();
        Console_SetForecolour(YELLOW);
        printf("Speed: %d\n", speed);
        printf("Direction: %d\n", direction);
        Console_Render();
        mode = getMode();
        HMI_Poll();
    }
}
/*Function for when step mode is selected*/
void stepMode() {
}
/*Function for when position mode is selected*/
void positionMode() {
}
/*Function for when no mode is selected*/
void nullMode() {
    displayText();
    printf("                      \n");
    printf("                      \n");
    printf("                      \n");
    Console_Render();
}

void setMode() {
    int mode = getMode();
    switch (mode) {
        case 1:
            continuousMode();
            break;
        case 2:
            stepMode();
            break;
        case 3:
            positionMode();
            break;
        default:
            nullMode();
            break;
    }
}

void main() {
    initialise();

    loop() {
        //Write data to LCD
        if (MXK_BlockSwitchTo(eMXK_HMI)) {

            displayText();
            
            //Set the mode
            setMode();

            if (MXK_Release())
                MXK_Dequeue();
        }

        //Read IR sensor
        //
        //

        //HMI code
        //        if (MXK_BlockSwitchTo(eMXK_HMI)) {
        //
        //            if (MXK_Release())
        //            MXK_Dequeue();
        //        }
        //
        //        //Stepper code
        //        if (MXK_BlockSwitchTo(eMXK_Motor)) {
        //
        //            if (MXK_Release())
        //            MXK_Dequeue();
        //        }
    }
}