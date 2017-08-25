/*
 * File:   main.c
 * Author: Jamin Early
 *
 * MX2: Sensing and Actuating
 */
#include <xc.h>
#include "ProcessorConfig.h"
#include "ISR.h"
#include "MXK.h"
#include "Config.h"
#include "Functions.h"
#include "ADC.h"

#include "Colours.h"
#include "Console.h"
#include <stdio.h>
#include <math.h>
#include "Motor.h"
#include "LCD.h"
#include "LED.h"
#include "HMI.h"

//Continuous
int direction = 200;
int speed = 0;
//Step Mode
int steps = 0;
int remainingSteps = 0;
int locked = 0;
int angle = 0;
int currentPos = 200;


//Global Variables
Motor Stepper;
extern ADC ADC_AN0;
int mode;
int dip;
bool upState;
bool downState;
bool leftState;
bool rightState;
float ADCVoltage = 0;
int IRDistance;
int diff;

//Function Prototypes
void initialise();
void getMode();
void displaySID();
void displayContinuousMode();
void displayStepMode();
void displayPositionMode();
void displayNullMode();
void displayDistance();
void continuousMode();
void stepMode();
void positionMode();
void nullMode();
void inputVar();


//Initialising function

void initialise() {
    //Init MXK Pins
    MXK_Init();

    //Init HMI
    if (MXK_BlockSwitchTo(eMXK_HMI)) {
        HMI_Init();
        LCD_Init();
        Console_Init();
        if (MXK_Release())
            MXK_Dequeue();
    }

    //Init Stepper motor
    if (MXK_BlockSwitchTo(eMXK_Motor)) {
        Motor_Init(&Stepper, MXK_MOTOR);
        if (MXK_Release())
            MXK_Dequeue();
    }

    //Init Interupts
    ISR_Enable();

    //Init ADC
    FunctInitADC();

    //Init Buttons
    FunctInitButton();
}

//Assigns various input variables such as dip switch and button states

void inputVar() {
    FunctADC();
    HMI_Poll();

    //Read and store the dip switch value in the 'dip' variable
    dip = DIPSwitch.mGetState();

    //Read the button states and store them in corresponding varables
    upState = HMIBoard.mUp.mGetState();
    downState = HMIBoard.mDown.mGetState();
    leftState = HMIBoard.mLeft.mGetState();
    rightState = HMIBoard.mRight.mGetState();
}

/*The getMode function sets the current mode as defined by the dip switches.
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
void getMode() {
    switch (dip) {
      case 1:
          mode = 1;
          return;
        case 2:
            mode = 2;
            return;
        case 4:
            mode = 3;
            return;
        default:
            mode = 0;
            return;
    }
}

//Displays SID

void displaySID() {
    Console_SetForecolour(GREEN);
    printf("Jamin Early 99133391\n\n");
}

//Displays text for continuous mode and manipulates the speed and direction variables based of button presses.

void displayContinuousMode() {
    //Staticlly display the current mode
    printf("Current Mode:\n Continuous Mode\n");
    printf("Direction: ");
    /*My motor spins clockwise when the value for direction is greater than 0.
     *The direction is shown on the screen, with 'CW' for clockwise and 'CCW' for counter-clockwise.
     */
    if (direction > 0) {
        printf("CW\n");
    } else {
        printf("CCW\n");
    }
    //Speed is displayed
    printf("Speed: %d\n", speed);

    /*Button states are read from their respective variables and direction and speed is altered respectively.
     *This is in accordance to the instructions outlined in the assignment brief.
     *These values are used in a later function, 'continuousMode()'.
     */
    if (rightState) {
        direction = 200;
    } else if (leftState) {
        direction = -200;
    } else if (upState) {
        speed++;
    } else if (downState && speed > 0) {
        speed--;
    }
}

//Displays text for step mode

void displayStepMode() {
  //Displays the current mode as step mode, the user-selected steps and the amount of steps remaining.
    printf("Current Mode:\n Step Mode\n");
    printf("Steps: %d\n", steps);
    printf("Remaining Steps: %d\n\n", remainingSteps);
    if (leftState == 1 && steps > 0) {
        steps--;
    } else if (rightState) {
        steps++;
    } else if (upState) {
        steps = 0;
      }
}

//Displays text for position mode

void displayPositionMode() {
  ADCVoltage = ADC_Voltage(&ADC_AN0);
  IRDistance = 58 * pow(ADCVoltage, -1.10);
  angle = ((IRDistance - 17) * (200) / (117 - 17));
  if (angle >= 200) {
      angle = 200;
  }
  if (angle < 0) {
      angle = 0;
  }
    printf("Current Mode:\n Position Mode\n\n");
    float angleDegrees = ((float) angle * 1.8);
    printf("Target Angle:\n %f \n", angleDegrees);

}

//Displays text when the mode is undefined

void displayNullMode() {
    printf("Current Mode:\n Undefined\n");
    printf("                      \n");
    printf("                      \n");
    printf("                      \n");
}

//Called when the current mode is undefined
void nullMode() {
//do nothing
}

//Called when the current mode is continuous

void continuousMode() {
    Motor_Speed(&Stepper, speed);
    Motor_Move(&Stepper, direction);
}

//Called when the current mode is step mode

void stepMode() {
    //    if (locked) {
    //        locked = 0;
    //        Stepper.mDelta = 0;
    //    }
    if (downState && Stepper.mDelta == 0) {
        Motor_Speed(&Stepper, KHZ(1));
        Motor_Move(&Stepper, steps);
        remainingSteps = steps;
    }
    if (Stepper.mDelta != 0) {
        remainingSteps--;
    }
}

//Called when the current mode is position Mode

void positionMode() {
    diff = angle - currentPos;
    if (Stepper.mDelta == 0 && diff > 0) {
        Motor_Speed(&Stepper, KHZ(1));
        Motor_Move(&Stepper, diff);
        currentPos--;
    }
    if (Stepper.mDelta == 0 && diff < 0) {
        Motor_Speed(&Stepper, KHZ(1));
        Motor_Move(&Stepper, diff);
        currentPos++;
    }
}

void main() {
    initialise();

    loop() {
        inputVar();
        //HMI code
        if (MXK_BlockSwitchTo(eMXK_HMI)) {
            getMode();
            printf("%c", ENDOFTEXT);
            printf("%d\n", angle);
            displaySID();
            Console_SetForecolour(RED);
            switch (mode) {
                case 1:
                    displayContinuousMode();
                    break;
                case 2:
                    displayStepMode();
                    break;
                case 3:
                    displayPositionMode();
                    break;
                default:
                    displayNullMode();
                    break;
            }
            Console_Render();
            HMI_SetNumber(IRDistance);
            HMI_Render();
            if (MXK_Release())
                MXK_Dequeue();
        }

        //Stepper code
        if (MXK_BlockSwitchTo(eMXK_Motor)) {
            switch (mode) {
                case 0:
                    nullMode();
                    break;
                case 1:
                    continuousMode();
                    break;
                case 2:
                    stepMode();
                    break;
                case 3:
                    positionMode();
                    break;
            }
            if (MXK_Release())
                MXK_Dequeue();
        }
    }
}
