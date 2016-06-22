/*
  Name: nand_media.h

*/
#ifndef _NAND_MEDIA_H_
#define  _NAND_MEDIA_H_
//----------------------------------------------------------------------------
#define K_SUCCES		0
#define K_ERROR			1
#define K_IN_PROGRESS	2
#define k_FINISHED		3
#define k_IGNORED		4
#define k_ABORTED		5
#define k_TIMEOUT		6
#define k_USBRESET		7
#define k_RESUME		8
#define k_media_copy_error_src		9
#define k_media_copy_error_dst		10
//----------------------------------------------------------------------------
#define nand_media_device_attrible_default     		0x00

#define nand_media_device_attrible_ignore_bad_cis     0x01


//------------------------------------------------------------------------------
//
#define nand_media_addr_4cyc  		0x01
#define nand_media_sts_rdy         		0x02         // r    sm device ready (reflects SM_nB/R signal)
#define nand_media_sts_256_page    	0x04         // rw   1=SM media is 256+8 bpp format, 0=SM media is 512+16 bpp format
#define nand_media_sts_wp          		0x08         // r    1=SM media is write protected.  only valid when smc enabled
#define nand_media_sts_wrprot        	0x10
#define nand_media_addr_5cyc  		0x20
#define nand_media_sts_2k_page    	0x40
//------------------------------------------------------------------------------
#define nand_page_size        		512
#define nand_redt_buffer_size   	16

//------------------------------------------------------------------------------
// Max Zone/Block/Sectors Data Definition
//------------------------------------------------------------------------------
#define nand_max_zones_per_table 		2
#define nand_max_zone_num        			8
#define nand_max_pb_per_zone       		1024
#define nand_max_lb_per_zone     		1000
#define nand_max_cis_sect        			0x08
//-----------------------------------------------------------------------------
// bit control macros , bit addr from 0 to 31
//-----------------------------------------------------------------------------
#define b_setbit(b_buffer, b_bitaddr) 		(b_buffer[b_bitaddr>>3]  |=  (1<<(b_bitaddr & 0x07)))
#define b_clrbit(b_buffer, b_bitaddr) 		(b_buffer[b_bitaddr>>3] &=  (~(1<<(b_bitaddr & 0x07))))
#define b_chkbit(b_buffer, b_bitaddr) 		(b_buffer[b_bitaddr>>3] &    (1<<(b_bitaddr & 0x07)))
#define b_flpbit(b_buffer, b_bitaddr) 		(b_buffer[b_bitaddr>>3] ^=  (1<<(b_bitaddr & 0x07))

#define b_map_phy_blk_used(b_map, b_blk )     	(   b_setbit(b_map, b_blk ))
#define b_map_phy_blk_free( b_map, b_blk )     	(   b_clrbit(b_map, b_blk ))
#define b_map_is_phy_blk_free(b_map, b_blk )  	(!(b_chkbit(b_map, b_blk)))
#define b_map_is_phy_blk_used(b_map, b_blk )  	(   b_chkbit(b_map, b_blk))


//------------------------------------------------------------------------------
#define nand_option_rderr_reassign  1  /* Reassign with Read Error */
#define nand_option_l2p_err_erase   1  /* BlockErase for Contradicted L2P Table */
#define nand_option_use_hw_ecc      0  /* 0:do sw ecc, 1:use hw ecc support */
//------------------------------------------------------------------------------
#define redt_reserved0		0x00
#define redt_reserved1		0x01
#define redt_reserved2		0x02
#define redt_reserved3		0x03
#define redt_data_status   	0x04
#define redt_block_status  	0x05
#define redt_lba1_hi       		0x06
#define redt_lba1_lo       		0x07
#define redt_lba2_hi       		0x0B
#define redt_lba2_lo       		0x0C
#define redt_ecc1_0        		0x0D
#define redt_ecc1_1        		0x0E
#define redt_ecc1_2        		0x0F
#define redt_ecc2_0        		0x08
#define redt_ecc2_1        		0x09
#define redt_ecc2_2        		0x0A
//------------------------------------------------------------------------------
//device id
#define nand_id_maker		0
#define nand_id_device		1
#define nand_id_3			2
#define nand_id_4			3
#define nand_id_5			4
#define nand_id_max			(nand_id_5+1)
//maker id
#define MAKER_TOSHIBA		0x98
#define MAKER_SAMSUNG		0xEC
#define MAKER_HYNIX			0xAD
#define MAKER_MICRON		0x2C
#define MAKER_RENESARS		0x07
#define MAKER_ST			0x20
//------------------------------------------------------------------------------
#if 1
#define nand_map_block_free              		0x07FF
#define nand_map_soft_l2p_binding 			0x0800
#define nand_map_blk_has_bad_data 			0x0000
#define nand_map_l2p_addr         			0x7FF
#else
#define nand_map_block_free              		0x0FFF
#define nand_map_soft_l2p_binding 		0x8000
#define nand_map_blk_has_bad_data 		0x4000
#define nand_map_l2p_addr         			0x0FFF
#endif


