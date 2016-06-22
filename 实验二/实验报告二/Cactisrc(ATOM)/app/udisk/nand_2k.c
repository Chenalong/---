#define _NAND_2K_C_

#include <instructions.h>
#include <bsp.h>
#include <types.h>
#include "nand.h"
#include "nand_media.h"
#include "nfc.h"
#include "udisk.h"
#include "usb_bulk.h"
#include "ums.h"
#include "udiskdes.h"
#include "ecc.h"
#include "nand_2k.h"

/******* nand 2k command list *************/
#define NFC2KCMD_READ_1 0x00
#define NFC2KCMD_READ_2 0x30
#define NFC2KCMD_READ_ID 0x90
#define NFC2KCMD_REAET 0xff
#define NFC2KCMD_PAGEWRITE_1 0x80
#define NFC2KCMD_PAGEWRITE_2 0x10
#define NFC2KCMD_CACHEWRITE_1 0x80
#define NFC2KCMD_CACHEWRITE_2 0x15
#define NFC2KCMD_COPYWRITE_0 0x35
#define NFC2KCMD_COPYWRITE_1 0x85
#define NFC2KCMD_COPYWRITE_2 0x10
#define NFC2KCMD_BOLCKERASE_1 0x60
#define NFC2KCMD_BOLCKERASE_2 0xd0
#define NFC2KCMD_RANDOMIN 0x85
#define NFC2KCMD_RANDOMOUT_1 0x05
#define NFC2KCMD_RANDOMOUT_2 0xe0
#define NFC2KCMD_READ_STATUS 0x70

extern unsigned char nand_addr_segment ;
void nand_set_read_addr_2k(void)
{
	unsigned int segment;						
#if 1
       unsigned char nand_rd_addr[5];
       segment = (nand_addr_rd_phy_blk* nand_pages_per_block + nand_addr_page )*nand_segments_per_page+nand_addr_segment;
	//printf("%x\n ",nand_addr_rd_phy_blk);
	//printf("%x\n ",nand_addr_page);
	//printf("%x\n ",nand_addr_segment);
	//printf("%x ",segment);
       nand_rd_addr[0] = 0x00;
	nand_rd_addr[1] = ((segment<<1)&0x06); 
	nand_rd_addr[2] = ((segment>>2)&0xff);
	nand_rd_addr[3] = ((segment>>10)&0xff);
	nand_set_addr(nand_rd_addr[0]);
	nand_set_addr(nand_rd_addr[1]); 
	nand_set_addr(nand_rd_addr[2]);
	nand_set_addr(nand_rd_addr[3]);
	if (nand_media_sts & nand_media_addr_4cyc)
		nand_set_addr(nand_addr_zone);
#else
	unsigned char nand_rd_addr_msb ;												
	unsigned char nand_rd_addr_mid ;
	unsigned char nand_rd_addr_lsb ;
	page = (((nand_addr_zone *nand_physical_blocks_per_zone) + nand_addr_rd_phy_blk) * nand_pages_per_block) + nand_addr_page ;
	if (nand_media_sts&nand_media_sts_256_page)
 		page <<= 1;
	nand_rd_addr_msb= ((page>>16)&0xff);
 	nand_rd_addr_mid = ((page>>8)&0xff);
	nand_rd_addr_lsb  = (page&0xff); 
	
	nand_set_addr(0x00);
	nand_set_addr(nand_rd_addr_lsb) ; 
	nand_set_addr(nand_rd_addr_mid) ; 
	if (nand_media_sts & nand_media_addr_4cyc)
		nand_set_addr(nand_rd_addr_msb);
	return ;
#endif	
}

void nand_set_write_addr_2k(void)
{
	unsigned int segment; 
	unsigned char nand_wr_addr[5];
	//
	segment = ( nand_addr_wr_phy_blk* nand_pages_per_block + nand_addr_page )*nand_segments_per_page+nand_addr_segment;
	//printf("IN %x \n",segment);
	nand_wr_addr[0] = 0x00;
	nand_wr_addr[1] = ((segment<<1)&0x06); 
	nand_wr_addr[2] = ((segment>>2)&0xff);
	nand_wr_addr[3] = ((segment>>10)&0xff);
	nand_set_addr(nand_wr_addr[0]);
	nand_set_addr(nand_wr_addr[1]); 
	nand_set_addr(nand_wr_addr[2]);
	nand_set_addr(nand_wr_addr[3]);
	if (nand_media_sts & nand_media_addr_4cyc)
		nand_set_addr(nand_addr_zone);
	return ;
}

