// TrainTest.cpp : 实现文件
//

#include "stdafx.h"
#include "test.h"
#include "TrainTest.h"
#include "afxdialogex.h"
#include "LicensePlateRecognise.h"
#include <atlbase.h>

// TrainTest 对话框

IMPLEMENT_DYNAMIC(TrainTest, CDialogEx)

TrainTest::TrainTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(TrainTest::IDD, pParent)
{

}

TrainTest::~TrainTest()
{
}

void TrainTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TrainTest, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &TrainTest::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &TrainTest::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &TrainTest::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &TrainTest::OnBnClickedButton4)
END_MESSAGE_MAP()


// TrainTest 消息处理程序


void TrainTest::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码

	AllocConsole();
	SetConsoleTitle(_T("SVM训练"));
	freopen("CONOUT$", "w+t", stdout);

	std::cout << "Run \"train svm\" for more usage." << std::endl;
	CString m_savepath = m_folderPath + "/svm.xml";
	
	USES_CONVERSION;
	char *folderPath = T2A(m_folderPath);
	char *savepath = T2A(m_savepath);
	
	LicensePlateRecognise::SvmTrain svm(folderPath, savepath);
	svm.train();

	cout << "Please close the window!" << endl;
	FreeConsole();
}


void TrainTest::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码

	AllocConsole();
	SetConsoleTitle(_T("ANN训练"));
	freopen("CONOUT$", "w+t", stdout);

	std::cout << "Run \"train ann\" for more usage." << std::endl;
	CString m_savepath = m_folderPath + "/ann.xml";

	USES_CONVERSION;
	char *folderPath = T2A(m_folderPath);
	char *savepath = T2A(m_savepath);

	LicensePlateRecognise::AnnTrain ann(folderPath, savepath);
	ann.train();

	cout << "Please close the window!" << endl;
	FreeConsole();

}


void TrainTest::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码

	AllocConsole();
	SetConsoleTitle(_T("ANNCH训练"));
	freopen("CONOUT$", "w+t", stdout);
	freopen("CONIN$", "r", stdin);

	std::cout << "Run \"train annCh\" for more usage." << std::endl;
	CString m_savepath = m_folderPath + "/annCh.xml";
	USES_CONVERSION;
	char *folderPath = T2A(m_folderPath);
	char *savepath = T2A(m_savepath);

	std::shared_ptr<LicensePlateRecognise::Kv> kv(new LicensePlateRecognise::Kv);
	kv->load("resources/text/chinese_mapping");

	bool isExit = false;
	while (!isExit) {
		LicensePlateRecognise::Utils::print_file_lines("resources/text/train_menu");
		std::cout << kv->get("make_a_choice") << ":";

		int select = -1;
		cin >> select;
		switch (select) {
		case 1:
		{
			LicensePlateRecognise::AnnChTrain ann(folderPath, savepath);
			ann.setNumberForCount(100);
			ann.train();
		}
			break;
		case 2:
		{
			LicensePlateRecognise::AnnChTrain ann(folderPath, savepath);
			ann.setNumberForCount(350);
			ann.train();
		}
			break;
		case 3:
		{
			LicensePlateRecognise::AnnChTrain ann(folderPath, savepath);
			ann.setNumberForCount(700);
			ann.train();
		}
			break;
		case 4:
		{
			LicensePlateRecognise::AnnChTrain ann(folderPath, savepath);
			ann.setNumberForCount(1000);
			ann.train();
		}
			break;
		case 5:
		{
			LicensePlateRecognise::AnnChTrain ann(folderPath, savepath);
			ann.setNumberForCount(1500);
			ann.train();
		}
		case 6:
			isExit = true;
			break;
		default:
			std::cout << kv->get("input_error") << ":";
			break;
		}
	}

	cout << "Please close the window!" << endl;
	FreeConsole();
}


void TrainTest::OnBnClickedButton4()
{
	// TODO:  在此添加控件通知处理程序代码
	TCHAR           szFolderPath[MAX_PATH] = { 0 };
	CString         strFolderPath = TEXT("");

	BROWSEINFO      sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T("请选择文件夹:");
	sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
	sInfo.lpfn = NULL;

	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			m_folderPath = szFolderPath;
			GetDlgItem(TRAIN_PATH_EDIT)->SetWindowTextW(m_folderPath);
		}
	}
	if (lpidlBrowse != NULL)
	{
		::CoTaskMemFree(lpidlBrowse);
	}
}
