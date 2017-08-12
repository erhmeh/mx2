/*******************************************************************************
File:			"FIFO.h"

Author:			David Ledger

Brief:			The FIFO structure is used to store the incoming bytes in a
                circular array.
				
Copyright (c) 2016 David Ledger. All rights reserved.
*******************************************************************************/

#ifndef FIFO_H
#define FIFO_H

#include "Types.h"


typedef struct FIFO FIFO;
typedef struct FIFO * const FIFOPtr;

//------------------------------------------------------------------------------
// FIFO structure
struct FIFO
{
    UINT16      mLength;
    UINT16      mItems;
    UINT16      mStart;
    UINT16      mEnd;
    
    //When this is positive looping loaded FIFO is initiated
    INT16       mIndex;

    union
    {
        volatile UINT8Ptr    mBuffer;
        volatile UINT16Ptr   mBuffer16;
    };
};


//------------------------------------------------------------------------------
// FIFO_Init
// 
// Initialize the FIFO
// Input:
//   FIFO is a pointer to a FIFO struct to initialize
// Output:
//   none
// Conditions:
//  Memory is avaliable and FIFO isn't nullptr, FIFO size>0
void FIFO_Init(FIFOPtr pInput, CUINT8Ptr pBuffer, CUINT16 pLength);

//------------------------------------------------------------------------------
// FIFO_Register
// Registers X items and initiates looping FIIO mode
// Input:
//   FIFO is a  pointer to a FIFO struct where data is to be stored
//   Number of items to register
// Output:
//   TRUE if data is properly saved
// Conditions:
//   Can be called instead of FIFO_Init
bool FIFO_Register(FIFOPtr pInput, CUINT8Ptr pBuffer, UINT16 pItems);

//------------------------------------------------------------------------------
// FIFO_Put
// 
// Enter one character into the FIFO
// Input:
//   FIFO is a  pointer to a FIFO struct where data is to be stored
//   data is a byte of data to store in the FIFO buffer
// Output:
//   TRUE if data is properly saved
// Conditions:
//   Assumes that FIFO_Init has been called
bool FIFO_Put(FIFOPtr pInput, CUINT8 pData); 
bool FIFO_Put16(FIFOPtr pInput, CUINT16 pData); 
//------------------------------------------------------------------------------
// FIFO_Get
// 
// Remove one character from the FIFO
// Input:
//   FIFO is a  pointer to a FIFO struct with data to be retrieved
//   dataPtr is a pointer to a memory location to place the retrieved byte
// Output:
//   TRUE if the operation was successful and the data is valid
// Conditions:
//   Assumes that FIFO_Init has been called
bool    FIFO_Get(FIFOPtr pInput, CUINT8Ptr pOutput);
bool    FIFO_Get16(FIFOPtr pInput, CUINT16Ptr pOutput);
//------------------------------------------------------------------------------
UINT8   FIFO_PRead(FIFOPtr pInput);
UINT8   FIFO_Read(FIFOPtr pInput);
UINT16  FIFO_Read16(FIFOPtr pInput);
//------------------------------------------------------------------------------
// FIFO_Items
// 
// Checks whether the FIFO contains any items
// Input:
//   FIFO is a  pointer to a FIFO struct with data to be retrieved
// Output:
//   TRUE if fifo is populated
// Conditions:
//   Assumes that FIFO_Empty has been called
inline bool FIFO_Items(FIFOPtr pInput);

//------------------------------------------------------------------------------
// FIFO_Empty
// 
// Checks whether the FIFO contains any items
// Input:
//   FIFO is a  pointer to a FIFO struct with data to be retrieved
// Output:
//   TRUE if fifo is empty
// Conditions:
//   Assumes that FIFO_Empty has been called
inline bool FIFO_Empty(FIFOPtr pInput);

//------------------------------------------------------------------------------
// FIFO_Full, FIFO_Half
// 
// Indicates whether FIFO is full or half full
// Input:
//   FIFO is a  pointer to a FIFO struct with data to be retrieved
// Output:
//   TRUE if fifo is full/half full
// Conditions:
//   Assumes that FIFO_Init has been called
inline bool FIFO_Full(FIFOPtr pInput);
inline bool FIFO_Half(FIFOPtr pInput);
#endif