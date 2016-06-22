#include<instructions.h>
#include <types.h>
#include <bsp.h>
#define NOK 0
#define OK  1
#define EEPROM_CNTRL 0x18011140
#define EEPROM_BASE  0x08000000
#define EEPROM_SIZE  0x8000

#define EEPROM_TCR 0x18011100
#define EEPROM_EIF   0x18011180


#define FCU_MAIN_ADDR       0x00000000
#define FCU_MAIN_SIZE       (64 * 1024 * 4)
#define FCU_INFO_ADDR       0x18010000
#define FCU_INFO_SIZE       (256 * 8)
#define FCU_ROW_SIZE        (32 * 4)  //64 words
#define FCU_MAIN_PAGE_SIZE  (8 * 32 * 4) //8 rows
#define FCU_INFO_PAGE_SIZE  (8 * 32 * 4) //4 rows
#define FCU_MAIN_PAGES      256
#define FCU_INFO_PAGES      1 
#define FCU_FCC_ADDR        (FCU_INFO_ADDR  + FCU_INFO_SIZE)
#define FCU_FWC_ADDR        (FCU_INFO_ADDR  + FCU_INFO_SIZE + 64)
#define FLASH_MAIN_BASE 0x00000000
#define FLASH_SIZE   0x40000
#define FLASH_PAGE_SIZE 1024

#define EEPROM_MODE 1


