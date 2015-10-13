/*******************************************************************************
********************************************************************************
** COPYRIGHT NOTIFICATION (c) 2013 HMS Industrial Networks AB                 **
**                                                                            **
** This code is the property of HMS Industrial Networks AB.                   **
** The source code may not be reproduced, distributed, or used without        **
** permission. When used together with a product from HMS, permission is      **
** granted to modify, reproduce and distribute the code in binary form        **
** without any restrictions.                                                  **
**                                                                            **
** THE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND. HMS DOES NOT    **
** WARRANT THAT THE FUNCTIONS OF THE CODE WILL MEET YOUR REQUIREMENTS, OR     **
** THAT THE OPERATION OF THE CODE WILL BE UNINTERRUPTED OR ERROR-FREE, OR     **
** THAT DEFECTS IN IT CAN BE CORRECTED.                                       **
********************************************************************************
********************************************************************************
** File Description:
** Object containing implementations for Crc calculations.
********************************************************************************
********************************************************************************
*/
#include "abcc_user_def.h"
#ifdef ABCC_USER_DRV_SPI
#include "abcc_td.h"

const UINT8 abBitReverseTable16[] = { 0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE, 0x1, 0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF };


/*
** Tables for generating the CRC32 checksum.
*/
const UINT32 crc_table32[] = { 0x4DBDF21CUL, 0x500AE278UL, 0x76D3D2D4UL, 0x6B64C2B0UL,
                               0x3B61B38CUL, 0x26D6A3E8UL, 0x000F9344UL, 0x1DB88320UL,
                               0xA005713CUL, 0xBDB26158UL, 0x9B6B51F4UL, 0x86DC4190UL,
                               0xD6D930ACUL, 0xCB6E20C8UL, 0xEDB71064UL, 0xF0000000UL };


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** CRC_Crc32()
**------------------------------------------------------------------------------
*/
UINT32 CRC_Crc32( UINT8* pbBufferStart, UINT16 iLength )
{
   UINT8 bCrcReverseByte;
   UINT16 i;
   UINT32 lCrc = 0x0;

   for(i = 0; i < iLength; i++)
   {
      bCrcReverseByte = lCrc ^ abBitReverseTable16[ (*pbBufferStart >> 4 ) & 0xf ];
      lCrc = (lCrc >> 4) ^ crc_table32[ bCrcReverseByte & 0xf ];
      bCrcReverseByte = lCrc ^ abBitReverseTable16[ *pbBufferStart  & 0xf ];
      lCrc = (lCrc >> 4) ^ crc_table32[ bCrcReverseByte & 0xf ];
      pbBufferStart++;
  }


    lCrc = ((UINT32)abBitReverseTable16 [ (lCrc & 0x000000F0UL) >> 4 ] )|
           ((UINT32)abBitReverseTable16 [ (lCrc & 0x0000000FUL) ] ) << 4 |
           ((UINT32)abBitReverseTable16 [ (lCrc & 0x0000F000UL ) >> 12 ] << 8)|
           ((UINT32)abBitReverseTable16 [ (lCrc & 0x00000F00UL ) >> 8 ] << 12)|
           ((UINT32)abBitReverseTable16 [ (lCrc & 0x00F00000UL ) >> 20 ]<< 16)|
           ((UINT32)abBitReverseTable16 [ (lCrc & 0x000F0000UL ) >> 16 ]<< 20)|
           ((UINT32)abBitReverseTable16 [ (lCrc & 0xF0000000UL ) >> 28 ]<< 24) |
           ((UINT32)abBitReverseTable16 [ (lCrc & 0x0F000000UL ) >> 24 ]<< 28);



   return lCrc;

} /* End of CRC_Crc32() */

#endif







/*******************************************************************************
** End of crc.c
********************************************************************************
*/
