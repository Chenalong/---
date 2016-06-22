// GfL.h: interface for the CGfL class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __CGFL_H
#define __CGFL_H

#define UINT unsigned int

//////////////////////////////////////////////////////////////////////
//通用库(class CGfL)
class CGfL
{
public:
    // 以下代码作者：0200980 胡德华
	static  bool Transform(bool *Out,bool *In,const char *Table,UINT len);//变换
	static  bool Xor(bool *InA,const bool *InB,UINT len);//异或
	static  bool RotateL(char *In,UINT len,UINT loop);//循环左移
    // 以下代码作者：0201005 刘红旗
	static  bool ByteToBit(bool *Out,const char *In,UINT len,UINT num=8);//字节组转换成位组
	static  bool BitToByte(char *Out,const bool *In,UINT len,UINT num=8);//位组转换成字节组
	static  bool HalfByteToByte(char *Out,const char *In,UINT len);//半字节组转换成字节组
	static  bool ByteToHalfByte(char *Out,const char *In,UINT len);//字节组转换成半字节组
	static  int  StrToHalfByte(char *Out,char *In,UINT len);//字符串转换成半字节组
	static  int  HalfByteToStr(char *Out,char *In,UINT len);//半字节组转换成字符串
};

#endif
