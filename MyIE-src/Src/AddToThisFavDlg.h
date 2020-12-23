#if !defined(AFX_ADDTOTHISFAVDLG_H__1928D62C_1538_40CE_9FFB_AC56F663BF6F__INCLUDED_)
#define AFX_ADDTOTHISFAVDLG_H__1928D62C_1538_40CE_9FFB_AC56F663BF6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddToThisFavDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddToThisFavDlg dialog

class CAddToThisFavDlg : public CDialog
{
// Construction
public:
	int		m_nAutoRunCmd;
	DWORD	m_dwProperty;
	CAddToThisFavDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddToThisFavDlg)
	enum { IDD = IDD_ADDURLHERE };
	CComboBox	m_command;
	CString	m_strFolder;
	CString	m_strName;
	CString	m_strURL;
	CButton	m_UseSpecial;
	CButton	m_bDLActivex;
	CButton	m_bDLApplet;
	CButton	m_bDLImage;
	CButton	m_bDLScript;
	CButton	m_bDLSound;
	CButton	m_bDLVideo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddToThisFavDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddToThisFavDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnUseSpecial();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDTOTHISFAVDLG_H__1928D62C_1538_40CE_9FFB_AC56F663BF6F__INCLUDED_)
