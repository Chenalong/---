/*
  Name: 

*/
#define _UMS_C_

#include <instructions.h>
#include <bsp.h>
#include <types.h>
#include <ukey_proto.h>
#include "ums.h"
#include "udisk.h"
#include "nand_media.h"
#include "usb_bulk.h"
#include "udiskdes.h"
#define DEBUG  0
unsigned char sensePtr;
//extern udisk_rd_wr_self_enable;
const UCHAR senseArray[senseWriteProtected+1][3] = 
{{0x0b, 0x08, 0x03},   // senseCRCError               0 // Set on CRC error.  Causes host to retry  
{0x05, 0x24, 0x00},    // senseInvalidFieldInCDB      1// 300 calls this InvalidCommandField   
{0x00, 0x00, 0x00},    // senseOk                     2
{0x02, 0x3a, 0x00},    // senseNoMedia                3
{0x03, 0x03, 0x00},    // senseWriteFault             4
{0x03, 0x11, 0x00},    // senseReadError              5
{0x03, 0x12, 0x00},    // senseAddrNotFound           6
{0x05, 0x20, 0x00},    // senseInvalidOpcode          7
{0x05, 0x21, 0x00},    // senseInvalidLBA             8
{0x05, 0x26, 0x00},    // senseInvalidParameter       9
{0x05, 0x53, 0x02},    // senseCantEject              0xa
{0x06, 0x28, 0x00},    // senseMediaChanged           0xb
{0x06, 0x29, 0x00},    // senseDeviceReset            0xc// Initial value.  Set in ATARESET.c
{0x07, 0x27, 0x00}};   // senseWriteProtected         0xd

//========================================
unsigned char  ms_out_command(CBW * pcbw,unsigned int *udisk_lba_param);
unsigned char  ms_in_command(CBW * pcbw,unsigned int *udisk_lba_param);
void ums_csw_send(unsigned char csw_result,CBW * pcbw);
extern void usb_bulk_in_stall_token();
extern void usb_bulk_out_stall_token();


