/* 
 * File:   IMU.h
 * Author: David Ledger
 *
 * Created on 16 January 2017, 1:30 PM
 */
#ifndef IMU_H
#define	IMU_H

#include "Types.h"
#include "I2C.h"
#include "MXK.h"
#include "LED.h"

// FXOS8700CQ I2C address
#define IMU_ADDR0    0x1E
#define IMU_ADDR1    0x1D
#define IMU_ADDR2    0x1C
#define IMU_ADDR3    0x1F

// FXOS8700CQ internal register addresses
#define FXOS8700CQ_STATUS       0x00
#define FXOS8700CQ_WHOAMI       0x0D
#define FXOS8700CQ_XYZ_DATA_CFG 0x0E
#define FXOS8700CQ_CTRL_REG1    0x2A
#define FXOS8700CQ_M_CTRL_REG1  0x5B
#define FXOS8700CQ_M_CTRL_REG2  0x5C
#define FXOS8700CQ_WHOAMI_VAL   0xC7

typedef struct IMUVec8  IMUVec8;
typedef struct IMUVec16 IMUVec16;
struct IMUVec8
{
    UINT8 x, y, z;
};
struct IMUVec16
{
    UINT16 x, y, z;
};

typedef struct IMU  IMU;
typedef IMU * const IMUPtr;
struct IMU
{
    UINT8       pStatus;
    IMUVec16    pOut;
    UINT16      pReserved1;
    UINT8       pF_Setup;
    UINT8       pTrig_CFG;
    UINT8       pSysMod;
    UINT8       pInt_Source;
    UINT8       pWho_Am_I;
    UINT8       pXYZ_Data_CFG;
    UINT8       pHP_Filter_Cutoff;
    UINT8       pPL_Status;
    UINT8       pPL_CFG;
    UINT8       pPL_Count;
    UINT8       pPL_BF_ZCOMP;
    UINT8       pPL_THS_REG;
    UINT8       pA_FFMT_CFG;
    UINT8       pA_FFMT_SRC;
    UINT8       pA_FFMT_THS_STAT;
    UINT8       pA_FFMT_COUNT;
    UINT8       pTransmient_CFG;
    UINT8       pTransmient_SRC;
    UINT8       pTransmient_THS;
    UINT8       pTransmient_COUNT;
    UINT8       pPulse_CFG;
    UINT8       pPulse_SRC;
    IMUVec8     pPulse_THS;
    UINT8       pPulse_TMLT;
    UINT8       pPulse_LTCY;
    UINT8       pPulse_WIND;
    UINT8       pASLP_Count;
    UINT8       pCTRL_REG1;
    UINT8       pCTRL_REG2;
    UINT8       pCTRL_REG3;
    UINT8       pCTRL_REG4;
    UINT8       pCTRL_REG5;
    IMUVec8     pOFF;
    UINT8       pM_DR_STATUS;
    IMUVec16    pM_OUT;
    IMUVec16    pCMP;
    IMUVec16    pM_OFF;
    IMUVec16    pMAX;
    IMUVec16    pMIN;
    UINT8       pTEMP;
    UINT8       pM_THS_CFG;
    UINT8       pM_THS_SRC;
    IMUVec16    pM_THS;
    UINT8       pM_THS_COUNT;
    UINT8       pM_CTRL_REG1;
    UINT8       pM_CTRL_REG2;
    UINT8       pM_CTRL_REG3;
    UINT8       pM_INT_SRC;
    UINT8       pA_VECM_CFG;
    UINT16      pA_VECM_THS;
    UINT8       pA_VECM_CNT;
    IMUVec16    pA_VECM_INIT;
    UINT8       pM_VECM_CFG;
    UINT16      pM_VECM_THS;
    UINT8       pM_VECM_CNT;
    IMUVec16    pM_VECM_INIT;
    IMUVec16    pA_FFMT_THS;
    UINT8       pReserved2;
};
typedef struct IMU_CMD IMU_CMD;
struct IMU_CMD
{
    UINT8       pRegister;
    UINT8Ptr    pData;
    UINT8       pCount;
};
bool IMU_I2C_Continue(I2CTPtr input);
bool IMU_ReadRegFinish      (I2CTPtr);
bool IMU_ReadRegContinue    (I2CTPtr);
bool IMU_ReadReg(UINT8 pRegister, UINT8Ptr pValue, UINT8 pCount, void (*ReadComplete)());
bool IMU_WriteRegFinish(I2CTPtr);
bool IMU_WriteRegContinue(I2CTPtr);
bool IMU_WriteReg(UINT8 pRegister, UINT8Ptr pValue, UINT8 pCount, void (*WriteComplete)());
bool IMU_ReadPoll();
void IMU_ReadComplete();
void IMU_Init();
#endif	/* IMU_H */