#if !defined(AFX_SHELL_H__A733B004_1724_11D4_9BCE_0000E85300AE__INCLUDED_)
#define AFX_SHELL_H__A733B004_1724_11D4_9BCE_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShellTreeCtrl.h : header file
//
#include "MyIE9Helper.h"
#include "BaseExpObj.h"
#include <shlobj.h>
#include "ShellPidl.h"

/////////////////////////////////////////////////////////////////////////////
// CShellTreeCtrl window

class CShellTreeCtrl : public CTreeCtrl, public CShellPidl
{
// Construction
public:
	CShellTreeCtrl();

	enum FindAttribs{type_drive,type_folder,type_file};

// Attributes
public:
	BOOL m_bOutaHere;

// Operations
public:
	void	PopulateTree();
	void	OnFolderExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	void	GetContextMenu(NMHDR* pNMHDR, LRESULT* pResult);
	BOOL	OnFolderSelected(NMHDR* pNMHDR, LRESULT* pResult, CString &szFolderPath);
	void	OnDeleteShellItem(NMHDR* pNMHDR, LRESULT* pResult);
	void	EnableImages();
	bool	SearchTree(HTREEITEM treeNode, CString szSearchName, FindAttribs attr);
	void	TunnelTree(CString szFindPath);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShellTreeCtrl)
	//}}AFX_VIRTUAL

protected:

// Implementation
public:
	BOOL GetCurFilePath(CString& str);
	virtual ~CShellTreeCtrl();
	CBaseExpObj* m_pShellTree;
	CImageList* pImageList;
	BOOL MatchExtensionFilter(LPSTR lpFileName);
	BOOL m_bDispFile;
	CString m_strFilter;

	// Generated message map functions
protected:
	void	FillTreeView(LPSHELLFOLDER lpsf, LPITEMIDLIST  lpifq, HTREEITEM     hParent);
	void	GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem);
 	static int CALLBACK TreeViewCompareProc(LPARAM, LPARAM, LPARAM);

	//{{AFX_MSG(CShellTreeCtrl)
	//}}AFX_MSG
	void	OnClick(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHELL_H__A733B004_1724_11D4_9BCE_0000E85300AE__INCLUDED_)
