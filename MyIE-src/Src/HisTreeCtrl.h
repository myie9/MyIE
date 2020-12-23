#if !defined(AFX_HISTREECTRL_H__A733B004_1724_11D4_9BCE_0000E85300AE__INCLUDED_)
#define AFX_HISTREECTRL_H__A733B004_1724_11D4_9BCE_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HisTreeCtrl.h : header file
//
#include "MyIE9Helper.h"
#include "BaseExpObj.h"

/////////////////////////////////////////////////////////////////////////////
// CHisTreeCtrl window

class CHisTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CHisTreeCtrl();

// Attributes
public:
	CStringArray m_astrHist;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHisTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	LPITEMIDLIST FindFolder(LPCSTR foldername, IShellFolder* pFolder);
	CBaseExpObj* m_pHisTree;
	virtual ~CHisTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHisTreeCtrl)
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTREECTRL_H__A733B004_1724_11D4_9BCE_0000E85300AE__INCLUDED_)
