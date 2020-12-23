#if !defined(AFX_SORTSAVEDLG_H__226D33F9_4C4F_476D_A4AA_551378F06726__INCLUDED_)
#define AFX_SORTSAVEDLG_H__226D33F9_4C4F_476D_A4AA_551378F06726__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SortSaveDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSortSaveDlg dialog

class CSortSaveDlg : public CDialog
{
// Construction
public:
	void GetUrl();
	void GetTitle();
	BOOL	m_bUrlReplaced;
	CString m_strUrl;
	CString m_strTitle;
	void NewDir(CString strNew);
	CString m_strSortSaveFolder;
	CSortSaveDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSortSaveDlg)
	enum { IDD = IDD_SORT_SAVE };
	CListCtrl	m_list;
	CString	m_strFileName;
	BOOL	m_bType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSortSaveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSortSaveDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNotSort();
	afx_msg void OnNew();
	afx_msg void OnOpen();
	afx_msg void OnRename();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDateSave();
	afx_msg void OnType();
	afx_msg void OnShowSortDlg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SORTSAVEDLG_H__226D33F9_4C4F_476D_A4AA_551378F06726__INCLUDED_)
