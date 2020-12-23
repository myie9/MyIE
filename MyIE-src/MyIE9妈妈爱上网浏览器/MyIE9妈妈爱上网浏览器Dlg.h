// MyIE9ÂèÂè°®ÉÏÍøä¯ÀÀÆ÷Dlg.h : header file
//

#if !defined(AFX_MYIE9DLG_H__0B9CE705_5920_4F51_873B_760ED3BF4C3C__INCLUDED_)
#define AFX_MYIE9DLG_H__0B9CE705_5920_4F51_873B_760ED3BF4C3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyIE9Dlg dialog

class CMyIE9Dlg : public CDialog
{
// Construction
public:
	CMyIE9Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMyIE9Dlg)
	enum { IDD = IDD_MYIE9_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyIE9Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyIE9Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYIE9DLG_H__0B9CE705_5920_4F51_873B_760ED3BF4C3C__INCLUDED_)