//wait a cbw coming
unsigned int ums_cbw_probe(CBW * pcbw)
{
	if(usb_probe_ep2_out_recieve((unsigned char *)(pcbw)))
		{
		if(check_cbw_valid(pcbw))
			{
			return TRUE;
			}
		}
	return FALSE;
}
//check media status,such as no media, media change
void media_check(unsigned char lun_no)
{
	if(lun_low_status[pudisk_lun_low_ex[lun_no]] & mask_lun_status_change)
			{
			sensePtr = senseMediaChanged;
			lun_low_status[pudisk_lun_low_ex[lun_no]] &= (~ mask_lun_status_change);
			}
	else if(!(lun_low_status[pudisk_lun_low_ex[lun_no]]& mask_lun_status_ready))
			{
			sensePtr = senseNoMedia;
			}
}
//check lun protect status
unsigned int ums_check_lun_protect(unsigned char lun)
{
	udisk_check_protect();
	if( nand_media_sts & nand_media_sts_wrprot )
		return TRUE;
	if(lun_low_status[pudisk_lun_low_ex[lun]] & mask_lun_status_protect )
		return TRUE;
	return FALSE;
}
//check cbw 's signature
unsigned int check_cbw_valid(CBW * pcbw)
{
	if(pcbw->dwCBWSignature == CBW_SIGNATURE)
		return TRUE;
	return FALSE;
}
//check fro security lun
void ums_check_security(unsigned char lun,unsigned int lba_start,unsigned char write_flag)
{
	static unsigned char refresh_disk_data_start_lba = TRUE;
	static unsigned int disk_data_start_lba;

	lun_security_enable = FALSE;
	if((write_flag == TRUE) || (write_flag == FALSE))
	if(pudisk_lun_low_ex[lun] == 0)
		{
		if(!(lun_low_status[0] & mask_lun_status_protect))
			{
			//
			if(TRUE == refresh_disk_data_start_lba)
				{
				//get disk_data_start_lba
				unsigned int udisk_lba_param[index_udisk_lba_parameter_all];
	  			udisk_lba_param[index_udisk_lba_start] = 0;
	  			udisk_lba_param[index_udisk_lba_xfer_count] = 1;
				//
				udisk_rd_wr_self_enable = TRUE;
				udisk_lba_param[index_udisk_lba_start] += lun_low_lba_start_ex[lun_low_status[0]&mask_lun_status_no];
				//
	  			if( K_SUCCES != udisk_read(udisk_lba_param))
         				{
         				disk_data_start_lba = 0xFFFFFFFF;
         				}
				else
					{
					//
					unsigned int reserved_sector_count;
					unsigned int fat_sector_count;
                			unsigned int root_dir_sector_count;
					reserved_sector_count = (nand_page_buff[14] + (nand_page_buff[15]  << 8));
					fat_sector_count = (nand_page_buff[22] + (nand_page_buff[23]  << 8));
					root_dir_sector_count = ((nand_page_buff[17] + (nand_page_buff[18]  << 8)) * 32 /512);
					disk_data_start_lba = reserved_sector_count + fat_sector_count*2 + root_dir_sector_count;
					//
					refresh_disk_data_start_lba = FALSE;
					}
				udisk_rd_wr_self_enable = FALSE;
				}
			//
			if((TRUE == write_flag) && (0 == lba_start))
				{
				refresh_disk_data_start_lba = TRUE;
				}
			else if(lba_start >= disk_data_start_lba)
				{
				lun_security_enable = TRUE;
				return;
				}
			}
		}
	else if(write_flag == 0xFF)
		{
		refresh_disk_data_start_lba = TRUE;
		}
	return;
}
//cbw processsing
unsigned int process_cbw(CBW * pcbw)
{
	unsigned int buff_nand_page[nand_page_size/4];
	unsigned int buff_nand_redt[nand_redt_buffer_size/4];
	//int i;
	nand_page_buff = (unsigned char *)buff_nand_page;
	nand_redt_data = (unsigned char *)buff_nand_redt;
	///
	unsigned int udisk_lba_param[index_udisk_lba_parameter_all];

#if DEBUG 
printf("Opcode = 0x%02x, lun = %d, \n",pcbw->cdb[0],pcbw->bCBWlun & 0x0F);		
#endif	
	//vendor command processing
	if(pcbw->cdb[0] >= CDB_ZTEIC_OPCODE_MIN)
		{
		sensePtr = senseOk;
			{
			unsigned char Result = USBS_PASSED;
			if(pcbw->cdb[0] == VENDOROP_ZTEIC) //pcbw->cdb[0]=0xff???
				{
				if((pcbw->cdb[1] == VENDOROP_ZTEIC_CHECK_0)&&
					(pcbw->cdb[2] == VENDOROP_ZTEIC_CHECK_1)&&
					(pcbw->cdb[3] == VENDOROP_ZTEIC_CHECK_2)&&
					(pcbw->cdb[4] == VENDOROP_ZTEIC_CHECK_3)&&
					(pcbw->cdb[5] == VENDOROP_ZTEIC_CHECK_4))
					{
#if DEBUG
printf("zteic 's vendor command = 0x%02x, \n",pcbw->cdb[9]);		
#endif						
					//zteic 's vendor command
						if(pcbw->cdb[9]  == VENDOROP_ZTEIC_USB_KEY)
							{//usb key 和key相关的传输
							if(VENDOROP_ZTEIC_USB_KEY_KCW == pcbw->cdb[6])
								{
								//kcw data
								
								return;
								}
							else
								{
								sensePtr = senseInvalidOpcode;
								}
							}
						//-----------------------------------------------
						else if(pcbw->cdb[9]  == VENDOROP_ZTEIC_GET_CAPACITY_EX)
							{
							//read out parameter 获得当前u盘的各个LUN的容量信息
							udisk_read_parameter(nand_page_buff);
							pcbw->dwCBWXferLength -= VENDOROP_ZTEIC_GET_CAPACITY_EX_DATALEN;
							usb_send(nand_page_buff + PARAMETER_FLASH_DISK_OFFSET,VENDOROP_ZTEIC_GET_CAPACITY_EX_DATALEN);
							//printf("VENDOROP_ZTEIC_GET_CAPACITY_EX_DATALEN=%x\n",VENDOROP_ZTEIC_GET_CAPACITY_EX_DATALEN);
							}
						else if(pcbw->cdb[9]  == VENDOROP_ZTEIC_SET_CAPACITY_EX)
							{
							//UCHAR capacity_ex[VENDOROP_ZTEIC_SET_CAPACITY_EX_DATALEN];
							#define capacity_ex		(nand_page_buff + 0x180)
							usb_receive(capacity_ex, VENDOROP_ZTEIC_SET_CAPACITY_EX_DATALEN);
							pcbw->dwCBWXferLength -= VENDOROP_ZTEIC_SET_CAPACITY_EX_DATALEN;
							udisk_read_parameter(nand_page_buff);
							memcpy(nand_page_buff + PARAMETER_FLASH_DISK_OFFSET+8,capacity_ex+8,VENDOROP_ZTEIC_SET_CAPACITY_EX_DATALEN-8);
							udisk_write_parameter(nand_page_buff);
							udisk_get_luns_lba_start();
							#undef capacity_ex
							}
						else if(pcbw->cdb[9]  == VENDOROP_ZTEIC_SET_LUN_NOT_READY)
							{
							lun_low_status[0] &= (~mask_lun_status_ready);
							//set it and then password need
							lun_low_status[0] |= (mask_lun_status_protect);
							//
							lun_low_status[1] &= (~mask_lun_status_ready);
							lun_low_status[2] &= (~mask_lun_status_ready);
							//before do format
							//set  refresh_disk_data_start_lba= TRUE in ums_check_security()
							ums_check_security(0,0,0xFF);
							//clear security disk's password
							udisk_read_parameter(nand_page_buff);
							memset(nand_page_buff +PARAMETER_SECURITY_DISK_PWD_OFFSET,0,VENDOROP_ZTEIC_SET_PASSWORD_DATALEN);
							udisk_write_parameter(nand_page_buff);
							//
							}
						else if(pcbw->cdb[9]  == VENDOROP_ZTEIC_FORMAT)
							{							
							buff_nand_page[0] = Get_Rand();
							buff_nand_page[1] = Get_Rand();
							WriteFlash(nand_page_buff,0x3f000+PARAMETER_SECURITY_DISK_PWD_OFFSET+PARAMETER_SECURITY_DISK_PWD_LEN, 8);
#if DEBUG
printf("set DES key, 0x%08x, 0x%08x,\n",buff_nand_page[0],buff_nand_page[1]);
#endif
                                                 set_des_key(buff_nand_page[0],buff_nand_page[1]);
							} 
						else if(pcbw->cdb[9]  == VENDOROP_ZTEIC_SET_LUN_READY)
							{
							udisk_get_luns_lba_start();
							//lun read file name enable before password is set
							if((lun_low_lba_start_ex[1] - lun_low_lba_start_ex[0]) >0)
								{
								lun_low_status[0] |= (mask_lun_status_ready | mask_lun_status_change);
								}
							//
							if((lun_low_lba_start_ex[2] - lun_low_lba_start_ex[1]) >0)
								{
								lun_low_status[1] |= (mask_lun_status_ready | mask_lun_status_change);
								}
							if((lun_low_lba_start_ex[3] - lun_low_lba_start_ex[2]) >0)
								{
								lun_low_status[2] |= (mask_lun_status_ready | mask_lun_status_change);
								}
							//udisk_read_parameter(nand_page_buff);

							usb_receive(nand_page_buff, VENDOROP_ZTEIC_SET_PASSWORD_DATALEN+1);
#if DEBUG
print_buf(nand_page_buff,64+1);
#endif
							if(nand_page_buff[64])
								{														
							WriteFlash(nand_page_buff,0x3f000+PARAMETER_SECURITY_DISK_PWD_OFFSET, VENDOROP_ZTEIC_SET_PASSWORD_DATALEN);
#if DEBUG
udisk_read_parameter(nand_page_buff);
print_buf(nand_page_buff+PARAMETER_SECURITY_DISK_PWD_OFFSET,64+8);
#endif
							#if 0
							USB_PULLUP_GPIO()
							USB_PULLUP_OUT()
							USB_PULLUP_LOW()
							delay_xxx(30000000);	
							USB_PULLUP_HIGH()
							#endif
								}
#ifdef DISK_MAX_THREE_ICON							
#else
//change the ptr from phisical lun3 to physical lun 0
								pudisk_lun_low_ex[0] = 2;	//first using phisical lun0 as logical lun0;
#endif							
							}
						else if(pcbw->cdb[9]  == VENDOROP_ZTEIC_SET_PASSWORD)
							{
							//this command should appear when security disk is been
							//UCHAR password[VENDOROP_ZTEIC_SET_PASSWORD_DATALEN];
							#define password		(nand_page_buff + 0x180+0x20)
							usb_receive(password, VENDOROP_ZTEIC_SET_PASSWORD_DATALEN);
#if DEBUG							
print_buf(password,64);
#endif
							pcbw->dwCBWXferLength -= VENDOROP_ZTEIC_SET_PASSWORD_DATALEN;
							//compare with pwd saved in chip
							udisk_read_parameter(nand_page_buff);
#if DEBUG							
print_buf(nand_page_buff +PARAMETER_SECURITY_DISK_PWD_OFFSET,64);
#endif
							if(memcmp(password,nand_page_buff +PARAMETER_SECURITY_DISK_PWD_OFFSET,VENDOROP_ZTEIC_SET_PASSWORD_DATALEN))
								{
								lun_low_status[3]  |=  (mask_lun_status_pwd_error);
								}
							else
								{
								ReadFlash(buff_nand_page,0x3f000+PARAMETER_SECURITY_DISK_PWD_OFFSET+PARAMETER_SECURITY_DISK_PWD_LEN, 8);
								set_des_key(buff_nand_page[0],buff_nand_page[1]);
								lun_low_status[3]  &=  (~mask_lun_status_pwd_error);
								if((lun_low_lba_start_ex[1] - lun_low_lba_start_ex[0]) >0)
									{
									if(lun_low_status[0] & mask_lun_status_enable)
										{
										//lun_low_status[0] |= (mask_lun_status_ready | mask_lun_status_change);
										lun_low_status[0] &= (~mask_lun_status_protect);
										lun_low_status[0] |= (mask_lun_status_change);
#ifdef DISK_MAX_THREE_ICON
//do nothing
#else
//change the ptr from phisical lun3 to physical lun 0
								pudisk_lun_low_ex[0] = 0;	//first using phisical lun0 as logical lun0;
#endif									
										}
			#if DEBUG	//
printf("lun_num = %d, lun_low_status[0]  = 0x%02x, \n[1]  = 0x%02x, [2]  = 0x%02x, [3]  = 0x%02x, \n",lun_num,lun_low_status[0],lun_low_status[1],lun_low_status[2],lun_low_status[3]);		
printf("pudisk_lun_low_ex[0]  = 0x%02x, [1]  = 0x%02x, \n[2]  = 0x%02x, [3]  = 0x%02x, \n",pudisk_lun_low_ex[0],pudisk_lun_low_ex[1],pudisk_lun_low_ex[2],pudisk_lun_low_ex[3]);		
#endif						
									}

								}
							//
							#undef password
							}
						else if(pcbw->cdb[9]  == VENDOROP_ZTEIC_CHANGE_PASSWORD)
							{
							//this command should appear when security disk is been
							//UCHAR password[VENDOROP_ZTEIC_CHANGE_PASSWORD_DATALEN];							
							#define password		(nand_page_buff + 0x180+0x20)
							usb_receive(password, VENDOROP_ZTEIC_CHANGE_PASSWORD_DATALEN);
							pcbw->dwCBWXferLength -= VENDOROP_ZTEIC_CHANGE_PASSWORD_DATALEN;
							//compare with pwd saved in chip
							udisk_read_parameter(nand_page_buff);
							if(0 == memcmp(password,nand_page_buff +PARAMETER_SECURITY_DISK_PWD_OFFSET,VENDOROP_ZTEIC_SET_PASSWORD_DATALEN))
								{
								//write password;
								lun_low_status[3]  &=  (~mask_lun_status_pwd_error);
								memcpy(nand_page_buff +PARAMETER_SECURITY_DISK_PWD_OFFSET,password + VENDOROP_ZTEIC_SET_PASSWORD_DATALEN, VENDOROP_ZTEIC_SET_PASSWORD_DATALEN);
								udisk_write_parameter(nand_page_buff);
								}
							else
								{
								lun_low_status[3]  |=  (mask_lun_status_pwd_error);
								}
							#undef password	
							}
						else if(pcbw->cdb[9]  == VENDOROP_ZTEIC_GET_PASSWORD_RESULT)
							{
							//unsigned char result = 0;
							nand_page_buff[0] = 0;
							if(lun_low_status[3] & mask_lun_status_pwd_error)
								{
								//result = 1;
								nand_page_buff[0] = 1;
								}
							//usb_send(&result , 1);
							usb_send(nand_page_buff, 1);
							pcbw->dwCBWXferLength -= 1;
							}
						else if(pcbw->cdb[9]  == VENDOROP_ZTEIC_GET_PROTECTSTS)
							{
							udisk_check_protect();
							if( nand_media_sts & nand_media_sts_wrprot )
								{
								nand_page_buff[0] = VENDOROP_ZTEIC_GET_PROTECTED;
								}
							else
								{
								nand_page_buff[0] = VENDOROP_ZTEIC_GET_UNPROTECTED;
								}
							if(lun_low_status[3] & mask_lun_status_pwd_error)
								{
								nand_page_buff[0] = 1;
								}
							usb_send(nand_page_buff, 1);
							pcbw->dwCBWXferLength -= 1;
							}
						else
							{//error vendor command
							Result = USBS_FAILED;
							}
						//exist if command ok
						if(Result == USBS_PASSED)
							{
							ums_csw_send(USBS_PASSED,pcbw);
							return ;
							}
					}
				}
			else
				{
				//
				restore_udisk_log2phy_buff();
				//
				if((pcbw->cdb[1] == VENDOROP_ZTEIC_RW_LBA_FLAG_0)&&
					(pcbw->cdb[6] == VENDOROP_ZTEIC_RW_LBA_FLAG_1)&&
					(pcbw->cdb[9] == VENDOROP_ZTEIC_RW_LBA_FLAG_2))
					{				
					//zteic read total udisk's lba
					if(pcbw->cdb[0] == VENDOROP_ZTEIC_READ_LBA)
						{
						udisk_lba_param[index_udisk_lba_start] = (pcbw->cdb[2]<<24) + (pcbw->cdb[3]<<16) +(pcbw->cdb[4]<<8) +pcbw->cdb[5];
	  					udisk_lba_param[index_udisk_lba_xfer_count] = pcbw->cdb[8];
						udisk_lba_param[index_udisk_lba_start] += lun_low_lba_start_ex[pcbw->cdb[7]];
						//udisk_read(udisk_lba_param);
#if DEBUG
printf("R nand_addr_start = 0x%08x\n",udisk_lba_param[index_udisk_lba_start]);
printf("nand_read_segment_num = 0x%08x\n\n",udisk_lba_param[index_udisk_lba_xfer_count]);
#endif					
						lun_security_enable = FALSE;
							if(udisk_lba_param[index_udisk_lba_start]<lun_low_lba_start_ex[1])
						lun_security_enable = TRUE;
						if(nand_media_sts&0x40)									
							udisk_read_2k(udisk_lba_param);						
						else
							udisk_read(udisk_lba_param);
						pcbw->dwCBWXferLength = (udisk_lba_param[index_udisk_lba_xfer_count] <<9);
						}
					//zteic write total udisk's lba
					else if(pcbw->cdb[0] == VENDOROP_ZTEIC_WRITE_LBA)
						{
						//unsigned short i;
						//unsigned char* nand_assign_map,*nand_log2phy_map;
						udisk_lba_param[index_udisk_lba_start]  = (pcbw->cdb[2]<<24) + (pcbw->cdb[3]<<16) +(pcbw->cdb[4]<<8) +pcbw->cdb[5];
	  					udisk_lba_param[index_udisk_lba_xfer_count] = pcbw->cdb[8];
						udisk_lba_param[index_udisk_lba_start] += lun_low_lba_start_ex[pcbw->cdb[7]];
#if DEBUG
printf("W nand_addr_start = 0x%08x\n",udisk_lba_param[index_udisk_lba_start]);
printf("nand_write_segment_num = 0x%08x\n\n",udisk_lba_param[index_udisk_lba_xfer_count]);
#endif
						lun_security_enable = FALSE;
							if(udisk_lba_param[index_udisk_lba_start]<lun_low_lba_start_ex[1])
						lun_security_enable = TRUE;
                                          lun_num |= ((pcbw->bCBWlun & 0x0F)<<4);
						udisk_write(udisk_lba_param);
						lun_num &=0xf;
						//printf("udisk_write ok!!!");
						//udisk_read(udisk_lba_param);
						//printf("udisk_write ok!!!");
						pcbw->dwCBWXferLength = (udisk_lba_param[index_udisk_lba_xfer_count] <<9);
						//printf("Zone = %x ",nand_addr_zone);
						//nand_log2phy_map = (nand_addr_zone ? (nand_log2phy_map_buff + buff_nand_log2phy_map_one_zone_size) : nand_log2phy_map_buff);	
						//nand_assign_map = (nand_addr_zone ? (nand_assign_map_buff + nand_assign_map_one_zone_size) : nand_assign_map_buff);
						/*
						for (i=0;i<1536;i++)
							{
							printf("%x = %x ",i,nand_log2phy_map[i]);
							if((i&0x0f)==0x0f)
								printf("\n");
							}
						for(i=0;i<128;i++)
							{
							printf("%x = %x ",i,nand_assign_map[i]);
							if((i&0x0f)==0x0f)
								printf("\n");
							}*/
						
						}
					else
						{
						Result = USBS_FAILED;
						}
					//exist if command ok
					if(Result == USBS_PASSED)
						{
						//printf("send result  ok!!!\n");
						//printf("%x\n",Result);
						ums_csw_send(USBS_PASSED,pcbw);
						//printf("send result  ok!!!\n");
						//printf("%x\n",Result);
						return ;
						}
					}
				}
			sensePtr = senseInvalidOpcode;
			}
		if(pcbw->dwCBWXferLength)
			{
	  		usb_bulk_in_stall_token();
			}
		ums_csw_send(USBS_FAILED,pcbw);
		return;
		}
	//=====================================================
	//turn on LED
	//udisk_led(LED_ON);
	//here is the standart SCSI command processing
	if((pcbw->bCBWFlags & 0x80) || (!pcbw->dwCBWXferLength))
		{
		ums_csw_send(ms_in_command(pcbw,udisk_lba_param),pcbw);
		}
	else
		{
		ums_csw_send(ms_out_command(pcbw,udisk_lba_param),pcbw);
		}
	//turn off LED
	//udisk_led(LED_OFF);
}
//send csw to host
void ums_csw_send(unsigned char csw_result,CBW * pcbw)
{
	CSW csw;
	csw.dwCSWSignature = CSW_SIGNATURE;
	csw.dwCSWtag = pcbw->dwCBWtag;
	csw.dwCSWResidue = pcbw->dwCBWXferLength;
	csw.bCSWStatus = csw_result;
	usb_send((unsigned char *)(&csw),13);
	
}
//scsi command with out data  transfer
#define  lun_cbw  (pcbw->bCBWlun & 0x0F)
unsigned char ms_out_command(CBW * pcbw,unsigned int *udisk_lba_param)
{
	//unsigned char lun_cbw = (pcbw->bCBWlun & 0x0F);
	sensePtr = senseOk;
	switch (pcbw->cdb[0])
		{
		case WRITE_06:
		case WRITE_10:
		case WRITE_AND_VERIFY_10:
	  		{
			media_check(lun_cbw);
			if(sensePtr == senseOk)
				{
				if( TRUE == ums_check_lun_protect(lun_cbw) )
					{
					sensePtr = senseWriteProtected;
					//printf("write protected----\n");
					}
				else
					{
					//
					restore_udisk_log2phy_buff();
					//
					if(pcbw->cdb[0] == WRITE_06)
						{
						udisk_lba_param[index_udisk_lba_start] = (pcbw->cdb[2]<<8) +pcbw->cdb[3];
						udisk_lba_param[index_udisk_lba_xfer_count] = pcbw->cdb[4];
						}
					else
						{
						udisk_lba_param[index_udisk_lba_start] = (pcbw->cdb[2]<<24) + (pcbw->cdb[3]<<16) +(pcbw->cdb[4]<<8) +pcbw->cdb[5];
						udisk_lba_param[index_udisk_lba_xfer_count] = (pcbw->cdb[7]<<8) +pcbw->cdb[8];
						}
					//----------------------
					//for key lun, because status always is writing protecting, so writing operation on key lun is always forbiden
					//if(lun_cbw == (lun_num-1))
					//	{
					//	while(udisk_lba_param[index_udisk_lba_xfer_count] )
					//		{
					//		usb_receive(nand_page_buff,512);
					//		udisk_lba_param[index_udisk_lba_xfer_count]--;
					//		}
					//	pcbw->dwCBWXferLength = 0;
					//	}
					//else
					//----------------------
						{
						//for security lun
						//ums_check_security(lun_cbw,udisk_lba_param[index_udisk_lba_start],TRUE);
						//
#if DEBUG
printf("W nand_addr_start = 0x%08x\n",udisk_lba_param[index_udisk_lba_start]);
printf("nand_write_segment_num = 0x%08x\n",udisk_lba_param[index_udisk_lba_xfer_count]);
#endif						
						udisk_lba_param[index_udisk_lba_start] += lun_low_lba_start_ex[(lun_low_status[pudisk_lun_low_ex[lun_cbw]]&mask_lun_status_no)];
						lun_security_enable = FALSE;
						if(udisk_lba_param[index_udisk_lba_start]<lun_low_lba_start_ex[1])
							lun_security_enable = TRUE;
						//
						lun_num |= ((pcbw->bCBWlun & 0x0F)<<4);
#if DEBUG
printf("nand_addr_start = 0x%08x, lun_num = %d, \n\n",udisk_lba_param[index_udisk_lba_start], lun_num);
#endif
						if( K_SUCCES != udisk_write(udisk_lba_param))
							{
							printf("udisk write error !!!!\n");
							if(senseWriteProtected != sensePtr)
								{
								sensePtr = senseWriteFault;
								}
							}
						lun_num &=0xf;
						//printf("%x\n ",udisk_lba_param[index_udisk_lba_xfer_count]);
						pcbw->dwCBWXferLength = (udisk_lba_param[index_udisk_lba_xfer_count] <<9);
						}
					}
				}
			if(pcbw->dwCBWXferLength)
				usb_bulk_out_stall_token();
			//printf("senseptr = %x\n",sensePtr);
			if(sensePtr == senseOk)
				return(USBS_PASSED);
			else
				return(USBS_FAILED);
			}                 
		case MODE_SELECT_06:
		case MODE_SELECT_10:
			{
			//unsigned char EpBuf[64];
			//usb_receive(EpBuf , pcbw->dwCBWXferLength);
	  		usb_receive(nand_page_buff , pcbw->dwCBWXferLength);
			pcbw->dwCBWXferLength = 0;
         		return(USBS_PASSED);
      			}                
		default:    
			sensePtr = senseInvalidOpcode;
			if(pcbw->dwCBWXferLength)
	  			usb_bulk_out_stall_token();
			return(USBS_FAILED);
		}
}
//scsi command without data transfer or with in data transfer
unsigned char  ms_in_command(CBW * pcbw,unsigned int *udisk_lba_param)
{
	//unsigned char lun_cbw = (pcbw->bCBWlun & 0x0F);
	unsigned int sts;
	switch (pcbw->cdb[0])
		{
		case INQUIRY:
			{
			BYTE i;
	  		//UCHAR inquiry_data[36];
	  		unsigned char buff[8] = {'A','T','O','M',' ',0x20,0x20,0x20};
	  		#define inquiry_data  (nand_page_buff + 0x180)
			UCHAR * lun_inquiry_info;
         		sensePtr = senseOk;
			//read out parameter
			udisk_read_parameter(nand_page_buff);
			//udisk_check_parameter(nand_page_buff);
		//print_buf(nand_page_buff,0x180);	
			//
			memset(inquiry_data,0,36);
			if(lun_cbw != (lun_num-1))
				{//for key set it as local disk, other set as removable disk
				inquiry_data[SCSI_INQUIRY_REMOVABLE_BYTE] |= SCSI_INQUIRY_REMOVABLE_BIT;
				}
			lun_inquiry_info = nand_page_buff +PARAMETER_INQUIRY_DATA_OFFSET
				+(lun_low_status[pudisk_lun_low_ex[lun_cbw]]& mask_lun_status_no)*(PARAMETER_INQUIRY_DATA_LUN_VENDOR_STRING_LEN +PARAMETER_INQUIRY_DATA_LUN_PRODUCT_STRING_LEN + PARAMETER_INQUIRY_DATA_LUN_VERSION_STRING_LEN);
			//
			memcpy(inquiry_data + SCSI_INQUIRY_VENDOR ,buff,PARAMETER_INQUIRY_DATA_LUN_VENDOR_STRING_LEN);
			memcpy(inquiry_data + SCSI_INQUIRY_PRODUCT ,lun_inquiry_info + PARAMETER_INQUIRY_DATA_LUN_VENDOR_STRING_LEN,PARAMETER_INQUIRY_DATA_LUN_PRODUCT_STRING_LEN );
			memcpy(inquiry_data + SCSI_INQUIRY_REVISION ,lun_inquiry_info + PARAMETER_INQUIRY_DATA_LUN_VENDOR_STRING_LEN +PARAMETER_INQUIRY_DATA_LUN_PRODUCT_STRING_LEN,PARAMETER_INQUIRY_DATA_LUN_VERSION_STRING_LEN );
			//
		//print_buf(inquiry_data,36);
			if(pcbw->dwCBWXferLength < 36)
				{
				usb_send(inquiry_data,pcbw->dwCBWXferLength);
				pcbw->dwCBWXferLength = 0;
				}
			else
				{
				usb_send(inquiry_data,36);
				pcbw->dwCBWXferLength -= 36;
				}
	  		if(pcbw->dwCBWXferLength)
	  			usb_bulk_in_stall_token();
         		return(USBS_PASSED);
			#undef  inquiry_data
      			}
      		case READ_06:
      		case READ_10:
      			{
			sensePtr = senseOk;
			media_check(lun_cbw);
			if(sensePtr == senseOk)
				{
	  			if(pcbw->cdb[0] == READ_06)
	  				{
	  				udisk_lba_param[index_udisk_lba_start] = (pcbw->cdb[2]<<8) +pcbw->cdb[3];
	  				udisk_lba_param[index_udisk_lba_xfer_count] = pcbw->cdb[4];
	  				}
	  			else
	  				{
	  				udisk_lba_param[index_udisk_lba_start] = (pcbw->cdb[2]<<24) + (pcbw->cdb[3]<<16) +(pcbw->cdb[4]<<8) +pcbw->cdb[5];
	  				udisk_lba_param[index_udisk_lba_xfer_count] = (pcbw->cdb[7]<<8) +pcbw->cdb[8];
	  				}
				//----------------------
				//for key lun
				if(lun_cbw == (lun_num-1))
					{
					memset(nand_page_buff , 0xFF , 512);
					while(udisk_lba_param[index_udisk_lba_xfer_count])
						{
						usb_send(nand_page_buff,512);
						udisk_lba_param[index_udisk_lba_xfer_count]--;
						}
					}
				else
				//----------------------
					{
					//
					restore_udisk_log2phy_buff();
#if DEBUG
printf("R nand_addr_start = 0x%08x\n",udisk_lba_param[index_udisk_lba_start]);
printf("nand_read_segment_num = 0x%08x\n",udisk_lba_param[index_udisk_lba_xfer_count]);
printf("R nand_flash_lun = 0x%08x\n",lun_cbw);
printf("lun_low_lba_start_ex[1] = 0x%08x\n",lun_low_lba_start_ex[1]);
#endif					
					//
					//for security lun
					//ums_check_security(lun_cbw,udisk_lba_param[index_udisk_lba_start],FALSE);
					//
					udisk_lba_param[index_udisk_lba_start] += lun_low_lba_start_ex[lun_low_status[pudisk_lun_low_ex[lun_cbw]]&mask_lun_status_no];
					lun_security_enable = FALSE;
					if(udisk_lba_param[index_udisk_lba_start]<lun_low_lba_start_ex[1])
						lun_security_enable = TRUE;
					//
					#if 1
#if DEBUG
printf("lun_security_enable=%x\n",lun_security_enable);
printf("nand_addr_start = 0x%08x\n\n",udisk_lba_param[index_udisk_lba_start]);
#endif					
					if(nand_media_sts&0x40)
						{
						if((0==lun_cbw)&&(lun_low_status[0] & mask_lun_status_protect))
							{
							while(udisk_lba_param[index_udisk_lba_xfer_count])
							{
								usb_send(nand_page_buff,512);
								udisk_lba_param[index_udisk_lba_xfer_count]--;
							}
							sts = K_ERROR;
							}
						else
							sts = udisk_read_2k(udisk_lba_param);
						}
					else
						sts = udisk_read(udisk_lba_param);
					#else
					sts = udisk_read(udisk_lba_param);
					#endif
		  			if( K_SUCCES != sts)
	         				{
	         				sensePtr = senseReadError;
	         				}
					
					}
				pcbw->dwCBWXferLength = (udisk_lba_param[index_udisk_lba_xfer_count] <<9);
				}
         		if(pcbw->dwCBWXferLength)
				usb_bulk_in_stall_token();
			if(sensePtr == senseOk)
		 		return(USBS_PASSED);
	 		else
            	 		return(USBS_FAILED); 
      			} 
      		case READ_FORMAT_CAPACITIES:
     		case READ_CAPACITY:
      			{ 
	  		//unsigned char capacity[8];
	  		#define capacity nand_page_buff
	  		sensePtr = senseOk;
			media_check(lun_cbw);
			if((sensePtr == senseOk)|(sensePtr == senseMediaChanged))
				{
				//
				//printf("<<lun_num : %x  lun_cbw: %x\n",lun_num,lun_cbw);
				if(lun_cbw == (lun_num-1))
					{
					capacity[0] = 0;
					capacity[1] = 0;
					capacity[2] = 0;
					capacity[3] = 1;
					capacity[4] = 0;
					capacity[5] = 0;
					capacity[6] = 0x02;
					capacity[7] = 0;
					}
				else
					{
					//unsigned int lun_lba_max;
					#define lun_lba_max (((unsigned int *)(nand_page_buff +8))[0])
					lun_lba_max=lun_low_lba_start_ex[(lun_low_status[pudisk_lun_low_ex[lun_cbw]]& mask_lun_status_no)+1] - lun_low_lba_start_ex[(lun_low_status[pudisk_lun_low_ex[lun_cbw]]& mask_lun_status_no)]-1;
					capacity[0] = (0xFF & (lun_lba_max>>24));
					capacity[1] = (0xFF & (lun_lba_max>>16));
					capacity[2] = (0xFF & (lun_lba_max>>8));
					capacity[3] = (0xFF & (lun_lba_max));
					capacity[4] = 0;
					capacity[5] = 0;
					capacity[6] = 0x02;
					capacity[7] = 0;
					#undef lun_lba_max
					}
				usb_send(capacity,8);
				pcbw->dwCBWXferLength -= 8;
				}
			if(pcbw->dwCBWXferLength)
				usb_bulk_in_stall_token();
         		if(sensePtr == senseOk)
		 		return(USBS_PASSED);
	 		else
				return(USBS_FAILED); 
			#undef  capacity
      			}
		case MODE_SELECT_06:
		case MODE_SELECT_10:
			return(USBS_PASSED);
            
		case VERIFY_10:
		case TEST_UNIT_READY:
		case STOP_START_UNIT:
		case FORMAT_UNIT:
		case PREVENT_ALLOW_MEDIUM_REMOVAL:
		case SEEK_10:
			{
			sensePtr = senseOk;
			media_check(lun_cbw);
#if DEBUG
printf("sensePtr = %d, \n",sensePtr);
#endif			
			if(sensePtr == senseOk)
				{
				if(FORMAT_UNIT == pcbw->cdb[0])
		 			{
		 			if( TRUE == ums_check_lun_protect(lun_cbw) )
						{
						sensePtr = senseWriteProtected;
						}
					else
						{
						//do format unit here;
						if(K_SUCCES != nand_media_erease_all_valid_block())
							{
							sensePtr = senseWriteFault;
							}
						}
		 			}
				}
			if(sensePtr == senseOk)
		 		return(USBS_PASSED);
		 	else
            	 		return(USBS_FAILED);            
      			}
      		case REQUEST_SENSE:
      			{
	  		//UCHAR sense_info[18];
	  		#define sense_info nand_page_buff
			memset(sense_info , 0 , 18);
			sense_info[0] = 0x70;
			sense_info[2] = senseArray[sensePtr][0];
			sense_info[7] = 18-8;
			sense_info[12] = senseArray[sensePtr][1];
			sense_info[13] = senseArray[sensePtr][2];
			usb_send(sense_info,18);
	      		if(pcbw->dwCBWXferLength = (pcbw->dwCBWXferLength - 18))
	  			usb_bulk_in_stall_token();
			sensePtr = senseOk;
             		return(USBS_PASSED);
			#undef sense_info
      			}
      		case MODE_SENSE_06:
     		case MODE_SENSE_10:
	  		{
			sensePtr = senseOk;
			media_check(lun_cbw);
			if(sensePtr == senseOk)
				{
				//UCHAR msense[8];
				#define msense nand_page_buff
			 	memset(msense,0,8);
			 	if(pcbw->cdb[0] == MODE_SENSE_06)
			 		{
			 		msense[0] = 4 - 1;
					if( TRUE == ums_check_lun_protect(lun_cbw) )
						msense[2] = 0x80;
					usb_send(msense,4);
					pcbw->dwCBWXferLength -= 4;
			 		}
			 	else
			 		{
			 		msense[1] = 8-2;
					if( TRUE == ums_check_lun_protect(lun_cbw) )
						msense[3] = 0x80;
					usb_send(msense,8);
					pcbw->dwCBWXferLength -= 8;
			 		}
				}
			if(pcbw->dwCBWXferLength)
	  			usb_bulk_in_stall_token();
			if(sensePtr == senseOk)
		 		return(USBS_PASSED);
		 	else
            	 		return(USBS_FAILED);            
      			}
      		default:         
         		sensePtr = senseInvalidOpcode;
	  		if(pcbw->dwCBWXferLength)
	  			usb_bulk_in_stall_token();
         		return(USBS_FAILED);
   }
}


