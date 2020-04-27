// singleTest.cpp : 实现文件
//


#include "stdafx.h"
#include "testPR.h"
#include "SingleTest.h"
#include "afxdialogex.h"
#include <atlconv.h>
#include "resource.h"

#include "Test.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestPRDlg 对话框



CSingleTest::CSingleTest(CWnd* pParent /*=NULL*/)
: CDialogEx(CSingleTest::IDD, pParent)
{
	
}

void CSingleTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSingleTest, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CSingleTest::OnBnClickedButton1)
	ON_BN_CLICKED(Result_BUTTON, &CSingleTest::OnBnClickedButton)
	ON_BN_CLICKED(PROCESS_BUTTON, &CSingleTest::OnBnClickedProcessButton)
END_MESSAGE_MAP()


// CtestPRDlg 消息处理程序
BOOL CSingleTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetClientRect(&m_dlgRect);

	namedWindow("view", 1);
	HWND hWnd = (HWND)cvGetWindowHandle("view");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(SRC_STATIC)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	::ShowWindow(hWnd, SW_HIDE);
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSingleTest::OnPaint()
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
		if (m_srcPath != "")
		{
			IplImage* pShowImg = resizeImage();

			DrawPicToHDC(pShowImg, SRC_STATIC);
		}
	}
}


void CSingleTest::OnBnClickedButton1()
{
	CFileDialog openfile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL);
	INT_PTR result = openfile.DoModal();

	if (result == IDOK)
	{		

		CString tmpSrcPath = openfile.GetPathName();

		USES_CONVERSION;
		string tmps_srcPath(W2A(tmpSrcPath));
		m_srcPath = tmps_srcPath;

		IplImage* pShowImg = resizeImage();

		DrawPicToHDC(pShowImg, SRC_STATIC);

	}
}

void CSingleTest::ShowIplImage(IplImage* img) {
	CDC* pDC = GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	rect.SetRect(0, 0, img->width, img->height);
	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
}

IplImage* CSingleTest::resizeImage()
{
	IplImage* pSrcImage = cvLoadImage(m_srcPath.c_str(), CV_LOAD_IMAGE_UNCHANGED);
	IplImage* pDstImage = NULL;

	CRect rectPC;
	GetDlgItem(SRC_STATIC)->GetWindowRect(&rectPC);

	double fWidth = (double)(rectPC.Width()) / (pSrcImage->width);
	double fHeight = (double)(rectPC.Height()) / (pSrcImage->height);

	if ((fWidth < 1) || (fHeight < 1))
	{
		double fScale = (fWidth > fHeight) ? fHeight:fWidth;

		CvSize czSize;
		czSize.width = pSrcImage->width * fScale;
		czSize.height = pSrcImage->height * fScale;
		pDstImage = cvCreateImage(czSize, pSrcImage->depth, pSrcImage->nChannels);
		cvResize(pSrcImage, pDstImage, CV_INTER_AREA);
	}
	else
	{
		pDstImage = pSrcImage;
	}

	return pDstImage;
}

void CSingleTest::DrawPicToHDC(IplImage* iplimg, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(iplimg);
	cimg.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
}

void CSingleTest::ChangeSize(UINT nID, int x, int y)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(nID);
	if (pWnd != NULL)
	{
		CRect rec;

		pWnd->GetWindowRect(&rec);
		ScreenToClient(&rec);   

		rec.left = rec.left;  
		rec.top = rec.top;
		rec.bottom = rec.bottom + y;
		rec.right = rec.right + x;
		pWnd->MoveWindow(rec);   
	}
}

int CSingleTest::test_plate_locate()
{
	cv::Mat src = imread(m_srcPath);

	vector<cv::Mat> resultVec;
	CPlateLocate plate;

	int result = plate.plateLocate(src, resultVec);
	size_t num = resultVec.size();
	if ((result == 0) && (num > 0))
	{
		cv::Mat resultMat,comMat;
		
		size_t j = 1;
		comMat = resultVec[0];
		while (j < num)
		{
			resultMat = resultVec[j];
			vconcat(comMat, resultMat, comMat);
			j++;
		}
		if ((comMat.rows > 0) && (comMat.cols > 0))
		{
			IplImage *img_src = (IplImage *)&IplImage(comMat);
			DrawPicToHDC(img_src, Locate_Img);
		}
		
	}

	return result;
}

int CSingleTest::test_plate_judge()
{
	cv::Mat src = imread(m_srcPath);

	vector<cv::Mat> matVec;

	vector<cv::Mat> resultVec;

	CPlateLocate lo;
	lo.setDebug(1);
	int resultLo = lo.plateLocate(src, matVec);

	if (0 != resultLo) return -1;

	int resultJu = PlateJudge::instance()->plateJudge(matVec, resultVec);

	if (0 != resultJu) return -1;

	cv::Mat resultMat, comMat;
	size_t num = resultVec.size();

	if (num > 0)
	{

		size_t j = 1;

		comMat = resultVec[0];
		while (j < num)
		{
			resultMat = resultVec[j];
			vconcat(comMat, resultMat, comMat);
			j++;
		}
		if ((comMat.rows > 0) && (comMat.cols > 0))
		{
			IplImage *img_src = (IplImage *)&IplImage(comMat);
			DrawPicToHDC(img_src, Judge_Img);
		}
	}

	return resultJu;
}

