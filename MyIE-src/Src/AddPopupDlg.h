#if !defined(AFX_ADDPOPUPDLG_H__7107E694_B70D_4C39_AC41_C02579B4FF7C__INCLUDED_)
#define AFX_ADDPOPUPDLG_H__7107E694_B70D_4C39_AC41_C02579B4FF7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddPopupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddPopupDlg dialog

class CAddPopupDlg : public CDialog
{
// Construction
public:
	CAddPopupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddPopupDlg)
	enum { IDD = IDD_ADD_POPUP };
	CString	m_strUrl;
	BOOL	m_bClosePage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddPopupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddPopupDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDPOPUPDLG_H__7107E694_B70D_4C39_AC41_C02579B4FF7C__INCLUDED_)
