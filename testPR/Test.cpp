// Test.cpp : 实现文件
//

#include "stdafx.h"
#include "testPR.h"
#include "Test.h"
#include "afxdialogex.h"
#include "LicensePlateRecognise.h"
using namespace LicensePlateRecognise;
using namespace cv;
using namespace std;
using namespace api;

CTest *CTest::m_testDlg = NULL;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// Test 对话框

IMPLEMENT_DYNAMIC(CTest, CDialogEx)

CTest::CTest(CWnd* pParent /*=NULL*/)
: CDialogEx(CTest::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BOOL CTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);


	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_testDlg = this;
	m_SettingDlg = new CSetting();
	m_batchTestDlg = new CBatchTest();
	m_singleTestDlg = new CSingleTest();
	m_trainTestDlg = new TrainTest();
	m_batchTestDlg->Create(IDD_TESTPR_BATCH, GetDlgItem(IDC_LPRTAB));
	m_singleTestDlg->Create(IDD_TESTPR_SINGLE, GetDlgItem(IDC_LPRTAB));
	m_trainTestDlg->Create(IDD_TRAIN_TEST, GetDlgItem(IDC_LPRTAB));

	m_LicensePlateRecogniseTab = ((CTabCtrl*)GetDlgItem(IDC_LPRTAB));
	m_LicensePlateRecogniseTab->InsertItem(0,_T("单张检测"));
	m_LicensePlateRecogniseTab->InsertItem(1, _T("批量检测"));
	m_LicensePlateRecogniseTab->InsertItem(2, _T("训练网络"));
	CRect rs;
	m_LicensePlateRecogniseTab->GetClientRect(&rs);

	rs.top += 25;
	rs.bottom -= 5;
	rs.left += 5;
	rs.right -= 5;

	m_batchTestDlg->MoveWindow(&rs);
	m_singleTestDlg->MoveWindow(&rs);
	m_trainTestDlg->MoveWindow(&rs);

	m_trainTestDlg->ShowWindow(false);
	m_batchTestDlg->ShowWindow(false);
	m_singleTestDlg->ShowWindow(true);

	m_LicensePlateRecogniseTab->SetCurSel(0);

    m_singleTestDlg->m_single_pd.setDetectType(PR_DETECT_CMSER);

	m_singleTestDlg->m_single_pd.setPDLifemode(false);

	m_singleTestDlg->m_single_pr.setDetectType(PR_DETECT_CMSER);

	m_singleTestDlg->m_single_pr.setPDLifemode(false);

	m_batchTestDlg->m_batch_pd.setDetectType(PR_DETECT_CMSER);

	m_batchTestDlg->m_batch_pd.setPDLifemode(false);

	return TRUE;
}

CTest::~CTest()
{
}

void CTest::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTest::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTest::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTest, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_LPRTAB, &CTest::OnTcnSelchangeEasyprtab)
	ON_COMMAND(ID_SETTING_PATAMETERSETTING, &CTest::OnSettingPatametersetting)
	ON_COMMAND(ID_ABOUT_VERSION, &CTest::OnAboutVersion)
END_MESSAGE_MAP()

// Test 消息处理程序

void CTest::OnTcnSelchangeEasyprtab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	int curSel = m_LicensePlateRecogniseTab->GetCurSel();
	switch (curSel)
	{
	case 0:
		m_batchTestDlg->ShowWindow(false);
		m_singleTestDlg->ShowWindow(true);
		m_trainTestDlg->ShowWindow(false);
		break;

	case 1:
		m_batchTestDlg->ShowWindow(true);
		m_singleTestDlg->ShowWindow(false);
		m_trainTestDlg->ShowWindow(false);
		break;

	case 2:
		m_batchTestDlg->ShowWindow(false);
		m_singleTestDlg->ShowWindow(false);
		m_trainTestDlg->ShowWindow(true);
		break;

	default:
		break;
	}

	*pResult = 0;
}


void CTest::OnSettingPatametersetting()
{

	if (m_SettingDlg->DoModal() == IDOK)
	{
		m_singleTestDlg->m_single_pd.setDetectType(m_SettingDlg->m_detect_Type);
		m_singleTestDlg->m_single_pd.setPDLifemode(m_SettingDlg->m_bLifeMode);
		m_singleTestDlg->m_single_pr.setDetectType(m_SettingDlg->m_detect_Type);
		m_singleTestDlg->m_single_pr.setPDLifemode(m_bLifeMode);

		m_batchTestDlg->m_batch_pd.setDetectType(m_SettingDlg->m_detect_Type);
		m_batchTestDlg->m_batch_pd.setPDLifemode(m_SettingDlg->m_bLifeMode);
	}
}


void CTest::OnAboutVersion()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
