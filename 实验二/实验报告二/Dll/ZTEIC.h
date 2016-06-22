



//vendor op
#define VENDOROP_ZTEIC             0xFF
#define VENDOROP_ZTEIC_CDBLEN      0x0A
#define VENDOROP_ZTEIC_CHECK_0     'A'
#define VENDOROP_ZTEIC_CHECK_1     'T'
#define VENDOROP_ZTEIC_CHECK_2     'O'
#define VENDOROP_ZTEIC_CHECK_3     'M'
#define VENDOROP_ZTEIC_CHECK_4     ' '
//read udisk lba (total)
#define VENDOROP_ZTEIC_READ_LBA                            0xFE
//write udisk lba (total)
#define VENDOROP_ZTEIC_WRITE_LBA                           0xFD
#define VENDOROP_ZTEIC_RW_LBA_FLAG_0                         'A'
#define VENDOROP_ZTEIC_RW_LBA_FLAG_1                         'T'
#define VENDOROP_ZTEIC_RW_LBA_FLAG_2                         'O'

//get ZTEIC flash disk total lba count,for format
//msb
//0--1---2--3   4--5--6--7  8--9--10--11     12--13--14--15   16--17--18--19   20--21--22--23
//total lba     lba size    lun0 lba max   lun1 lba max    lun2 lba max   lun3 lba max
#define VENDOROP_ZTEIC_GET_CAPACITY_EX                     0x01
#define VENDOROP_ZTEIC_GET_CAPACITY_EX_DATALEN             0x18
#define VENDOROP_ZTEIC_SET_CAPACITY_EX                     0x02
#define VENDOROP_ZTEIC_SET_CAPACITY_EX_DATALEN             0x18
//set common volume or security volume to renum,by test unit ready->not ready,request sence->media change,test unit ready->ready
#define VENDOROP_ZTEIC_SET_LUN_NOT_READY                   0x03
#define VENDOROP_ZTEIC_SET_LUN_READY                       0x04
#define VENDOROP_ZTEIC_LUN0             0x00
#define VENDOROP_ZTEIC_LUN1             0x01
#define VENDOROP_ZTEIC_LUN2             0x02
#define VENDOROP_ZTEIC_LUNALL           0xFF
//send password ro flash disk to enable security disk
#define VENDOROP_ZTEIC_SET_PASSWORD                       0x05
#define VENDOROP_ZTEIC_SET_PASSWORD_DATALEN               0x40
#define VENDOROP_ZTEIC_GET_PASSWORD                       0x06
#define VENDOROP_ZTEIC_GET_PASSWORD_DATALEN               0x01
#define VENDOROP_ZTEIC_CHANGE_PASSWORD                    0x07
#define VENDOROP_ZTEIC_CHANGE_PASSWORD_DATALEN            0x80

//usb key
#define VENDOROP_ZTEIC_USB_KEY                       	  0xFF
#define VENDOROP_ZTEIC_USB_KEY_KCW                        0x00
#define VENDOROP_ZTEIC_USB_KEY_KCW_LEN                    0x10
#define VENDOROP_ZTEIC_USB_KEY_DATA_IN                     0x01
#define VENDOROP_ZTEIC_USB_KEY_DATA_OUT                    0x02
#define VENDOROP_ZTEIC_USB_KEY_KSW                         0x03
#define VENDOROP_ZTEIC_USB_KEY_KSW_LEN                      0x10
#define VENDOROP_ZTEIC_USB_KEY_EXIT                       	0x04

//get protect status
#define VENDOROP_ZTEIC_GET_PROTECTSTS                     0x08
#define VENDOROP_ZTEIC_GET_PROTECTSTS_DATALEN             0x01
#define VENDOROP_ZTEIC_GET_PROTECTED				0x01
#define VENDOROP_ZTEIC_GET_UNPROTECTED				0x00


#define U_KEY_IOCTL_test CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)