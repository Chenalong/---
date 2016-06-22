
#include "mdlintf.h"


extern const U32 ECC_p[];
extern const U32 ECC_a[];
extern const U32 ECC_b[];
extern const U32 ECC_P0x[];
extern const U32 ECC_P0y[];
extern const U32 ECC_N[];

U8 judge_ECC_null(ECC_POINT *x1);

/***************************************************************/
/***************************************************************/
/**************** my big int handle library ********************/

/*  clear all bits in a large integer storage block.  */
// test passed 2.10
void int_null( BIGINT *a)
{
	memset(a, 0, sizeof(BIGINT));
}

/*  copy one BIGINT block to another  */
// test passed 2.10
void int_copy(BIGINT *a, BIGINT *b)
{
        *b=*a;
}

void bigint_to_char(BIGINT *a, char *c)
{
    int i;
    for(i=0;i<6;i++)
    {
        c[4*i] = (a->data[i+1])>>24;
        c[4*i+1] = (a->data[i+1])>>16;
        c[4*i+2] = (a->data[i+1])>>8;
        c[4*i+3] = (a->data[i+1]);
    }
}

void char_to_bigint(char *c, BIGINT *a)
{
    S32 i;
    a->data[0] = 0;
    for(i=0;i<6;i++)
    {
        a->data[i+1] = ((c[4*i]<<24)&0xff000000) | ((c[4*i+1]<<16)&0xff0000) | ((c[4*i+2]<<8)&0xff00) | (c[4*i+3]&0xff);

    }
}

void point_to_char(ECC_POINT *x,char *c)
{
    if(judge_ECC_null(x))
        c[0] = 0;
    else
        c[0] = 4;
    bigint_to_char(&(x->x),c+1);
    bigint_to_char(&(x->y),c+25);
}

void char_to_point(char *c, ECC_POINT *x)
{
    char_to_bigint(c+1, &(x->x));
    char_to_bigint(c+25, &(x->y));
}

U8 bitint_bit_value(BIGINT *a, U16 i)
{
        U32 x = i>>5;
        U32 y = 1<<(i&0x1f);
        if(a->data[ECC_MAXLONG-1-x]&y)
            return 1;
        else
            return 0;
}

S8 judge_bigint(BIGINT *a,BIGINT *b)
{

            if(a->data[0] > b->data[0])
                return 1;
            else if(a->data[0] < b->data[0])
              return -1;
            if(a->data[1] > b->data[1])
                return 1;
            else if(a->data[1] < b->data[1])
              return -1;
            if(a->data[2] > b->data[2])
                return 1;
            else if(a->data[2] < b->data[2])
              return -1;
            if(a->data[3] > b->data[3])
                return 1;
            else if(a->data[3] < b->data[3])
              return -1;
            if(a->data[4] > b->data[4])
                return 1;
            else if(a->data[4] < b->data[4])
              return -1;
            if(a->data[5] > b->data[5])
                return 1;
            else if(a->data[5] < b->data[5])
              return -1;
            if(a->data[6] > b->data[6])
                return 1;
            else if(a->data[6] < b->data[6])
              return -1;
        return 0;
}


U8 judge_bigint_null(BIGINT *a)
{

            if(a->data[0])
                return 0;
            if(a->data[1])
                return 0;
            if(a->data[2])
                return 0;
            if(a->data[3])
                return 0;
            if(a->data[4])
                return 0;
            if(a->data[5])
                return 0;
            if(a->data[6])
                return 0;
        return 1;
}

// c = a + b;
// test passed 2.10
void bigint_add(BIGINT *a,BIGINT *b,BIGINT *c)
{
        S32 i;
        U32 temp,temp1,carry=0;
        for(i=(ECC_MAXLONG-1); i>=0; i--)
        {
            temp = a->data[i];
            temp1 = c->data[i] = a->data[i] + b->data[i];
            c->data[i] = temp1 + carry;
            //if((carry==1&&temp1==0xffffffff)||c->data[i] < temp)
            if((carry==1&&temp1==0xffffffff)||temp1< temp)
                carry = 1;
            else
                carry = 0;
        }
}

