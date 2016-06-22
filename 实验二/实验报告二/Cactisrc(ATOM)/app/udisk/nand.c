
//#ifndef _NAND_C_
//#define _NAND_C_

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
//#include <usbdrv.h>

//writing protect detecting
//#define 	CheckWriteProtectInWriting()		;
//extern udisk_rd_wr_self_enable;

				
void nand_set_read_addr(void) //设置读地址 
{
	unsigned int page;					
	unsigned char nand_rd_addr_msb ;												
	unsigned char nand_rd_addr_mid ;
	unsigned char nand_rd_addr_lsb ;
	//
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
}
void nand_set_write_addr(void)//设置写地址 
{
	unsigned int page; 
	unsigned char nand_wr_addr_msb ;
	unsigned char nand_wr_addr_mid ;
	unsigned char nand_wr_addr_lsb ;
	//
	page = (((nand_addr_zone * nand_physical_blocks_per_zone) + nand_addr_wr_phy_blk) * nand_pages_per_block) + nand_addr_page ;
	//printf("%x\n ",page);
	//printf("IN %x \n",page);
	if (nand_media_sts&nand_media_sts_256_page)
 		page <<= 1;
	nand_wr_addr_msb= ((page>>16)&0xff);
 	nand_wr_addr_mid = ((page>>8)&0xff);
	nand_wr_addr_lsb  = (page&0xff);
	
	nand_set_addr(0x00);
	nand_set_addr(nand_wr_addr_lsb) ; 
	nand_set_addr(nand_wr_addr_mid) ; 
	if (nand_media_sts & nand_media_addr_4cyc)
		nand_set_addr(nand_wr_addr_msb);
	return ;
}
void nand_set_erase_addr(void)//设置擦写地址 
{
	unsigned int page; 
	unsigned char nand_wr_addr_msb ;
	unsigned char nand_wr_addr_mid ;
	unsigned char nand_wr_addr_lsb ;
	unsigned char save_page;
	
	save_page = nand_addr_page ;
	nand_addr_page=0;
	//
	page = (((nand_addr_zone * nand_physical_blocks_per_zone) + nand_addr_wr_phy_blk) * nand_pages_per_block) + nand_addr_page ;
	if (nand_media_sts&nand_media_sts_256_page)
 		page <<= 1;
	nand_wr_addr_msb= ((page>>16)&0xff);
 	nand_wr_addr_mid = ((page>>8)&0xff);
	nand_wr_addr_lsb  = (page&0xff);  
	//
	nand_addr_page=save_page ;
	
	nand_set_addr(nand_wr_addr_lsb) ; 
	nand_set_addr(nand_wr_addr_mid) ; 
	//printf("erase %x\n",page);
	if (nand_media_sts & nand_media_addr_4cyc)
		nand_set_addr(nand_wr_addr_msb);
	return ;
}
void nand_get_hw_and_set_sw_ecc(void)
{
	unsigned int ecc ;
	ecc = nand_get_ecc();
	nand_redt_data[redt_ecc1_0] = nand_redt_data[redt_ecc2_0] = (ecc & 0xFF);
	nand_redt_data[redt_ecc1_1] = nand_redt_data[redt_ecc2_1] = ((ecc>>8) & 0xFF);
	nand_redt_data[redt_ecc1_2] = nand_redt_data[redt_ecc2_2] = ((ecc>>16) & 0xFF);
}
unsigned int nand_wait_rdy_with_timeout(unsigned int ticks)
{
	/*unsigned int timer = timer0_get_ms() + ticks;
	while(1)
		{
		if(get_nfsr_rb())
			return K_SUCCES;
		if(timer < timer0_get_ms())
			return k_TIMEOUT;
		}*/
	unsigned short i;

	//printf("nand_wait_rdy_with_timeout-----------");
	do
		{
		//i = 500;
		i = 50;
		while(i--);
		if(nand_is_ready())
			{
			return K_SUCCES;		
			}
		}while(1);
	return k_TIMEOUT;
}

void nand_reset_device(void)
{	
	nand_set_cmd(NFCMD_RST);
	nand_wait_rdy_with_timeout(k_nand_busy_reset_timeout) ;
	nand_set_standby();
	return;
}

void nand_soft_reset(void)
{
	nand_set_cmd(NFCMD_RST);
	nand_wait_rdy_with_timeout(k_nand_busy_reset_timeout) ;
	//
	nand_set_cmd(NFCMD_READ0);
	nand_wait_rdy_with_timeout(k_nand_busy_read_timeout);
	//
	nand_set_standby();
	return;
}

