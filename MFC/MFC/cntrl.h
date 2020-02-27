#pragma once
#include "afxcmn.h"


// cntrl dialog

class cntrl : public CDialogEx
{
	DECLARE_DYNAMIC(cntrl)

public:
	cntrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~cntrl();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_ctrlInvert;
	BOOL m_BGcolor;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CSliderCtrl m_vertslid;
	CSliderCtrl m_horslid;
	int m_vertslidv;
	int m_horslidv;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
