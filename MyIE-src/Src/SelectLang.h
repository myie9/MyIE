#if !defined(AFX_SELECTLANG_H__957AAD75_CF72_4BCC_AABA_A125301AED5E__INCLUDED_)
#define AFX_SELECTLANG_H__957AAD75_CF72_4BCC_AABA_A125301AED5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectLang.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectLang dialog

class CSelectLang : public CDialog
{
// Construction
public:
	CSelectLang(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectLang)
	enum { IDD = IDD_SELECT_LANG };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectLang)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectLang)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTLANG_H__957AAD75_CF72_4BCC_AABA_A125301AED5E__INCLUDED_)
