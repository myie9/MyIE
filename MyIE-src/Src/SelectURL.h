#if !defined(AFX_SELECTURL_H__BCD4200C_4A37_4805_BB05_3BEF618CA375__INCLUDED_)
#define AFX_SELECTURL_H__BCD4200C_4A37_4805_BB05_3BEF618CA375__INCLUDED_

#include "SortListCtrl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectURL.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectURL dialog

class CSelectURL : public CDialog
{
// Construction
public:
	int	 m_nUrlType;//0->3
	void OpenSelected();
	void LoadNewUrl();
	//   
	void LoadLastVisiting();
	void LoadLastClosed();
	void LoadVisting();
	void LoadClosed();
	void OkClosed();
	//
	CSelectURL(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectURL)
	enum { IDD = IDD_SELECT_URL };
	CTabCtrl	m_tab;
	CSortListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectURL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSelectURL)
	afx_msg void OnSelectAll();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectNone();
	virtual void OnOK();
	afx_msg void OnOffline();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDelCurrent();
	afx_msg void OnDelUrl();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditTitle();
	afx_msg void OnEditUrl();
	afx_msg void OnRdblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOpenCurrent();
	afx_msg void OnOpenSelected();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTURL_H__BCD4200C_4A37_4805_BB05_3BEF618CA375__INCLUDED_)
