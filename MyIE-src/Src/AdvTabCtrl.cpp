// AdvTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "AdvTabCtrl.h"
#include "MainFrm.h"
#include "Gf_AppStatic.h"
#include "Consts_gf.h"
#include "GfAppString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PT_IN_ICON(point, rect)		(point.x < rect.left + 20)
#define TAB_NEED_CAPTURE			(g_bClickClose==1 || (g_bClickClose==2 && pmf->m_nTabStyle!=2))

BOOL g_bCloseDown = FALSE;//click icon down
BOOL g_bShowCloseIcon = FALSE;//show close icon
int  g_nCloseTabID;

/////////////////////////////////////////////////////////////////////////////
// CAdvTabCtrl
//
CAdvTabCtrl::CAdvTabCtrl()
{
	_dwLastXTime = 0;
	m_bFirstTimeFocus = TRUE;
	
	m_bTracking= FALSE;
	
	m_hDragCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_POINTER_DRAG));
	TabCtrlItem.pszText = m_szText;
	TabCtrlItem.cchTextMax = MAX_PATH-1;
	m_crSelColour     = GetSysColor(COLOR_BTNTEXT); //RGB(0,0,255);
	m_crUnselColour   = GetSysColor(COLOR_GRAYTEXT);//COLOR_BTNTEXT); //RGB(50,50,50);
	m_crSelBKColour   = GetSysColor(COLOR_BTNFACE); //RGB(0,0,255);
	m_crUnselBKColour = RGB(214,211,206);//RGB(180,180,180);
	
	m_bDragging = FALSE;
	*m_szText = 0;
}

CAdvTabCtrl::~CAdvTabCtrl()
{	
}

BEGIN_MESSAGE_MAP(CAdvTabCtrl, CEnTabCtrl)
//{{AFX_MSG_MAP(CAdvTabCtrl)
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONUP()
ON_WM_LBUTTONDBLCLK()
ON_WM_MBUTTONDOWN()
ON_WM_LBUTTONDOWN()
ON_WM_RBUTTONUP()
ON_WM_PAINT()
ON_WM_RBUTTONDOWN()

ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvTabCtrl message handlers

LRESULT CAdvTabCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam) 
{
	m_bTracking= FALSE;
	
	m_bMoveToCloseIcon=FALSE;
	
	RECT rect;
	GetItemRect(GetCurSel(), &rect);
	
	rect.left = GetCloseIconLeft(rect);
	
	
	RedrawWindow(&rect);
	
	return 0;
	
}

void CAdvTabCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	
	m_bMoveToCloseIcon = FALSE;
	
	if (g_bCloseDown)
	{
		GetItem(g_nCloseTabID, &TabCtrlItem);
		CChildFrame* tcf = ((CChildFrame*)TabCtrlItem.lParam);
		if (!tcf)
			return;
		CMyIE9View *pView = tcf->m_pView;
		int id = GetTabIDFromPoint(point);
		RECT rect;
		GetItemRect(id,&rect);
		//
		if (id==g_nCloseTabID)
		{
			if ( PT_IN_ICON(point, rect) )
			{
				if (!g_bShowCloseIcon)
				{
					g_bShowCloseIcon = 1;
					pView->m_nOlderIcon = pView->m_nOldIcon;
					pmf->SetTabIcon(TI_CLOSE,id, pView);
				}
			}
			else
			{
				if (g_bShowCloseIcon)
				{
					g_bShowCloseIcon = 0;
					pmf->SetTabIcon( pView->m_nOlderIcon,g_nCloseTabID, pView);
				}
			}
		}
		else//else tab or not on tab
		{
			if (g_bShowCloseIcon)
			{
				g_bShowCloseIcon = 0;
				pmf->SetTabIcon( pView->m_nOlderIcon,g_nCloseTabID, pView);
			}
		}
	}
	else if(m_bDragging && (nFlags & MK_LBUTTON))
	{
		int id = GetTabIDFromPoint(point);
		if(id<0 || id==m_nDragTabID)
			SetCursor(m_hOldCursor);
		else
			SetCursor(m_hDragCursor);
	}
	
	//gfx
	if(CGf_AppStatic::_instance._bShowXIconInTab)
	{
		
		int id=GetTabIDFromPoint(point);
		
		if (IsInCloseIconArea(id, point) )
		{
			m_bMoveToCloseIcon=TRUE;
			
			UpdateTipText( GetXTip(), this, id);
		}
		else
		{
			m_bMoveToCloseIcon=FALSE;
			
			CString strTitle="";
			CChildFrame* tcf = GetChildFrame(id);
			if (tcf)
			{				
				
				GET_TAB_TITLE(tcf, strTitle);	
				
			}
			
			UpdateTipText( strTitle, this, id);
			
			
		}
		
		RECT rect;
		GetItemRect(id, &rect);
		
		rect.left = GetCloseIconLeft(rect);
		
		static BOOL bMoveToCloseIcon = FALSE;
		
		if (bMoveToCloseIcon != m_bMoveToCloseIcon)
		{
			bMoveToCloseIcon = m_bMoveToCloseIcon;
			RedrawWindow(&rect);
		}
		
		StartTrackMouseLeave();
	}
	
	CEnTabCtrl::OnMouseMove(nFlags, point);
}

