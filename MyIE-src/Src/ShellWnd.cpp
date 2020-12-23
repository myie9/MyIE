// ShellWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "ShellWnd.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )

#define ID_SHELLWND_COMBO	101

/////////////////////////////////////////////////////////////////////////////
// CShellWnd

CShellWnd::CShellWnd()
{
}

CShellWnd::~CShellWnd()
{
}


BEGIN_MESSAGE_MAP(CShellWnd, CWnd)
	//{{AFX_MSG_MAP(CShellWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(IDOK, OnRefresh)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(ID_SHELLWND_COMBO, OnComboSelChange)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CShellWnd message handlers

int CShellWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ShellTreeCtrl.Create(WS_CHILD|WS_VISIBLE|TVS_SHOWSELALWAYS|TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT|TVS_TRACKSELECT,
		CRect(0,0,0,0), this, 100);
	m_ShellTreeCtrl.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	// bring the tooltips to front
	CWnd* pTT = m_ShellTreeCtrl.FromHandle((HWND) m_ShellTreeCtrl.SendMessage(TVM_GETTOOLTIPS));
    if (pTT != NULL)
		pTT->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,
			SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);

	if (!m_fType.Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWN|CBS_LOWERCASE, CRect(0,0,0,0), this, ID_SHELLWND_COMBO))
	{
		return -1;      // fail to create
	}
	//m_fType.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	CFont* font = m_ShellTreeCtrl.GetFont();
	m_fType.SetFont(font);
	m_fType.AddString("*.htm;*.html;*.shtm;*.shtml;*.mht;*.txt;");
	m_fType.AddString("*.txt;");
	m_fType.AddString("*.htm;*.html;");
	m_fType.AddString("*.c;*.cpp;*.h;");
	m_fType.AddString("*.bmp;*.jpg;*.gif;*.png;");
	m_fType.AddString("*.*");
	//
	CImageList img;
	m_wndRef.CreateEx(this);
	img.Create(13, 15, ILC_COLORDDB|ILC_MASK, 1, 1);
	HBITMAP hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_REFRESH));
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);
	m_wndRef.GetToolBarCtrl().SetImageList(&img);
	img.Detach();

	m_wndRef.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT|TBSTYLE_TOOLTIPS  );
	m_wndRef.SetButtonInfo(0, IDOK, TBSTYLE_BUTTON, 0);
	m_wndRef.GetToolBarCtrl().GetToolTips()->UpdateTipText(IDS_REFRESH, this, IDOK);

	return 0;
}

void CShellWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	int nSize= 23;//button size
	m_fType.MoveWindow(0, 1, cx-nSize+2, 150);//nSize-2);
	m_wndRef.MoveWindow(cx-nSize+2, 1, nSize, nSize);
	m_ShellTreeCtrl.MoveWindow(0,nSize-2,cx,cy-nSize+2);
}

void CShellWnd::OnRefresh()
{
	CString str;
	if(m_ShellTreeCtrl.GetCurFilePath(str))
	{
		LockWindowUpdate();
		m_fType.GetWindowText(m_ShellTreeCtrl.m_strFilter);
		if(m_ShellTreeCtrl.m_strFilter.Right(1) != ";")
			m_ShellTreeCtrl.m_strFilter += ";";
		m_ShellTreeCtrl.DeleteAllItems();
		m_ShellTreeCtrl.PopulateTree();
		m_ShellTreeCtrl.TunnelTree(str);
		UnlockWindowUpdate();
	}
}
void CShellWnd::OnComboSelChange()
{
	CString str;
	m_fType.GetLBText(m_fType.GetCurSel(), str); 
	m_fType.SetWindowText(str);
	OnRefresh();
}
//#pragma optimize( "s", off )

