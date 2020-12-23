#if !defined(AFX_POPUPDLG_H__9A927296_0A9F_11D4_9BC9_0000E85300AE__INCLUDED_)
#define AFX_POPUPDLG_H__9A927296_0A9F_11D4_9BC9_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PopupDlg.h : header file
//

#include "SAPrefsDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CPopupDlg dialog

class CPopupDlg : public CSAPrefsSubDlg
{
	DECLARE_DYNCREATE(CPopupDlg)
// Construction
public:
	void SetPopGroupEnable(BOOL state);
	void SetListGroupEnable(BOOL state);
	CPopupDlg();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPopupDlg)
	enum { IDD = IDD_POPUPLIST };
	CListCtrl	m_UnPopupList;
	CListCtrl	m_PopupList;
	BOOL	m_bKillAutoWin;
	BOOL	m_bKillDupWin;
	BOOL	m_bKillListWin;
	BOOL	m_bKillPop;
	BOOL	m_bNotConfirmPopupFilter;
	BOOL	m_bPlayPopupFilterSound;
	int		m_nPopDelay;
	BOOL	m_bFilterRefresh;
	BOOL	m_bNotFilterClickDialog;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPopupDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDelete();
	afx_msg void OnEdit();
	afx_msg void OnInsert();
	afx_msg void OnEndlabeleditPopuplist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkPopuplist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnablePopWin();
	afx_msg void OnUnInsert();
	afx_msg void OnUnDelete();
	afx_msg void OnUnEdit();
	afx_msg void OnDblclkUnPopuplist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditUnPopuplist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillListWin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPUPDLG_H__9A927296_0A9F_11D4_9BC9_0000E85300AE__INCLUDED_)
