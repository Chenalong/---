#ifndef __CCORE_NFC_H__
#define __CCORE_NFC_H__

#define NAND_FLASH_PAGE_SIZE 512



// 	Nand Flash Controller
//	==========================
#define NFCBASE 	    (0x9803FC00+0x0)
#define NFCR    	    NFCBASE
#define NFSR    	    ( NFCBASE + 0x04 )
#define NFECC   	    ( NFCBASE + 0x08 )
#define NFTCR   	    ( NFCBASE + 0x0C )
#define NFCMD   	    ( NFCBASE + 0x10 )
#define NFADDR  	    ( NFCBASE + 0x14 )
#define NFDATA  	    ( NFCBASE + 0x18 )

#define NFCR_ERST		0x08
#define NFCR_ECCE		0x04
#define NFCR_NFCE		0x02
#define NFCR_NFE		0x01

#define NFSR_RB			0x01

#define NFECC_ECC2		0x00FF0000
#define NFECC_ECC1		0x0000FF00
#define NFECC_ECC0		0x000000FF

#define NFTCR_RAT		0x0000F000
#define NFTCR_WAT		0x00000F00
#define NFTCR_AHT		0x00000070
#define NFTCR_AST		0x00000007

#define set_nfcr_val(val)		{REG8( NFCR  )= val;}
#define set_nftcr_val(val)		{REG32( NFTCR  )= val;}

#define set_nfcmd_val(val)	{REG8( NFCMD )= val;}
#define set_nfaddr_val(val)	{REG8( NFADDR )= val;}
#define set_nfdata_val(val)	{REG8( NFDATA )= val;}

#define get_nfcr() 	 		i_l8u( NFCR    ,0 )
#define get_nfsr() 	 		i_l8u( NFSR    ,0 )    
#define get_nfecc() 		REG32( NFECC )    
#define get_nftcr() 		REG32( NFTCR )
//#define get_nfcmd() 		i_l8u( NFCMD   ,0 )
//#define get_nfaddr() 		i_l8u( NFADDR  ,0 )
#define get_nfdata() 		i_l8u( NFDATA  ,0 )

#define set_nfcr_erst()		set_nfcr_val( get_nfcr() | NFCR_ERST  )
#define set_nfcr_ecce()		set_nfcr_val( get_nfcr() | NFCR_ECCE  )
#define set_nfcr_nfce()		set_nfcr_val( get_nfcr() | NFCR_NFCE  )
#define set_nfcr_nfe()		set_nfcr_val( get_nfcr() | NFCR_NFE	  )

#define clr_nfcr_erst()		set_nfcr_val( get_nfcr() & ~NFCR_ERST  )
#define clr_nfcr_ecce()		set_nfcr_val( get_nfcr() & ~NFCR_ECCE  )
#define clr_nfcr_nfce()		set_nfcr_val( get_nfcr() & ~NFCR_NFCE  )
#define clr_nfcr_nfe()		set_nfcr_val( get_nfcr() & ~NFCR_NFE  )

#define get_nfcr_erst()		(( get_nfcr() & NFCR_ERST  )>>3)
#define get_nfcr_ecce()		(( get_nfcr() & NFCR_ECCE  )>>2)
#define get_nfcr_nfce()		(( get_nfcr() & NFCR_NFCE  )>>1)
#define get_nfcr_nfe()		( get_nfcr() & NFCR_NFE   )

#define get_nfsr_rb()		( get_nfsr() & NFSR_RB )

#define get_nfecc_ecc2()	(( get_scccr() & NFECC_ECC2 )>>16 )
#define get_nfecc_ecc1()	(( get_scccr() & NFECC_ECC1 )>>8 )
#define get_nfecc_ecc0()	( get_scccr() & NFECC_ECC0 )

#define get_nftcr_rat()		(( get_nftcr() & NFTCR_RAT ) >>12 )
#define get_nftcr_wat()		(( get_nftcr() & NFTCR_WAT ) >>8 )
#define get_nftcr_aht()		(( get_nftcr() & NFTCR_AHT ) >>4 )
#define get_nftcr_ast()		(( get_nftcr() & NFTCR_AST ) >>0 )

//#define set_nftcr_rat_val(val)	set_nftcr_val( (get_nftcr()&(~NFTCR_RAT)) | ((val<<12)&NFTCR_RAT))
//#define set_nftcr_wat_val(val)	set_nftcr_val( (get_nftcr()&(~NFTCR_WAT)) | ((val<<8) &NFTCR_WAT))
//#define set_nftcr_aht_val(val)	set_nftcr_val( (get_nftcr()&(~NFTCR_AHT)) | ((val<<4) &NFTCR_AHT))
//#define set_nftcr_ast_val(val)	set_nftcr_val( (get_nftcr()&(~NFTCR_AST)) |  (val&NFTCR_AST) )

//	nand flash command defines
#define NFCMD_PROGRAM1	0x80
#define NFCMD_PROGRAM2	0x10
#define NFCMD_ERASE1		0x60
#define NFCMD_ERASE2		0xD0
#define NFCMD_READSTS		0x70
#define NFCMD_READ0		0x00
#define NFCMD_READ1		0x01
#define NFCMD_READ2		0x50
#define NFCMD_READ_REDT	0x50
#define NFCMD_READID		0x90
#define NFCMD_RST			0xFF
#define NFCMD_CBPROG1		0x00
#define NFCMD_CBPROG2		0x8A




extern void NandFlash_Init(void);
extern void NandFlash_Reset(void);
extern void NandFlash_ReadId(unsigned char *idbuf);
extern void NandFlash_SetAddress(unsigned long addr);
extern unsigned char NandFlash_WritePage(unsigned char *buf,unsigned long addr,char eccmode);
extern unsigned char NandFlash_ReadPage(unsigned char *buf,unsigned long addr, char eccmode);
extern unsigned char NandFlash_BlockErase(unsigned long addr);
extern unsigned char NandFlash_Copyback( unsigned long saddr, unsigned long daddr );

#endif

//===================================================

