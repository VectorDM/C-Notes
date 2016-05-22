// ECGView.h : interface of the CECGView class
//
/////////////////////////////////////////////////////////////////////////////
#include <algorithm>

using namespace std;

#if !defined(AFX_ECGVIEW_H__1E6AEA5A_34C4_45A9_BAF2_B19AC96E0F7A__INCLUDED_)
#define AFX_ECGVIEW_H__1E6AEA5A_34C4_45A9_BAF2_B19AC96E0F7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CECGView : public CScrollView
{
protected: // create from serialization only
	CECGView();
	DECLARE_DYNCREATE(CECGView)

// Attributes 
public:
	CECGDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CECGView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void LineDashLine(CDC *pDC, int xStart, int yStart, int xEnd, int yEnd, COLORREF color);

	void R_detect(long int *origin, int *R_Position,int *R_NegativePosition, int N_data, int *R_num,int *R_negativePositionNum);
	void LineDashLineR(CDC *pDC, int xStart, int yStart, int xEnd, int yEnd);
	void Q_detect( int long *origin, int *R_Position, int *Q_Position, int *R_num);
	void S_detect( int long *origin, int *R_Position, int *S_Position, int *R_num);


	void Erosion(long int *origin, int* structElement,long int *Eroded,int N_data, int se);
	void Dilation(long int *origin, int* structElement,long int *Dilated,int N_data, int se);


	void OpeningClosing(long int *origin, int* structElement, long int *OC,int N_data, int se);
	void ClosingOpening(long int *origin, int* structElement, long int *CO,int N_data, int se);

//	void R_detect(long int *origin, int *R_Position, int N_data, int *R_num);



	void ECG_Plot(CDC *pDC);
	char fname[20];
	bool m_fileOpen;
	virtual ~CECGView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CECGView)
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ECGView.cpp
inline CECGDoc* CECGView::GetDocument()
   { return (CECGDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECGVIEW_H__1E6AEA5A_34C4_45A9_BAF2_B19AC96E0F7A__INCLUDED_)
