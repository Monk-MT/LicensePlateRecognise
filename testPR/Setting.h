#include "SingleTest.h"
#include "BatchTest.h"
#include"Resource.h"
#pragma once


// CSetting �Ի���

class CSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CSetting)

public:
	CSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetting();

	int m_detect_Type = 0;

	bool m_bLifeMode = false;
// �Ի�������
	enum { IDD = SETTING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	CSingleTest m_single;
	CBatchTest m_batch;
public:
	afx_msg void OnBnClickedOk();
};
