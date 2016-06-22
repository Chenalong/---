/*
    GF2m.c
*/
#include <instructions.h>
#include <bsp.h>
#include "GF2m.h"

#define OK 0
#define FAIL 1

void ECCGF2m_BIGINT_add(ECCGF2m_BIGINT *a,ECCGF2m_BIGINT *b,ECCGF2m_BIGINT *c);
void ECCGF2m_BIGINT_Rshift(ECCGF2m_BIGINT *a, ECCGF2m_BIGINT *c);
U8  ECCGF2m_BIGINT_bit_value(ECCGF2m_BIGINT *a,U32 i);
void ECCGF2m_BIGINT_MUL(ECCGF2m_BIGINT *a, ECCGF2m_BIGINT *b, ECCGF2m_BIGINT *f, ECCGF2m_BIGINT *c);
void ECCGF2m_BIGINT_MAIAInv(ECCGF2m_BIGINT *a,ECCGF2m_BIGINT *f,ECCGF2m_BIGINT *t);
void ECCGF2m_BIGINT_MUL_COMB(ECCGF2m_BIGINT *a, ECCGF2m_BIGINT *b, ECCGF2m_Double_BIGINT *c);
void ECCGF2m_BIGINT_MUL2(ECCGF2m_BIGINT *a, ECCGF2m_BIGINT *f, ECCGF2m_BIGINT *c);
void ECCGF2m_calc_c(ECCGF2m_BIGINT *b,ECCGF2m_BIGINT *f,ECCGF2m_BIGINT *c);
void ECCGF2m_point_double(ECCGF2m_ECC_CURVE *x1, ECCGF2m_BIGINT *f, ECCGF2m_ECC_CURVE *x2);
void ECCGF2m_point_add(ECCGF2m_ECC_CURVE *x0, ECCGF2m_ECC_CURVE *x1, ECCGF2m_BIGINT *f, ECCGF2m_ECC_CURVE *x2);



extern U32 ECCGF2m_f[];
extern U32 ECCGF2m_a[];
extern U32 ECCGF2m_b[];
extern U32 ECCGF2m_G0x[];
extern U32 ECCGF2m_G0y[];
extern U32 ECCGF2m_k[];

ECCGF2m_BIGINT ECCGF2m_c;
U32 ECCGF2m_m;
U32 ECCGF2m_int;
U32 ECCGF2m_double_int;

#if 0
void ECCGF2m_to_char(BIGINT *a, char *c)
{
    int i,len;
	len = (ECCGF2m_m+1)>>5;
	if((ECCGF2m_m+1)&0x1f)
		len++;
    for(i=0;i<len;i++)
    {
        c[4*i] = (a->data[i])>>24;
        c[4*i+1] = (a->data[i])>>16;
        c[4*i+2] = (a->data[i])>>8;
        c[4*i+3] = (a->data[i]);
    }
}

void char_to_ECCGF2m(char *c, BIGINT *a)
{
    S32 i;
    a->data[0] = 0;
    for(i=0;i<6;i++)
    {
        a->data[i+1] = ((c[4*i]<<24)&0xff000000) | ((c[4*i+1]<<16)&0xff0000) | ((c[4*i+2]<<8)&0xff00) | (c[4*i+3]&0xff);

    }
}

void ECCGF2m_point_to_char(ECC_POINT *x,char *c)
{
    if(judge_ECC_null(x))
        c[0] = 0;
    else
        c[0] = 4;
    ECCGF2m_to_char(&(x->x),c+1);
    ECCGF2m_to_char(&(x->y),c+25);
}

void ECCGF2m_char_to_point(char *c, ECC_POINT *x)
{
    char_to_ECCGF2m(c+1, &(x->x));
    char_to_ECCGF2m(c+25, &(x->y));
}
#endif

