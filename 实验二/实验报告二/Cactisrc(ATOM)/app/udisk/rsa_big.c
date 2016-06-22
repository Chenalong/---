/****************************************************************************

 ****************************************************************************/

/*

   RSA_big.c

*/

#include <instructions.h>
#include <bsp.h>
#include "bigint.h"
#include "REG1222.h"
#include "rsa_big.h"
void RSA_bigint_sub(RSA_BIGINT *a, RSA_BIGINT *b, RSA_BIGINT *c);
U32 RSA_calc_bit_length(RSA_BIGINT *a);
void RSA_bigint_mod(RSA_BIGINT *a, RSA_BIGINT *p,RSA_BIGINT *c);
void RSA_bigint_clear(RSA_BIGINT *a);

#define RSA_BIG_DEBUG 0

#define PTL 12
// 小素数表
const U16 PrimeTable[PTL] =
{       3,    5,    7,    11,   13,   17,   19,   23,   29,   31,
        37,   41,   	
};

extern long rngctl_data;    // defined in init.c
/*
const U32 FORELEN[32]={ 0x1,0x3,0x7,0xf,0x1f,0x3f,0x7f,0xff,
                                     0x1ff,0x3ff,0x7ff,0xfff,0x1fff,0x3fff,0x7fff,0xffff,
                                     0x1ffff,0x3ffff,0x7ffff,0xfffff,0x1fffff,0x3fffff,0x7fffff,0xffffff,
                                     0x1ffffff,0x3ffffff,0x7ffffff,0xfffffff,0x1fffffff,0x3fffffff,0x7fffffff,0xffffffff};
*/

/*****************************************************************
第一个参数是输入的数据，可能是明文，或密钥参数，
第二个参数是其8位字节长度，
第三个参数是转化后的大数结构
*******************************************************************/
int Data_to_Bigdata(U8 *in, U16 len, RSA_BIGINT *out)
{
	S32 i, j; 
	U32 temp, mod, Sum;
	//U8 i,j;

	i = len;
	out->len=0;
	RSA_bigint_clear(out);	/* 先把大数清零 */
	out->len=len>>2;		/* 字节（8位）长度转换为字（32位）长度 */
	if(len%4!=0)
		out->len=out->len+1;/* 考虑字节数不是4的整倍数时的情况 */
	Sum=0;

	j=0;
	for(i=len-1; i>=0; i--)	/* 从低位到高位取来字节，每凑够4字节赋值给大数的一个32位单元 */
	{		
		mod=j%4;
		temp=in[i];
		Sum=Sum+(temp<<(8*mod));
		if(mod==3)
		{
			out->data[j/4]=Sum;
			Sum=0;
		}
	j++;
	}
	if(mod!=3)
		out->data[(j-1)/4]=Sum;	/* 最后不够4字节部分赋值给大数的一个32位单元 */

	return 0;
}

/* 下面是RSA部分用到的函数 */
/****************************** RSA_bigint_clear ************************
Input   : a--- pointer of RSA_BIGINT type
Output : none
Return : none
Function: clear no use data of RSA_BIGINT type
**********************************************************************/
void RSA_bigint_clear(RSA_BIGINT *a)
{
    S32 i;
    for(i=(RSA_MAXLONG-1);i>=(S32)(a->len);i--)
	   a->data[i] = 0;
    while(a->len &&(!a->data[a->len-1])) a->len--;
}
/****************************** RSA_bigint_compare_value ***************
Input   : a,b--- pointer of RSA_BIGINT type
Output : none
Return : 1(a>b); 0(a==b); -1(a<b);
Function: compare two data of RSA_BIGINT type
**********************************************************************/
S8 RSA_bigint_compare_value(RSA_BIGINT *a, RSA_BIGINT *b)
{
         int i;
	  if(a->len>b->len)
	  	return 1;
	  if(a->len<b->len)
	  	return -1;
	  	  	
	  for(i=(a->len-1);i>=0;i--)	   	
		{
             	if(a->data[i]>b->data[i])
			return 1;
		if(a->data[i]<b->data[i])
			return -1;
		}
	  return 0;
	  	
}

/****************************** RSA_bigint_add *************************
Input   : a,b--- pointer of RSA_BIGINT type
Output : c --- pointer of RSA_BIGINT type
Return : none
Function: c = a+b; unsigned add;
**********************************************************************/
void RSA_bigint_add(RSA_BIGINT *a, RSA_BIGINT *b, RSA_BIGINT *c)
{
        int i;
	 U32 temp0,temp1,carry,len;
	 carry=0;
	 if(a->len >= b->len)
	 	len = a->len;
	 else
	 	len = b->len;
	 
	 for(i=0;i<len;i++)
	 	{
              temp1=a->data[i];
		temp0=a->data[i]+b->data[i];
              c->data[i]=temp0+carry;
		if(temp0<temp1 ||c->data[i]<temp0)
                  carry = 1;
              else
                  carry = 0;
		}
	 if(carry)
	 	{
	 	c->data[i] = 1;
		c->len = len+1;
	 	}
	 else
	 	c->len = len;
	 RSA_bigint_clear(c);

}

/****************************** RSA_bigint_sign_add *********************
Input   : a,b--- pointer of RSA_BIGINT type
             flag_a--- the sign of a
             flag_b--- the sign of b
Output : c --- pointer of RSA_BIGINT type
             flag_c--- the sign of c
Return : none
Function: c = a+b;signed add
**********************************************************************/
void RSA_bigint_sign_add(RSA_BIGINT *a, U8 flag_a,RSA_BIGINT *b, U8 flag_b, RSA_BIGINT *c, U8 *flag_c)
{
    if((flag_a+flag_b)!=1)
    {
        RSA_bigint_add(a,b,c);
        *flag_c = flag_a;
    }
    else
    {
        if(RSA_bigint_compare_value(a,b)>=0)
        {
            RSA_bigint_sub(a,b,c);
            *flag_c = flag_a;
        }
        else
        {
            RSA_bigint_sub(b,a,c);
            *flag_c = flag_b;
        }
    }
}

