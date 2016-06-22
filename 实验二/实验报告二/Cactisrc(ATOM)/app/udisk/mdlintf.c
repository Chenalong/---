
/*

   mdlintf.c

*/
#include <instructions.h>
#include <bsp.h>
#include "mdlintf.h"

#include "REG1222.h"

#define USE_LITTLE_ENDIAN 0

extern long rngctl_data;    // defined in init.c

/*************************** ABmodN_little ******************************
Input   : out,a,b,n--- pointer of U32
             nlen---the length of n
Output : none;
Return : none;
Function:  out = a*b mod (n); 
**********************************************************************/ 
void ABmodN_little(U32 *out, U32 *a, U32 *b, U32 *n, U32 nlen)
{
    U32 j,t_n;
    t_n = nlen>>5;
  	if(nlen&0x1f)
    	t_n++;
    // set mode register
	REG32(RSAMODEADD) = RSA_2048_MODE;      		
	// set n length    	   	
	REG32(RSANLENADD) = nlen;
       // set b data
  //printf("nlen=%d,%d...", nlen,t_n);    
  //print_buf(a,4*t_n);     
  //print_buf(b,4*t_n);
  //print_buf(n,4*t_n);     
	for(j=0;j<t_n;j++)		
	{      
		 //REG32(RSABBASE+j*4)=b[j];
		 if(j<t_n)
		 {
		 REG32(RSAABASE+j*4)=a[j];
		 REG32(RSANBASE+j*4)=n[j];
		 }
		 else
		 		 {
		 REG32(RSAABASE+j*4)=0;
		 REG32(RSANBASE+j*4)=0;
		 }
	}
	
	REG32(RSACOMADD)=RSAHABCOM;
  	REG32(RSACTLADD)=0x1;
	do
 	{
	 j=REG32(RSACTLADD);	  
 	}while (j&1);

	for(j=0;j<t_n;j++)
	{
          if(j<t_n)
          REG32(RSABBASE+j*4)=b[j];
          else
          REG32(RSABBASE+j*4)=0;
	}

	REG32(RSACOMADD)=RSAABCOM;
  	REG32(RSACTLADD)=0x1;
	do
 	{
	 j=REG32(RSACTLADD);	  	 
 	}while (j&1);


 	for(j=0;j<t_n;j++)
 	{
	  out[j] = REG32(RSAABASE+j*4); 
	}
 //print_buf(n,4*t_n);		
}

/************************* AEmodN_for_RSA *****************************
Input   : out,a,e,n--- pointer of U32
             nlen---the length of n
             elen---the length of e
Output : none;
Return : 0(OK),1(ERROR);
Function:  out = a^b mod (n); 
**********************************************************************/ 
U8 AEmodN_for_RSA(U32 *out, U32 *a, U32 *e, U32 elen, U32 *n, U32 nlen)
{
        U32 j,t_n,t_e;
    // set mode register
	REG32(RSAMODEADD) = RSA_DEFAULT_MODE;
	
	 // check e length	
        if(elen<2)	
		return 1;

  	t_n = nlen>>5;
  	if(nlen&0x1f)
    		t_n++;
    	t_e = elen>>5;
  	if(elen&0x1f)
    		t_e++;    		    	    	
	REG32(RSANLENADD) = nlen;	// set n length
	REG32(RSAELENADD) = elen;	// set e length
	
	for(j=0;j<t_e;j++)		
	{
	 REG32(RSAEBASE+j*4)=e[j];	// set e data
	}
	for(j=0;j<t_n;j++)		
	{
		REG32(RSAABASE+j*4)=a[j];	// set a data
		REG32(RSANBASE+j*4)=n[j];	// set n data
	}
	REG32(RSACOMADD)=RSAHAECOM;
  	REG32(RSACTLADD)=0x1;
	do
 	{
	 j=REG32(RSACTLADD);
 	}while (j&1);
		
 	for(j=0;j<t_n;j++)
 	{
	  out[j] = REG32(RSABBASE+j*4);
 	}
	return 0;
}

