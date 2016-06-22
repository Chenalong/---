

#define _NAND_MEDIA_C_
#include <instructions.h>
#include <bsp.h>
#include <types.h>
#include "nand_media.h"
#include "nand.h"
#include "ecc.h"
#include "udisk.h"

//-----------------------------------------------------------------
//
unsigned char * nand_redt_data;	//redundant data buffer
unsigned char * nand_page_buff;

unsigned char  nand_log2phy_map_buff[buff_nand_log2phy_map_size];
unsigned char  nand_assign_map_buff[nand_assign_map_size];
unsigned short nand_assign_start[2];
unsigned char nand_num_zones;
unsigned short nand_physical_blocks_per_zone;
unsigned short nand_logical_blocks_per_zone;
unsigned short nand_logical_blocks_per_boot_zone; 
unsigned char nand_pages_per_block;
unsigned char nand_media_sts;
//						
unsigned char nand_segments_per_page;
unsigned char nand_boot_block;  
unsigned char nand_boot_page;
// media address of current operation
unsigned char nand_addr_zone;       			// zone/segment component of address
unsigned char nand_assign_zone;
unsigned char nand_addr_page;     			// 0-based sector/page (512 bytes) within block
unsigned short nand_addr_rd_phy_blk; 		// 0-based physical block # within zone/segment
unsigned short nand_addr_wr_phy_blk;		 // 0-based physical block # within zone/segment
unsigned short nand_addr_log_blk;   			 // 0-based logical block # within zone/segment 
unsigned char nand_addr_segment ;      		// 0-based page that may contain 1 or more sectors (2048 byte page nand flash) 
                                 					//   or contained partial sector data (256 byte pages for sm)								 
//
unsigned char nand_media_device_attrible ;//= nand_media_device_attrible_default;
//
unsigned char indata;
unsigned char nand_write_state_2k;



unsigned short nand2k_copy_rd_blk;
unsigned short nand2k_copy_wr_blk;
unsigned short nand2k_copy_log_blk;
unsigned char nand2k_need_copy_tail;  // needed:0, else no need
unsigned char nand2k_copy_start_page;
unsigned char  nand2k_copy_start_segment;	
unsigned char nand2k_copy_addr_zone;


unsigned int nand_identify_media(void);
unsigned int nand_map_build_sector_map(void);
unsigned int nand_media_identify(void);
unsigned int nand_map_lba2addr_rd(unsigned int lba);
void nand_map_log2phy_set(unsigned char *pmap,unsigned short log_block,unsigned short phy_block);
unsigned short  nand_map_log2phy_get(unsigned char *pmap,unsigned short log_block);
unsigned int nand_write_log2phy_2k(unsigned int * udisk_lba_param);
unsigned int nand_read_log2phy_2k(unsigned int * udisk_lba_param);


//==============================================
void debug_set_write_data(void)
{
	memset(nand_page_buff,indata,512);
}
void debug_get_write_data(void)
{
	unsigned char bend,userin;
	
	printf("\nPlease Input Data for LB Write:\n");
	printf("Data from 0 to 255 and such as 0D,1D,2D.....\n");

	bend = 0;
	indata=0;
	while(1)
		{
	switch(userin = uart_receive_byte())
		{
		case '0':
			indata = 0 + (indata*10);
			break;
		case '1':
			indata = 1 + (indata*10);
			break;
		case '2':
			indata = 2 + (indata*10);
			break;
		case '3':
			indata = 3 + (indata*10);
			continue;
		case '4':
			indata = 4 + (indata*10);
			break;
		case '5':
			indata = 5 + (indata*10);
			break;
		case '6':
			indata = 6 + (indata*10);
			break;
		case '7':
			indata = 7 + (indata*10);
			break;
		case '8':
			indata = 8 + (indata*10);
			break;
		case '9':
			indata = 9 + (indata*10);
			break;
		case 'd':
		case 'D':
			printf("You Input Data is:  %d\n",indata);
			bend = 1;
			break;
		default:
			break;
		}
		if(bend)
			break;
		}
}	
void debug_get_lba(unsigned int * udisk_lba_param)
{
	unsigned int lba,count;
	unsigned char userin,bend;
	
	printf("\nPlease Input LBA Address:\n");
	printf("(LBA from 0 to %d " , (nand_num_zones * nand_logical_blocks_per_zone *nand_pages_per_block-1));
	printf("and Input Such as: 0D,1000D,1005D.......)\n");

	lba =0;
	bend = 0;
	while(1)
		{
	switch(userin = uart_receive_byte())
		{
		case '0':
			lba = 0 + (lba*10);
			break;
		case '1':
			lba = 1 + (lba*10);
			break;
		case '2':
			lba = 2 + (lba*10);
			break;
		case '3':
			lba = 3 + (lba*10);
			continue;
		case '4':
			lba = 4 + (lba*10);
			break;
		case '5':
			lba = 5 + (lba*10);
			break;
		case '6':
			lba = 6 + (lba*10);
			break;
		case '7':
			lba = 7 + (lba*10);
			break;
		case '8':
			lba = 8 + (lba*10);
			break;
		case '9':
			lba = 9 + (lba*10);
			break;
		case 'D':
		case 'd':
			printf("You Input Addr is:  %d\n",lba);
			if(lba >(nand_num_zones * nand_logical_blocks_per_zone*nand_pages_per_block-1))
				{
				printf("Too Large LBA.Please input again.\n");
				printf("\nPlease Input LBA Address:\n");
				printf("(LBA from 0 to %d " , (nand_num_zones * nand_logical_blocks_per_zone *nand_pages_per_block-1));
				printf("and Input Such as: 0D,1000D,1005D.......)\n");
				lba = 0;
				break;
				}
			//
			bend = 1;
			break;
		default:
			break;
		}
		if(bend)
			break;
		}
	
	printf("\nPlease Input LB Count:\n");
	printf("(Count from 1 to %d " , (nand_num_zones * nand_logical_blocks_per_zone *nand_pages_per_block-lba));
	printf("and Input Such as: 0D,1000D,1005D.......)\n");
	
	count = 0;
	bend = 0;
	while(1)
		{
	switch(userin = uart_receive_byte())
		{
		case '0':
			count = 0 + (count*10);
			break;
		case '1':
			count = 1 + (count*10);
			break;
		case '2':
			count = 2 + (count*10);
			break;
		case '3':
			count = 3 + (count*10);
			break;
		case '4':
			count = 4 + (count*10);
			break;
		case '5':
			count = 5 + (count*10);
			break;
		case '6':
			count = 6 + (count*10);
			break;
		case '7':
			count = 7 + (count*10);
			break;
		case '8':
			count = 8 + (count*10);
			break;
		case '9':
			count = 9 + (count*10);
			break;
		case 'D':
		case 'd':
			printf("You Input Count is:  %d\n",count);
			if((count >(nand_num_zones * nand_logical_blocks_per_zone*nand_pages_per_block-lba))||(count == 0))
				{
				printf("Incorrect count.Please input again.\n");
				printf("\nPlease Input LB Count:\n");
				printf("(Count from 1 to %d " , (nand_num_zones * nand_logical_blocks_per_zone *nand_pages_per_block-lba));
				printf("and Input Such as: 0D,1000D,1005D.......)\n");
				count = 0;
				break;
				}
			//
			bend = 1;
			break;
		default:
			break;
		}
		if(bend)
			break;
		}
	udisk_lba_param[index_udisk_lba_xfer_count]  = count;
	udisk_lba_param[index_udisk_lba_start] = lba;
}
void debug_printf_nand_info(void)
{
	printf("nand_num_zones:                             %d\n",nand_num_zones);                         
	printf("nand_physical_blocks_per_zone:        %d\n",nand_physical_blocks_per_zone);                     
	printf("nand_logical_blocks_per_zone:           %d\n",nand_logical_blocks_per_zone);                         
	printf("nand_logical_blocks_per_boot_zone:   %d\n",nand_logical_blocks_per_boot_zone);                     
	printf("nand_pages_per_block:                     %d\n",nand_pages_per_block);
}
void debug_printf_l2p_map(void)
{
	unsigned int i;
	unsigned char *pbuff;
	
	printf("l2b mapping table:\n");
	
	printf(" table 0\n---------------------------------------------");
	pbuff = nand_log2phy_map_buff;
	for(i = 0; i< (nand_max_lb_per_zone);i++)
		{
		if(!(i%15))
			printf("\n");
		printf("L%x-P%x ",i,nand_map_log2phy_get(pbuff,i));
		}
	printf("\n table 1\n---------------------------------------------");
	pbuff = nand_log2phy_map_buff +( buff_nand_log2phy_map_one_zone_size);
	for(i = 0; i< (nand_max_lb_per_zone);i++)
		{
		if(!(i%15))
			printf("\n");
		printf("L%x-P%x ",i,nand_map_log2phy_get(pbuff,i));
		}
	printf("\n");
}
void debug_printf_assign_map(void)
{
	unsigned int i;
	unsigned char *pbuff;

	
	printf("assign mapping table:");
	printf(" table 0\n---------------------------------------------");
	pbuff = nand_assign_map_buff;
	for(i = 0; i< (nand_assign_map_one_zone_size);i++)
		{
		if(!(i%16))
			printf("\n");
		printf("%x ",pbuff[i]);
		}
	printf("\n table 1\n---------------------------------------------");
	pbuff = nand_assign_map_buff + (nand_assign_map_one_zone_size);
	for(i = 0; i< (nand_max_pb_per_zone/8);i++)
		{
		if(!(i%16))
			printf("\n");
		printf("%x ",pbuff[i]);
		}
	printf("\n");
}
void debug_page_buff(unsigned char *buff,unsigned int count)
{
	unsigned int i;
	
	for(i = 0; i < count ;i ++)
		{
		if((i%0x10) == 0)
			printf("\n");
		printf("%x ",buff[i] );
		}
	printf("\n");
}
//----------------------------------------------------------------------------------
unsigned int bit_count8(unsigned char val)
{
	unsigned int count=0;
	unsigned int i = 8;

	while(i--)
		{
			if(val & 0x01)
				count ++;
			val >>= 1;
		}
	return count;
}
unsigned int bit_count16(unsigned short val)
{
	unsigned int count=0;
	unsigned int i = 16;

	while(i--)
		{
		if(val & 0x01)
			count ++;
		val >>= 1;
		}
	return count;
}

