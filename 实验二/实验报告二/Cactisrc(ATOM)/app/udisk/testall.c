
#include <usb.h>
#include <ukey_proto.h>
#include <instructions.h>
#include <bsp.h>
#include "bigint.h"
#include "REG1222.h"
#include <types.h>
#include "mydef.h"
#include "md5.h"
#include "GF2m.h"
#include "rsa_big.h"

//============
//pierce.hu add
#include "ums.h"
#include "nand_media.h"
//============

#define NULL 0

#define DEBUG 1
#undef DEBUG

#define KCW_SIGN 0xa5
#define KCW_SIGN_ERR 2
#define KSW_SIGN 0x5a

#define SSX33_ENCYPT(x,y) ((void (*)())(0x8001f000))(x,y)
#define SSX33_DECYPT(x,y) ((void (*)())(0x8001f000+0x1c))(x,y)
#define SSX33_GENERATE_KEY(x,y) ((int (*)())(0x8001f000+0x1c+0x1c))(x,y)
#define SSF33_ZJ() ((int (*)())(0x8001f000+0x1c+0x1c+0x1c))()

#define SCD_DEBUG_EN 0



unsigned char g_io_buf[2400];
#define ECC_GG2m_191
//#define ECC_GG2m_409

#ifdef ECC_GG2m_409
// f(x) = x(409)+x(87)+1; need 14 word
#define ECC2m_BIT_LENFTH 409
const U32      ECCGF2m_f[]={1,0,0x800000,0,0,0,0,0,0,0,0,0,0x02000000,0};
const U32     ECCGF2m_a[]={1,0,0,0,0,0,0,0,0,0,0,0,0,0};
const U32     ECCGF2m_b[]={0x7B13545F,0x4F50AE31,0xD57A55AA,0x72822F6C,0xA9A197B2 ,0xD6AC27C8 ,0x4761FA99,0xF1F3DD67,0x7FD6422E,0x3B7B476B,0x5C4B9A75,0xC8EE9FEB,0x0021A5C2,0};
const U32 ECCGF2m_G0x[]={0xBB7996A7,0x60794E54,0x5603AEAB,0x8A118051,0xDC255A86 ,0x34E59703 ,0xB01FFE5B,0xF1771D4D,0x441CDE4A,0x64756260,0x496B0C60,0xD088DDB3,0x015D4860,0};
const U32 ECCGF2m_G0y[]={0x0273C706,0x81C364BA,0xD2181B36,0xDF4B4F40,0x38514F1F ,0x5488D08F ,0x0158AA4F,0xA7BD198D,0x7636B9C5,0x24ED106A,0x2BBFA783,0xAB6BE5F3,0x0061B1CF,0};
const U32     ECCGF2m_k[]={0x300700fe,0xf6db2b07,0x7ae91e9e,0x23bbb370,0xf9920863 ,0x37010895 ,0xD2181B36,0xDF4B4F40,0x38514F1F,0x5488D08F,0x0158AA4F,0x64756260,0x006B0C60,0};
#endif

#ifdef ECC_GG2m_191
// f(x) = x(191)+x(9)+1; need 7 word
#define ECC2m_BIT_LENFTH 191
const U32 ECCGF2m_f[] = {0x00000201,0,0,0,0,0x80000000,0};
const U32 ECCGF2m_a[]={0xF7526267 ,0x276B649E ,0x54E12640 ,0x6A68F565 ,0x67675263 ,0x2866537B ,0};
const U32 ECCGF2m_b[]={0x0AA185EC ,0x5462F5DE ,0x9495A3D9 ,0x67B0081B ,0x1F00786F ,0x2E45EF57 ,0};
const U32 ECCGF2m_G0x[]={0x4AE1AA0D ,0x9137F2C8 ,0xB21A9C36 ,0xC4F299D7 ,0xA23206F9 ,0x36B3DAF8 ,0};
const U32 ECCGF2m_G0y[]={0xF98018FB ,0x2418EA0E ,0x0EA245CA ,0x332932E7 ,0x33B3F95E ,0x765BE734 ,0};
const U32 ECCGF2m_k[]={0x300700fe ,0xf6db2b07 ,0x7ae91e9e ,0x23bbb370 ,0xf9920863 ,0x37010895,0};
#endif

