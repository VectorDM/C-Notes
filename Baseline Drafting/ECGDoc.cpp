// ECGDoc.cpp : implementation of the CECGDoc class
//

#include "stdafx.h"
#include "ECG.h"

#include "ECGDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CECGDoc

IMPLEMENT_DYNCREATE(CECGDoc, CDocument)

BEGIN_MESSAGE_MAP(CECGDoc, CDocument)
	//{{AFX_MSG_MAP(CECGDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECGDoc construction/destruction

CECGDoc::CECGDoc()
{
	// TODO: add one-time construction code here

}

CECGDoc::~CECGDoc()
{
}

BOOL CECGDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CECGDoc serialization

void CECGDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CECGDoc diagnostics

#ifdef _DEBUG
void CECGDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CECGDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CECGDoc commands
