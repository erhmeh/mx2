#include "Functions.h"
#include <stdio.h>
#include "Types.h"
#include "Button.h"
#include "Motor.h"
#include "IMU.h"
#include "HMI.h"
#include "LCD.h"
#include "Console.h"
#include "MXK.h"
#include "ISR.h"
#include "ADC.h"
#include "Config.h"
#include "USBWrap.h"
#include "Timer0.h"
#include "EUSART.h"

#ifdef ASMMODE
volatile UINT8	LCD_OUT;
extern void	MYASM();
extern void	OUTCHAR(char);
#endif

Motor	Stepper;

//Temporary variable used for frequency calculations
CUINT16 HzInc = 10;
UINT16  Hz = 10;
INT16   DIST = 100;
UINT16  HzOld;
UINT8	k = 0;

void FunctInitButton()
{
    //Configures the test button press event to call the 
    //Event_TestButton function when a change in state occurs
    Button_Init(&TestButton, DEBOUNCE, MXK_InitTestButton, MXK_TestButton, Event_TestButton);
    
    //Configures the DIP buttons press event to call the
    //Event_DIPSwitches function when a value change occurs
    Button_Init(&DIPSwitch, DEBOUNCE, MXK_InitDIPSwitch, MXK_DIPSwitch, Event_DIPSwitch);
}
void FunctInitIMU()
{
    if (MXK_BlockSwitchTo(eMXK_IMU))
    {
	IMU_Init();
	if(MXK_Release())
	    MXK_Dequeue();
    }
}
void FunctInitMotor()
{
    //Motor module internally manages bus contention
    
    //Initializes the motor to move by 100 units at the default speed (1KHz)
    Motor_Init(&Stepper, MXK_MOTOR);
    Motor_Move(&Stepper, DIST);

    //This will make the motor move continously instead of in discrete numbers
    // of steps
    #ifdef MOTORCONTINIOUS
    Motor_Continious(&Stepper, 1);
    #endif
}

ADC ADC_AN0;
ADC ADC_AN1;
ADC ADC_AN2;
ADC ADC_AN3;

void FunctInitADC()
{
    //Initialize channels
    ADC_Init(&ADC_AN0, eADC_Ch0);
    ADC_Init(&ADC_AN1, eADC_Ch1);
    ADC_Init(&ADC_AN2, eADC_Ch2);
    ADC_Init(&ADC_AN3, eADC_Ch3);
    
    //Initialize sequence
    ADC_Sequence(&ADC_AN0, &ADC_AN1);
    ADC_Sequence(&ADC_AN1, &ADC_AN2);
    ADC_Sequence(&ADC_AN2, &ADC_AN3);
}

void FunctInitLCD()
{
    if (MXK_BlockSwitchTo(eMXK_HMI))
    {
	////////////////////////////////////////////////////////////////////////
#ifdef ISR_SPI
	ISR_Enable();
#endif
	//Sends commands to the LCD to turn it on correctly and configure its memory
	// to work with 16 bit colors
	LCD_Init();
	
	//Wait until LCD isn't busy
#ifdef ISR_SPI
	while (LCD_Busy());
#endif
	
#ifdef ISR_SPI
	ISR_Disable();
#endif
	////////////////////////////////////////////////////////////////////////
	
	//Sets the terminal emulator to have a dark blue background with a white
	// foreground
#ifdef TETRIS
	TetrisFieldInit(&gTetrisField);
	
	/*
    eLN = 240,
    eSQ = 1632,
    eLL = 1570,
    eRL = 1604,
    eTE = 3648,
    eLZ = 1584,
    eRZ = 1728
	 */
	eBlockType remap[7] = {eLN, eSQ, eLL, eRL, eTE, eLZ, eRZ};
	TetrisBlockInit(&gTestBlock, remap[rand_between(0,6)]);
	TetrisBlockDraw(&gTestBlock);
#else
	Console_Init();
#endif

	//Release the HMI
	if(MXK_Release())
	    MXK_Dequeue();
    }
}
void FunctInitHMI()
{
    ////////////////////////////////////////////////////////////////////////
    if (MXK_BlockSwitchTo(eMXK_HMI))
    ////////////////////////////////////////////////////////////////////////
    {
	//Initializes the HMI, the SPI port within the HMI and sets the default state
	HMI_Init();
	
	//Sets the bar-graph to all on (This is overridden by the button routines)
	HMI_SetBar(0x00);

	//Sets the seven segment display to display David
    #ifdef TETRIS
	HMI_SetSegments((str)"tEtrIS");
    #else
	HMI_SetSegments((str)"Printf");
    #endif

	//Release the MXK bus
	if(MXK_Release())
	    MXK_Dequeue();
    }
    ////////////////////////////////////////////////////////////////////////
}

void FunctInitEUSART()
{
    EUSART_Init(9800, eEUSART_Master, eEUSART_Async, eEUSART_8Bit);
}
void FunctInitUSB()
{
    USB_Init();
}