void nand_set_erase_addr_2k(void)
{
#if 1
	nand_set_addr((nand_addr_wr_phy_blk<<6)&0xff);
	nand_set_addr((nand_addr_wr_phy_blk>>2)&0xff);
	if (nand_media_sts & nand_media_addr_4cyc)
		nand_set_addr(nand_addr_zone&0xff);
	//unsigned int block;
	//block = nand_addr_wr_phy_blk+nand_addr_zone*nand_physical_blocks_per_zone;
	//printf("erase %x\n",nand_addr_wr_phy_blk+nand_addr_zone*nand_physical_blocks_per_zone);
	return ;
#else
	unsigned int segment; 
	unsigned char nand_er_addr[3];
	unsigned char nand_addr_page_save;
	unsigned char nand_addr_segment_save;
	
	nand_addr_page_save=nand_addr_page;
	nand_addr_page=0;
	nand_addr_segment_save=nand_addr_segment;
	nand_addr_segment=0;
	segment = ( nand_addr_wr_phy_blk* nand_pages_per_block + nand_addr_page )*nand_segments_per_page+nand_addr_segment;
	nand_er_addr[0] = ((segment>>2)&0xff);
	nand_er_addr[1] = ((segment>>10)&0xff);
	nand_er_addr[2] = ((segment>>18)&0xff);
	nand_set_addr(nand_er_addr[0]) ; 
	nand_set_addr(nand_er_addr[1]) ; 
	//printf("erase %x\n",segment);
	if (nand_media_sts & nand_media_addr_4cyc)
		nand_set_addr(nand_er_addr[2]);
	nand_addr_page=nand_addr_page_save;
	nand_addr_segment=nand_addr_segment_save;
	return ;
#endif
}

void nand_read_page_test_2k(char *buf,int len,unsigned int page)
{
	unsigned int i;
	nand_set_cmd(NFC2KCMD_READ_1);// Ð´0x00µ½ÃüÁî¼Ä´æÆ÷È¥¿ªÆô¶ÁÃüÁî¡£
	nand_set_addr(0x00);
	nand_set_addr(0x00);
	nand_set_addr((page>>2)&0xff);
	nand_set_addr((page>>10)&0xff);
	if (nand_media_sts & nand_media_addr_4cyc)
		nand_set_addr((page>>18)&0xff);
	//nand_set_read_addr_2k();//  Ð´µØÖ·µ½µØÖ·¼Ä´æÆ÷¡£
	nand_set_cmd(NFC2KCMD_READ_2);// Ð´0x30µ½ÃüÁî¼Ä´æÆ÷È¥¿ªÆô¶ÁÃüÁî¡£
	//reset ECC here
	nand_reset_ecc();
	//
	if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
		{
		nand_reset_device();
		printf("nand not ready!");
		return;
		}
		for(i = 0; i < len; i++)
			{
			buf[i] = nand_get_data();
			}
		
		//
		if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
			{
			nand_reset_device();
			printf("nand not ready!");
		       return;
			}

	//
	nand_set_standby();

}

void nand_write_page_test_2k(char *buf,int len,unsigned int page)
{
	unsigned int i;
	unsigned char sts;
	
	nand_set_cmd(NFC2KCMD_PAGEWRITE_1);
	nand_set_addr(page&0xff);
	nand_set_addr((page>>8)&0x07);
	nand_set_addr((page>>11)&0xff);
	nand_set_addr((page>>19)&0xff);
	if (nand_media_sts & nand_media_addr_4cyc)
		nand_set_addr((page>>27)&0xff);
	//reset ECC here
	nand_reset_ecc();
	//
		for(i = 0; i < len; i++)
			{
			nand_set_data(buf[i]);
			}
	nand_set_cmd(NFC2KCMD_PAGEWRITE_2);	
		//
		if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
			{
			nand_reset_device();
			printf("nand not ready!");
		       return;
			}
	nand_set_cmd(NFC2KCMD_READ_STATUS);
	sts = nand_get_data();
	if(sts&1)
		printf("nand write error! 0x%02x, \n",sts);
	else
		printf("nand write success! 0x%02x, \n",sts);
	
	//
	nand_set_standby();

}

void nand_erase_page_test_2k(int segment)
{
	unsigned int i;
	unsigned char sts;
	
	nand_set_cmd(NFC2KCMD_BOLCKERASE_1);
	nand_set_addr(((segment>>2)&0xff));
	nand_set_addr(((segment>>10)&0xff));
	//reset ECC here
	nand_reset_ecc();
	
	nand_set_cmd(NFC2KCMD_BOLCKERASE_2);	
		//
		if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
			{
			nand_reset_device();
			printf("nand not ready!");
		       return;
			}
	nand_set_cmd(NFC2KCMD_READ_STATUS);
	sts = nand_get_data();
	if(sts&1)
		printf("nand erase error! 0x%02x, \n",sts);
	else
		printf("nand erase success! 0x%02x, \n",sts);
	
	//
	nand_set_standby();
}