#define buff_nand_log2phy_map_one_zone_size	(nand_max_pb_per_zone*12/8) // 1024*12/8
#define buff_nand_log2phy_map_size				(nand_max_zones_per_table*buff_nand_log2phy_map_one_zone_size)
#define nand_assign_map_one_zone_size			(nand_max_pb_per_zone/8)//1024/8
#define nand_assign_map_size					(nand_max_zones_per_table*nand_assign_map_one_zone_size)

//------------------------------------------------------------------------------
extern unsigned int bit_count8(unsigned char val);
extern unsigned int bit_count16(unsigned short val);
extern void nand_media_init_nand_parameter(void);
extern unsigned int nand_init_media(void);
extern unsigned int nand_media_erease_all_valid_block(void);
extern unsigned int nand_media_seek_cis(void);
extern unsigned int nand_media_read_extra_data(void);
extern unsigned int nand_media_is_phyblock_valid(void);
extern unsigned int nand_media_is_page_data_valid(void);
extern unsigned int nand_media_is_phyblock_blank(void);
extern unsigned int  nand_media_set_size(unsigned char idcode);
extern unsigned int nand_map_resolve_conflict(void);
extern unsigned int nand_map_rebind_log2phy(void);
extern unsigned int nand_map_dealing_writing_error(unsigned char * databuff);

#ifndef _NAND_MEDIA_C_

extern unsigned char nand_num_zones;
extern unsigned short nand_physical_blocks_per_zone;
extern unsigned short nand_logical_blocks_per_zone;
extern unsigned short nand_logical_blocks_per_boot_zone; 
extern unsigned char nand_pages_per_block;

extern unsigned char nand_segments_per_page;
extern unsigned char nand_boot_block;  
extern unsigned char nand_boot_page;

extern unsigned char nand_addr_zone;
extern unsigned char nand_assign_zone;
extern unsigned char nand_addr_page;
extern unsigned short nand_addr_rd_phy_blk;
extern unsigned short nand_addr_wr_phy_blk;
extern unsigned short nand_addr_log_blk;


extern unsigned char nand_media_device_attrible;
extern unsigned char nand_media_sts;
extern unsigned char * nand_redt_data;
extern unsigned char * nand_page_buff;
extern unsigned char nand_log2phy_map_buff[buff_nand_log2phy_map_size/4];
extern unsigned char  nand_assign_map_buff[nand_assign_map_size/4];

extern unsigned char nand_addr_zone;
extern unsigned char nand_addr_page;
extern unsigned char nand_write_state_2k;

extern unsigned short nand_assign_start[2];

extern unsigned short nand2k_copy_rd_blk;
extern unsigned short nand2k_copy_wr_blk;
extern unsigned short nand2k_copy_log_blk;
extern unsigned char nand2k_need_copy_tail;  // needed:0, else no need
extern unsigned char nand2k_copy_start_page;
extern unsigned char  nand2k_copy_start_segment;	
extern unsigned char nand2k_copy_addr_zone;

#endif



//-----------------------------------------
//for debug
extern void debug_get_lba(unsigned int * udisk_lba_param);
extern void debug_set_write_data(void);
extern void debug_get_write_data(void);
extern void debug_printf_nand_info(void);
extern void debug_printf_l2p_map(void);
extern void debug_printf_assign_map(void);
extern void debug_page_buff(unsigned char *buff , unsigned int count);
//----------------------------------------
#endif
