#include "stdafx.h"
#include "MyIE9.h"
#include "ChevBar.h"
#include "ToolMenu.h"
#include "Menudata.h"
#include "MainFrm.h"
#include <wininet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChevBar

CChevBar::CChevBar()
{
	SetBorders();
}

BEGIN_MESSAGE_MAP(CChevBar, CControlBar)
	//{{AFX_MSG_MAP(CChevBar)
	ON_WM_NCCREATE()
	ON_WM_PAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_NOTIFY_REFLECT(RBN_HEIGHTCHANGE, OnHeightChange)
	ON_NOTIFY_REFLECT(RBN_ENDDRAG, OnHeightChange)
	ON_MESSAGE(RB_SHOWBAND, OnShowBand)
	ON_MESSAGE_VOID(WM_RECALCPARENT, OnRecalcParent)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT( RBN_CHEVRONPUSHED, OnChevronPushed )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChevBar

void CChevBar::OnChevronPushed( NMHDR * pNotifyStruct, LRESULT* result )
{
	NMREBARCHEVRON* pChev = (NMREBARCHEVRON*) pNotifyStruct;

	// Has the band of the chevron that generated this message
	int	iBand = pChev->uBand;
	//gf int	FavSize;

	// Have to get the child window handle this band holds
	REBARBANDINFO	rbinfo;
	rbinfo.cbSize = sizeof(rbinfo);
	rbinfo.fMask = RBBIM_CHILD | RBBIM_ID;
	GetReBarCtrl().GetBandInfo ( iBand, &rbinfo );

	// Check if the child window is a toolbar
	// Some rebar bands may have other windows, so that is left as an exercise
	// to the user :-)
	CToolBar *pTBar = (CToolBar *)CToolBar::FromHandle ( rbinfo.hwndChild );
	ASSERT(pTBar);
	ASSERT(pTBar->IsKindOf(RUNTIME_CLASS(CToolBar)));

	// Create a popup menu to show hidden buttons
	CToolMenu	pop;
	pop.CreatePopupMenu ();

	// The CToolMenu uses the toolbar pointer to get associated text and image
	// information. If there is a better way please do so.
	// This menu was created with local scope, hence the temp pointer
	// pTBar can be stored i guess...
	pop.m_pImg = pTBar->GetToolBarCtrl().GetImageList();

	// We have to determine the hidden buttons
	//	1. Obtain band rectangle (subtract chevron width if you want to)
	//	2. Get the button rectangle
	//	3. Check if the intersecting rectangle is same as the button rectangle
	//	4. If they intersect then the button is shown

	CRect rectTool;
	pTBar->GetWindowRect(&rectTool);

	// It depends on the user to decide if this subtraction is necessary, if they feel
	// happy about the chevron hiding a part of a button then remove this code
	CRect rectChevron;
	rectChevron = pChev->rc;

	// Screen co-ordinates for Menu to be displayed
	CPoint	ptMenu;
	ptMenu.x = rectChevron.left;
	ptMenu.y = rectChevron.bottom;
	ClientToScreen ( &ptMenu );

	// This flag indicates if atleast one has been added to the menu
	// POPUP Menu is shown only if atleast one item has to be shown
	BOOL	bAtleastOne=FALSE;
	int iCount, iButtonCount = pTBar->GetToolBarCtrl().GetButtonCount();
	UINT iStyle, id;
	for ( iCount = 0 ; iCount < iButtonCount ; iCount++ )
	{
		// Get the id of the toolbar button
		id = pTBar->GetItemID( iCount );
		// If the button is a separator then we can also add a separator to the
		// popup menu
		if (  pTBar->GetButtonStyle ( iCount ) & TBSTYLE_SEP )
		{
			// It wouldnt be nice if there is a separator as the first item in the menu
			if ( bAtleastOne )
				pmf->MenuAppendSeparator(&pop); //pop.AppendMenu ( MF_SEPARATOR );
		}
		else
		{
			// Get the button rectangle
			CRect rectButton;
			pTBar->GetItemRect ( iCount, &rectButton );
			// Check the intersection of the button and the band
			//CRect interRect;
			//interRect.IntersectRect ( &rectButton, &rectBand );

			// if the intersection is not the same as button then
			// the button is not completely visible, so add to menu
			//if ( rectButton.right >= rectBand.right-10 )
			if ( rectButton.right >= rectTool.Width() )
			{
				UINT	iMenuStyle;
				
				// Have to reflect the state of the menu item, so check the state of the 
				// button and enable or disable the items
				if ( pTBar->GetToolBarCtrl().IsButtonEnabled ( id ) )
					iMenuStyle = MF_ENABLED;
				else
					iMenuStyle = MF_DISABLED;
				if ( pTBar->GetToolBarCtrl().IsButtonPressed ( id ) || pTBar->GetToolBarCtrl().IsButtonChecked ( id ))
					iMenuStyle |= MF_CHECKED;
				// Add the item to the menu with the id of the toolbar button
				// This id is used to notify the parent if the user selects that item
				CMenuData * md= new CMenuData;
				md->m_pImgList = pop.m_pImg;
				if(rbinfo.wID == ID_VIEW_LINKS)
				{
					if( id == ID_LINK_POPUP)
					{
						iMenuStyle |= MF_POPUP;
						TBBUTTON tbb;
						if( pTBar->GetToolBarCtrl().GetButton( iCount,  &tbb ))
						{
							if(IsMenu((HMENU)tbb.dwData))
							{
								pTBar->GetButtonInfo ( iCount, id, iStyle, md->m_nBitmap);
								SetMenuText(g_astrLinks[iCount], md);
								HMENU hmenu= CopyMenu((HMENU)tbb.dwData);
								pop.AppendMenu ( iMenuStyle|MF_OWNERDRAW, (UINT)hmenu, (char*)md );//spaces+g_astrLinks[iCount-1]
							}
							else
								delete md;
						}
					}
					else
					{
						pTBar->GetButtonInfo ( iCount, id, iStyle, md->m_nBitmap);
						SetMenuText(g_astrLinks[iCount], md);
						pop.AppendMenu ( iMenuStyle|MF_OWNERDRAW, id , (char*)md );
					}
				}
				else if(rbinfo.wID == ID_VIEW_FAVS)
				{
					if(id == ID_FAV_POPUP)
					{
//gf						FavSize=pmf->m_nFavSize;
						iMenuStyle |= MF_POPUP;
						TBBUTTON tbb;
						if( pTBar->GetToolBarCtrl().GetButton( iCount,  &tbb ))
						{
							if(IsMenu((HMENU)tbb.dwData))
							{
								pTBar->GetButtonInfo ( iCount, id, iStyle, md->m_nBitmap);
								SetMenuText(g_astrFavs[iCount], md);
								HMENU hmenu= CopyMenu((HMENU)tbb.dwData);
								pop.AppendMenu ( iMenuStyle|MF_OWNERDRAW, (UINT)hmenu, (char*)md );//spaces+g_astrLinks[iCount-1]
							}
							else
								delete md;
						}
					}
					else if ( pmf->m_bFavShowUrl )
					{
						pTBar->GetButtonInfo ( iCount, id, iStyle, md->m_nBitmap);
						SetMenuText(g_astrFavs[iCount], md);
						pop.AppendMenu ( iMenuStyle|MF_OWNERDRAW, id , (char*)md );
					}
				}
				else if(rbinfo.wID == ID_VIEW_EX_TOOL)
				{
					pTBar->GetButtonInfo ( iCount, id, iStyle, md->m_nBitmap);
					SetMenuText(pmf->m_ExternalUtilList.m_UtilList.GetAt(iCount)->m_strUtilName, md);
					pop.AppendMenu ( iMenuStyle|MF_OWNERDRAW, id , (char*)md );
				}
				else if(rbinfo.wID == ID_VIEW_TOOLBAR)
				{
					LOADSTR(strText,id);
					if ( strText.GetLength() )
					{
						int iPos = strText.Find ( '\n', 0 );
						if ( iPos != -1 )
						{
							strText = strText.Mid( iPos+1 );
						}
					}
					if( pTBar->GetButtonStyle ( iCount ) & TBSTYLE_DROPDOWN && (id!= ID_EDIT_EDIT && id!=ID_TOOLS_EMAIL && id!=ID_GO_BACK && id!=ID_GO_FORWARD) || id == ID_GROUP)
					{
						iMenuStyle |= MF_POPUP;
						TBBUTTON tbb;
						if( pTBar->GetToolBarCtrl().GetButton( iCount,  &tbb ))
						{
							pTBar->GetButtonInfo ( iCount, id, iStyle, md->m_nBitmap);
							SetMenuText(strText, md);
							CMenu menu;
							CMenu* pPopup;
							if(id== ID_FILE_SAVEAS)
							{
								pPopup = SMP_SAVE;
							}
							else if(id == ID_FONT_DROPDOWN)
							{
								pPopup = SMP_FONTS;
							}
							else if (id == ID_ENCODE_DROPDOWN)
							{
								pPopup = SMP_ENCODE;
							}
							else if(id== ID_NEWWINDOW)
							{
								pPopup = SMP_NEW; //gfm
							}
							else if(id== ID_TOOLS_EXTERNAL_AUTORUN)
							{
								pPopup = SMP_EXTOOLS;
							}
							else if(id== ID_TOOLS_SETBG)
							{
								pPopup = SMP_BGCOLOR;
							}
							else if(id== ID_GROUP)
							{
								g_bRTab=0;
								pPopup = SMP_GROUP;
							}
							else if(id== ID_OPTIONS_LOADIMAGE)
							{
								pPopup = SMP_DOWNLOAD;
							}
							else if(id== ID_TOOLS_AUTOSCROLL_USEAUTOSCROLLING)
							{
								pPopup = SMP_AUTOSCROLL;
							}
							else if(id== ID_EDIT_UNDOCLOSE)
							{
								pPopup = SMP_UNDO;
							}
							else if(id== ID_TOOLS_ALL)
							{
								pPopup = SMP_CLEAN;
							}
							else if(id== ID_SC_FILLFORM)
							{
								pPopup = SMP_AUTOFILL;
							}
							else if(id == ID_OPTIONS_USE_PROXY)
							{
								menu.CreatePopupMenu();
								pmf->BuildProxyMenu(&menu);
								pPopup = &menu;
							}
							else if(id == ID_OPTIONS_USE_WEBP)
							{
								menu.CreatePopupMenu();
								pmf->BuildWebProxyMenu(&menu, 2);
								pPopup = &menu;
							}
							else if(id == ID_GO_UP_PAGE)
							{
								menu.CreatePopupMenu();
								pmf->BuildGoUpMenu(&menu);
								pPopup = &menu;
							}

							HMENU hmenu = CopyMenu(pPopup->m_hMenu);
							pop.AppendMenu ( iMenuStyle|MF_OWNERDRAW, (UINT)hmenu, (char*)md );
							menu.DestroyMenu();
						}
					}
					else
					{
						pTBar->GetButtonInfo ( iCount, id, iStyle, md->m_nBitmap);
						SetMenuText(strText, md);
						pop.AppendMenu ( iMenuStyle|MF_OWNERDRAW, id , (char*)md );
					}
				}
				else if(rbinfo.wID == ID_MENU_BAR)
				{
					if(pmf!=NULL)
					{
						pmf->GetMenu()->GetMenuString(iCount, str, MF_BYPOSITION);
						CMenu * pPopup;
						pPopup = pmf->GetMenu()->GetSubMenu(iCount);
						if(pPopup!=NULL)
						{
							HMENU hmenu = CopyMenu(pPopup->m_hMenu);
							pmf->MenuAppendOwnerItem( &pop, MF_POPUP, (UINT)hmenu, str );
						}
					}
				}
				// Yeah, have added one, so can show the menu
				bAtleastOne=TRUE;
			}
		}
	}
	// Show the menu if atleast one item has been added
	if ( bAtleastOne )
		pop.TrackPopupMenu ( TPM_LEFTALIGN|TPM_TOPALIGN, ptMenu.x, ptMenu.y, pmf);

	//gf À¬»øÈËÐ´µÄ
// 	if(rbinfo.wID == ID_VIEW_FAVS)
// 		pmf->m_nFavSize=FavSize;

	// Delete our menu
	pop.DestroyMenu ();
}