unsigned int nand_erase_block_2k(void)
{
	unsigned int result = K_SUCCES;


	nand_set_cmd(NFC2KCMD_BOLCKERASE_1);
	nand_set_erase_addr_2k();
	nand_set_cmd_ex(NFC2KCMD_BOLCKERASE_2);
	if(K_SUCCES!= (result = nand_wait_rdy_with_timeout(k_nand_busy_read_timeout)))
		{
		nand_reset_device();
		}
	//maybe it should read status
	nand_set_cmd_ex(NFC2KCMD_READ_STATUS);
	while((nand_get_data()&(0x40))!=0x40);
	//
	nand_set_standby();
	return result;
}

unsigned int nand_erase_block_test_2k(block)
{
	unsigned int result = K_SUCCES;


	nand_set_cmd(NFC2KCMD_BOLCKERASE_1);
	nand_set_addr((block<<6)&0xff);
	nand_set_addr((block>>2)&0xff);
	if (nand_media_sts & nand_media_addr_4cyc)
		nand_set_addr((block>>10)&0xff);
	nand_set_cmd_ex(NFC2KCMD_BOLCKERASE_2);
	if(K_SUCCES!= (result = nand_wait_rdy_with_timeout(k_nand_busy_read_timeout)))
		{
		nand_reset_device();
		}
	//maybe it should read status
	nand_set_cmd_ex(NFC2KCMD_READ_STATUS);
	while((nand_get_data()&(0x40))!=0x40);
	//
	nand_set_standby();
	return result;
}


unsigned int nand_check_data_status_valid_2k(unsigned char page, unsigned char count)
{
	unsigned char page_save;
	unsigned int result;
	unsigned char data_status;

	page_save = nand_addr_page;
	nand_addr_page = page;
	result = K_SUCCES;

	nand_set_cmd(NFC2KCMD_READ_1);
	nand_set_addr(page&0xff);
	nand_set_addr(((page>>8)&0x07)|0x08); 
	nand_set_addr((page>>11)&0xff);
	nand_set_addr((page>>19)&0xff);
	if (nand_media_sts & nand_media_addr_4cyc)
		nand_set_addr(nand_addr_zone);
	nand_set_cmd(NFC2KCMD_READ_2);
	//reset ECC here
	nand_reset_ecc();
	//
	while(count --)
		{
		if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
			{
			nand_reset_device();
			result = K_ERROR;
			}
		nand_get_data();
		nand_get_data();
		nand_get_data();
		nand_get_data();
		data_status = nand_get_data();
		nand_get_data();
		nand_get_data();
		nand_get_data();
		nand_get_data();
		nand_get_data();
		nand_get_data();
		nand_get_data();
		nand_get_data();
		nand_get_data();
		nand_get_data();
		nand_get_data();
		if(data_status != 0xFF)
			{
			if(bit_count8(data_status) <4)
				{
				result = K_ERROR;
				break;
				}
			}
		}
	if(K_SUCCES!= (result = nand_wait_rdy_with_timeout(k_nand_busy_read_timeout)))
		{
		nand_reset_device();
		}
	nand_set_standby();
	nand_addr_page = page_save;
	return result;
}

unsigned int nand_check_data_status_blank_2k()
{
    	unsigned int i;
	unsigned char addr_save;
	addr_save = nand_addr_rd_phy_blk;
	nand_addr_rd_phy_blk = nand_addr_wr_phy_blk ;
	nand_read_extra_data_2k();
	nand_addr_rd_phy_blk = addr_save;
	//printf("nand_redt_data[6]=%x",nand_redt_data[6]);
	for(i=0;i<nand_redt_buffer_size;i++)
		if(nand_redt_data[i]!=0xff)
			return K_ERROR;
	return K_SUCCES;
}

void nand_cmp_read_write(int page)
{
    unsigned int i,j,*ptr;
    unsigned char my_buf[2048+80],a,sts;
    ptr = (int *)my_buf;
    for(i=0;i<(512+16);i++)
    ptr[i] = Get_Rand();
    //nand_write_page_test_2k(my_buf,2048+64,page);
    nand_set_cmd(NFC2KCMD_PAGEWRITE_1);
	nand_set_read_addr_2k();
	
	//reset ECC here
	nand_reset_ecc();
	//
		for(i = 0; i < 2048+64; i++)
			{
			nand_set_data(my_buf[i]);
			}
	nand_set_cmd(NFC2KCMD_PAGEWRITE_2);	
		//
		if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
			{
			nand_reset_device();
			printf("nand not ready!");
		       return;
			}
	nand_set_cmd(NFC2KCMD_READ_STATUS);
	sts = nand_get_data();
	if(sts&1)
		printf("nand write error! 0x%02x, \n",sts);
	else
		printf("nand write success! 0x%02x, \n",sts);
	
	//
	nand_set_standby();
	
    nand_set_cmd(NFC2KCMD_READ_1);// Ð´0x00µ½ÃüÁî¼Ä´æÆ÷È¥¿ªÆô¶ÁÃüÁî¡£ 
	nand_set_read_addr_2k();//  Ð´µØÖ·µ½µØÖ·¼Ä´æÆ÷¡£
	nand_set_cmd(NFC2KCMD_READ_2);// Ð´0x30µ½ÃüÁî¼Ä´æÆ÷È¥¿ªÆô¶ÁÃüÁî¡£
	//reset ECC here
	nand_reset_ecc();
	//
	if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
		{
		nand_reset_device();
		printf("nand not ready!");
		return;
		}
	for(j = 0; j < 2048+64; j++)
	    {
            if ( nand_get_data() != my_buf[j])
		      printf("an error have!!");  
	    }
	if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
	    {
	        nand_reset_device();
	        printf("nand not ready!");
        }
	nand_set_standby();
    return ;
} 

