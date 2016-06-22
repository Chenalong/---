#ifndef __BIGINT_H
#define __BIGINT_H

typedef char S8;
typedef short S16;
typedef long S32;
typedef long long S64;

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned long U32;
typedef unsigned long long U64;

#define INDEX S16
#define ECC_WORDSIZE	32
#define ECC_NUMBITS		192
#define	ECC_NUMWORD		(ECC_NUMBITS/ECC_WORDSIZE)
#define ECC_MAXLONG		(ECC_NUMWORD+1)
#define ECC_BYTENUM		(ECC_NUMBITS/8)

typedef struct {
	U32 data[ECC_MAXLONG];
}  BIGINT;

typedef struct {
	U32 data[2*ECC_MAXLONG];
}  TWO_BIGINT;

typedef struct {
	U32 e[ECC_MAXLONG];
}  FIELD2N;

typedef struct
{
        BIGINT x;
        BIGINT y;
} ECC_POINT;

typedef struct
{
        BIGINT x;
        BIGINT y;
        BIGINT z;
} ECC_CURVE;

typedef struct {
    U32 state[8], length, curlen;
    U8 buf[64];
} sha_state;

/****************************** ecc_point_mul *********************
Input   : k--- pointer of BIGINT type
             x--- pointer of ECC_POINT type
             y--- pointer of ECC_POINT type
Output : none;
Return : none;
Function:  y = k*x, p field ; 
**********************************************************************/ 
void ecc_point_mul(BIGINT *k,ECC_POINT *x, ECC_POINT *y);

/****************************** ECC_test_point *********************
Input   : p--- pointer of BIGINT type
             x--- pointer of BIGINT type          
Output : none;
Return : 0: x in the ecc, 1: x not in ecc ;
Function:  test x is in the ecc or not, p field ; 
**********************************************************************/ 
U8 ECC_test_point(ECC_POINT *x,BIGINT *p);

#endif

