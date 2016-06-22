#ifndef __GF2M_H
#define __GF2M_H

#include "bigint.h"

#define ECCGF2m_WORDSIZE	32
#define ECCGF2m_M_VALUE     511
#define ECCGF2m_NUMBITS		(ECCGF2m_M_VALUE+1)
#define	ECCGF2m_NUMWORD		((ECCGF2m_NUMBITS+ECCGF2m_WORDSIZE-1)/ECCGF2m_WORDSIZE)
#define ECCGF2m_ECC_MAXLONG		(ECCGF2m_NUMWORD+1)
#define ECCGF2m_BYTENUM		(ECCGF2m_NUMBITS/8)

typedef struct {
	U32 d[ECCGF2m_ECC_MAXLONG];
//	U32 bit_len;
}  ECCGF2m_BIGINT;

typedef struct {
	U32 d[2*ECCGF2m_NUMWORD+1];
//	U32 bit_len;
}  ECCGF2m_Double_BIGINT;

typedef struct
{
        ECCGF2m_BIGINT x;
        ECCGF2m_BIGINT y;
} ECCGF2m_ECC_POINT;

typedef struct
{
        ECCGF2m_BIGINT x;
        ECCGF2m_BIGINT y;
        ECCGF2m_BIGINT z;
} ECCGF2m_ECC_CURVE;

/*************************** ECCGF2m_init ******************************
Input   : mlen---the length of GF2m field bit length
Output : none;
Return : none;
Function:  initialize the GF2m field calculate; 
**********************************************************************/ 
void ECCGF2m_init(U32 m);

/****************************** ECCGF2m_calc_bitlen *********************
Input   : a--- pointer of ECCGF2m_BIGINT type
Output : none
Return : bit length;
Function:  calc the bit length of a; 
**********************************************************************/ 
U32 ECCGF2m_calc_bitlen(ECCGF2m_BIGINT *a);


// algorithms library on curve 

/****************************** ECCGF2m_point_mul *********************
Input   : k--- pointer of ECCGF2m_BIGINT type
             x--- pointer of ECCGF2m_ECC_POINT type
             y--- pointer of ECCGF2m_ECC_POINT type
Output : none;
Return : none;
Function:  y = k*x, GF2m field ; 
**********************************************************************/ 
void ECCGF2m_point_mul(ECCGF2m_BIGINT *k,ECCGF2m_ECC_POINT *x, ECCGF2m_ECC_POINT *y);

/****************************** ECCGF2m_point_mul *********************
Input   : f--- pointer of ECCGF2m_BIGINT type
             x--- pointer of ECCGF2m_ECC_POINT type          
Output : none;
Return : 0: x in the ecc, 1: x not in ecc ;
Function:  test x is in the ecc or not ; 
**********************************************************************/ 
U8 ECCGF2m_test_point(ECCGF2m_ECC_POINT *x,ECCGF2m_BIGINT *f);

#endif

