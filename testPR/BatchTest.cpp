// BatchTest.cpp : 实现文件
//

#include "stdafx.h"
#include "testPR.h"
#include "BatchTest.h"
#include "afxdialogex.h"

// CBatchTest 对话框

IMPLEMENT_DYNAMIC(CBatchTest, CDialogEx)

CBatchTest::CBatchTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBatchTest::IDD, pParent)
{

}

CBatchTest::~CBatchTest()
{
}

void CBatchTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBatchTest, CDialogEx)
	ON_BN_CLICKED(BATCH_CHOOSE_BTN, &CBatchTest::OnBnClickedChooseBtn)
	ON_BN_CLICKED(BATCH_START_BTN, &CBatchTest::OnBnClickedStartBtn)
END_MESSAGE_MAP()

BOOL CBatchTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_BatchList = ((CListCtrl*)GetDlgItem(BTACH_SHOW_LIST));

	DWORD dwStyle = ::GetWindowLong(m_BatchList->m_hWnd, GWL_STYLE);
	dwStyle &= ~(LVS_TYPEMASK);
	dwStyle &= ~(LVS_EDITLABELS);

	SetWindowLong(m_BatchList->m_hWnd, GWL_STYLE, dwStyle | LVS_REPORT | LVS_NOLABELWRAP | LVS_SHOWSELALWAYS);

	DWORD styles = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyleEx(m_BatchList->m_hWnd, styles, styles);

	m_BatchList->SetExtendedStyle(m_BatchList->GetExtendedStyle() | LVS_EX_SUBITEMIMAGES);

	m_BatchList->DeleteAllItems();
	m_BatchList->InsertColumn(0, _T("文件名"));
	m_BatchList->InsertColumn(1, _T("识别结果"));
	m_BatchList->InsertColumn(2, _T("车牌检测用时(ms)"));
	m_BatchList->InsertColumn(3, _T("字符识别用时(ms)"));
	m_BatchList->InsertColumn(4, _T("是否正确"));
	m_BatchList->SetColumnWidth(0, 130);
	m_BatchList->SetColumnWidth(1, 130);
	m_BatchList->SetColumnWidth(2, 130);
	m_BatchList->SetColumnWidth(3, 130);
	m_BatchList->SetColumnWidth(4, 130);

	return TRUE;
}
// CBatchTest 消息处理程序


void CBatchTest::OnBnClickedChooseBtn()
{
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
			GetDlgItem(BATCH_PATH_EDIT)->SetWindowTextW(m_folderPath);
		}
	}
	if (lpidlBrowse != NULL)
	{
		::CoTaskMemFree(lpidlBrowse);
	}
}


void CBatchTest::OnBnClickedStartBtn()
{
	if (m_folderPath.IsEmpty())
	{
		return;
	}

	CPaintDC paintDC(this);

	CFileFind fileFinder;
	CString strPicFile = TEXT("");
	CString strFilePath;
	CString strFileName; 
	int nIndex = 0;
	m_BatchList->DeleteAllItems();

	char buffer[260] = { 0 };

	USES_CONVERSION;
	string tmps_srcPath(W2A(m_folderPath));
	vector<string> imgPathTrain = utils::getFiles(tmps_srcPath.c_str());
	int fileNum = imgPathTrain.size();


	CProgressCtrl myProCtrl;


	CRect rect, proRect;
	GetClientRect(&rect);
	proRect.left = rect.left + rect.Width() / 2 - 100;
	proRect.top = rect.top + rect.Height() / 2 - 20;
	proRect.right = rect.right - rect.Width() / 2 + 100;
	proRect.bottom = rect.bottom - rect.Height() / 2 + 20;

	myProCtrl.Create(WS_VISIBLE, proRect, this, 99);
	myProCtrl.SetRange(0, fileNum);
	countRight = 0;

	for (auto file : imgPathTrain)
	{
		auto image = cv::imread(file);
		if (!image.data) {
			fprintf(stdout, ">> Invalid image: %s  ignore.\n", file.c_str());
			continue;
		}

		strFileName = (utils::getFileName(file)).c_str();
		

		processPlate(image, strFileName);

		m_BatchList->InsertItem(nIndex, strFileName);
		m_BatchList->SetItemText(nIndex, 1, m_plateResult);
		m_BatchList->SetItemText(nIndex, 2, m_pdTime);
		m_BatchList->SetItemText(nIndex, 3, m_crTime);
		m_BatchList->SetItemText(nIndex, 4, m_istrue);
		nIndex++;

		myProCtrl.OffsetPos(1);
		
		double percentage = (double)countRight / ((double)nIndex);
		CString str;
		str.Format(_T("%.2f%%"), (percentage*100.0));
		GetDlgItem(SHOW_PERCENTAGE)->SetWindowTextW(str);
	}
}


