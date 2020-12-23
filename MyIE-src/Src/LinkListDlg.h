#if !defined(AFX_LINKLISTDLG_H__0D39EF01_730C_11D5_9BB3_8D1C1810813E__INCLUDED_)
#define AFX_LINKLISTDLG_H__0D39EF01_730C_11D5_9BB3_8D1C1810813E__INCLUDED_

#include "SortListCtrl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LinkListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLinkListDlg dialog

class CLinkListDlg : public CDialog
{
// Construction
public:
	CToolBar m_ToolBar;
	int		m_nToolbarWidth,m_nToolbarHeight;
	//
	void UpdateTitleInfo();
	//
	void OnSelectAll();
	void OnSelectNone();
	void OnSelectUp();
	void OnSelectDown();
	void OnSelectOdd();
	void OnSelectEven();
	//
	void OnOpenSelect();
	void OnCloseSelect();
	void OnCopyUrl();
	void OnEditUrl();
	void OnRefreshLinkList();
	//
	void OnUpdate();
	void FillLinkList();
	CLinkListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLinkListDlg)
	enum { IDD = IDD_LINKLIST };
	CEdit	m_name;
	CComboBox	m_url;
	CSortListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLinkListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLinkListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChangeUrl();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnOK();
	afx_msg void OnRdblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINKLISTDLG_H__0D39EF01_730C_11D5_9BB3_8D1C1810813E__INCLUDED_)