// for 192 p field ECC

U32 const ECC_p[]={0,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFF};
U32 const ECC_a[]={0,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFC};
U32 const ECC_b[]={0,0x64210519,0xE59C80E7,0x0FA7E9AB,0x72243049,0xFEB8DEEC,0xC146B9B1};
U32 const ECC_P0x[]={0,0x188DA80E,0xB03090F6,0x7CBF20EB,0x43A18800,0xF4FF0AFD,0x82FF1012};
U32 const ECC_P0y[]={0,0x07192B95,0xFFC8DA78,0x631011ED,0x6B24CDD5,0x73F977A1,0x1E794811};
U32 const ECC_N[]={0,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0x99DEF836,0x146BC9B1,0xB4D22831};

const U32 ECC_kA[]={0,0x3ac0e717,0xeb61602e,0xfcbb1de8,0x1aa144a2,0x72b44ba1,0xf16936ac};
const U32 ECC_kB[]={0,0x25fbb32e,0xfbec6ecb,0x1314332a,0x026582db,0x7be00c05,0x1cf2fa80};



extern ECCGF2m_BIGINT ECCGF2m_c;
extern U32 ECCGF2m_m;
extern U32 ECCGF2m_int;
extern U32 ECCGF2m_double_int;

void print_buf(U8 *buf,U32 len)
{
    U32 i;
    printf("print buf begin, buf len = %d, buffer data = \n",len);
    for(i=0;i<len;i++)
    {
    	if((i&0x0f)==0x0f)
    	    printf("0x%02x,\n",buf[i]);
    	else
            printf("0x%02x,",buf[i]);	
    }
    printf("****** print buffer end *******\n");	
}

/****************************** print_ecc_str *****************************
Input   : a--- pointer of RSA_BIGINT type
Output : none
Return : none
Function: print the data of BIGINT type
**********************************************************************/
void print_ecc_str( BIGINT *a)
{ 
       int i;
	   print_byte(0x0d);
	   print_byte(0x0a);
	printf("\necc big begin!\n" );
	   print_byte(0x0d);
	   print_byte(0x0a);	
    	for(i=0;i<ECC_MAXLONG;i++)
		printf("0x%08x,",a->data[i]);

}

/****************************** print_bigint *****************************
Input   : a--- pointer of RSA_BIGINT type
Output : none
Return : none
Function: print the data of RSA_BIGINT type
**********************************************************************/
void print_bigint(RSA_BIGINT *a)
{
       U32 i;
	printf("bigint length = %d, \n bigint data begin!\n",a->len);
    	for(i=0;i<a->len;i++)
		printf("0x%x,",a->data[i]);
       printf("\n bigint data end!\n");
}

/****************************** print_GF2m *****************************
Input   : a--- pointer of RSA_BIGINT type
Output : none
Return : none
Function: print the data of ECCGF2m_BIGINT type
**********************************************************************/
void print_GF2m(ECCGF2m_BIGINT *a)
{
       U32 i;
	printf("G2m len= %d, \n G2m begin!\n",ECCGF2m_int);
    	for(i=0;i<ECCGF2m_int;i++)
		printf("0x%08x,",a->d[i]);
       printf("\n G2m end!\n");
}
#if 1
void test_ecc_p_func()
{    
    BIGINT *a,*b,*p,*ka,c;
    ECC_POINT p0,pA;
	U32 rlen;
	char prikey[30],outbuf[3*24+1],hash[32],pubkey[60];
	char *str="abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	char *nstr="Hello World!";
    printf("start test ecc p field func!...\n");
    a = (BIGINT *)ECC_a;
    b = (BIGINT *)ECC_b;
    p = (BIGINT *)ECC_p;
    ka = (BIGINT *)ECC_kA;


    memcpy(p0.x.data, &ECC_P0x, 4*ECC_MAXLONG);
    memcpy(p0.y.data, &ECC_P0y, 4*ECC_MAXLONG);

    ecc_point_mul(ka,&p0,&pA);

	if(ECC_test_point(&pA,ECC_p))
		printf("not in ecc curve");
	else
		printf("in ecc curve!");
	
	   print_byte(0x0d);
	   print_byte(0x0a);	
printf("************* ECDSA **************************");
    bigint_to_char(ka,prikey);
    ecdsa_signature(str,56,prikey,outbuf,&rlen);
print_buf(outbuf,rlen);
	sha256_memory(str,56,hash);
	//print_buf(hash,32);
    point_to_char(&pA,pubkey);
	//print_buf(pubkey, 50);
    if(ecdsa_verification(outbuf,rlen,hash,pubkey))
		printf("verification fail!");
    else
		printf("verification pass!");

	   print_byte(0x0d);
	   print_byte(0x0a);	
printf("************* ECES **************************");
    ecc_point_mul((BIGINT *)ECC_kB,&p0,&pA);
	print_ecc_str(&pA.x);
	print_ecc_str(&pA.y);
	if(ECC_test_point(&pA,ECC_p))
		printf("not in ecc curve");
	else
		printf("in ecc curve!");
    point_to_char(&pA,pubkey);	
    e_random(nstr,12,pubkey,outbuf,&rlen);
print_buf(outbuf,rlen);
	bigint_to_char((BIGINT *)ECC_kB,prikey);
    d_random(outbuf, rlen, prikey,  outbuf,&rlen);
//print_buf(outbuf,rlen);
outbuf[rlen]=0;
print_string(outbuf);
print_byte(0x0d);
print_byte(0x0a);  
printf("test ecc p field func end!...\n");
print_byte(0x0d);
print_byte(0x0a);  
}
#endif