void  nand_read_segment_2k(void)
{
	unsigned int i;
	nand_set_cmd(NFC2KCMD_READ_1);
	nand_set_read_addr_2k();
	nand_set_cmd(NFC2KCMD_READ_2);
	//reset ECC here
	nand_reset_ecc();
	//
	if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
		{
		nand_reset_device();
		printf("nand not ready!");
		return ;
		}
	for(i = 0; i < 0x200; i++)
		{
		nand_page_buff[i] = nand_get_data();
		//printf("%x = %x ",i,nand_page_buff[i]);
		//if ((i&0x0f)==0x0f)
			//printf("\n");
		}
	if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
		{
		nand_reset_device();
		return ;
		}
	nand_set_cmd(NFC2KCMD_RANDOMOUT_1);
	nand_set_addr(0x00+nand_addr_segment*nand_redt_buffer_size);
	nand_set_addr(0x08); 
	nand_set_cmd(NFC2KCMD_RANDOMOUT_2);
	for(i = 0; i < nand_redt_buffer_size; i++)
		{
		nand_redt_data[i] = nand_get_data();
		//printf(" %x ",nand_redt_data[i]);
		}
	//printf("read a segment redt data!!! \n");
	if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
		{
		nand_reset_device();
		return ;
		}
	nand_set_cmd_ex(NFC2KCMD_READ_STATUS);
	while((nand_get_data()&(kbm_nand_status_ready))!=kbm_nand_status_ready);
	//nand_read_extra_data_2k();
	//for(i = 0; i < nand_redt_buffer_size; i++)
		//{
		//nand_redt_data[i] = nand_get_data();
		//printf(" %x ",nand_redt_data[i]);
		//}
	//printf("read a segment redt data!!! \n");
	nand_set_standby();
	//return ;
		
}

void  nand_read_extra_data_2k (void)
{
    	unsigned int i,sts;
    	unsigned int segment;
	segment = ( nand_addr_rd_phy_blk* nand_pages_per_block + nand_addr_page )*nand_segments_per_page+nand_addr_segment;
    	nand_set_cmd(NFC2KCMD_READ_1);
	nand_set_addr(0x00+nand_addr_segment*nand_redt_buffer_size);
	nand_set_addr(0x08); 
	nand_set_addr((segment>>2)&0xff);
	nand_set_addr((segment>>10)&0xff);
	if (nand_media_sts & nand_media_addr_4cyc)
		nand_set_addr(nand_addr_zone);
	nand_set_cmd(NFC2KCMD_READ_2);
	//reset ECC here
	nand_reset_ecc();
	//
	if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
		{
		nand_reset_device();
		printf("nand not ready!");
		return K_ERROR;
		}
	//printf("\n %x",nand_addr_wr_phy_blk);
	//printf("\n %x",nand_addr_page);
	//printf("\n %x",nand_addr_segment);
	//printf("\n %x",segment);
	nand_redt_data[0] = nand_get_data();
	nand_redt_data[1] = nand_get_data();
	nand_redt_data[2] = nand_get_data();
	nand_redt_data[3] = nand_get_data();
	nand_redt_data[4] = nand_get_data();
	nand_redt_data[5] = nand_get_data();
	nand_redt_data[6] = nand_get_data();
	nand_redt_data[7] = nand_get_data();
	nand_redt_data[8] = nand_get_data();
	nand_redt_data[9] = nand_get_data();
	nand_redt_data[10] = nand_get_data();
	nand_redt_data[11] = nand_get_data();
	nand_redt_data[12] = nand_get_data();
	nand_redt_data[13] = nand_get_data();
	nand_redt_data[14] = nand_get_data();
	nand_redt_data[15] = nand_get_data();
		
		if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
			{
			nand_reset_device();
			printf("nand not ready!");
		       return K_ERROR;
			}
	//printf("nand_redt_data[6]=%x",nand_redt_data[6]);
	nand_set_standby();
       return K_SUCCES;
}