/****************************** RSA_bigint_sub *************************
Input   : a,b--- pointer of RSA_BIGINT type
Output : c --- pointer of RSA_BIGINT type
Return : none
Function: c = a-b; unsigned sub; must ensure a>=b 
**********************************************************************/
void RSA_bigint_sub(RSA_BIGINT *a, RSA_BIGINT *b, RSA_BIGINT *c)
{
        int i;
	 U32 temp0,temp1,carry,len;
	 carry=0;
	 len = a->len;
	 for(i=0;i<len;i++)
	 	{
              temp1=a->data[i];
		temp0=a->data[i] -b->data[i];
              c->data[i]=temp0 -carry;
		if(temp0>temp1 ||c->data[i]>temp0)
                  carry = 1;
              else
                  carry = 0;
		}
        c->len = len;
        RSA_bigint_clear(c);

}

/****************************** RSA_bigint_sign_sub *********************
Input   : a,b--- pointer of RSA_BIGINT type
             flag_a--- the sign of a
             flag_b--- the sign of b
Output : c --- pointer of RSA_BIGINT type
             flag_c--- the sign of c
Return : none
Function: c = a-b;signed sub
**********************************************************************/
void RSA_bigint_sign_sub(RSA_BIGINT *a, U8 flag_a,RSA_BIGINT *b, U8 flag_b, RSA_BIGINT *c, U8 *flag_c)
{
    if((flag_a+flag_b)==1)
    {
        RSA_bigint_add(a,b,c);
        *flag_c = flag_a;
    }
		else
    {
        if(RSA_bigint_compare_value(a,b)>=0)
        {
            RSA_bigint_sub(a,b,c);
            *flag_c = flag_a;
        }
        else
        {
            RSA_bigint_sub(b,a,c);
            if(flag_b)
                *flag_c =0;
            else
                *flag_c =1;
        }
    }
}

/****************************** RSA_bigint_mod_add ********************
Input   : a,b,p--- pointer of RSA_BIGINT type
Output : c --- pointer of RSA_BIGINT type
Return : none
Function: c = (a+b) mod (p); 
**********************************************************************/
void RSA_bigint_mod_add(RSA_BIGINT *a, RSA_BIGINT *b, RSA_BIGINT *p, RSA_BIGINT *c)
{
	RSA_bigint_add(a, b, c);
	if(RSA_bigint_compare_value(c, p)>=0)
		RSA_bigint_sub(c, p, c);

}

/****************************** RSA_bigint_mod_sub ********************
Input   : a,b,p--- pointer of RSA_BIGINT type
Output : c --- pointer of RSA_BIGINT type
Return : none
Function: c = (a-b) mod (p); 
**********************************************************************/
void RSA_bigint_mod_sub(RSA_BIGINT *a, RSA_BIGINT *b, RSA_BIGINT *p, RSA_BIGINT *c)
{
       RSA_BIGINT t;
	if(RSA_bigint_compare_value(a, b)<0)
	       {
	        RSA_bigint_add(a, p, &t);
		 RSA_bigint_sub(&t, b, c);
		}
	else
		RSA_bigint_sub(a, b, c);

}

/****************************** RSA_bigint_div_two **********************
Input   : a,--- pointer of RSA_BIGINT type
Output : c --- pointer of RSA_BIGINT type
Return : none
Function: c = a/2; must ensure a is an even 
**********************************************************************/
void RSA_bigint_div_two(RSA_BIGINT *a, RSA_BIGINT *c)
{
        int i;
	 U32 temp0,temp1,carry,len;
	 carry=0;
	 len = a->len;
	 for(i=(len-1);i>=0;i--)
	 	{
              temp0=a->data[i];
		c->data[i]=a->data[i]>>1;
		if(carry)
		    c->data[i] |= 0x80000000;
		carry=temp0&1;
		}
	 if(c->data[len-1])
	 	c->len = len;
	 else
	 	c->len = len-1;
         //RSA_bigint_clear(c);
}

/****************************** RSA_bitint_bit_value *********************
Input   : a--- pointer of RSA_BIGINT type
Output : i --- bit number
Return : 1; 0;
Function: check the bit value 
**********************************************************************/
U8 RSA_bitint_bit_value(RSA_BIGINT *a,U32 i)
{
        U32 x = i>>5;
        U32 y = 1<<(i&0x1f);
        if(a->data[x]&y)
            return 1;
        else
            return 0;

}

/****************************** RSA_bigint_mod_mul_two *****************
Input   : a,p--- pointer of RSA_BIGINT type
Output : c --- pointer of RSA_BIGINT type
Return : none
Function: c = (a*2) mod (p); 
**********************************************************************/
void RSA_bigint_mod_mul_two(RSA_BIGINT *a, RSA_BIGINT *p,RSA_BIGINT *c)
{
    U32 temp,len,carry;
    S32 i;
    carry=0;
    len = a->len;
    c->data[RSA_MAXLONG-1] = 0;
    for(i=0; i<len; i++)
    {
        temp = a->data[i];
        c->data[i] = (temp<<1) + carry;
        carry = (temp&0x80000000)>>31;
    }
    if(carry)
	{
	c->data[i] = 1;
	c->len = len+1;
    	}
    else
	c->len = len;

    RSA_bigint_clear(c);

    if(RSA_bigint_compare_value(c,p)>=0)
    {
        RSA_bigint_sub(c,p,c);
    }

}

/****************************** RSA_bigint_mod_mul ********************
Input   : a,b,p--- pointer of RSA_BIGINT type
Output : c --- pointer of RSA_BIGINT type
Return : none
Function: c = (a*b) mod (p); 
**********************************************************************/
#if 1
void RSA_bigint_mod_mul(RSA_BIGINT *a,RSA_BIGINT *b,RSA_BIGINT *p,RSA_BIGINT *c)
{
     U32 i ,len;
     RSA_BIGINT s;
     
          ABmodN_little(c->data,a->data,b->data,p->data,RSA_calc_bit_length(p));
          i = p->len - 1;
          while(i<RSA_NUMWORD)
    	      {
              if(c->data[i])
			  break;	
		i--;		
    	      }
         if(i<RSA_NUMWORD)	
             c->len = i+1;
         else
             c->len = 0;
         RSA_bigint_clear(c);
  	  
}