//------------------------------------------------------------------------------------
void nand_map_log2phy_set(unsigned char *pmap,unsigned short log_block,unsigned short phy_block)
{
	unsigned char * _pmap = (pmap + ((log_block>>1)*3));
	//printf("%x\n",log_block);
	if(log_block & 0x01)
		{
		_pmap[1] = (unsigned char )(phy_block & 0xFF);
		_pmap[2] = (_pmap[2]& 0x0F) + ( (unsigned char )((phy_block>>4) & 0xF0));
		}
	else
		{
		_pmap[0] = (unsigned char )(phy_block & 0xFF);
		_pmap[2] = (_pmap[2]& 0xF0) + ( (unsigned char )((phy_block>>8) & 0x0F));
		}
}
unsigned short  nand_map_log2phy_get(unsigned char *pmap,unsigned short log_block)
//传入log2phy表地址和逻辑地址，返回物理地址

{
	unsigned char * _pmap = (pmap + ((log_block>>1)*3));//3//3个字节对应两个BLOCK
	unsigned short phy_block = _pmap[2];
	if(log_block & 0x01)
		{
		phy_block = ((phy_block & 0xF0)<<4) + _pmap[1] ;
		}
	else
		{
		phy_block = ((phy_block & 0x0F)<<8) + _pmap[0] ;
		}

	                    //3个字节存两个相连的BLOCK地址，格式为
	                    //_pmap[0]:block0[0...7];_pmap[1]:block1[0....7];_pmap[2]:block0[8..11]+(block[8..11]<<4)
	return phy_block;
}
unsigned int nand_init_media(void)
{
	//printf("nand_init_media\n");
	REG32(0x9803F50C) = 0x5555555;
	nand_enable_controller();
	nand_enable_ecc() ;
	nand_soft_reset();
	//nand_media_identify();
	nand_media_check_format();
	return K_SUCCES;
}
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
unsigned int nand_media_erase_all(void)
{
	//printf("nand_media_erase_all():\n");
	nand_erase_block_test(0, (nand_num_zones * nand_physical_blocks_per_zone));
	return K_SUCCES;
}
unsigned int nand_media_phy2log(void)
{
	unsigned short addr1;
	unsigned short addr2;

	addr1 = nand_redt_data[redt_lba1_hi];
	addr1 = (addr1 << 8) + nand_redt_data[redt_lba1_lo];
	addr2 = nand_redt_data[redt_lba2_hi];
	addr2 = (addr2 << 8) + nand_redt_data[redt_lba2_lo];
	if (addr1==addr2)
		{
		if ((addr1 &0xF000)==0x1000)
			{
			if(0==(bit_count16(addr1)&0x01))
				{
				nand_addr_log_blk= ((addr1>>1) &0x03FF);
				return K_SUCCES;
				}
			}
		}

	if (bit_count16(addr1^addr2)!=0x01)
		{
		// 0, or 2+ bit difference. 
		trace1("nand_media_phy2log error 0, \n");
		trace3("addr1: %x  addr2: %x\n",addr1,addr2);
		return K_ERROR;
		}

	// 1 bit error
	if ((addr1 &0xF000)==0x1000)
		{
		if (0 == (bit_count16(addr1) &0x01))
			{
			nand_addr_log_blk=((addr1>>1) &0x03FF);
			return K_SUCCES;
			}
		}
	if ((addr2 &0xF000)==0x1000)
		{
		if (0 == (bit_count16(addr2) &0x01))
			{
			nand_addr_log_blk=((addr1>>1) &0x03FF);
			return K_SUCCES;
			}
		}
	trace1("nand_media_phy2log error 1");
	return K_ERROR;
}
unsigned int nand_media_read_sector(void)
{
	if(nand_media_sts&0x40)
		{
		//printf("nand_read_segment_2k\n");
		return (nand_read_segment_2k());
		}
	else
		{
		return (nand_read_sector());
		}	
}
unsigned int nand_media_validate_sector(unsigned char *buff)
{
/*	ecc_sw_calculate(buff);
	
	if(K_SUCCES != ecc_check_rd_error())
		{
		if(K_SUCCES != ecc_check_and_correct(buff))
			return K_ERROR;
		}*/
	return K_SUCCES;
}
unsigned int nand_media_write_sector(void)
{
	if (nand_media_sts&0x40)
		{
		return(nand_write_segment_2k());
		}
	else
		{
		return (nand_write_sector());
		}
}
unsigned int nand_media_erase_block(void)
{
	unsigned char save_page ;

	if( (nand_addr_wr_phy_blk == nand_map_block_free) )
		return K_ERROR;
/*#ifndef nand_repair
	if( (nand_addr_zone==0) && (nand_addr_wr_phy_blk<=nand_boot_block))
		return K_ERROR;
#endif*/
	if (nand_media_sts&0x40)
		{
		return(nand_erase_block_2k());
		}
	else
		{
		return( nand_erase_block());
		}
}
unsigned int nand_media_erase_rd_block(void)
{
	unsigned int _nand_addr_wr_phy_blk;
	unsigned char * nand_log2phy_map;
	unsigned char  * nand_assign_map;

	//
	//printf("nand_media_erase_rd_block() -- 0\n");
	//
	nand_log2phy_map = (nand_addr_zone ? (nand_log2phy_map_buff + buff_nand_log2phy_map_one_zone_size) : nand_log2phy_map_buff);
	if(nand_addr_rd_phy_blk == nand_map_block_free)
		{
		nand_map_log2phy_set(nand_log2phy_map,nand_addr_log_blk, nand_map_block_free);
		}
	else
		{
		nand_assign_map = (nand_addr_zone ? (nand_assign_map_buff + nand_assign_map_one_zone_size) : nand_assign_map_buff);
		_nand_addr_wr_phy_blk = nand_addr_wr_phy_blk;
		nand_addr_wr_phy_blk = (nand_addr_rd_phy_blk &nand_map_l2p_addr);

		if( nand_addr_rd_phy_blk & nand_map_soft_l2p_binding)
			{
			b_map_phy_blk_free(nand_assign_map,nand_addr_rd_phy_blk);
			}
		else
			{
			if(K_SUCCES != nand_media_erase_block())
				{
				if(K_SUCCES != nand_media_erase_block())
					{
					//erase error,mask it bad block;
					}
				else
					{
					b_map_phy_blk_free(nand_assign_map,nand_addr_rd_phy_blk);
					}
				}
			else
				{
				b_map_phy_blk_free(nand_assign_map,nand_addr_rd_phy_blk);
				}
			}
		nand_map_log2phy_set(nand_log2phy_map,nand_addr_log_blk, nand_map_block_free);
		nand_addr_rd_phy_blk = nand_map_block_free;
		nand_addr_wr_phy_blk = _nand_addr_wr_phy_blk;
		}
	return K_SUCCES;
}
unsigned int nand_media_erease_all_valid_block(void)
{
	;
	return K_SUCCES;
}
unsigned int nand_media_seek_cis(void)
{
	nand_addr_zone = 0;
	nand_addr_page = 0;

	for(nand_addr_rd_phy_blk =0;nand_addr_rd_phy_blk < ( nand_physical_blocks_per_zone - 
		nand_logical_blocks_per_zone -1);nand_addr_rd_phy_blk++)
		{
			if(K_SUCCES !=nand_media_read_extra_data())
				{
				nand_soft_reset();
				return K_ERROR;
				}
			if(K_SUCCES == nand_media_is_phyblock_valid() )
				break;
		}
	
	printf("first valid block:%d\n",nand_addr_rd_phy_blk);
	if (nand_addr_rd_phy_blk==(nand_physical_blocks_per_zone-nand_logical_blocks_per_zone-1))
		{
		//error - valid cis block not found
		//now for many cheap flash,it maybe often act,maybe other method is needed.
		nand_soft_reset();
		//printf("error\n");
		return K_ERROR;
		}
	while(nand_addr_page < nand_max_cis_sect)
		{
		if(nand_addr_page)
			{
			if(K_SUCCES != nand_media_read_extra_data() )
				{
				nand_soft_reset();
				return K_ERROR;
				}
			}
		if(K_SUCCES == nand_media_is_page_data_valid() )
			{
			if(K_SUCCES != nand_media_read_sector())
				{
				nand_soft_reset();
				return K_ERROR;
				}
			//
			//debug_page_buff(nand_page_buff , 512);		//printf
			//can verify CIS or not
			//if(!(nand_media_device_attrible & nand_media_device_attrible_ignore_bad_cis))
			if(0)
				{
				if(K_SUCCES != nand_media_validate_sector(nand_page_buff))
					{
					nand_soft_reset();
					return  K_ERROR;
					}
				}
			//
			nand_boot_block = nand_addr_rd_phy_blk;
			nand_boot_page = nand_addr_page;
			return K_SUCCES;
			}
		nand_addr_page ++;
		}
	nand_soft_reset();
	return K_ERROR;
}
unsigned int nand_media_read_extra_data(void)
{
	if (nand_media_sts&0x40)
		return (nand_read_extra_data_2k());
	else 
		return (nand_read_extra_data());
}
unsigned int nand_media_write_extra_data(void)
{
	return (nand_write_extra_data());
}
unsigned int nand_media_clear_extra_data(void) 
{
	memset(nand_redt_data, 0xff, nand_redt_buffer_size) ;
	return K_SUCCES;
}
unsigned int nand_media_resolve_conflict(void)
{
	unsigned short temp ;
	if (K_SUCCES != nand_map_resolve_conflict())
		{
		temp = ((nand_addr_rd_phy_blk > nand_addr_wr_phy_blk) ? nand_addr_wr_phy_blk : nand_addr_rd_phy_blk);
		nand_addr_wr_phy_blk =  ((nand_addr_rd_phy_blk > nand_addr_wr_phy_blk) ? nand_addr_rd_phy_blk : nand_addr_wr_phy_blk);
		nand_addr_rd_phy_blk = temp ;
		}
	return K_SUCCES;
}
unsigned int nand_media_is_phyblock_blank(void)
{
	unsigned int i;
	
	 for (i=0; i<nand_redt_buffer_size; i++)
	 	{
	 		if(nand_redt_data[i] != 0xFF)
				return K_ERROR;
	 	}
	 return K_SUCCES ;
}
unsigned int nand_media_is_phyblock_reserved(void) 
{
	if ((!nand_addr_zone) && (nand_addr_rd_phy_blk <= nand_boot_block))
		return K_SUCCES;
	return K_ERROR;
}
unsigned int nand_media_is_phyblock_valid(void)
{
	unsigned char blk_status = nand_redt_data[redt_block_status];

	if (blk_status==0xFF)
		return K_SUCCES ;
	if (blk_status==0x00)
		return K_ERROR;
	if (bit_count8(blk_status)<7)	
		return K_ERROR;
	return K_SUCCES ;
}
unsigned int nand_media_is_page_data_valid(void)
{
	unsigned char data_status = nand_redt_data[redt_data_status] ;

	if (data_status==0xFF)
		return K_SUCCES;
	if (data_status==0x00)
		return K_ERROR;
	if (bit_count8(data_status)<5)
		return K_ERROR;
	return K_SUCCES;
}