void ECCGF2m_get_rand(ECCGF2m_BIGINT *a)
{
              int elen,ilen,i,tmp;
		elen = ECCGF2m_m +1;
		ilen = elen>>5;
		if(elen&0x1f)
			ilen++;
		for(i=0; i<ilen; i++)
		        a->d[i] = Get_Rand();             
              a->d[0] |= 1;
		tmp = (elen-1)&0x1f;	  
		for(i=31;i>tmp;i--)
                      a->d[ilen -1]  &= (~(1<<i));
		a->d[ilen -1] |= (1<<tmp);
		a->d[ilen ] = 0;
}

void ECCGF2m_calc_c(ECCGF2m_BIGINT *b,ECCGF2m_BIGINT *f,ECCGF2m_BIGINT *c)
{
#if 0
       U32 j,m_t;
       ECCGF2m_BIGINT u,v;
       ECCGF2m_BIGINT_MUL2(b,f,&u);
       v = u;
	m_t =  ECCGF2m_m-3;	
	j = 9;
	while(!(m_t&(1<<j)))
		j--;
	while(j)
		{
              j--;
		ECCGF2m_BIGINT_MUL2(&u,f,&u);
		if(m_t&(1<<j))
			ECCGF2m_BIGINT_MUL(&u,&v,f,&u);
		}   
       *c = u;
#else
       U32 j;
       ECCGF2m_BIGINT u;
       ECCGF2m_BIGINT_MUL2(b,f,&u);
       for(j=0;j<(ECCGF2m_m-3);j++)
           ECCGF2m_BIGINT_MUL2(&u,f,&u);	   
       *c = u;
#endif	   
}

void ECCGF2m_init(U32 m)
{
        ECCGF2m_m = m;
        ECCGF2m_int = ((m+ECCGF2m_WORDSIZE-1)/ECCGF2m_WORDSIZE)+1;
        ECCGF2m_double_int = 2*ECCGF2m_int-1;
        ECCGF2m_calc_c((ECCGF2m_BIGINT *)ECCGF2m_b,(ECCGF2m_BIGINT *)ECCGF2m_f,&ECCGF2m_c);
}

U32 ECCGF2m_calc_bitlen(ECCGF2m_BIGINT *a)
{
        U32 j,x;
        x = ECCGF2m_int;
        	while(!(a->d[x-1]))
		{
		x--;
		if(!x)
			break;
		}
	if(x)
		{
		j=31;
		while(!(a->d[x-1]&(1<<j)))
			j--;
		j = ((x-1)<<5)+j+1;
		}
	else
		j = 0;
        return j;
}

void ECCGF2m_BIGINT_add(ECCGF2m_BIGINT *a,ECCGF2m_BIGINT *b,ECCGF2m_BIGINT *c)
{
	U32 j;
	for(j=0;j<ECCGF2m_int;j++)
		c->d[j] = a->d[j] ^  b->d[j];

}

void ECCGF2m_BIGINT_Rshift(ECCGF2m_BIGINT *a, ECCGF2m_BIGINT *c)
{
	int j;
	U32 flag,temp;
	flag =0 ;
	for(j=(ECCGF2m_int-1);j>=0;j--)
		{
		temp = a->d[j];
		c->d[j] = (temp>>1)|flag;
		flag = temp<<31;
		}
}

U8  ECCGF2m_BIGINT_bit_value(ECCGF2m_BIGINT *a,U32 i)
{
        U32 x = i>>5;
        U32 y = 1<<(i&0x1f);
        if(a->d[x]&y)
            return 1;
        else
            return 0;
}

void ECCGF2m_BIGINT_MUL(ECCGF2m_BIGINT *a, ECCGF2m_BIGINT *b, ECCGF2m_BIGINT *f, ECCGF2m_BIGINT *c)
{
       GF2m_ABmodF_hardware(c->d,a->d,b->d,f->d,ECCGF2m_m);
       c->d[ECCGF2m_int-1] = 0;	
}

void ECCGF2m_BIGINT_MUL_COMB(ECCGF2m_BIGINT *a, ECCGF2m_BIGINT *b, ECCGF2m_Double_BIGINT *c)
{
        GF2m_ABmul_hardware(c->d,a->d,b->d,ECCGF2m_m);		
}

