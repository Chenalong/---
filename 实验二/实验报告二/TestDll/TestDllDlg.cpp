// TestDllDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestDll.h"
#include "TestDllDlg.h"
#include "md5.h"
#include "des.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "GfL.h"
#include "../Dll/UKEYDll.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDllDlg dialog

CTestDllDlg::CTestDllDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDllDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDllDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDllDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDllDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDllDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	ON_BN_CLICKED(IDOK2, OnOk2)
	ON_BN_CLICKED(IDOK3, OnOk3)
	ON_BN_CLICKED(IDOK4, OnOk4)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDllDlg message handlers

BOOL CTestDllDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestDllDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestDllDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestDllDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestDllDlg::OnButton1() 
{
	HANDLE hUKEY = Enum_Devices();
	if(hUKEY)
		MessageBox("枚举设备成功", "提示", MB_OK);
	else
		MessageBox("枚举设备失败", "提示", MB_OK);

	CloseHandle(hUKEY);

}



void CTestDllDlg::OnButton2() 
{
	U8 count = 0;
	unsigned char Key_ID[8]={'\0'};
	strcpy((char *)Key_ID, "A2DD6890");

	HANDLE hUKEY = Enum_Devices();

	SetUKEY_ID(hUKEY, Key_ID);
	SetCert_Count(hUKEY, 0);
	MessageBox("初始化成功", "提示", MB_OK);

	CloseHandle(hUKEY);
}


void CTestDllDlg::OnButton3() 
{
	DEVICE_CERT testCert;
	U8 count = 0;

	HANDLE hUKEY = Enum_Devices();
	GetCert_Count(hUKEY, &count);

	strcpy((char *)testCert.PassWord, "6666");
	strcpy((char *)testCert.Host_ID, "0123456");
	AddUKEYCert(hUKEY, 0, &testCert);

	count++;
	SetCert_Count(hUKEY, count);
	CloseHandle(hUKEY);

	MessageBox("添加证书成功", "提示", MB_OK);
}

void CTestDllDlg::OnOK() 
{
	HANDLE hUKEY = Enum_Devices();
	
	U8 PrvKey[24]={0};
	U8 PubKey[48]={0};
	int keyno;
    GenECC_KeyPair(hUKEY, &keyno, PrvKey, PubKey);
    
	U8 buf[16] = {0};
	U8 ecc_sign_buf[100] = {0};

	// 获取文本类容
	
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	if(str.GetLength() > 15)
	{
		MessageBox("加密数据太长,最长是16个字节");
		str.Empty();
		return ;
	}
	
	strcpy((char *)buf, str);
	int str_len = str.GetLength();
	str.Empty();	//不加这一句话提示报错

	
	
	MessageBox((char *)buf, "待签名数据为：", MB_OK);

	ECC_Signature(hUKEY, buf, str_len +1, 0, ecc_sign_buf);

	U8 tmp[97] = {0};  //以前是48 ，返回的数据是48位，所以开成97位，如果小了会验证失败，可能越界把ecc_sign_buf的类容给覆盖了
	CGfL ss;
	ss.ByteToHalfByte((char *)tmp, (char *)ecc_sign_buf, 48);
	ss.HalfByteToStr((char *)tmp, (char *)tmp, 96);
	MessageBox((char *)tmp, "签名结果为：", MB_OK);

	BOOL bRes = ECC_Verify(hUKEY, ecc_sign_buf, str_len +1, PubKey, buf);

	MessageBox((char *)buf, "认证结果为：", MB_OK);

	if(bRes) MessageBox("认证成功", "提示", MB_OK);
	
	CloseHandle(hUKEY);
}

void CTestDllDlg::OnButton4() 
{
	unsigned char hash[20] = {0};
	unsigned char msg[512];
	for(int i=0; i<512; i++) msg[i] = i;

	SHA1(hash, msg, 512);

	U8 tmp[40];
	CGfL ss;
	ss.ByteToHalfByte((char *)tmp, (char *)hash, 20);
	ss.HalfByteToStr((char *)tmp, (char *)tmp, 40);

	MessageBox((char *)tmp, "字串0--511生成的摘要为：", MB_OK);
}

void CTestDllDlg::OnButton5() 
{
	HANDLE hUKEY = Enum_Devices();

    DEVICE_LOG log;
	log.Event_ID = 11;
	strcpy((char *)log.Event_Param, "rjc");

	AddLog(hUKEY, 0, &log);
	MessageBox((char *)log.Event_Param, "添加日志为：", MB_OK);

	CloseHandle(hUKEY);
}


void CTestDllDlg::OnButton10() 
{
	HANDLE hUKEY = Enum_Devices();

	DEVICE_LOG log;
	memset(&log, 0, sizeof(DEVICE_LOG));
	ReadLog(hUKEY, 0, &log);

	MessageBox((char *)log.Event_Param, "读取日志为：", MB_OK);

	CloseHandle(hUKEY);
}