unsigned int nand_media_block_has_bad_data(void) 
{
	if (nand_media_sts&0x40)
		{
		if( K_SUCCES != nand_check_data_status_valid_2k(0, nand_pages_per_block))
		return K_SUCCES;//has bad data
		}
	else
		{
		if( K_SUCCES != nand_check_data_status_valid(0, nand_pages_per_block))
		return K_SUCCES;//has bad data
		}
	return K_ERROR;
}
//--------------------------------------------------------------
//by reading maker ID and device ID ,setting parameter for nand flash 
//--------------------------------------------------------------
void nand_media_init_nand_parameter(void)
{
	nand_num_zones = 0;
	nand_physical_blocks_per_zone    		=0; 
	nand_logical_blocks_per_zone = 0;                                                              
	nand_logical_blocks_per_boot_zone 		=0;                         
	nand_pages_per_block              			=0; 
	nand_segments_per_page = 1;
	nand_media_sts =0;
}
unsigned int nand_identify_media_format(void)
{
	unsigned char id_list[nand_id_max];
	
	nand_read_id(id_list);

	switch(id_list[nand_id_maker])
		{
		case MAKER_SAMSUNG:
				nand_physical_blocks_per_zone    	=1024;                          
				nand_logical_blocks_per_zone      		=1000;                          
				nand_logical_blocks_per_boot_zone 	=1000;                         
							
				switch(id_list[nand_id_device])
					{
					//---------------------------------------
					//large page
					//16 GBit
					case 0xD5:
							{
							// 2GB nand flash,2k page size
							nand_num_zones                   			=16;                            
							nand_pages_per_block              		=64;               
							nand_segments_per_page 			= 4;
							nand_media_sts |= nand_media_addr_4cyc ;  /* 4 cycle addressing */  
							nand_media_sts &= ~nand_media_sts_256_page ;	
							nand_media_sts|=nand_media_sts_2k_page;
					    		}
							return K_SUCCES;
							break;
					//8 GBit
					case 0xD3:
							{
							// 1GB nand flash,2k page size
							nand_num_zones                   			=8;                            
							nand_pages_per_block              		=64;               
							nand_segments_per_page 			= 4;
							nand_media_sts |= nand_media_addr_4cyc ;  /* 4 cycle addressing */  
							nand_media_sts &= ~nand_media_sts_256_page ;	
							nand_media_sts|=nand_media_sts_2k_page;
					    		}
							return K_SUCCES;
							break;
					// 4 GBit
					case 0xDC:
							{
							//512 MB nand flash,2k page size
							nand_num_zones                   			=4;                            
							nand_pages_per_block              		=64;               
							nand_segments_per_page 			= 4;
							nand_media_sts |= nand_media_addr_4cyc ;  /* 4 cycle addressing */  
							nand_media_sts &= ~nand_media_sts_256_page ;	
							nand_media_sts|=nand_media_sts_2k_page;
					    		}
							return K_SUCCES;
							break;
					// 2Gbit
					case 0xDA:
							{
							//256 MB nand flash,2k page size
							nand_num_zones                   			=2;                            
							nand_pages_per_block              		=64;               
							nand_segments_per_page 			= 4;
							nand_media_sts |= nand_media_addr_4cyc ;  /* 4 cycle addressing */  
							nand_media_sts &= ~nand_media_sts_256_page ;	
							nand_media_sts|=nand_media_sts_2k_page;
					    		}
							return K_SUCCES;
							break;
							break;
					// 1 Gbit
					case 0xF1:
							{
							//128 MB nand flash,2k page size
							nand_num_zones                   			=1;                            
							nand_pages_per_block              		=64;               
							nand_segments_per_page 			= 4;
							nand_media_sts &= ~nand_media_addr_4cyc ;  // 3 cycle addressing 
							nand_media_sts &= ~nand_media_sts_256_page ;	
							nand_media_sts|=nand_media_sts_2k_page;
					    		}
							return K_SUCCES;
							break;
					//--------------------------------------
					//small page					             
					//128Mbit (16M)NAND   
				       case 0x73:    
						{
						// 16 MB nand flash. num_zones:1,            
						// phy_blks_per_zone:1024, log_blks_per_zone:1000,        
						// pages_per_blk:32 sectors_per_page:1                
						nand_num_zones                   			=1;                            						                         
						nand_pages_per_block              			=32;          
						nand_segments_per_page = 1;
						nand_media_sts &= ~nand_media_addr_4cyc ;  // 3 cycle addressing   
						nand_media_sts &= ~nand_media_sts_256_page ;	  
				    		}
				      		break;                                                                   
				//256Mbit (32M)NAND                                                          
	    			case 0x75:       
					{
					// "32 MB nand flash. num_zones:2,            
					// phy_blks_per_zone:1024, log_blks_per_zone:1000,        
					// pages_per_blk:32 sectors_per_page:1                 
					nand_num_zones                   			=2;                            				                         
					nand_pages_per_block              			=32;   
					nand_segments_per_page = 1;
					nand_media_sts &= ~nand_media_addr_4cyc ;  // 3 cycle addressing   
					nand_media_sts &= ~nand_media_sts_256_page ;	  
				    	}
				      	break;                                                                   
				//512Mbit (64M)NAND                                                          
	    			case 0x76:        
					{
					// 64 MB nand flash. num_zones:4,            
					// phy_blks_per_zone:1024, log_blks_per_zone:1000,        
					// pages_per_blk:32 sectors_per_page:1 
					nand_num_zones                   			=4;                            				                         
					nand_pages_per_block              			=32;           
					nand_segments_per_page = 1;
					nand_media_sts |= nand_media_addr_4cyc ;  /* 4 cycle addressing */  
					nand_media_sts &= ~nand_media_sts_256_page ;	  
				    	}
				      	break;                                                                   
				/* 1Gbit(128M)NAND */                                                          
	    			case 0x79:                  
					{
					// 128 MB nand flash. num_zones:8,           
					// phy_blks_per_zone:1024, log_blks_per_zone:1000,        
					// pages_per_blk:32 sectors_per_page:1
					nand_num_zones                   			=8;                            					                         
					nand_pages_per_block              			=32;                 
					nand_segments_per_page = 1;
					nand_media_sts |= nand_media_addr_4cyc ;  /* 4 cycle addressing */  
					nand_media_sts &= ~nand_media_sts_256_page ;	
					nand_media_sts&=~nand_media_sts_2k_page;
				    	}
				      	break; 
					default:
						return K_ERROR;
						break;
					
					}
				break;
		case MAKER_TOSHIBA:
				{;}
				break;
		case MAKER_HYNIX:
				{;}
				break;
		case MAKER_MICRON:
				{;}
				break;
		case MAKER_RENESARS:
				{;}
				break;
		case MAKER_ST:
					{;}
				break;
		default:
			return K_ERROR;
			
		}
	return K_SUCCES;
}
#if 0
unsigned int  nand_media_set_size(unsigned char id_device)
{
  	switch (id_device)
	  	{
		/* 8Mbit (1M) NAND */ /*
	    	case 0x6E:
	    	case 0xE8:
	    	case 0xEC:
					{
					// 1 MB nand flash num_zones:1, 
					// phy_blks_per_zone:256, log_blks_per_zone:250,
				       //  pages_per_blk:16 sectors_per_page:0.5              
					nand_num_zones                   			=1;                            
					nand_physical_blocks_per_zone    		=256;                          
					nand_logical_blocks_per_zone      		=250;                          
					nand_logical_blocks_per_boot_zone 		=1000;                         
					nand_pages_per_block              			=8;          
					nand_segments_per_page = 1;
					nand_media_sts &= ~nand_media_addr_4cyc ;  //3 cycle addressing   
					nand_media_sts |= nand_media_sts_256_page ;
				    	}
				      break;                                                                   
		// 16Mbit (2M) NAND                                                          
	    	case 0x64:                                                                 
	    	case 0xEA:                                                                 
	    	case 0xF1:         
					{
					// 2 MB nand flash. num_zones:1,             
				       // phy_blks_per_zone:512, log_blks_per_zone:500,          
				       // pages_per_blk:16 sectors_per_page:0.5
					nand_num_zones                   			=1;                            
					nand_physical_blocks_per_zone    		=512;                          
					nand_logical_blocks_per_zone      		=500;                          
					nand_logical_blocks_per_boot_zone 		=1000;                         
					nand_pages_per_block              			=8;        
					nand_segments_per_page = 1;
					nand_media_sts &= ~nand_media_addr_4cyc ;  // 3 cycle addressing   
					nand_media_sts |= nand_media_sts_256_page ;
				    	}
				      break;                                                                   
		//32Mbit (4M) NAND                                                          
	    	case 0x6B:                                                                 
	    	case 0xE3:                                                                 
	    	case 0xE5: 
					{
					// 4 MB nand flash. num_zones:1,             
					// phy_blks_per_zone:512, log_blks_per_zone:500,          
					// pages_per_blk:16 sectors_per_page:1 
					nand_num_zones                   			=1;                            
					nand_physical_blocks_per_zone    		=512;                          
					nand_logical_blocks_per_zone      		=500;                          
					nand_logical_blocks_per_boot_zone 		=500;                         
					nand_pages_per_block              			=16;         
					nand_segments_per_page = 1;
					nand_media_sts &= ~nand_media_sts_256_page ;                              
					nand_media_sts &= ~nand_media_addr_4cyc ;  // 3 cycle addressing   
	    				}
				      	break;                                                                   
		// 64Mbit (8M) NAND                                                         
	    	case 0xE6:                       
					{
					// 8 MB nand flash. num_zones:1,             
					// phy_blks_per_zone:1024, log_blks_per_zone:1000,        
					// pages_per_blk:16 sectors_per_page:1                  
					nand_num_zones                   			=1;                            
					nand_physical_blocks_per_zone    		=1024;                          
					nand_logical_blocks_per_zone      		=1000;                          
					nand_logical_blocks_per_boot_zone 		=1000;                         
					nand_pages_per_block              			=16;      
					nand_segments_per_page = 1;
					nand_media_sts &= ~nand_media_addr_4cyc ; // 3 cycle addressing   
					nand_media_sts &= ~nand_media_sts_256_page ;	  
				    	}
				      	break;       */                                                            
		                                                       
                                                                  
	      	default:                                                                 
		//Illegal Device ID Code           
	      	return K_ERROR;
  }                                                                            
  return K_SUCCES;
}
#endif
//------------------------------------------------
unsigned int nand_media_check_format(void)
{
	// check nand media size
	if(K_SUCCES != nand_identify_media_format())//检测flash格式
		{		
		printf("read identify error!!");
		return K_ERROR;
		}
	//
	//printf("nand_media_check_format->nand_identify_media_format ok\n");
	//
	// seek the first used sector	
	/*if(K_SUCCES != nand_media_seek_cis())
		{
		//maybe can repair nand flash here
		//
		printf("error\n");
		return K_ERROR;
		}*/
	//
	//printf("nand_media_check_format->nand_media_seek_cis ok\n");
	//
	
	nand_addr_zone = 0;
	nand_assign_zone = 0;
	// set sector map
	//printf("ok!!!\n");
	if(K_SUCCES != nand_map_build_sector_map())
		{
#ifdef DISK_TEST_MODE_ENABLE		
		printf("build sector map error!!!");
#endif
		return K_ERROR;		
		}
	return K_SUCCES;
}
#if 0
unsigned int nand_media_identify(void)
{
	//printf("identify nand media\n");
	if(K_SUCCES != nand_media_check_format())
		//检测flash媒体
		{
		//printf("%d",nand_num_zones);
		//printf("%d",nand_physical_blocks_per_zone);
		//printf("%d",nand_logical_blocks_per_zone);
		//printf("%d",nand_logical_blocks_per_boot_zone);
		//printf("can not identify nand media\n");
		return K_ERROR;
		}
	//printf("identify nand media end\n");
	return K_SUCCES;
}
#endif
unsigned int nand_media_bind_log2phy(void)
{
	unsigned short _lba;
	
	nand_redt_data[redt_reserved0] = 
	nand_redt_data[redt_reserved1] =
	nand_redt_data[redt_reserved2] =
	nand_redt_data[redt_reserved3] =0xFF;
	//
	nand_redt_data[redt_data_status] = 0xFF;
	nand_redt_data[redt_block_status] = 0xFF;
	//
	_lba = ((nand_addr_log_blk <<1)+0x1000);
	if((bit_count16(_lba )&0x01))
		_lba++;
	nand_redt_data[redt_lba1_lo] = nand_redt_data[redt_lba2_lo] =(_lba&0xFF);
	nand_redt_data[redt_lba1_hi] = nand_redt_data[redt_lba2_hi] =((_lba>>8)&0xFF);
	
	return K_SUCCES;
}
unsigned int nand_media_copy_sector(void)
{
	if(nand_addr_wr_phy_blk == nand_map_block_free )
		{
		return K_ERROR;
		}
	
	if(nand_addr_rd_phy_blk != nand_map_block_free)
		{
		if(K_SUCCES != nand_media_read_sector())
			return K_ERROR;
		}
	else
		{
		memset(nand_page_buff,0xFF,512);
		nand_media_clear_extra_data();
		}
	//
	nand_media_bind_log2phy();

	if(K_SUCCES != nand_media_write_sector())
		{
		printf("nand_media_write_sector error\n");
		return K_ERROR;
		}
	return K_SUCCES;
}
unsigned int nand_media_copy_sector_2k(void)
{
	if(nand_addr_wr_phy_blk == nand_map_block_free )
		{
		return K_ERROR;
		}
	
	if(nand_addr_rd_phy_blk != nand_map_block_free)
		{
		//printf("nand_addr_rd_phy_blk != nand_map_block_free\n");
		if(K_SUCCES != nand_media_read_sector())
			return K_ERROR;
		}
	else
		{
		memset(nand_page_buff,0xFF,512);
		nand_media_clear_extra_data();
		}
	//
	nand_media_bind_log2phy();
/*
	unsigned i;
	for(i=0;i<16;i++)
		printf("%x = %x  ",i,nand_redt_data[i]);
	printf("\n");
*/
	if(K_SUCCES != nand_media_write_sector())
		{
		printf("nand_media_write_sector error\n");
		return K_ERROR;
		}
	return K_SUCCES;
}



