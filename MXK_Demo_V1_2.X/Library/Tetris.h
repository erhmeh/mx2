/* 
 * File:   Tetris.h
 * Author: David Ledger
 *
 * Created on 6 March 2017, 4:58 PM
 */

#ifndef TETRIS_H
#define	TETRIS_H

#include "Types.h"
#include "Colours.h"

#define BLOCK_W         6
#define BORDER          1

#define BLOCKS_WIDE     10
#define BLOCKS_HIGH     20

#define OFFSET_X        1
#define OFFSET_Y        1

#define LBORDER BORDER
#define RBORDER BLOCK_W - BORDER

typedef struct  Block                   Block;
typedef struct  TetrisField             TetrisField;
typedef struct  TetrisBlock             TetrisBlock;

typedef struct  Block *                 BlockPtr;
typedef struct  TetrisField * const     TetrisFieldPtr;
typedef struct  TetrisBlock *           TetrisBlockPtr;

extern          TetrisField     gTetrisField;
extern          TetrisBlock     gTestBlock;

typedef enum
{
    eBlockStateInactive = 0,
    eBlockStateActive,
    eBlockStateDynamic,
    eBlockStateError
}   eBlockState;
typedef enum
{
    eBlockBody,
    eBlockBorder,
    eBlockOutside,
    eBlockInactive
}   eBlockCheck;
typedef enum
{
    eLN = 240,
    eSQ = 1632,
    eLL = 1570,
    eRL = 1604,
    eTE = 3648,
    eLZ = 1584,
    eRZ = 1728
}   eBlockType;

struct Block
{
    Colour          mBase;
    Colour          mBorder;
    
    UINT8           mX; //Position in pixels, not blocks
    UINT8           mY; //Position in pixels, not blocks
    
    eBlockState     mState;
    bool            mChanged;
    
    union
    {
        UINT16 mCount;
        struct
        {
            UINT8 mRow;
            UINT8 mColumn;
        };
    };
};
struct TetrisField
{
    bool        mBorderRedraw;
    Block       mBlocks[BLOCKS_WIDE][BLOCKS_HIGH];
};
struct TetrisBlock
{
    eBlockType  mType;
    UINT8       mX, mY;
    BlockPtr    mBlocks[4][4];
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void BlockInit         ( BlockPtr pInput, 
                         Colour pBase,   Colour pBorder, 
                         UINT8 pX,       UINT8 pY);

void BlockSet           (BlockPtr pInput, eBlockState pValue);
void BlockOn            (BlockPtr pInput);
void BlockOff           (BlockPtr pInput);
void BlockDraw          (BlockPtr pInput);
void BlockClear         (BlockPtr pInput);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void TetrisFieldInit    (TetrisFieldPtr pInput);
void TetrisFieldDraw    (TetrisFieldPtr pInput);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void TetrisBlockInit    (TetrisBlockPtr pInput, eBlockType pType);
void TetrisBlockDraw    (TetrisBlockPtr pInput);
void TetrisBlockRotate  (TetrisBlockPtr pInput);

inline bool TetrisBlockRowClear     (TetrisBlockPtr pInput, UINT8 pRow);
inline bool TetrisBlockColumnClear  (TetrisBlockPtr pInput, UINT8 pColumn);

bool TetrisMoveL    (TetrisBlockPtr pInput);
bool TetrisMoveR    (TetrisBlockPtr pInput);
bool TetrisMoveU    (TetrisBlockPtr pInput);
bool TetrisMoveD    (TetrisBlockPtr pInput);

bool TetrisBlockShiftL  (TetrisBlockPtr pInput);
bool TetrisBlockShiftR  (TetrisBlockPtr pInput);
bool TetrisBlockShiftU  (TetrisBlockPtr pInput);
bool TetrisBlockShiftD  (TetrisBlockPtr pInput);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif	/* TETRIS_H */

