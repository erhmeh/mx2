/*******************************************************************************
File:		"FIFO.c"

Author:		David Ledger

Brief:		The FIFO structure is used to store the bytes in a circular array.
				
Copyright (c) 2016 David Ledger. All rights reserved.
*******************************************************************************/

#include "FIFO.h"

//Macros to avoid confusing syntax below
inline UINT16 Start_2(FIFOPtr pInput)
{
    return (pInput->mStart >> 1);
}
inline UINT16 End_2(FIFOPtr pInput)
{
    return (pInput->mEnd >> 1);
}
inline bool LoopMode(FIFOPtr pInput)
{
    return (pInput->mIndex >= 0);
}
//------------------------------------------------------------------------------
// FIFO_Init
// Initialize the FIFO
// Input:
//   FIFO is a pointer to a FIFO struct to initialize
// Output:
//   none
// Conditions:
//  Memory is avaliable and FIFO isn't nullptr, FIFO size>0
void FIFO_Init(FIFOPtr pInput, CUINT8Ptr pBuffer, CUINT16 pLength)
{
    ////////////////////////////////////////////////////////////////////////////
    
    ifnull(pBuffer)
	return;
    ifzero (pLength)
	return;
    ifnull (pInput)
	return;
    
    ////////////////////////////////////////////////////////////////////////////
    
    pInput->mStart  = 0;
    pInput->mEnd    = 0;	//Declares there to be no data in buffer
    pInput->mItems  = 0;
    pInput->mIndex  = -1;
    pInput->mLength = pLength;	//This is the buffer length
    pInput->mBuffer = pBuffer;
    
    ////////////////////////////////////////////////////////////////////////////
}

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
bool FIFO_Register(FIFOPtr pInput, CUINT8Ptr pBuffer, UINT16 pItems)
{
    ////////////////////////////////////////////////////////////////////////////
    ifnull(pBuffer)
	return false;
    ifzero (pItems)
	return false;
    ifnull (pInput)
	return false;
    
    ////////////////////////////////////////////////////////////////////////////
    pInput->mStart  = 0;
    pInput->mEnd    = pItems;	//Declares there to be no data in buffer
    pInput->mItems  = pItems;
    pInput->mIndex  = 0;	// > 0 means looping fifo mode active
    pInput->mLength = pItems;	//This is the buffer length
    pInput->mBuffer = pBuffer;
    ////////////////////////////////////////////////////////////////////////////
    return true;
}

//------------------------------------------------------------------------------
// FIFO_Put
// Enter one character into the FIFO
// Input:
//   FIFO is a  pointer to a FIFO struct where data is to be stored
//   data is a byte of data to store in the FIFO buffer
// Output:
//   TRUE if data is properly saved
// Conditions:
//   Assumes that FIFO_Init has been called
bool FIFO_Put(FIFOPtr pInput, CUINT8 pData)
{
    if (FIFO_Full(pInput))
	return false;

    //The index of the array is looped around to zero, the modulo ensures this.
    pInput->mBuffer[pInput->mEnd] = pData;
    ++ pInput->mEnd;
    ++ pInput->mItems;
    
    //Number of bytes in FIFO increase when a byte is added 	 	
    if (pInput->mEnd >= pInput->mLength)
      pInput->mEnd = 0;
    
    return true;
}
bool FIFO_Put16(FIFOPtr pInput, CUINT16 pData)
{
    if (FIFO_Full(pInput))
      return false;

    //The index of the array is looped around to zero, the modulo ensures this.
    pInput->mBuffer16[ End_2(pInput) ] = pData;
    pInput->mEnd    +=	2;
    pInput->mItems  +=	2;
    
    //Number of bytes in FIFO increase when a byte is added 	 	
    if (pInput->mEnd >= pInput->mLength)
      pInput->mEnd = 0;
    
    return true;
}

