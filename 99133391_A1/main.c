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




void main()
{
    //Init MXK Pins
    MXK_Init();
	
	bool lockedMode = 1;
    //Continuous
    int Direction = 200;
    int Speed = 0;
    //Step Mode    
    int Steps = 0;
    
    //IRStuff
    
    
    //Init HMI
    if (MXK_BlockSwitchTo(eMXK_HMI)) {
        HMI_Init();
        LCD_Init();
        Console_Init();
        if (MXK_Release())
            MXK_Dequeue();
    }
    
    //Init stepper
    Motor Stepper;
    if (MXK_BlockSwitchTo(eMXK_Motor)) {
        Motor_Init(&Stepper,MXK_MOTOR);
        if (MXK_Release())
                MXK_Dequeue();
    }

    //Init interrupts
    //
    //
    ISR_Enable();
    extern ADC ADC_AN0;
    FunctInitADC();
	FunctInitButton();
    
    loop()
    {
        //Read IR sensor
        // 
        //
        FunctADC();
        HMI_Poll();
        int DipValue = DIPSwitch.mGetState();
        bool UpValue = HMIBoard.mUp.mGetState();
        bool DownValue = HMIBoard.mDown.mGetState();
        bool LeftValue = HMIBoard.mLeft.mGetState();
        bool RightValue = HMIBoard.mRight.mGetState();
        
        
        //HMI code
        if (MXK_BlockSwitchTo(eMXK_HMI)) {
            printf("%c",ENDOFTEXT);
            printf("Matt Woods\n9914517\nAssignment1\n\n");
            
            
            printf("DIP Switches: %d\n",DipValue);
            switch (DipValue){
                case 1:
                    printf("Continuous Mode:\n");
                    printf("Direction: ");
                    if(Direction>0){
                        printf("CCW\n");
                    }else{
                        printf("CW\n");
                    }
                    printf("Speed: %dHz\n",Speed);
                    if(LeftValue){
                        Direction = -200;
                    }else if(RightValue){
                        Direction = 200;
                    }else if(UpValue){
                        Speed++;
                    }else if (DownValue && Speed>0){
                        Speed--;
                    }
                    break;
                case 2:
                    printf("Step Mode:\n");
                    printf("Steps: %d\n\n",Steps);
                    if(LeftValue == 1 && Steps > 0){
                        Steps--;
                    }else if(RightValue){
                        Steps++;
                    }else if(UpValue){
                        Steps=0;
                    }else if (DownValue){
                        //Run
                    }
                    break;
				case 4:
					printf("Position Mode\n");
					
					break;
                default:
					lockedMode = 1;
                    printf("Invalid Mode!\n\n\n\n\n\n");
                    
            }
            Console_Render();
            float ADCVoltage = ADC_Voltage(&ADC_AN0);           

            int IRDistance = 65*pow(ADCVoltage, -1.10);
            
            HMI_SetNumber(IRDistance);
            HMI_Render();
            if (MXK_Release())
            MXK_Dequeue();
        }

        //Stepper code
        if (MXK_BlockSwitchTo(eMXK_Motor)) {
            switch (DipValue){
                case 1:
                    Motor_Speed(&Stepper,KHZ(Speed));
                    Motor_Move(&Stepper,Direction);
                    break;
                case 2:
					if(lockedMode){
						lockedMode=0;
						Stepper.mDelta = 0;
					}
					if(DownValue && Stepper.mDelta == 0){
						Motor_Speed(&Stepper, KHZ(1));
						Motor_Move(&Stepper, Steps);
					}
                    break;
				case 4:
					if(lockedMode){
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