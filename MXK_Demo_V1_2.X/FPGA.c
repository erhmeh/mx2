//#include "FPGA.h"
//#include "SPI.h"
//
//enum FPGA_Commands
//{
//	WriteEnable = 0x06,
//	WriteDisable = 0x04,
//	ReadIdentification = 0x9F,
//	ReadStatusRegister = 0x05,
//	WriteStatusRegister = 0x01,
//	WritetoLockRegister = 0xE5,
//	ReadLockRegister = 0xE8,
//	ReadDataBytes = 0x03,
//	ReadDataBytesHS = 0x0B,
//	PageWrite = 0x0A,
//	PageProgram = 0x02,
//	PageErase = 0xDB,
//	SubSectorErase = 0x20,
//	SectorErase = 0xD8,
//	BulkErase = 0xC7,
//	PowerDown = 0xB9,
//	WakeUp = 0xAB
//};
//
//UINT8 const SPI_Read(SPIPtr pInput);
//
//union Address
//{
//	UINT32		Address;
//	struct
//	{
//	    UINT8 const	Data[4];
//	};
//};
//void        Flash_SendByte(UINT8 const pData);
//UINT8 const Flash_ReadByte();
//void        Flash_SendAddress(UINT32 pAddress);
//void        Flash_SendCommand(FPGA_Commands const pCommand);
//void        Flash_Swap3(UINT8 Data[3]);
//
//void Flash_Init()
//{
//    SPI_Init(MHZ(1), eSPI2, eSPI_Mode0);
//}
//
//UINT8 const SPI_Read(SPIPtr pInput)
//{
//    return SPI_Send(pInput, 0);
//}
//void Flash_SendAddress(UINT32 pAddress)
//{
//	Address Temp{ 0 };
//	Temp.Address = pAddress & 0xfff;
//
//	UINT8 i = 0;
//	for (i = 0; i < 3; ++i)
//		SPI_Send(&SPI2, Temp.Data[i]);
//}
//void Flash_SendCommand(FPGA_Commands const pCommand)
//{
//	SPI_Send(&SPI2, (UINT8)pCommand);
//}
//void Flash_SendByte(UINT8 const pData)
//{
//	SPI_Send(&SPI2, (UINT8)pData);
//}
//UINT8 const Flash_ReadByte()
//{
//	return SPI_Send(&SPI2, 0);
//}
//void Flash_WriteEnable()
//{
//	//Write Enable
//	Flash_SendCommand(WriteEnable);
//}
//void Flash_WriteDisable()
//{
//    //Write Enable
//    Flash_SendCommand(WriteDisable);
//}
//void Flash_Swap3(UINT8 Data[3])
//{
//	UINT8 i = 0;
//	for (i = 0; i < 3; ++i)
//		Data[i] = SPI_Send(&SPI2, Data[i]);
//}
//void Flash_ReadID(FPGA_ID * const pInput)
//{
//	Flash_SendCommand(ReadIdentification);
//	Flash_Swap3(pInput->Data);
//}
//void Flash_ReadStatus(FPGA_Status * const pInput)
//{
//	Flash_SendCommand(ReadStatusRegister);
//	pInput->Data = Flash_ReadByte();
//}
//void Flash_WriteStatus(FPGA_Status * const pStatus)
//{
//	Flash_SendCommand(WriteStatusRegister);
//	Flash_SendByte(pStatus->Data);
//}
//void Flash_ReadLock(UINT32 const pAddress, Flash_LockRegister * const pOutput)
//{
//	Flash_SendCommand(ReadLockRegister);
//	Flash_SendAddress(pAddress);
//	pOutput->Data = Flash_ReadByte();
//}
//void Flash_ReadBytes(UINT32 const pAddress, UINT8 * const pData, UINT16 pCount)
//{
//	if (pCount == 0)
//		return;
//	if (pCount > 256)
//		pCount = 256;
//
//	Flash_SendCommand(ReadDataBytes);
//	Flash_SendAddress(pAddress);
//
//	UINT16 i = 0;
//	for (i = 0; i < pCount; ++i)
//		pData[i] = Flash_ReadByte();
//}
//void Flash_PageWrite(UINT32 const pAddress, UINT8 * const pData, UINT16 pCount)
//{
//	if (pCount == 0)
//		return;
//	if (pCount > 256)
//		pCount = 256;
//
//	Flash_SendCommand(PageWrite);
//	Flash_SendAddress(pAddress);
//
//	UINT16 i = 0;
//	for (i = 0; i < pCount; ++i)
//		Flash_SendByte(pData[i]);
//}
//void Flash_PageErase(UINT32 const pAddress)
//{
//	Flash_SendCommand(PageErase);
//	Flash_SendAddress(pAddress);
//}
//void Flash_SubsectorErase(UINT32 const pAddress)
//{
//	Flash_SendCommand(SubSectorErase);
//	Flash_SendAddress(pAddress);
//}
//void Flash_SectorErase(UINT32 const pAddress)
//{
//	Flash_SendCommand(SectorErase);
//	Flash_SendAddress(pAddress);
//}
//void Flash_BulkErase(UINT32 const pAddress)
//{
//	Flash_SendCommand(BulkErase);
//}
//void Flash_PowerDown()
//{
//	Flash_SendCommand(PowerDown);
//}
//void Flash_ReleasePowerDown()
//{
//	Flash_SendCommand(WakeUp);
//}
