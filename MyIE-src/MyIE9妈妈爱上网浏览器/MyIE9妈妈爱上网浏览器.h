// MyIE9ÂèÂè°®ÉÏÍøä¯ÀÀÆ÷.h : main header file for the MYIE9ÂèÂè°®ÉÏÍøä¯ÀÀÆ÷ application
//

#if !defined(AFX_MYIE9_H__50D6ECF9_5F0B_4164_9666_936A28921697__INCLUDED_)
#define AFX_MYIE9_H__50D6ECF9_5F0B_4164_9666_936A28921697__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyIE9App:
// See MyIE9ÂèÂè°®ÉÏÍøä¯ÀÀÆ÷.cpp for the implementation of this class
//

class CMyIE9App : public CWinApp
{
public:
	CMyIE9App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyIE9App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyIE9App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYIE9_H__50D6ECF9_5F0B_4164_9666_936A28921697__INCLUDED_)
