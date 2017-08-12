#include "Tetris.h"
#include "LCD.h"

TetrisField gTetrisField;
TetrisBlock gTestBlock;

const UINT8 BLOCKS_COUNT = (BLOCKS_WIDE*BLOCKS_HIGH);
const UINT16 LNVALS[] = {240,   17476,  3840,   8738};
const UINT16 SQVALS[] = {1632,  1632,   1632,   1632};
const UINT16 LLVALS[] = {1570,  736,    17504,  1856};
const UINT16 RLVALS[] = {1604,  3616,   1094,   368};
const UINT16 TEVALS[] = {3648,  1124,   1248,   610};
const UINT16 LZVALS[] = {864,   1122,   864,    1122};
const UINT16 RZVALS[] = {1584,  306,    1584,   306};

void BlockInit(BlockPtr pInput, Colour pBase, Colour pBorder, UINT8 pX, UINT8 pY)
{
    pInput->mCount	= 0;
    pInput->mBase	= pBase;
    pInput->mBorder	= pBorder;
    pInput->mState	= eBlockStateInactive;
    pInput->mChanged	= true;
    
    //Multiply location by 8
    pX			*= BLOCK_W;
    pY			*= BLOCK_W;
    pInput->mX		= pX + OFFSET_X;
    pInput->mY		= pY + OFFSET_Y;
}
void BlockSet(BlockPtr pInput, eBlockState pValue)
{
    pInput->mState	= pValue;
    pInput->mChanged	= true;
}
void BlockDraw(BlockPtr pInput)
{
    if (pInput->mChanged)
    {
	pInput->mChanged = false;
	if (pInput->mState != eBlockStateInactive)
	{
	    //Set window to the 8x8 at the above block
	    Set_Addr_Window(pInput->mX, pInput->mY, pInput->mX + BLOCK_W - 1, pInput->mY + BLOCK_W - 1);

	    pInput->mCount = 0;
	    
	    
	    UINT8 x, y;
	    for (x=0; x < BLOCK_W; x++)
		for (y=0; y < BLOCK_W; y++)
		{
		    ////////////////////////////////////////////////////////////////
		    if (    (LBORDER <= y) &&    
			    (y < RBORDER) && 
			    (LBORDER <= x) && 
			    (x  < RBORDER))
			//Middle Region
			LCD_Word_Write(pInput->mBase);
		    else
			//Border Region
			LCD_Word_Write(pInput->mBorder);

		    ////////////////////////////////////////////////////////////////
		};
	}
	else
	    BlockClear(pInput);
    }
}
eBlockCheck BlockInside(BlockPtr pInput, UINT8 pX, UINT8 pY)
{
    ifzero (pInput->mState)
	return eBlockInactive;
    
    if (pX < pInput->mX || pY < pInput->mY )
	return eBlockOutside;
    
    if (pX > pInput->mX + BLOCK_W || pY > pInput->mY + BLOCK_W)
	return eBlockOutside;
    
    pInput->mRow = pX - pInput->mX;
    pInput->mColumn = pY - pInput->mY;
    
    if (    (LBORDER <=	pInput->mRow && pInput->mRow	    < RBORDER) && 
	    (LBORDER <=	pInput->mColumn && pInput->mColumn  < RBORDER)
	    )
	//Middle Region
	return eBlockBody;
    
    //Border Region
    return eBlockBorder;
}
void BlockClear(BlockPtr pInput)
{
    //Set window to the 8x8 at the above block
    Set_Addr_Window(pInput->mX, pInput->mY, pInput->mX + BLOCK_W - 1, pInput->mY + BLOCK_W - 1);
    pInput->mCount = 0;
    
    UINT8 x, y;
    for (x=0; x < BLOCKS_WIDE; x++)
        for (y=0; y < BLOCKS_HIGH; y++)
	    LCD_Word_Write(BLACK);
}
void TetrisFieldInit    (TetrisFieldPtr pInput)
{
    UINT8 x, y;
    
    pInput->mBorderRedraw = true;
    
    for (x=0; x < BLOCKS_WIDE; x++)
        for (y=0; y < BLOCKS_HIGH; y++)
	    BlockInit(&(pInput->mBlocks[x][y]), RED, BLUE, x + 1, y);
}
void TetrisFieldDraw(TetrisFieldPtr pInput)
{
    UINT8 x, y;
    static Point    lCorner;
    static Size	    lSize;
    
    if (pInput->mBorderRedraw)
    {
	//Draws the field border
	lCorner.x = 0;
	lCorner.y = 0;
	
	lSize.x = BLOCK_W;
	lSize.y = BLOCK_W * 21;
	
	LCD_FillRectangle(WHITE, lCorner, lSize);
	lCorner.x = BLOCK_W;
	lCorner.y = BLOCK_W * 20;
	
	lSize.x = BLOCK_W * 11;
	lSize.y = BLOCK_W;
	
	LCD_FillRectangle(WHITE, lCorner, lSize);
	
	lCorner.x = BLOCK_W * 11;
	lCorner.y = 0;
	
	lSize.x = BLOCK_W;
	lSize.y = BLOCK_W * 20;
	
	LCD_FillRectangle(WHITE, lCorner, lSize);
	
	pInput->mBorderRedraw = false;
    }

    for (x=0; x < BLOCKS_WIDE; x++)
        for (y=0; y < BLOCKS_HIGH; y++)
	    BlockDraw(&(pInput->mBlocks[x][y]));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline void TetrisBlockFrame(TetrisBlockPtr pInput)
{
    BlockPtr block;
    UINT8 x, y, pX, pY;
  
    
    //Faster to not de-reference every time
    pX = pInput->mX;
    pY = pInput->mY;
    
    for (x = 0; x < 4; x++)
        for (y = 0; y < 4; y++)
	{
	    block = &(gTetrisField.mBlocks[ x + pX ][ y + pY ]);
            pInput->mBlocks[ x ][ y ] = block;
	}
}

void TetrisBlockInit    (TetrisBlockPtr pInput, eBlockType pType)
{
    ifnull (pInput)
        return;
    
    //This configures all of the block data
    pInput->mType = pType;
    
    //The block top left corner
    //Default frame
    pInput->mX	= 4;
    pInput->mY	= 4;
    TetrisBlockFrame(pInput);
}
void TetrisBlockDraw    (TetrisBlockPtr pInput)
{
    volatile bool    active;
    UINT8   x, y;
    UINT16  v = pInput->mType;
    
    for ( x = 0; x < 4; x ++ )
        for ( y = 0; y < 4; y ++ )
        {
	    active = v & 1;
	    
	    if (active)
		BlockSet(pInput->mBlocks[x][y], eBlockStateDynamic);
	    
            v >>= 1;
        }
}

inline 
bool TetrisBlockSwap	(BlockPtr pItem1, BlockPtr pItem2)
{
    if ( !( pItem1->mState == eBlockStateDynamic || 
	    pItem1->mState == eBlockStateInactive) )
	return true;
    if ( !( pItem2->mState == eBlockStateDynamic || 
	    pItem2->mState == eBlockStateInactive) )
	return true;
    
    eBlockState pBuffer = pItem1->mState;
    BlockSet(pItem1, pItem2->mState);
    BlockSet(pItem2, pBuffer);
    
    return false;
}
#define o(x)	(3-x)
#define LT(x,y)	[x][y]
#define RT(x,y)	LT(o(y), x)
#define RB(x,y)	RT(o(y), x)
#define LB(x,y)	RB(o(y), x)
inline bool TetrisBlockClockwise(TetrisBlockPtr pInput, UINT8 pX, UINT8 pY)
{
    bool	undo = false;
    BlockPtr	BlockA, BlockB;

    //Rotate Corners
    BlockA = (*pInput).mBlocks	LT(pX, pY);
    BlockB = (*pInput).mBlocks	RT(pX, pY);
    undo |= TetrisBlockSwap(BlockA, BlockB);
    
    BlockA = (*pInput).mBlocks	LT(pX, pY);
    BlockB = (*pInput).mBlocks	RB(pX, pY);
    undo |= TetrisBlockSwap(BlockA, BlockB);
    
    BlockA = (*pInput).mBlocks	LT(pX, pY);
    BlockB = (*pInput).mBlocks	LB(pX, pY);
    undo |= TetrisBlockSwap(BlockA, BlockB);
    
    return undo;
}
inline bool TetrisBlockAnticlockwise(TetrisBlockPtr pInput, UINT8 pX, UINT8 pY)
{
    bool	undo = false;
    BlockPtr	BlockA, BlockB;

    //Rotate Corners
    BlockA = (*pInput).mBlocks	RT(pX, pY);
    BlockB = (*pInput).mBlocks	LT(pX, pY);
    undo |= TetrisBlockSwap(BlockA, BlockB);
    
    BlockA = (*pInput).mBlocks	RT(pX, pY);
    BlockB = (*pInput).mBlocks	LB(pX, pY);
    undo |= TetrisBlockSwap(BlockA, BlockB);
    
    BlockA = (*pInput).mBlocks	RT(pX, pY);
    BlockB = (*pInput).mBlocks	RB(pX, pY);
    undo |= TetrisBlockSwap(BlockA, BlockB);
    
    return undo;
}
#undef LT
#undef RT
#undef RB
#undef LB
#undef o

inline bool TetrisBlockRowOccupied(TetrisBlockPtr pInput, UINT8 pRow)
{
    return (	((*pInput).mBlocks[0][pRow])->mState 
	    ||	((*pInput).mBlocks[1][pRow])->mState 
	    ||	((*pInput).mBlocks[2][pRow])->mState 
	    ||	((*pInput).mBlocks[3][pRow])->mState);
}
inline bool TetrisBlockColumnOccupied(TetrisBlockPtr pInput, UINT8 pColumn)
{
    return (	((*pInput).mBlocks[pColumn][0])->mState 
	    ||	((*pInput).mBlocks[pColumn][1])->mState 
	    ||	((*pInput).mBlocks[pColumn][2])->mState 
	    ||	((*pInput).mBlocks[pColumn][3])->mState);
}

inline bool TetrisBlockRowClear(TetrisBlockPtr pInput, UINT8 pRow)
{
    return !(	((*pInput).mBlocks[0][pRow])->mState 
	    ||	((*pInput).mBlocks[1][pRow])->mState 
	    ||	((*pInput).mBlocks[2][pRow])->mState 
	    ||	((*pInput).mBlocks[3][pRow])->mState);
}
inline bool TetrisBlockColumnClear(TetrisBlockPtr pInput, UINT8 pColumn)
{
    return !(	((*pInput).mBlocks[pColumn][0])->mState 
	    ||	((*pInput).mBlocks[pColumn][1])->mState 
	    ||	((*pInput).mBlocks[pColumn][2])->mState 
	    ||	((*pInput).mBlocks[pColumn][3])->mState);
}

#define LEFT 0
#define RIGHT 3
#define BOTTOM 3
#define TOP 0

bool TetrisBlockShiftL(TetrisBlockPtr pInput)
{
    bool    undo = false;
    UINT8   col;
    UINT8   row;
    //While nothing is in the first column
    for (col = 1; col < 4; col++)
	for (row = 0; row < 4; row++)
	    undo |= TetrisBlockSwap( (*pInput).mBlocks[col][row],
					(*pInput).mBlocks[col-1][row]);
    
    return undo;
}
bool TetrisBlockShiftR(TetrisBlockPtr pInput)
{
    bool    undo = false;
    UINT8   col;
    UINT8   row;
    //While nothing is in the first column
    for (col = 0u; col < 3u; col++)
	for (row = 0u; row < 4u; row++)
	    undo |= TetrisBlockSwap( (*pInput).mBlocks[3u-col][row],
					(*pInput).mBlocks[2u-col][row]);
    return undo;
}
bool TetrisBlockShiftU(TetrisBlockPtr pInput)
{
    bool    undo = false;
    UINT8   col;
    UINT8   row;
    //While nothing is in the first column
    for (col = 0u; col < 4u; col++)
	for (row = 1u; row < 4u; row++)
	    undo |= TetrisBlockSwap( (*pInput).mBlocks[col][row],
					(*pInput).mBlocks[col][row-1u]);
    return undo;
}
bool TetrisBlockShiftD(TetrisBlockPtr pInput)
{
    bool    undo = false;
    UINT8   col;
    UINT8   row;
    //While nothing is in the last row
    for (col = 0u; col < 4u; col++)
	for (row = 0u; row < 3u; row++)
	    undo |= TetrisBlockSwap( (*pInput).mBlocks[col][2u-row],
					(*pInput).mBlocks[col][3u-row]);
    
    return undo;
}

inline void TetrisBlockReframe(TetrisBlockPtr pInput)
{
    while (TetrisBlockColumnClear(pInput, LEFT))
    {
	pInput->mX ++;
	TetrisBlockFrame(pInput);
    }
    ////////////////////////////////////////////////////////////////////////////
    while (TetrisBlockRowClear(pInput, BOTTOM))
    {
	pInput->mY --;
	TetrisBlockFrame(pInput);
    }
}

bool TetrisMoveL    (TetrisBlockPtr pInput)
{
    if (pInput->mX > 0u)
    {
	if(TetrisBlockColumnOccupied(pInput, LEFT))
	{
	    pInput->mX --;
	    TetrisBlockFrame(pInput);
	}
	TetrisBlockShiftL(pInput);
	TetrisBlockReframe(pInput);
    }
    return true;
}
bool TetrisMoveR    (TetrisBlockPtr pInput)
{
    if (pInput->mX < BLOCKS_WIDE)
    {
	if(TetrisBlockColumnOccupied(pInput, RIGHT))
	{
	    pInput->mX ++;
	    TetrisBlockFrame(pInput);
	}
        TetrisBlockShiftR(pInput);
	TetrisBlockReframe(pInput);
    }
    return true;
}
bool TetrisMoveU    (TetrisBlockPtr pInput)
{
    if (pInput->mY > 0u)
    {
	if(TetrisBlockRowOccupied(pInput, TOP))
	{
	    pInput->mY --;
	    TetrisBlockFrame(pInput);
	}
	TetrisBlockShiftU(pInput);
	TetrisBlockReframe(pInput);
    }
    return true;
}
bool TetrisMoveD    (TetrisBlockPtr pInput)
{
    if ( pInput->mY < BLOCKS_HIGH - 4u)
    {
	if(TetrisBlockRowOccupied(pInput, BOTTOM))
	{
	    pInput->mY ++;
	    TetrisBlockFrame(pInput);
	}
	TetrisBlockShiftD(pInput);
	TetrisBlockReframe(pInput);
    }
    return true;
}

void TetrisBlockRotate  (TetrisBlockPtr pInput)
{
    static bool undo = false;
    
    //Perform rotation clockwise 90 degrees
    undo |= TetrisBlockClockwise(pInput, 0, 0);
    undo |= TetrisBlockClockwise(pInput, 0, 1);
    undo |= TetrisBlockClockwise(pInput, 1, 0);
    undo |= TetrisBlockClockwise(pInput, 1, 1);

    //If there is an issue, undo it
    if (undo)
    {
	TetrisBlockAnticlockwise(pInput, 0, 0);
	TetrisBlockAnticlockwise(pInput, 0, 1);
	TetrisBlockAnticlockwise(pInput, 1, 0);
	TetrisBlockAnticlockwise(pInput, 1, 1);
    }
    ////////////////////////////////////////////////////////////////////////////
    //Move block left edge
    undo = false;
    while (TetrisBlockColumnClear(pInput, 0))
	undo |= TetrisBlockShiftL(pInput);
    
    ////////////////////////////////////////////////////////////////////////////
    if (undo)
    {
	//Shift frame right until, 
	undo = false;
    }
    ////////////////////////////////////////////////////////////////////////////
    //Move to lowest row
    undo = false;
    while (TetrisBlockRowClear(pInput, 3))
	undo |= TetrisBlockShiftD(pInput);
    
    ////////////////////////////////////////////////////////////////////////////
    //Move block to match previous bottom
    
}