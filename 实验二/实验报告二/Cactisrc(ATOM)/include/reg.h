
// 	Nand Flash Controller
//	==========================
#define NFCBASE 	    (0x18002000)
#define NFCR    	    NFCBASE
#define NFSR    	    ( NFCBASE + 0x04 )
#define NFECC   	    ( NFCBASE + 0x08 )
#define NFTCR   	    ( NFCBASE + 0x0C )
#define NFCMD   	    ( NFCBASE + 0x10 )
#define NFADDR  	    ( NFCBASE + 0x14 )
#define NFDATA  	    ( NFCBASE + 0x1000)
//===================================================

//	PPC RAM
//	===============================
#define  PPCRAMBASE	(0x10000000)

//	UDC
//	===============================
#define  UDCBASE	(0x18000000)

//	RSA
//	===============================
#define  RSABASE	(0x18020000)

//	SPI
//	===============================
#define  SPIBASE	(0x1802E000)
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
#define  UARTBASE	(0x1802E400)

//	PPC
//	===============================
#define  PPCBASE	(0x1802E800)
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
#define  RNGBASE	(0x1802F000)
#define  RNGCR		(RNGBASE)
#define  RNGDR		(RNGBASE + 0x04)

//	SEC
//	===============================
#define  SECBASE	(0x1802F400)

//	DES
//	===============================
#define  DESBASE	(0x1802F800)
#define  DESCTRL	(DESBASE+0x0)
#define  DESDATA        (DESBASE+0x4)
#define  DESIV          (DESBASE+0x8)
#define  DESKEY         (DESBASE+0xc)

//	INTC
//	===============================
#define  INTCBASE	(0x1803F000)

//	CGUPMC
//	===============================
#define  CGUPMCBASE	(0x1803F100)

//	TMU
//	===============================
#define  TMUBASE	(0x1803F200)

//	WDT
//	===============================
#define  WDTBASE	(0x1803F400)

//	GPIO
//	===============================
#define  GPIOBASE	(0x1803F500)
#define GPDR            (GPIOBASE)
#define GPDIR           (GPIOBASE+0x04)
#define GPALR           (GPIOBASE+0x0c)
#define GPAUR           (GPIOBASE+0x10)
#define GPIDR           (GPIOBASE+0x14)
#define GPIER           (GPIOBASE+0x18)
#define GPFR 		(GPIOBASE+0x1C)


//	SCD
//	===============================
#define  SCDBASE	(0x1803FD00)
#define SCDDR         (SCDBASE)
#define SCDCR         (SCDBASE+0x04)
#define SCDSR         (SCDBASE+0x08)
#define SCDTFR        (SCDBASE+0x0C)
#define SCDECR        (SCDBASE+0x10)
#define IOCSR0        (SCDBASE+0x14)
#define IOCSR1        (SCDBASE+0x18)

//	SCC0
//	===============================
#define  SCC0BASE	(0x1803FE00)
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
#define  SCC1BASE	(0x1803FF00)
#define SCC1BASER       SCC1BASE
#define SCC1DR         (SCC1BASER+0x0)
#define SCC1FDR        (SCC1BASER+0x4)
#define SCC1CR         (SCC1BASER+0x8)
#define SCC1SR         (SCC1BASER+0xc)
#define SCC1TFR        (SCC1BASER+0x10)
#define SCC1EGTR       (SCC1BASER+0x14)
#define SCC1ECR        (SCC1BASER+0x18)
#define SCC1RTOR       (SCC1BASER+0x1c)

//  INTC
//	===============================
#define INTCBASE 0x1803F000
#define ISR     (INTCBASE)
#define IMR     (INTCBASE+0x04)
#define IMSR    (INTCBASE+0x08)
#define IMCR    (INTCBASE+0x0c)
#define IPR     (INTCBASE+0x10)
#define RISIFR  (INTCBASE+0x14)