void ECCGF2m_BIGINT_MUL2(ECCGF2m_BIGINT *a, ECCGF2m_BIGINT *f, ECCGF2m_BIGINT *c)
{
       GF2m_ABmodF_hardware(c->d,a->d,a->d,f->d,ECCGF2m_m);
       c->d[ECCGF2m_int-1] = 0;		
}

void ECCGF2m_BIGINT_MAIAInv(ECCGF2m_BIGINT *a,ECCGF2m_BIGINT *f,ECCGF2m_BIGINT *t)
{
	ECCGF2m_BIGINT b,c,u,v,x;
	// 1. set b=1, c=0, u=a, v=f;
	memset(&b,0,ECCGF2m_int<<2);
	memset(&c,0,ECCGF2m_int<<2);
	b.d[0] = 1;
	u = *a;
	v = *f;
GF2m_MAIA_2:
	// 2.
	while(!(u.d[0]&1))
		{
		// 2.1
		ECCGF2m_BIGINT_Rshift(&u,&u);
		// 2.2
		while(b.d[0]&1)
			ECCGF2m_BIGINT_add(&b,f,&b);
		ECCGF2m_BIGINT_Rshift(&b,&b);
		}	
	// 3.
	if((u.d[0]==1)&&(ECCGF2m_calc_bitlen(&u)==1))
		{
		*t = b;
		return;
		}
	// 4.
	if(ECCGF2m_calc_bitlen(&u)<ECCGF2m_calc_bitlen(&v))
		{
		x = v;
		v = u;
		u = x;
		x = c;
		c = b;
		b = x;
		}
	// 5.
	ECCGF2m_BIGINT_add(&u,&v,&u);
	ECCGF2m_BIGINT_add(&b,&c,&b);
	// 6.
	goto GF2m_MAIA_2;

}

/////////////////////////////////////////////////////
///////////    ECC     //////////////////////////////                            //
/////////////////////////////////////////////////////
void ECCGF2m_clear_point(ECCGF2m_ECC_CURVE *a)
{
        memset(a->x.d,0,ECCGF2m_int<<2);
        memset(a->y.d,0,ECCGF2m_int<<2);
        memset(a->z.d,0,ECCGF2m_int<<2);
}

// if a=0 return 1;else return 0
U8 ECCGF2m_judge_bigint(ECCGF2m_BIGINT *a)
{
        U32 j;
        for(j=0;j<ECCGF2m_int;j++)
        {
            if(a->d[j])
                return 0;
        }
        return 1;
}

// ecc curve point double
void ECCGF2m_point_double(ECCGF2m_ECC_CURVE *x1, ECCGF2m_BIGINT *f, ECCGF2m_ECC_CURVE *x2)
{
        ECCGF2m_BIGINT t1,t2,t3,t4;
        if(ECCGF2m_judge_bigint(&(x1->y))||ECCGF2m_judge_bigint(&(x1->z)))
        {
            //memset(x2,0,sizeof(ECCGF2m_ECC_CURVE));
            ECCGF2m_clear_point(x2);
            x2->x.d[0] = x2->y.d[0] = 1;
        }
        t1 = x1->x;
        t2 = x1->y;
        t3 = x1->z;
        t4  = ECCGF2m_c;
        ECCGF2m_BIGINT_MUL(&t2,&t3,f,&t2); // 3
        ECCGF2m_BIGINT_MUL2(&t3,f,&t3);         // 4
        ECCGF2m_BIGINT_MUL(&t3,&t4,f,&t4);    // 5
        ECCGF2m_BIGINT_MUL(&t3,&t1,f,&t3);      // 6
        ECCGF2m_BIGINT_add(&t2,&t3,&t2);             // 7
        ECCGF2m_BIGINT_add(&t1,&t4,&t4);             // 8
        ECCGF2m_BIGINT_MUL2(&t4,f,&t4);              // 9
        ECCGF2m_BIGINT_MUL2(&t4,f,&t4);              // 10
        ECCGF2m_BIGINT_MUL2(&t1,f,&t1);         // 11
        ECCGF2m_BIGINT_add(&t1,&t2,&t2);             // 12
        ECCGF2m_BIGINT_MUL(&t2,&t4,f,&t2);           // 13
        ECCGF2m_BIGINT_MUL2(&t1,f,&t1);              // 14
        ECCGF2m_BIGINT_MUL(&t1,&t3,f,&t1);          // 15
        ECCGF2m_BIGINT_add(&t1,&t2,&t2);             // 16
        x2->x = t4;
        x2->y = t2;
        x2->z = t3;

}