void CAdvTabCtrl::UpdateTipText(LPCTSTR lpszText, CWnd* pWnd, UINT nIDTool)
{
	CString strOld;
	GetToolTips()->GetText( strOld, pWnd, nIDTool);
	
	if (strOld.Compare(lpszText) != 0)
	{
		GetToolTips()->UpdateTipText( lpszText, pWnd, nIDTool);
	}
	
	
}

void CAdvTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	try
	{
		
		SetFirstTimeFocus(point);
		
		if (g_bClickClose==2)
		{
			if (ClickCloseDown(point))
				goto _Exit;
		}
		if(!m_bDragging && (nFlags & MK_LBUTTON))
		{
			m_bDragging = TRUE;
			m_hOldCursor = GetCursor();
			m_nDragTabID = GetTabIDFromPoint(point);
			TabCtrlItem.mask = TCIF_PARAM|TCIF_IMAGE|TCIF_TEXT|TCIF_STATE;
			GetItem(m_nDragTabID, &TabCtrlItem);		
			if(GetCurSel() == m_nDragTabID)
				m_bSel = TRUE;
			else
				m_bSel = FALSE;
		}
		
	}
	catch (...) {ASSERT(false);}
	
_Exit:	
	CEnTabCtrl::OnLButtonDown(nFlags, point);
	
	
}

void CAdvTabCtrl::CloseViewByClickXIcon(const CPoint& point)
{  
	DWORD dwNow = GetTickCount();
	
	DWORD dwGap = dwNow - _dwLastXTime;
	
	if ( dwGap >400 || 0 == _dwLastXTime )
	{
		_dwLastXTime = dwNow;
		
		ClickedXIcon(point, FALSE);
	}
	
}

void CAdvTabCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	try{
		
		CloseViewByClickXIcon(point);
		
		if (g_bClickClose==2 && g_bCloseDown)
		{
			ClickCloseUp(point);
			goto _Exit;
		}
		if(m_bDragging)
		{
			int id = GetTabIDFromPoint(point);
			m_bDragging = FALSE;
			if(id>=0 && id!=m_nDragTabID)
			{
				if(id<m_nDragTabID)
					id--;
				//insert new tab
				TabCtrlItem.mask = TCIF_TEXT|TCIF_PARAM|TCIF_IMAGE|TCIF_STATE;
				InsertItem( id+1, &TabCtrlItem );
				//delete tab
				if(id+1<m_nDragTabID)
					m_nDragTabID ++;
				DeleteItem(m_nDragTabID);
				//select this tab
				if(m_nDragTabID<id+1)
					id--;
				if(m_bSel)
					SetCurSel(id+1);
				if (g_bShowIndex)
					pmf->UpdateTabTitleIndex( min(id+1, m_nDragTabID) );
				
				AfxGetMainWnd()->PostMessage(WM_UPDATE_TAB_TIP);
			}
		}
	}catch(...){}
	
_Exit:	
	CEnTabCtrl::OnLButtonUp(nFlags, point);
}

int CAdvTabCtrl::GetTabIDFromPoint(const CPoint &point)
{
	TCHITTESTINFO info;
	info.pt = point;
	info.flags = TCHT_NOWHERE;
	int id = HitTest(&info);
	return id;
}

void CAdvTabCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(pmf->m_nAllowDBC>0)
	{
		int nID=GetTabIDFromPoint(point);
		if (!IsInCloseIconArea(nID, point))
		{
			ProcessClick(nFlags, point, 0);
		}
		
	}
	
	CEnTabCtrl::OnLButtonDblClk(nFlags, point);
}

void CAdvTabCtrl::OnMButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(pmf->m_nAllowMC>0)
		ProcessClick(nFlags, point, 1);
	
	CEnTabCtrl::OnMButtonDown(nFlags, point);
}

