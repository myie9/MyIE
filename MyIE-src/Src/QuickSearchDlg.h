#if !defined(AFX_QUICKSEARCHDLG_H__00CE86D7_ACEB_498F_85F5_4FA57F0B3B46__INCLUDED_)
#define AFX_QUICKSEARCHDLG_H__00CE86D7_ACEB_498F_85F5_4FA57F0B3B46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QuickSearchDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CQuickSearchDlg dialog

class CQuickSearchDlg : public CDialog
{
// Construction
public:
	CQuickSearchDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CQuickSearchDlg)
	enum { IDD = IDD_QUICKSEARCH };
	CEdit	m_cIcon;
	CEdit	m_cSearchEngine;
	CEdit	m_cName;
	CListCtrl	m_EngineList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuickSearchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CQuickSearchDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnUpdate();
	afx_msg void OnInsert();
	afx_msg void OnDelete();
	afx_msg void OnItemchangedSeachlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClean();
	afx_msg void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bUpdating;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUICKSEARCHDLG_H__00CE86D7_ACEB_498F_85F5_4FA57F0B3B46__INCLUDED_)
