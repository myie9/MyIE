#if !defined(AFX_FAVTREECTRL_H__A733B004_1724_11D4_9BCE_0000E85300AE__INCLUDED_)
#define AFX_FAVTREECTRL_H__A733B004_1724_11D4_9BCE_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FavTreeCtrl.h : header file
//
#include "MyIE9Helper.h"
#include "BaseExpObj.h"

/////////////////////////////////////////////////////////////////////////////
// CFavTreeCtrl window

class CFavTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CFavTreeCtrl();

// Attributes
public:
	CStringList m_astrFav;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFavTreeCtrl)
	//}}AFX_VIRTUAL

protected:
	CString m_strOldText;
	UINT    m_nScrollTimer, m_nExpandTimer;
	UINT    m_nTimerTicks;

// Implementation
public:
	void MoveSubMenuOrder(LPCSTR source, LPCSTR target);
	int m_HalfItemHeight;
	BOOL m_bMoveIntoFolder;
	void GetItemPath(HTREEITEM hti, CString& path);
	HTREEITEM m_hDragItem, m_hDropItem, m_hOldDropItem;
	BOOL m_bDragging;
	int ExpandNode(HTREEITEM hItem);
	BOOL GetURL(CString& path, CString& url);
	HTREEITEM m_hStartFolder;
	HTREEITEM m_hItem;
	virtual ~CFavTreeCtrl();
	CBaseExpObj* m_pFavTree;
	CImageList* pImageList;

	// Generated message map functions
protected:
	void BuildMenuOrder(CString& path, HTREEITEM hParent);
	//{{AFX_MSG(CFavTreeCtrl)
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRfavCopy();
	afx_msg void OnRfavReplace();
	afx_msg void OnRfavDel();
	afx_msg void OnRfavOpenall();
	afx_msg void OnRfavStartFolder();
	afx_msg void OnRfavStart();
	afx_msg void OnRfavOpenNew();
	afx_msg void OnRfavAddshortcut();
	afx_msg void OnRfavAlias();
	afx_msg void OnRfavfolderFind();
	afx_msg void OnRfavProperty();
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnRfavfolderReset();
	afx_msg void OnRfavRename();
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRfavfolderOnly();
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnAddtoGroup(UINT nID);
	DECLARE_MESSAGE_MAP()
private:
	CString GetCurrentItemFile();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAVTREECTRL_H__A733B004_1724_11D4_9BCE_0000E85300AE__INCLUDED_)
