#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#include <string>
#include <string.h>

using namespace std;

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

void DLL_EXPORT SomeFunction(const LPCSTR sometext);
void DLL_EXPORT getMD5(char* ret_str,char* input);
string DLL_EXPORT changeHex(int a);
unsigned int* DLL_EXPORT add(string str);
void DLL_EXPORT mainLoop(unsigned int M[]);


#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