unsigned int nand_media_copy_block(unsigned int page_num)
{
	//printf("nand_media_copy_block()\n");
	//printf("nand_addr_rd_phy_blk : %x\nnand_addr_wr_phy_blk: %x\n",nand_addr_rd_phy_blk,nand_addr_wr_phy_blk);

	while(page_num--)
		{
		if(K_SUCCES != nand_media_copy_sector())
			{
			return K_ERROR;
			}
		nand_addr_page++;
		}
	return K_SUCCES;
}
unsigned int nand_media_copy_block_2k(unsigned int segment_num)
{
	//printf("nand_media_copy_block()\n");
	//printf("nand_addr_rd_phy_blk : %x\nnand_addr_wr_phy_blk: %x\n",nand_addr_rd_phy_blk,nand_addr_wr_phy_blk);
#if 0
	while(segment_num--)
		{
		if(K_SUCCES != nand_media_copy_sector_2k())
			{
			return K_ERROR;
			}
		nand_addr_segment++;
		if(nand_addr_segment==nand_segments_per_page)
			{
			nand_addr_segment=0;
			nand_addr_page++;
			}
		}
#else
      unsigned long temp;
      unsigned char end_page,end_segment,first_count,word_count,last_count,i;
      first_count = word_count = last_count = 0;
      	  
      temp = (nand_addr_page<<2)+nand_addr_segment;
      temp	+= segment_num;
      end_page = temp>>2;
      end_segment = temp&3;
      if(end_page==nand_addr_page)     		
      		first_count = segment_num;
      else
      	{
		first_count = 4-nand_addr_segment;
       	first_count &= 3;
		word_count = end_page - nand_addr_page - 1;
		if(0==nand_addr_segment)
			word_count++;
		last_count = end_segment;
      	}
#ifdef DISK_TEST_MODE_ENABLE
printf("*** nand_addr_page = 0x%02x, nand_addr_segment = 0x%02x, \n",nand_addr_page,nand_addr_segment);
printf("total = 0x%02x, first = 0x%02x, word = 0x%02x, last = 0x%02x, \n",segment_num,first_count,word_count,last_count);
#endif		  
       // step 1, head no page alliy
      	while(first_count--)
		{
		if(K_SUCCES != nand_media_copy_sector_2k())			
			return K_ERROR;			
		nand_addr_segment++;
		nand_addr_page +=(nand_addr_segment>>2);
		nand_addr_segment &= 3;		
		}
		
	// step 2, alliy, page copy back
      	while(word_count--)
		{
		if(K_SUCCES != nand2k_media_page_copy())			
			return K_ERROR;					
		nand_addr_page++;			
		}
		
       // step 3, tail no page alliy
      	while(last_count--)
		{
		if(K_SUCCES != nand_media_copy_sector_2k())			
			return K_ERROR;			
		nand_addr_segment++;
		nand_addr_page +=(nand_addr_segment>>2);
		nand_addr_segment &= 3;
		}      
      	
#endif
	return K_SUCCES;
}
unsigned int nand_media_copy_block_head(void)
{
	unsigned char nand_addr_page_save;

	//
	//printf("nand_media_copy_block_head()\n");
	//
	nand_addr_page_save = nand_addr_page;
	nand_addr_page = 0;
	return (nand_media_copy_block(nand_addr_page_save));
}
unsigned int nand_media_copy_block_head_2k(void)
{
	unsigned int segment_num;

	//
	//printf("nand_media_copy_block_head()\n");
	//
	segment_num =nand_addr_page*nand_segments_per_page+nand_addr_segment;
	nand_addr_page = 0;
	nand_addr_segment=0;
	if(K_SUCCES != nand_media_copy_block_2k(segment_num))
			{
			printf("nand_media_copy_block_2k error!!!\n");
			return K_ERROR;
			}
	if ((nand_write_state_2k&1)==0)
		{
		unsigned wr_addr_save,rd_addr_save;
		unsigned char *nand_assign_map;
		nand_assign_map = (nand_addr_zone ? (nand_assign_map_buff + nand_assign_map_one_zone_size) : nand_assign_map_buff);
		wr_addr_save = nand_addr_wr_phy_blk;
		rd_addr_save = nand_addr_rd_phy_blk;
		nand_addr_wr_phy_blk = nand_addr_rd_phy_blk;
		b_map_phy_blk_free(nand_assign_map, nand_addr_wr_phy_blk);
		if(K_SUCCES != nand_erase_block_2k())
			{
			printf("nand_erase_block_2k error!!!\n");
			return K_ERROR;
			}
		nand_addr_wr_phy_blk = wr_addr_save;
		nand_addr_rd_phy_blk = rd_addr_save;
		}
	return K_SUCCES;
}
unsigned int nand_media_copy_block_tail(void)
{
	//
	//printf("nand_media_copy_block_tail()\n");
	//
	unsigned char *nand_assign_map;
	nand_assign_map = (nand_addr_zone ? (nand_assign_map_buff + nand_assign_map_one_zone_size) : nand_assign_map_buff);
	if (nand_media_sts&0x40)
		{
		if(K_SUCCES !=nand_media_copy_block_2k((nand_pages_per_block - nand_addr_page)*nand_segments_per_page-nand_addr_segment))
			{
			return K_ERROR;
			}
		nand_addr_wr_phy_blk = nand_addr_rd_phy_blk;
		b_map_phy_blk_free(nand_assign_map, nand_addr_wr_phy_blk);
		return (nand_erase_block_2k());
		}
	else
		{
		if(K_SUCCES !=nand_media_copy_block(nand_pages_per_block - nand_addr_page))
			{
			return K_ERROR;
			}
		}
	return (nand_map_rebind_log2phy());
}
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
unsigned int nand_map_alloc_wr_blk(void)
{
	unsigned int bretry;
	unsigned int index;
	unsigned char  * nand_assign_map;
	
	index = (nand_addr_zone?1:0 );
	nand_assign_map = (nand_addr_zone ? (nand_assign_map_buff + nand_assign_map_one_zone_size) : nand_assign_map_buff);
	
	bretry = TRUE;
	//
	//printf("Get Free:");
	//
	while(bretry == TRUE)
		{
		for(nand_addr_wr_phy_blk = nand_assign_start[index] ; nand_addr_wr_phy_blk< nand_max_pb_per_zone;nand_addr_wr_phy_blk ++)
			{
				if(b_map_is_phy_blk_free(nand_assign_map , nand_addr_wr_phy_blk))
					{
					b_map_phy_blk_used(nand_assign_map , nand_addr_wr_phy_blk);
					if((++nand_assign_start[index] ) >= nand_max_pb_per_zone)
						{
						nand_assign_start[index]  = 0;
						}
					//
					//printf("PB0 %x\n",nand_addr_wr_phy_blk);
					//printf("nand_assign_start  %x\n",nand_assign_start[index] );
					//
					//nand_erase_block_2k();
					return K_SUCCES;
					}
			}
		for(nand_addr_wr_phy_blk = 0;nand_addr_wr_phy_blk< nand_assign_start[index];nand_addr_wr_phy_blk ++)
			{
				if(b_map_is_phy_blk_free(nand_assign_map , nand_addr_wr_phy_blk))
					{
					b_map_phy_blk_used(nand_assign_map , nand_addr_wr_phy_blk);
					if(( ++ nand_assign_start[index] ) >= nand_max_pb_per_zone)
						{
						nand_assign_start[index]  = 0;
						}
					//
					//printf("PB1 %x\n",nand_addr_wr_phy_blk);
					//
					//nand_erase_block_2k();
					return K_SUCCES;
					}
			}
		bretry = FALSE;
		}
	nand_addr_wr_phy_blk = nand_map_block_free ;
	//
	//printf("PB  none free \n");
	//
	unsigned int i ;
	for(i=0;i<128;i++)
		{
		printf("%x = %x ",i,nand_assign_map[i]);
		if((i&0x0f)==0x0f)
			printf("\n");
		}
	printf("%x\n",nand_assign_zone);
	return K_ERROR;
}

