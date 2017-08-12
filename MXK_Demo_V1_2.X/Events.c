#include "Events.h"
#include "Button.h"
#include "LED.h"
#include "HMI.h"
#include <stdio.h>
#include "USBWrap.h"
#include "Config.h"
#include "ADC.h"
////////////////////////////////////////////////////////////////////////////////
void Event_ADCSequenceComplete(UINT8 pLength)
{
    //This indicates that a conversion sequence has completed. You can safely
    // access the data stored in the sequence structures
    
    LED_Toggle(eLED2);
}
////////////////////////////////////////////////////////////////////////////////
void Event_USBRecieve(UINT8 pLength)
{   
#if defined(USBTEST) && defined(TETRIS)
    UINT8 index;
    for (index = 0; index < pLength; index++)
    {
	switch (ToUpper(USBInputBuffer[index]))
	{
	    case KEY_UP:
		TetrisMoveU(&gTestBlock);
		break;
	    case KEY_DOWN:
		TetrisMoveD(&gTestBlock);
		break;
	    case KEY_LEFT:
		TetrisMoveL(&gTestBlock);
		break;
	    case KEY_RIGHT:
		TetrisMoveR(&gTestBlock);
		break;
	    case KEY_ROTATE:
		TetrisBlockRotate(&gTestBlock);
		break;
	};
    }
#else
    #ifdef USB_CONSOLE
	#ifdef LCDTEST
	USBInputBuffer[pLength] = '\0';
	
	//Prints a random number between 0 and 6
	if (USBInputBuffer[0]=='~')
	    printf("%u ", rand_between(0,4));
	else
	    printf("%s", (str)USBInputBuffer);
	#endif
    #endif
#endif
}
////////////////////////////////////////////////////////////////////////////////
void Event_DIPSwitch(UINT8 pState)
{
    UINT8 Bar = HMI_GetBar();
    Bar &= 0x0F;
    pState <<= 4;

    //Sets the bargraph to the bits defined by Bar or'd with state
    HMI_SetBar(pState | Bar);
    
#ifdef USBTEST
    USB_Print("DIP\n");
#endif
    
    //Toggle the action LED (LED1)
    LED_Toggle(eLED1);
}
////////////////////////////////////////////////////////////////////////////////
void Event_HMI_DIPSwitch(UINT8 pState)
{
    UINT8   Bar = HMI_GetBar();
    HMI_SetBar(pState | Bar);
    
    Bar &= 0xF0;
    pState &= 0x0F;
    
#ifdef USBTEST
    USB_Print("HMI DIP\n");
#endif

    LED_Toggle(eLED1);
}
////////////////////////////////////////////////////////////////////////////////
UINT8 i, j, u;
static UINT16 val;
void Event_TestButton(UINT8 pState)
{
    ////////////////////////////////////////////////////////////////////////////
    //Code in this function can be freely changed.
    static UINT8 j = 0;
    
    ////////////////////////////////////////////////////////////////////////////
    //Increment on falling edge (button release)
    if (pState == BUTTON_DOWN)
    {
#ifdef TETRIS
	TetrisBlockRotate(&gTestBlock);
#endif
#ifdef USBTEST
	USB_Print("Test Button\n");
#endif
	j++;
	val += j;
	HMI_SetNumber( val );
	
	//Set the LED on the HMI near the button corresponding to J
	HMI_SetButton( j & 3 );
    }

    ////////////////////////////////////////////////////////////////////////////
    //Toggle on rising and falling
    LED_Toggle(eLED1);
}
////////////////////////////////////////////////////////////////////////////////
UINT16 pShiftVal = 0;
void Event_HMI_LeftButton(UINT8 pState)
{
    //Button is down not up
    HMI_SetLeft(pState);
    
    if (pState == BUTTON_DOWN)
    {
	pShiftVal <<= 1;
	if (pShiftVal == 0xFF00)
	    pShiftVal = 0;
	else if (pShiftVal < 0xFF)
	    pShiftVal |= 1;
	////////////////////////////////////////////////////////////////////////////
#ifdef TETRIS
	TetrisMoveL(&gTestBlock);
#endif
#ifdef USBTEST
	USB_Print("Left\n");
#endif
	////////////////////////////////////////////////////////////////////////////
	HMI_SetBar(pShiftVal & 0xFF);
    }

    //Toggle the action LED (LED1)
    LED_Toggle(eLED1);
}
////////////////////////////////////////////////////////////////////////////////
void Event_HMI_RightButton(UINT8 pState)
{
    //Button is down not up
    HMI_SetRight(pState);
    
    if (pState == BUTTON_DOWN)
    {
	if (pShiftVal == 0)
	    pShiftVal = 0xFF00;
	pShiftVal >>= 1;
	////////////////////////////////////////////////////////////////////////////
#ifdef TETRIS
	TetrisMoveR(&gTestBlock);
#endif
#ifdef USBTEST
	USB_Print("Right\n");
#endif
	////////////////////////////////////////////////////////////////////////////
	HMI_SetBar(pShiftVal & 0xFF);
    }
    //Toggle the action LED (LED1)
    LED_Toggle(eLED1);
}
////////////////////////////////////////////////////////////////////////////////
void Event_HMI_UpButton(UINT8 pState)
{
    //Anything between these braces can be changed to user code
    HMI_SetUp(pState);
    
    if (pState == BUTTON_DOWN)
    {
	val += 1;
	HMI_SetNumber(val);
	////////////////////////////////////////////////////////////////////////
#ifdef TETRIS
	TetrisMoveU(&gTestBlock); 
#endif
#ifdef USBTEST
	USB_Print("Up\n");
#endif
    }    
    ////////////////////////////////////////////////////////////////////////////
    //Toggle the action LED (LED1)
    LED_Toggle(eLED1);
}
////////////////////////////////////////////////////////////////////////////////
void Event_HMI_DownButton(UINT8 pState)
{
    //Anything between these braces can be changed to user code
    HMI_SetDown(pState);  
    
    if (pState == BUTTON_DOWN)
    {
	val -= 1;
	HMI_SetNumber(val);
	////////////////////////////////////////////////////////////////////////////
#ifdef TETRIS
	TetrisMoveD(&gTestBlock);
#endif
#ifdef USBTEST
	USB_Print("Down\n");
#endif
    }
    ////////////////////////////////////////////////////////////////////////////
    //Toggle the action LED (LED1)
    LED_Toggle(eLED1);
}