void CTestDllDlg::OnButton11() 
{
	HANDLE hUKEY = Enum_Devices();

	DeleteLog(hUKEY, 0);
	MessageBox("删除日志成功", "提示", MB_OK);

	CloseHandle(hUKEY);
}


void CTestDllDlg::OnButton6() 
{
	unsigned char Key_ID[9]={0x00};

	HANDLE hUKEY = Enum_Devices();

	GetUKEY_ID(hUKEY, Key_ID);

	Key_ID[9] = '\0';
	MessageBox((char *)Key_ID, "设备序列号为：", MB_OK);

	CloseHandle(hUKEY);
}


void CTestDllDlg::OnButton7() 
{
	U8 count = 0;

	HANDLE hUKEY = Enum_Devices();

	GetCert_Count(hUKEY, &count);

	CString tmp;
	tmp.Format("证书数目为：%d\n", count);
	MessageBox(tmp,"提示", MB_OK);

	CloseHandle(hUKEY);	
}

void CTestDllDlg::OnButton8() 
{
	HANDLE hUKEY = Enum_Devices();

    U8 count = 0;
	DEVICE_CERT  testCert;
	memset(&testCert, 0, sizeof(DEVICE_CERT));

	GetCert_Count(hUKEY, &count);
	
	CString info;
	for(int i=0; i<count; i++)
	{
		ReadUKEYCert(hUKEY, i, &testCert);
		info.Format("读出证书%d成功\n", i);
		MessageBox(info, "提示", MB_OK);
	}

	CloseHandle(hUKEY);	
}

// 产生随机数
void CTestDllDlg::OnButton9() 
{
	HANDLE hUKEY = Enum_Devices();

	U8 buf[16] = {0};
	GetRNG(hUKEY, buf, 16);	

	U8 tmp[32];
	CGfL ss;
	ss.ByteToHalfByte((char *)tmp, (char *)buf, 16);
	ss.HalfByteToStr((char *)tmp, (char *)tmp, 32);

	MessageBox((char *)tmp, "产生的随机数为：", MB_OK);

	CloseHandle(hUKEY);	
}

void CTestDllDlg::OnButton12() 
{
	HANDLE hUKEY = Enum_Devices();

	U8 PrvKey[24]={0};
	U8 PubKey[48]={0};
	int keyno;
    GenECC_KeyPair(hUKEY, &keyno, PrvKey, PubKey);

	U8 tmp[96];
	CGfL ss;
	ss.ByteToHalfByte((char *)tmp, (char *)PubKey, 48);
	ss.HalfByteToStr((char *)tmp, (char *)tmp, 96);
	MessageBox((char *)tmp, "产生公钥为：", MB_OK);

	CloseHandle(hUKEY);	
}

void CTestDllDlg::OnButton13() 
{
	HANDLE hUKEY = Enum_Devices();

	ECC_KEYS EccKeys;
	memset(EccKeys.PrvKey, 0x38, 24);
	memset(EccKeys.PubKey, 0x36, 48);

	RestoreECC_Key(hUKEY, 0, &EccKeys);
	memset(&EccKeys, 0, sizeof(ECC_KEYS));
	ReadECC_Key(hUKEY, 0, &EccKeys);	

	MessageBox("导入密钥对成功", "提示", MB_OK);
	
	CloseHandle(hUKEY);	
}




void CTestDllDlg::OnButton15() 
{
	HANDLE hUKEY = Enum_Devices();
    EnableBoot(hUKEY);	
}

void CTestDllDlg::OnOk2() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	MessageBox(str);
	
}

void CTestDllDlg::Onddd() 
{
	// TODO: Add your control notification handler code here
	
}

void CTestDllDlg::OnOnButton14() 
{
	// TODO: Add your control notification handler code here
	HANDLE hUKEY = Enum_Devices();
	
	U8 PrvKey[24]={0};
	U8 PubKey[48]={0};
	int keyno;
    GenECC_KeyPair(hUKEY, &keyno, PrvKey, PubKey);
    
	U8 buf[16] = {0};
	U8 ecc_encrypt_buf[100] = {0};

	// 获取文本类容
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	if(str.GetLength() > 15)
	{
		MessageBox("加密数据太长,最长是16个字节");
		str.Empty();
		return ;
	}
	//MessageBox(str);
	//str.Empty();
	//str.ReleaseBuffer();
	//printf("%d\n",str.GetLength());
	strcpy((char *)buf, str);
	int str_len = str.GetLength();
	str.Empty();	//不加这一句话提示报错
	//if(str.GetLength() <10)
	//{
	//	MessageBox("加密类容太短，长度必修大于10字节");
	//	return ;
	//}

	MessageBox((char *)buf, "明文为：", MB_OK);

	ECC_Encrypt(hUKEY, buf, str_len +1, PubKey, ecc_encrypt_buf);

	U8 tmp[72];
	CGfL ss;
	ss.ByteToHalfByte((char *)tmp, (char *)ecc_encrypt_buf, 36);
	ss.HalfByteToStr((char *)tmp, (char *)tmp, 72);
	MessageBox((char *)tmp, "加密为：", MB_OK);

	memset(buf, 0, 16);
	ECC_Decrypt(hUKEY, ecc_encrypt_buf, str_len + 1, 0, buf);

	MessageBox((char *)buf, "解密为：", MB_OK);
	
	CloseHandle(hUKEY);
}