#ifdef ASMMODE
void FunctInitASM()
{
    LCD_OUT = 0;
    
    Timer0_Init	    (HZ(500));
    Timer0_Start    (FunctProcessASM);
}
void FunctProcessASM()
{
    if (LCD_OUT)
    {
	printf("%c", LCD_OUT);
	LCD_OUT = 0;
	FunctHMI();
	
	LED_Toggle(STATUS_LED);
    }
    Timer0_Restart();
}

void FunctASM()
{
    MYASM();
}
#endif
////////////////////////////////////////////////////////////////////////////////
void FunctEUSART()
{
    EUSART_Poll();
}
void FunctUSB()
{
    USB_Poll();
}
void FunctButton()
{
    MXK_Poll();
}
void FunctIMU()
{
//    //Switch to the IMU Module address
//    if (MXK_BlockSwitchTo(eMXK_IMU))
//    IMU_WriteReg(0x2F, strs, 3, MXK_Release);
//
//    if (MXK_BlockSwitchTo(eMXK_IMU))
//    IMU_ReadReg(0x2F, bufs, 3, MXK_Release);
}
void FunctMotor()
{
    if (MXK_BlockSwitchTo(eMXK_Motor))
    {
    #ifdef MOTORCONTINIOUS
	Hz = HMI_GetBar() * 500;
	if (Hz != HzOld)
	    Motor_Speed(&Stepper, Hz);
	HzOld = Hz;
    #else
	if (Stepper.mDelta == 0)
	{
	    Hz += HzInc;
	    if (Hz >= DIST)
		Hz = HzInc;

	    LED_Toggle(STATUS_LED);
	    DIST = -DIST;

	    Motor_Speed(&Stepper, Hz);
	    Motor_Move(&Stepper, DIST);
	}
    #endif
	if(MXK_Release())
	    MXK_Dequeue();
    }
}
void FunctADC()
{
    ADC_Start(&ADC_AN0);
}

void FunctHMI()
{
    if (MXK_BlockSwitchTo(eMXK_HMI))
    {
#ifdef HMIBUTTONTEST
	HMI_Poll();
#endif
#ifdef SEVENSEGTEST
	HMI_Render();
#endif
#ifdef LCDTEST
#ifdef TETRIS
	TetrisFieldDraw(&gTetrisField);
#else
	////////////////////////////////////////////////////////////////////
	//Render a table of values on the LCD with tab separated columns
#ifndef	ASMMODE
    #ifndef USB_CONSOLE
	static UINT8 v = RENDER_PRIORITY;
	if (!v--)
	{
	    v = RENDER_PRIORITY;
	    ////////////////////////////////////////////////////////////////
	    //Update the LCD screen with printf 's
	    Console_SetForecolour(RED);
	    printf("%c", ENDOFTEXT);
	    printf("SYSTEM STATUS:\n");
	    printf(COMPILEDATE);
	    printf(" ");
	    printf(VERSION);
	    printf("\n");

	#ifdef BUTTONTEST
	    Console_SetForecolour(LIGHTBLUE);
	    printf("MICRO TST\t=%u\n",	TestButton.mGetState());
	    printf("MICRO DIP\t=%u\n",	DIPSwitch.mGetState());

	    Console_SetForecolour(CYAN);
	    printf("HMI DIP\t\t=%u\n",	HMIBoard.mDIP.mGetState());
	    printf("HMI UP\t\t=%u\n",	HMIBoard.mUp.mGetState());
	    printf("HMI DOWN\t=%u\n",	HMIBoard.mDown.mGetState());
	    printf("HMI LEFT\t=%u\n",	HMIBoard.mLeft.mGetState());
	    printf("HMI RIGHT\t=%u\n",	HMIBoard.mRight.mGetState());
	#endif
	#ifdef MOTORTEST
	    //Displays the step rate and the RPM of the motor assuming 1/16
	    // micro-stepping
	    Console_SetForecolour(YELLOW);
	    printf("Step rate\t=%u Hz\n", Hz);
	#endif

	#ifdef ADCTEST
	    Console_SetForecolour(GREEN);
	    printf("ADC ADC_AN0\t=%.3f\n", ADC_Voltage(&ADC_AN0));
	    printf("ADC ADC_AN1\t=%.3f\n", ADC_Voltage(&ADC_AN1));
	    printf("ADC ADC_AN2\t=%.3f\n", ADC_Voltage(&ADC_AN2));
	    printf("ADC ADC_AN3\t=%.3f\n", ADC_Voltage(&ADC_AN3));
	    printf("Random\t\t=%u\n", rand_between(0, 100));
	#endif 
	    Console_Render();
	}
    #else
    Console_Render();
    #endif
#else
    Console_Render();
#endif	
#endif
#endif
	if(MXK_Release())
	    MXK_Dequeue();
    }
}

