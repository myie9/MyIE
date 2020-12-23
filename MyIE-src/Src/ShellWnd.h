#if !defined(AFX_SHELL_H__F2360A43_3338_11D4_9BB1_FED37F9D968F__INCLUDED_)
#define AFX_SHELL_H__F2360A43_3338_11D4_9BB1_FED37F9D968F__INCLUDED_

#include "ShellTreeCtrl.h"	// Added by ClassView

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ShellWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShellWnd window

class CShellWnd : public CWnd
{
// Construction
public:
	CShellWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShellWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	CToolBar m_wndRef;
	CComboBox m_fType;
	CShellTreeCtrl m_ShellTreeCtrl;
	virtual ~CShellWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShellWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRefresh();
	//}}AFX_MSG
	afx_msg void OnComboSelChange();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHELL_H__F2360A43_3338_11D4_9BB1_FED37F9D968F__INCLUDED_)
