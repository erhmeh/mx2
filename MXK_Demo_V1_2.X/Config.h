/* 
 * File:   Config.h
 * Author: David Ledger
 *
 * Created on 28 February 2017, 3:21 PM
 */
#ifndef CONFIG_H
#define	CONFIG_H
#define VERSION     "1.0"
#define COMPILEDATE __DATE__

//#define     ASMMODE     //Allows you to develop code in the ASM file
//#define     PRODUCTION
//#define     ISR_NOBLOCK
//#define     ISR_USB0
//#define     ISR_SPI
//#define     ISR_I2C

////////////////////////////////////////////////////////////////////////////////
//To enable one module, uncomment it below
#ifndef     ASMMODE
    //#define     BUTTONTEST
    #define     LCDTEST
    #define     SEVENSEGTEST
    #define     MOTORTEST
    //#define     USBTEST
    //#define     ADCTEST
    //#define     EUSARTTEST      //UNCOMMENT AFTER IMPLEMENTATION IS MADE

    #ifdef EUSARTTEST
        #define EUSARTISR
    #endif

    #ifdef ADCTEST
        #define ADCISR  
    #endif

    #ifdef MOTORTEST
//        #define   MOTORCONTINIOUS
    #else
//      #define   TETRIS      //INCOMPLETE
//      #define   IMUTEST     //NOT YET AVALIABLE
    #endif
#else
    #define     LCDTEST
    #ifdef      TETRIS
        #undef      TETRIS
    #endif
#endif

////////////////////////////////////////////////////////////////////////////////
//To active continuous motor mode uncomment the below
#define     CONSOLE_BUFFERED
//#define   HMI_RENDER_DELAYED
#define     DEBOUNCE            2

////////////////////////////////////////////////////////////////////////////////
#ifdef USBTEST
    #ifdef TETRIS
        #define KEY_UP      'W'
        #define KEY_DOWN    'S'
        #define KEY_LEFT    'A'
        #define KEY_RIGHT   'D'
        #define KEY_ROTATE  '\r'
        #define USB_ECHO
    #else
        #define USB_ECHO
//        #define USB_CONSOLE
    #endif
#endif
////////////////////////////////////////////////////////////////////////////////
#ifdef HMI_RENDER_DELAYED
    #define     HMI_RENDER_DELAY    100 //us
#endif
////////////////////////////////////////////////////////////////////////////////
#ifdef ISR_SPI
    #define SPI_FILLBUFFER
    #define SPI_BUFFERSIZE 64 //    64/8 = 8

    #if SPI_BUFFERSIZE >= 64
        #define HMI_LOOPING_FIFO
    #endif
#endif
////////////////////////////////////////////////////////////////////////////////
//The address of the module
#define	MXK_MOTOR   0b1000
#define MXK_IMU     0b0100
#define MXK_FPGA    0b0010
#define MXK_HMI     0b0001

////////////////////////////////////////////////////////////////////////////////
//      DO NOT TOUCH THIS CODE
#if (defined(SEVENSEGTEST) || defined(LCDTEST))
    #define HMITEST
#endif
#if (defined(BUTTONTEST) && defined(HMITEST))
    #define HMIBUTTONTEST
#endif
#if (defined (LCDTEST))
    //Where 0 is the highest possible
    #define RENDER_PRIORITY 200
#endif
////////////////////////////////////////////////////////////////////////////////
#endif	/* CONFIG_H */