/*************************** ABmul_hardware ******************************
Input   : out,a,b--- pointer of U32
             nlen---the length of n
Output : none;
Return : none;
Function:  out = a*b ;
note: a and b bit length must <= 1024 
**********************************************************************/ 
void ABmul_hardware(U32 *out, U32 *a, U32 alen,U32 *b, U32 blen)
{
    U32 nlen,j,t_a,t_b,t_n;
    // set mode register
	REG32(RSAMODEADD) = RSA_1024_MODE;	
	// set n length    	   	
	nlen=alen >blen ? alen:blen;
	nlen<<=1;
    nlen=nlen >66 ? nlen:66; 
    REG32(RSANLENADD) = nlen;
	
	t_a = alen>>5;
  	if(alen&0x1f)
    	t_a++;
    t_b = blen>>5;
  	if(blen&0x1f)
    	t_b++;
    t_n = nlen>>5;
  	if(nlen&0x1f)
    	t_n++;
        
    // set a,b data
	for(j=0;j<t_n;j++)		
	{
		 if(j<t_a)
            REG32(RSAABASE+j*4)=a[j];
		 else
		    REG32(RSAABASE+j*4)=0;
		 if(j<t_b)   
		    REG32(RSABBASE+j*4)=b[j];
		 else
            REG32(RSABBASE+j*4)=0;   
	}
	
	REG32(RSACOMADD)=RSAHABCOM;
  	REG32(RSACTLADD)=0x1;
	do
 	{
	 j=REG32(RSACTLADD);	  
 	}while (j&1);

 	for(j=0;j<(t_a+t_b);j++)
 	{
	  out[j] = REG32(RSAABASE+j*4); 
	}		
}

void GF2m_ABmul_hardware(U32 *out, U32 *a, U32 *b, U32 mlen)
{
	U32 int_len,j;
       // set mode register
	REG32(RSAMODEADD) = RSA_ECC_MODE;
  	int_len = (mlen+1)>>5;
  	if((mlen+1)&0x1f)
    		int_len++;

#if 0	
printf("mlem=%d,int_len=%d,\n",mlen,int_len);
printf("\n   A value...");
print_buf(a,int_len<<2);	
printf("\n   B value...");
print_buf(b,int_len<<2);
#endif	

	REG32(RSANLENADD) = mlen;
	
	for(j=0;j<int_len;j++)
		{
		REG32(ECC_A+j*4)=a[j];
		REG32(ECC_B+j*4)=b[j];
		}
#if 1
	// clear C
	for(j=0;j<32;j++)
		REG32(ECC_CL+j*4)=0;
#endif
	
	REG32(RSACOMADD)=ECC_ABmul_CMD;
  	REG32(RSACTLADD)=0x1;
	do
 	{
	 j=REG32(RSACTLADD);	  
 	}while (j&1);

 	for(j=0;j<(int_len<<1);j++)
 	{
	  out[j] = REG32(ECC_S+j*4); 
	}
#if 0	
printf("\n   out value...");
print_buf(out,int_len<<3);
#endif	

}

void GF2m_CmodF_hardware(U32 *out, U32 *c, U32 *f,U32 mlen)
{
	U32 int_len,j;
       // set mode register
	REG32(RSAMODEADD) = RSA_ECC_MODE;
  	int_len = (mlen+1)>>5;
  	if((mlen+1)&0x1f)
    		int_len++;

	REG32(RSANLENADD) = mlen;
	
	for(j=0;j<(int_len<<1);j++)
		REG32(ECC_CL+j*4)=c[j];
	for(j=0;j<int_len;j++)
		REG32(ECC_F+j*4)=f[j];
	
	REG32(RSACOMADD)=ECC_CmodF_CMD;
  	REG32(RSACTLADD)=0x1;
	do
 	{
	 j=REG32(RSACTLADD);	  
 	}while (j&1);

 	for(j=0;j<int_len;j++)
 	{
	  out[j] = REG32(ECC_S+j*4); 
	}		

}