// c = a - b; must a>=b
// test passed 2.10
void bigint_sub(BIGINT *a,BIGINT *b,BIGINT *c)
{
        BIGINT u;
        U32 temp,carry = 0;

                temp = b->data[6] + carry;
                u.data[6] = a->data[6] - temp;
                if(a->data[6]<temp)
                    carry = 1;
                else
                    carry = 0;
                temp = b->data[5] + carry;
                u.data[5] = a->data[5] - temp;
                if(((temp==0)&&(carry!=0))||a->data[5]<temp)
                    carry = 1;
                else
                    carry = 0;
                temp = b->data[4] + carry;
                u.data[4] = a->data[4] - temp;
                if(((temp==0)&&(carry!=0))||a->data[4]<temp)
                    carry = 1;
                else
                    carry = 0;
                temp = b->data[3] + carry;
                u.data[3] = a->data[3] - temp;
                if(((temp==0)&&(carry!=0))||a->data[3]<temp)
                    carry = 1;
                else
                    carry = 0;
                temp = b->data[2] + carry;
                u.data[2] = a->data[2] - temp;
                if(((temp==0)&&(carry!=0))||a->data[2]<temp)
                    carry = 1;
                else
                    carry = 0;
                temp = b->data[1] + carry;
                u.data[1] = a->data[1] - temp;
                if(((temp==0)&&(carry!=0))||a->data[1]<temp)
                    carry = 1;
                else
                    carry = 0;
                temp = b->data[0] + carry;
                u.data[0] = a->data[0] - temp;


        *c = u;
}

// test passed 2.10
void bigint_mod_add(BIGINT *a,BIGINT *b,BIGINT *p,BIGINT *c)
{
        bigint_add(a,b,c);
        if(judge_bigint(c,p)>=0)
        {
            bigint_sub(c,p,c);
        }
}

// test passed 2.10
void bigint_mod_sub(BIGINT *a,BIGINT *b,BIGINT *p,BIGINT *c)
{
       BIGINT x;
       if(judge_bigint(a,b)>=0)  // c = a-b
       {
           bigint_sub(a,b,c);
       }
       else                    // c = a+p-b
       {
           bigint_add(a,p,&x);
//print_ecc_str(&(x));		   
           bigint_sub(&x,b,c);
//print_ecc_str(c);		   
       }
}

void bigint_mod_mul_two(BIGINT *a,BIGINT *p,BIGINT *c)
{
    U32 temp,carry=0;
    S32 i;
    c->data[0] = 0;
    for(i=(ECC_MAXLONG-1); i>=0; i--)
    {
        temp = a->data[i];
        c->data[i] = (temp<<1) + carry;
        carry = (temp&0x80000000)>>31;
    }
    if(judge_bigint(c,p)>=0)
    {
        bigint_sub(c,p,c);
    }

}

void bigint_div_two(BIGINT *a,BIGINT *c)
{
    U32 temp, carry;
        temp = a->data[0];
        c->data[0] = temp>>1;
        carry = temp&1;
        temp = a->data[1];
        c->data[1] = temp>>1;
        if(carry)
            c->data[1] |= 0x80000000;
        carry = temp&1;
        temp = a->data[2];
        c->data[2] = temp>>1;
        if(carry)
            c->data[2] |= 0x80000000;
        carry = temp&1;
        temp = a->data[3];
        c->data[3] = temp>>1;
        if(carry)
            c->data[3] |= 0x80000000;
        carry = temp&1;
        temp = a->data[4];
        c->data[4] = temp>>1;
        if(carry)
            c->data[4] |= 0x80000000;
        carry = temp&1;
        temp = a->data[5];
        c->data[5] = temp>>1;
        if(carry)
            c->data[5] |= 0x80000000;
        carry = temp&1;
        temp = a->data[6];
        c->data[6] = temp>>1;
        if(carry)
            c->data[6] |= 0x80000000;
}

