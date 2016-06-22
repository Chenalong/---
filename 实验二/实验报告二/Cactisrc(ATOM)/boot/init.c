/* Name : init.c
 *  system  initialize module
 */
/*
 * CHANGELOG:
 * 05-06-2005   modified by Zhushan for Zi1222  
 * 17-08-2003	modified by Stanley.Zhu<zhu.shan@zte.com.cn>
 * 23-07-2003	created by ARCA TECH
*-------------------------------------------------------------------------*/

#include <instructions.h>
#include <bsp.h>
#include <config.h>
#include <uart.h>
#include "pp.h"

void uart_init_dynamic(int lcrl);
void pp_sram_init();
unsigned long sys_clk;

//	RNG
//	===============================
#define  RNGBASE	(0x1802F000)
#define  RNGCTRL		(RNGBASE)
#define  RNGDATA		(RNGBASE + 0x04)

extern long rngctl_data;
/**************************** RNG_Init ********************************
Input   : none; 
Output : none;
Return : none;
Function:  initialize the RNG module; 
**********************************************************************/ 
void RNG_Init()
{

   REG32(RNGCTRL)=rngctl_data;
}

/**************************** Get_Rand ********************************
Input   : x--- the RNG control register value 
Output : none;
Return : the random;
Function:  get a random of U32 type; 
**********************************************************************/ 
unsigned long Get_Rand()
{
    unsigned long x;
	while((REG32(RNGCTRL)&2)==0);
	x=REG32(RNGDATA);
	REG32(RNGCTRL) &=0xfffffffd;
	return x;
}

char *end_of_data_section ="END";
long rngctl_data;

unsigned char chip96mode=0;

/*stack top ,which defined in link script*/
extern int _stack;
void print_string(unsigned char *string);
void print_byte(char x);

#define CFCR 0x1803F100
#define PLCR 0x1803F110
#define FCCR 0x18010800
#if 1
void set_sysclk(unsigned long clk)
{
  if(REG32(CFCR)&0xfff)
    {
   	    rngctl_data=0x4b5;
        REG32(FCCR) = 0x00055;
        sys_clk=24000000;
        chip96mode=0;
        RNG_Init();
        return;    
    }
  switch (clk)
  {    
     case 24000000: // = 24Mhz
        REG32(CFCR)=0x46bb66;
   	    rngctl_data=0x4b5;
        REG32(FCCR) = 0x00055;
        sys_clk=24000000;
	    break;
    case 96000000:  // = 96Mhz
         REG32(CFCR)=0x42bb22;
         rngctl_data=0xff5;
         REG32(FCCR) = 0x30064;
         sys_clk=96000000;
	    break;
    case 144000000:  // = 144Mhz
         REG32(CFCR)=0x41bb11;
         rngctl_data=0x1685;
         REG32(FCCR) = 0x40064;
         sys_clk=144000000;
	    break;
    default : // default = 36Mhz
        REG32(CFCR)=0x45bb55;
  	    rngctl_data=0x965;
        sys_clk=36000000;
        REG32(FCCR) = 0x10055;
    break;  
  }
  // config CGU and start
  REG32(PLCR)=0x2f000132;
  chip96mode=1;
  RNG_Init();
}


