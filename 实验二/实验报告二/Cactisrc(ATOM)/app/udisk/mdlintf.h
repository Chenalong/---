#ifndef __MdlINTF_H
#define __MdlINTF_H

#include <instructions.h>
#include <bsp.h>
//#include <types.h>
#include "bigint.h"
#include "mydef.h"

/*************************** ABmodN_little ******************************
Input   : out,a,b,n--- pointer of U32
             nlen---the length of n
Output : none;
Return : none;
Function:  out = a*b mod (n); 
**********************************************************************/ 
void ABmodN_little(U32 *out, U32 *a, U32 *b, U32 *n, U32 nlen);

/************************* AEmodN_for_RSA *****************************
Input   : out,a,e,n--- pointer of U32
             nlen---the length of n
             elen---the length of e
Output : none;
Return : 0(OK),1(ERROR);
Function:  out = a^b mod (n); 
**********************************************************************/ 
U8 AEmodN_for_RSA(U32 *out, U32 *a, U32 *e, U32 elen, U32 *n, U32 nlen);

/*************************** ABmul_hardware ******************************
Input   : out,a,b--- pointer of U32
             nlen---the length of n
Output : none;
Return : none;
Function:  out = a*b ;
note: a and b bit length must <= 1024 
**********************************************************************/ 
void ABmul_hardware(U32 *out, U32 *a, U32 alen,U32 *b, U32 blen);

/*************************** GF2m_ABmul_hardware ******************************
Input   : out,a,b--- pointer of U32
             mlen---the length of GF2m field bit length
Output : none;
Return : none;
Function:  out = a*b, GF2m field ;
note: a and b bit length must <= 511 ;
**********************************************************************/ 
void GF2m_ABmul_hardware(U32 *out, U32 *a, U32 *b, U32 mlen);

/*************************** GF2m_CmodF_hardware ******************************
Input   : out,c,f--- pointer of U32
             mlen---the length of GF2m field bit length
Output : none;
Return : none;
Function:  out = c mod f, GF2m field ;
note: c and f bit length must <= 511 
**********************************************************************/ 
void GF2m_CmodF_hardware(U32 *out, U32 *c, U32 *f,U32 mlen);

/*************************** GF2m_ABmodF_hardware ******************************
Input   : out,a,b,f--- pointer of U32
             mlen---the length of GF2m field bit length
Output : none;
Return : none;
Function:  out = a*b mod f, GF2m field ;
note: a, f and b bit length must <= 511 ; 
**********************************************************************/ 
void GF2m_ABmodF_hardware(U32 *out, U32 *a, U32 *b, U32 *f,U32 mlen);

/**************************** RNG_Init ********************************
Input   : none; 
Output : none;
Return : none;
Function:  initialize the RNG module; 
**********************************************************************/ 
void RNG_Init();

/**************************** Get_Rand ********************************
Input   : x--- the RNG control register value 
Output : none;
Return : the random;
Function:  get a random of U32 type; 
**********************************************************************/ 
unsigned long Get_Rand();

/*************************** brev_transfer ******************************
Input   : dst,src--- pointer of U32
             len---the length of src data
Output : none;
Return : none;
Function:  endian convert ; 
**********************************************************************/ 
void brev_transfer(U32 *dst, U32 *src, U32 len);

/*************************** brev_transfer ******************************
Input   : buf--- pointer of U32
             len---the length of src data
Output : none;
Return : none;
Function:  buffer convert ;
Note: len must is a  even;
**********************************************************************/ 
void swap_buffer(U32 *buf,U32 len);

/*************************** brev_transfer ******************************
Input   : key,desiv,din,dout--- pointer of U32
             dcntl---the DES control register value
Output : none;
Return : none;
Function:  DES/TDES calc;
Note: if use ECB mode, the desiv can set be NULL;
         dcntl value should be incording to zi1222 spec;
**********************************************************************/ 
void DES_hardware_crypt(U32 *key,U32 *desiv,U32 *din,U32 *dout,U32 dcntl);

/*************************** HCMD_CALC ******************************
Input   : n--- pointer of U32
             nlen---the length of n bit length
Output : none;
Return : none;
Function:  calc the H value in p field ;
               it only use for 192 bit p field ECC ;
**********************************************************************/ 
void HCMD_CALC(U32 *n, U32 nlen);

/*************************** ABmodN_FOR_ECC ******************************
Input   : out,a,b--- pointer of U32          
Output : none;
Return : none;
Function:  out = a*b mod n, calc the value in p field ;
               it only use for 192 bit p field ECC ;
**********************************************************************/ 
void ABmodN_FOR_ECC(U32 *out, U32 *a, U32 *b);

#endif