// c = (a+b)/2
void bigint_ab_two(BIGINT *a,BIGINT *b,BIGINT *c)
{
    U32 temp, carry, t0, t1;
    c->data[0] = 0;
    t0 = (a->data[1]&1);
    t1 = (b->data[1]&1);
    temp = (a->data[1]>>1) + (b->data[1]>>1);
    if(t0&&t1)
        temp++;
    c->data[1] = temp;
    carry = t0^t1;

    t0 = (a->data[2]&1);
    t1 = (b->data[2]&1);
    temp = (a->data[2]>>1) + (b->data[2]>>1);
    if(t0&&t1)
        temp++;
    if(carry)
    {
        if(temp&0x80000000)
            c->data[1]++;
        temp += 0x80000000;
    }
    c->data[2] = temp;
    carry = t0^t1;

    t0 = (a->data[3]&1);
    t1 = (b->data[3]&1);
    temp = (a->data[3]>>1) + (b->data[3]>>1);
    if(t0&&t1)
        temp++;
    if(carry)
    {
        if(temp&0x80000000)
            c->data[2]++;
        temp += 0x80000000;
    }
    c->data[3] = temp;
    carry = t0^t1;

    t0 = (a->data[4]&1);
    t1 = (b->data[4]&1);
    temp = (a->data[4]>>1) + (b->data[4]>>1);
    if(t0&&t1)
        temp++;
    if(carry)
    {
        if(temp&0x80000000)
            c->data[3]++;
        temp += 0x80000000;
    }
    c->data[4] = temp;
    carry = t0^t1;

    t0 = (a->data[5]&1);
    t1 = (b->data[5]&1);
    temp = (a->data[5]>>1) + (b->data[5]>>1);
    if(t0&&t1)
        temp++;
    if(carry)
    {
        if(temp&0x80000000)
            c->data[4]++;
        temp += 0x80000000;
    }
    c->data[5] = temp;
    carry = t0^t1;

    t0 = (a->data[6]&1);
    t1 = (b->data[6]&1);
    temp = (a->data[6]>>1) + (b->data[6]>>1);
    if(t0&&t1)
        temp++;
    if(carry)
    {
        if(temp&0x80000000)
            c->data[5]++;
        temp += 0x80000000;
    }
    c->data[6] = temp;

}

void bigint_mod_mul(BIGINT *a,BIGINT *b,BIGINT *p,BIGINT *c)
{  
   
    ABmodN_FOR_ECC(c->data+1,a->data+1,b->data+1);

    c->data[0] = 0;    
}

// 扩展欧几里德算法的变体 a*c = 1 mod(p),output c
// test passed 2.10
void bigint_EEA_algo(BIGINT *a,BIGINT *p,BIGINT *c)
{
         BIGINT u, v, A, C;
         u = *a;
         v = *p;
         S8 temp;
         // step 1
         memset(&A,0,sizeof(A));
         A.data[ECC_MAXLONG-1] = 1;
         memset(&C,0,sizeof(C));
         // step 2
EEA_algo1_1:
         // step 2.1
             while((u.data[ECC_MAXLONG-1]&1)==0)
             {
                 bigint_div_two(&u,&u);
                 if((A.data[ECC_MAXLONG-1]&1)==0)
                     bigint_div_two(&A,&A);
                 else
                 {
                     //bigint_add(&A,p,&A);
                     //bigint_div_two(&A,&A);
                     bigint_ab_two(&A,p,&A);
                 }
             }
         // step 2.2
             while((v.data[ECC_MAXLONG-1]&1)==0)
             {
                 bigint_div_two(&v,&v);
                 if((C.data[ECC_MAXLONG-1]&1)==0)
                     bigint_div_two(&C,&C);
                 else
                 {
                     //bigint_add(&C,p,&C);
                     //bigint_div_two(&C,&C);
                     bigint_ab_two(&C,p,&C);
                 }
             }
         // step 2.3
             temp = judge_bigint(&u,&v);
             if(temp==0)
                 goto EEA_algo1_2;
             if(temp>0)
             {
                  bigint_sub(&u,&v,&u);
                  bigint_mod_sub(&A,&C,p,&A);
             }
             else
             {
                  bigint_sub(&v,&u,&v);
                  bigint_mod_sub(&C,&A,p,&C);
             }
         goto EEA_algo1_1;
EEA_algo1_2:
         // step 3
         *c = C;
}

