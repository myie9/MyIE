#if !defined(AFX_MONITOR_H__0BA46B1E_2D26_48B7_BDE1_897A03A1E461__INCLUDED_)
#define AFX_MONITOR_H__0BA46B1E_2D26_48B7_BDE1_897A03A1E461__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Monitor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMonitor dialog

class CMonitor : public CDialog
{
// Construction
public:
	void CollectorSetTrans(int mode);
	void CollectorTrans(int mode);
	CMonitor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMonitor)
	enum { IDD = IDD_MONITOR };
	CStatic	m_percent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonitor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMonitor)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONITOR_H__0BA46B1E_2D26_48B7_BDE1_897A03A1E461__INCLUDED_)