#else
void RSA_bigint_mod_mul(RSA_BIGINT *a,RSA_BIGINT *b,RSA_BIGINT *p,RSA_BIGINT *c)
{
     U32 i ,len;
     RSA_BIGINT s;
              if(a->len >= b->len)
			  len = a->len;
              else
	                len = b->len;
              i = (len<<5) -1; 
	 
              if(RSA_bitint_bit_value(b,i))
                       s = *a;
              else
                       memset(&s,0,sizeof(s));
              while(i)
			  {
                       i--;
                       RSA_bigint_mod_mul_two(&s,p,&s);
                       if(RSA_bitint_bit_value(b,i))
				   RSA_bigint_mod_add(&s,a,p,&s);         
                       }
             *c = s;  	      
}
#endif

void RSA_bigint_mod_mul2(RSA_BIGINT *a,RSA_BIGINT *b,RSA_BIGINT *p,RSA_BIGINT *c)
{
     U32 i ,len;
     RSA_BIGINT s;

              if(a->len >= b->len)
			  len = a->len;
              else
	                len = b->len;
              i = (len<<5) -1;

              if(RSA_bitint_bit_value(b,i))
                       s = *a;
              else
                       memset(&s,0,sizeof(s));
              while(i)
			  {
                       i--;
                       RSA_bigint_mod_mul_two(&s,p,&s);
                       if(RSA_bitint_bit_value(b,i))
				   RSA_bigint_mod_add(&s,a,p,&s);
                       }
             *c = s;

}

/****************************** RSA_bigint_inverse **********************
Input   : a,p--- pointer of RSA_BIGINT type
Output : c --- pointer of RSA_BIGINT type
Return : none
Function:  (a*c) mod (p) = 1; get the c 
**********************************************************************/
U8 RSA_bigint_inverse(RSA_BIGINT *a,RSA_BIGINT *p,RSA_BIGINT *c)
{
#if 1
         RSA_BIGINT u, v, A, B, C, D;
         S8 temp;
//	  U32 count=0;
         // 假设：0 标志正值； 1 标志负值
         U8 flag_A,flag_B,flag_C,flag_D;
//printf("enter RSA_bigint_inverse...\n");
//print_bigint(a);
//print_bigint(p);
//print_long(&u);
//print_long(&D);
//print_long(&flag_D);
         flag_A=flag_B=flag_C=flag_D=0;
         u = *p;
         v = *a;
         memset(&A,0,sizeof(A));
         B=C=D=A;
         A.data[0] = D.data[0] = 1;
         A.len = D.len = 1;
RSA_EEA_algo_1:

//printf("count=0x%x...\n",++count);
	  
         while((u.data[0]&1)==0)
         {
             RSA_bigint_div_two(&u,&u);
//printf("W.0\n");
             if((A.data[0]&1) || (B.data[0]&1))
             {                 
                 RSA_bigint_sign_add(&A,flag_A,a,0,&A,&flag_A);                
//printf("W.1\n");
                 RSA_bigint_sign_sub(&B,flag_B,p,0,&B,&flag_B);
//printf("W.2\n");
             }
             RSA_bigint_div_two(&A,&A);
//printf("W.3\n");
             RSA_bigint_div_two(&B,&B);
         }
//printf("A.\n");		 
         while((v.data[0]&1)==0)
         {
             RSA_bigint_div_two(&v,&v);
//printf("A.0\n");
             if((C.data[0]&1) || (D.data[0]&1))
             {                
                 RSA_bigint_sign_add(&C,flag_C,a,0,&C,&flag_C);                 
//printf("A.1\n");
		   RSA_bigint_sign_sub(&D,flag_D,p,0,&D,&flag_D);
//printf("A.2\n");
             }
             RSA_bigint_div_two(&C,&C);
//printf("A.3\n");
             RSA_bigint_div_two(&D,&D);
         }
//printf("B.\n");
         temp = RSA_bigint_compare_value(&u,&v);
//printf("B.0\n");
         if(temp==0)
             goto RSA_EEA_algo_2;
         if(temp>0)
         {
              RSA_bigint_sub(&u,&v,&u);              
//printf("B.1\n");
              RSA_bigint_sign_sub(&A,flag_A,&C,flag_C,&A,&flag_A);              
//printf("B.2\n");
              RSA_bigint_sign_sub(&B,flag_B,&D,flag_D,&B,&flag_B);
//printf("B.3\n");
         }
         else
         {
              RSA_bigint_sub(&v,&u,&v);              
//printf("B.4\n");
              RSA_bigint_sign_sub(&C,flag_C,&A,flag_A,&C,&flag_C);              
//printf("B.5\n");
              RSA_bigint_sign_sub(&D,flag_D,&B,flag_B,&D,&flag_D);
//printf("B.6\n");
         }
         goto RSA_EEA_algo_1;
RSA_EEA_algo_2:

         while(RSA_bigint_compare_value(&D,p)>0)
            RSA_bigint_sub(&D,p,&D);
         if(flag_D)
            RSA_bigint_sub(p,&D,&D);
         *c = D;
	  if(v.len!=1 || v.data[0]!=1)
	  	return 1;
	  return 0;

#else
         RSA_BIGINT u, v, A, C;
         S8 temp;
         u = *a;
         v = *p;
         // step 1
         memset(&A,0,sizeof(A));
         A.data[0] = 1;
	  A.len = 1;
         memset(&C,0,sizeof(C));

         // step 2
RSA_EEA_algo1_1:
         // step 2.1
             while((u.data[0]&1)==0)
             {
                 RSA_bigint_div_two(&u,&u);
                 if((A.data[0]&1)==0)
                     RSA_bigint_div_two(&A,&A);
                 else
                 {
                     RSA_bigint_add(&A,p,&A);
                     RSA_bigint_div_two(&A,&A);
                     //bigint_ab_two(&A,p,&A);
                 }
             }
         // step 2.2
             while((v.data[0]&1)==0)
             {
                 RSA_bigint_div_two(&v,&v);
                 if((C.data[0]&1)==0)
                     RSA_bigint_div_two(&C,&C);
                 else
                 {
                     RSA_bigint_add(&C,p,&C);
                     RSA_bigint_div_two(&C,&C);
                     //bigint_ab_two(&C,p,&C);
                 }
             }
         // step 2.3
             temp = RSA_bigint_compare_value(&u,&v);
             if(temp==0)
             {
                 RSA_bigint_sub(&u,&v,&u);
                 RSA_bigint_mod_sub(&A,&C,p,&A);
                 goto RSA_EEA_algo1_2;
             }
             if(temp>0)
             {
                  RSA_bigint_sub(&u,&v,&u);
                  RSA_bigint_mod_sub(&A,&C,p,&A);
             }
             else
             {
                  RSA_bigint_sub(&v,&u,&v);
                  RSA_bigint_mod_sub(&C,&A,p,&C);
             }
         goto RSA_EEA_algo1_1;
RSA_EEA_algo1_2:
         // step 3
         *c = C;
#endif
}

