#ifndef __RSA_BIG_H
#define __RSA_BIG_H

#include "bigint.h"

#define RSA_WORDSIZE	32
#define RSA_NUMBITS		2048
#define RSA_NUMWORD	(RSA_NUMBITS/RSA_WORDSIZE)
#define RSA_MAXLONG	(RSA_NUMWORD+1)
#define RSA_BYTENUM	(RSA_NUMBITS/8)

// low address save low data, and high address save high data
// sample: 0x1111111122222222 save as data[0]=0x22222222, data[1]=0x11111111
typedef struct {
       U32 len;
	U32 data[RSA_MAXLONG];
}  RSA_BIGINT;

/****************************** Get_Prime_new **************************
Input   : prime--- pointer of RSA_BIGINT type
             len--- length of prime
Output : none
Return : none;
Function:  get a prime; 
**********************************************************************/ 
void Get_Prime_new(RSA_BIGINT *prime,U32 length);

/*************************** RSA_Get_Key_For_CRT **********************
Input   : u,e,dp,dq,n,p,q,n--- pointer of RSA_BIGINT type
             nlen--- bit length of n
             elen--- bit length of e
Output : none
Return : none;
Function:  get RSA key which is used for CRT; 
**********************************************************************/ 
void RSA_Get_Key_For_CRT(RSA_BIGINT *u,RSA_BIGINT *e,RSA_BIGINT *dp,RSA_BIGINT *dq,RSA_BIGINT *p,RSA_BIGINT *q,RSA_BIGINT *n,U32 nlen, U32 elen);


/**************************** RSA_bigint_mod_e_CRT *********************
Input   : in,u,dp,dq,p,q,out--- pointer of RSA_BIGINT type
Output : none
Return : none;
Function:  do RSA encrypt or decrypt with CRT; 
**********************************************************************/ 
void RSA_bigint_mod_e_CRT(RSA_BIGINT *in, RSA_BIGINT *u,RSA_BIGINT *dp,RSA_BIGINT *dq,RSA_BIGINT *p,RSA_BIGINT *q,RSA_BIGINT *out);


/**************************** RSA_bigint_mod_e_CRT *********************
Input   : a,e,n,out--- pointer of RSA_BIGINT type;
Output : none;
Return : none;
Function:  do RSA encrypt or decrypt without CRT; out = a^e mod n; 
**********************************************************************/ 
void RSA_bigint_mod_e(RSA_BIGINT *a,RSA_BIGINT *e,RSA_BIGINT *n,RSA_BIGINT *out);

/****************************** RSA_Get_Key_new **********************
Input   : e,d,n--- pointer of RSA_BIGINT type
             nlen--- bit length of n
             elen--- bit length of e
Output : none
Return : none;
Function:  get RSA key; 
**********************************************************************/ 
void RSA_Get_Key_new(RSA_BIGINT *e,RSA_BIGINT *d,RSA_BIGINT *n,U32 nlen, U32 elen);

/****************************** RSA_calc_bit_length *********************
Input   : a--- pointer of RSA_BIGINT type
Output : none
Return : bit length;
Function:  calc the bit length of a; 
**********************************************************************/ 
U32 RSA_calc_bit_length(RSA_BIGINT *a);
#endif

///////////////////////////////////////////////////////////////////////////////
// End of Files
///////////////////////////////////////////////////////////////////////////////
