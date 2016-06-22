#ifndef __UART_H__
#define __UART_H__

typedef char S8;
typedef short S16;
typedef long S32;
typedef long long S64;

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned long U32;
typedef unsigned long long U64;

#define UART_BASIC_ADDRESS (0x1802e400)
#define UART_DR (UART_BASIC_ADDRESS)
#define UART_RSR_ECT (UART_BASIC_ADDRESS+4)
#define UART_LCR_H (UART_BASIC_ADDRESS+8)
#define UART_LCR_M (UART_BASIC_ADDRESS+0x0c)
#define UART_LCR_L (UART_BASIC_ADDRESS+0x10)
#define UART_CR (UART_BASIC_ADDRESS+0x14)
#define UART_FR (UART_BASIC_ADDRESS+0x18)
#define UART_IIR_ICR (UART_BASIC_ADDRESS+0x1c)
#define UART_REIER (UART_BASIC_ADDRESS+0x24)



#endif
