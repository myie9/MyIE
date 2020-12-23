#if !defined(AFX_PROXYINFO_H__49D838A1_157D_11D5_9BB1_884549ED8B33__INCLUDED_)
#define AFX_PROXYINFO_H__49D838A1_157D_11D5_9BB1_884549ED8B33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProxyInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProxyInfo dialog

class CProxyInfo : public CDialog
{
// Construction
public:
	CProxyInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProxyInfo)
	enum { IDD = IDD_PROXY };
	CString	m_ProxyName;
	CString	m_Proxy;
	int		m_Speed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProxyInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProxyInfo)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROXYINFO_H__49D838A1_157D_11D5_9BB1_884549ED8B33__INCLUDED_)