void CChevBar::OnHeightChange(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	// does the CChevBar have a frame?
	CFrameWnd* pFrameWnd = GetParentFrame();
	if (pFrameWnd != NULL)
	{
		PostMessage(WM_RECALCPARENT);
	}
	*pResult = 0;
}

LRESULT CChevBar::OnShowBand(WPARAM wParam, LPARAM)
{
	LRESULT lResult = Default();
	if (lResult)
	{
		// keep window visible state in sync with band visible state
		REBARBANDINFO rbBand;
		rbBand.cbSize = sizeof(rbBand);
		rbBand.fMask = RBBIM_CHILD|RBBIM_STYLE;
		VERIFY(DefWindowProc(RB_GETBANDINFO, wParam, (LPARAM)&rbBand));
		CControlBar* pBar = DYNAMIC_DOWNCAST(CControlBar, CWnd::FromHandlePermanent(rbBand.hwndChild));
		BOOL bWindowVisible;
		if (pBar != NULL)
			bWindowVisible = pBar->IsVisible();
		else
			bWindowVisible =  (::GetWindowLong(rbBand.hwndChild, GWL_STYLE) & WS_VISIBLE) != 0;
		BOOL bBandVisible = (rbBand.fStyle & RBBS_HIDDEN) == 0;
		if (bWindowVisible != bBandVisible)
			VERIFY(::ShowWindow(rbBand.hwndChild, bBandVisible ? SW_SHOW : SW_HIDE));
	}
	return lResult;
}

