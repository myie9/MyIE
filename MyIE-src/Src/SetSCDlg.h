#if !defined(AFX_SETSCDLG_H__17B4F8D4_6F27_11D4_9BF3_0000E85300AE__INCLUDED_)
#define AFX_SETSCDLG_H__17B4F8D4_6F27_11D4_9BF3_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetSCDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetSCDlg dialog

class CSetSCDlg : public CDialog
{
// Construction
public:
	CSetSCDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetSCDlg)
	enum { IDD = IDD_SET_KEY };
	CComboBox	m_keylist;
	CEdit	m_url;
	CListCtrl	m_list;
	CString	m_strEdit;
	BOOL	m_bUseIeKey;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetSCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetSCDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnUpdate();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeKeylist();
	afx_msg void OnAppend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETSCDLG_H__17B4F8D4_6F27_11D4_9BF3_0000E85300AE__INCLUDED_)
