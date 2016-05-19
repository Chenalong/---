#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#define MAX 100
/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

struct slink
{
    int  bignum[MAX];
    /*bignum[98]用来标记正负号，1正，0负bignum[99]来标记实际长度*/
    struct slink *next;
};

#ifdef __cplusplus
extern "C"
{
#endif
void DLL_EXPORT sub(int a[MAX],int b[MAX],int c[MAX] );
void DLL_EXPORT print(int a[MAX]);
int DLL_EXPORT cmp(int a1[MAX],int a2[MAX]);
void DLL_EXPORT mov(int a[MAX],int *b);
void DLL_EXPORT mul(int a1[MAX],int a2[MAX],int *c);
void DLL_EXPORT add(int a1[MAX],int a2[MAX],int *c);
void DLL_EXPORT sub(int a1[MAX],int a2[MAX],int *c);
void  DLL_EXPORT mod(int a[MAX],int b[MAX],int  *c);
void  DLL_EXPORT divt(int t[MAX],int b[MAX],int  *c,int *w);
void DLL_EXPORT mulmod(int a[MAX],int b[MAX],int n[MAX],int *m);
void DLL_EXPORT expmod(int a[MAX],int p[MAX],int n[MAX],int *m);
int DLL_EXPORT is_prime_san(int p[MAX] );
int DLL_EXPORT coprime(int e[MAX],int s[MAX]);
void DLL_EXPORT prime_random(int *p,int *q);
void  DLL_EXPORT erand(int e[MAX],int m[MAX]);
void DLL_EXPORT rsad(int e[MAX],int g[MAX],int *d);
unsigned long  DLL_EXPORT rsa(unsigned long p,unsigned long q,unsigned long e);
void DLL_EXPORT loadpkey(int e[MAX],int n[MAX]);
void DLL_EXPORT loadskey(int d[MAX],int n[MAX]);
void DLL_EXPORT savepkey(int e[MAX],int n[MAX]);
void DLL_EXPORT saveskey(int d[MAX],int n[MAX]);
void  DLL_EXPORT printbig(struct slink *h);
void  DLL_EXPORT tencrypto(int e[MAX], int n[MAX]);
void  DLL_EXPORT tdecrypto(int d[MAX], int n[MAX]);
struct slink* DLL_EXPORT input(void);
struct slink* DLL_EXPORT jiami(int  e[MAX],int  n[MAX],struct  slink *head);
void DLL_EXPORT jiemi(int d[MAX],int n[MAX],struct  slink *h);
void DLL_EXPORT menu();


#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
