#pragma once
#include"Resource.h"

// TrainTest �Ի���

class TrainTest : public CDialogEx
{
	DECLARE_DYNAMIC(TrainTest)

public:
	TrainTest(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TrainTest();

// �Ի�������
	enum { IDD = IDD_TRAIN_TEST };

protected:
	CString m_folderPath;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
