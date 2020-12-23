#if !defined(AFX_POPUPCONFIRM_H__35BAD344_5363_49DD_B0EA_B686B68AE95B__INCLUDED_)
#define AFX_POPUPCONFIRM_H__35BAD344_5363_49DD_B0EA_B686B68AE95B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PopupConfirm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPopupConfirm dialog

class CPopupConfirm : public CDialog
{
// Construction
public:
	CPopupConfirm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPopupConfirm)
	enum { IDD = IDD_POPUP_CONFIRM };
	CEdit	m_url;
	CString	m_strURL;
	BOOL	m_bNotConfirmPopupFilter;
	BOOL	m_bAddToPopList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupConfirm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPopupConfirm)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPUPCONFIRM_H__35BAD344_5363_49DD_B0EA_B686B68AE95B__INCLUDED_)
