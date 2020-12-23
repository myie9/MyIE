#if !defined(AFX_FAVTREE_H__CC415E43_148C_11D4_9BB0_CFFDB04BA878__INCLUDED_)
#define AFX_FAVTREE_H__CC415E43_148C_11D4_9BB0_CFFDB04BA878__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FavTree.h : header file
//

#include "BaseExpObj.h"
#include "FavWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CFavTree window

class CFavTree : public CBaseExpObj
{
// Construction
public:
	CFavTree();
	CFavWnd m_wndFav;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFavTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	//DWORD m_dwHidden;
//	BOOL ResolveLink(LPCSTR lpszLinkFile, LPSTR lpszPath);
	int BuildFavorites(LPCTSTR pszPath, int nStartPos, HTREEITEM hItem, int nLevel=1);
	BOOL Update(int nLevel);
	virtual void Destroy();
	LPCSTR GetTitle();
	virtual ~CFavTree();
	CString strTitle;
	virtual BOOL Initialize();
	virtual BOOL Create(CWnd* pParent);
	virtual CWnd* GetWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFavTree)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

private:
	BOOL m_bFirstInitial;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAVTREE_H__CC415E43_148C_11D4_9BB0_CFFDB04BA878__INCLUDED_)
