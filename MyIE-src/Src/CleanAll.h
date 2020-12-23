#if !defined(AFX_CLEANALL_H__DF78C57F_91B9_4D77_81D0_53A137F94566__INCLUDED_)
#define AFX_CLEANALL_H__DF78C57F_91B9_4D77_81D0_53A137F94566__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CleanAll.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCleanAll dialog

class CCleanAll : public CDialog
{
// Construction
public:
	int m_nCleanAllMode;
	CCleanAll(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCleanAll)
	enum { IDD = IDD_CLEAN_ALL };
	BOOL	m_bCache;
	BOOL	m_bCookies;
	BOOL	m_bFormData;
	BOOL	m_bHistory;
	BOOL	m_bTypedAddress;
	BOOL	m_bTypedSearch;
	BOOL	m_bUndoClose;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCleanAll)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCleanAll)
	afx_msg void OnSelectAll();
	afx_msg void OnSelectNone();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLEANALL_H__DF78C57F_91B9_4D77_81D0_53A137F94566__INCLUDED_)