/****************************** RSA_bigint_mul_U32 *********************
Input   : a--- pointer of RSA_BIGINT type
             data--- unsigned long data
Output : c --- pointer of RSA_BIGINT type
Return : none
Function:  c = a*data; must ensure (a->len)<RSA_MAXLONG 
**********************************************************************/ 
void RSA_bigint_mul_U32(RSA_BIGINT *a,U32 data,RSA_BIGINT *c)
{
          U32 i,len,carry;
	   U64 temp64;
	   len = a->len;
	   carry=0;
	   for(i=0;i<len;i++)
	   	{
              temp64 = (U64)(a->data[i])*(U64)data;
              temp64 += (U64)carry;
		c->data[i]=(U32)temp64;
		carry = (U32)(temp64>>32);

	   	}
           if(carry)
           {
	   c->data[i] = carry;
	   c->len = len+1;
           }
           else
           c->len = len;
           RSA_bigint_clear(c);

}

/****************************** RSA_bigint_shl **************************
Input   : a--- pointer of RSA_BIGINT type
             data--- unsigned long data
Output : none
Return : none
Function:  shift a left one double word; 
**********************************************************************/   
void RSA_bigint_shl(RSA_BIGINT *a,U32 data)
{
          U32 i;
	   if(data==0)
	   	return;
	   for(i=(a->len+data-1);i>=data;i--)
	   	{
              a->data[i] = a->data[i-data];
	   	}
	   for(i=0;i<data;i++)
	       a->data[i]=0;
	   a->len += data;
}

/****************************** RSA_bigint_shr **************************
Input   : a--- pointer of RSA_BIGINT type
             data--- unsigned long data
Output : none
Return : none
Function:  shift a right one double word; 
**********************************************************************/  
void RSA_bigint_shr(RSA_BIGINT *a,U32 data)
{
          U32 i,left;
	   if(data==0 || (data>a->len))
	   	return;
           left = a->len - data;
	   for(i=0;i<left;i++)
	   	{
              a->data[i] = a->data[i+data];
	   	}
	   a->len = left;
           RSA_bigint_clear(a);
}

/****************************** RSA_bigint_mul *************************
Input   : a,b--- pointer of RSA_BIGINT type
Output : c --- pointer of RSA_BIGINT type
Return : none
Function: c = a*b; unsigned mul;
**********************************************************************/
U8 RSA_bigint_mul(RSA_BIGINT *a,RSA_BIGINT *b,RSA_BIGINT *c)
{
    U32 lena,lenb,i;
#if 1
       lena = a->len;
	   lenb = b->len;
	   if(lena > (RSA_NUMWORD/2) || lenb > (RSA_NUMWORD/2))
			return 1;
	   ABmul_hardware(c->data,a->data,RSA_calc_bit_length(a),b->data,RSA_calc_bit_length(b));

          i = (a->len+b->len) - 1;
          while(i<RSA_NUMWORD)
    	      {
              if(c->data[i])
			  break;	
		      i--;		
    	      }
         if(i<RSA_NUMWORD)	
             c->len = i+1;
         else
             c->len = 0;
         RSA_bigint_clear(c);                             		
#else
          
	   U64 temp64;
	   RSA_BIGINT s,t;
	   lena = a->len;
	   lenb = b->len;

	   if(lena > (RSA_NUMWORD/2) || lenb > (RSA_NUMWORD/2))
			return 1;
	   
	   memset(&s,0,sizeof(s));
          
	   for(i=0;i<lenb;i++)
	   	{
	   	RSA_bigint_mul_U32(a,b->data[i],&t);
                RSA_bigint_shl(&t,i);
		RSA_bigint_add(&s,&t,&s);

	   	}
           *c = s;
#endif           
          return 0;
}

/****************************** RSA_bigint_judge_div ********************
Input   : a--- pointer of RSA_BIGINT type
             data--- unsigned long data
Output : none
Return : 1(a/data!=0); 0(a/data==0);
Function:  judge a/data; 
**********************************************************************/ 
U8 RSA_bigint_judge_div(RSA_BIGINT *a,U32 data)
{
          S32 i;
          U32 len,carry;
	  U64 temp64;
          if(data==0)
              return 1;
	  len = a->len;
	  carry = 0;
	  for(i=(len-1);i>=0;i--)
	  	{
                temp64 = ((U64)carry<<32);
	  	temp64 += (U64)(a->data[i]);
		//temp64 /= data;
		carry = (U32)(temp64%(U64)data);
		//carry = calc_data_div(temp64,data);
	  	}
	  if(carry)
	  	return 1;
	  else
	  	return 0;

}

/////////////////////////////////////////////////////////////
////////    RSA FUNCTION CODE     ///////
/////////////////////////////////////////////////////////////

/****************************** check_big_value **************************
Input   : data--- pointer of U32 type
             len--- length of data pointer
Output : none
Return : 0(data==0); 1(data!=0);
Function:  check data =0 or not; 
**********************************************************************/ 
U8 check_big_value(U32 *data,U32 len)
{
    U32 i;
    if(data[0]!=1)
      return 1;
    for(i=1;i<len;i++)
	{
		if(data[i])
		return 1;
	}
    return 0;	
}

void  RSA_bigint_div_table(RSA_BIGINT *a,U32 *buf)
{
          S32 i,ii;
          U32 len,carry;
	  U64 temp64,data;
          for(ii=0;ii<12;ii++)
          {
          data =  (U64)PrimeTable[ii];
	  len = a->len;
	  carry = 0;
	  for(i=(len-1);i>0;i--)
	  	{
                temp64 = ((U64)carry<<32);
	  	temp64 += (U64)(a->data[i]);
		carry = (U32)(temp64%data);
	  	}
          buf[ii] = carry;
          }
}