void set_clk(unsigned long clk)
{
  //disable interrupt
  dis_interrupt();
  //wait uart not busy
  if(chip96mode)
  {
  //while(REG8(UART_FR)&0x08);
  switch (clk)
  {    
     case 24000000: // = 24Mhz
        REG32(CFCR)=0x46fb66;
        REG32(FCCR) = 0x00055;
   	    rngctl_data=0x4b5;
        sys_clk=24000000;
   	    break;
    case 96000000:  // = 96Mhz
         REG32(FCCR) = 0x30064;
         REG32(CFCR)=0x42fb22;
         rngctl_data=0xff5;
         sys_clk=96000000;
 	    break;
    case 144000000:  // = 144Mhz
         REG32(FCCR) = 0x40064;
         REG32(CFCR)=0x43fb31;
         rngctl_data=0x1685;
         sys_clk=144000000;
 	    break;
    default : // default = 36Mhz
        REG32(CFCR)=0x45fb55;
        REG32(FCCR) = 0x10055;
  	    rngctl_data=0x965;
        sys_clk=36000000;
   break;  
  }
  
  //rng register init;
  RNG_Init();
  //pp_sram_init();
  uart_init_dynamic(0);
  //enable interrupt
  enb_interrupt();
  }
}
#endif
/*
  copy SYS_DATA and DATA to ram space ,if you will modify this function
  please read link scripts first!
*/ 

 void copy_rom_to_ram()
{
extern long _esys_data, _edata,_erodata;
unsigned char *src, *dest, *end_dest;
unsigned char *idest,*iend_dest;

 src = (unsigned char*) (&_erodata); 
 dest = (unsigned char*) 0x0c000020; 
 end_dest = (unsigned char*) (&_esys_data);
 for (dest;dest <end_dest; dest ++)
   {
     *dest = *src; src++;
   }
 dest = (unsigned char*) (&_esys_data); 

 end_dest = (unsigned char*) (&_edata);

 for (dest;dest <end_dest; dest ++)
   {
     *dest = *src; src++;
   }

}

/*system initialize */
#define set_iocsr1_d()                   set_iocsr1(get_iocsr1() | IOCSR_D)
#define clr_iocsr1_d()                   set_iocsr1(get_iocsr1() & ~IOCSR_D) 
#define get_iocsr1_d()                   (get_iocsr1() & IOCSR_D)>>6

#define set_iocsr1_di_out()              set_iocsr1(get_iocsr1() | IOCSR_DI)
#define clr_imask_bit(int_type)  do{i_s32((1<<int_type),IMCR,0);}while(0)

void usb_initialize();
int main();

#define GPIO_BASE 0x1803F500
#define GPDR           (GPIO_BASE)
#define GPDIR         (GPIO_BASE+4) // 0:input; 1:output
#define GPALR         (GPIO_BASE+0xc)
#define GPAUR         (GPIO_BASE+0x10)
#define GPIDR         (GPIO_BASE+0x14)
#define GPIER         (GPIO_BASE+0x18)
#define GPFR         (GPIO_BASE+0x1c)

/*初始化串口函数*/ 
void uart_init()
{
    long uart_div=0;
    uart_div= (sys_clk /(115200*16))-1; 
    /*set gpio25 gpio26  mode to  uart*/
    REG32(GPAUR) &= 0xFFEBFFFF;
    REG32(GPAUR) |=0x140000;
    /* stop uart module */
	REG8(UART_CR) = 0;
	/* set uart setting,baudrate=115200 */
	REG8(UART_LCR_M) = 0;
	
	REG8(UART_LCR_L) =uart_div;//39;//39;//51;//39;//51;//39;//76;//51;//76;//51;//103;//76;//39;//76;//51;//39;//51;//76;
	REG8(UART_LCR_H) = 0x70;
    /* start uart module */
    REG8(UART_CR) = 0x11;		
}

/*初始化串口函数*/ 
void uart_init_dynamic(int lcrl)
{
    long uart_div=0;
    uart_div= (sys_clk /(115200*16))-1; 
    //wait for not busy
    while(REG8(UART_FR)&0x08);
    /* stop uart module */
	REG8(UART_CR) = 0;
	REG8(UART_LCR_M) = 0;
	REG8(UART_LCR_L) =uart_div;
	REG8(UART_LCR_H) = 0x70;
    /* start uart module */
    REG8(UART_CR) = 0x11;		
}
/*初始化SRAM 读些时序及输出，输入接口函数*/ 