void nand_write_extra_date_2k(void)
{
	unsigned int i,sts;
    	unsigned int segment;
		
	segment = ( nand_addr_wr_phy_blk* nand_pages_per_block + nand_addr_page )*nand_segments_per_page+nand_addr_segment;
	//printf("%x ",segment);
	nand_set_cmd(NFC2KCMD_PAGEWRITE_1);
	nand_set_addr(0x00+nand_addr_segment*nand_redt_buffer_size);
	nand_set_addr(0x08); 
	nand_set_addr((segment>>2)&0xff);
	nand_set_addr((segment>>10)&0xff);
	if (nand_media_sts & nand_media_addr_4cyc)
		nand_set_addr((segment>>18)&0xff);
	//reset ECC here
	nand_reset_ecc();
	//
	//printf("nand_write_extra_date_2k");
	for(i = 0; i <nand_redt_buffer_size; i++)
		{
		nand_set_data(nand_redt_data[i]);
		//printf("%3x=%2x ",i+512,nand_redt_data[i]);
		}
	//for(i = 0; i <nand_redt_buffer_size; i++)
		//printf("%3x=%2x ",i+2048+nand_addr_segment*nand_segments_per_page,nand_redt_data[i]);
		//printf("\n\n");
	//CheckWriteProtectInWriting();
	nand_set_cmd(NFC2KCMD_PAGEWRITE_2);	
		//
	if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
		{
		nand_reset_device();
		printf("nand not ready!");
		return;
		}
	while(!nand_is_ready())
		{
		udisk_usb_bulk_out_ex();
		des_encrypt();
		//writing protect detecting
		//CheckWriteProtectInWriting();
		des_encrypt();
		}
	//maybe it should read status
	nand_set_cmd_ex(NFC2KCMD_READ_STATUS);
	sts = nand_get_data();
	if(sts&1)
		printf("nand write error! 0x%02x, \n",sts);
	else
		printf("nand write success! 0x%02x, \n",sts);
	while((sts&(0x40))!=0x40);
		{
		udisk_usb_bulk_out_ex();
		des_encrypt();
		//writing protect detecting
		//CheckWriteProtectInWriting();
		des_encrypt();
		}
	if( kbm_nand_status_write_failed & i)
		{
		//wait data transfer end
		while(FALSE == udisk_usb_bulk_done_ex())
			{
			udisk_usb_bulk_out_ex();
			}
		//here should do write error dealing
		//nand_map_dealing_writing_error(buff);
		return ;
		}
	nand_set_standby();
}

void nand_write_segment_test_2k(segment)
{
	unsigned int i,*ptr;
	unsigned char my_buf[512+16],sts;
	
    	ptr = (int *)my_buf;
   	 for(i=0;i<(128+4);i++)
    		ptr[i] = Get_Rand();
	nand_set_cmd(NFC2KCMD_PAGEWRITE_1);
	nand_set_addr(segment&0xff);
	nand_set_addr((segment>>8)&0x07);
	nand_set_addr((segment>>11)&0xff);
	nand_set_addr((segment>>19)&0xff);
	if (nand_media_sts & nand_media_addr_4cyc)
		nand_set_addr((segment>>27)&0xff);
	//reset ECC here
	nand_reset_ecc();
	//
		for(i = 0; i <512; i++)
			{
			nand_set_data(my_buf[i]);
			}
	nand_set_cmd(NFC2KCMD_RANDOMIN);
	nand_addr_segment= ((segment/512)% (nand_pages_per_block *nand_segments_per_page)) % nand_segments_per_page ; 
		//
		if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
			{
			nand_reset_device();
			printf("nand not ready!");
		       return;
			}
	
	//nand_set_cmd(NFC2KCMD_PAGEWRITE_1);
	nand_set_addr(0x00+nand_addr_segment*nand_redt_buffer_size);
	nand_set_addr(0x08); 
	//nand_set_addr((segment>>11)&0xff);
	//nand_set_addr((segment>>19)&0xff);
	//reset ECC here
	nand_reset_ecc();
	for(i = 512; i <512+16; i++)
			{
			nand_set_data(my_buf[i]);
			}
	
	/*nand_set_cmd(NFC2KCMD_READ_STATUS);
	sts = nand_get_data();
	if(sts&1)
		printf("nand write error! 0x%02x, \n",sts);
	else
		printf("nand write success! 0x%02x, \n",sts);*/
	
	
	//
		
	nand_set_cmd(NFC2KCMD_PAGEWRITE_2);	
		//
		if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
			{
			nand_reset_device();
			printf("nand not ready!");
		       return;
			}
	nand_set_cmd(NFC2KCMD_READ_STATUS);
	sts = nand_get_data();
	if(sts&1)
		printf("nand write error! 0x%02x, \n",sts);
	else
		printf("nand write success! 0x%02x, \n",sts);
	//
	nand_set_standby();
}

