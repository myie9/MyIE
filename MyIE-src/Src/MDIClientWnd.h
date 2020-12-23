#if !defined(AFX_MDICLIENTWND_H__69901519_D894_4FB2_B61F_7CA23136F29F__INCLUDED_)
#define AFX_MDICLIENTWND_H__69901519_D894_4FB2_B61F_7CA23136F29F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MDIClientWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMDIClientWnd window

class CMDIClientWnd : public CWnd
{
// Construction
public:
	CMDIClientWnd();

// Attributes
public:
    WNDPROC* GetSuperWndProcAddr();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDIClientWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMDIClientWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMDIClientWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDICLIENTWND_H__69901519_D894_4FB2_B61F_7CA23136F29F__INCLUDED_)