U32 bigint_div(BIGINT *a,BIGINT *b,BIGINT *c,U8 *flag)
{
    U32 i;
    S8 temp;
    temp = judge_bigint(a,b);
    i=0;
    *flag = 0;
    if(temp<0)
    {
        *c = *a;
    }
    else if(temp==0)
    {
        memset(c,0,sizeof(c));
        i = 1;
    }
    else
    {
        while(judge_bigint(a,b)>=0)
        {
            bigint_sub(a,b,a);
            i++;
            if(i>0xffff)
            {    *flag = 1;break;}
        }
        *c = *a;
    }
    return i;
}

void bigint_mod_mul_int(BIGINT *a,U32 x,BIGINT *p,BIGINT *b)
{
    *b = *a;
    for(;x>1;x--)
    {
       bigint_mod_add(b,a,p,b);
    }
}

void bigint_mod_val(BIGINT *in, BIGINT *p, BIGINT *out)
{
    if(out!=in)
        *out = *in;
    while(judge_bigint(out,p)>=0)
    {
        bigint_sub(in,p,out);
    }
}

/***************************************************************/
/***************************************************************/
/**************** my ECC function library **********************/

U8 judge_ECC_null(ECC_POINT *x1)
{
    if(!judge_bigint_null(&(x1->x)))
        return 0;
    else if(!judge_bigint_null(&(x1->y)))
        return 0;
    else
        return 0;
}