void usbkey_initialize()
{
  // ecc gf2m field initialize 
  ECCGF2m_init(ECC2m_BIT_LENFTH);
  // rng initialize
  RNG_Init();
  // eeprom interrupt
  //EepromIntInit(); 
#if 0  
  test_ecc_p_func();
#endif  

}

void usbkey_csw_send(unsigned char csw_result,CBW * pcbw, unsigned long len)
{
	CSW csw;
	csw.dwCSWSignature = CSW_SIGNATURE;
	csw.dwCSWtag = pcbw->dwCBWtag;
	csw.dwCSWResidue = pcbw->dwCBWXferLength;
	csw.bCSWStatus = csw_result;
	usb_send(g_io_buf,len);
	usb_send((unsigned char *)(&csw),13);
}

void process_usbkey(CBW * pcbw)
{
	U8 MD5_Encrypt[50];
	U8 MD5_Decrypt[50];
 RSA_BIGINT *rsa_p,*rsa_q,*rsa_u,*rsa_e,*rsa_dp,*rsa_dq,*rsa_n,*in_buf,*out_buf;
 ECCGF2m_ECC_POINT *GF2m_g0,*GF2m_gk;
 ECCGF2m_BIGINT *GF2m_t;
 BIGINT *ka;
 ECC_POINT *p0,*pA;  
 Ukey_KCW *kcw;
 U32 nlen,elen,count; // count is used for IN bytes
 int i,j;
 Ukey_KSW ksw;
 U16 do_count;
  
	count = 0;
       kcw = (Ukey_KCW *)g_io_buf;
//printf("process_usbkey handle...\n");
//print_long(&i);
	//set usb_key_mode

	if(pcbw->bCBWFlags&0x80) //  device to host	
		{		
		// device send data to host
		usb_send(g_io_buf,pcbw->dwCBWXferLength);
		}
	else // host to device
		{// receive usbkey data
		g_usb_receive(g_io_buf , pcbw->dwCBWXferLength);
print_buf(kcw,16);
		//usb_key main handle
		   // 命令解析
      		switch(kcw->bKCWINS)
     		{
     		case BOOT_BACK:
			{
			printf("enter boot back...\n");
			enable_boot(0x212a45b7,0x78346321,0x23e545ba);
			break;
     			}
      		case INT_FUNC:  
     			{
          		if(kcw->bKCWCLA) // decrypt                     
             			{
             			delay_xxx(100);
             			printf("RNG interrupt handle...\n");                                 
          			}
	   		else // encrypt              
          			{
              		delay_xxx(500000); 
	       		printf("PAE interrupt handle...\n");
          			}
	   		ksw.bKSWStatus = 0xfb;	     
              	break;	       
     			}      
		case ECC_G2M:  
     			{
			GF2m_gk = (ECCGF2m_ECC_POINT *)(g_io_buf+16+128);
			GF2m_t = (ECCGF2m_BIGINT *)ECCGF2m_k;
			GF2m_g0 = (ECCGF2m_ECC_POINT *)(g_io_buf+16);
			memcpy(GF2m_g0->x.d, &ECCGF2m_G0x, 4*ECCGF2m_int);
    			memcpy(GF2m_g0->y.d, &ECCGF2m_G0y, 4*ECCGF2m_int);		
          		switch(kcw->bKCWCLA)		
	 			{
              		case 0: // sign
              		printf("ECC_G2M sign...\n");   
					start_timer0();
					ECCGF2m_point_mul(GF2m_t,GF2m_g0,GF2m_gk);
					delay_xxx(1000);
					nlen = stop_timer0();			  		
              			break;
				case 1: // veri
				printf("ECC_G2M veri...\n"); 
					start_timer0();
					ECCGF2m_point_mul(GF2m_t,GF2m_g0,GF2m_gk);
					delay_xxx(1000);
					ECCGF2m_point_mul(GF2m_t,GF2m_g0,GF2m_gk);
					nlen = stop_timer0();	
              			break;
				default: 
					printf("ECC_G2M key...\n"); 
					GF2m_t = (ECCGF2m_BIGINT *)(g_io_buf+16+256);
					ECCGF2m_get_rand(GF2m_t);
//print_GF2m(GF2m_t);
					start_timer0();
					ECCGF2m_point_mul(GF2m_t,GF2m_g0,GF2m_gk);
					nlen = stop_timer0();
					//print_GF2m(GF2m_t);
					//print_GF2m(&(GF2m_gk->x));
					//print_GF2m(&(GF2m_gk->y));
	if(ECCGF2m_test_point(GF2m_gk,(ECCGF2m_BIGINT *)ECCGF2m_f))
		printf("not in ecc gf2m curve");
	else
		printf("in ecc gf2m curve!");		
					memcpy(g_io_buf,GF2m_t->d,(ECCGF2m_int-1)<<2); // k
					memcpy(g_io_buf+((ECCGF2m_int-1)<<2),GF2m_gk->x.d,(ECCGF2m_int-1)<<2); // Px
					memcpy(g_io_buf+((ECCGF2m_int-1)<<3),GF2m_gk->y.d,(ECCGF2m_int-1)<<2); // Py
					count = ((ECCGF2m_int-1)<<3)+((ECCGF2m_int-1)<<2);
					break;       			
				}
	   		nlen = 0xffffffff-nlen;		   
			memcpy(&ksw,&nlen,4);	     
              	break;	       
     			}    
        
            case Get_ID:
            {
                nlen = 8;
                elen = 0x08000000;
                ksw.bKSWStatus = 0xfb; 
                ReadEeprom(g_io_buf, elen, nlen);
                print_buf(g_io_buf, nlen);
                count = 8;
                break;
            }
            
           
     		case READ_FIX_ADDRESS:  
     			{
               	//test_addr=kcw->flag.ads;
               	count=kcw->dKCWInLength;
               	nlen=kcw->dKCWInLength;
			elen = kcw->flag.ads;
               	ksw.bKSWStatus = 0xfb;  
               	//memset(temp_buffer,0,2048); 
               	if(kcw->flag.ads > 0x80040000)
                		ReadEeprom(g_io_buf,elen,nlen);
               	else
                		ReadFlash(g_io_buf,elen,nlen);
 		        print_buf(g_io_buf,nlen);		   
               	break;	       
     			}   
     			
		    case Set_ID://设置USBKEY的ID号，该函数慎用，初始化时一次 
            {
                nlen = 8;
                elen = 0x08000000;
                ksw.bKSWStatus = 0xfb; 
               	for(i=0;i<8;i++)					
					g_io_buf[i] = Get_Rand();
					
                EepromAutoWrite(g_io_buf, elen, nlen);
                memset(g_io_buf,0,2048); 
          		ReadEeprom(g_io_buf,elen,nlen);
                print_buf(g_io_buf, nlen);
                count = 8;
                break;
            }
            	
     		case FIXED_ADDRESS_TEST:  
     			{
               	//print_string("Fixed Address test ...\n");
               	nlen=kcw->dKCWOutLength; // 
               	count=kcw->dKCWInLength;
			elen = kcw->flag.ads; // ads
               	if(kcw->flag.ads > 0x80040000)
               		{
               		if(EepromAutoWrite(g_io_buf+16,elen,nlen))
               			{
                  			ksw.bKSWStatus = 0;  
                  			printf("Write Address eeprom :0x%08x Error!\n",elen);                  
               			}
               		else
                			ksw.bKSWStatus = 0xfb;              
               		memset(g_io_buf,0,2048); 
               		ReadEeprom(g_io_buf,elen,count);
               		}
               	else
               		{
               		printf("Fixed Address test ...addr=%08x\n",elen);
               		if(WriteFlash(g_io_buf+16,elen,nlen)==FALSE)
               			{
                  				ksw.bKSWStatus = 0;  
                  				printf("Write Address flash :0x%08x Error!\n",elen);                  
               		}
               		else
               			 ksw.bKSWStatus = 0xfb;
              
                		memset(g_io_buf+16,0,2048); 
                		ReadFlash(g_io_buf,elen,count);
               		}
               	break;	       
     			}   
    		 case RSA_FUNC:
     			{
			//*rsa_p,*rsa_q,*rsa_u,*rsa_e,*rsa_dp,*rsa_dq,*rsa_n,*in_buf,*out_buf;
			rsa_u = (RSA_BIGINT *)(g_io_buf+16);
			rsa_e = (RSA_BIGINT *)(g_io_buf+16+66*4*1);
			rsa_dp = (RSA_BIGINT *)(g_io_buf+16+66*4*2);
			rsa_dq = (RSA_BIGINT *)(g_io_buf+16+66*4*3);
			rsa_p = (RSA_BIGINT *)(g_io_buf+16+66*4*4);
			rsa_q = (RSA_BIGINT *)(g_io_buf+16+66*4*5);
			rsa_n = (RSA_BIGINT *)(g_io_buf+16+66*4*6);
			in_buf = (RSA_BIGINT *)(g_io_buf+16+66*4*7);
			out_buf = (RSA_BIGINT *)(g_io_buf+16+66*4*8);
						                                                                                                                                                                                                                                                           
	 		memcpy(&do_count,kcw,2); 
	 		switch(kcw->bKCWCLA)		
	 			{
              		case 0: // get prime
              			nlen = kcw->flag.ads;
                       		Get_Prime_new(rsa_u,nlen>>1);
					count = nlen>>4;
			  		memcpy(g_io_buf,rsa_u->data,count);					
			  		break;
              		case 1: // get rsa key no CRT
#if 1
					count = kcw->flag.ads;
					elen = kcw->Rev.sum;	
					start_timer0();
			 		while(do_count--)		   
                       		RSA_Get_Key_new(rsa_e,rsa_dp,rsa_n,count,elen);
			  		nlen = stop_timer0();
			  		nlen = 0xffffffff-nlen;		   
			  		memcpy(&ksw,&nlen,4);
					nlen = count>>3;
              			memcpy(g_io_buf,rsa_e->data,nlen);	 // e
			  		memcpy(g_io_buf+nlen,rsa_dp->data,nlen); // d
			  		memcpy(g_io_buf+(nlen<<1),rsa_n->data,nlen); // n
			  		count = 3*nlen;
#endif			  
			  		break;
              		case 0x81: // get rsa key with CRT
              		       count = kcw->flag.ads;
					elen = kcw->Rev.sum;
			//printf("nlen = %d, elen = %d,...\n",count,elen); 
			//printf("do_count = %d, \n",do_count);
					start_timer0();							
			  		while(do_count--)		   
                       			RSA_Get_Key_For_CRT(rsa_u,rsa_e,rsa_dp,rsa_dq,rsa_p,rsa_q,rsa_n,count,elen);
			  		nlen = stop_timer0();
			  		nlen = 0xffffffff-nlen;				
			  		memcpy(&ksw,&nlen,4);
			//print_bigint(rsa_p);
					nlen = count>>3;
              			memcpy(g_io_buf,rsa_u->data,nlen>>1);	 // u
			  		memcpy(g_io_buf+(nlen>>1),rsa_e->data,nlen); //e
			  		memcpy(g_io_buf+(nlen>>1)+nlen,rsa_dp->data,nlen>>1); // dp
			  		memcpy(g_io_buf+2*(nlen>>1)+nlen,rsa_dq->data,nlen>>1); // dq
			  		memcpy(g_io_buf+3*(nlen>>1)+nlen,rsa_p->data,nlen>>1); // p
			  		memcpy(g_io_buf+4*(nlen>>1)+nlen,rsa_q->data,nlen>>1); // q
			  		memcpy(g_io_buf+5*(nlen>>1)+nlen,rsa_n->data,nlen); //
			  		count = 5*(nlen>>1)+2*nlen;
			  		break;
              		default: // calc rsa rate
                      		 in_buf->len = out_buf->len = kcw->flag.ads>>5;
                       		 for(j=0;j<in_buf->len;j++)
	                    			in_buf->data[j] = Get_Rand();
                       		 in_buf->data[in_buf->len-1] &=0x7fffffff;
                       		 RSA_bigint_clear(in_buf);
			  
                            	RSA_Get_Key_For_CRT(rsa_u,rsa_e,rsa_dp,rsa_dq,rsa_p,rsa_q,rsa_n,kcw->flag.ads,kcw->Rev.sum);
			       	start_timer0();		   
			       	RSA_bigint_mod_e_CRT(in_buf,rsa_u,rsa_dp,rsa_dq,rsa_p,rsa_q,out_buf);		                               			  	
			  		nlen = stop_timer0();
			 		nlen = 0xffffffff-nlen;		   
			  		memcpy(&ksw,&nlen,4);		   
			  	break;			  			  

	 		}
	 		//ksw.bKSWStatus = 0xfb; 
	 		break;
     		}
     		
			case MD5_FUNC:
			{
				memset(MD5_Encrypt,0,sizeof(MD5_Encrypt));
				memset(MD5_Decrypt,0,sizeof(MD5_Decrypt));
				memcpy(MD5_Encrypt,g_io_buf + 16,kcw->dKCWOutLength);
				
				if(MD5_Encrypt[0] == 'c')
				    ksw.DKSWSW1 = 0x01;
                else
                    ksw.DKSWSW1 = 0x02;
				
				//ksw.DKSWSW1 = MD5_Encrypt[0];
				Cal_MD5(MD5_Encrypt,MD5_Decrypt);
				memcpy(g_io_buf,MD5_Decrypt,16);
				count = 16;
				break;
			}
     		
     		case ECC_FUNC:  // ECC模块测试
     		{
               switch(kcw->dKCWtag)
               	{
               	case 0:   // 签名 
                            //printf("a.");
                            ka = (BIGINT *)ECC_kA;
               	       bigint_to_char(ka,g_io_buf+16+1024+128);
                            //printf("b."); 
                            start_timer0();
				ecdsa_signature(g_io_buf+16,kcw->dKCWOutLength,g_io_buf+16+1024+128,g_io_buf,&count);
                            nlen = stop_timer0();
			       nlen = 0xffffffff-nlen;		   
			       memcpy(&ksw,&nlen,4);
				//printf("c.");
				//memcpy(g_io_buf,g_io_buf+16+1024,elen);			
				break;
               	case 1: // 验证
               	       sha256_memory(g_io_buf+16,kcw->dKCWOutLength-48,g_io_buf+16+1024);
				pA = (ECC_POINT *)(g_io_buf+16+1024+128+128);
				ka = (BIGINT *)ECC_kA;
				p0 = (ECC_POINT *)(g_io_buf+16+1024+128);
				memcpy(p0->x.data, &ECC_P0x, 4*ECC_MAXLONG);
    				memcpy(p0->y.data, &ECC_P0y, 4*ECC_MAXLONG);
				ecc_point_mul(ka,p0,pA);
				point_to_char(pA,g_io_buf+16+1024+128+128+128);
                            start_timer0();
				if(ecdsa_verification(g_io_buf+16+kcw->dKCWOutLength-48,48,g_io_buf+16+1024,g_io_buf+16+1024+128+128+128))
		              	{
		              	//printf("verification fail!");
					ksw.DKSWSW1 = 0;
                            	}
                            else
		              	{
		              	//printf("verification pass!");
					ksw.DKSWSW1 = 0xfb;	
                            	}
				nlen = stop_timer0();
			       nlen = 0xffffffff-nlen;		   
			       memcpy(&ksw,&nlen,4);			
				break;
               	case 2: // 加密
                            pA = (ECC_POINT *)(g_io_buf+16+1024+128+128);
				ka = (BIGINT *)ECC_kA;
				p0 = (ECC_POINT *)(g_io_buf+16+1024+128);
				memcpy(p0->x.data, &ECC_P0x, 4*ECC_MAXLONG);
    				memcpy(p0->y.data, &ECC_P0y, 4*ECC_MAXLONG);
				ecc_point_mul(ka,p0,pA);
				point_to_char(pA,g_io_buf+16+1024+128+128+128);
                            e_random(g_io_buf+16,kcw->dKCWOutLength,g_io_buf+16+1024+128+128+128,g_io_buf,&count);
				break;
               	case 3: // 解密
               	       ka = (BIGINT *)ECC_kA;
			       bigint_to_char(ka,g_io_buf+16+1024);   
                            d_random(g_io_buf+16,kcw->dKCWOutLength, g_io_buf+16+1024,  g_io_buf+1,&count);
//temp_buffer[rlen]=0;							
//print_string(temp_buffer);
				g_io_buf[0] = count;
				count++;
				//print_string( g_io_buf+1);
			//print_long(count);	
				break;				
			default:		// 密钥对
			       ka = (BIGINT *)(g_io_buf+16+1024);
				p0 = (ECC_POINT *)(g_io_buf+16+1024+128);     
				pA = (ECC_POINT *)(g_io_buf+16+1024+128+256);   
				get_rand_bigint(ka);
				memcpy(p0->x.data, &ECC_P0x, 4*ECC_MAXLONG);
    				memcpy(p0->y.data, &ECC_P0y, 4*ECC_MAXLONG);
				ecc_point_mul(ka,p0,pA);
				memcpy(g_io_buf,&(ka->data[1]),24);
				memcpy(g_io_buf+24,&(pA->x.data[1]),24);
				memcpy(g_io_buf+48,&(pA->y.data[1]),24);
				brev_transfer(g_io_buf,g_io_buf,18);
				count = 72;
				break;			   	
               	}
			break;   
     }	 
     case RNG_FUNC:  // RSA模块测试
     {
	printf("RNG_FUNC handle number = %d,...\n",kcw->flag.ads);
			 switch(kcw->dKCWtag)
               	{
               	case 0:
				count = kcw->dKCWInLength;	
				j = kcw->flag.ads;
				for(i=0;i<j;i++)					
					g_io_buf[i] = Get_Rand();					
				break;
			default:
				nlen=0;
                            for(i=0;i<kcw->flag.ads;i++)
				{
				//printf("1.");
				//if(Rand_test())
				//	nlen++;
                		}
				//nlen=0;
				memcpy(&ksw,&nlen,4);
				//print_long(ksw);
				ksw.DKSWSW1 = 0xfb;
				break;
              	}

		//ksw.bKSWStatus = 0;
               break;
     }
     case SSF33_FUNC:  // RSA模块测试
     {
#if 0 // now no ssf33 in use	 	
	 if((REG32(0x8001ff80)==0x52340651) && (REG32(0x8001ff84)==0x20040813))
	 	{
          SSX33_GENERATE_KEY(temp_buffer+16,temp_buffer+1024);
	   if(kcw->bKCWCLA) // decrypt                     
             SSX33_DECYPT(temp_buffer, temp_buffer+1024);                                 
          else // encrypt              
             SSX33_ENCYPT(temp_buffer, temp_buffer+1024);
	   ksw.bKSWStatus = 0xfb;	  
	 	}
	 else
	 	ksw.bKSWStatus = 0;
#endif	 
          break;
     }	 

     default:
            break;  
           
   }
	//printf("5...%d, \n",count);
	memcpy(g_io_buf+count,&ksw,16);	
	//printf("6...\n");
	
	//usbkey_csw_send(USBS_PASSED,pcbw,count);
		
		}

	ums_csw_send(USBS_PASSED,pcbw);
	

}







