#if !defined(AFX_SELPROXYCATDLG_H__779B75A1_39AD_11D5_9BB1_C4E0857D2102__INCLUDED_)
#define AFX_SELPROXYCATDLG_H__779B75A1_39AD_11D5_9BB1_C4E0857D2102__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelProxyCatDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelProxyCatDlg dialog

class CSelProxyCatDlg : public CDialog
{
// Construction
public:
	BOOL m_bDisableClear;
	CSelProxyCatDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelProxyCatDlg)
	enum { IDD = IDD_SELPROXYCAT };
	CButton	m_ClearProxy;
	CComboBox	m_Proxies;
	BOOL	m_bClear;
	CString	m_strCatName;
	int		m_SelCates;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelProxyCatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelProxyCatDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELPROXYCATDLG_H__779B75A1_39AD_11D5_9BB1_C4E0857D2102__INCLUDED_)