BOOL CChevBar::_AddBar(CWnd* pBar, REBARBANDINFO* pRBBI)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pBar != NULL);
	ASSERT(::IsWindow(pBar->m_hWnd));

	pRBBI->cbSize = sizeof(REBARBANDINFO);
	pRBBI->fMask |= RBBIM_CHILD | RBBIM_CHILDSIZE;
	pRBBI->hwndChild = pBar->m_hWnd;

	CSize size;
	CControlBar* pTemp = DYNAMIC_DOWNCAST(CControlBar, pBar);
	if (pTemp != NULL)
	{
		size = pTemp->CalcFixedLayout(FALSE, m_dwStyle & CBRS_ORIENT_HORZ);
	}
	else
	{
		CRect rect;
		pBar->GetWindowRect(&rect);
		size = rect.Size();
	}
	//WINBUG: COMCTL32.DLL is off by 4 pixels in its sizing logic.  Whatever
	//  is specified as the minimum size, the system rebar will allow that band
	//  to be 4 actual pixels smaller!  That's why we add 4 to the size here.
	pRBBI->cxMinChild = size.cx ; //(_afxComCtlVersion < VERSION_IE401 ? 4 : 0);
	pRBBI->cyMinChild = size.cy ;
	BOOL bResult = (BOOL)DefWindowProc(RB_INSERTBAND, (WPARAM)-1, (LPARAM)pRBBI);

	CFrameWnd* pFrameWnd = GetParentFrame();
	if (pFrameWnd != NULL)
		pFrameWnd->RecalcLayout();

	return bResult;
}