unsigned int nand_map_build_sector_map(void)
{
	unsigned int i;
	unsigned short max_lb,pb,max_used_pb;
	unsigned int bad_data;
	unsigned char * nand_log2phy_map;
	unsigned char  * nand_assign_map;
	
	nand_log2phy_map = (nand_addr_zone ? (nand_log2phy_map_buff + buff_nand_log2phy_map_one_zone_size): nand_log2phy_map_buff);
	nand_assign_map = (nand_addr_zone ? (nand_assign_map_buff + nand_assign_map_one_zone_size) : nand_assign_map_buff);
	nand_addr_page = 0;
	nand_addr_segment=0;

	// init log2phy table, and empty the assign map
	for( i=0 ; i < nand_logical_blocks_per_zone; i++ )
	//for( i=0 ; i < nand_physical_blocks_per_zone; i++ )
		{
		//printf("nand_physical_blocks_per_zone = %x\n",nand_physical_blocks_per_zone);
		nand_map_log2phy_set(nand_log2phy_map,i, nand_map_block_free );
		}
#if 0
	for ( i=0; i < (nand_assign_map_one_zone_size); i++ )
		{
		nand_assign_map[i] = 0;
		}
#else	
	memset(nand_assign_map,0,nand_assign_map_one_zone_size);
#endif
	//
	//printf("nand_map_build_sector_map....................\n");
	//
	max_used_pb = 0;
	max_lb = (nand_addr_zone ? nand_logical_blocks_per_zone : nand_logical_blocks_per_boot_zone);
	for(nand_addr_rd_phy_blk = 0; nand_addr_rd_phy_blk < nand_physical_blocks_per_zone;nand_addr_rd_phy_blk ++)
		{
		//printf("%d\n",nand_addr_rd_phy_blk);
		/*if(K_SUCCES == nand_media_is_phyblock_reserved())
			{
			//判断该block是否为保留block
			printf("\nnand_addr_rd_phy_blk : %x",nand_addr_rd_phy_blk);
			printf(" reserved\n");
			//
			b_map_phy_blk_used(nand_assign_map,nand_addr_rd_phy_blk);
			//在assign_map相应位置1
			continue;
			}*/
		if(K_SUCCES != nand_media_read_extra_data())
			{
			//判断该block读写是否正常
			printf("\nnand_addr_rd_phy_blk : %x",nand_addr_rd_phy_blk);
			printf(" read_extra_data error\n");
			//
			b_map_phy_blk_used(nand_assign_map,nand_addr_rd_phy_blk);
			continue;
			}
		if(K_SUCCES != nand_media_is_phyblock_valid())
			{
			//判断该block是否可用
			printf("nand_addr_rd_phy_blk : %x",nand_addr_rd_phy_blk);
			printf("phyblock invalid\n");
			//
			b_map_phy_blk_used(nand_assign_map,nand_addr_rd_phy_blk);
			continue;
			}
		if(K_SUCCES == nand_media_is_phyblock_blank())
			{
			//判断该block是否为空
			//
			//printf("%x  :",nand_addr_rd_phy_blk);
			//printf(" phyblock blank\n");
			//
			continue;
			}	
		//
		//printf("%x  :",nand_addr_rd_phy_blk);
		//printf(" phyblock not blank\n");
		b_map_phy_blk_used(nand_assign_map,nand_addr_rd_phy_blk);
		//
		max_used_pb = nand_addr_rd_phy_blk;
		//
		bad_data = K_ERROR;
		if(K_SUCCES == (bad_data = nand_media_block_has_bad_data()))
			{
			//this block has bad data
			//
			printf("\nnand_addr_rd_phy_blk : %x",nand_addr_rd_phy_blk);
			printf(" phyblock has bad data");
			//
			;
			}
		// pyh to log
		if(K_SUCCES != nand_media_phy2log())
			{
			//
			printf("\nnand_addr_rd_phy_blk : %x",nand_addr_rd_phy_blk);
			printf("  phy2log error");
			//
			continue;
			}
		if(nand_addr_log_blk >= max_lb)
			{
			continue;
			}	
		//
		//printf("\nnand_addr_log_blk : %x",nand_addr_log_blk);
		//
		if((nand_map_log2phy_get(nand_log2phy_map,nand_addr_log_blk) & nand_map_l2p_addr) == nand_map_block_free)
			{
			//printf("nand_addr_rd_phy_blk: %x\n",nand_addr_rd_phy_blk);
			//printf("nand_addr_log_blk: %x\n\n",nand_addr_log_blk);
			nand_map_log2phy_set(nand_log2phy_map,nand_addr_log_blk, (nand_addr_rd_phy_blk | ((K_SUCCES == bad_data) ? nand_map_blk_has_bad_data : 0)));
			continue;
			}		
		//below is important to deal with one lb to more pb 
		//printf("deal with two PHY blocks which have the same LOG block.\n");
		//printf("Zone: %x\n",nand_addr_zone);
		//printf("nand_addr_rd_phy_blk: %x\n",nand_addr_rd_phy_blk);
		//printf("nand_addr_log_blk: %x\n",nand_addr_log_blk);
		//
		pb = nand_addr_rd_phy_blk ;
		nand_addr_wr_phy_blk = (nand_map_log2phy_get(nand_log2phy_map,nand_addr_log_blk) & nand_map_l2p_addr); 
#if 0
		nand_media_resolve_conflict();
#else
		nand_map_resolve_conflict();
#endif
		nand_map_log2phy_set(nand_log2phy_map,nand_addr_log_blk,nand_addr_rd_phy_blk );
		if( K_SUCCES == nand_media_erase_block() )
			{
			b_map_phy_blk_free( nand_assign_map, nand_addr_wr_phy_blk ) ;
			}
		//
		nand_addr_rd_phy_blk = pb ;
		nand_addr_page=0;
		nand_addr_wr_phy_blk=nand_map_block_free;		
		}
	       /*
		for (i=0;i<((nand_logical_blocks_per_zone>>1)*3);i++)
		{
		if ((i&0x0f)==0)
		printf ("\n%4d:",i);
		printf("%x ",nand_log2phy_map_buff[i]);		
		}
		for ( i=0; i < nand_assign_map_one_zone_size; i++ )
		{
		if ((i&0x0f)==0)
		printf ("\n%4d:",i);
		printf("%x  ",nand_assign_map[i]);
		}
		*/	
	//printf("\n");
	//debug_printf_l2p_map();	//printf
	//
	if( (nand_assign_start[(nand_addr_zone ? 1 : 0)] =(max_used_pb +1))  >= nand_physical_blocks_per_zone)
		{
		nand_assign_start[(nand_addr_zone ? 1 : 0)] = 0;
		}
	/*printf("nand_assign_start[1]=%x\n",nand_assign_start[1]);
	printf("nand_assign_start[0]=%x\n",nand_assign_start[0]);
	printf("%x\n ",nand_addr_zone);
	i=(nand_addr_zone?1:0);
	printf("%x\n ",i);
	for(i=0;i<nand_assign_map_size;i++)
		printf("%x ",nand_assign_map[i]);*/
	return K_SUCCES;
}
unsigned int nand_map_lba2addr_rd(unsigned int lba)
{
	unsigned int lb_all;
	unsigned short lb_save;
	unsigned char page_save;
	unsigned char * nand_log2phy_map;
		{
		lb_all = (lba / nand_pages_per_block);
		nand_addr_page = (lba % nand_pages_per_block);
		nand_addr_segment =0;
		}
	//-----------------------------------
	if(lb_all >= nand_logical_blocks_per_boot_zone)
		{
		lb_all -= nand_logical_blocks_per_boot_zone;
		nand_addr_zone = (lb_all / nand_logical_blocks_per_zone) +1 ;
		nand_addr_log_blk = (lb_all % nand_logical_blocks_per_zone);
		}
	else
		{
		nand_addr_zone = 0;
		nand_addr_log_blk =lb_all ;
		}
	//
	if(nand_addr_zone >= nand_num_zones)
		{
		//it is larger than the max zone of the nand flash
		return K_ERROR;
		}
	//
	if( nand_addr_zone && (nand_addr_zone != nand_assign_zone))
		{//new zone and need new log2phy map making
		//
		//printf("zone 1 active^^^^^^^^^^^^^^^^^^\n");
		//
		nand_assign_zone = 0;
		lb_save = nand_addr_log_blk;
		page_save = nand_addr_page;
		if(K_SUCCES != nand_map_build_sector_map())
			{
			//
			return K_ERROR;
			}
		nand_assign_zone = nand_addr_zone;
		nand_addr_log_blk = lb_save;
		nand_addr_page = page_save;
		}
	//get lb's pb from map
	nand_log2phy_map = (nand_addr_zone ? (nand_log2phy_map_buff + buff_nand_log2phy_map_one_zone_size) : nand_log2phy_map_buff);	
	nand_addr_rd_phy_blk = (nand_map_log2phy_get(nand_log2phy_map,nand_addr_log_blk) & nand_map_l2p_addr);
	//
	if(nand_addr_rd_phy_blk == nand_map_block_free)
		{//now no pb fro lb
		unsigned short pb_wr_save;

		pb_wr_save = nand_addr_wr_phy_blk;
		if(K_SUCCES!= nand_map_alloc_wr_blk())//循环查找空的block
			{
			return K_ERROR;
			}
		nand_addr_rd_phy_blk = nand_addr_wr_phy_blk;
		nand_map_log2phy_set(nand_log2phy_map,nand_addr_log_blk, (nand_addr_rd_phy_blk | nand_map_soft_l2p_binding));
		nand_addr_wr_phy_blk = pb_wr_save;
		}
	//
	nand_media_bind_log2phy();
	return K_SUCCES;
}

