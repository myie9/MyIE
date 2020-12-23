#if !defined(AFX_COMBOBAR_H__CC843194_A55D_11D4_9B47_0000E85300AE__INCLUDED_)
#define AFX_COMBOBAR_H__CC843194_A55D_11D4_9B47_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddressBarEx.h : header file
//
#include <afxole.h>
#include "AddressBarEx.h"
#include "Gf_CComboBoxEx.h"

/////////////////////////////////////////////////////////////////////////////
// CComboBar window

class CComboBar : public CToolBar
{
// Construction
public:
	CComboBar();

// Attributes
public:
	CAddDropTarget	ms_dropTarget;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	int FindStringExact(int start, CString& str);
	void GetSelText(int index, CString& str);
	CGf_CComboBoxEx m_wndComboBox;
	virtual ~CComboBar();
	void Init();
	void InitImage();
	void InitButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CComboBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOBAR_H__CC843194_A55D_11D4_9B47_0000E85300AE__INCLUDED_)
