#ifndef __UDC_H__
#define __UDC_H__

#include <regops.h>

#define UDC_FULL_SPEED           0x3
#define UDC_LOW_SPEED            0x2

#define UDC_REMOTE_WAKEUP_ENB    0x1
#define UDC_REMOTE_WAKEUP_DIS    0x0
#define UDC_SELF_POWERED         0x1
#define UDC_BUS_POWERED          0x0
#define UDC_SYNC_FRAME_ENB       0x1
#define UDC_SYNC_FRAME_DIS       0x0

#define UDC_RDMA_ENB             0x4
#define UDC_TDMA_ENB             0x8
#define UDC_DMA_UPDATE_DESC      0x10
#define UDC_BIG_ENDIAN           0x20
#define UDC_BUF_FILL             0x40
#define UDC_THRESHOLD_ENB        0x80
#define UDC_BURST_ENB            0x100
#define UDC_DMA_MODE             0x200
#define UDC_BRLEN_3              0x30000

#define UDC_BRLEN_SINGLE         0x00000
#define UDC_BRLEN_INCR           0x10000
#define UDC_BRLEN_WRAP4          0x20000
#define UDC_BRLEN_INCR4          0x30000
#define UDC_BRLEN_WRAP8          0x40000
#define UDC_BRLEN_INCR8          0x50000
#define UDC_BRLEN_WRAP16         0x60000
#define UDC_BRLEN_INCR16         0x70000

#define UDC_FLUSH_FIFO           0x2
#define UDC_STALL                0x1
#define UDC_SNOOP                0x4
#define UDC_POLL_DEMAND          0x8

#define UDC_DEV_STS_CFG          0x0000000f
#define UDC_DEV_STS_INTF         0x000000f0
#define UDC_DEV_STS_ALT          0x00000f00
#define UDC_DEV_STS_SUSP         0x00001000
#define UDC_DEV_STS_SPD          0x00006000
#define UDC_DEV_STS_TS           0xfffc0000

#define UDC_DEV_INTR_SC          0x1
#define UDC_DEV_INTR_SI          0x2
#define UDC_DEV_INTR_UR          0x8
#define UDC_DEV_INTR_US          0x10
#define UDC_DEV_INTR_SOF         0x20
#define UDC_DEV_INTR_ENUM        0x40


#define UDC_BASE_ADDR            0x18000000

#define UDC_EP0InControl         0x18000000
#define UDC_EP0InStatus          0x18000004
#define UDC_EP0InBufSize         0x18000008
#define UDC_EP0InMaxPktSize      0x1800000c
#define UDC_EP0InDescPtr         0x18000014

#define UDC_EP1InControl         0x18000020
#define UDC_EP1InStatus          0x18000024
#define UDC_EP1InBufSize         0x18000028
#define UDC_EP1InMaxPktSize      0x1800002c
#define UDC_EP1InDescPtr         0x18000034

#define UDC_EP0OutControl        0x18000200
#define UDC_EP0OutStatus         0x18000204
#define UDC_EP0OutFrmNum         0x18000208
#define UDC_EP0OutMaxPktSize     0x1800020c
#define UDC_EP0OutSetupBufPtr    0x18000210
#define UDC_EP0OutDescPtr        0x18000214

#define UDC_EP2OutControl        0x18000240
#define UDC_EP2OutStatus         0x18000244
#define UDC_EP2OutFrmNum         0x18000248
#define UDC_EP2OutMaxPktSize     0x1800024c
#define UDC_EP2OutDescPtr        0x18000254

#define UDC_DevConfig            0x18000400
#define UDC_DevControl           0x18000404
#define UDC_DevStatus            0x18000408
#define UDC_DevIntr              0x1800040c
#define UDC_DevIntrMask          0x18000410
#define UDC_EPIntr               0x18000414
#define UDC_EPIntrMask           0x18000418

// UDC CSR Registers
#define UDC_CSR               0x18000500
#define UDC_EP0Info           0x18000504
#define UDC_EP1Info           0x18000508
#define UDC_EP2Info           0x1800050c
#define UDC_EP3Info           0x18000510

// FIFO MAP address
// if we adjust fifo size, this macro should also be modified
#define UDC_InFifo            0x18000880 
#define UDC_OutFifo           0x18000800
#define UDC_WRCNFRM           0x1800041c
#define UDC_WRZeroLen         0x18000420

#define UDC_EP0MaxPktSize        0x8
#define UDC_EP1MaxPktSize        0x40
#define UDC_EP2MaxPktSize        0x40

//#define UDC_EP0BufSize           (UDC_EP0MaxPktSize / 4)
//#define UDC_EP1BufSize           (UDC_EP1MaxPktSize / 4)
//#define UDC_EP2BufSize           (UDC_EP2MaxPktSize / 4)

#define UDC_EP0BufSize           0x2
#define UDC_EP1BufSize           0x20
#define UDC_EP2BufSize           0x20

#define UDC_CTRL_TYPE            0x00
#define UDC_BULK_TYPE            0x02
#define UDC_INTR_TYPE            0x03

#define UDC_DIR_IN               0x1
#define UDC_DIR_OUT              0x0

#define UDC_DMA_DONE             0x2
#define UDC_DMA_BUSY             0x1
#define UDC_DESC_LAST            0x08000000

#define UDC_ENDPT0               0x0
#define UDC_ENDPT1               0x1
#define UDC_ENDPT2               0x2

#define UDC_INTR_EPIN0           0x1
#define UDC_INTR_EPIN1           0x2
#define UDC_INTR_EPOUT0          0x10000
#define UDC_INTR_EPOUT2          0x40000

#define UDC_SETUP_ADDR       0xffffffff

#define UDC_EP_STS_PID           0x0f
#define UDC_EP_STS_OUT           0x30
#define UDC_EP_STS_IN            0x40
#define UDC_EP_STS_BNA           0x80
#define UDC_EP_STS_FE            0x100
#define UDC_EP_STS_HE            0x200
#define UDC_EP_STS_PKTSIZE       0x03ff800

#define UDC_EP_OUT_RCV_DAT       0x010
#define UDC_EP_OUT_SETUP_DAT     0x020

#define UDC_DEV_CTRL_RES         0x1
#define UDC_DEV_CTRL_TF          0x2
#define UDC_DEV_CTRL_RDE         0x4
#define UDC_DEV_CTRL_TDE         0x8
#define UDC_DEV_CTRL_DU          0x10
#define UDC_DEV_CTRL_BE          0x20
#define UDC_DEV_CTRL_BF          0x40
#define UDC_DEV_CTRL_THE         0x80
#define UDC_DEV_CTRL_BREN        0x100
#define UDC_DEV_CTRL_BRLEN       0x00ff0000
#define UDC_DEV_CTRL_THLEN       0xff000000

#define UDC_MAX_ENDPT_NUM        0x3

#endif
