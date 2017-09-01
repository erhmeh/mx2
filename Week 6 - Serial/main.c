/*
 * File: main.c
 * Author: Chadwick Aryana *
 * MX2: Week 6 iRobot Basic
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
#include <pic18f67j50.h>
#include "Motor.h"
#include "LCD.h"
#include "LED.h"
#include "HMI.h"

void eusart_putch(uint8_t uartSendChar) { //EUSART data transmit function
    while (!TXSTA1bits.TRMT) {
    }; //Wait for transmission to finish
    TXREG1 = uartSendChar; //Send character (EUSART transmit register)
}

char eusart_getch(void) { //EUSART data receive function
    uint8_t uartReceiveChar = RCREG1; //EUSART receive register
    while (!PIR1bits.RC1IF) {
    }; //Wait for character input
    return uartReceiveChar; //Return contents of EUSART receive register
}

void main() {
    //Init MXK Pins
    MXK_Init();
    //Init HMI
    if
        (MXK_BlockSwitchTo
            (eMXK_HMI)) {
        Console_Init();
        HMI_Init();
        LCD_Init();
        if
            (MXK_Release())
            MXK_Dequeue();

    }

    //Setup pins on MXK
    TRISCbits.TRISC6 = 0; //TX Pin (Output)
    TRISCbits.TRISC7 = 1; //RX Pin (Input)

    //Setup Registers
    TXSTA1 = 0b00100000; //Setup TXSTA1 Register
    /* TXSTA1 Register Configuration
     * CSRC = 0
     * TX9 = 0 8-bit tranmission
     * TXEN = 1 Enable transmission
     * SYNC = 0 Asynchronous mode
     * SENDB = 0 Disable sync break?
     * BRGH = 0
     * TMRT = 0
     * TX9D = X
     */

    RCSTA1 = 0b10010000; //Setup TXSTA1 Register
    /* RCSTA1 Register Configuration
     * SPEN = 1 Enable serial port
     * RX9 = 0 8-bit receiving
     * SREN = X
     * CREN = 1 Enable continuous receiving
     * ADDEN = X
     * FERR = 0 Disable framing errors
     * OERR = 0 Disable overrun errors
     * RX9D = X
     */

    BAUDCON1bits.DTRXP = 0; //Disable inversion
    BAUDCON1bits.SCKP = 0; //High idle state
    BAUDCON1bits.WUE = 0; //Disable monitoring on the RX pin
    BAUDCON1bits.ABDEN = 0; //Disable automatic baud detection

    //Setup baud rate
    TXSTA1bits.BRGH = 1;
    BAUDCON1bits.BRG16 = 0;
    //SPBRGH1:SPBRG1 calculated as 13 when; Set BRGH, Clear BRG16
    SPBRG1 = 13; //0.16% Error

    ISR_Enable(); //Enable Interrupts

    //iRobot Init
    eusart_putch(128); //Setup iRobot
    eusart_putch(132); //Activate "Full Mode"

    loop() {
        eusart_putch(); //Send Data
        eusart_getch(); //Receive Data
        //HMI code
        if (MXK_BlockSwitchTo(eMXK_HMI)) {
            //Put shit from eusart_getch() i.e. robot readouts here and other relevant crap
            if (MXK_Release())
                MXK_Dequeue();
        }
    }
}

