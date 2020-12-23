// ShellTree.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "ShellTree.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CShellTree

CShellTree::CShellTree()
{
}

CShellTree::~CShellTree()
{
	m_wndShell.m_ShellTreeCtrl.DeleteAllItems();
}



/////////////////////////////////////////////////////////////////////////////
// CShellTree message handlers

LPCSTR CShellTree::GetTitle()
{
	LOADSTR(strTitle ,IDS_TITLE_SHELL);
	return (LPCSTR)strTitle;
}

UINT _cdecl InitTree(LPVOID Tree)
{
	if(Tree == NULL)
		return 0;

	CShellTree *pShell = (CShellTree*)Tree;
	try{
		pShell->m_wndShell.LockWindowUpdate();
		pShell->m_wndShell.m_ShellTreeCtrl.PopulateTree();

		if(((CMyIE9App*)AfxGetApp())->m_strDefaultDir != "")
			pShell->m_wndShell.m_ShellTreeCtrl.TunnelTree(((CMyIE9App*)AfxGetApp())->m_strDefaultDir);

		pShell->m_wndShell.UnlockWindowUpdate();
	}catch(...)
	{
	}

	return 0;
}

BOOL CShellTree::Initialize()
{
	// TODO: Add your command handler code here
	m_wndShell.m_ShellTreeCtrl.EnableImages();
	m_wndShell.m_fType.SetWindowText(m_wndShell.m_ShellTreeCtrl.m_strFilter);

	AfxBeginThread(InitTree, (LPVOID)this);

	return TRUE;
}

BOOL CShellTree::Create(CWnd* pParent)
{
	m_wndShell.Create(NULL,NULL, WS_CHILD|WS_VISIBLE,CRect(0,0,0,0), pParent, 100);
	m_wndShell.m_ShellTreeCtrl.m_pShellTree = this;
	return TRUE;
}

CWnd* CShellTree::GetWindow()
{
	return (CWnd*)&m_wndShell;
}

void CShellTree::Destroy()
{
	try{
	m_wndShell.m_ShellTreeCtrl.DeleteAllItems();
	}catch(...){}
}

BOOL CShellTree::Update(int nLevel)
{
	return TRUE;
}