void ECCGF2m_point_add(ECCGF2m_ECC_CURVE *x0, ECCGF2m_ECC_CURVE *x1, ECCGF2m_BIGINT *f, ECCGF2m_ECC_CURVE *x2)
{
        ECCGF2m_BIGINT t1,t2,t3,t4,t5,t6,t7,t8,t9;
        t1 = x0->x;  // 1
        t2 = x0->y;
        t3 = x0->z;
        t4 = x1->x;
        t5 = x1->y;

        if(!ECCGF2m_judge_bigint((ECCGF2m_BIGINT *)ECCGF2m_a))  // 2
                memcpy(t9.d,ECCGF2m_a,ECCGF2m_int<<2);
        if(!((x1->z.d[0]==1)&&(ECCGF2m_calc_bitlen(&(x1->z))==1))) // 3
        {
                t6 = t1;
                ECCGF2m_BIGINT_MUL2(&t6,f,&t7);
                ECCGF2m_BIGINT_MUL(&t1,&t7,f,&t1);
                ECCGF2m_BIGINT_MUL(&t6,&t7,f,&t7);
                ECCGF2m_BIGINT_MUL(&t2,&t7,f,&t2);
        }
        ECCGF2m_BIGINT_MUL2(&t3,f,&t7);       // 4
        ECCGF2m_BIGINT_MUL(&t4,&t7,f,&t8);    // 5
        ECCGF2m_BIGINT_add(&t1,&t8,&t1);      // 6
        ECCGF2m_BIGINT_MUL(&t3,&t7,f,&t7);    // 7
        ECCGF2m_BIGINT_MUL(&t5,&t7,f,&t8);    // 8
        ECCGF2m_BIGINT_add(&t2,&t8,&t2);      // 9
        if(ECCGF2m_judge_bigint(&t1))         // 10
        {
                if(ECCGF2m_judge_bigint(&t2))
                {
                        ECCGF2m_clear_point(x2);
                        return;
                }
                else
                {
                        ECCGF2m_clear_point(x2);
                        x2->x.d[0] = x2->y.d[0] = 1;
                        return;
                }
        }
        ECCGF2m_BIGINT_MUL(&t2,&t4,f,&t4);    // 11
        ECCGF2m_BIGINT_MUL(&t1,&t3,f,&t3);    // 12
        ECCGF2m_BIGINT_MUL(&t3,&t5,f,&t5);    // 13
        ECCGF2m_BIGINT_add(&t4,&t5,&t4);      // 14
        ECCGF2m_BIGINT_MUL2(&t3,f,&t5);       // 15
        ECCGF2m_BIGINT_MUL(&t4,&t5,f,&t7);      // 16
        if(!((x1->z.d[0]==1)&&(ECCGF2m_calc_bitlen(&(x1->z))==1))) // 17
                ECCGF2m_BIGINT_MUL(&t3,&t6,f,&t3);
        ECCGF2m_BIGINT_add(&t2,&t3,&t4);      // 18
        ECCGF2m_BIGINT_MUL(&t2,&t4,f,&t2);    // 19
        ECCGF2m_BIGINT_MUL2(&t1,f,&t5);       // 20
        ECCGF2m_BIGINT_MUL(&t1,&t5,f,&t1);    // 21
        if(!ECCGF2m_judge_bigint((ECCGF2m_BIGINT *)ECCGF2m_a))  // 22
        {
                ECCGF2m_BIGINT_MUL2(&t3,f,&t8);
                ECCGF2m_BIGINT_MUL(&t8,&t9,f,&t9);
                ECCGF2m_BIGINT_add(&t1,&t9,&t1);
        }
        ECCGF2m_BIGINT_add(&t1,&t2,&t1);    // 23
        ECCGF2m_BIGINT_MUL(&t1,&t4,f,&t4);  // 24
        ECCGF2m_BIGINT_add(&t4,&t7,&t2);    // 25
        // 26
        x2->x = t1;
        x2->y = t2;
        x2->z = t3;
}