int CSingleTest::test_plate_detect()
{
	cv::Mat src = imread(m_srcPath);

	vector<CPlate> resultVec;

	int result = m_single_pd.plateDetect(src, resultVec);
	size_t num = resultVec.size();

	if ((0 == result) && (num > 0))
	{
		cv::Mat resultMat, comMat;
		size_t j = 1;
		comMat = resultVec[0].getPlateMat();
		while (j < num)
		{
			resultMat = resultVec[j].getPlateMat();
			vconcat(comMat, resultMat, comMat);
			j++;
		}
		if ((comMat.rows > 0) && (comMat.cols > 0))
		{
			IplImage *img_src = (IplImage *)&IplImage(comMat);
			DrawPicToHDC(img_src, Detect_Img);
		}
	}

	return result;
}

int CSingleTest::test_chars_segment() {

	cv::Mat src = cv::imread(m_srcPath);

	std::vector<cv::Mat> resultVec;
	vector<CPlate> plateVec;
	CCharsSegment plate;
	int result = 1;


	int resultPD = m_single_pd.plateDetect(src, plateVec);

	if (resultPD != 0)
	{
		return resultPD;
	}

	size_t numPlate = plateVec.size();

	for (size_t j = 0; j < numPlate; j++)
	{
		CPlate item = plateVec.at(j);

		Mat plateMat = item.getPlateMat();

		result = plate.charsSegment(plateMat, resultVec);

		if (result == 0) {

			size_t num = resultVec.size();
			size_t i = 0;
			while (i < num)
			{
				IplImage *img_src = (IplImage *)&IplImage(resultVec[i++]);
				
				switch (i) {
				case 1:
					DrawPicToHDC(img_src, Char_Img_1);
					break;
				case 2:
					DrawPicToHDC(img_src, Char_Img_2);
					break;
				case 3:
					DrawPicToHDC(img_src, Char_Img_3);
					break;
				case 4:
					DrawPicToHDC(img_src, Char_Img_4);
					break;
				case 5:
					DrawPicToHDC(img_src, Char_Img_5);
					break;
				case 6:
					DrawPicToHDC(img_src, Char_Img_6);
					break;
				case 7:
					DrawPicToHDC(img_src, Char_Img_7);
					break;
				}
			}
		}
	}

	return result;
}


int CSingleTest::test_gray_Binary()
{
	cv::Mat src = cv::imread(m_srcPath);

	std::vector<cv::Mat> resultVec;
	vector<CPlate> plateVec;
	CCharsSegment plate;
	int result = 1;
	cv::Mat grayMat, binaMat, comHMat, comVMat;
	

	int resultPD = m_single_pd.plateDetect(src, plateVec);

	if (resultPD != 0)
	{
		return resultPD;
	}

	size_t numPlate = plateVec.size();

	for (size_t j = 0; j < numPlate; j++)
	{
		CPlate item = plateVec.at(j);

		Mat plateMat = item.getPlateMat();

		cvtColor(plateMat, grayMat, CV_BGR2GRAY);

		Color color;
		if (item.getPlateLocateType() == CMSER) {
			color = item.getPlateColor();
		}
		else {
			int w = plateMat.cols;
			int h = plateMat.rows;
			Mat tmpMat = plateMat(Rect_<double>(w * 0.1, h * 0.1, w * 0.8, h * 0.8));
			color = getPlateType(tmpMat, true);
		}
		binaMat = grayMat.clone();
		spatial_ostu(binaMat, 8, 2, color);

		hconcat(grayMat, binaMat, comHMat);

		
		if (j == 0)
		{
			comVMat = comHMat;
		}
		else
		{
			IplImage *img_src = (IplImage *)&IplImage(comVMat);
			DrawPicToHDC(img_src, GB_Img);
		}

	}

	if ((comVMat.rows > 0) && (comVMat.cols > 0))
	{
		IplImage *img_src = (IplImage *)&IplImage(comVMat);
		DrawPicToHDC(img_src, GB_Img);
	}


	return result;
}



void CSingleTest::OnBnClickedProcessButton()
{
		test_plate_locate();
		test_plate_judge();
		test_plate_detect();
		test_gray_Binary();
		test_chars_segment();
}

void CSingleTest::OnBnClickedButton()
{
	vector<CPlate> plateVec;
	Mat src = imread(m_srcPath);
	int result = m_single_pr.plateRecognize(src, plateVec);

	if (result == 0)
	{
		size_t num = plateVec.size();

		std::string single_result[10];
		std::string std_resultStr;
		CString resultStr;

		for (size_t i = 0; i < num; i++)
		{
			single_result[i] += plateVec[i].getPlateStr();
		}


		
		int num_of_plate = 0;
		for (int i = 0; i < num; ++i)
		{
			if (single_result[i].length() >= 8)
			{
				single_result[num_of_plate++] == single_result[i];
			}
		}

		
		for (int i = 0; i < 13; ++i) 
		{
			char single_character[10];
			int count_character_number[10];
			int number_of_character;
			bool is_add = false;
			single_character[0] = single_result[0][i];
			count_character_number[1] = 1;
			number_of_character = 1;
			for (int j = 1; j < num_of_plate; j++) 
			{
				char temp = single_result[j][i];
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
			std_resultStr += single_character[max_character_location];
		}
		resultStr = std_resultStr.c_str();


		SetDlgItemText(RESULT_EDIT2, resultStr);
	}
}
