// FavWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "FavWnd.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CFavWnd

CFavWnd::CFavWnd()
{
	m_bFolderOnly = FALSE;
}

CFavWnd::~CFavWnd()
{
}


BEGIN_MESSAGE_MAP(CFavWnd, CWnd)
	//{{AFX_MSG_MAP(CFavWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_FAVORITES_ORGNIZEFAVORITES, OnFavoritesOrgnizefavorites)
	ON_COMMAND(ID_FAVORITES_ADDTOFAVORITES, OnFavoritesAddtofavorites)
	ON_COMMAND(ID_FAVORITES_BEST, OnFavoritesBest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFavWnd message handlers

int CFavWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_FavTreeCtrl.Create(WS_CHILD|WS_VISIBLE|TVS_SHOWSELALWAYS|TVS_LINESATROOT|
		TVS_FULLROWSELECT |TVS_TRACKSELECT | TVS_EDITLABELS,
		CRect(0,0,0,0), this, 100);
	m_FavTreeCtrl.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	// bring the tooltips to front
	CWnd* pTT = m_FavTreeCtrl.FromHandle((HWND) m_FavTreeCtrl.SendMessage(TVM_GETTOOLTIPS));
    if (pTT != NULL)
		pTT->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,
			SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);

	InitImage();
	//
	if (!m_ToolBar.CreateEx(this))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	// set up toolbar properties
	m_ToolBar.GetToolBarCtrl().SetButtonWidth(16, 100);
	CImageList img;
	img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 3, 1);
	HBITMAP hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_FAV_EXP));
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);
	m_ToolBar.SendMessage(TB_SETIMAGELIST, 0, (LPARAM)img.m_hImageList);
	img.Detach();

	m_ToolBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_LIST);
	m_ToolBar.SetButtons(NULL, 3);

	// set up each toolbar button
	CString str;
	LOADSTR(str ,IDS_FAV_ADD);
	m_ToolBar.SetButtonInfo(0, ID_FAVORITES_ADDTOFAVORITES, TBSTYLE_BUTTON|TBSTYLE_AUTOSIZE, 0);
	m_ToolBar.SetButtonText(0, str);
	
	LOADSTR(str ,IDS_FAV_ORG);
	m_ToolBar.SetButtonInfo(1, ID_FAVORITES_ORGNIZEFAVORITES, TBSTYLE_BUTTON|TBSTYLE_AUTOSIZE, 1);
	m_ToolBar.SetButtonText(1, str);

	LOADSTR(str ,IDS_FAV_MOST);
	m_ToolBar.SetButtonInfo(2, ID_FAVORITES_BEST, TBSTYLE_BUTTON|TBSTYLE_AUTOSIZE, 2);
	m_ToolBar.SetButtonText(2, str);

	m_ToolBar.SetBarStyle(m_ToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);

	CRect rectToolBar;
	// set up toolbar button sizes
	m_ToolBar.GetItemRect(1, &rectToolBar);
	m_ToolBar.SetSizes(rectToolBar.Size(), CSize(16,14));

	if (!m_Rebar.Create(this,RBS_BANDBORDERS|RBS_DBLCLKTOGGLE | RBS_REGISTERDROP |RBS_VARHEIGHT ,CCS_NODIVIDER |CCS_NOPARENTALIGN |WS_BORDER |WS_CHILD |WS_CLIPCHILDREN |WS_CLIPSIBLINGS |WS_VISIBLE | CBRS_TOP))
	{
		return -1;      // fail to create
	}

	m_Rebar.AddBar(&m_ToolBar);
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE|RBBIM_STYLE;
	rbbi.fStyle = RBBS_NOGRIPPER;
	rbbi.cxMinChild = rectToolBar.Width();
	rbbi.cyMinChild = rectToolBar.Height();
	rbbi.cx = rbbi.cxIdeal = rectToolBar.Width()*2;
	m_Rebar.GetReBarCtrl().SetBandInfo(0, &rbbi);
	return 0;
}

void CFavWnd::InitImage()
{
	CImageList img;
	img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 3, 1);
	HBITMAP hbmp = pmf->GetBitmap("FavBar.bmp");
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);
	m_FavTreeCtrl.SetImageList(&img, TVSIL_NORMAL);
	img.Detach();
}

void CFavWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_Rebar.MoveWindow(1,1, cx-1,26);
	m_FavTreeCtrl.MoveWindow(0,27,cx,cy-27);
}

void CFavWnd::OnFavoritesOrgnizefavorites() 
{
	// TODO: Add your command handler code here
	pmf->SendMessage(WM_COMMAND, ID_FAVORITES_ORGNIZEFAVORITES, 0);
}

void CFavWnd::OnFavoritesAddtofavorites() 
{
	// TODO: Add your command handler code here
	pmf->SendMessage(WM_COMMAND, ID_FAVORITES_ADDTOFAVORITES, 0);
}

void CFavWnd::OnFavoritesBest() 
{
	// TODO: Add your command handler code here
	m_bFolderOnly = !m_bFolderOnly;

	pmf->m_pwndExpObj->Update(1);
	m_ToolBar.GetToolBarCtrl().CheckButton(ID_FAVORITES_BEST, m_bFolderOnly);
}

//#pragma optimize( "s", off)
