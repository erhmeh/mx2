///* 
// * File:   FPGA.h
// * Author: User
// *
// * Created on 22 May 2017, 8:01 PM
// */
//
//#ifndef FPGA_H
//#define	FPGA_H
//
//#include "Types.h"
//
//
//////////////////////////////////////////////////////////////////////////////////
//void Flash_Init();
//void Flash_WriteEnable();
//void Flash_WriteDisable();
//////////////////////////////////////////////////////////////////////////////////
//typedef union FPGA_ID FPGA_ID;
//union FPGA_ID
//{
//	UINT8 Data[3];
//	struct
//	{
//		UINT8   mManufacturerID;
//		UINT8   mMemoryType;
//		UINT8   mMemoryCapacity;
//	};
//};
//void Flash_ReadID(FPGA_ID * const pInput);
//////////////////////////////////////////////////////////////////////////////////
//typedef union FPGA_Status FPGA_Status;
//union FPGA_Status
//{
//	UINT8	Data;
//	struct
//	{
//		UINT8   WIP : 1;
//		UINT8   WEL : 1;
//
//		UINT8   BP0 : 1;
//		UINT8   BP1 : 1;
//		UINT8   BP2 : 1;
//
//		UINT8 : 2;
//		UINT8   SRWD : 1;
//	};
//	struct
//	{
//		UINT8   WIP : 1;
//		UINT8   WEL : 1;
//		UINT8   BP : 3;
//		UINT8 : 2;
//		UINT8   SRWD : 1;
//	};
//};
//void Flash_ReadStatus(FPGA_Status * const pInput);
//void Flash_WriteStatus(FPGA_Status * const pStatus);
//////////////////////////////////////////////////////////////////////////////////
//union Flash_LockRegister
//{
//	UINT8 Data;
//	struct
//	{
//		UINT8 WriteLock : 1;
//		UINT8 LockDown : 1;
//		UINT8 : 6;
//	};
//};
//void Flash_ReadLock(UINT32 const pAddress, Flash_LockRegister * const pOutput);
//////////////////////////////////////////////////////////////////////////////////
//void Flash_ReadBytes(UINT32 const pAddress, UINT8 * const pData, UINT16 pCount);
//void Flash_PageWrite(UINT32 const pAddress, UINT8 * const pData, UINT16 pCount);
//void Flash_PageErase(UINT32 const pAddress);
//void Flash_SubsectorErase(UINT32 const pAddress);
//void Flash_SectorErase(UINT32 const pAddress);
//void Flash_BulkErase(UINT32 const pAddress);
//void Flash_PowerDown();
//void Flash_ReleasePowerDown();
//
//#endif
//