void ecc_curve_double(ECC_CURVE *c1,BIGINT *p,ECC_CURVE *c2)
{
#if 0
    BIGINT t1,t2,t3,t4,t5,temp;
    // t1=x1, t2=y1, t3=z1
    //t1 = c1->x;
    //t2 = c1->y;
    //t3 = c1->z;
    if(judge_bigint_null(&t2) || judge_bigint_null(&t3))
    {
        memset(c2,0,sizeof(ECC_CURVE));
        c2->x.data[ECC_MAXLONG-1] = 1;
        c2->y.data[ECC_MAXLONG-1] = 1;
        return;
    }
    // t4=a
    //memcpy(t4.data, &ECC_a, 4*ECC_MAXLONG);
    // t5=t3*t3
    bigint_mod_mul(&(c1->z), &(c1->z), p, &t5);
    // t5=t5*t5
    bigint_mod_mul(&t5, &t5, p, &t5);
    // t5=t4*t5
    bigint_mod_mul((BIGINT *)ECC_a, &t5, p, &t5);
    // t4=t1*t1
    bigint_mod_mul(&(c1->x), &(c1->x), p, &t4);
    // t4=3*t4
    bigint_mod_mul_two(&t4,p,&temp);
    bigint_mod_add(&temp,&t4,p,&t4);
    // t4=t4+t5
    bigint_mod_add(&t4,&t5,p,&t4);
    // t3=t2*t3
    bigint_mod_mul(&(c1->y), &(c1->z), p, &t3);
    // 6. t3=2*t3
    bigint_mod_mul_two(&t3,p,&(c2->z));
    // 7. t2=t2*t2
    bigint_mod_mul(&(c1->y), &(c1->y), p, &t2);
    // 8. t5=t1*t2
    bigint_mod_mul(&(c1->x), &t2, p, &t5);
    // 9. t5=4*t5
    bigint_mod_mul_two(&t5,p,&t5);
    bigint_mod_mul_two(&t5,p,&t5);
    // 10. t1=t4*t4
    bigint_mod_mul(&t4, &t4, p, &t1);
    // 11. t1=t1-2*t5
    bigint_mod_mul_two(&t5,p,&temp);
    bigint_mod_sub(&t1,&temp,p,&(c2->x));
    // 12. t2=t2*t2
    bigint_mod_mul(&t2, &t2, p, &t2);
    // 13. t2=8*t2
    bigint_mod_mul_two(&t2,p,&t2);
    bigint_mod_mul_two(&t2,p,&t2);
    bigint_mod_mul_two(&t2,p,&t2);
    // 14. t5=t5-t1
    bigint_mod_sub(&t5,&t1,p,&t5);
    // 16. t5=t4*t5
    bigint_mod_mul(&t5, &t4, p, &t5);
    // 17. t2=t5-t2
    bigint_mod_sub(&t5,&t2,p,&(c2->y));
    // 18. x2=t1,y2=t2,z2=t3
    //c2->x = t1;
    //c2->y = t2;
    //c2->z = t3;

#else
    BIGINT m,s,a,temp;
    //BIGINT *t_a;
    //get M
    bigint_mod_mul(&(c1->x), &(c1->x), p, &m);
    bigint_mod_add(&m,&m,p,&a);
    bigint_mod_add(&a,&m,p,&m);
    bigint_mod_mul(&(c1->z), &(c1->z), p, &s);
    bigint_mod_mul(&s, &s, p, &s);
    //memcpy(a.data, &ECC_a, 4*ECC_MAXLONG);
    //t_a = (BIGINT *)ECC_a;
    bigint_mod_mul((BIGINT *)ECC_a, &s, p, &s);
    bigint_mod_add(&m,&s,p,&m);
    // get Z2
    bigint_mod_mul(&(c1->y), &(c1->z), p, &(c2->z));
    bigint_mod_mul_two(&(c2->z),p,&(c2->z));
    // get S
    bigint_mod_mul(&(c1->y), &(c1->y), p, &temp);
    bigint_mod_mul(&temp, &(c1->x), p, &s);
    bigint_mod_mul_two(&s,p,&s);
    bigint_mod_mul_two(&s,p,&s);
    // get T
    bigint_mod_mul(&temp, &temp, p, &a);
    bigint_mod_mul_two(&a, p, &a);
    bigint_mod_mul_two(&a, p, &a);
    bigint_mod_mul_two(&a, p, &a);

    // get X2
    bigint_mod_mul(&m,&m,p,&temp);
    bigint_mod_mul_two(&s,p,&(c2->x));
    bigint_mod_sub(&temp,&(c2->x),p,&(c2->x));

    // get Y2
    bigint_mod_sub(&s,&(c2->x),p,&s);
    bigint_mod_mul(&m, &s, p, &m);
    bigint_mod_sub(&m,&a,p,&(c2->y));
#endif
}

