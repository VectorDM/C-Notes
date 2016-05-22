// ECGDoc.h : interface of the CECGDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ECGDOC_H__A238CC78_6AFF_489B_8336_05F3029E39F8__INCLUDED_)
#define AFX_ECGDOC_H__A238CC78_6AFF_489B_8336_05F3029E39F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CECGDoc : public CDocument
{
protected: // create from serialization only
	CECGDoc();
	DECLARE_DYNCREATE(CECGDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CECGDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CECGDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CECGDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECGDOC_H__A238CC78_6AFF_489B_8336_05F3029E39F8__INCLUDED_)
