#if !defined(AFX_INPUTALIAS_H__17B4F8D6_6F27_11D4_9BF3_0000E85300AE__INCLUDED_)
#define AFX_INPUTALIAS_H__17B4F8D6_6F27_11D4_9BF3_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputAlias.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputAlias dialog

class CInputAlias : public CDialog
{
// Construction
public:
	CInputAlias(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputAlias)
	enum { IDD = IDD_INPUT_ALIAS };
	CEdit	m_edURL;
	CString	m_alias;
	CString	m_url;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputAlias)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputAlias)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTALIAS_H__17B4F8D6_6F27_11D4_9BF3_0000E85300AE__INCLUDED_)
