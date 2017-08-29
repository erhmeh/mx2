#include "Functions.h"
#include <stdio.h>
#include "Types.h"
#include "Button.h"
#include "Motor.h"
#include "HMI.h"
#include "LCD.h"
#include "LED.h"
#include "Console.h"
#include "MXK.h"
#include "ISR.h"
#include "Config.h"
#include "Timer0.h"

void FunctInitButton() {
    //Configures the test button press event to call the 
    //Event_TestButton function when a change in state occurs
    Button_Init(&TestButton, DEBOUNCE, MXK_InitTestButton, MXK_TestButton, Event_TestButton);

    //Configures the DIP buttons press event to call the
    //Event_DIPSwitches function when a value change occurs
    Button_Init(&DIPSwitch, DEBOUNCE, MXK_InitDIPSwitch, MXK_DIPSwitch, Event_DIPSwitch);
}

////////////////////////////////////////////////////////////////////////////////

void FunctButton() {
    MXK_Poll();
}