
// MFCView.cpp : implementation of the CMFCView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFC.h"
#endif

#include "MFCDoc.h"
#include "MFCView.h"
#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cntrl.h"
#pragma comment (lib, "wsock32.lib")
DWORD WINAPI IMPORTDATA(LPVOID pParam);
DWORD WINAPI CONTROL(LPVOID pParam);

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CMFCView

IMPLEMENT_DYNCREATE(CMFCView, CView)

	BEGIN_MESSAGE_MAP(CMFCView, CView)
		// Standard printing commands
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCView::OnFilePrintPreview)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_WM_ERASEBKGND()
		ON_COMMAND(ID_FUNCTION_BEGIN, &CMFCView::OnFunctionBegin)
		ON_COMMAND(ID_FUNCTION_STOP, &CMFCView::OnFunctionStop)
	END_MESSAGE_MAP()

	// CMFCView construction/destruction

	CMFCView::CMFCView()
	{
		CreateThread(0,0,CONTROL, this ,0 ,0 );
		//*********************** Initial Data **********************	
		m_VER = new int[1];
		m_HOR = new int[1];
		m_INVERT = new BOOL[1];
		m_COLORBG = new BOOL[1];
		iPort = 1024;
		_COUNTER = 0;
		hex  = "0123456789ABCDEF";
		for (int _cnt=0;_cnt<512;_cnt++)
		{szMessage[_cnt] = 0; szMessageString[_cnt] = 0;}
		DATA = new int[40000];
		WSAStartup(MAKEWORD(1,1), &wsda);
		s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		_STOP = 2;
		imagecall = 0;
		if(s == SOCKET_ERROR)
		{
			MessageBox(L"Error Call to socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) failed",L"ERROR",0);
			exit(1);
		}

		addr.sin_family = AF_INET;
		addr.sin_port = htons(iPort);
		addr.sin_addr.s_addr = INADDR_ANY;

		if(bind(s, (struct sockaddr *) &addr, sizeof(addr)) == SOCKET_ERROR)
		{
			MessageBox(L"Error Call to bind(s, (struct sockaddr *) &addr, sizeof(addr)) failed ",L"ERROR",0);
			exit(1);
		}


	}

	CMFCView::~CMFCView()
	{
	}

	BOOL CMFCView::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs

		return CView::PreCreateWindow(cs);
	}

	// CMFCView drawing


	void CMFCView::OnFilePrintPreview()
	{
#ifndef SHARED_HANDLERS
		AFXPrintPreview(this);
#endif
	}

	BOOL CMFCView::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// default preparation
		return DoPreparePrinting(pInfo);
	}

	void CMFCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add extra initialization before printing
	}

	void CMFCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add cleanup after printing
	}

	void CMFCView::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void CMFCView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


#ifdef _DEBUG
	void CMFCView::AssertValid() const
	{
		CView::AssertValid();
	}

	void CMFCView::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}

	CMFCDoc* CMFCView::GetDocument() const // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCDoc)));
		return (CMFCDoc*)m_pDocument;
	}
#endif //_DEBUG


	void CMFCView::OnDraw(CDC* dc)
	{
		CRect rcBounds;
		GetClientRect(&rcBounds);
		_CMemDC	pDC(dc, rcBounds);

		if(m_COLORBG[0])
			pDC->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH)));
		else pDC->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));

		screenwidth = rcBounds.Width();
		screenheight = rcBounds.Height(); 
		prmtmp = float(screenheight*0.0039215686)/float(m_VER[0]);

		//********************** GRIDING *********************
		CPen GRUN_pen;
		GRUN_pen.CreatePen(PS_DOT, 1, RGB(0,255,0));
		pDC->SelectObject(GRUN_pen);

		for(int j=0 ; j<screenheight ; )
		{
			pDC->MoveTo(0,j);
			pDC->LineTo(screenwidth,j);
			j = j+20;
		}
		pDC->TextOutW(0,0,L"LAWINE. ELECTRONICS");
		float GRIDZRO ;
		if(!m_INVERT[0])
			GRIDZRO = float((screenheight/2)-prmtmp*30) ;
		else GRIDZRO = float((screenheight/2)+prmtmp*30) ;
		
		CPen DRW_pen;
		if(m_COLORBG[0])
			DRW_pen.CreatePen(PS_SOLID, 1, RGB(255,255,255));
		else DRW_pen.CreatePen(PS_SOLID, 1, RGB(0,0,0));

		pDC->SelectObject(DRW_pen);
		pDC->MoveTo(0,screenheight/2);
		for(int i =0 ; i<_COUNTER ;i++)  
		{
			DATA[i] = min(DATA[i],255);DATA[i] = max(DATA[i],0);
			float _temp;
			if(!m_INVERT[0])
				_temp =   (screenheight/2) - prmtmp*DATA[i];
			else _temp =   (screenheight/2) + prmtmp*DATA[i];
			pDC->LineTo(i,_temp);
			pDC->MoveTo(i,_temp);

		}
	}

	BOOL CMFCView::OnEraseBkgnd(CDC* pDC)
	{
		return 0;
	}

	DWORD WINAPI IMPORTDATA(LPVOID pParam)
	{
		CMFCView *p = (CMFCView*) pParam ;
		int i=0;
		while(!p->_STOP)
		{
			p->iRemoteAddrLen = sizeof(p->remote_addr);
			p->ret = recvfrom(p->s, p->szMessage, sizeof(p->szMessage), 0, (struct sockaddr *) &(p->remote_addr), &(p->iRemoteAddrLen));
			if(p->ret == SOCKET_ERROR)
			{
				p->MessageBox(L"Error Call to recvfrom(s, szMessage, sizeof(szMessage), 0, (struct sockaddr *) &remote_addr, &iRemoteAddrLen); failed ",L"ERROR",0);
				exit(1);
			}

			for(i=0; i<p->ret; i++)
			{
				p->szMessageString[0] = p->hex[p->szMessage[i]>>4];
				p->szMessageString[1] =  p->hex[p->szMessage[i]&0xF];
				if(p->szMessageString[0] > 57)
					p->DATA[p->_COUNTER] = 16*(p->szMessageString[0]-55) ;
				else
					p->DATA[p->_COUNTER] = 16*(p->szMessageString[0]-48) ;

				if(p->szMessageString[1] > 57)
					p->DATA[p->_COUNTER] += (p->szMessageString[1]-55) ;
				else
					p->DATA[p->_COUNTER] += (p->szMessageString[1]-48) ;

				p->_COUNTER ++;
				p->imagecall ++;
				if(p->imagecall >= 100)
				{p->Invalidate();
				p->imagecall = 0;}
				if(p->_COUNTER >= p->screenwidth)
				{	
					for(int J =0 ; J<p->_COUNTER ; J++)  
						p->DATA[J] = p->DATA[J+10];
					p->_COUNTER = p->screenwidth-10;
				}
				i += p->m_HOR[0] ;
			}
		}
		return 0 ;
	}

	void CMFCView::OnFunctionBegin()
	{
		_STOP = 0;
		CreateThread(0,0,IMPORTDATA, this ,0 ,0 );

	}

	void CMFCView::OnFunctionStop()
	{
		_STOP = 1;
	}

	DWORD WINAPI CONTROL(LPVOID pParam)
	{
		CMFCView *p = (CMFCView*) pParam ;
		cntrl DLG;
		p->m_VER = &DLG.m_vertslidv ;
		p->m_HOR = &DLG.m_horslidv ;
		p->m_INVERT = &DLG.m_ctrlInvert ;
		p->m_COLORBG = &DLG.m_BGcolor;
		DLG.DoModal();

		return 0;
	} 