void pp_sram_init()
{
  int i=0;

  //set gpio mode to sram
  REG32(GPALR) &= 0xCC000000;
  REG32(GPAUR) &= 0xFFFCC0FF;
  REG32(GPAUR) |= 0x012400 ;
  REG32(GPALR) |= 0x128AAAAA ; 

  REG32(ExtPortCTL)=0x01d00; 
 if(sys_clk>96000000)
  { 
  REG32(PPORT_CTL_REG1)=0x020001;
  //ADDR[2:0]
  REG32(PPORT_CTL_REG3)=0x000001;
  REG32(PPORT_CTL_REG4)=0x000001;
  REG32(PPORT_CTL_REG5)=0x000001;
  //ALE1:2 	 high valid Tset=1 
  REG32(PPORT_CTL_REG6)=0x01801;
  REG32(PPORT_CTL_REG7)=0x01801;
  //RE/WE      Low valid Tset=1
  REG32(PPORT_CTL_REG8)=0x200001;
  REG32(PPORT_CTL_REG9)=0x020001;
  //Tsig=2 Thold=1
  REG32(PPORT_WR_DATA_CTL)=0x02000;
  REG32(PPORT_RD_DATA_CTL)=0x00200;
   //enable RB
  REG32(PPORT_CTL_34)=0xc00;
 }
else
 { 
   REG32(PPORT_CTL_REG1)=0x010001;
  //ADDR[2:0]
  REG32(PPORT_CTL_REG3)=0x000001;
  REG32(PPORT_CTL_REG4)=0x000001;
  REG32(PPORT_CTL_REG5)=0x000001;
  //ALE1:2 	 high valid Tset=1 
  REG32(PPORT_CTL_REG6)=0x00801;
  REG32(PPORT_CTL_REG7)=0x00801;
  //RE/WE      Low valid Tset=1
  REG32(PPORT_CTL_REG8)=0x100001;
  REG32(PPORT_CTL_REG9)=0x000001;
  //Tsig=2 Thold=1
  REG32(PPORT_WR_DATA_CTL)=0x00000;
  REG32(PPORT_RD_DATA_CTL)=0x00100;
   //enable RB
  REG32(PPORT_CTL_34)=0xc00;
 }
}



/*注意,在zi1221上片内ram的起始地址是0x10000000,
  在Cacti上片内ram的起始地址是0x0c000000,并口ram的起始地址是0x10000000
*/ 
#define enable_boot(x,y,z) ((void (*)())(0x80000800))(x,y,z)
 void init(void)
{
  int failed_p = 0,flag=0;
  //set gpio 21 to L , unselect ext sram
  REG32(GPDIR)|= 0x00d00000;  
  REG32(GPDR) &=0xff2fffff; 
  USB_PULLUP_LOW();
  //
  //enable_boot(0x212a45b7,0x78346321,0x23e545ba);
  copy_rom_to_ram();
  set_sysclk(96000000);
  uart_init();
  pp_sram_init();
  usb_initialize();
  printf("Cacti start...chip96mode=%d ..\r\n",chip96mode);
  //printf("GPDIR = 0x%x ,GPDR = 0x%x , GPAUR = 0x%x \n",REG32(GPDIR),REG32(GPDR),REG32(GPAUR));
  failed_p = main();
  if (failed_p == 1)	 fail_();
  if (failed_p == 0)     pass_();
}

 void fail_()      
{                                                     
   print_string("Application failed!\n");
   while(1);                                           
}

 void pass_()                                         
{                                                     
   print_string("Application passed!\n");
   while(1);                                            
}


void print_string(unsigned char *string)
{
   while(*string)
    {
        if(*string=='\n')
          print_byte(0x0d);
        print_byte(*string++);
    }
}

void print_long(unsigned long x)
{
  int i,m;
  unsigned char led_addr[10];
    
  for (i=0;i<8;i++)
    {
      m = (x>>(i*4))&0xf;
      if (m>9)
	m =  m-10+'A';
      else m = m+'0';
      led_addr[7-i]=m;
    }
    led_addr[8]='\n';
    led_addr[9]=0;
    print_string(led_addr);
      
}

 void delay_xxx(int x)
{
    while(x--);
}


void uart_send_byte(char a)
{
      while(REG8(UART_FR)&0x20);
      REG8(UART_DR) = a;
}


unsigned char uart_receive_byte()
{
      while(REG8(UART_FR)&0x10);
      return REG8(UART_DR);  
}

void print_byte(char x)
{
  uart_send_byte(x);     
}

