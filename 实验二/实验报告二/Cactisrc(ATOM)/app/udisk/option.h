#ifndef __OPTION_H__
#define __OPTION_H__
/*++

Copyright (c) 2004 GoldInfo Technology Co., Ltd.

Module Name:

        option.h

Abstract:

        Header file related to pre-compile option

Environment:

        ccs2.0 IDE

Revision History:

        01/5/2004      Mikal   created

--*/

//*****************************************************************************
// Data type
//*****************************************************************************

// 8-bit
#define CHAR            signed char 
#define UCHAR           unsigned char
#define PCHAR           signed char *
#define PUCHAR          unsigned char *
#define BYTE            unsigned char

// 16-bit
#define SHORT           signed short
#define USHORT          unsigned short
#define PSHORT          signed short *
#define PUSHORT         unsigned short *
#define WORD            unsigned short

// 32-bit
#define LONG            signed long
#define ULONG           unsigned long
#define PLONG           signed long *
#define PULONG          unsigned long *
#define DOUBLEWORD      unsigned long

// boolean
#define FALSE           0
#define TRUE            1


#define  Word32            long  int      
#define  Word16            short     


#define  UWord32           unsigned long     
#define  UWord16           unsigned short   
#define  UCHAR             unsigned char        


//*****************************************************************************

#define  G_NandFlash_Type    3
#define  G_FAT16_Format      1


#if (G_FAT12_Format==1)

#if (G_NandFlash_Type==1)
#define NandFlash_64MB       1

#else 
#define NandFlash_128MB      2

#endif
#endif 

#if (G_FAT16_Format==1)

#if(G_NandFlash_Type==3)
#define NandFlash_256MB      3

#else 
#define NandFlash_512MB      4

#endif 
#endif


//*****************************************************************************
//usb define mass storage
#define MSDC_OPTION      0x01  
#define G_bChoose_MSDC   0x01
#endif

