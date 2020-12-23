#if !defined(AFX_CUSTOMIZETOOLBAR_H__528BAA53_7182_11D4_9BF4_0000E85300AE__INCLUDED_)
#define AFX_CUSTOMIZETOOLBAR_H__528BAA53_7182_11D4_9BF4_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomizeToolbar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomizeToolbar dialog

class CCustomizeToolbar : public CDialog
{
// Construction
public:
	CCustomizeToolbar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCustomizeToolbar)
	enum { IDD = IDD_CUSTOMIZE };
	CListCtrl	m_curbtn;
	CListCtrl	m_avabtn;
	BOOL	m_bColorIcon;
	int		m_nTextOpt;
	int		m_bSmallToolBar;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomizeToolbar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString str; 
	
	// Generated message map functions
	//{{AFX_MSG(CCustomizeToolbar)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnMoveDown();
	afx_msg void OnMoveUp();
	afx_msg void OnRemove();
	afx_msg void OnAdd();
	afx_msg void OnDblclkAvabtn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCurbtn(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMIZETOOLBAR_H__528BAA53_7182_11D4_9BF4_0000E85300AE__INCLUDED_)