void nand_read_id_ex(void)
{
	return;
}
void nand_read_id(unsigned char * id_list)
{
	unsigned char nand_id_ex_1;
	unsigned char nand_has_unique_id;

	nand_set_cmd(NFCMD_READID);
	nand_set_addr(0);
	id_list[nand_id_maker]  = nand_get_data() ;
	id_list[nand_id_device] = nand_get_data() ;
	id_list[nand_id_3] = nand_get_data() ;
 	id_list[nand_id_4] = nand_get_data() ;
 	id_list[nand_id_5] = nand_get_data() ;
	nand_set_standby();
	//
	nand_id_ex_1=0 ;
	if (nand_id_4 == 0xc0)
		nand_read_id_ex() ;
	return ;
}
unsigned int nand_read_extra_data_test(unsigned int plba,unsigned int count)
{
	unsigned char t0,t1,t2; 
	if(count == 0)
		return K_ERROR;
	//
	t0 = nand_addr_zone;
	t1 = nand_addr_rd_phy_blk;
	t2 = nand_addr_page;
	//
	//
	while(count --)
		{
		//
		printf("PHY Block: %x\n",plba);
		//
		nand_addr_zone = (plba/(1024*32));
		nand_addr_rd_phy_blk = ((plba%(1024*32))/32);
		nand_addr_page = plba%32;
		plba++;
		//
		//printf("nand_addr_zone:              %x\n",nand_addr_zone);
		//printf("nand_addr_rd_phy_blk:     %x\n",nand_addr_rd_phy_blk);
		//printf("nand_addr_page:             %x\n",nand_addr_page);
		//
		nand_read_extra_data();
		//
		debug_page_buff(nand_redt_data,16);
		//
		}
	//
	nand_addr_zone = t0;
	nand_addr_rd_phy_blk = t1;
	nand_addr_page =t2;
	//
	return K_SUCCES;
}
unsigned int nand_read_extra_data(void)
{
	unsigned int i;
	
	nand_set_cmd(NFCMD_READ_REDT);
	nand_set_read_addr();
	//
	//printf("nand.c nand_read_extra_data() 0\n");
	//
	if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
		{
		//
		//printf("nand.c nand_read_extra_data() 1\n");
		//
		nand_reset_device();
		return K_ERROR;
		}
	//
	//printf("nand.c nand_read_extra_data() 2\n");
	//
	if(!(nand_media_sts & nand_media_sts_256_page))
		{
		for(i = 0; i < nand_redt_buffer_size; i++)
			{
			nand_redt_data[i] = nand_get_data();
			}
		if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
			{
			nand_reset_device();
			return K_ERROR;
			}
		}
	else
		{//there is some thing to do for 256bytes page
		;
		}
	//maybe it should read status
	//
	//printf("nand.c nand_read_extra_data() 5\n");
	//
	nand_set_cmd_ex(NFCMD_READSTS);
	while((nand_get_data()&(kbm_nand_status_ready))!=kbm_nand_status_ready);
	nand_set_standby();	
	return K_SUCCES;
}
unsigned int nand_read_sector(void)
{
	unsigned int i;
	unsigned int ecc_get;

	nand_set_cmd(NFCMD_READ0);
	nand_set_read_addr();
	//reset ECC here
	nand_reset_ecc();
	//
	if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
		{
		nand_reset_device();
		return K_ERROR;
		}
	if(!(nand_media_sts & nand_media_sts_256_page))
		{
		for(i = 0; i < 0x200; i++)
			{
			nand_page_buff[i] = nand_get_data();
			}
		//here do ecc check
		ecc_get =nand_get_ecc();
		//
		for(i = 0; i < nand_redt_buffer_size; i++)
			{
			nand_redt_data[i] = nand_get_data();
			}
		if(ECC_UNCORRECTABLE_ERROR == ecc_check_and_correct(nand_page_buff,(unsigned char*)(&ecc_get)))
			{
			//sensePtr = ;
			;//return K_ERROR;
			//printf("nand_read_sector error\nnand_addr_rd_phy_blk:%x\n",nand_addr_rd_phy_blk);
			}
		//
		if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
			{
			nand_reset_device();
			return K_ERROR;
			}
		}
	else
		{//there is some thing to do for 256bytes page
		;
		}
	//maybe it should read status
	nand_set_cmd_ex(NFCMD_READSTS);
	while((nand_get_data()&(kbm_nand_status_ready))!=kbm_nand_status_ready);
	//
	nand_set_standby();

	return K_SUCCES;
}
unsigned int nand_read_sector_test(unsigned int plba,unsigned int count)
{
	unsigned char t0,t1,t2; 
	if(count == 0)
		return K_ERROR;
	//
	t0 = nand_addr_zone;
	t1 = nand_addr_rd_phy_blk;
	t2 = nand_addr_page;
	//
	//
	while(count --)
		{
		nand_addr_zone = (plba/(1024*32));
		nand_addr_rd_phy_blk = ((plba%(1024*32))/32);
		nand_addr_page = plba%32;
		plba++;
		//printf("nand_addr_zone:              %x\n",nand_addr_zone);
		//printf("nand_addr_rd_phy_blk:     %x\n",nand_addr_rd_phy_blk);
		//printf("nand_addr_page:             %x\n",nand_addr_page);
		nand_read_sector();
		}
	//
	nand_addr_zone = t0;
	nand_addr_rd_phy_blk = t1;
	nand_addr_page =t2;
	//
	return K_SUCCES;
}

