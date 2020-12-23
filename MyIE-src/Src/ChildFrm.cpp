// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "MyIE9.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include <..\src\afximpl.h>
#include "InputPassword.h"
#include "mshtml.h"
#include "Gf_AppStatic.h"
#include "Consts_gf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_VIEW_SET_FOCUS	(WM_USER + 0x1000)

CChildFrame * g_pPreWin = NULL;
/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
//{{AFX_MSG_MAP(CChildFrame)
ON_WM_CLOSE()
ON_WM_MDIACTIVATE()
ON_WM_SYSCOMMAND()
ON_WM_SIZE()
ON_WM_NCACTIVATE()
ON_WM_NCPAINT()
//}}AFX_MSG_MAP

ON_MESSAGE(WM_VIEW_SET_FOCUS, OnViewSetFocus) 
ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString) //gf

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	m_bLock = FALSE;
	m_bProtect = FALSE;
	m_pView = NULL;
	
	m_pBlankChild = NULL;
	m_bSkipSel = FALSE;
	m_bForceActive = FALSE;
	m_nActive = FALSE;
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	try
	{
		
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs
		
		cs.lpszClass = AfxRegisterWndClass(
			CS_DBLCLKS,                       // if you need double-clicks
			NULL,                             // no cursor (use default)
			(HBRUSH)::GetStockObject(SYSTEM_FONT),//gf NULL,                             // no background brush
			LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PAGE))); // app icon
		ASSERT(cs.lpszClass);
		
		cs.style = WS_CHILD  | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
			| FWS_ADDTOTITLE | WS_THICKFRAME  | WS_MAXIMIZEBOX| WS_MINIMIZEBOX ;
		cs.cx = 520;
		cs.cy = 320; 
		
		if( !CMDIChildWnd::PreCreateWindow(cs) )
			return FALSE;
		
		BOOL bMax;
		if( pmf != NULL)
		{
			if(pmf->MDIGetActive(&bMax)==NULL)
			{
				if (g_bMax==1)
					bMax = TRUE;
				cs.style |= WS_VISIBLE;
			}
		}
		else
			bMax = TRUE;
		if(bMax)
			cs.style |= WS_MAXIMIZE;
		
		if(bMax)
			g_bMax = 1;
		else
			g_bMax = 0;
		
		return TRUE;
	}
	catch (...)
	{
		ASSERT(false);
		return FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	try
	{
		if(pmf!=NULL)
		{
			if (m_bLock && g_bConfirmCloseLock)
			{
				if(MSGBOX(IDS_CLOSE_LOCK_CONF, MB_YESNO|MB_ICONQUESTION)==IDNO)
					return;
			}
			int cn = pmf->m_wndTab.GetItemCount();
			//
			if (pmf->m_bKeepOnePage && cn==1)
			{
				if (this->m_pView->m_lpszUrl == "about:blank")
					return;
			}
			int nTabID;
			if(!IsWindowVisible() && pmf->IsWindowVisible())
				nTabID = -1;
			else
			{
				nTabID = pmf->FindTab(this, TRUE);
				if (cn>0)
					pmf->m_nWin= cn-1;
			}
			if(nTabID>=0 && nTabID<cn)
			{
				pmf->m_wndTab.DeleteItem(nTabID);
				if (g_bShowIndex)
					pmf->UpdateTabTitleIndex(nTabID);
				
				if(m_pView!=NULL)
				{
					if(m_pView->m_nDupID>nTabID)
						m_pView->m_nDupID--;
					
					if(m_pView->m_lpszUrl!="about:blank")
					{
						pmf->AddToUndoList(m_strTitle, m_pView->m_lpszUrl); //gf
//						pmf->m_strUndoURL[pmf->m_nUndoEnd] = m_pView->m_lpszUrl;
//						pmf->m_strUndoName[pmf->m_nUndoEnd] = m_strTitle;
//						
//						pmf->m_nUndoEnd = (pmf->m_nUndoEnd+1)%MAX_UNDO;
//						if(pmf->m_nUndoStart == pmf->m_nUndoEnd)
//							pmf->m_nUndoStart = (pmf->m_nUndoStart + 1)%MAX_UNDO;
						
						pmf->UpdateUndoMenu();
					}
				}
				if(pmf->m_bAutoTab)
					pmf->PostMessage(WM_UPDATE_TAB, TRUE);
				else
					pmf->PostMessage(WM_UPDATE_TAB, FALSE);
				
				pmf->PostMessage(WM_UPDATE_TAB_TIP);
				
				g_bVerbose = TRUE;
			}
			
			if(m_bProtect)
				pmf->m_nProtectNum--;
			
			//update address bar
			if(cn==1 && nTabID>=0)// the window may not exist at tab, exam: filter window
			{
				pmf->SetAddressText("");
				pmf->m_SysMenuBar.PostMessage(WM_IDLEUPDATECMDUI, TRUE, 0);
				g_nPercent = -1;
			}
			
			g_nNextTab = -2;
			//5.3.7, when pop window be killed, it not visible, then nTabID<0
			//then no use to change current tab
			if(m_pView!=NULL && (m_pView->m_bIsActive || m_pView->m_nDupID>=0) && nTabID>=0)
			{
				if(m_pView->m_nDupID>=0 && pmf->m_bActiveNewWindow)
					g_nNextTab = m_pView->m_nDupID;
				else if(pmf->m_nCloseSequence==0)
				{
					cn = pmf->m_wndTab.GetItemCount();
					g_nNextTab = (cn>0 ? cn-1 : 0);
				}
				else if(pmf->m_nCloseSequence==1)
				{
					g_nNextTab = nTabID;

					//gf
					int nMaxIndexNow = cn-2;
					if (g_nNextTab> nMaxIndexNow)
					{
						g_nNextTab=nMaxIndexNow;
					}
				}
				else if(pmf->m_nCloseSequence==2)
				{
					if (nTabID > 0)
						g_nNextTab = nTabID-1;
					else
						g_nNextTab = 0;
				}
			}
			if(g_nNextTab>=0)
			{
				TCITEM TabCtrlItem;
				TabCtrlItem.mask = TCIF_PARAM;
				
				pmf->m_wndTab.GetItem(g_nNextTab, &TabCtrlItem);
				CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
				nTabID = g_nNextTab;
				g_nNextTab = -2;
				if(tcf!=NULL)
				{
					pmf->MDIActivate(tcf);
					pmf->m_wndTab.SetCurSel(nTabID);
				}
			}
		}
	}catch(...){}
	
	try
	{
		
		CMDIChildWnd::OnClose();
		//
		if (pmf->m_nWin==0)
		{
			pmf->SetMessageText("");
			if (pmf->m_bKeepOnePage)
				pmf->SendMessage(WM_COMMAND, ID_FILE_NEW_BLANK, 0);
		}
		
	}catch(...){ASSERT(false);}
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	CMyIE9View* pvw = NULL;
	
	try{
		pvw = new CMyIE9View;
		if(pvw==NULL)
			return FALSE;
		if(pmf && pmf->m_nWin >0)
		{
			if(pmf->m_bActiveNewWindow || m_bForceActive)
				pvw->m_bIsActive=TRUE;
			else
				pvw->m_bIsActive=FALSE;
		}
		else
			pvw->m_bIsActive = TRUE;
		
		CRect rect;
		try{
			GetClientRect(&rect);
			pvw->Create(NULL, "", WS_CHILD|WS_VISIBLE, rect, this, AFX_IDW_PANE_FIRST,NULL);
			m_pView = pvw;
	}catch(...){}
	
	}catch(...){}
	
	return TRUE; //CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

BOOL CChildFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	try{
		// pump through current view FIRST
		CWnd* pw = GetWindow(GW_CHILD);
		if (pw != NULL && pw->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}catch(...){}
	
	return FALSE;
}

void CChildFrame::OnViewSetFocus(WPARAM wParam, LPARAM lParam)
{
	try{
		m_pView->m_wndBrowser.SetFocus();
	}catch(...){}
}

void CChildFrame::ViewSetFocus()
{
	try{
		if (m_pView->m_lpszUrl != "about:blank")
			PostMessage(WM_VIEW_SET_FOCUS);
	}catch(...){}
}
void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CGf_AppStatic::_instance._curChild = pActivateWnd; //gfx ±êÇ©¼Óx

	if(m_pView == NULL)
		return;
	
	try{
		if(m_pView->m_nProgress==-10)
			m_pView->m_nProgress=-1;
		if(m_bProtect && bActivate && !m_pView->m_bIsActive && g_pPreWin!=this)
		{
			m_pView->ShowWindow(SW_HIDE);
			CInputPassword dlg;
			BOOL validate = FALSE;
			if(dlg.DoModal()==IDOK)
			{
				if(dlg.m_strPassword == m_strPsw)
					validate = TRUE;
				else
					MSGBOX(IDS_PASSWORD_ERROR);
			}
			if(!validate)
			{
				g_pPreWin = this;
				//go back 
				if(pmf->m_wndTab.GetItemCount()==pmf->m_nProtectNum)
				{
					BOOL olda = pmf->m_bActiveNewWindow;
					pmf->m_bActiveNewWindow = TRUE;
					CChildFrame* ncf = pmf->NewChildWindow(2,0);
					pmf->m_bActiveNewWindow = olda;
					pmf->m_wndAddress->SetFocus();
				}
				else
					pmf->MDINext();
				m_pView->ShowWindow(SW_SHOW);
				return;
			}
			else
				m_pView->ShowWindow(SW_SHOW);
		}
		
		CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
		
		if(bActivate)//only setfocus after load after, so must focus at here
			ViewSetFocus();
		
		if(!m_bProtect && g_pPreWin == this && bActivate)
		{
			m_pView->m_bIsActive = bActivate;
			return;
		}
		
		if(bActivate)
			g_pPreWin = this;
		
		m_pView->m_bIsActive = bActivate;
		
		if (!pmf->m_bForbidMainTitle)
			SetWindowText(m_strTitle);//!!must be here
		
		//set address bar
		if(m_pView->m_bIsActive)
		{
			if (m_pView->m_lpszUrl && !m_pView->m_lpszUrl.IsEmpty())
				pmf->SetAddress(m_pView->m_lpszUrl);
			pmf->m_bIsSecure = m_pView->m_bIsSecure;
			
			//get the current tab
			int nTabID=0;
			if(m_bSkipSel)
				nTabID = m_nCurTabID;
			else
				nTabID = pmf->FindTab(this);
			if(g_nNextTab>=0 && g_nNextTab!=nTabID)
			{
				m_bSkipSel = FALSE;
				return;
			}
			//select current tab
			if(!m_bSkipSel)
				pmf->m_wndTab.SetCurSel( nTabID );
			int oi = m_pView->m_nOldIcon;
			if(oi==TI_100 || oi==TI_SAVE || m_pView->m_nProgress<0)
			{
				if(m_bLock)
					pmf->SetTabIcon( TI_LOCK, nTabID, m_pView);
				else if(m_bProtect)
					pmf->SetTabIcon( TI_PROTECT, nTabID, m_pView);
				else
				{	if (m_pView->m_nOldIcon==TI_100 ||
				(!g_bShowFavIcon && IS_FAVICON(m_pView->m_nOldIcon)) ||
				(!g_bShowRead && m_pView->m_nOldIcon==TI_READ) 
				)
				pmf->SetTabIcon( TI_NO, nTabID, m_pView);
				}
				//update tab bar
				if(oi*m_pView->m_nOldIcon <=0 && pmf->m_nTabStyle==2)
					pmf->PostMessage(WM_UPDATE_TAB, FALSE);
			}
			//set status bar
			m_pView->OnStatusTextChange("");
			g_nPercent = m_pView->m_nProgress;
		}
		
		m_bSkipSel = FALSE;
		
	}catch(...){}
}

void CChildFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	try{
		
		// TODO: Add your message handler code here and/or call default
		if(nID ==SC_MAXIMIZE)
		{
			g_bMax = 1;
		}
		else if(nID ==SC_RESTORE || nID == SC_MINIMIZE)
		{
			g_bMax = 3;
		}
		
		CMDIChildWnd::OnSysCommand(nID, lParam);
		
	}catch(...){ASSERT(false);}
}

void CChildFrame::OnSize(UINT nType, int cx, int cy) 
{
	try{
		
		CMDIChildWnd::OnSize(nType, cx, cy);
		// TODO: Add your message handler code here
		if(m_pView!=NULL && g_bMax!=1)
		{
			CRect rect;
			GetClientRect(&rect);
			if(m_pView->GetWidth()!= rect.Width() || m_pView->GetHeight()!=rect.Height())
			{
				m_pView->PostMessage(WM_SIZE, nType, (rect.Height()<<16)+rect.Width());
			}
		}
		
	}catch(...){ASSERT(false);}
}

BOOL CChildFrame::OnNcActivate(BOOL bActive) 
{
	try{
		
		// TODO: Add your message handler code here and/or call default
		if(!bActive)
		{
			//stop the timer;
			if(m_pView->m_nScrollTimer)
				m_pView->KillTimer(m_pView->m_nScrollTimer);
			m_pView->m_nScrollTimer = 0;
		}
		else
		{
			//start to scroll
			if(!m_pView->m_nScrollTimer && (m_pView->m_bUseAutoScroll || pmf->m_bAllScroll) && m_pView->m_nSrSpeed!=0)
			{
				if(m_pView->m_nProgress==100 || m_pView->m_nProgress<0 || (!m_pView->m_bScrollAfterDL && m_pView->m_nProgress>=0))
				{
					if(!pmf->m_bMouseCtrlScrSpeed)
						m_pView->m_nLastScrSpd = (UINT)((g_fSeed*100)/(m_pView->m_nSrSpeed*m_pView->m_nSrSpeed));
					else
						m_pView->m_nLastScrSpd = STOP_TIMER;
					m_pView->m_nScrollTimer = m_pView->SetTimer(2132, m_pView->m_nLastScrSpd, NULL);
				}
			}
		}
		
		return CMDIChildWnd::OnNcActivate(bActive);
		
	}catch(...){ASSERT(false); return FALSE;}
}

void CChildFrame::OnNcPaint()
{
	try{
		
		if (g_bMax!=1)
			CMDIChildWnd::OnNcPaint();
		
	}catch(...){ASSERT(false);}
}


LRESULT CChildFrame::OnSetMessageString(WPARAM wParam,LPARAM lParam)
{
	try
	{
		return CFrameWnd::OnSetMessageString(wParam, lParam);
	}
	catch (...) {
		ASSERT(false);
		return 0;
	}
}
