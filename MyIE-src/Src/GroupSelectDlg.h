#if !defined(AFX_GROUPSELECTDLG_H__FD998058_389D_418E_B127_351B93A79C47__INCLUDED_)
#define AFX_GROUPSELECTDLG_H__FD998058_389D_418E_B127_351B93A79C47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GroupSelectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGroupSelectDlg dialog

class CGroupSelectDlg : public CDialog
{
// Construction
public:
	void GetUnqBlankName(CString &newblnk);
	CGroupSelectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGroupSelectDlg)
	enum { IDD = IDD_GROUP_SELECT };
	CComboBox	m_name;
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGroupSelectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetTime();
	virtual void OnOK();
	afx_msg void OnSelectAll();
	afx_msg void OnSelectNone();
	afx_msg void OnUpdate();
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GROUPSELECTDLG_H__FD998058_389D_418E_B127_351B93A79C47__INCLUDED_)