//写EEPROM函数 
unsigned char WriteEeprom(char *Buf,int Add,int NbByte )
{
    int i,j;
    unsigned long  addr=EEPROM_BASE,pos,mydata,tempdata,myaddr,count,lastaddr;
    char *ptr,tempdata8,lastflag=0;
    int totallen=0;
    Add &=0x7fff;
    pos=Add &3;
    addr+=Add;
    ptr=Buf;
    if(addr+NbByte>EEPROM_BASE+EEPROM_SIZE)
           return NOK;  
    // enable single-word erase + program
    REG32(EEPROM_CNTRL)=1;
    while(REG32(EEPROM_CNTRL)&0x80000000);
    count = 0;
    
    if(pos>0)
    {
        myaddr = addr&0xfffffffc;
        mydata = REG32(myaddr);
   
        if((pos+NbByte)>4)
        {
            memcpy((char *)(&mydata) +pos,Buf,4-pos);
            count += 4-pos; 
        }
        else
        {
            memcpy((char *)(&mydata) +pos,Buf,NbByte);
            count = NbByte;
        }
        REG32(myaddr) = mydata;
        ptr += (4-pos); 
        addr = myaddr +4;              
    }
    while(REG32(EEPROM_CNTRL)&0x80000000);     
    totallen=NbByte-count;
    while(totallen>0)
    {
     if(totallen >=4)
        {
         memcpy(&tempdata,ptr,4);
         ptr+=4;
         REG32(addr)=tempdata;
        }
     else
      {
         tempdata = REG32(addr);
         memcpy(&tempdata,ptr,totallen);
         ptr+=totallen;
         REG32(addr)=tempdata;
         lastaddr=addr;
         lastflag=1;
      } 
     totallen -=4; 
     // wait if eeprom busy
     while(REG32(EEPROM_CNTRL)&0x80000000);
     addr+=4;
     if(totallen <=0)
       break;
    }
    REG32(EEPROM_CNTRL)=0;
    addr=EEPROM_BASE + Add;
    /*verify the eeprom */
    for(j=0;j<3;j++)
    {
     i_dcache_invalidate();
     if(pos>0)
       {
          if(REG32(myaddr)!= mydata)
              return NOK;
       }
     if(lastflag==1)      
      {if(REG32(lastaddr)!= tempdata)
        return NOK;
      }  
     for(i=0;i<NbByte;i++)
     {
      tempdata8=REG8(addr+i);
      if(tempdata8!=Buf[i])
        return NOK;  
     }
    } 
    return OK;
}
/****************************** EepromAutoWrite *********************
Input   : Buf--- pointer of buffer
             Add---the data write the address  
             NbByte---the number of data which will write to eeprom
Output : none
Return : =0 means OK, other is failed; 
Function:  write some data to eeprom; 
**********************************************************************/ 
unsigned long EepromAutoWrite(unsigned char *Buf,  unsigned long int Add,unsigned long NbByte )
{
    int i;
    unsigned long addr=EEPROM_BASE,pos,current_page;
    int totallen=0;
    unsigned char *ptr;
    unsigned char tempdata8;
           
    Add &=0x7fff;
    pos=Add &3;
    addr+=Add;
    ptr=Buf;
    if(addr+NbByte>EEPROM_BASE+EEPROM_SIZE)
           return NOK;  
    // wait for eeprom ready   
    while(REG32(EEPROM_CNTRL)&0x80000000);
    
    // step 1: first write byte
    if(pos>0)
    {
      // enable single-word erase + program 
      REG32(EEPROM_CNTRL)|=1;
      totallen =4-pos;
      totallen = totallen > NbByte?NbByte:totallen;
      for(i=0;i<totallen;i++)
      {
       REG8(addr+i)=*ptr++;
	// wait for single-word erase + program ready   
       while(REG32(EEPROM_CNTRL)&0x80000000);
      }
      // disable single-word erase + program	
      REG32(EEPROM_CNTRL)&=0xfc;	  
      addr+=totallen;
      totallen=NbByte -totallen;      	  
    }
    else
      totallen=NbByte;
    
    // step 2: secend write multi 	
    current_page = addr&0xffffffc0;
    // enable multi-word erase + program 
    REG32(EEPROM_CNTRL)|=3;	
    while(totallen>=4)
    	{
    	memcpy(&i,ptr,4);
       REG32(addr) = i;
	addr += 4;
       totallen -= 4;
	ptr += 4;
	if((addr&0xffffffc0)!=current_page)
		{
		current_page = addr&0xffffffc0;
		// disable multi-word erase + program	 	
    		REG32(EEPROM_CNTRL)&=0xfc;
    		// wait for multi-word erase + program ready
    		while(REG32(EEPROM_CNTRL)&0x80000000);
		// enable multi-word erase + program 
    		REG32(EEPROM_CNTRL)|=3;	
		}
    	}
    // disable multi-word erase + program	 	
    REG32(EEPROM_CNTRL)&=0xfc;
    // wait for multi-word erase + program ready
    while(REG32(EEPROM_CNTRL)&0x80000000);

    // step 3: last write byte	
    if(totallen>0)
    	{
    	// enable single-word erase + program 
       REG32(EEPROM_CNTRL)|=1;
	for(i=0;i<totallen;i++)
      		{
       	REG8(addr+i)=*ptr++;
		// wait for single-word erase + program ready   
       	while(REG32(EEPROM_CNTRL)&0x80000000);
      		}
	// disable single-word erase + program	
       REG32(EEPROM_CNTRL)&=0xfc;
    	}

    // check the data
    i_dcache_invalidate();
    addr=EEPROM_BASE + Add;
    for(i=0;i<NbByte;i++)
    {
     tempdata8=REG8(addr+i);
     if(tempdata8!=Buf[i])
      return 1;  
    }
    return 0;
}

//功能描述：从EEPROM指定的地址Add开始填充Data指定的值，长度为NbByte。Add是EEPROM的虚拟地址
BYTE FillEeprom(BYTE Data, DWORD NbByte , DWORD Add)
{
    int i;
    DWORD pos,w_len;
    unsigned char t_buf[64];
    memset(t_buf,Data,64);
    pos = Add&0x3f;
    if(pos>=NbByte)
    	    w_len = NbByte;
    else
    	   	w_len = pos;   
    if(EepromAutoWrite(t_buf,Add,w_len))
    	  return 1; 
    NbByte -= w_len;
    Add += w_len;
    while(NbByte>0)
    	{
    		if(NbByte>=64)
    	    w_len = 64;
        else
    	   	w_len = NbByte;
    	  if(EepromAutoWrite(t_buf,Add,w_len))
    	   return 1;
    	  NbByte -= w_len;
        Add += w_len;    	    		
    	}
    return 0;
}

