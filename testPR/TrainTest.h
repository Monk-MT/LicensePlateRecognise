#pragma once
#include"Resource.h"

// TrainTest 对话框

class TrainTest : public CDialogEx
{
	DECLARE_DYNAMIC(TrainTest)

public:
	TrainTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TrainTest();

// 对话框数据
	enum { IDD = IDD_TRAIN_TEST };

protected:
	CString m_folderPath;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
