#if !defined(AFX_SETREFINTERVALDLG_H__738280E1_370E_11D5_9BB1_87FB15FC5A16__INCLUDED_)
#define AFX_SETREFINTERVALDLG_H__738280E1_370E_11D5_9BB1_87FB15FC5A16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetRefIntervalDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetRefIntervalDlg dialog

class CSetRefIntervalDlg : public CDialog
{
// Construction
public:
	CSetRefIntervalDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetRefIntervalDlg)
	enum { IDD = IDD_SETREFRESH };
	UINT	m_nRefInt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetRefIntervalDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetRefIntervalDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETREFINTERVALDLG_H__738280E1_370E_11D5_9BB1_87FB15FC5A16__INCLUDED_)