unsigned int nand_write_segment_2k(void)
{
	unsigned int i;
	unsigned char sts;
	
    	nand_set_cmd(NFC2KCMD_PAGEWRITE_1);
	nand_set_write_addr_2k();
	//reset ECC here
	nand_reset_ecc();
	//
	for(i = 0; i <512; i++)
		{
		nand_set_data(nand_page_buff[i]);
		}
	nand_get_hw_and_set_sw_ecc();
	nand_set_cmd(NFC2KCMD_RANDOMIN);
	if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
			{
			nand_reset_device();
			printf("nand not ready!");
		       return;
			}
	
	//nand_set_cmd(NFC2KCMD_PAGEWRITE_1);
	nand_set_addr(0x00+nand_addr_segment*nand_redt_buffer_size);
	nand_set_addr(0x08); 
	//nand_set_addr((segment>>11)&0xff);
	//nand_set_addr((segment>>19)&0xff);
	//reset ECC here
	nand_reset_ecc();
	for(i = 0; i <nand_redt_buffer_size; i++)
			{
			nand_set_data(nand_redt_data[i]);
			//printf(" %x ",nand_redt_data[i]);
			}
	//printf(" 2k segment write ok!!!!!!!!\n");
	CheckWriteProtectInWriting();
	
	nand_set_cmd(NFC2KCMD_PAGEWRITE_2);	
		//
	if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
		{
		nand_reset_device();
		printf("nand not ready!");
	       return K_ERROR;
		}
	nand_set_cmd(NFC2KCMD_READ_STATUS);
	while(((i=nand_get_data())&(kbm_nand_status_ready))!=kbm_nand_status_ready)
			{
				//writing protect detecting
				CheckWriteProtectInWriting();
			}
	nand_set_standby();
	if(i & kbm_nand_status_write_failed)
		{
		return K_ERROR;
		}
	return K_SUCCES;
}

unsigned int nand_read_begin_split_2k(unsigned int * udisk_lba_param)
{
	unsigned short sector_count;
	unsigned char buff_ptr = 0;
	unsigned short i,j;
	unsigned char * buff;
	unsigned int new_buff512[512/4];
	unsigned int ecc_get;
#ifdef DISK_TEST_MODE_ENABLE		
printf("\nR nand_addr_start = 0x%08x\n",udisk_lba_param[index_udisk_lba_start]);
printf("r nand_write_segment_num = 0x%08x\n",udisk_lba_param[index_udisk_lba_xfer_count]);
	//printf("addr from s %x\n",(unsigned int)new_buff512);
#endif
	if(K_SUCCES != nand_map_lba2addr_rd(udisk_lba_param[index_udisk_lba_start]))
		{
			return K_ERROR;
		}
	if(K_SUCCES == nand_media_block_has_bad_data())
		{
		if(K_SUCCES != nand_check_data_status_valid_2k(nand_addr_page,udisk_lba_param[index_udisk_lba_split_count]))
			{
			nand_reset_device();
			return K_ERROR;
			}
		nand_reset_device();
		}
#ifdef DISK_TEST_MODE_ENABLE		
print_goble();
#endif
	sector_count = udisk_lba_param[index_udisk_lba_split_count];
	udisk_bulk_init_ex();
	start_des_data_init();
	//------------------
	//printf("lba_split_count=%d\n",sector_count);
	//-----------------
	while(sector_count)
		{
		/*if(TRUE == udisk_rd_wr_self_enable)
			{
			//printf("\n%x\n",sector_count);
			nand_read_segment_2k();
			nand_read_extra_data_2k();
			}
		else*/
		//-------------------------------------------
			{
			//nand_read_segment_2k();
			nand_set_cmd(NFC2KCMD_READ_1);
			nand_set_read_addr_2k();
			nand_set_cmd(NFC2KCMD_READ_2);
			//reset ECC here
			nand_reset_ecc();
			if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
				{
				nand_reset_device();
				printf("nand not ready!");
				return;
				}
			buff = ((buff_ptr & 0x01)?nand_page_buff : (unsigned char *)new_buff512);
			buff_ptr ++;
			//get data in buff
					{
					for(i = 0; i < 512;)
						{
						buff[i] = nand_get_data();
						buff[i+1] = nand_get_data();
						buff[i+2] = nand_get_data();
						buff[i+3] = nand_get_data();
						buff[i+4] = nand_get_data();
						buff[i+5] = nand_get_data();
						buff[i+6] = nand_get_data();
						buff[i+7] = nand_get_data();
						buff[i+8] = nand_get_data();
						buff[i+9] = nand_get_data();
						buff[i+10] = nand_get_data();
						buff[i+11] = nand_get_data();
						buff[i+12] = nand_get_data();
						buff[i+13] = nand_get_data();
						buff[i+14] = nand_get_data();
						buff[i+15] = nand_get_data();
						//printf("%x\n",buff[i]);
						i +=16;
						des_decrypt();
						udisk_usb_bulk_in_ex();
						}
					}
			//here do ecc check
			ecc_get =nand_get_ecc();
			//
			if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
				{
				nand_reset_device();
				return ;
				}
			nand_set_cmd(NFC2KCMD_RANDOMOUT_1);
			nand_set_addr(0x00+nand_addr_segment*nand_redt_buffer_size);
			nand_set_addr(0x08); 
			nand_set_cmd(NFC2KCMD_RANDOMOUT_2);
			for(i = 0; i < nand_redt_buffer_size; i++)
				{
				nand_redt_data[i] = nand_get_data();
				//printf(" %x ",nand_redt_data[i]);
				}
			//printf("\n");
			//printf("read a segment redt data!!! \n");
			if(ECC_UNCORRECTABLE_ERROR == ecc_check_and_correct(buff,(unsigned char*)(&ecc_get)))
				{
				//sensePtr = ;
				;//return K_ERROR;
				//printf("nand_read_begin_split error\nnand_addr_rd_phy_blk:%x\n",nand_addr_rd_phy_blk);
			
				}
			//
			//wait usb send end
			while(FALSE == udisk_usb_bulk_done_ex())
				{
				des_decrypt();
				udisk_usb_bulk_in_ex();
				}
			//start usb send
			start_des_data_crypt(buff);
			udisk_usb_bulk_start_ex(buff);
			//
			}
		//---------------------------------------------------
		while(!nand_is_ready())
			{
			des_decrypt();
			udisk_usb_bulk_in_ex();
			}
		nand_addr_segment+=1;
		if (nand_addr_segment==nand_segments_per_page)
			{
			nand_addr_page+=1;
			nand_addr_segment=0;
			}
		sector_count --;
		}
	//wait usb send end
	while(FALSE == udisk_usb_bulk_done_ex())
		{
		des_decrypt();
		udisk_usb_bulk_in_ex();
		}
	return K_SUCCES;
}