void CAdvTabCtrl::ProcessClick(UINT nFlags, CPoint point, int nClickButton)
{
	try
	{
		TCHITTESTINFO hti;
		hti.pt = point;
		hti.flags = TCHT_ONITEM | TCHT_ONITEMICON | TCHT_ONITEMLABEL ;
		int nTabID = HitTest(&hti);
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		GetItem(nTabID, &TabCtrlItem);
		if(TabCtrlItem.lParam != NULL)
		{
			CChildFrame* tcf = ((CChildFrame*)TabCtrlItem.lParam);
			if (!tcf || !tcf->m_pView)
				return;
			if (tcf->m_bProtect && !tcf->m_pView->m_bIsActive)
				return;
			if((nClickButton==0 && pmf->m_nAllowDBC==1) || (nClickButton==1 && pmf->m_nAllowMC==1) || (nClickButton==2 && pmf->m_nAllowRC==1))
			{
				//2.2 313 post instead send
				//gf				tcf->PostMessage(WM_CLOSE);
				//				tcf->m_pView->m_bToClose = TRUE;
				// 				//tcf->MDIDestroy();
				
				CLOSE_PAGE_FORCE(tcf);
				
			}
			else if((nClickButton==0 && pmf->m_nAllowDBC==2) || (nClickButton==1 && pmf->m_nAllowMC==2) || (nClickButton==2 && pmf->m_nAllowRC==2))
			{
				//save as
				tcf->m_pView->PostMessage(WM_COMMAND, ID_FILE_SAVEAS, 0);
			}
			else if((nClickButton==0 && pmf->m_nAllowDBC==3) || (nClickButton==1 && pmf->m_nAllowMC==3) || (nClickButton==2 && pmf->m_nAllowRC==3))
			{
				//save html
				tcf->m_pView->PostMessage(WM_COMMAND, ID_FILE_SAVEHTML, 0);
			}
			else if((nClickButton==0 && pmf->m_nAllowDBC==4) || (nClickButton==1 && pmf->m_nAllowMC==4) || (nClickButton==2 && pmf->m_nAllowRC==4))
			{
				//save html
				tcf->m_pView->PostMessage(WM_COMMAND, ID_FILE_AUTOSAVE, 0);
			}
			else if((nClickButton==0 && pmf->m_nAllowDBC==5) || (nClickButton==1 && pmf->m_nAllowMC==5) || (nClickButton==2 && pmf->m_nAllowRC==5))
			{
				pmf->LockTab(tcf);
			}
			else if((nClickButton==0 && pmf->m_nAllowDBC==6) || (nClickButton==1 && pmf->m_nAllowMC==6) || (nClickButton==2 && pmf->m_nAllowRC==6))
			{
				tcf->m_pView->PowerRefresh();
			}
			else if((nClickButton==0 && pmf->m_nAllowDBC==7) || (nClickButton==1 && pmf->m_nAllowMC==7) || (nClickButton==2 && pmf->m_nAllowRC==7))
			{
				tcf->m_pView->m_bSortSave = TRUE;
				tcf->m_pView->PostMessage(WM_COMMAND, ID_FILE_AUTOSAVE, 0);
			}
		}
	}catch(...){}
}

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlEx operations

void CAdvTabCtrl::SetColours(COLORREF bSelColour, COLORREF bUnselColour)
{
	m_crSelColour = bSelColour;
	m_crUnselColour = bUnselColour;
	Invalidate();
}

void CAdvTabCtrl::SetBKColours(COLORREF bSelBKColour, COLORREF bUnselBKColour)
{
	m_crSelBKColour = bSelBKColour;
	m_crUnselBKColour = bUnselBKColour;
	Invalidate();
}

void CAdvTabCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	SetFirstTimeFocus(point);
	
	// TODO: Add your message handler code here and/or call default
	if (g_bClickClose==1)
		ClickCloseDown(point);
	
	CEnTabCtrl::OnRButtonDown(nFlags, point);
}

void CAdvTabCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	pmf->_bCustemRClickTab=FALSE;
	m_bDragging = FALSE;
	
	BOOL bNotPressedCtrl = ( (nFlags & MK_CONTROL)==0 ); //ctrl时弹菜单
	
	if (bNotPressedCtrl && ClickedXIcon(point, TRUE) )
	{
		
	}
	//gf if(pmf->m_nAllowRC>0 && (nFlags & MK_CONTROL)!=0 )
	else if(pmf->m_nAllowRC>0 && bNotPressedCtrl ) //ctrl时弹菜单,不进此if
	{
		pmf->_bCustemRClickTab=TRUE;
		ProcessClick(nFlags, point, 2);
	}
	else if (g_bClickClose==1 && g_bCloseDown)
	{
		if (ClickCloseUp(point))
			return;//not pop menu
	}
	
	CEnTabCtrl::OnRButtonUp(nFlags, point);
}

