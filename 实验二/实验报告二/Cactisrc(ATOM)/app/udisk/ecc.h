/*
  Name: 
  Copyright: 
  Author: 
  Date: 17-06-05 13:59
  Description: 
*/

#ifndef _ECC_H_
#define _ECC_H_

#define 		ECC_NO_ERROR			 		0
#define 		ECC_CORRECTABLE_ERROR		1
#define 		ECC_ECC_ERROR					2
#define 		ECC_UNCORRECTABLE_ERROR		3

extern unsigned int ecc_check_and_correct(unsigned char *buff,unsigned char* calc_ecc);

#endif