void CBatchTest::processPlate(Mat& src, CString strFileName)
{
	LARGE_INTEGER nFreq;
	LARGE_INTEGER start, end;
	double eslapsPD;
	double eslapsCR;


	std::vector<CPlate> plateVec;

	CCharsRecognise cr;

	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&start);

	int resultPD = m_batch_pd.plateDetect(src, plateVec);


	QueryPerformanceCounter(&end);
	eslapsPD = (double)(end.QuadPart - start.QuadPart) / (double)nFreq.QuadPart;
	size_t num = plateVec.size();

	if ((resultPD == 0) && (num > 0))
	{		
		int index = 0;
		std::string single_plate_result[10];
		std::string single_color_result[10];
		double single_eslapsCR[10];
		std::string plateIdentify = "";

		for (size_t j = 0; j < num; j++)
		{

		    CString plateIndex = _T("");
			plateIndex.Format(_T("%d"), j);

			CPlate item = plateVec.at(j);
			Mat plateMat = item.getPlateMat();

			Color color = item.getPlateColor();
			if (color == UNKNOWN)
			{
				color = getPlateType(plateMat, true);
				item.setPlateColor(color);
			}

			single_color_result[j] = cr.getPlateColor(color);

			QueryPerformanceFrequency(&nFreq);
			QueryPerformanceCounter(&start);

			cr.charsRecognise(item, single_plate_result[j]);

			QueryPerformanceCounter(&end);
			single_eslapsCR[j] = (double)(end.QuadPart - start.QuadPart) / (double)nFreq.QuadPart;
		
		}

		int num_of_plate = 0;
		for (int i = 0; i < num; ++i)
		{
			if (single_plate_result[i].length() >= 8)
			{
				single_plate_result[num_of_plate++] == single_plate_result[i];
			}
		}
		
		if (num_of_plate != 0)
		{
			for (int i = 0; i < 8 && num_of_plate != 0; ++i)
			{
				char single_character[10];
				int count_character_number[10];
				int number_of_character;
				bool is_add = false;
				single_character[0] = single_plate_result[0][i];
				count_character_number[1] = 1;
				number_of_character = 1;
				for (int j = 1; j < num_of_plate; j++)
				{
					char temp = single_plate_result[j][i];
					for (int k = 0; k < number_of_character; ++k)
					{
						if (temp == single_character[k]) ++count_character_number[k];
						is_add = true;
						break;
					}
					if (is_add == false)
					{
						single_character[number_of_character] = temp;
						count_character_number[number_of_character++] = 1;
						is_add = false;
					}
				}

				int max_character_location = 0;
				for (int j = 0; j < number_of_character; ++j)
					if (count_character_number[j] > count_character_number[max_character_location])
						max_character_location = j;
				plateIdentify += single_character[max_character_location];
			}
		}

		CString C_pLateIdentify;
		C_pLateIdentify = plateIdentify.c_str();
		if (C_pLateIdentify == strFileName) {
			m_istrue = "正确";
			++countRight;
		}
		else m_istrue = "错误";

		int num_of_color = 0;
		for (int i = 0; i < num; ++i)
		{
			if (single_plate_result[i] != "")
			{
				single_plate_result[num_of_color++] == single_plate_result[i];
			}
		}

		int count_each_color[4] = { 0 };
		for (int i = 0; i < num_of_color; ++i)
		{
			if (single_color_result[i] == "蓝牌") ++count_each_color[0];
			else if (single_color_result[i] == "黄牌") ++count_each_color[1];
			else if (single_color_result[i] == "白牌") ++count_each_color[2];
			else if (single_color_result[i] == "未知") ++count_each_color[3];
		}
		int max_color = 0;
		for (int i = 1; i < 4; ++i)
			if (count_each_color[i] > count_each_color[max_color])
				max_color = i;
		switch (max_color)
		{
		case 0:
			plateIdentify = "蓝牌：" + plateIdentify;
			break;
		case 1:
			plateIdentify = "黄牌：" + plateIdentify;
			break;
		case 2:
			plateIdentify = "白牌：" + plateIdentify;
			break;
		case 3:
			plateIdentify = "未知：" + plateIdentify;
			break;
		}
		double eslapsCR = 0;
		for (int j = 0; j < num; ++j)
			eslapsCR += single_eslapsCR[j];
		eslapsCR /= (double)num;

		m_pdTime.Format(_T("%.3f"), eslapsPD * 1000);
		m_crTime.Format(_T("%.3f"), eslapsCR * 1000);
		m_plateResult = plateIdentify.c_str();
		
	}
}
