#if !defined(AFX_SETALIASDLG_H__17B4F8D5_6F27_11D4_9BF3_0000E85300AE__INCLUDED_)
#define AFX_SETALIASDLG_H__17B4F8D5_6F27_11D4_9BF3_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetAliasDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetAliasDlg dialog

class CSetAliasDlg : public CDialog
{
// Construction
public:
	CSetAliasDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetAliasDlg)
	enum { IDD = IDD_SET_ALIAS };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetAliasDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetAliasDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnNew();
	afx_msg void OnUpdate();
	afx_msg void OnDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETALIASDLG_H__17B4F8D5_6F27_11D4_9BF3_0000E85300AE__INCLUDED_)
