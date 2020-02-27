// cntrl.cpp : implementation file
//

#include "stdafx.h"
#include "MFC.h"
#include "cntrl.h"
#include "afxdialogex.h"


// cntrl dialog

IMPLEMENT_DYNAMIC(cntrl, CDialogEx)

cntrl::cntrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(cntrl::IDD, pParent)
	, m_vertslidv(0)
	, m_horslidv(0)
{
}

cntrl::~cntrl()
{
}

void cntrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_vertslid);
	DDX_Control(pDX, IDC_SLIDER2, m_horslid);
	DDX_Slider(pDX, IDC_SLIDER1, m_vertslidv);
	DDV_MinMaxInt(pDX, m_vertslidv, 1, 20);
	DDX_Slider(pDX, IDC_SLIDER2, m_horslidv);
	DDV_MinMaxInt(pDX, m_horslidv, 1, 1000);
}


BEGIN_MESSAGE_MAP(cntrl, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &cntrl::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &cntrl::OnBnClickedOk)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &cntrl::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &cntrl::OnNMCustomdrawSlider2)
	ON_BN_CLICKED(IDC_BUTTON1, &cntrl::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &cntrl::OnBnClickedButton2)
END_MESSAGE_MAP()


// cntrl message handlers


void cntrl::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


void cntrl::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void cntrl::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(1);
	*pResult = 0;
}


void cntrl::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(1);
	*pResult = 0;
}


BOOL cntrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_vertslid.SetRange(1,20);
	m_vertslid.SetPos(1);

	m_horslid.SetRange(1,1000);
	m_horslid.SetPos(1);

	m_ctrlInvert = 0;
	m_BGcolor = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void cntrl::OnBnClickedButton1()
{
	 if(m_ctrlInvert)
		 m_ctrlInvert = 0;
	 else
		 m_ctrlInvert = 1;
}


void cntrl::OnBnClickedButton2()
{
	if(m_BGcolor)
		 m_BGcolor = 0;
	 else
		 m_BGcolor = 1;
}