unsigned int nand_read_log2phy_2k(unsigned int * udisk_lba_param)
{
	unsigned int result,copy_num;
	unsigned int lb_all,lba,temp;
	unsigned short lb_save,page_save,segment_save;
	unsigned addr_save;
	unsigned char bak_nand_addr_page,bak_nand_addr_segment;
	unsigned char * nand_log2phy_map;

	lba = udisk_lba_param[index_udisk_lba_start];
	lb_all = lba >>8;
    	nand_addr_page = (lba&0xff)>>2;
	extern	unsigned char nand_addr_segment ; 
    	nand_addr_segment=lba&3;
	if(lb_all >= nand_logical_blocks_per_boot_zone)
		{
		nand_addr_zone = (lb_all / nand_logical_blocks_per_zone)  ;
		nand_addr_log_blk = (lb_all % nand_logical_blocks_per_zone);
		}
	else
		{
		nand_addr_zone = 0;
		nand_addr_log_blk =lb_all ;
		}
	//
	if(nand_addr_zone >= nand_num_zones)
		{
		//it is larger than the max zone of the nand flash
		return K_ERROR;
		}
	//printf("\n\nnand_addr_zone = %x   nand_assign_zone = %x\n\n",nand_addr_zone,nand_assign_zone);
	//
	if( nand_addr_zone && (nand_addr_zone != nand_assign_zone))
		{//new zone and need new log2phy map making
		//
		//printf("zone 1 active^^^^^^^^^^^^^^^^^^\n");
		//
		nand_assign_zone = 0;
		lb_save = nand_addr_log_blk;
		page_save = nand_addr_page;
		segment_save = nand_addr_segment;
		if(K_SUCCES != nand_map_build_sector_map())
			{
			//
			return K_ERROR;
			}
		nand_assign_zone = nand_addr_zone;
		nand_addr_log_blk = lb_save;
		nand_addr_page = page_save;
		nand_addr_segment = segment_save;
		}
	//nand_assign_zone = 0;
	
	
       temp = lba&0xff;
	temp +=  udisk_lba_param[index_udisk_lba_xfer_count];
	if(temp>256)
		udisk_lba_param[index_udisk_lba_split_max_count] =256-(lba&0xff);
	else
		udisk_lba_param[index_udisk_lba_split_max_count] =udisk_lba_param[index_udisk_lba_xfer_count];

	//get lb's pb from map
	nand_log2phy_map = (nand_addr_zone ? (nand_log2phy_map_buff + buff_nand_log2phy_map_one_zone_size) : nand_log2phy_map_buff);	
	nand_addr_rd_phy_blk = (nand_map_log2phy_get(nand_log2phy_map,nand_addr_log_blk) & nand_map_l2p_addr);
	//
	return K_SUCCES;
}