BOOL CChevBar::AddBar(CWnd* pBar, LPCTSTR pszText, CBitmap* pbmp, DWORD dwStyle)
{
	REBARBANDINFO rbBand;
	rbBand.fMask = RBBIM_STYLE;
	rbBand.fStyle = dwStyle;
	if (pszText != NULL)
	{
		rbBand.fMask |= RBBIM_TEXT;
		rbBand.lpText = const_cast<LPTSTR>(pszText);
	}
	if (pbmp != NULL)
	{
		rbBand.fMask |= RBBIM_BACKGROUND;
		rbBand.hbmBack = (HBITMAP)*pbmp;
	}
	return _AddBar(pBar, &rbBand);
}

BOOL CChevBar::AddBar(CWnd* pBar, COLORREF clrFore, COLORREF clrBack, LPCTSTR pszText, DWORD dwStyle)
{
	REBARBANDINFO rbBand;
	rbBand.fMask = RBBIM_STYLE | RBBIM_COLORS;
	rbBand.fStyle = dwStyle;
	rbBand.clrFore = clrFore;
	rbBand.clrBack = clrBack;
	if (pszText != NULL)
	{
		rbBand.fMask |= RBBIM_TEXT;
		rbBand.lpText = const_cast<LPTSTR>(pszText);
	}
	return _AddBar(pBar, &rbBand);
}