unsigned int nand_write_extra_data(void)
{
	unsigned int i;
	
	nand_set_cmd(NFCMD_READ_REDT);
	nand_set_cmd_ex(NFCMD_PROGRAM1);
	nand_set_write_addr();
	
	if(!(nand_media_sts & nand_media_sts_256_page))
		{
		for(i = 0; i < nand_redt_buffer_size; i++)
			{
			nand_set_data(nand_redt_data[i]);
			}
		nand_set_cmd_ex(NFCMD_PROGRAM2);
		if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_programming_timeout))
			{
			nand_reset_device();
			return K_ERROR;
			}
		}
	else
		{//there is some thing to do for 256bytes page
		;
		}
	nand_set_standby();
	return K_SUCCES;
}
unsigned int nand_write_extra_data_to_set_bad_block(void)
{
	unsigned int i;
	
	nand_set_cmd(NFCMD_READ_REDT);
	nand_set_cmd_ex(NFCMD_PROGRAM1);
	nand_set_write_addr();
	
	if(!(nand_media_sts & nand_media_sts_256_page))
		{
		for(i = 0; i < nand_redt_buffer_size; i++)
			{
			nand_set_data(0);
			}
		nand_set_cmd_ex(NFCMD_PROGRAM2);
		if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_programming_timeout))
			{
			nand_reset_device();
			return K_ERROR;
			}
		}
	else
		{//there is some thing to do for 256bytes page
		;
		}
	nand_set_standby();
	return K_SUCCES;
}
unsigned int nand_write_sector(void)
{
	unsigned int i;
	
	//--------------------------
	//writing protect detecting
	CheckWriteProtectInWriting();
	//
	nand_set_cmd(NFCMD_READ0);
	nand_set_cmd_ex(NFCMD_PROGRAM1);
	nand_set_write_addr();
	//reset ECC here
	nand_reset_ecc();
	//
	if(!(nand_media_sts & nand_media_sts_256_page))
		{
		for(i = 0; i < 0x200; i++)
			{
			nand_set_data(nand_page_buff[i]);
			}
		//set ECC here
		nand_get_hw_and_set_sw_ecc();
		//
		for(i = 0; i < nand_redt_buffer_size; i++)
			{
			nand_set_data(nand_redt_data[i]);
			}
		//
		CheckWriteProtectInWriting();
		//
		nand_set_cmd_ex(NFCMD_PROGRAM2);
		if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_programming_timeout))
			{
			nand_reset_device();
			return K_ERROR;
			}
		//
		//maybe it should read status
		nand_set_cmd_ex(NFCMD_READSTS);
		while(((i=nand_get_data())&(kbm_nand_status_ready))!=kbm_nand_status_ready)
			{
				//writing protect detecting
				CheckWriteProtectInWriting();
			}
		//
		}
	else
		{//there is some thing to do for 256bytes page
		;
		}
	nand_set_standby();
	if(i & kbm_nand_status_write_failed)
		{
		return K_ERROR;
		}
	return K_SUCCES;
}
unsigned int nand_write_sector_test(unsigned int plba,unsigned int count)
{
	unsigned char t0,t1,t2; 
	if(count == 0)
		return K_ERROR;
	//
	t0 = nand_addr_zone;
	t1 = nand_addr_wr_phy_blk;
	t2 = nand_addr_page;
	//
	//
	while(count --)
		{
		nand_addr_zone = (plba/(1024*32));
		nand_addr_wr_phy_blk = ((plba%(1024*32))/32);
		nand_addr_page = plba%32;
		plba++;
		nand_write_sector();
		}
	//
	nand_addr_zone = t0;
	nand_addr_wr_phy_blk = t1;
	nand_addr_page =t2;
	//
	return K_SUCCES;
}
unsigned int nand_check_data_status_valid(unsigned char page, unsigned char count)
{
	unsigned char page_save;
	unsigned int result;
	unsigned char data_status;

	page_save = nand_addr_page;
	nand_addr_page = page;
	result = K_SUCCES;

	nand_set_cmd(NFCMD_READ_REDT);
	nand_set_read_addr();

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
unsigned int nand_erase_block(void)
{
	unsigned int result = K_SUCCES;

	nand_set_cmd(NFCMD_ERASE1);
	nand_set_erase_addr();
	nand_set_cmd_ex(NFCMD_ERASE2);
	if(K_SUCCES!= (result = nand_wait_rdy_with_timeout(k_nand_busy_read_timeout)))
		{
		nand_reset_device();
		}
	//maybe it should read status
	nand_set_cmd_ex(NFCMD_READSTS);
	while((nand_get_data()&(0x40))!=0x40);
	//
	nand_set_standby();
	return result;
}
unsigned int nand_erase_block_test(unsigned int pb,unsigned int count)
{
	unsigned char t0,t1,t2; 
	if(count == 0)
		return K_ERROR;
	//
	t0 = nand_addr_zone;
	t1 = nand_addr_wr_phy_blk;
	t2 = nand_addr_page;
	//
	//
	while(count --)
		{
		nand_addr_zone = (pb/1024);
		nand_addr_wr_phy_blk = (pb%1024);
		nand_addr_page = 0;
		pb++;
		nand_erase_block();
		}
	//
	nand_addr_zone = t0;
	nand_addr_wr_phy_blk = t1;
	nand_addr_page =t2;
	//
	return K_SUCCES;;
}
//----------------------------------------------------------------------------
extern unsigned char nand_addr_segment ;
unsigned int nand_read_begin_xfer(unsigned int * udisk_lba_param)
{
	if(K_SUCCES != nand_map_lba2addr_rd(udisk_lba_param[index_udisk_lba_start]))
		{
			return K_ERROR;
		}		
		udisk_lba_param[index_udisk_lba_split_max_count] = nand_pages_per_block - nand_addr_page;
	
	// udisk_lba_param[index_udisk_lba_split_max_count]：指针所指block最大可容纳的数据量

	if(udisk_lba_param[index_udisk_lba_xfer_count] < udisk_lba_param[index_udisk_lba_split_max_count])
		{
		udisk_lba_param[index_udisk_lba_split_max_count] = udisk_lba_param[index_udisk_lba_xfer_count];
		//printf("lba_split_max_count=%d\n",udisk_lba_param[index_udisk_lba_split_max_count]);
		}
	return K_SUCCES;
		
    // udisk_lba_param[index_udisk_lba_xfer_count]:所需的空间 
}
unsigned int nand_read_end_xfer(unsigned int * udisk_lba_param)
{
	return K_SUCCES;
}
void usb_send_ep1_64_32_ex(unsigned char *addr);
unsigned int nand_read_begin_split(unsigned int * udisk_lba_param)
{
	unsigned char sector_count;
	unsigned char buff_ptr = 0;
	unsigned short i,j;
	unsigned char * buff;
	unsigned int new_buff512[512/4];
	unsigned int ecc_get;

	//printf("addr from s %x\n",(unsigned int)new_buff512);
	if(K_SUCCES != nand_map_lba2addr_rd(udisk_lba_param[index_udisk_lba_start]))
		{
			return K_ERROR;
		}
	if(K_SUCCES == nand_media_block_has_bad_data())
		{
		if(K_SUCCES != nand_check_data_status_valid(nand_addr_page,udisk_lba_param[index_udisk_lba_split_count]))
			{
			nand_reset_device();
			return K_ERROR;
			}		
		nand_reset_device();
		}
	//
	nand_set_cmd(NFCMD_READ0);
	nand_set_read_addr(); 
	if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
		{
		nand_reset_device();
		return K_ERROR;
		}
	//==================================================
	sector_count = udisk_lba_param[index_udisk_lba_split_count];
	//-----------------
	udisk_bulk_init_ex();
	start_des_data_init();
	//------------------
	while(sector_count)
		{
		//reset ECC here
		nand_reset_ecc();
		//do transfer data from nand to usb here
		//-------------------------------------------
		if( TRUE == udisk_rd_wr_self_enable )		
			{
			printf("udisk_rd_wr_self_enable\n");
			for(i = 0; i < 512; i++)
				{
				nand_page_buff[i] = nand_get_data();
				}
			for(i = 0; i < nand_redt_buffer_size; i++)
				{
				nand_redt_data[i] = nand_get_data();
				}
			}
		else
		//-------------------------------------------
			{
			//change buff
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
						i +=16;
						des_decrypt();
						udisk_usb_bulk_in_ex();
						}
					}
			//here do ecc check
			ecc_get =nand_get_ecc();
			//
			for(i = 0; i < nand_redt_buffer_size; i++)
				{
				nand_redt_data[i] = nand_get_data();
				}
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
unsigned int nand_read_end_split_first(unsigned int * udisk_lba_param)
{
	udisk_lba_param[index_udisk_lba_split_max_count] = nand_pages_per_block*nand_segments_per_page;
	return nand_read_end_split(udisk_lba_param);
}
unsigned int nand_read_end_split(unsigned int * udisk_lba_param)
{
	if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
		{
		nand_reset_device();
		return K_ERROR;
		}
	nand_set_standby();
	return K_SUCCES;
}
unsigned int nand_write_begin_xfer(unsigned int * udisk_lba_param)
{
	if(K_SUCCES != nand_map_write_begin(udisk_lba_param[index_udisk_lba_start]))
		{
			printf("nand_map_write_begin error\n");
			return K_ERROR;
		}
	nand_soft_reset();
	//
	udisk_lba_param[index_udisk_lba_split_max_count] = nand_pages_per_block - nand_addr_page;
	if(udisk_lba_param[index_udisk_lba_xfer_count] < udisk_lba_param[index_udisk_lba_split_max_count])
		{
		udisk_lba_param[index_udisk_lba_split_max_count] = udisk_lba_param[index_udisk_lba_xfer_count];
		}
	
	return K_SUCCES;
}
unsigned int nand_write_end_xfer(unsigned int * udisk_lba_param)
{
	if(nand_addr_page < ( nand_pages_per_block -1 ))
		{			
		nand_addr_page ++;
		if(K_SUCCES != nand_media_copy_block_tail())
			{
			return K_ERROR;
			}
		}
	return K_SUCCES;
}
void usb_receive_ep2_64_32_ex(char *buf);
unsigned int nand_write_begin_split(unsigned int * udisk_lba_param)
{
	unsigned char _nand_addr_zone;
	unsigned short i,j;
	unsigned char sector_count;
	unsigned char * buff;
	unsigned int new_buff512[512/4];
	unsigned char buff_ptr ;

	_nand_addr_zone = nand_addr_zone;
	if(K_SUCCES != nand_map_lba2addr_rd(udisk_lba_param[index_udisk_lba_start]))
		{
		return K_ERROR;
		}
	if(nand_addr_zone != _nand_addr_zone)
		{
		nand_soft_reset();
		}
	if(nand_addr_wr_phy_blk == nand_map_block_free)
		{
		if(K_SUCCES != nand_map_alloc_wr_blk())
			{
			return K_ERROR;
			}
		}
	sector_count = udisk_lba_param[index_udisk_lba_split_count];
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
		//printf("write NO.%x sector !!",sector_count);
		
		//printf("%x\n",sector_count);
		//wait usb receive 512 BYTEs end
		while(FALSE == udisk_usb_bulk_done_ex())
			{
			des_encrypt();
			udisk_usb_bulk_out_ex();
			des_encrypt();
			//printf("udisk_usb_bulk_done_ex error !!!!!");
			}
		while(FALSE == des_done())
			{
			des_encrypt();
			des_encrypt();
			}
		//--------------------------
		CheckWriteProtectInWriting();
		//
		nand_set_cmd(NFCMD_READ0);
		nand_set_cmd_ex(NFCMD_PROGRAM1);
		nand_set_write_addr();
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
					udisk_usb_bulk_out_ex();
					}
				}
			//set ECC here
			nand_get_hw_and_set_sw_ecc();
			udisk_usb_bulk_out_ex();
			//
			for(i = 0; i < nand_redt_buffer_size; i++)
				{
				nand_set_data(nand_redt_data[i]);
				}
			//----------------------------------------------
			//writing protect detecting
			CheckWriteProtectInWriting();
			//
			udisk_usb_bulk_out_ex();
			//
			nand_set_cmd_ex(NFCMD_PROGRAM2);
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
			nand_set_cmd_ex(NFCMD_READSTS);
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
			nand_addr_page ++;
			sector_count --;
			}
		}
	udisk_lba_param[index_udisk_lba_split_max_count] = nand_pages_per_block;
	return K_SUCCES;
}
unsigned int nand_write_end_split(unsigned int * udisk_lba_param)
{
	nand_set_standby();

	nand_addr_page --;
		
	if(nand_addr_page >=(nand_pages_per_block -1) )
		{
		if(K_SUCCES != nand_map_rebind_log2phy())
			{
			return K_ERROR;
			}
		}
	return K_SUCCES;
}


