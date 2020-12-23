#if !defined(AFX_INPUTPASSWORD_H__9C717923_A0B8_11D4_9B45_0000E85300AE__INCLUDED_)
#define AFX_INPUTPASSWORD_H__9C717923_A0B8_11D4_9B45_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputPassword.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputPassword dialog

class CInputPassword : public CDialog
{
// Construction
public:
	CInputPassword(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputPassword)
	enum { IDD = IDD_PASSWORD };
	CString	m_strPassword;
	BOOL	m_bRemPsw;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputPassword)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputPassword)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTPASSWORD_H__9C717923_A0B8_11D4_9B45_0000E85300AE__INCLUDED_)