U8 RSA_bigint_judge_div2(RSA_BIGINT *a,U32 *buf,U32 ii)
{
         U64 temp64;
         temp64 = (U64)buf[ii]<<32;
         temp64 += (U64)(a->data[0]);
         if(temp64%(U64)PrimeTable[ii])
	  	return 1;
	  else
	  	return 0;
}

/****************************** RSA_key_test_func *********************
Input   : e,d,n--- pointer of RSA_BIGINT type
Output : none
Return : 0(yes); 1(no);
Function:  check the RSA key; 
**********************************************************************/ 
U8 RSA_key_test_func(RSA_BIGINT *u,RSA_BIGINT *e,RSA_BIGINT *dp,RSA_BIGINT *dq,RSA_BIGINT *p,RSA_BIGINT *q,RSA_BIGINT *n)
{
	RSA_BIGINT datain,dataout;
	U32 i;
       U8 *ptr;
	datain.len = dataout.len = (p->len)<<1;
       ptr = (U8 *)&(dataout.data[0]);
	for(i=0;i<(datain.len<<2);i++)
		ptr[i] = i;
	Data_to_Bigdata(ptr,datain.len<<2,&datain);
	
	//memset(datain.data,1,(datain.len)<<2);
       //RSA_bigint_clear(&datain);
       //RSA_bigint_mod2(in,p,&a);
	RSA_bigint_mod_e_CRT(&datain,u,e,e,p,q,&dataout);
	//RSA_bigint_mod_e(&datain,e,n,&dataout);
	RSA_bigint_mod_e_CRT(&dataout,u,dp,dq,p,q,&dataout);   
	if(memcmp(datain.data,dataout.data,(datain.len)<<2))
	{    		
        return 1;
	}
	return 0;	
	
}