BOOL CChevBar::Create(CWnd* pParentWnd, DWORD dwCtrlStyle, DWORD dwStyle, UINT nID)
{
	ASSERT_VALID(pParentWnd);   // must have a parent
	ASSERT (!((dwStyle & CBRS_SIZE_FIXED) && (dwStyle & CBRS_SIZE_DYNAMIC)));

	// save the style
	m_dwStyle = (dwStyle & CBRS_ALL);
	if (nID == AFX_IDW_REBAR)
		m_dwStyle |= CBRS_HIDE_INPLACE;

	dwStyle &= ~CBRS_ALL;
	dwStyle |= CCS_NOPARENTALIGN|CCS_NOMOVEY|CCS_NODIVIDER|CCS_NORESIZE|RBS_VARHEIGHT;
	dwStyle |= dwCtrlStyle;

	// initialize common controls
	VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_COOL_REG));

	// create the HWND
	CRect rect; rect.SetRectEmpty();
	if (!CWnd::Create(REBARCLASSNAME, NULL, dwStyle, rect, pParentWnd, nID))
		return FALSE;

	return TRUE;
}

BOOL CChevBar::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!CControlBar::OnNcCreate(lpCreateStruct))
		return FALSE;

	// if the owner was set before the rebar was created, set it now
	if (m_hWndOwner != NULL)
		DefWindowProc(RB_SETPARENT, (WPARAM)m_hWndOwner, 0);

	return TRUE;
}

void CChevBar::OnRecalcParent()
{
	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT(pFrameWnd != NULL);
	pFrameWnd->RecalcLayout();
}

void CChevBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler)
{
	//UpdateDialogControls(pTarget, bDisableIfNoHandler);
}

CSize CChevBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	// the union of the band rectangles is the total bounding rect
	int nCount = DefWindowProc(RB_GETBANDCOUNT, 0, 0);
	REBARBANDINFO rbBand;
	rbBand.cbSize = sizeof(rbBand);
	int nTemp;

	try{
	// sync up hidden state of the bands
	for (nTemp = nCount; nTemp--; )
	{
		rbBand.fMask = RBBIM_CHILD|RBBIM_STYLE;
		VERIFY(DefWindowProc(RB_GETBANDINFO, nTemp, (LPARAM)&rbBand));
		CControlBar* pBar = DYNAMIC_DOWNCAST(CControlBar, CWnd::FromHandlePermanent(rbBand.hwndChild));
		BOOL bWindowVisible;
		if (pBar != NULL)
			bWindowVisible = pBar->IsVisible();
		else
			bWindowVisible =  (::GetWindowLong(rbBand.hwndChild, GWL_STYLE) & WS_VISIBLE) != 0;
		BOOL bBandVisible = (rbBand.fStyle & RBBS_HIDDEN) == 0;
		if (bWindowVisible != bBandVisible)
			VERIFY(DefWindowProc(RB_SHOWBAND, nTemp, bWindowVisible));
	}
	}catch(...){}

	// determine bounding rect of all visible bands
	CRect rectBound; rectBound.SetRectEmpty();
	try{
	for (nTemp = nCount; nTemp--; )
	{
		rbBand.fMask = RBBIM_STYLE;
		VERIFY(DefWindowProc(RB_GETBANDINFO, nTemp, (LPARAM)&rbBand));
		if ((rbBand.fStyle & RBBS_HIDDEN) == 0)
		{
			CRect rect;
			VERIFY(DefWindowProc(RB_GETRECT, nTemp, (LPARAM)&rect));
			rectBound |= rect;
		}
	}
	// add borders as part of bounding rect
	if (!rectBound.IsRectEmpty())
	{
		CRect rect; rect.SetRectEmpty();
		CalcInsideRect(rect, bHorz);
		rectBound.right -= rect.Width();
		if(m_dwStyle & CBRS_ALIGN_TOP)
			rectBound.bottom -= rect.Height()-2;
		else
			rectBound.bottom -= rect.Height();
	}
	}catch(...){}

	return CSize( (bHorz && bStretch) ? 32767 : rectBound.Width(),
				 (!bHorz && bStretch) ? 32767 : rectBound.Height());
}

CSize CChevBar::CalcDynamicLayout(int /*nLength*/, DWORD dwMode)
{
	if (dwMode & LM_HORZDOCK)
		ASSERT(dwMode & LM_HORZ);
	return CalcFixedLayout(dwMode & LM_STRETCH, dwMode & LM_HORZ);
}

BOOL CChevBar::OnEraseBkgnd(CDC*)
{
	return (BOOL)Default();
}

