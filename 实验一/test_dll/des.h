#ifndef __DES_H__
#define __DES_H__

#include <windows.h>

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif
//////////////////////////////////////////////////////////////////////////
/*
    Provided by ������, Northeastern University (www.neu.edu.cn)
    Email: blackdrn@sohu.com
	This product is free for use.
*/
//////////////////////////////////////////////////////////////////////////

enum    {ENCRYPT,DECRYPT};
//enum    bool{false,true}; // if bool is not supported,use this or just replace with char
// and use 1 for true,0 for false;
//////////////////////////////////////////////////////////////////////////

// Type��ENCRYPT:����,DECRYPT:����
// ���������(Out)�ĳ��� >= ((datalen+7)/8)*8,����datalen�������8�ı�������С������
// In ����= Out,��ʱ��/���ܺ󽫸������뻺����(In)������
// ��keylen>8ʱϵͳ�Զ�ʹ��3��DES��/����,����ʹ�ñ�׼DES��/����.����16�ֽں�ֻȡǰ16�ֽ�

bool DLL_EXPORT Des_Go(char *Out,char *In,long datalen,const char *Key,int keylen,bool Type = ENCRYPT);

//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // __DES_H__