/****************************** Get_Prime_new **************************
Input   : prime--- pointer of RSA_BIGINT type
             len--- length of prime
Output : none
Return : none;
Function:  get a prime; 
**********************************************************************/ 
#if 1
void Get_Prime_new(RSA_BIGINT *prime,U32 length)
{
         S32 i;
	  U32 int_len,my_buf[20];
	  RSA_BIGINT temp;
	  U32 datain[RSA_NUMWORD/2+1], dataout[RSA_NUMWORD/2+1];
	  	  
	  int_len = length>>5;
	  memset(datain,0,sizeof(datain));

        // get random big int number
        for(i=0; i<int_len; i++)
		prime->data[i] = Get_Rand();                
	 prime->data[0] |= 1;
        prime->data[int_len-1] |= 0xc0000000;
	 prime->len = int_len;
	 RSA_bigint_clear(prime);
	 
	 RSA_bigint_div_table(prime,my_buf);

get_next_bigint:
	
	prime->data[0] += 2;
       					
	 // use little prime to check the random big int number
	 for(i=0; i<12; i++)
	 	{
	       if(RSA_bigint_judge_div2(prime,my_buf,i)==0)
	       	{
			goto get_next_bigint;
	       	}
	 	}
	// use stronge to check the random big int number 
	 temp = *prime;
	 temp.data[0] -= 1;
        for(datain[0]=2;datain[0]<6;datain[0]++)
        	{
        	if(datain[0]!=4)
        		{
              	AEmodN_for_RSA(dataout,datain,temp.data,length,prime->data,length);
              	if(check_big_value(dataout,prime->len))
              		{
		       	goto get_next_bigint;
              		}
        		}
        	}

}
#else
void Get_Prime_new(RSA_BIGINT *prime,U32 length)
{
      S32 i;
	  U32 int_len;
	  RSA_BIGINT temp;
	  U32 datain[RSA_NUMWORD/2+1], dataout[RSA_NUMWORD/2+1];
	  
	  int_len = length>>5;
	  memset(datain,0,sizeof(datain));

        // get random big int number
        for(i=0; i<int_len; i++)
		prime->data[i] = Get_Rand();
	 prime->data[0] |= 1;
        prime->data[int_len-1] |= 0xc0000000;
	 prime->len = int_len;
	 RSA_bigint_clear(prime);

get_next_bigint:
	
	prime->data[0] += 2;
       					
	 // use little prime to check the random big int number
	 for(i=0; i<PTL; i++)	 
	 	{
	       if(RSA_bigint_judge_div(prime,PrimeTable[i])==0)
	       	{
			goto get_next_bigint;
					}
	 	}

	// use stronge to check the random big int number 
	 temp = *prime;
	 temp.data[0] -= 1;
        for(datain[0]=2;datain[0]<6;datain[0]++)
        	{
              AEmodN_for_RSA(dataout,datain,temp.data,length,prime->data,length);
              if(check_big_value(dataout,prime->len))
              	{
		       goto get_next_bigint;
              	}
        	}

}
#endif
/*************************** RSA_Get_Key_For_CRT **********************
Input   : u,e,dp,dq,n,p,q,n--- pointer of RSA_BIGINT type
             nlen--- bit length of n
             elen--- bit length of e
Output : none
Return : none;
Function:  get RSA key which is used for CRT; 
**********************************************************************/ 
#if 0
void RSA_Get_Key_For_CRT(RSA_BIGINT *u,RSA_BIGINT *e,RSA_BIGINT *dp,RSA_BIGINT *dq,RSA_BIGINT *p,RSA_BIGINT *q,RSA_BIGINT *n,U32 nlen, U32 elen)
{
	 U32 tmp,i,temp;
        nlen >>= 1;	
 RSA_Get_Key_again:
        // get strong prime P, Q
start_timer0();
        Get_Prime_new(p,nlen);
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step 0 time count is 0x%x,\n",temp);		
start_timer0();
	 Get_Prime_new(q,nlen);
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step 1 time count is 0x%x,\n",temp);

if(RSA_bigint_compare_value(p,q)<0)
{
        *u = *p;
        *p = *q;
        *q = *u;
}

         // P=P-1; Q=Q-1;
         p->data[0] --;
	  q->data[0] --;
#if 1
	  e->len=1;
         switch(elen)
        	{      
              case 2:  e->data[0] = 3; break;
              case 5:  e->data[0] = 17; break;
              case 17:e->data[0] = 65537; break;
              default:
              e->len = elen>>5;
		if(elen&0x1f)
			(e->len)++;
		for(i=0; i<e->len; i++)
		        e->data[i] = Get_Rand();             
              e->data[0] |= 1;
		tmp = (elen-1)&0x1f;	  
		for(i=31;i>tmp;i--)
                      e->data[e->len -1]  &= (~(1<<i));
		e->data[e->len -1] |= (1<<tmp);
              break;
              }
	RSA_bigint_clear(e);
#else
         memset(e,0,sizeof(RSA_BIGINT));
	  e->len=1;
	  e->data[0] = 65537;
#endif
         // get (P-1)*(Q-1)
start_timer0();         
	  RSA_bigint_mul(p, q, dq);
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step 2 time count is 0x%x,\n",temp);
	  // get d
start_timer0();	  
	if(RSA_bigint_inverse(e,dq,dp))  //dp是私钥	  	
		goto RSA_Get_Key_again;
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step 3 time count is 0x%x,\n",temp);	  		
	  // get dq = d mod q
start_timer0();	  
         RSA_bigint_mod(dp,q,dq);
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step 4 time count is 0x%x,\n",temp);		 
         // get dp = d mod p
start_timer0();    
         RSA_bigint_mod(dp,p,dp);
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step 5 time count is 0x%x,\n",temp);
         // P=P+1; Q=Q+1;
	  p->data[0] ++;
	  q->data[0] ++;
start_timer0();
	  // get u; u*q=1 mod p;
	  if(RSA_bigint_inverse(q,p,u))
               goto RSA_Get_Key_again;
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step 6 time count is 0x%x,\n",temp);	  
	 // get n=P*Q
start_timer0();	 
        RSA_bigint_mul(p, q, n);
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step 7 time count is 0x%x,\n",temp);		
start_timer0();	 
	  // check the key
         if(RSA_key_test_func(u,e,dp,dq,p,q,n))
		 goto RSA_Get_Key_again;
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step 8 time count is 0x%x,\n",temp);		 
	    	  
}
#else
void RSA_Get_Key_For_CRT(RSA_BIGINT *u,RSA_BIGINT *e,RSA_BIGINT *dp,RSA_BIGINT *dq,RSA_BIGINT *p,RSA_BIGINT *q,RSA_BIGINT *n,U32 nlen, U32 elen)
{
	 U32 tmp,i;
//printf("0...\n");	 
        nlen >>= 1;	
 RSA_Get_Key_again:
        // get strong prime P, Q
       Get_Prime_new(p,nlen);
//printf("1...\n");
	 Get_Prime_new(q,nlen);
//printf("2...\n");
if(RSA_bigint_compare_value(p,q)<0)
{
        *u = *p;
        *p = *q;
        *q = *u;
}

         // P=P-1; Q=Q-1;
         p->data[0] --;
	  q->data[0] --;
#if 1
	  e->len=1;
         switch(elen)
        	{      
              case 2:  e->data[0] = 3; break;
              case 5:  e->data[0] = 17; break;
              case 17:e->data[0] = 65537; break;
              default:
              e->len = elen>>5;
		if(elen&0x1f)
			(e->len)++;
		for(i=0; i<e->len; i++)
		        e->data[i] = Get_Rand();             
              e->data[0] |= 1;
		tmp = (elen-1)&0x1f;	  
		for(i=31;i>tmp;i--)
                      e->data[e->len -1]  &= (~(1<<i));
		e->data[e->len -1] |= (1<<tmp);
              break;
              }
	RSA_bigint_clear(e);
//printf("3...\n");	
#else
         memset(e,0,sizeof(RSA_BIGINT));
	  e->len=1;
	  e->data[0] = 65537;
#endif
         // get (P-1)*(Q-1)
	  RSA_bigint_mul(p, q, dq);
//printf("4...\n");
	  // get d
	if(RSA_bigint_inverse(e,dq,dp))  //dp是私钥	  	
		goto RSA_Get_Key_again;
//printf("5...\n");
//print_bigint(dp);
//print_bigint(q);
	  // get dq = d mod q
         RSA_bigint_mod(dp,q,dq);
         // get dp = d mod p
         RSA_bigint_mod(dp,p,dp);
//printf("6...\n");
         // P=P+1; Q=Q+1;
	  p->data[0] ++;
	  q->data[0] ++;

	  // get u; u*q=1 mod p;
	  if(RSA_bigint_inverse(q,p,u))
               goto RSA_Get_Key_again;
//printf("7...\n");	  
	 // get n=P*Q
        RSA_bigint_mul(p, q, n); 
//printf("8...\n");	 
	  // check the key
         if(RSA_key_test_func(u,e,dp,dq,p,q,n))
		 goto RSA_Get_Key_again;
//printf("9...\n");	    	  
}
#endif
/*************************** RSA_bigint_add_one ************************
Input   : a--- pointer of RSA_BIGINT type
Output : none
Return : none
Function: a = a+1; 
**********************************************************************/
void RSA_bigint_add_one(RSA_BIGINT *a)
{
   U32 i,carry;
   carry = 1;
   for(i=0;i<a->len;i++)
   {
       a->data[i] += carry;
       if(a->data[i]<carry)
           carry = 1;
       else
           return;
   }
   if(carry)
   {
       a->data[a->len] = 1;
       a->len += 1;
   }
}