void CChevBar::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS* lpncsp)
{
	// calculate border space (will add to top/bottom, subtract from right/bottom)
	CRect rect; rect.SetRectEmpty();
	BOOL bHorz = (m_dwStyle & CBRS_ORIENT_HORZ) != 0;
	CControlBar::CalcInsideRect(rect, bHorz);

	// adjust non-client area for border space
	lpncsp->rgrc[0].left += rect.left;
	lpncsp->rgrc[0].top += rect.top;
	lpncsp->rgrc[0].right += rect.right;
	lpncsp->rgrc[0].bottom += rect.bottom;
}

void CChevBar::OnNcPaint()
{
	EraseNonClient();
	Default();
}

void CChevBar::OnPaint()
{
	Default();
}

LRESULT CChevBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	try
	{
	
	// special handling for certain messages (forwarding to owner/parent)
	switch (message)
	{
	case WM_POPMESSAGESTRING:
	case WM_SETMESSAGESTRING:
		return GetOwner()->SendMessage(message, wParam, lParam);
	}
	return CControlBar::WindowProc(message, wParam, lParam);

	}
	catch (...) {
		ASSERT(false);
		return CControlBar::WindowProc(message, wParam, lParam);
	}
}

IMPLEMENT_DYNAMIC(CChevBar, CControlBar)

HMENU CChevBar::CopyMenu(HMENU hMenu)
{
	int iCount;
	HMENU hCopyMenu = CreatePopupMenu();
	MENUITEMINFO info; 
	char lpNewItem[200];
	UINT state;

	for(iCount=0; iCount<GetMenuItemCount(hMenu);iCount++)
	{
	    info.cbSize = sizeof (MENUITEMINFO); // must fill up this field
	    info.fMask = MIIM_DATA|MIIM_TYPE|MIIM_ID|MIIM_SUBMENU;    // get the state of the menu item
	    info.dwTypeData = NULL;
		info.cch = 0;
		GetMenuItemInfo(hMenu, iCount ,TRUE , &info);
		GetMenuString(hMenu, iCount, lpNewItem, 200, MF_BYPOSITION);
		state = GetMenuState( hMenu, iCount, MF_BYPOSITION); 
 
		if(IsMenu(info.hSubMenu))
		{
			HMENU hNewMenu = CopyMenu(info.hSubMenu);
			if(info.fType & MFT_OWNERDRAW)
			{
				CMenuData * md1;
				md1 = new CMenuData;
				md1->m_nBitmap = ((CMenuData*)info.dwItemData)->m_nBitmap;
				md1->m_pImgList = ((CMenuData*)info.dwItemData)->m_pImgList;
				md1->m_bIni = ((CMenuData*)info.dwItemData)->m_bIni;
				SetMenuText (((CMenuData*)info.dwItemData)->m_szMenuText, md1);
				SetURL (((CMenuData*)info.dwItemData)->m_szURL, md1);
				AppendMenu(hCopyMenu, MF_POPUP|MF_ENABLED|MF_OWNERDRAW|info.fType, (UINT)hNewMenu, (char*)md1);
			}
			else
				AppendMenu(hCopyMenu, MF_POPUP|MF_ENABLED, (UINT)hNewMenu, lpNewItem);
		}
		else if(info.fType & MFT_OWNERDRAW)
		{
			CMenuData * md1;
			md1 = new CMenuData;
			md1->m_nBitmap = ((CMenuData*)info.dwItemData)->m_nBitmap;
			md1->m_pImgList = ((CMenuData*)info.dwItemData)->m_pImgList;
			md1->m_bIni = ((CMenuData*)info.dwItemData)->m_bIni;
			SetMenuText (((CMenuData*)info.dwItemData)->m_szMenuText, md1);
			SetURL (((CMenuData*)info.dwItemData)->m_szURL, md1);
			AppendMenu(hCopyMenu, MF_OWNERDRAW|info.fType, GetMenuItemID(hMenu, iCount),(char*)md1);
		}
		else
		{	int id = GetMenuItemID(hMenu, iCount);
			AppendMenu(hCopyMenu, MF_STRING|info.fType, id,lpNewItem);
			CheckMenuItem(hCopyMenu, id, state|MF_BYCOMMAND);
			EnableMenuItem(hCopyMenu, id, state);
		}
	}

	return hCopyMenu;
}
