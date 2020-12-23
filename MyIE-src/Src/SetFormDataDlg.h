#if !defined(AFX_SETFORMDATADLG_H__85BD59DB_DFCA_4827_9EC9_E6CCD4A37E39__INCLUDED_)
#define AFX_SETFORMDATADLG_H__85BD59DB_DFCA_4827_9EC9_E6CCD4A37E39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetFormDataDlg.h : header file
//
#include "SortListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CSetFormDataDlg dialog

class CSetFormDataDlg : public CDialog
{
// Construction
public:
	void	FillFormItem(int index);
	CSetFormDataDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetFormDataDlg)
	enum { IDD = IDD_SET_FORM_DATA };
	CSortListCtrl	m_url;
	CSortListCtrl	m_form;
	CButton	m_AutoSubmit;
	BOOL	m_bEncryptStringItem;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetFormDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	static int CALLBACK MyCompareEntries(LPARAM lp1, LPARAM lp2, LPARAM lpList );

	// Generated message map functions
	//{{AFX_MSG(CSetFormDataDlg)
	afx_msg void OnEditItem();
	virtual BOOL OnInitDialog();
	afx_msg void OnDelUrl();
	afx_msg void OnDblclkForm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDelUrlAll();
	afx_msg void OnSetMainPwd();
	afx_msg void OnSetFillPwd();
	afx_msg void OnAutoSubmit();
	afx_msg void OnItemchangedUrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkUrl(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SortByDefault();
	void LoadFormUrl();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETFORMDATADLG_H__85BD59DB_DFCA_4827_9EC9_E6CCD4A37E39__INCLUDED_)
