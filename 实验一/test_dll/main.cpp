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
        m[i]=p[i]=q[i]=n[i]=d[i]=e[i]=0;/*/�򵥳�ʼ��һ��*/

    while (1)
    {
        menu();
        c=getchar();
        getchar();//���ܻس���
        if ((c=='r') || (c=='R'))//����r������Կ��
        {
            for(i=0; i<MAX; i++)
                m[i]=p[i]=q[i]=n[i]=d[i]=e[i]=0;
            printf("\n\n\n\n\n\n\n\n\n");
            printf("\n\n�����Կ�Բ������£�\n\n");
            prime_random(p,q);/*/�����������������*/
            mul(p,q,n);
            printf("�� p��q �ó� n :");
            print(n);
            mov(p,p1);
            p1[0]--;
            mov(q,q1);
            q1[0]--;      /*/q-1;*/
            mul(p1,q1,m);//m=(p-1)*(q-1)
            erand(e,m);
            rsad(e,m,d);
            printf("��Կ�Բ�����ɣ����ڿ���ֱ�ӽ��мӽ����ļ�!\n");
            printf("\n������������˵���������");
            getchar();
        }
        else if((c=='l') || (c=='L'))
        {
            printf("\n\nѡ������Կ���ͣ�������Կ(P)���ǽ�����Կ(S)?");
            c=getchar();
            getchar();
            if ((c=='p')||(c=='P'))
                loadpkey(e,n);
            else if((c=='s')||(c=='S'))
                loadskey(d,n);
            printf("\n������������˵���������");
            getchar();
        }
        else if ((c=='e') || (c=='E'))
        {
            tencrypto( e,  n);
            printf("\n�����ļ��������!\n");
            printf("\n������������˵���������");
            getchar();
            getchar();
        }
        else if ((c=='d') || (c=='D'))
        {
            tdecrypto( d, n);
            printf("\n�����ļ��������!\n");
            printf("\n������������˵���������");
            getchar();
            getchar();
        }
        else if ((c=='s')||(c=='S'))
        {
            savepkey(e,n);
            printf("\n");
            saveskey(d,n);
            printf("\n������������˵���������");
            getchar();
            getchar();
        }
        else if((c=='T') || (c=='t'))
        {
            head=input();
            h1=jiami(  e,  n, head);
            jiemi(  d,  n, h1);
            printf("\nRSA���Թ������!\n");
            printf("\n������������˵���������");
            getchar();
        }
        else if ((c=='Q') || (c=='q'))
            break;
    }
}

void test_des()
{
    //cout << ENCRYPT << endl;
    char key[]= "2342348792840",buf[255];
	//char str[]="Welcome to My 3-DES Test! --WangJunchuan\n"
		       //"Northeastern University (www.neu.edu.cn)\n"
			   //"Email: blackdrn@sohu.com";
	char str[]="chenalong";
    //cout << "hello_world" << endl;
	memset(buf, 0, sizeof(buf));

	strcpy(buf, str);
	puts("\nBefore encrypting");
	puts(buf);
	char result[100];
	//cout << sizeof(str) << endl;
	Des_Go(result, buf,  key, ENCRYPT);

	puts("\nAfter encrypting");
	puts(result);

	Des_Go(buf, result,  key, DECRYPT);
	puts("\nAfter decrypting");
	puts(buf);

	//getchar();
}
int main()
{
    /*
    char contend[] = "27d779cba60f2b5d48debdd970c8e732";
    char ency_file[10000];
    //cout << "start ency" << endl;
    rsa_jia_mi(ency_file,contend);
    char decy_file[1000];
    cout << decy_file <<endl;
    cout << "==============" << endl;
    cout << ency_file << endl;
    cout << "==============" << endl;

    rsa_jie_mi(decy_file,ency_file);
    cout << decy_file << endl;
    //cout << "end ency" << endl;
    //test_rsa();

    //test_rsa();
    */
    test_des();
    return 0;
}
//i am chen along