unsigned int nand_write_log2phy_2k(unsigned int * udisk_lba_param)
{
	unsigned int result,copy_num;
	unsigned int lb_all,lba,temp;
	unsigned short lb_save,page_save,segment_save;
	unsigned addr_save;
	unsigned char bak_nand_addr_page,bak_nand_addr_segment;
	unsigned char * nand_log2phy_map,* nand_assign_map;

	lba = udisk_lba_param[index_udisk_lba_start];
	lb_all = lba >>8;
    	nand_addr_page = (lba&0xff)>>2;
	extern	unsigned char nand_addr_segment ; 
    	nand_addr_segment=lba&3;
	if(lb_all >= nand_logical_blocks_per_boot_zone)
		{
		nand_addr_zone = (lb_all / nand_logical_blocks_per_zone)  ;
		nand_addr_log_blk = (lb_all % nand_logical_blocks_per_zone);
		}
	else
		{
		nand_addr_zone = 0;
		nand_addr_log_blk =lb_all ;
		}
	//
	if(nand_addr_zone >= nand_num_zones)
		{
		//it is larger than the max zone of the nand flash
		printf("it is larger than the max zone of the nand flash\n");
		return K_ERROR;
		}
	//if(nand_addr_zone!=0)
		//printf("\n\nnand_addr_zone = %x   nand_assign_zone = %x\n\n",nand_addr_zone,nand_assign_zone);
	//
	//
	if( nand_addr_zone && (nand_addr_zone != nand_assign_zone))
		{//new zone and need new log2phy map making
		//
		//printf("zone 1 active^^^^^^^^^^^^^^^^^^\n");
		//
		nand_assign_zone = 0;
		lb_save = nand_addr_log_blk;
		page_save = nand_addr_page;
		segment_save = nand_addr_segment;
		if(K_SUCCES != nand_map_build_sector_map())
			{
			//
			printf("nand_map_build error!! \n");
			return K_ERROR;
			}
		nand_assign_zone = nand_addr_zone;
		nand_addr_log_blk = lb_save;
		nand_addr_page = page_save;
		nand_addr_segment = segment_save;
		}
	//nand_assign_zone = 0;

       temp = lba&0xff;
	temp +=  udisk_lba_param[index_udisk_lba_xfer_count];
//printf("temp = 0x%08x\n",temp);	
	if(temp>256)
		udisk_lba_param[index_udisk_lba_split_max_count] =256-(lba&0xff);
	else
		udisk_lba_param[index_udisk_lba_split_max_count] =udisk_lba_param[index_udisk_lba_xfer_count];


/*****************************************************/
	nand_log2phy_map = (nand_addr_zone ? (nand_log2phy_map_buff + buff_nand_log2phy_map_one_zone_size): nand_log2phy_map_buff);
	nand_assign_map = (nand_addr_zone ? (nand_assign_map_buff + nand_assign_map_one_zone_size) : nand_assign_map_buff);
	nand_addr_wr_phy_blk = (nand_map_log2phy_get(nand_log2phy_map,nand_addr_log_blk) & nand_map_l2p_addr);

	//
	if(nand_addr_wr_phy_blk == nand_map_block_free) // 没有映射，查找新的block
		{
		if(K_SUCCES!= nand_map_alloc_wr_blk())//循环查找空的block
			{
			return K_ERROR;
			}
		nand_write_state_2k|=8;
		nand_map_log2phy_set(nand_log2phy_map,nand_addr_log_blk, nand_addr_wr_phy_blk );
		b_map_phy_blk_used(nand_assign_map,nand_addr_wr_phy_blk);
		nand_write_state_2k|=4;
		nand_write_state_2k&=0xf7;
		//空block 不需要write tail
		}
	else // 有映射
		{		
		nand_addr_rd_phy_blk = nand_addr_wr_phy_blk;
#ifdef DISK_TEST_MODE_ENABLE		
printf("nand_addr_wr_phy_blk = 0x%08x, \n",nand_addr_wr_phy_blk);
#endif			
              result = nand_block_check_space_2k(udisk_lba_param); // 1:have enough segment; 0:no space;
		if(result && nand2k_need_copy_tail)
			{  // have enough segment, and no need copy
			nand_write_state_2k|=2;
#ifdef DISK_TEST_MODE_ENABLE			
printf("have enough segment!!\n");
#endif
			return K_SUCCES;
			}
		else
			{ // no enough segment, need seach new block and copy head and tail			
			// ???	
			copy_num= (nand_addr_page<<2)+nand_addr_segment;
			temp = (nand2k_copy_start_page<<2)+nand2k_copy_start_segment;
			if((0==nand2k_need_copy_tail)&&(copy_num>=temp))  
				{// some block no need copy
#ifdef DISK_TEST_MODE_ENABLE			
printf("some block no need copy!\n");
#endif								
				if(copy_num>temp)
					{ // need copy some block first
#ifdef DISK_TEST_MODE_ENABLE			
printf("need copy some block first!, copy_len=%d,\n",copy_num-temp);
#endif					
					bak_nand_addr_page = nand_addr_page;
					bak_nand_addr_segment = nand_addr_segment;
					nand_addr_page = nand2k_copy_start_page;
					nand_addr_segment = nand2k_copy_start_segment;
					nand_addr_rd_phy_blk = nand2k_copy_rd_blk;
					if(K_SUCCES != nand_media_copy_block_2k(copy_num-temp))
						{
						printf("nand_media_copy_block_2k error!!!\n");
						return K_ERROR;
						}
					nand_addr_rd_phy_blk = nand_addr_wr_phy_blk;
					nand_addr_page = bak_nand_addr_page;
					nand_addr_segment = bak_nand_addr_segment;
					}				
				copy_num += udisk_lba_param[index_udisk_lba_xfer_count];
				if(0x100>(copy_num))
					{						
					nand2k_copy_start_page = copy_num>>2;
					nand2k_copy_start_segment = copy_num&3;
#ifdef DISK_TEST_MODE_ENABLE			
printf("still has some block need copy! page=%d, segment=%d, \n",nand2k_copy_start_page,nand2k_copy_start_segment);
#endif					
					}
				else
					{ // need erase block 
#ifdef DISK_TEST_MODE_ENABLE			
printf("no block need copy, and need erase block, 0x%08x,\n",nand_addr_wr_phy_blk);
#endif					
					nand2k_need_copy_tail = 1;					
					nand_addr_wr_phy_blk = nand2k_copy_rd_blk;
					b_map_phy_blk_free(nand_assign_map, nand_addr_wr_phy_blk);
					if(K_SUCCES != nand_erase_block_2k())
						{
						printf("nand_erase_block_2k error!!!\n");
						return K_ERROR;
						}
					nand_addr_wr_phy_blk = nand_addr_rd_phy_blk;
					}
				}
			else
				{
#if 1
				if(0==nand2k_need_copy_tail)
					{ // spec handle for copy_num<temp
					nand2k_copy_block_tail(udisk_lba_param);
					}
#endif				
				if(K_SUCCES != nand_map_alloc_wr_blk())
					{
					printf("nand_map_alloc_wr_blk error !!!!");
					return K_ERROR;
					}
					nand_map_log2phy_set(nand_log2phy_map,nand_addr_log_blk, nand_addr_wr_phy_blk );
					b_map_phy_blk_used(nand_assign_map,nand_addr_wr_phy_blk);
				// first copy head
				if(K_SUCCES != nand2k_media_copy_block_head(udisk_lba_param))
					{
					printf("nand2k_media_copy_block_head error !!!!");
					return K_ERROR;
					}
				copy_num += udisk_lba_param[index_udisk_lba_xfer_count];
				//if((nand_addr_log_blk>1)&&(0x100>(copy_num)))
				if((nand_addr_log_blk>1)&&(0x100>(copy_num))&&(0==(lun_num&0xf0)))
					{											
#ifdef DISK_TEST_MODE_ENABLE			
printf("set copy address...!\n");
#endif					
					nand2k_need_copy_tail = 0;
					nand2k_copy_rd_blk = nand_addr_rd_phy_blk;
					nand2k_copy_wr_blk = nand_addr_wr_phy_blk;
					nand2k_copy_log_blk = nand_addr_log_blk;
					nand2k_copy_addr_zone = nand_addr_zone;
					nand2k_copy_start_page = copy_num>>2;
					nand2k_copy_start_segment = copy_num&3;						
					}
				else
					{// no need copy data
					nand2k_media_copy_block_tail(udisk_lba_param,1);
					//second copy tail
					}
				}
			}
		}
	//printf("nand_write_log2phy_2k end \n");
	return K_SUCCES;	
}

unsigned int nand_map_dealing_writing_error(unsigned char * databuff)
{
	//save current read
	unsigned short  _nand_addr_rd_phy_blk = nand_addr_rd_phy_blk;
	unsigned short  _nand_addr_wr_phy_blk = nand_addr_wr_phy_blk;
	unsigned short  _nand_addr_wr_phy_blk2 ;
	unsigned char _nand_addr_page = nand_addr_page;
	unsigned char tempbuff[512];
	//save nand_page_buff;
	memcpy(tempbuff,nand_page_buff,0x200);
	//set nand_page_buff
	memcpy(nand_page_buff,databuff,0x200);
	//set current wr block as rd block
	nand_addr_rd_phy_blk = nand_addr_wr_phy_blk;
	//
get_new_free_block_for_writing:
	if(K_SUCCES != nand_map_alloc_wr_blk())
		{
		nand_addr_rd_phy_blk = _nand_addr_rd_phy_blk;
		memcpy(nand_page_buff,tempbuff,0x200);
		return K_ERROR;
		}
	//write nand_addr_page
	if(K_SUCCES != nand_write_sector())
		{
		nand_addr_page = 0;
		nand_write_extra_data_to_set_bad_block();
		nand_addr_page = _nand_addr_page;
		nand_soft_reset();
		goto get_new_free_block_for_writing;
		}
	//if needing,do copy header
	if(0 != nand_addr_page)
		{
		if(K_SUCCES != (nand_media_copy_block_head()))
			{
			nand_addr_page = 0;
			nand_write_extra_data_to_set_bad_block();
			//
			nand_addr_page = _nand_addr_page;
			nand_soft_reset();
			goto get_new_free_block_for_writing;
			}
		else
			{
			//set rd block bad block
			//save new wr block
			_nand_addr_wr_phy_blk2 = nand_addr_wr_phy_blk;
			 nand_addr_wr_phy_blk = _nand_addr_wr_phy_blk;
			 //
			 nand_addr_page = 0;
			 nand_write_extra_data_to_set_bad_block();
			 //restore wr block
			 nand_addr_wr_phy_blk = _nand_addr_wr_phy_blk2;
			 nand_addr_page = _nand_addr_page;
			}
		}
	nand_addr_rd_phy_blk = _nand_addr_rd_phy_blk;
	memcpy(nand_page_buff,tempbuff,0x200);
	return K_SUCCES;
}
unsigned int nand_map_erase_block(void)
{
	unsigned char  * nand_assign_map;
	nand_assign_map = (nand_addr_zone ? (nand_assign_map_buff + nand_assign_map_one_zone_size) : nand_assign_map_buff);
	if(K_SUCCES != nand_media_erase_block())
		{
		if(K_SUCCES != nand_media_erase_block())
			{
			//set it bad block
			//
			return K_ERROR;
			}
		}
	b_map_phy_blk_free(nand_assign_map,nand_addr_rd_phy_blk);
	return K_SUCCES;
}
unsigned int nand_map_write_begin(unsigned int lba)
{
	unsigned int result;
	if(K_SUCCES != nand_map_lba2addr_rd(lba))
		{
			return K_ERROR;
		}
	if(nand_addr_page != 0)
		{
		if(K_SUCCES != nand_map_alloc_wr_blk())
			{
			return K_ERROR;
			}
		if(K_SUCCES != (result = nand_media_copy_block_head()))
			{
			if(k_media_copy_error_src == result )
				{
					;
				}
			else
				{
				nand_map_erase_block();
				printf("erase ........................................\n");
				}
			return K_ERROR;
			}
		}
	else
		{
		if(K_SUCCES != nand_map_alloc_wr_blk())
			{
			return K_ERROR;
			}
		}
	return K_SUCCES;
}

