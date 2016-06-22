#ifndef   __REG1222_H
#define   __REG1222_H

// 	Nand Flash Controller
//	==========================
#define NFCBASE 	    (0x9803FC00)
#define NFCR    	    NFCBASE
#define NFSR    	    ( NFCBASE + 0x04 )
#define NFECC   	    ( NFCBASE + 0x08 )
#define NFTCR   	    ( NFCBASE + 0x0C )
#define NFCMD   	    ( NFCBASE + 0x10 )
#define NFADDR  	    ( NFCBASE + 0x14 )
#define NFDATA  	    ( NFCBASE + 0x18 )
//===================================================

//	PPC RAM
//	===============================
#define  PPCRAMBASE	(0x90000000)

//	UDC
//	===============================
#define  UDCBASE	(0x98000000)

//	RSA
//	===============================
#define RSA_RAM_SIZE 0x400

#define RSACOMADD 0x98020000
#define RSACTLADD (RSACOMADD+4)
#define RSANLENADD (RSACOMADD+8)
#define RSAELENADD (RSACOMADD+0x0c)
#define RSAMODEADD (RSACOMADD+0x10)

#define RSABASE 0x98020800
#define RSAEBASE (RSABASE+0x80)
#define RSAABASE RSABASE
#define RSABBASE (RSABASE+0x200)
#define RSANBASE (RSABASE+0x300)
#define RSAHBASE (RSABASE+0x280)
#define RSAB2BASE (RSABASE+0x100)
#define RSAMBASE (RSABASE+0x180)

#define ECC_A RSABASE
#define ECC_CL (RSABASE+0x80)
#define ECC_CH (RSABASE+0xc0)
#define ECC_S (RSABASE+0x80)
#define ECC_B (RSABASE+0x200)
#define ECC_F (RSABASE+0x240)

#define RSAHCOM 0x2
#define RSAHAECOM 0x6
#define RSAHABCOM 0xa
#define RSAAECOM 0x4
#define RSAABCOM 0x8

#define ECC_ABmul_CMD 0xd
#define ECC_CmodF_CMD 0xe
#define ECC_ABmodF_CMD 0xf

#define RSA_DEFAULT_MODE 0x0
#define RSA_2048_MODE 0x1
#define RSA_1024_MODE 0x2
#define RSA_ECC_MODE 0x3



//	SPI
//	===============================
#define  SPIBASE	(0x9802E000)
#define SPI_BASIC_ADDRESS SPIBASE
#define SPI_CNTL_REG0 (SPI_BASIC_ADDRESS)
#define SPI_CNTL_REG1 (SPI_BASIC_ADDRESS+4)
#define SPI_FIFO_REG (SPI_BASIC_ADDRESS+8)
#define SPI_STATUS_REG (SPI_BASIC_ADDRESS+0x0c)
#define SPI_DIV_REG (SPI_BASIC_ADDRESS+0x10)
#define SPI_INTERRUPT_REG (SPI_BASIC_ADDRESS+0x14)
#define SPI_REV_REG0 (SPI_BASIC_ADDRESS+0x18-0x3c)
#define SPI_REV_REG1 (SPI_BASIC_ADDRESS+0x40-0x90)
#define SPI_REV_REG2 (SPI_BASIC_ADDRESS+0x94-0xff)

//	UART
//	===============================
#define  UARTBASE	(0x9802E400)

//	PPC
//	===============================
#define  PPCBASE	(0x9802E800)
#define PPORT_REG_BASE PPCBASE
#define PPORT_DATA_BASE  (PPORT_REG_BASE+0x300)
#define PPORT_CTL_REG1 (PPORT_REG_BASE+0)
#define PPORT_CTL_REG2 (PPORT_REG_BASE+4)
#define PPORT_CTL_REG3 (PPORT_REG_BASE+8)
#define PPORT_CTL_REG4 (PPORT_REG_BASE+0x0c)
#define PPORT_CTL_REG5 (PPORT_REG_BASE+0x10)
#define PPORT_CTL_REG6 (PPORT_REG_BASE+0x14)
#define PPORT_CTL_REG7 (PPORT_REG_BASE+0x18)
#define PPORT_CTL_REG8 (PPORT_REG_BASE+0x1c)
#define PPORT_CTL_REG9 (PPORT_REG_BASE+0x20)


//	RNG
//	===============================
#define  RNGBASE	(0x9802F000)
#define  RNGCTRL		(RNGBASE)
#define  RNGDATA		(RNGBASE + 0x04)

//	SEC
//	===============================
#define  SECBASE	(0x9802F400)

//	DES
//	===============================
#define  DESBASE	(0x9802F800)
#define  DCNTRL	(DESBASE+0x0)
#define  DDAT        (DESBASE+0x4)
#define  DESIV          (DESBASE+0x8)
#define  DKEY         (DESBASE+0xc)

//	INTC
//	===============================
#define  INTCBASE	(0x9803F000)

//	CGUPMC
//	===============================
#define  CGUPMCBASE	(0x9803F100)

//	TMU
//	===============================
#define  TMUBASE	(0x9803F200)

//	WDT
//	===============================
#define  WDTBASE	(0x9803F400)

//	GPIO
//	===============================
#define  GPIOBASE	(0x9803F500)
#define GPDR            (GPIOBASE)
#define GPDIR           (GPIOBASE+0x04)
#define GPALR           (GPIOBASE+0x0c)
#define GPAUR           (GPIOBASE+0x10)
#define GPIDR           (GPIOBASE+0x14)
#define GPIER           (GPIOBASE+0x18)
#define GPFR 		(GPIOBASE+0x1C)

//	NFC
//	===============================
#define  NFCBASE	(0x9803FC00)

//	SCD
//	===============================
#define  SCDBASE	(0x9803FD00)
#define SCDDR         (SCDBASE)
#define SCDCR         (SCDBASE+0x04)
#define SCDSR         (SCDBASE+0x08)
#define SCDTFR        (SCDBASE+0x0C)
#define SCDECR        (SCDBASE+0x10)
#define IOCSR0        (SCDBASE+0x14)
#define IOCSR1        (SCDBASE+0x18)

//	SCC0
//	===============================
#define  SCC0BASE	(0x9803FE00)
#define SCC0BASER       SCC0BASE
#define SCC0DR         (SCC0BASER+0x0)
#define SCC0FDR        (SCC0BASER+0x4)
#define SCC0CR         (SCC0BASER+0x8)
#define SCC0SR         (SCC0BASER+0xc)
#define SCC0TFR        (SCC0BASER+0x10)
#define SCC0EGTR       (SCC0BASER+0x14)
#define SCC0ECR        (SCC0BASER+0x18)
#define SCC0RTOR       (SCC0BASER+0x1c)

//	SCC1
//	===============================
#define  SCC1BASE	(0x9803FF00)
#define SCC1BASER       SCC1BASE
#define SCC1DR         (SCC1BASER+0x0)
#define SCC1FDR        (SCC1BASER+0x4)
#define SCC1CR         (SCC1BASER+0x8)
#define SCC1SR         (SCC1BASER+0xc)
#define SCC1TFR        (SCC1BASER+0x10)
#define SCC1EGTR       (SCC1BASER+0x14)
#define SCC1ECR        (SCC1BASER+0x18)
#define SCC1RTOR       (SCC1BASER+0x1c)




#endif

