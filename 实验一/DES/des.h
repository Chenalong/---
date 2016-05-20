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
    Provided by 王俊川, Northeastern University (www.neu.edu.cn)
    Email: blackdrn@sohu.com
	This product is free for use.
*/
//////////////////////////////////////////////////////////////////////////

enum    {ENCRYPT,DECRYPT};
//enum    bool{false,true}; // if bool is not supported,use this or just replace with char
                            // and use 1 for true,0 for false;
//////////////////////////////////////////////////////////////////////////

// Type—ENCRYPT:加密,DECRYPT:解密
// 输出缓冲区(Out)的长度 >= ((datalen+7)/8)*8,即比datalen大的且是8的倍数的最小正整数
// In 可以= Out,此时加/解密后将覆盖输入缓冲区(In)的内容
// 当keylen>8时系统自动使用3次DES加/解密,否则使用标准DES加/解密.超过16字节后只取前16字节

bool DLL_EXPORT Des_Go(char *Out,char *In,char *Key,bool Type = ENCRYPT);

//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // __DESMAIN_H__