/**************************** RSA_bigint_mod ***************************
Input   : a,p--- pointer of RSA_BIGINT type
Output : c --- pointer of RSA_BIGINT type
Return : none
Function: c = a mod (p); 
**********************************************************************/
void RSA_bigint_mod(RSA_BIGINT *a, RSA_BIGINT *p,RSA_BIGINT *c)
{
#if 1
    U32 i,len;
    RSA_BIGINT s;
    if(RSA_bigint_compare_value(a,p)<0)
    {
        if(a!=c)
            *c = *a;
        return;
    }

    s = *a;
    len = a->len - p->len;
    RSA_bigint_shr(&s,s.len-p->len);
    while(RSA_bigint_compare_value(&s,p)>=0)
        RSA_bigint_sub(&s,p,&s);

    for(i=0;i<(len<<5);i++)
    {
       RSA_bigint_mod_mul_two(&s,p,&s);
       if(a->data[len-1-(i>>5)]&(1<<(31-(i&0x1f))))
           RSA_bigint_add_one(&s);
    }
    *c = s;
#else
    U32 i,len;
    RSA_BIGINT s,t;
    if(RSA_bigint_compare_value(a,p)<0)
    {
        if(a!=c)
            *c = *a;
        return;
    }
    s = *a;
    len = a->len - p->len;
    RSA_bigint_shr(&s,len);
    while(RSA_bigint_compare_value(&s,p)>=0)
        RSA_bigint_sub(&s,p,&s);
    if(!len)
    	{
    	*c = s;
       return;
    	}
    memset(&t,0,sizeof(RSA_BIGINT));
    t.data[len]=1;
    t.len = len + 1;
    while(RSA_bigint_compare_value(&t,p)>=0)
        RSA_bigint_sub(&t,p,&t);
    		
    RSA_bigint_mod_mul(&s,&t,p,&s);
    //	q.len = 2;
    //RSA_bigint_mod_mul2(&s,&t,p,&w);
//print_bigint(	&q);
//print_bigint(	&w);	
    memcpy(t.data,a->data,len<<2);
    t.len = len;
    RSA_bigint_clear(&t);	
    RSA_bigint_mod_add(&s,&t,p,c);
#endif
}
void RSA_bigint_mod2(RSA_BIGINT *a, RSA_BIGINT *p,RSA_BIGINT *c)
{
    U32 i,len;
    RSA_BIGINT s,t;
    if(RSA_bigint_compare_value(a,p)<0)
    {
        if(a!=c)
            *c = *a;
        return;
    }
    s = *a;
    len = a->len - p->len;
    RSA_bigint_shr(&s,len);
    while(RSA_bigint_compare_value(&s,p)>=0)
        RSA_bigint_sub(&s,p,&s);
    //len
    memset(&t,0,sizeof(RSA_BIGINT));
    t.data[len]=1;
    t.len = len + 1;
    while(RSA_bigint_compare_value(&t,p)>=0)
        RSA_bigint_sub(&t,p,&t);
//printf("EEEEEEEEEEEEEEEEEEE\n");	
//print_bigint(	&s);
//print_bigint(	&t);
//print_bigint(	p);
//RSA_bigint_mod_mul(&s,&t,p,&q);
#if 1    		
    RSA_bigint_mod_mul(&s,&t,p,&s);
#else    //	q.len = 2;
    RSA_bigint_mod_mul2(&s,&t,p,&s);
#endif
//print_bigint(	&q);
//print_bigint(	&s);
//printf("EEEEEEEEEEEEEEEEEEE\n");
    memcpy(t.data,a->data,len<<2);
    t.len = len;
    RSA_bigint_clear(&t);	
    RSA_bigint_mod_add(&s,&t,p,c);
}

/****************************** RSA_calc_bit_length *********************
Input   : a--- pointer of RSA_BIGINT type
Output : none
Return : bit length;
Function:  calc the length of a; 
**********************************************************************/ 
U32 RSA_calc_bit_length(RSA_BIGINT *a)
{
	U32 bitlen,temp;
	S32 i;
	temp = a->len - 1;
	bitlen = temp<<5;
       for(i=31;i>=0;i--)
       	{
       	if(a->data[temp]&(1<<i))
			break;
       	}
       bitlen += (U32)(i+1);
	return bitlen;
	
}

/**************************** RSA_bigint_mod_e_CRT *********************
Input   : in,u,dp,dq,p,q,out--- pointer of RSA_BIGINT type
Output : none
Return : none;
Function:  do RSA encrypt or decrypt with CRT; 
**********************************************************************/ 
#if 1
void RSA_bigint_mod_e_CRT(RSA_BIGINT *in, RSA_BIGINT *u,RSA_BIGINT *dp,RSA_BIGINT *dq,RSA_BIGINT *p,RSA_BIGINT *q,RSA_BIGINT *out)
{
    RSA_BIGINT a,b;
    U32 elen,nlen;
//printf("a.");    
    RSA_bigint_clear(in);
    RSA_bigint_clear(u);
    RSA_bigint_clear(dp);
    RSA_bigint_clear(dq);
    RSA_bigint_clear(p);
    RSA_bigint_clear(q);
//printf("b.");        
    // get a = in^e mod p
    RSA_bigint_mod2(in,p,&a); 
//printf("x.");	
    elen = RSA_calc_bit_length(dp);
//printf("y.");
    nlen = RSA_calc_bit_length(p);	
//printf("z.elen = %d, nlen = %d,",elen,nlen);
    AEmodN_for_RSA(a.data,a.data,dp->data,elen,p->data,nlen);
RSA_bigint_clear(&a);
//printf("c.");
    // get b = in^e mod q
    RSA_bigint_mod2(in,q,&b);
		elen = RSA_calc_bit_length(dq);
    nlen = RSA_calc_bit_length(q);	
    AEmodN_for_RSA(b.data,b.data,dq->data,elen,q->data,nlen);
RSA_bigint_clear(&b);
//printf("d.");
    // get the value
       RSA_bigint_mod_sub(&a,&b,p,&a);
       RSA_bigint_mod_mul(&a,u,p,&a);
//printf("e.");
       RSA_bigint_mul(&a,q,&a);
       RSA_bigint_add(&a,&b,out);
}
#else
void RSA_bigint_mod_e_CRT(RSA_BIGINT *in, RSA_BIGINT *u,RSA_BIGINT *dp,RSA_BIGINT *dq,RSA_BIGINT *p,RSA_BIGINT *q,RSA_BIGINT *out)
{
    RSA_BIGINT a,b;
    U32 elen,nlen,temp;

    // get a = in^e mod p
start_timer0();
    RSA_bigint_mod(in,p,&a);    //?
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step 1 time count is 0x%x,\n",temp);

start_timer0();
    elen = RSA_calc_bit_length(dp);
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step 2 time count is 0x%x,\n",temp);

start_timer0();
    nlen = RSA_calc_bit_length(p);
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step 3 time count is 0x%x,\n",temp);	
/*   
printf("RSA_bigint_mod_e_CRT...\n");

printf("a...\n");
print_bigint(&a);
printf("dp...0x%x,0x%x,\n",elen,nlen);
print_bigint(dp);
printf("p...\n");
print_bigint(p);
*/
start_timer0();
    AEmodN_for_RSA(a.data,a.data,dp->data,elen,p->data,nlen);
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step 4 time count is 0x%x,\n",temp);
//printf("a...\n");
//print_bigint(&a);

    // get b = in^e mod q
start_timer0();    
    RSA_bigint_mod(in,q,&b); //?
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step 5 time count is 0x%x,\n",temp);

start_timer0();	
    elen = RSA_calc_bit_length(dq);
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step 6 time count is 0x%x,\n",temp);

start_timer0();	
    nlen = RSA_calc_bit_length(q);
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step 7 time count is 0x%x,\n",temp);	
/*
printf("sfsfsfafa...\n");

printf("b...\n");
print_bigint(&b);
printf("dp...0x%x,0x%x,\n",elen,nlen);
print_bigint(dp);
printf("q...\n");
print_bigint(q);
 */
start_timer0(); 
    AEmodN_for_RSA(b.data,b.data,dq->data,elen,q->data,nlen);
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step 8 time count is 0x%x,\n",temp);
//printf("b...\n");
//print_bigint(&b);

    // get the value
start_timer0();    
       RSA_bigint_mod_sub(&a,&b,p,&a);
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step 9 time count is 0x%x,\n",temp);
/*
printf("START...\n");       
printf("a...\n");
print_bigint(&a);
printf("u...\n");
print_bigint(u);
printf("p...\n");
print_bigint(p);       
*/
start_timer0();
       RSA_bigint_mod_mul(&a,u,p,&a);
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step a time count is 0x%x,\n",temp);
//printf("a...\n");
//print_bigint(&a); 
start_timer0();
       RSA_bigint_mul(&a,q,&a);
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step b time count is 0x%x,\n",temp);
//printf("a...\n");
//print_bigint(&a);
//printf("b...\n");
//print_bigint(&b);
start_timer0();
       RSA_bigint_add(&a,&b,out);
temp = stop_timer0();
temp = 0xffffffff-temp;
printf("step c time count is 0x%x,\n",temp);	   
//printf("out...\n");
//print_bigint(out);
}
#endif
/**************************** RSA_bigint_mod_e_CRT *********************
Input   : a,e,n,out--- pointer of RSA_BIGINT type;
Output : none;
Return : none;
Function:  do RSA encrypt or decrypt without CRT; out = a^e mod n; 
**********************************************************************/ 
void RSA_bigint_mod_e(RSA_BIGINT *a,RSA_BIGINT *e,RSA_BIGINT *n,RSA_BIGINT *out)
{
    U32 i,elen;
    RSA_BIGINT u;
    elen = e->len;
    i = (elen<<5)-1;
    while(!(e->data[i>>5]&(1<<(i&0x1f))))
        i--;
    u = *a;
    while(i)
    {
        i--;
        RSA_bigint_mod_mul(&u, &u, n, &u);

        if((e->data[i>>5]&(1<<(i&0x1f))))
        {
            RSA_bigint_mod_mul(&u,a,n,&u);
        }
    }
    *out = u;

}

