#if !defined(AFX_FAVWND_H__F2360A43_3338_11D4_9BB1_FED37F9D968F__INCLUDED_)
#define AFX_FAVWND_H__F2360A43_3338_11D4_9BB1_FED37F9D968F__INCLUDED_

#include "FavTreeCtrl.h"	// Added by ClassView
#include "ChevBar.h"	// Added by ClassView

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FavWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFavWnd window

class CFavWnd : public CWnd
{
// Construction
public:
	CFavWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFavWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitImage();
	BOOL m_bFolderOnly;
	CChevBar m_Rebar;
	CToolBar m_ToolBar;
	CFavTreeCtrl m_FavTreeCtrl;
	virtual ~CFavWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFavWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFavoritesOrgnizefavorites();
	afx_msg void OnFavoritesAddtofavorites();
	afx_msg void OnFavoritesBest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAVWND_H__F2360A43_3338_11D4_9BB1_FED37F9D968F__INCLUDED_)
