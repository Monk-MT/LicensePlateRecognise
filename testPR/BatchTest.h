#pragma once
#include <string>
#include"Resource.h"
#include "LicensePlateRecognise.h"

using namespace LicensePlateRecognise;
using namespace cv;
using namespace std;
using namespace api;

// CBatchTest �Ի���

class CBatchTest : public CDialogEx
{
	DECLARE_DYNAMIC(CBatchTest)

public:
	CBatchTest(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBatchTest();

// �Ի�������
	enum { IDD = IDD_TESTPR_BATCH };
	CPlateRecognize m_batch_pd;
protected:
	CListCtrl* m_BatchList;
	CString m_folderPath;
	CString m_plateResult;
	std::string m_srcPath;
	CString m_pdTime;
	CString m_crTime;
	CString m_istrue;
	int countRight;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	void processPlate(Mat& src, CString strFileName);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedChooseBtn();
	afx_msg void OnBnClickedStartBtn();
};
