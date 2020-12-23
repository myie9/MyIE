// FlyDlg.h: interface for the CFlyDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLYDLG_H__1373CA29_D89B_401F_A411_1D80727F6234__INCLUDED_)
#define AFX_FLYDLG_H__1373CA29_D89B_401F_A411_1D80727F6234__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "SAPrefsDialog.h"

class CFlyDlg : public CSAPrefsSubDlg
{
	DECLARE_DYNCREATE(CFlyDlg)
// Construction
public:
	void SetFlyGroupEnable(BOOL state);
	void SetListGroupEnable(BOOL state);
	CFlyDlg();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFlyDlg)
	enum { IDD = IDD_FLYLIST };
	CListCtrl	m_UnFlyList;
	CListCtrl	m_FlyList;

	BOOL	m_bKillFly;
	BOOL m_bKillFlyWhenFresh;
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFlyDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDelete();
	afx_msg void OnEdit();
	afx_msg void OnInsert();
	afx_msg void OnEndlabeleditFlylist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkFlylist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnableFlyWin();
	afx_msg void OnUnInsert();
	afx_msg void OnUnDelete();
	afx_msg void OnUnEdit();
	afx_msg void OnDblclkUnFlylist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditUnFlylist(NMHDR* pNMHDR, LRESULT* pResult);
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_FLYDLG_H__1373CA29_D89B_401F_A411_1D80727F6234__INCLUDED_)