/*************************** RSA_key_test_func_old **********************
Input   : e,d,n--- pointer of RSA_BIGINT type
Output : none
Return : 0(yes); 1(no);
Function:  check the RSA key; 
**********************************************************************/ 
U8 RSA_key_test_func_old(RSA_BIGINT *e,RSA_BIGINT *d,RSA_BIGINT *n)
{
	U32 datain[32],dataout[32];
	U32 i,elen,nlen,dlen;
	
	memset(datain,1,sizeof(datain));

       elen = RSA_calc_bit_length(e);
	
	nlen = RSA_calc_bit_length(n);
	AEmodN_for_RSA(dataout,datain,e->data,elen,n->data,nlen);

	dlen = RSA_calc_bit_length(d);

	AEmodN_for_RSA(dataout,dataout,d->data,dlen,n->data,nlen);
	if(memcmp(datain,dataout,(n->len)<<2))
	{
		return 1;
	}
	return 0;	
	
}

/****************************** RSA_Get_Key_new **********************
Input   : e,d,n--- pointer of RSA_BIGINT type
             nlen--- bit length of n
             elen--- bit length of e
Output : none
Return : none;
Function:  get RSA key; 
**********************************************************************/ 
void RSA_Get_Key_new(RSA_BIGINT *e,RSA_BIGINT *d,RSA_BIGINT *n,U32 nlen, U32 elen)
{
         U32 i,count,tmp;
	  RSA_BIGINT FaiM,p,q;
	  
	  nlen >>= 1;
	  
RSA_Get_Key_again:
	
         if(elen>17)
		 count = 0;
	  else
	  	 count = 0x1000;
		  
	  // get prime P
         Get_Prime_new(&p,nlen);
         // get prime Q
	  Get_Prime_new(&q,nlen);

	  // get N=P*Q
	  RSA_bigint_mul(&p, &q, n);
	  
         // get (P-1)*(Q-1)
         p.data[0] -=1;
	  q.data[0] -=1;
	  RSA_bigint_mul(&p, &q, &FaiM);
	  
RSA_Get_Key_e:

	  if(elen>17)
		 count++;

	  // get e
	  e->len=1;
         switch(elen)
        	{      
              case 2:  e->data[0] = 3; break;
              case 5:  e->data[0] = 17; break;
              case 17:e->data[0] = 65537; break;
              default:
              e->len = elen>>5;			  
		if(elen&0x1f)
			(e->len)++;
		for(i=0; i<e->len; i++)
		        e->data[i] = Get_Rand();             
              e->data[0] |= 1;
              tmp = (elen-1)&0x1f;	  
		//e->data[e->len -1] &= FORELEN[elen];
		for(i=31;i>tmp;i--)
                      e->data[e->len -1]  &= (~(1<<i));
		e->data[e->len -1] |= (1<<tmp);
              break;
              }
		RSA_bigint_clear(e);

              if(RSA_bigint_compare_value(e, &FaiM) >=0)
			  goto RSA_Get_Key_e;
	  // get d
	  if(RSA_bigint_inverse(e,&FaiM,d))
	  	{
	  	if(count>=0x1000)
			goto RSA_Get_Key_again;
	  	goto RSA_Get_Key_e;
	  	}

	  // check the key
         if(RSA_key_test_func_old(e,d,n))
		 goto RSA_Get_Key_again;

}