// y=k*x
void ECCGF2m_point_mul(ECCGF2m_BIGINT *k,ECCGF2m_ECC_POINT *x, ECCGF2m_ECC_POINT *y)
{
    ECCGF2m_BIGINT *p;
    ECCGF2m_ECC_CURVE c_u,c_x;
    U32 i = ECCGF2m_m-1;
    p = (ECCGF2m_BIGINT *)ECCGF2m_f;
	
//printf("p...\n");
//    print_GF2m(p);
	
    if(ECCGF2m_judge_bigint(k))
    {
        memset(y->x.d,0,ECCGF2m_int<<2);
        memset(y->y.d,0,ECCGF2m_int<<2);
        return;
    }

    while(!ECCGF2m_BIGINT_bit_value(k,i))
        i--;
    c_x.x = x->x;
    c_x.y = x->y;

    memset(&(c_x.z),0,ECCGF2m_int<<2);
    c_x.z.d[0] = 1;
    c_u = c_x;

    while(i)
    {
        i--;
        ECCGF2m_point_double(&c_u, p, &c_u);
//printf("c_u...\n");
//    print_GF2m(&c_u.x);
//    print_GF2m(&c_u.y);
//    print_GF2m(&c_u.z);	
        if(ECCGF2m_BIGINT_bit_value(k,i))
        {
            ECCGF2m_point_add(&c_u, &c_x, p, &c_u);
        }
    }
    ECCGF2m_BIGINT_MAIAInv(&(c_u.z),p,&(c_u.z));
    ECCGF2m_BIGINT_MUL2(&(c_u.z),p,&(c_x.z));
    ECCGF2m_BIGINT_MUL(&(c_u.x), &(c_x.z), p, &(y->x));
    ECCGF2m_BIGINT_MUL(&(c_u.y), &(c_u.z), p, &(y->y));
    ECCGF2m_BIGINT_MUL(&(y->y), &(c_x.z), p, &(y->y));
}

// y*y+x*y=x*x*x+a*x*x+b;
U8 ECCGF2m_test_point(ECCGF2m_ECC_POINT *x,ECCGF2m_BIGINT *f)
{
    ECCGF2m_BIGINT *a,*b,t0,t1,tL;
    a = (ECCGF2m_BIGINT *)ECCGF2m_a;
    b = (ECCGF2m_BIGINT *)ECCGF2m_b;
    ECCGF2m_BIGINT_MUL2(&(x->y),f,&t0);
    ECCGF2m_BIGINT_MUL(&(x->x),&(x->y),f,&t1);
    ECCGF2m_BIGINT_add(&t0,&t1,&tL);

    ECCGF2m_BIGINT_MUL2(&(x->x),f,&t0);
    ECCGF2m_BIGINT_MUL(&(x->x),&t0,f,&t1);
    ECCGF2m_BIGINT_MUL(a,&t0,f,&t0);
    ECCGF2m_BIGINT_add(&t0,&t1,&t0);
    ECCGF2m_BIGINT_add(&t0,b,&t0);

     if(memcmp(tL.d,t0.d,ECCGF2m_int<<2))
         return 1;
     else
         return 0;
}


