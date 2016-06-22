
#ifndef _NAND_H__
#define _NAND_H__

#include "nfc.h"
/***************************************************************************
SmartMedia Command & Status Definition
***************************************************************************/
/* SmartMedia Command */
#define k_nand_read         		0x00 
#define k_nand_read2        		0x01 
#define k_nand_write       			0x10 
#define k_nand_read_redt    		0x50 
#define k_nand_erase1       		0x60 
#define k_nand_read_status  		0x70 
#define k_nand_read_id      		0x90 
#define k_nand_write_data   		0x80 
#define k_nand_erase2       		0xD0 
#define k_nand_reset_chip   		0xFF 

// new nand flash command opcodes
#define k_nand_cmd_read_for_data      		0x30
#define k_nand_cmd_read_for_copy      		0x35
#define k_nand_cmd_read_cache_next    		0x31  /* toshiba only */
#define k_nand_cmd_read_cache_final   		0x3f  /* toshiba only */
#define k_nand_cmd_read_offset_addr   		0x05
#define k_nand_cmd_read_offset_data   		0xE0
#define k_nand_cmd_write_cache_data   		0x85
#define k_nand_cmd_write_cache        			0x15
#define k_nand_cmd_write_page         			k_nand_write
#define k_nand_cmd_read_id            			0x90 
#define k_nand_cmd_read_id_ex         			0x91


/* SmartMedia Status */
#define kbm_nand_status_write_failed    		0x01  /* 0:Pass, 1:Fail */
#define kbm_nand_status_suspended       		0x20  /* 0:Not Suspended, 1:Suspended */
#define kbm_nand_status_ready           		0x40  /* 0:Busy, 1:Ready */
#define kbm_nand_status_n_wr_protect    		0x80  /* 0:Protect, 1:Not Protect */

/* SmartMedia Busy Time (1bit:0.1ms) */
#define k_nand_busy_programming_timeout 	21  /* tPROG : 20ms ----- Program Time */
#define k_nand_busy_erase_timeout       		400 /* tBERASE: 400ms ----- Block Erase Time */
#define k_nand_busy_read_timeout        		2   /* tR : 100us ----- Data transfer Time */
#define k_nand_busy_reset_timeout       		7   /* tRST : 6ms ----- Device Resetting Time */
// new nand flash timeouts
#define k_nand_write_page_timeout     		2   /* 700 usecs max */


/* Hardware Timer (1bit:0.1ms) */
#define k_nand_busy_power_up_timeout    	301 // 300ms ------ Power On Wait Time TIME_PON*/
#define k_nand_card_check_poll_interval 		21  // 20ms  ------ Card Check Interval Timer CDCHK */
#define k_nand_time_wp_poll_interval    		6   // 5ms  ------ WP Check Interval Timer */








//----------------------------------------------------------------
//samsung 's nand using 0x3000,maybe ok
//hynix 's using 0x3100 may be ok
//#define nand_enable_controller() 		{set_nftcr_val(0x3000);set_nfcr_nfe();}
#define nand_enable_controller() 		{set_nftcr_val(0x3100);set_nfcr_nfe();}
//#define nand_enable_controller() 		{set_nfcr_nfe();}
#define nand_disable_controller()		{clr_nfcr_nfe();}
#define nand_enable_ecc() 			{set_nfcr_ecce();}
#define nand_disable_ecc() 			{clr_nfcr_ecce();}
#define nand_reset_ecc() 				{set_nfcr_erst();}

//=================================================
#define nand_set_cmd(cmd)				{set_nfcr_nfce();i_nop;i_nop;set_nfcmd_val(cmd);}
#define nand_set_cmd_ex(cmd)			{set_nfcmd_val(cmd);}
#define nand_set_standby()				{clr_nfcr_nfce();}
#define nand_set_addr(addr)				{set_nfaddr_val(addr);}
#define nand_get_data()					(REG8( NFDATA ))
//#define nand_is_ready()					((REG8( NFSR))&NFSR_RB)
#define nand_is_ready()					(get_nfsr_rb())
#define nand_set_data(val)				{set_nfdata_val(val);}
//#define nand_enable_controller() 			{set_nfcr_nfe();}
//#define nand_disable_controller()			{clr_nfcr_nfe();}

#define nand_get_ecc()					(get_nfecc())
#define udc_set_reg(addr, val) (i_s32((val), (addr), 0))
#define 	CheckWriteProtectInWriting()		{\
				udisk_check_protect();\
				if( nand_media_sts & nand_media_sts_wrprot )\
					{\
					sensePtr = senseWriteProtected;\
					return K_ERROR;\
					}\
				}
//----------------------------------------------------------------
extern void nand_soft_reset(void);
extern void nand_reset_device(void);
extern void nand_read_id(unsigned char * id_list);
extern unsigned int nand_wait_rdy_with_timeout(unsigned int ticks);
extern unsigned int nand_read_extra_data(void);
extern unsigned int nand_read_extra_data_test(unsigned int plba,unsigned int count);
extern unsigned int nand_read_sector(void);
extern unsigned int nand_write_sector_test(unsigned int plba,unsigned int count);
extern unsigned int nand_write_extra_data(void);
extern unsigned int nand_write_extra_data_to_set_bad_block(void);
extern unsigned int nand_write_sector(void);
extern unsigned int nand_write_sector_test(unsigned int plba,unsigned int count);
extern unsigned int nand_check_data_status_valid(unsigned char page, unsigned char count);
extern unsigned int nand_erase_block(void);
extern unsigned int nand_erase_block_test(unsigned int pb,unsigned int count);
//----------------------------------------------------------------
extern unsigned int nand_read_begin_xfer(unsigned int * udisk_lba_param);
extern unsigned int nand_read_end_xfer(unsigned int * udisk_lba_param);
extern unsigned int nand_read_begin_split(unsigned int * udisk_lba_param);
extern unsigned int nand_read_end_split_first(unsigned int * udisk_lba_param);
extern unsigned int nand_read_end_split(unsigned int * udisk_lba_param);
extern unsigned int nand_write_begin_xfer(unsigned int * udisk_lba_param);
extern unsigned int nand_write_end_xfer(unsigned int * udisk_lba_param);
extern unsigned int nand_write_begin_split(unsigned int * udisk_lba_param);
extern unsigned int nand_write_end_split(unsigned int * udisk_lba_param);


//---------------------------------------------------------------------------------
#ifndef _NAND_C_

#endif
//---------------------------------------------------------------------------------

#endif


