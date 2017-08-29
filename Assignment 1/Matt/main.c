/* 
 * File:   main.c
 * Author: Robert Stephenson
 *
 * MX2: MXK Skeleton
 */
#include <xc.h>
#include "ProcessorConfig.h"
#include "ISR.h"
#include "ADC.h"
#include "MXK.h"
#include "Config.h"
#include "Functions.h"

#include "Colours.h"
#include "Console.h"
#include <stdio.h>
#include <math.h>
#include "Motor.h"
#include "LCD.h"
#include "LED.h"
#include "HMI.h"

bool lockedMode = 1;
//Continuous
int Direction = 200;
int Speed = 0;
//Step Mode    
int Steps = 0;


//Global Variables
Motor Stepper;
extern ADC ADC_AN0;
int dipMode;
bool upState;
bool downState;
bool leftState;
bool rightState;

void init() {
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

    //Init stepper

    if (MXK_BlockSwitchTo(eMXK_Motor)) {
        Motor_Init(&Stepper, MXK_MOTOR);
        if (MXK_Release())
            MXK_Dequeue();
    }

    //Init interrupts
    //
    //
    ISR_Enable();

    FunctInitADC();
    FunctInitButton();
}

void inputVar() {
    FunctADC();
    HMI_Poll();
    dipMode = DIPSwitch.mGetState();
    upState = HMIBoard.mUp.mGetState();
    downState = HMIBoard.mDown.mGetState();
    leftState = HMIBoard.mLeft.mGetState();
    rightState = HMIBoard.mRight.mGetState();
}

void printToScreen(){
    printf("%c", ENDOFTEXT);
            Console_SetForecolour(GREEN);
            printf("Jamin Early 99133391\n\n");
            switch (dipMode) {
                case 1:
                    printf("Continuous Mode:\n");
                    printf("Direction: ");
                    if (Direction > 0) {
                        printf("CCW\n");
                    } else {
                        printf("CW\n");
                    }
                    printf("Speed: %dHz\n", Speed);
                    if (leftState) {
                        Direction = -200;
                    } else if (rightState) {
                        Direction = 200;
                    } else if (upState) {
                        Speed++;
                    } else if (downState && Speed > 0) {
                        Speed--;
                    }
                    break;
                case 2:
                    printf("Step Mode:\n");
                    printf("Steps: %d\n\n", Steps);
                    if (leftState == 1 && Steps > 0) {
                        Steps--;
                    } else if (rightState) {
                        Steps++;
                    } else if (upState) {
                        Steps = 0;
                    } else if (downState) {
                        //Run
                    }
                    break;
                case 4:
                    printf("Position Mode\n");

                    break;
                default:
                    lockedMode = 1;
                    printf("Undefined\n\n\n\n\n\n");

            }
            Console_Render();
            float ADCVoltage = ADC_Voltage(&ADC_AN0);

            int IRDistance = 65 * pow(ADCVoltage, -1.10);

            HMI_SetNumber(IRDistance);
            HMI_Render();
}

void main() {
    init();

    loop() {
        inputVar();

        //HMI code
        if (MXK_BlockSwitchTo(eMXK_HMI)) {
            printToScreen();
            if (MXK_Release())
                MXK_Dequeue();
        }

        //Stepper code
        if (MXK_BlockSwitchTo(eMXK_Motor)) {
            switch (dipMode) {
                case 1:
                    Motor_Speed(&Stepper, KHZ(Speed));
                    Motor_Move(&Stepper, Direction);
                    break;
                case 2:
                    if (lockedMode) {
                        lockedMode = 0;
                        Stepper.mDelta = 0;
                    }
                    if (downState && Stepper.mDelta == 0) {
                        Motor_Speed(&Stepper, KHZ(1));
                        Motor_Move(&Stepper, Steps);
                    }
                    break;
                case 4:
                    if (lockedMode) {
                        int rotation = 0;
                        lockedMode = 0;
                        int movetoPos = 0;
                    }


                    break;
                default:
                    break;
            }
            if (MXK_Release())
                MXK_Dequeue();
        }
    }
}