//功能描述：读指定长度的EEPROM数据到buf。
BYTE ReadEeprom(BYTE *Buf, DWORD Add, DWORD Length)
{
    SHORT i;
    DWORD addr=EEPROM_BASE;
    Add &= 0x7fff;
    addr+=Add;
    if(addr+Length>EEPROM_BASE+EEPROM_SIZE)
        return NOK;  
    for(i=0;i<Length;i++)
        *Buf++=REG8(addr++);
    return OK;
}


//功能描述：从FLASH指定的地址读数据到Buf，Add是FLASH的虚拟地址。数据长度不大于一个PAGE,允许跨页读取。
void ReadFlash(char *Buf, int Add, int Length)
{    
    int i;
    int addr=Add;
    for(i=0;i<Length;i++)
      *Buf++=REG8(addr++);
}

void zi1222_erase_flash(unsigned long newaddr )
{
  /*disable interrupt during write flashrom*/
  newaddr&=0x3ffff;
  REG32(FCU_FWC_ADDR)=5;
  newaddr &= 0xfffffc00;
  /* write a data to erase the page*/
  REG32(FCU_MAIN_ADDR+newaddr)=0;
  REG32(FCU_FWC_ADDR)=0;
}  

DWORD __write_flash(DWORD newaddr ,DWORD *buf, DWORD length)
{
  int i=0,fccr;
  unsigned int *from;
  unsigned long addr=newaddr,pageaddr=0;
  /*disable interrupt during write flashrom*/
  addr&=0x3ffff;
  from = (unsigned int *)buf;
  REG32(FCU_FWC_ADDR)=5;
  pageaddr=addr & 0xfffffc00;
  /* write a data to erase the page*/
  REG32(FCU_MAIN_ADDR+pageaddr)=0;
  REG32(FCU_FWC_ADDR)=0;
  /*enable single-word program */
  REG32(FCU_FWC_ADDR)=1;
  /*write data to dstaddr*/
  for (i=0; i< length; i+=4)
      REG32(FCU_MAIN_ADDR+addr+i)=*from++;
  REG32(FCU_FWC_ADDR)=0;
  return 1;
}


//功能描述：写数据到指定的FLASH地址, 数据长度任意（不会大于一个PAGE），
//Add是FLASH的虚拟地址，允许跨页操作,不影响页内其它数据
BYTE WriteFlash(BYTE *Buf, DWORD Add,DWORD Length)
{
    BYTE tempbuf[1024];
    DWORD len,copylen;
    DWORD pagestart_addr=0,pos=0;
    BYTE *ptr;
    DWORD i; 
    len=Length;
    if(Add+Length>FLASH_MAIN_BASE+FLASH_SIZE)
        return NOK;
    pagestart_addr = Add & 0xfffffc00;
    pos = Add & 0x3ff;
    copylen= FLASH_PAGE_SIZE -pos ;
    ptr=Buf;
    if(copylen > len)
        copylen =len;
    do
    {
        if(copylen < FLASH_PAGE_SIZE)
          ReadFlash(tempbuf,pagestart_addr,FLASH_PAGE_SIZE);
        memcpy(tempbuf+pos,ptr,copylen);
          __write_flash((DWORD)pagestart_addr ,(DWORD *)tempbuf,FLASH_PAGE_SIZE);
        pagestart_addr +=FLASH_PAGE_SIZE;
        pos =0;
        len -= copylen;
        ptr+=copylen;
        if(len > FLASH_PAGE_SIZE)
            copylen = FLASH_PAGE_SIZE;
        else
            copylen = len;
    }while(len>0);
    return OK;  
}



//功能描述：写一页（1K）数据到指定的FLASH。Add是FLASH的虚拟地址，且是该页的起始地址。
BYTE WriteFlashPage(BYTE *Buf, DWORD Add)
{
  return __write_flash(Add,(DWORD *)Buf,FLASH_PAGE_SIZE);
}
