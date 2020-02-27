
// MFCView.h : interface of the CMFCView class
//

#pragma once


class CMFCView : public CView 
{
protected: // create from serialization only
	CMFCView();
	DECLARE_DYNCREATE(CMFCView)

	// Attributes
public:
	CMFCDoc* GetDocument() const;

	// Operations
public:

	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// Implementation
public:
	virtual ~CMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//************** DATA ************
	WSADATA wsda;
	SOCKET s;
	SOCKADDR_IN addr, remote_addr;
	u_short iPort ;
	int  ret, iRemoteAddrLen;
	char *hex ;
	char szMessage[4096], szMessageString[4096];
	BOOL _STOP ;
	float prmtmp ;
	int screenwidth;
	int screenheight;
	int *DATA ;
	afx_msg void _draw(void);
	int _COUNTER ;
	int imagecall ;
	int *m_VER;
	int *m_HOR;
	BOOL *m_INVERT ;
	BOOL *m_COLORBG ;
	//************** DATA ************

	afx_msg void OnFunctionBegin();
	afx_msg void OnFunctionStop();
};

#ifndef _DEBUG  // debug version in MFCView.cpp
inline CMFCDoc* CMFCView::GetDocument() const
{ return reinterpret_cast<CMFCDoc*>(m_pDocument); }
#endif

