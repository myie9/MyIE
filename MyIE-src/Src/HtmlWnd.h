#if !defined(AFX_HTMLWND_H__74460E25_1598_11D4_9BCE_0000E85300AE__INCLUDED_)
#define AFX_HTMLWND_H__74460E25_1598_11D4_9BCE_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HtmlWnd.h : header file
//

#include "WebBrowser2.h"
#include "MyIE9Helper.h"


/////////////////////////////////////////////////////////////////////////////
// CHtmlWnd window

class CHtmlWnd : public CWebBrowser2
{
// Construction
public:
	CHtmlWnd();
	DECLARE_EVENTSINK_MAP()

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHtmlWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHtmlWnd();


	// Generated message map functions
protected:
	//{{AFX_MSG(CHtmlWnd)
	//}}AFX_MSG
	virtual void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* bCancel);
	DECLARE_MESSAGE_MAP()

  // Event map declaration for WebBrowser control events.

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLWND_H__74460E25_1598_11D4_9BCE_0000E85300AE__INCLUDED_)