BOOL CAdvTabCtrl::ClickCloseDown(CPoint point)
{
	g_nCloseTabID = GetTabIDFromPoint(point);
	TabCtrlItem.mask = TCIF_PARAM|TCIF_IMAGE|TCIF_TEXT|TCIF_STATE;
	GetItem(g_nCloseTabID, &TabCtrlItem);		
	//
	CChildFrame* tcf = ((CChildFrame*)TabCtrlItem.lParam);
	if (!tcf)
		return FALSE;
	CMyIE9View *pView = tcf->m_pView;
	if ( !(tcf->m_bProtect && !pView->m_bIsActive) )//not close not active protect tab
	{
		RECT rect;
		GetItemRect(g_nCloseTabID,&rect);
		if ( PT_IN_ICON(point, rect) )
		{
			g_bCloseDown = 1;
			g_bShowCloseIcon = 1;
			pView->m_nOlderIcon = pView->m_nOldIcon;
			pmf->SetTabIcon(TI_CLOSE,g_nCloseTabID, pView);
			if (TAB_NEED_CAPTURE)
				SetCapture();
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CAdvTabCtrl::ClickCloseUp(CPoint point)
{
	if (g_bCloseDown)
	{
		g_bCloseDown = 0;
		if (TAB_NEED_CAPTURE)
			ReleaseCapture();
		if (g_bShowCloseIcon)		
		{
			g_bShowCloseIcon = 0;
			GetItem(g_nCloseTabID, &TabCtrlItem);
			CChildFrame* tcf = ((CChildFrame*)TabCtrlItem.lParam);
			if (!tcf)	return FALSE ;
			//			tcf->PostMessage(WM_CLOSE);
			//			tcf->m_pView->m_bToClose = TRUE;
			CLOSE_PAGE_FORCE(tcf); //gf
			
			pmf->SetTabIcon(tcf->m_pView->m_nOlderIcon,g_nCloseTabID,tcf->m_pView);
			return TRUE;
		}
	}
	return FALSE;
}

void CAdvTabCtrl::SetFirstTimeFocus(CPoint point)
{
	
	//gfx
	if(!CGf_AppStatic::_instance._bShowXIconInTab)
	{
		return;
	}
	
	int nTabID = GetTabIDFromPoint(point);
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	GetItem(nTabID, &TabCtrlItem);
	if(TabCtrlItem.lParam != NULL)
	{
		CChildFrame* tcf = ((CChildFrame*)TabCtrlItem.lParam);
		
		if (tcf != CGf_AppStatic::_instance._curChild )
		{
			m_bFirstTimeFocus = TRUE;
		}
		else
		{
			m_bFirstTimeFocus = FALSE;
		}
		
	}
}

BOOL CAdvTabCtrl::ClickedXIcon(const CPoint &point, BOOL bByRightClick)
{
	//gfx
	if(CGf_AppStatic::_instance._bShowXIconInTab)
	{
		int nTabID = GetTabIDFromPoint(point);
		
		CChildFrame* tcf = GetChildFrame(nTabID);
		if (!tcf)
		{
			return FALSE;
		}
		
		BOOL bCloseByCloseIcon = FALSE;
		if (IsInCloseIconArea(nTabID, point))
		{
			bCloseByCloseIcon=TRUE;
		}
		
		if (m_bFirstTimeFocus)
		{
			bCloseByCloseIcon=FALSE;
		}
		
		if (bCloseByCloseIcon)
		{
			if (bByRightClick)
			{
				pmf->_bCustemRClickTab=TRUE;
				
				g_bRTab = 0; //close current right
				pmf->OnFileCloseAllRight();
				
			}
			else //by left click
			{
				m_bMoveToCloseIcon=FALSE;
				
				CLOSE_PAGE_FORCE(tcf);
				//					tcf->m_pView->m_bToClose=TRUE;
				//  					tcf->PostMessage(WM_CLOSE);					
				
			}
			
			return TRUE;
			
		}
		
		
		
		
	}
	
	return FALSE;
}

CChildFrame* CAdvTabCtrl::GetChildFrame(int nTabID)
{
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	GetItem(nTabID, &TabCtrlItem);
	if(TabCtrlItem.lParam != NULL)
	{
		CChildFrame* tcf = ((CChildFrame*)TabCtrlItem.lParam);
		if (!tcf || !tcf->m_pView)
		{
			return NULL;
		}
		else
		{
			return tcf;
		}
		
		
	}
	
	return NULL;
}

CString CAdvTabCtrl::GetXTip()
{
	return CGfAppString::GetString(IDS_X_TIP_CLOSE_ALL_RIGHT);
	
}

void CAdvTabCtrl::StartTrackMouseLeave()
{
	
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags=TME_LEAVE;
		tme.hwndTrack=m_hWnd;
		
		m_bTracking = _TrackMouseEvent(&tme);
	}
}