unsigned int nand_block_have_enough_segment(unsigned int * udisk_lba_param)
{
	unsigned int i;
	unsigned char addr_page_save,addr_segment_save;
	
	addr_page_save=nand_addr_page;
	addr_segment_save=nand_addr_segment;
	for(i=0;i<udisk_lba_param[index_udisk_lba_split_max_count];i++)
		{
		if(K_SUCCES != nand_check_data_status_blank_2k())
			{
			//printf("This segment is not blank !!\n");
			nand_addr_page=addr_page_save;
			nand_addr_segment=addr_segment_save;
			return 0;			
			}
		nand_addr_segment++;
		if (nand_addr_segment==nand_segments_per_page)
			{
			nand_addr_segment=0;
			nand_addr_page++;
			}
		}
	nand_addr_page=addr_page_save;
	nand_addr_segment=addr_segment_save;
	return 1;
	
	
}

unsigned int nand_write_begin_split_2k(unsigned int * udisk_lba_param)
{
	unsigned char _nand_addr_zone;
	unsigned short i,j;
	unsigned short sector_count;
	unsigned char * buff;
	unsigned int new_buff512[512/4];
	unsigned char buff_ptr ;

	sector_count = udisk_lba_param[index_udisk_lba_split_max_count];
	new_buff512[12] = 0;
	//
	udisk_bulk_init_ex();
	start_des_data_init();
	//
	if(sector_count)
		{
		start_des_data_crypt(nand_page_buff);
		udisk_usb_bulk_start_ex(nand_page_buff);
		}
	buff_ptr = 0;
	while(sector_count)
		{
		//printf("1.write NO.%x sector !!",sector_count);
		//wait usb receive 512 BYTEs end
		while(FALSE == udisk_usb_bulk_done_ex())
			{
			//printf("%x\n",bulk_buff_ptr);
			des_encrypt();
			udisk_usb_bulk_out_ex();
			des_encrypt();
			//printf("udisk_usb_bulk_done_ex error !!!!!");
			}
		while(FALSE == des_done())
			{
			printf("des_done error !!!!!");
			des_encrypt();
			des_encrypt();
			}
		//--------------------------
		CheckWriteProtectInWriting();
		//
		nand_set_cmd(NFC2KCMD_PAGEWRITE_1);
		nand_set_write_addr_2k();
		//reset ECC here
		nand_reset_ecc();
		//---------------------------
		if(sector_count >1)
			{
			buff = ((buff_ptr & 0x01) ? nand_page_buff : (unsigned char *)new_buff512);
			start_des_data_crypt(buff);
			udisk_usb_bulk_start_ex(buff);
			}
		//---------------------------------
			{
			//change to buff which has been filled with data
			buff = ((buff_ptr & 0x01) ? (unsigned char *)new_buff512:nand_page_buff);
			buff_ptr ++;
				{
			//#ifdef DISK_TEST_MODE_ENABLE
			//for (i=0;i<512;i++)
				//{
				//printf("%3x=%2x  ",i,buff[i]);
				//if ((i&0x0f)==0x0f)
					//{printf("\n");}
				//}
			//#endif
				for(i = 0; i < 512;)
					{
					nand_set_data(buff[i]);
					nand_set_data(buff[i+1]);
					nand_set_data(buff[i+2]);
					nand_set_data(buff[i+3]);
					nand_set_data(buff[i+4]);
					nand_set_data(buff[i+5]);
					nand_set_data(buff[i+6]);
					nand_set_data(buff[i+7]);
					nand_set_data(buff[i+8]);
					nand_set_data(buff[i+9]);
					nand_set_data(buff[i+10]);
					nand_set_data(buff[i+11]);
					nand_set_data(buff[i+12]);
					nand_set_data(buff[i+13]);
					nand_set_data(buff[i+14]);
					nand_set_data(buff[i+15]);
					i += 16;
					//udisk_usb_bulk_out_ex();
					}
			
				}
			//set ECC here
			nand_get_hw_and_set_sw_ecc();
			udisk_usb_bulk_out_ex();
			//
			nand_set_cmd(NFC2KCMD_RANDOMIN);
			if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
				{
				nand_reset_device();
				printf("nand not ready!");
		       	return;
				}
			nand_set_addr(0x00+nand_addr_segment*nand_redt_buffer_size);
			nand_set_addr(0x08); 
			nand_reset_ecc();
			for(i = 0; i < nand_redt_buffer_size; i++)
				{
				nand_set_data(nand_redt_data[i]);
				//printf("%x = %x ",i,nand_redt_data[i]);
				//nand_set_data(i);
				}
			//printf("\n");
			CheckWriteProtectInWriting();
			//
			udisk_usb_bulk_out_ex();
			//
			nand_set_cmd_ex(NFC2KCMD_COPYWRITE_2);
			//--------------------------------------------
			while(!nand_is_ready())
				{
				udisk_usb_bulk_out_ex();
				des_encrypt();
				//writing protect detecting
				//CheckWriteProtectInWriting();
				des_encrypt();
				}
			//maybe it should read status
			nand_set_cmd_ex(NFC2KCMD_READ_STATUS);
			while(((i=nand_get_data())&(0x40))!=0x40);
				{
				udisk_usb_bulk_out_ex();
				des_encrypt();
				//writing protect detecting
				CheckWriteProtectInWriting();
				des_encrypt();
				}
			if( kbm_nand_status_write_failed & i)
				{
				//wait data transfer end
				while(FALSE == udisk_usb_bulk_done_ex())
					{
					udisk_usb_bulk_out_ex();
					}
				//here should do write error dealing
				nand_map_dealing_writing_error(buff);
				return K_ERROR;
				}
			nand_addr_segment++;
			if (nand_addr_segment==nand_segments_per_page)
				{
				nand_addr_page+=1;
				nand_addr_segment=0;
				}
			sector_count --;
			}
		}
	return K_SUCCES;
}

