// GfL.h: interface for the CGfL class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __CGFL_H
#define __CGFL_H

#define UINT unsigned int

//////////////////////////////////////////////////////////////////////
//ͨ�ÿ�(class CGfL)
class CGfL
{
public:
    // ���´������ߣ�0200980 ���»�
	static  bool Transform(bool *Out,bool *In,const char *Table,UINT len);//�任
	static  bool Xor(bool *InA,const bool *InB,UINT len);//���
	static  bool RotateL(char *In,UINT len,UINT loop);//ѭ������
    // ���´������ߣ�0201005 ������
	static  bool ByteToBit(bool *Out,const char *In,UINT len,UINT num=8);//�ֽ���ת����λ��
	static  bool BitToByte(char *Out,const bool *In,UINT len,UINT num=8);//λ��ת�����ֽ���
	static  bool HalfByteToByte(char *Out,const char *In,UINT len);//���ֽ���ת�����ֽ���
	static  bool ByteToHalfByte(char *Out,const char *In,UINT len);//�ֽ���ת���ɰ��ֽ���
	static  int  StrToHalfByte(char *Out,char *In,UINT len);//�ַ���ת���ɰ��ֽ���
	static  int  HalfByteToStr(char *Out,char *In,UINT len);//���ֽ���ת�����ַ���
};

#endif
