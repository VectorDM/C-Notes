// ECG.h : main header file for the ECG application
//

#if !defined(AFX_ECG_H__9F51C9CD_78DA_46FE_8CEF_D460F00E892A__INCLUDED_)
#define AFX_ECG_H__9F51C9CD_78DA_46FE_8CEF_D460F00E892A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CECGApp:
// See ECG.cpp for the implementation of this class
//

class CECGApp : public CWinApp
{
public:
	CECGApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CECGApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CECGApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECG_H__9F51C9CD_78DA_46FE_8CEF_D460F00E892A__INCLUDED_)