unsigned int nand_write_end_xfer_2k(unsigned int * udisk_lba_param)
{
	if(nand_addr_page!= nand_pages_per_block)
		{
		if(K_SUCCES != nand_media_copy_block_tail())
			{
			return K_ERROR;
			}
		}
		
	return K_SUCCES;
}

void nand2k_media_page_copy()
{
       unsigned int i;
	if(nand_addr_wr_phy_blk == nand_map_block_free )
		{
		return K_ERROR;
		}
	
	if(nand_addr_rd_phy_blk == nand_map_block_free)
		{		
		return K_ERROR;
		}
	nand_set_cmd(NFC2KCMD_READ_1);
	nand_set_read_addr_2k();
	nand_set_cmd(NFC2KCMD_COPYWRITE_0);
	// wait for ready
	if(K_SUCCES!= nand_wait_rdy_with_timeout(2))
		{
		nand_reset_device();
		printf("nand not ready!");
		return ;
		}
	nand_set_cmd(NFC2KCMD_COPYWRITE_1);
	nand_set_write_addr_2k();
	nand_set_cmd(NFC2KCMD_COPYWRITE_2);
	if(K_SUCCES!= nand_wait_rdy_with_timeout(2))
		{
		nand_reset_device();
		printf("nand not ready!");
	       return K_ERROR;
		}
	nand_set_cmd(NFC2KCMD_READ_STATUS);
	while(((i=nand_get_data())&(kbm_nand_status_ready))!=kbm_nand_status_ready)
			{
				//writing protect detecting
				CheckWriteProtectInWriting();
			}
	nand_set_standby();
	if(i & kbm_nand_status_write_failed)
		{
		return K_ERROR;
		}	
	return K_SUCCES;
}	

	