//------------------------------------------------------------------------------
// FIFO_Get
// Remove one character from the FIFO
// Input:
//   FIFO is a  pointer to a FIFO struct with data to be retrieved
//   dataPtr is a pointer to a memory location to place the retrieved byte
// Output:
//   TRUE if the operation was successful and the data is valid
// Conditions:
//   Assumes that FIFO_Init has been called
bool FIFO_Get(FIFOPtr pInput, CUINT8Ptr pOutput)
{
    if (FIFO_Empty(pInput))
	return false;
    
    //Sets the data at the address of dataPtr to the buffer start item
    *pOutput = pInput->mBuffer[pInput->mStart];
    ++pInput->mStart;
    --pInput->mItems;
    
    //One item was requested, one item is removed
    if (pInput->mStart >= pInput->mLength)
	pInput->mStart = 0;

    return true;
}
bool FIFO_Get16(FIFOPtr pInput, CUINT16Ptr pOutput)
{
    if (FIFO_Empty(pInput))
	return false;
    
    //Sets the data at the address of dataPtr to the buffer start item
    *pOutput = pInput->mBuffer16[Start_2(pInput)];
    pInput->mStart  +=	2;
    pInput->mItems  -=	2;
    
    //One item was requested, one item is removed
    if (pInput->mStart >= pInput->mLength)
	pInput->mStart = 0;

    return true;
}

UINT8 FIFO_PRead(FIFOPtr pInput)
{
    INT16Ptr i = &(pInput->mIndex);
    UINT8 out = pInput->mBuffer[*i];
    
    //Increment index
    ++(*i);
    if ((*i) >= pInput->mEnd)
	(*i) = pInput->mStart;
    
    //Return the value
    return out;
}

//Same as above, assumes not empty and returns directly
UINT8 FIFO_Read(FIFOPtr pInput)
{
    //In loop mode it is always a passive read
    if (LoopMode(pInput))
	return FIFO_PRead(pInput);
    
    //Otherwise proceed normally
    ++pInput->mStart;
    --pInput->mItems;
    
    //One item was requested, one item is removed
    if (pInput->mStart >= pInput->mLength)
	pInput->mStart = 0;
    
    return (pInput->mBuffer[pInput->mStart]);
}
//Same as above, assumes not empty and returns directly
UINT16 FIFO_Read16(FIFOPtr pInput)
{
    UINT16 i = Start_2(pInput);

    pInput->mStart  +=	2;
    pInput->mItems  -=	2;
    
    //One item was requested, one item is removed
    if (pInput->mStart >= pInput->mLength)
	pInput->mStart = 0;
    
    return (pInput->mBuffer16[i]);
}

//------------------------------------------------------------------------------
// FIFO_Items
// 
// Checks whether the FIFO contains any items
// Input:
//   FIFO is a  pointer to a FIFO struct with data to be retrieved
// Output:
//   TRUE if FIFO is populated
// Conditions:
//   Assumes that FIFO_Empty has been called
inline bool FIFO_Items(FIFOPtr pInput)
{
    return (pInput->mItems > 0);
}

//------------------------------------------------------------------------------
// FIFO_Empty
// 
// Indicates whether FIFO is empty
// Input:
//   FIFO is a  pointer to a FIFO struct with data to be retrieved
// Output:
//   TRUE if FIFO is empty
// Conditions:
//   Assumes that FIFO_Init has been called
inline bool FIFO_Empty(FIFOPtr pInput)
{
    return (pInput->mItems == 0);
}

//------------------------------------------------------------------------------
// FIFO_Full
// 
// Indicates whether FIFO is full
// Input:
//   FIFO is a  pointer to a FIFO struct with data to be retrieved
// Output:
//   TRUE if FIFO is empty
// Conditions:
//   Assumes that FIFO_Init has been called
inline bool FIFO_Full(FIFOPtr pInput)
{
    return (pInput->mItems >= pInput->mLength);
}
inline bool FIFO_Half(FIFOPtr pInput)
{
    return (pInput->mItems >= (pInput->mLength>>1));
}


