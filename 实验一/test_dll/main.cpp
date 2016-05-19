#include <iostream>
#include <stdio.h>
#include <des.h>
#include <rsa.h>

using namespace std;

void test_rsa()
{
    int  i;
    char  c;
    int  p[MAX],q[MAX],n[MAX],d[MAX],e[MAX],m[MAX],p1[MAX],q1[MAX];
    struct slink *head,*h1,*h2;
    for(i=0; i<MAX; i++)
        m[i]=p[i]=q[i]=n[i]=d[i]=e[i]=0;/*/简单初始化一下*/

    while (1)
    {
        menu();
        c=getchar();
        getchar();//接受回车符
        if ((c=='r') || (c=='R'))//操作r产生密钥对
        {
            for(i=0; i<MAX; i++)
                m[i]=p[i]=q[i]=n[i]=d[i]=e[i]=0;
            printf("\n\n\n\n\n\n\n\n\n");
            printf("\n\n随机密钥对产生如下：\n\n");
            prime_random(p,q);/*/随机产生两个大素数*/
            mul(p,q,n);
            printf("由 p、q 得出 n :");
            print(n);
            mov(p,p1);
            p1[0]--;
            mov(q,q1);
            q1[0]--;      /*/q-1;*/
            mul(p1,q1,m);//m=(p-1)*(q-1)
            erand(e,m);
            rsad(e,m,d);
            printf("密钥对产生完成，现在可以直接进行加解密文件!\n");
            printf("\n按任意键回主菜单…………");
            getchar();
        }
        else if((c=='l') || (c=='L'))
        {
            printf("\n\n选择导入密钥类型：加密密钥(P)还是解密密钥(S)?");
            c=getchar();
            getchar();
            if ((c=='p')||(c=='P'))
                loadpkey(e,n);
            else if((c=='s')||(c=='S'))
                loadskey(d,n);
            printf("\n按任意键回主菜单…………");
            getchar();
        }
        else if ((c=='e') || (c=='E'))
        {
            tencrypto( e,  n);
            printf("\n加密文件操作完成!\n");
            printf("\n按任意键回主菜单…………");
            getchar();
            getchar();
        }
        else if ((c=='d') || (c=='D'))
        {
            tdecrypto( d, n);
            printf("\n解密文件操作完成!\n");
            printf("\n按任意键回主菜单…………");
            getchar();
            getchar();
        }
        else if ((c=='s')||(c=='S'))
        {
            savepkey(e,n);
            printf("\n");
            saveskey(d,n);
            printf("\n按任意键回主菜单…………");
            getchar();
            getchar();
        }
        else if((c=='T') || (c=='t'))
        {
            head=input();
            h1=jiami(  e,  n, head);
            jiemi(  d,  n, h1);
            printf("\nRSA测试工作完成!\n");
            printf("\n按任意键回主菜单…………");
            getchar();
        }
        else if ((c=='Q') || (c=='q'))
            break;
    }
}

void test_des()
{
    char key[]={0,2,0,0,9,3,5,1,9,8,0,0,9,1,7},buf[255];
	//char str[]="Welcome to My 3-DES Test! --WangJunchuan\n"
		       //"Northeastern University (www.neu.edu.cn)\n"
			   //"Email: blackdrn@sohu.com";
	char str[]="lihuaihui";

	memset(buf, 0, sizeof(buf));
	strcpy(buf, str);
	puts("\nBefore encrypting");
	puts(buf);
	Des_Go(buf, buf, sizeof(str), key, sizeof(key), ENCRYPT);
	puts("\nAfter encrypting");
	puts(buf);

	Des_Go(buf, buf, sizeof(str), key, sizeof(key), DECRYPT);
	puts("\nAfter decrypting");
	puts(buf);

	//getchar();
}
int main()
{
    test_des();
    return 0;
}