void GF2m_ABmodF_hardware(U32 *out, U32 *a, U32 *b, U32 *f,U32 mlen)
{
	U32 int_len,j;
       // set mode register
	REG32(RSAMODEADD) = RSA_ECC_MODE;
  	int_len = (mlen+1)>>5;
  	if((mlen+1)&0x1f)
    		int_len++;

	REG32(RSANLENADD) = mlen;
#if 0	
printf("mlem=%d,int_len=%d,\n",mlen,int_len);
printf("\n   A value...");
print_buf(a,int_len<<2);	
printf("\n   B value...");
print_buf(b,int_len<<2);
printf("\n   F value...");
print_buf(f,int_len<<2);
#endif	
	for(j=0;j<int_len;j++)
		{
		REG32(ECC_A+j*4)=a[j];
		REG32(ECC_B+j*4)=b[j];
		REG32(ECC_F+j*4)=f[j];
		}
#if 1
	// clear C
	for(j=0;j<32;j++)
		REG32(ECC_CL+j*4)=0;
#endif
	
	REG32(RSACOMADD)=ECC_ABmodF_CMD;
  	REG32(RSACTLADD)=0x1;
	do
 	{
	 j=REG32(RSACTLADD);	  
 	}while (j&1);

 	for(j=0;j<int_len;j++)
 	{
	  out[j] = REG32(ECC_S+j*4); 
	}
#if 0	
printf("\n   out value...");
print_buf(out,int_len<<2);
#endif
}

void RSA_clear_memory()
{
	int j;
	for(j=0;j<(RSA_RAM_SIZE/4);j++)
		REG32(RSABASE+j*4)=0;
}

void HCMD_CALC(U32 *n, U32 nlen)
{
        U32 j,t_n;
// set mode register
	REG32(RSAMODEADD) = RSA_DEFAULT_MODE;		
        t_n = nlen>>5;	
	REG32(RSANLENADD) = nlen;	// set n length

	for(j=0;j<t_n;j++)		// set b data
	{
		 nlen = t_n-j-1;
		 REG32(RSANBASE+j*4)=n[nlen];
	//	 printf("a=%x,b=%x,p=%x...\n",a[t_n-j-1],b[t_n-j-1],n[t_n-j-1]);	
	}
	
	REG32(RSACOMADD)=RSAHCOM;
  	REG32(RSACTLADD)=0x1;
	do
 	{
	 j=REG32(RSACTLADD);	  
 	}while (j&1);
}

void ABmodN_FOR_ECC(U32 *out, U32 *a, U32 *b)
{
        U32 j,i;
	// set mode register
	REG32(RSAMODEADD) = RSA_DEFAULT_MODE;	
#if 1        
	for(j=0;j<6;j++)		// set b data
	{
		 i = 6-j-1;
		 REG32(RSABBASE+j*4)=b[i];
		 REG32(RSAABASE+j*4)=a[i];
	//	 printf("a=%x,b=%x,p=%x...\n",a[t_n-j-1],b[t_n-j-1],n[t_n-j-1]);	
	}
#else
#endif	
	REG32(RSACOMADD)=RSAABCOM;
  	REG32(RSACTLADD)=0x1;

	do
 	{
	 j=REG32(RSACTLADD);	  
 	}while (j&1);

#if 1
 	for(j=0;j<6;j++)
 	{
	  out[6-j-1] = REG32(RSAABASE+j*4);
//	  printf("out=%x...\n",out[t_n-j-1]);  
	}
#else
#endif		
}

/*
void RAND_pseudo_bytes(char *buf,int len)
{
	len>>=2;
	int i;
	unsigned char *temp = (unsigned char *)buf;
	for(i=0;i<len;i++)
	{
		REG32(0x9802f000)=0xc85;
		while((REG32(0x9802f000)&2)==0);
		*(temp+i) = REG32(0x9802f004);
	}		
}
*/

