#if !defined(AFX_HISTORYTREE_H__CC415E43_148C_11D4_9BB0_CFFDB04BA878__INCLUDED_)
#define AFX_HISTORYTREE_H__CC415E43_148C_11D4_9BB0_CFFDB04BA878__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HistoryTree.h : header file
//

#include "BaseExpObj.h"
#include <UrlHist.h>
#include "HisTreeCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CHistoryTree window

class CHistoryTree : public CBaseExpObj
{
// Construction
public:
	CHistoryTree();
	CHisTreeCtrl m_wndHisTree;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistoryTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bIE4;
	void AddHistory(IShellFolder* pFolder,HTREEITEM root, int nImg=0, int nOImg=0, BOOL bNeedCmp=FALSE);
	BOOL StrRetToStr(STRRET StrRet, LPTSTR* str, LPITEMIDLIST pidl);
	BOOL Update(int nLevel);
//for ie4 and win95
	CTime montime;
	int today;
	HTREEITEM FindAndInsert(HTREEITEM hRoot, LPCSTR text, int nImg, int nOImg, int data=0);
	BOOL insertUrl(LPSTATURL pSUrl,int ind);
//end
	virtual void Destroy();
	LPCSTR GetTitle();
	virtual ~CHistoryTree();
	CString strTitle;
	virtual BOOL Initialize();
	virtual BOOL Create(CWnd* pParent);
	virtual CWnd* GetWindow();
	IMalloc * m_pMalloc;
	BOOL ResolveHistory(IShellFolder* pFolder, LPCITEMIDLIST pidl, LPTSTR* lpszURL);

	// Generated message map functions
protected:
	//{{AFX_MSG(CHistoryTree)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTORYTREE_H__CC415E43_148C_11D4_9BB0_CFFDB04BA878__INCLUDED_)