// 注意输入点不能相同
void ecc_curve_add(ECC_CURVE *c0,ECC_CURVE *c1,BIGINT *p,ECC_CURVE *c2)
{
    BIGINT u0,u1,s0,s1,c_temp,c_temp1;
    // get U0
    bigint_mod_mul(&(c1->z), &(c1->z), p, &c_temp);
    bigint_mod_mul(&c_temp, &(c0->x), p, &u0);
    // get S0
    bigint_mod_mul(&(c0->y), &(c1->z), p, &s0);
    bigint_mod_mul(&s0, &c_temp, p, &s0);
    // get U1
    bigint_mod_mul(&(c0->z), &(c0->z), p, &c_temp);
    bigint_mod_mul(&c_temp, &(c1->x), p, &u1);
    // get S1
    bigint_mod_mul(&(c1->y), &(c0->z), p, &s1);
    bigint_mod_mul(&s1, &c_temp, p, &s1);

    // get W=u0
    bigint_mod_sub(&u0,&u1,p,&c_temp);
    // get R=s0
    bigint_mod_sub(&s0,&s1,p,&c_temp1);
    // get T=u1
    bigint_mod_add(&u0,&u1,p,&u1);
    u0 = c_temp;
    // get M=s1
    bigint_mod_add(&s0,&s1,p,&s1);
    s0 = c_temp1;

    // get Z2
    bigint_mod_mul(&(c0->z), &(c1->z), p, &c_temp);
    bigint_mod_mul(&c_temp, &u0, p, &(c2->z));
    // get X2
    bigint_mod_mul(&s0, &s0, p, &c_temp);
    bigint_mod_mul(&u1, &u0, p, &c_temp1);
    bigint_mod_mul(&c_temp1, &u0, p, &c_temp1);
    bigint_mod_sub(&c_temp,&c_temp1,p,&(c2->x));
    // get Y2
    bigint_mod_mul_two(&(c2->x), p, &c_temp);
    bigint_mod_sub(&c_temp1,&c_temp,p,&c_temp);
    bigint_mod_mul(&c_temp, &s0, p, &c_temp);
    bigint_mod_mul(&s1, &u0, p, &c_temp1);
    bigint_mod_mul(&c_temp1, &u0, p, &c_temp1);
    bigint_mod_mul(&c_temp1, &u0, p, &c_temp1);
    bigint_mod_sub(&c_temp,&c_temp1,p,&(c2->y));
    if(c2->y.data[ECC_MAXLONG-1]&1)
       bigint_add(&(c2->y),p,&(c2->y));
    bigint_div_two(&(c2->y),&(c2->y));
}

#if 1
void ecc_add_parameter(ECC_POINT *x1, ECC_POINT *x2, BIGINT *p, int i,BIGINT *c)
{
    BIGINT u,v;
    BIGINT *temp_a;
    if(i)
    {
        bigint_mod_sub(&(x2->y),&(x1->y),p,&u);
        bigint_mod_sub(&(x2->x),&(x1->x),p,&v);
        bigint_EEA_algo(&v,p,&v);
        bigint_mod_mul(&u,&v,p,c);
    }
    else
    {
        bigint_mod_mul(&(x1->x),&(x1->x),p,&u);
        bigint_mod_add(&u,&u,p,&v);
        bigint_mod_add(&u,&v,p,&u);
        //memcpy(v.data, &ECC_a, 4*ECC_MAXLONG);
        temp_a = (BIGINT *)ECC_a;
        bigint_mod_add(&u,temp_a,p,&u);
        bigint_mod_add(&(x1->y),&(x1->y),p,&v);
        bigint_EEA_algo(&v,p,&v);
        bigint_mod_mul(&u,&v,p,c);
    }
}

// x3=x1+x2
void ecc_point_add(ECC_POINT *x1, ECC_POINT *x2, BIGINT *p, ECC_POINT *x3)
{
    BIGINT u,v,x_out,y_out;
    int i;
    if(judge_ECC_null(x1))
    {
        x_out = x2->x;
        y_out = x2->y;
    }
    else if(judge_ECC_null(x2))
    {
        x_out = x1->x;
        y_out = x1->y;
    }
    else
    {
        i = 1;
        if(!judge_bigint(&(x1->x),&(x2->x)))
        {
            i = 0;
            bigint_mod_mul(&(x1->y),&(x2->y),p,&y_out);
//print_ecc_str(&y_out);
			
            if(judge_bigint_null(&y_out))
            {
                memset(x3,0,sizeof(x3));
                return;
            }
        }
    // get x3
//printf("get x3");    
        ecc_add_parameter(x1,x2,p,i,&v);
//print_ecc_str(&v);	
        bigint_mod_mul(&v,&v,p,&u);
//print_ecc_str(&u);		
        bigint_mod_sub(&u,&(x1->x),p,&u);
//print_ecc_str(&u);		
        bigint_mod_sub(&u,&(x2->x),p,&x_out);
//print_ecc_str(&x_out);		
    // get y3
//printf("get y3");
//print_ecc_str(&(x1->x));		
        bigint_mod_sub(&(x1->x),&x_out,p,&u);
//print_ecc_str(&u);	
        bigint_mod_mul(&u,&v,p,&u);
//print_ecc_str(&u);		
        bigint_mod_sub(&u,&(x1->y),p,&y_out);
//print_ecc_str(&y_out);		
		
    }
    x3->x = x_out;
    x3->y = y_out;
}
#endif