/*************************** brev_transfer ******************************
Input   : dst,src--- pointer of U32
             len---the length of src data
Output : none;
Return : none;
Function:  endian convert ; 
**********************************************************************/ 
void brev_transfer(U32 *dst, U32 *src, U32 len)
{
       U32 i,temp;
	for(i=0;i<len;i++)
		{
		i_brev(temp,src[i]);
		dst[i]=temp;
		}
}

/*************************** brev_transfer ******************************
Input   : buf--- pointer of U32
             len---the length of src data
Output : none;
Return : none;
Function:  buffer convert ;
Note: len must is a  even;
**********************************************************************/ 
void swap_buffer(U32 *buf,U32 len)
{
      U32 i;
      for(i=0;i<(len>>1);i++)                    
		i_swap(buf[i],buf+len-i-1,0);      	
      brev_transfer(buf,buf,len); 
}

/*************************** brev_transfer ******************************
Input   : key,desiv,din,dout--- pointer of U32
             dcntl---the DES control register value
Output : none;
Return : none;
Function:  DES/TDES calc;
Note: if use ECB mode, the desiv can set be NULL;
         you need select the function according to the endian mode, 
         by set USE_LITTLE_ENDIAN = 1 or 0;
**********************************************************************/ 
//#if USE_LITTLE_ENDIAN
#if 1
void DES_hardware_crypt(U32 *key,U32 *desiv,U32 *din,U32 *dout,U32 dcntl)
{
     REG32(DCNTRL) = dcntl&0xffffff7f;

     REG32(DDAT) = din[0];
//printf("din[0] = 0x%x,",din[0]);	 
     REG32(DDAT) = din[1];
//printf("din[1] = 0x%x,",din[1]);

     if(dcntl&0x02)
	 {
	 REG32(DESIV) =desiv[0];
	 REG32(DESIV) =desiv[1];
	 }
	
     if(dcntl&0x30)
	 {
	 REG32(DKEY) = key[4];		
	 REG32(DKEY) = key[5];   		
	 }
	
     if(dcntl&0x20)
	{
	REG32(DKEY) = key[2];
	REG32(DKEY) = key[3];
	}
	 
     REG32(DKEY) = key[0];
//printf("key[0] = 0x%x,",key[0]);	 
     REG32(DKEY) = key[1];
//printf("key[1] = 0x%x,",key[1]);

//printf("dcntl = 0x%x,",dcntl);	 
     REG32(DCNTRL) = dcntl;
     while (REG32(DCNTRL)&0x80);
//printf("DCNTRL = 0x%x,",REG32(DCNTRL));	 


     dout[0] = REG32(DDAT);
//printf("dout[0] = 0x%x,",dout[0]);		 
     dout[1] = REG32(DDAT);
//printf("dout[1] = 0x%x,",dout[1]);

}
#else
void DES_hardware_crypt(U32 * key, U32 * desiv, U32 * din, U32 * dout, U32 dcntl)
{
     U32 temp[2];
     brev_transfer(temp,din,2);
     REG32(DDAT) = temp[0];
     REG32(DDAT) = temp[1];

     if(dcntl&0x02)
	 {
        brev_transfer(temp,desiv,2);
	 REG32(DESIV) =temp[0];
	 REG32(DESIV) =temp[1];
	 }
	
     if(dcntl&0x30)
	 {
        brev_transfer(temp,key+4,2);
	 REG32(DKEY) = temp[0];		
	 REG32(DKEY) = temp[1];   		
	 }
	
     if(dcntl&0x20)
	{
	brev_transfer(temp,key+2,2);
	REG32(DKEY) = temp[0];
	REG32(DKEY) = temp[1];
	}
	 
     brev_transfer(temp,key,2);	 
     REG32(DKEY) = temp[0];
     REG32(DKEY) = temp[1];
	 
     REG32(DCNTRL) = dcntl;
     while (REG32(DCNTRL)&0x80);

     dout[0] = REG32(DDAT);
     dout[1] = REG32(DDAT);
     brev_transfer(dout,dout,2);	 
}
#endif



