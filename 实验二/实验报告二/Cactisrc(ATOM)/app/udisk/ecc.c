/*
  Name: 
  Copyright: 
  Author: 
  Date: 17-06-05 13:59
  Description: 
*/

#define _ECC_C_

#include <instructions.h>
#include <bsp.h>
#include <types.h>
#include "ecc.h"
#include "nand_media.h"

unsigned int ecc_sw_correct(unsigned char *buff, unsigned char *buff_ecc, unsigned char* calc_ecc)
{

	unsigned int  iCompecc;
	unsigned char iEccsum;
    	unsigned char  iIndex;
    	unsigned char  nT1 , nT2 ;

	for ( iIndex = 0; iIndex <2; iIndex++)
    		{
        	nT1 ^= (((*buff_ecc) >> iIndex) & 0x01);
        	nT2 ^= (((*calc_ecc) >> iIndex) & 0x01);
    		}

	iCompecc = 0;
	for (iIndex = 0; iIndex < 3; iIndex++)
    		{
        	iCompecc |= ((~(*buff_ecc++) ^ ~(*calc_ecc++)) << iIndex * 8);
		}

	iEccsum = 0;
	for(iIndex = 0; iIndex < 24; iIndex++) 
		{
        	iEccsum += ((iCompecc >> iIndex) & 0x01);
		}
	
	switch (iEccsum) 
		{
		case 0 :
			//printf("ECC_NO_ERROR\n");
        		return ECC_NO_ERROR;

		case 1 :
			//printf("ECC_ECC_ERROR\n");
        		return ECC_ECC_ERROR;

    		case 12 :
        		if (nT1 != nT2)
        			{
		            	nT1 = ((iCompecc >> 17 & 1) << 8) + ((iCompecc >> 15 & 1) << 7) + ((iCompecc >> 13 & 1) << 6)
		                      + ((iCompecc >> 11 & 1) << 5) + ((iCompecc >> 9 & 1) << 4) + ((iCompecc >> 7 & 1) << 3)
		                      + ((iCompecc >> 5 & 1) << 2) + ((iCompecc >> 3 & 1) << 1) + (iCompecc >> 1 & 1);
		            	nT2 =  (((iCompecc >> 23 & 1) << 2) + ((iCompecc >> 21 & 1) << 1) + (iCompecc >> 19 & 1));
		            	buff[nT1] = (buff[nT1] ^ (1 << nT2));
				//printf("iCompecc = %d\n",iCompecc);
		            	//printf("RESULT : one bit error\r\n");
		            	//printf("byte = %d, bit = %d\r\n", nT1, nT2);       
            			return ECC_CORRECTABLE_ERROR;
        			}
        		else
            			return ECC_UNCORRECTABLE_ERROR;

		default :
			//printf("ECC_UNCORRECTABLE_ERROR\n");
        		return ECC_UNCORRECTABLE_ERROR;
    	}   
}
unsigned int ecc_check_and_correct(unsigned char *buff,unsigned char* calc_ecc)
{
#if 1
	unsigned int result;
	if(K_SUCCES != memcmp(nand_redt_data + 0x0D, calc_ecc , 3))
		{
		//printf("ecc error 0\n");
		//printf("ECC:read %x%x%x, cal %x%x%x\n",buff[8],buff[9],buff[0x0a],calc_ecc[0],calc_ecc[1],calc_ecc[2]);
		result = ecc_sw_correct(buff, nand_redt_data + 0x0D,calc_ecc);
		if((ECC_ECC_ERROR == result) || (ECC_UNCORRECTABLE_ERROR == result))
			{
			if(K_SUCCES != memcmp(nand_redt_data + 0x08, calc_ecc, 3))
				{
				//printf("ecc error 1\n");
				//printf("ECC:read %x%x%x, cal %x%x%x\n",buff[8],buff[9],buff[0x0a],calc_ecc[0],calc_ecc[1],calc_ecc[2]);
				return(ecc_sw_correct(buff + 0x100, nand_redt_data + 0x08,calc_ecc));
				}
			}
		}
#endif	
	return ECC_NO_ERROR;
}