void ichange(unsigned char *input,unsigned char *normal)
{
	int i = 0;
	for(;input[i];i++)
	{
		normal[i*2] = input[i] / 16;
		normal[i * 2 + 1] = input[i] % 16;
	}
	normal[i*2] = 0;
	int j = i * 2;
	for(i = 0;i<j;i++)
	{
		if(normal[i] <10)
			normal[i] = normal[i] + '0';
		else
			normal[i] = normal[i] + 'a' - 10;
	}
}
// MD5值的计算
void CTestDllDlg::OnOk3() 
{
	HANDLE hUKEY = Enum_Devices();
	if(!hUKEY)
	{
		MessageBox("连接设备失败", "提示", MB_OK);
		return ;
	}
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	
	unsigned char encrypt[100],decrypt[100];
	strcpy((char*)encrypt,str);
	MessageBox((char *)encrypt, "类容为", MB_OK);
	str.Empty();
	
	int rec = MD5_Decrypt(hUKEY,encrypt,decrypt);
	//if(rec)
		//MessageBox("TRUE", "返回值", MB_OK);
	//else
	//	MessageBox("FALSE", "返回值", MB_OK);
	

	//MessageBox((char *)encrypt, "计算hash值为", MB_OK);
	//Cal_MD5(encrypt,decrypt);
	unsigned char normal[100];
	//CGfL ss;
	//ss.ByteToHalfByte((char *)normal, (char *)decrypt, 16);
	//ss.HalfByteToStr((char *)normal, (char *)normal, 72);
	
	
	ichange(decrypt,normal);
	MessageBox((char*)normal, "计算hash值为", MB_OK);
}

// DES加密

void CTestDllDlg::OnOk4() 

{
	// TODO: Add your control notification handler code here
	//=================================产生随机数========================================
	HANDLE hUKEY = Enum_Devices();
	U8 buf[16] = {0};
	GetRNG(hUKEY, buf, 16);	

	U8 tmp[32];
	CGfL ss;
	ss.ByteToHalfByte((char *)tmp, (char *)buf, 16);
	ss.HalfByteToStr((char *)tmp, (char *)tmp, 32);
	MessageBox((char *)tmp, "产生的随机数为：", MB_OK);
	//=================================产生随机数========================================
	
	char key[8];
	memcpy(key,tmp,8);

	//获取文本框类容
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	char encrypt[100],decrypt[100],result[100];
	strcpy(encrypt,str);
	DES_Encrypt(encrypt,key,decrypt);
	MessageBox((char *)decrypt, "DES加密的值", MB_OK);
	DES_Decrypt(decrypt,key,result);
	MessageBox((char *)result, "DES解密的值", MB_OK);

	CloseHandle(hUKEY);	
	
}

void CTestDllDlg::OnButton14() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	HANDLE hUKEY = Enum_Devices();
	
	U8 PrvKey[24]={0};
	U8 PubKey[48]={0};
	int keyno;
    GenECC_KeyPair(hUKEY, &keyno, PrvKey, PubKey);
    
	U8 buf[16] = {0};
	U8 ecc_encrypt_buf[100] = {0};

	// 获取文本类容
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	if(str.GetLength() > 15)
	{
		MessageBox("加密数据太长,最长是16个字节");
		str.Empty();
		return ;
	}
	//MessageBox(str);
	//str.Empty();
	//str.ReleaseBuffer();
	//printf("%d\n",str.GetLength());
	strcpy((char *)buf, str);
	int str_len = str.GetLength();
	str.Empty();	//不加这一句话提示报错
	//if(str.GetLength() <10)
	//{
	//	MessageBox("加密类容太短，长度必修大于10字节");
	//	return ;
	//}

	MessageBox((char *)buf, "明文为：", MB_OK);

	ECC_Encrypt(hUKEY, buf, str_len +1, PubKey, ecc_encrypt_buf);

	U8 tmp[72];
	CGfL ss;
	ss.ByteToHalfByte((char *)tmp, (char *)ecc_encrypt_buf, 36);
	ss.HalfByteToStr((char *)tmp, (char *)tmp, 72);
	MessageBox((char *)tmp, "加密为：", MB_OK);

	memset(buf, 0, 16);
	ECC_Decrypt(hUKEY, ecc_encrypt_buf, str_len + 1, 0, buf);

	MessageBox((char *)buf, "解密为：", MB_OK);
	
	CloseHandle(hUKEY);
}
