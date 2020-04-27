// Setting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testPR.h"
#include "Setting.h"
#include "afxdialogex.h"
#include "LicensePlateRecognise.h"
using namespace LicensePlateRecognise;
using namespace cv;
using namespace std;
using namespace api;


// CSetting �Ի���

IMPLEMENT_DYNAMIC(CSetting, CDialogEx)


CSetting::CSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetting::IDD, pParent)
{
	m_detect_Type = 7;

	m_bLifeMode = true;
}

CSetting::~CSetting()
{
}

void CSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CSetting::OnInitDialog()
{
	if (m_detect_Type & 1)
	{
		((CButton*)GetDlgItem(SOBEL_LOCATE_CHECK))->SetCheck(TRUE);
	}
	if (m_detect_Type & 2)
	{
		((CButton*)GetDlgItem(COLOR_LOCATE_CHECK))->SetCheck(TRUE);
	}
	if (m_detect_Type & 4)
	{
		((CButton*)GetDlgItem(MSER_LOCATE_CHECK))->SetCheck(TRUE);
	}

	
	if (m_bLifeMode)
	{
		((CButton*)GetDlgItem(LIFEMODE_RADIO))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(NO_LIFEMODE_RADIO))->SetCheck(TRUE);
	}
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


BEGIN_MESSAGE_MAP(CSetting, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetting::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetting ��Ϣ�������


void CSetting::OnBnClickedOk()
{
	if (((CButton*)GetDlgItem(MSER_LOCATE_CHECK))->GetCheck())
	{
		((CButton*)GetDlgItem(MSER_LOCATE_CHECK))->SetCheck(TRUE);
		m_detect_Type = m_detect_Type | LicensePlateRecognise::PR_DETECT_CMSER;
	}

	if (((CButton*)GetDlgItem(COLOR_LOCATE_CHECK))->GetCheck())
	{
		
		m_detect_Type = m_detect_Type | LicensePlateRecognise::PR_DETECT_COLOR;
	}

	if (((CButton*)GetDlgItem(SOBEL_LOCATE_CHECK))->GetCheck())
	{
		
		m_detect_Type = m_detect_Type | LicensePlateRecognise::PR_DETECT_SOBEL;
	}


	if (((CButton*)GetDlgItem(NO_LIFEMODE_RADIO))->GetCheck())
	{
		m_bLifeMode = false;
	}
	else
	{
		m_bLifeMode = true;
	}

	CDialogEx::OnOK();
}