// y=k*x
void ecc_point_mul(BIGINT *k,ECC_POINT *x, ECC_POINT *y)
{
#if 1
    BIGINT *p;
    ECC_CURVE c_u,c_x;
    U16 i = ECC_NUMBITS-1;
    p = (BIGINT *)ECC_p;
HCMD_CALC((U32 *)(ECC_p+1),ECC_NUMBITS);
    if(judge_bigint_null(k))
    {
        memset(y,0,sizeof(y));
        return;
    }

    while(!bitint_bit_value(k,i))
        i--;
    c_x.x = x->x;
    c_x.y = x->y;
    
    memset(&(c_x.z),0,sizeof(BIGINT));
    c_x.z.data[ECC_MAXLONG-1] = 1;
    c_u = c_x;

    while(i)
    {
        i--;
        ecc_curve_double(&c_u, p, &c_u);
        if(bitint_bit_value(k,i))
        {
            ecc_curve_add(&c_u, &c_x, p, &c_u);
        }
    }    
    bigint_EEA_algo(&(c_u.z),p,&(c_u.z));
    bigint_mod_mul(&(c_u.x), &(c_u.z), p, &(y->x));
    bigint_mod_mul(&(y->x), &(c_u.z), p, &(y->x));

    bigint_mod_mul(&(c_u.y), &(c_u.z), p, &(y->y));
    bigint_mod_mul(&(y->y), &(c_u.z), p, &(y->y));
    bigint_mod_mul(&(y->y), &(c_u.z), p, &(y->y));

#else
    BIGINT *p;
    U32 i = ECC_NUMBITS-1;
    ECC_POINT u;
    //memcpy(p.data, &ECC_p, 4*ECC_MAXLONG);
    p = (BIGINT *)ECC_p;
HCMD_CALC(ECC_p+1,ECC_NUMBITS);
    if(judge_bigint_null(k))
    {
        memset(y,0,sizeof(y));
        return;
    }

    while(!bitint_bit_value(k,i))
        i--;
    u = *x;

    while(i)
    {
        i--;
//printf("a.");		
        ecc_point_add(&u, &u, p, &u);
//print_ecc_str(&u.x);
        if(bitint_bit_value(k,i))
        {
//printf("b.");        
            ecc_point_add(&u,x,p,&u);      
//print_ecc_str(&u.x);			
        }
    }
    *y = u;
#endif
}

// test point
// y*y = x*x*x + a*x+b;
U8 ECC_test_point(ECC_POINT *x,BIGINT *p)
{
     BIGINT u,v;
     u = x->x;
     HCMD_CALC((U32 *)(ECC_p+1),ECC_NUMBITS);
     bigint_mod_mul(&u,&u,p,&v);
     bigint_mod_mul(&v,&u,p,&v);
     bigint_mod_mul((BIGINT *)ECC_a,&u,p,&u);
     bigint_mod_add(&v,&u,p,&u);
     bigint_mod_add(&u,(BIGINT *)ECC_b,p,&u);
     bigint_mod_mul(&(x->y),&(x->y),p,&v);	 
     return(judge_bigint(&u,&v));
}


