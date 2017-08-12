#include "IMU.h"
#include "ISR.h"

IMU_CMD pCMD;
bool Read = false;

bool IMU_I2C_Continue(I2CTPtr input)
{
    return false;
}
//The protocol chain call for a register read or block read
bool IMU_ReadRegFinish(I2CTPtr)
{
    pCMD.pCount = 0;
    return false;
}
bool IMU_ReadRegContinue    (I2CTPtr)
{
    //I2CTPtr pInput, UINT8 pAddress, UINT8Ptr pData, UINT8 pCount
    return (I2C_RRead(eI2C_Ch1, IMU_ADDR0, pCMD.pData, pCMD.pCount, IMU_ReadRegFinish));
}
bool IMU_ReadReg(UINT8 pRegister, UINT8Ptr pValue, UINT8 pCount, void (*ReadComplete)())
{
    static UINT8 r;
    r = pRegister;
    ifzero(pCMD.pCount)
    {
        ////////////////////////////////////////////////////////////////////////
        pCMD.pRegister  = r;
        pCMD.pData      = pValue;
        pCMD.pCount     = pCount;
        ////////////////////////////////////////////////////////////////////////
        I2C_Write(  eI2C_Ch1,       IMU_ADDR0, 
                    &r,             1, 
                    ReadComplete,   IMU_ReadRegContinue);
        ////////////////////////////////////////////////////////////////////////
        return true;
    }
    return false;
}

bool IMU_WriteRegFinish(I2CTPtr)
{
    pCMD.pCount = 0;
    return false;
}
bool IMU_WriteRegContinue(I2CTPtr)
{
    //I2CTPtr pInput, UINT8 pAddress, UINT8Ptr pData, UINT8 pCount
    return (I2C_RWrite(eI2C_Ch1, IMU_ADDR0, pCMD.pData, pCMD.pCount, IMU_WriteRegFinish));
}
bool IMU_WriteReg(UINT8 pRegister, UINT8Ptr pValue, UINT8 pCount, void (*WriteComplete)())
{
    static UINT8 r;
    r = pRegister;
    ifzero(pCMD.pCount)
    {
        ////////////////////////////////////////////////////////////////////////
        pCMD.pRegister  = r;
        pCMD.pData      = pValue;
        pCMD.pCount     = pCount;
        ////////////////////////////////////////////////////////////////////////
        I2C_Write(  eI2C_Ch1,       IMU_ADDR0, 
                    &r,             1, 
                    WriteComplete,  IMU_WriteRegContinue);
        ////////////////////////////////////////////////////////////////////////
        return true;
    }
    return false;
}

bool IMU_ReadPoll()
{
    if (Read)
    {
        Read = false;
        return true;
    }
    return false;    
}
void IMU_ReadComplete()
{
    Read = true;
}
void IMU_Init()
{ 
    ISR_Enable();
    ////////////////////////////////////////////////////////////////////////////
    UINT8 pData;
    
    ////////////////////////////////////////////////////////////////////////////
    I2C_Init(eI2C_Ch1, KHZ(100), eI2C_HighSpeed, eI2C_Master);
    
    ////////////////////////////////////////////////////////////////////////////
    pCMD.pRegister  = 0;
    pCMD.pData      = 0;
    pCMD.pCount     = 0;
    ////////////////////////////////////////////////////////////////////////////
    //Check the WHO_AM_I register
    Read = false;
    if (IMU_ReadReg(FXOS8700CQ_WHOAMI, &pData, 1, IMU_ReadComplete))
    {
        //Wait for I2C read of WHOAMI
        while(!IMU_ReadPoll());
        
        //Register matches expected value
        if (pData == FXOS8700CQ_WHOAMI_VAL)
        {
            pData = 0x00;
            ////////////////////////////////////////////////////////////////////
            return;
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////
    //Calibrate
    
    ISR_Disable();
}