unsigned int nand_map_write_begin_2k(unsigned int * udisk_lba_param)
{
	unsigned int result;
	unsigned char *nand_assign_map,*nand_log2phy_map;
	nand_assign_map = (nand_addr_zone ? (nand_assign_map_buff + nand_assign_map_one_zone_size) : nand_assign_map_buff);
	nand_log2phy_map = (nand_addr_zone ? (nand_log2phy_map_buff + buff_nand_log2phy_map_one_zone_size): nand_log2phy_map_buff);
	if((nand_write_state_2k&8)==0)
		{
		unsigned addr_save;
		if(nand_block_have_enough_segment(udisk_lba_param))
			{
			nand_write_state_2k|=2;
			//printf("have enough segment!!\n");
			return K_SUCCES;
			}
		else
			{
			nand_addr_rd_phy_blk = nand_addr_wr_phy_blk;
			if(K_SUCCES != nand_map_alloc_wr_blk())
				{
				printf("nand_map_alloc_wr_blk error !!!!");
				return K_ERROR;
				}
			nand_erase_block_2k();
			nand_map_log2phy_set(nand_log2phy_map,nand_addr_log_blk, nand_addr_wr_phy_blk );
			b_map_phy_blk_used(nand_assign_map,nand_addr_wr_phy_blk);
			if(K_SUCCES != (result = nand_media_copy_block_head_2k()))
				{
				if(k_media_copy_error_src == result )
					{
						;
					}
				else
					{
					nand_map_erase_block();
					//printf("erase ........................................\n");
					}
				printf("nand_media_copy_block_head_2k!!!!");
				return K_ERROR;
				}
			}
		/*nand_media_copy_block_head_2k();
		addr_save=nand_addr_wr_phy_blk;
		nand_addr_wr_phy_blk=nand_addr_rd_phy_blk;
		nand_map_erase_block();
		printf("erase .............................!");
		nand_addr_wr_phy_blk=addr_save;*/
		}
	else
		{
		nand_write_state_2k|=4;
		nand_write_state_2k&=0xf7;
		//空block 不需要write tail
		return K_SUCCES;
		}
	//printf("nand_map_write_begin_2k %x\n",nand_addr_wr_phy_blk);
	//printf("nand_map_write_begin_2k %x\n",nand_addr_rd_phy_blk);
	
}

// return 0: means error
unsigned short nand_get_valid_segment_num(unsigned short total_seg)
{
	unsigned short save_log_blk,return_val,i;
	save_log_blk = nand_addr_log_blk ;
	return_val = 0;
	for(i=0;i<total_seg;i++)
		{
		nand_addr_page = i/nand_segments_per_page;
		nand_addr_segment = i%nand_segments_per_page;
		if ( K_SUCCES != nand_media_read_extra_data() )		
			return 0;
		
		if ( K_SUCCES != nand_media_phy2log() )		
			return 0;
		if(save_log_blk == nand_addr_log_blk)
			return_val++;
		}
	return return_val;
	
}

unsigned int nand_map_resolve_conflict(void)
{
#if 0
	unsigned short save_rd_pb ;
	unsigned short save_wr_pb ;
	unsigned short save_lb ;
	unsigned short pb1 ;
	unsigned short lb1_first ;
	unsigned short lb1_last ;
	unsigned short pb2 ;
	unsigned short lb2_first ;
	unsigned short lb2_last ;
	
	save_rd_pb = nand_addr_rd_phy_blk ;
	save_wr_pb = nand_addr_wr_phy_blk ;
	save_lb = nand_addr_log_blk ;
	
	pb1 = nand_addr_rd_phy_blk ;
	pb2 = nand_addr_wr_phy_blk ;
	lb1_first = lb2_first = nand_addr_log_blk ;

	nand_addr_page = ( nand_pages_per_block-1);

	if ( K_SUCCES != nand_media_read_extra_data() )
		{
		nand_addr_rd_phy_blk = pb2 ;
		nand_addr_wr_phy_blk = pb1 ;
		nand_addr_log_blk = save_lb ;
		return K_SUCCES;
		}
	if ( K_SUCCES != nand_media_phy2log() )
		{
		nand_addr_rd_phy_blk = pb2 ;
		nand_addr_wr_phy_blk = pb1 ;
		nand_addr_log_blk = save_lb ;
		return K_SUCCES ;
		}
	lb1_last = nand_addr_log_blk ;
	if( lb1_first != lb1_last)
		{
		nand_addr_rd_phy_blk = pb2 ;
		nand_addr_wr_phy_blk = pb1 ;
		nand_addr_log_blk = save_lb ;
		return K_SUCCES ;
		}

	nand_addr_rd_phy_blk = pb2 ;
	if ( K_SUCCES != nand_media_read_extra_data() )
		{
		nand_addr_rd_phy_blk = pb1 ;
		nand_addr_wr_phy_blk = pb2 ;
		nand_addr_log_blk = save_lb ;
		return K_SUCCES ;
		}
	if ( K_SUCCES != nand_media_phy2log() )
		{
		nand_addr_rd_phy_blk = pb1 ;
		nand_addr_wr_phy_blk = pb2 ;
		nand_addr_log_blk = save_lb ;
		return K_SUCCES ;
		}
	
 	lb2_last = nand_addr_log_blk ;
	if( lb2_first != lb2_last)
		{
		nand_addr_rd_phy_blk = pb1 ;
		nand_addr_wr_phy_blk = pb2 ;
		nand_addr_log_blk = save_lb ;
    		return K_SUCCES;
		}
	nand_addr_rd_phy_blk = save_rd_pb ;
	nand_addr_wr_phy_blk = save_wr_pb ;
	nand_addr_log_blk = save_lb ;
	return K_ERROR;
#else
       // conflict 3 rule: 1. check head segment; 2. check tail segment; 3 check all valid segment;
	unsigned short save_rd_blk ,save_wr_blk ,save_log_blk, total_segment;
	unsigned short temp , rd_log_blk,wr_log_blk ,temp1;
	
	// rule 1: has checked
	save_rd_blk = nand_addr_rd_phy_blk ;
	save_wr_blk = nand_addr_wr_phy_blk ;
	save_log_blk = nand_addr_log_blk ;
	total_segment = nand_pages_per_block*nand_segments_per_page;
#ifdef DISK_TEST_MODE_ENABLE		
printf(" rule 2: check tail segment; \n");	
#endif	
	// rule 2: check tail segment; 
	temp = total_segment -1 ;
	nand_addr_page = temp/nand_segments_per_page;
	nand_addr_segment = temp%nand_segments_per_page;
	if ( K_SUCCES != nand_media_read_extra_data() )		
		goto conflict_wr_ok;
printf(" 1.\n");		
	if ( K_SUCCES != nand_media_phy2log() )		
		goto conflict_wr_ok;
printf(" 2.\n");			
	rd_log_blk = nand_addr_log_blk;
	nand_addr_rd_phy_blk = nand_addr_wr_phy_blk ;
	if ( K_SUCCES != nand_media_read_extra_data() )		
		goto conflict_rd_ok;
printf(" 3.\n");			
	if ( K_SUCCES != nand_media_phy2log() )		
		goto conflict_rd_ok;
printf(" 4.\n");				
	wr_log_blk = nand_addr_log_blk;
	if((rd_log_blk == save_log_blk)&&(wr_log_blk != save_log_blk))		
		goto conflict_rd_ok;
printf(" 5.\n");			
	if((rd_log_blk != save_log_blk)&&(wr_log_blk == save_log_blk))		
		goto conflict_wr_ok;
printf(" 6.\n");			

	// rule 3: check all valid segment;
	nand_addr_rd_phy_blk = save_rd_blk ;
	nand_addr_log_blk = save_log_blk;
	temp = nand_get_valid_segment_num(total_segment);
	if(0==temp)
		goto conflict_wr_ok;
printf(" 7.\n");	
	nand_addr_rd_phy_blk = save_wr_blk ;
	nand_addr_log_blk = save_log_blk;
	temp1 = nand_get_valid_segment_num(total_segment);
	if(0==temp1)
		goto conflict_rd_ok;
printf(" 8.\n");		
	if(temp<temp1)
		goto conflict_wr_ok;
printf(" 9.\n");	
conflict_rd_ok:
		nand_addr_rd_phy_blk = save_rd_blk ;
		nand_addr_wr_phy_blk = save_wr_blk ;
		nand_addr_log_blk = save_log_blk;
		nand_addr_page = 0;
		nand_addr_segment = 0; 
		return K_SUCCES;
		
conflict_wr_ok:
		nand_addr_rd_phy_blk = save_wr_blk ;
		nand_addr_wr_phy_blk = save_rd_blk ;
		nand_addr_log_blk = save_log_blk;
		nand_addr_page = 0;
		nand_addr_segment = 0;
		return K_SUCCES ;	
#endif
}
unsigned int nand_map_rebind_log2phy(void)
{
	unsigned char * nand_log2phy_map;
	if(K_SUCCES != nand_media_erase_rd_block())
		{
		printf("nand_media_erase_rd_block() error\n");
		return K_ERROR;
		}
	nand_log2phy_map = (nand_addr_zone ? (nand_log2phy_map_buff +buff_nand_log2phy_map_one_zone_size) : nand_log2phy_map_buff);
	nand_map_log2phy_set(nand_log2phy_map,nand_addr_log_blk, nand_addr_wr_phy_blk);
	nand_addr_wr_phy_blk = nand_map_block_free;
	return K_SUCCES;
}


