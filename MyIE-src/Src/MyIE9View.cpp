// MyIE9View.cpp : implementation of the CMyIE9View class
//
#include "stdafx.h"
#include "MyIE9.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include <wininet.h>

#include <Mshtmdid.h>
#include "MyIE9View.h"
#include "mshtml.h"
#include "SetRefIntervaldlg.h"

#include <mmsystem.h> //for sndplaysound
#include "SortSaveDlg.h"

#include "GFBusiness.h" //gfB
#include "HelperStr.h"
#include "FilterSites.h"

#include "Gf_Config.h"
#include "Gf_TranslateUrl.h"
#include "Gf_AppStatic.h"
#include "Gf_html.h"
#include "Consts_gf.h"
#include "RegHelper.h"
#include "Gf_Win.h"

//####################################################
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//####################################################
#define FN_LEN		220 //file name length
#define POP_DELAY	pmf->m_nPopDelay
#define MG_LEN		9 //mouse gesture len

//####################################################
#define CAN_ACTIVE(tcf)		(!(tcf->m_nActive&INW_INACTIVE) && (pmf->m_bActiveNewWindow || tcf->m_bForceActive))

//####################################################
char g_cGestureStack[MG_LEN], g_cGestureString[MG_LEN];
int g_nGestureHead, g_nGestureTail;

//####################################################
/////////////////////////////////////////////////////////////////////////////
// CMyIE9View

IMPLEMENT_DYNCREATE(CMyIE9View, CFixedHtmlView)

BEGIN_MESSAGE_MAP(CMyIE9View, CFixedHtmlView)
ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
ON_COMMAND(ID_EDIT_CUT, OnEditCut)
ON_COMMAND(ID_EDIT_FIND, OnEditFind)
ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
ON_COMMAND(ID_OPTIONS_LOADIMAGE, OnOptionsLoadimage)
ON_UPDATE_COMMAND_UI(ID_OPTIONS_LOADIMAGE, OnUpdateOptions)
ON_COMMAND(ID_OPTIONS_ALLOWSCRIPTS, OnOptionsAllowscripts)
ON_COMMAND(ID_OPTIONS_LOADSOUNDS, OnOptionsLoadsounds)
ON_COMMAND(ID_OPTIONS_LOADVIDEOS, OnOptionsLoadvideos)
ON_COMMAND(ID_OPTIONS_USE_WEBP, OnOptionsUseWebp)
ON_UPDATE_COMMAND_UI(ID_OPTIONS_USE_WEBP, OnUpdateOptionsUseWebp)
ON_COMMAND(ID_OPTIONS_ALLOWACTIVEX, OnOptionsAllowactivex)
ON_COMMAND(ID_OPTIONS_ALLOWJAVAAPPLET, OnOptionsAllowjavaapplet)
ON_UPDATE_COMMAND_UI(ID_GO_BACK, OnUpdateGoBack)
ON_UPDATE_COMMAND_UI(ID_GO_FORWARD, OnUpdateGoForward)
ON_UPDATE_COMMAND_UI(ID_OPTIONS_ALLOWSCRIPTS, OnUpdateOptions)
ON_UPDATE_COMMAND_UI(ID_OPTIONS_LOADSOUNDS, OnUpdateOptions)
ON_UPDATE_COMMAND_UI(ID_OPTIONS_LOADVIDEOS, OnUpdateOptions)
ON_UPDATE_COMMAND_UI(ID_OPTIONS_ALLOWACTIVEX, OnUpdateOptions)
ON_UPDATE_COMMAND_UI(ID_OPTIONS_ALLOWJAVAAPPLET, OnUpdateOptions)
ON_WM_SIZE()
ON_UPDATE_COMMAND_UI(ID_VIEW_FONTS_LARGEST, OnUpdateViewFonts)
ON_UPDATE_COMMAND_UI(ID_VIEW_FONTS_LARGE, OnUpdateViewFonts)
ON_UPDATE_COMMAND_UI(ID_VIEW_FONTS_MEDIUM, OnUpdateViewFonts)
ON_UPDATE_COMMAND_UI(ID_VIEW_FONTS_SMALL, OnUpdateViewFonts)
ON_UPDATE_COMMAND_UI(ID_VIEW_FONTS_SMALLEST, OnUpdateViewFonts)
//{{AFX_MSG_MAP(CMyIE9View)
ON_WM_PARENTNOTIFY()
ON_WM_TIMER()
ON_WM_CLOSE()
ON_COMMAND(ID_TOOLS_AUTOREF, OnToolsAutoref)
ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOREF, OnUpdateToolsAutoref)
ON_COMMAND(ID_TOOLS_REFFRE, OnToolsReffre)
ON_COMMAND(ID_FILE_SAVEHTML, OnFileSavehtml)
ON_WM_RBUTTONUP()
ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
ON_COMMAND(ID_FILE_PROPERTY, OnFileProperty)
ON_WM_MOUSEMOVE()
ON_COMMAND(ID_FILE_AUTOSAVE, OnFileAutoSave)
ON_WM_LBUTTONUP()
ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOSCROLL_PAGE0, OnUpdateToolsAutoscrollPage)
ON_COMMAND(ID_VIEW_PAGESIZE_ADD10, OnViewPagesizeAdd10)
ON_COMMAND(ID_VIEW_PAGESIZE_100, OnViewPagesize100)
ON_COMMAND(ID_VIEW_PAGESIZE_200, OnViewPagesize200)
ON_COMMAND(ID_VIEW_PAGESIZE_50, OnViewPagesize50)
ON_COMMAND(ID_VIEW_PAGESIZE_DEC10, OnViewPagesizeDec10)
ON_COMMAND(ID_FILE_IMPORT_EXPORT, OnFileImportExport)
ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveAs)
ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOSCROLL_PAGE1, OnUpdateToolsAutoscrollPage)
ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOSCROLL_PAGE2, OnUpdateToolsAutoscrollPage)
ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOSCROLL_PAGE4, OnUpdateToolsAutoscrollPage)
ON_COMMAND(ID_FILE_AUTOSAVE_SORT, OnFileAutoSaveSort)
//}}AFX_MSG_MAP
ON_COMMAND(ID_GO_BACK, OnGoBack)
ON_COMMAND(ID_GO_FORWARD, OnGoForward)
ON_COMMAND(ID_TOOLS_AUTOSCROLL_AUTOSCROLLINGAFTERDOWNLOADING, OnToolsAutoscrollAutoscrollingafterdownloading)
ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOSCROLL_AUTOSCROLLINGAFTERDOWNLOADING, OnUpdateToolsAutoscrollAutoscrollingafterdownloading)
ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOSCROLL_FAST, OnUpdateToolsAutoscrollSpeed)
ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOSCROLL_MEDIUM, OnUpdateToolsAutoscrollSpeed)
ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOSCROLL_SLOW, OnUpdateToolsAutoscrollSpeed)
ON_COMMAND(ID_TOOLS_AUTOSCROLL_USEAUTOSCROLLING, OnToolsAutoscrollUseautoscrolling)
ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOSCROLL_USEAUTOSCROLLING, OnUpdateToolsAutoscrollUseautoscrolling)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyIE9View construction/destruction
void CMyIE9View::SetSpecialProperty(DWORD dwNewProperty)
{
	try{
		
		m_dwProperty = dwNewProperty;
		ChangeProperty();
		m_bSpecialProperty = true;
	}catch(...){ASSERT(false);}
}

void CMyIE9View::SetDefaultProperty()
{
	m_dwProperty = m_dwPropertyDefault;
}

void CMyIE9View::SetFullProperty()
{
	m_dwProperty = 0;
	m_dwProperty |= DLCTL_DLIMAGES|DLCTL_VIDEOS|DLCTL_BGSOUNDS;
	m_dwProperty &= ~DLCTL_NO_SCRIPTS|~DLCTL_NO_JAVA|~DLCTL_NO_RUNACTIVEXCTLS;
}

CMyIE9View::CMyIE9View()
:m_lpszUrl("") //gf
,_strUrlBefClick("")
{
//	_strOldKilledFlyUrl = "";
//	_bClickedLinkForFly = FALSE;

	_bRefreshing = FALSE;

	_bRefeshedForFilter = FALSE;

	_bScriptForFilter = FALSE;

	_bMaybeOpenNewWinByUser = FALSE;
	_iPageSize = 100;

	// TODO: add construction code here
	m_iHighLightedCount = 0;

//gfpop del	m_bAutoPopWin = FALSE;
	//
	m_bIsPopUp = FALSE;
	m_nFilterDup = 0;
	m_bEnWebProxy = FALSE;
	m_bIsCapture = FALSE;
	m_bIsActive = FALSE;
	m_bReUsed = FALSE;
	m_bBack= FALSE;
	m_bForward = FALSE;
	m_nProgress = -1;
	m_bIsNewPopup = FALSE;
	m_bDelayAdd = TRUE;
	m_nDupID = -1;
	m_bToClose = FALSE;
	m_bIsSecure = FALSE;
	m_nFontSize = g_nDefFontSize; //4, smallest, 3, small, 2, medium, 1, large, 0, largest
	
	m_dwPropertyDefault = m_dwProperty = pmf->m_dwDefaultProperty;
	
	m_bForceRefresh = FALSE;
	m_bFirst = TRUE;
	m_nSrSpeed = 1; //1-slow, 2-medium, 3-fast
	m_bScrollAfterDL = TRUE;
	m_bUseAutoScroll = FALSE;
	m_nScrollTimer = 0;
	m_nOldIcon = -1;
	m_nCurHistory = -1;
	m_nHistoryLength = 0;
	m_ptLClick.x=0;
	m_ptLClick.y = 0;
	m_ptRClick.x=0;
	m_ptRClick.y=0;
	
	g_nGestureHead = 0;
	g_nGestureTail = 0;
	
	m_nAutoRefreshTimer = 0;
	m_bUseAutoRef = FALSE;
	m_nRefreshInterval = pmf->m_nDefRefreshInterval;
	//
	m_nScrollSize = 0;
	m_bSpecialProperty = false;
	m_strFindKey = "";
	m_bFindPrev = 0;
	m_bHighLighted = 0;
	m_strHighLightKey = "";
	m_bSortSave = false;
	m_bForbidAutoNavigate = false;
	m_nInNewWindow = 0;
	//
	m_strOpenNewUrl.Empty();
	m_nAutoRunCmd = 0;
	
	m_bStillOpenBadSite = FALSE;
	
	m_bPageOpenedInThisWindow = FALSE;
}

CMyIE9View::~CMyIE9View()
{
	try{
		if(m_nScrollTimer)
			KillTimer(m_nScrollTimer);
		if(m_nAutoRefreshTimer)
			KillTimer(m_nAutoRefreshTimer);
	}catch(...){}
}

BOOL CMyIE9View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	try{
		cs.lpszClass = AfxRegisterWndClass(
			CS_DBLCLKS,                       // if you need double-clicks
			NULL,                             // no cursor (use default)
			NULL,                             // no background brush
			NULL); // app icon
		ASSERT(cs.lpszClass);
	}catch(...){ASSERT(false);}
	
	BOOL bRet = FALSE;
	try{
		
		bRet = CFixedHtmlView::PreCreateWindow(cs);
	}catch(...){ASSERT(false);}
	
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// CMyIE9View drawing

void CMyIE9View::OnInitialUpdate(int nOpenType, LPCSTR strUrl)
{
	try
	{
		m_bFirst = TRUE;
		CFixedHtmlView::OnInitialUpdate();

//		m_pBrowserApp->put_RegisterAsBrowser(TRUE); //gfdb

		
		if (nOpenType == 1)// && pmf->m_strHomePage.GetLength())
			GoHome();
		else if (nOpenType == 2)
		{
			if(strUrl!=NULL && strUrl!="")
			{
				Navigate2(strUrl, 0, NULL);
				m_lpszUrl = strUrl;
			}
			else
			{
				m_lpszUrl = "about:blank";
				Navigate2(m_lpszUrl, 0, NULL);
			}
		}
		else
		{
			m_lpszUrl = "about:blank";
			Navigate2(m_lpszUrl, 0, NULL);
		}
		m_strWebProxy = pmf->m_strDefaultWebProxy;
	}catch(...){}
}

/////////////////////////////////////////////////////////////////////////////
// CMyIE9View diagnostics

#ifdef _DEBUG
void CMyIE9View::AssertValid() const
{
	CFixedHtmlView::AssertValid();
}

void CMyIE9View::Dump(CDumpContext& dc) const
{
	CFixedHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyIE9View message handlers
void CMyIE9View::OnTitleChange(LPCTSTR lpszText)
{
	try{
		CChildFrame* tcf = ((CChildFrame*)GetParentFrame());
		if(tcf!=NULL)
		{
			if (!pmf->m_bForbidMainTitle)
			{
				if(tcf->m_strStickName != "")
				{
					tcf->SetWindowText(tcf->m_strStickName);
				}
				else
				{
					tcf->SetWindowText(lpszText);
				}
			}
			tcf->m_strTitle = lpszText;
		}
		
		CString url("");
		//need pay attention!
		try{
			GetLocationURL(url);
		}catch(...){}
		
		
		BOOL bSetAddress = TRUE;
		if(!url.IsEmpty())
		{
			if(m_lpszUrl == url)
				bSetAddress = FALSE;
			else
				m_lpszUrl = url;
			//for history
			if(m_pITravelLog == NULL)
				SetHistory(url);
		}
		else
			bSetAddress = FALSE;
		
		if( strlen(lpszText)>0)//gfl if(m_pITravelLog == NULL && strlen(lpszText)>0)
			m_strLastTitle = lpszText;
		
		if(pmf!=NULL)
		{
			//set the new title of tab
			if(tcf->m_strStickName=="")
			{
				int nTabID = pmf->FindTab(tcf);
				if (g_bShowIndex)
				{
					CString strTitle;
					strTitle.Format("%d-%s",nTabID+1,lpszText);
					pmf->SetTabTitle( strTitle, nTabID );
				}
				else
					pmf->SetTabTitle( lpszText, nTabID );
			}
			if(bSetAddress && m_bIsActive)
			{
				pmf->SetAddress(url);
			}
			//update tab bar
			if(!pmf->m_bAutoTab)
				pmf->PostMessage(WM_UPDATE_TAB);
		}
	}catch(...){}
}

void WBShowScrollBar(CMyIE9View* pvw)
{
	try
	{
		LPDISPATCH pDisp = NULL;
		IHTMLDocument2 *pHTMLDoc = NULL;
		IHTMLElement * pHTMLElem = NULL;
		IHTMLBodyElement *pHTMLBody = NULL;
		try
		{
			pDisp = pvw->GetHtmlDocument();
			if( pDisp )
			{
				if (SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
				{
					if(SUCCEEDED(pHTMLDoc->get_body( &pHTMLElem )))
					{
						if (SUCCEEDED(pHTMLElem->QueryInterface(IID_IHTMLBodyElement, (void**)&pHTMLBody)))
						{
							pHTMLBody->put_scroll(g_bstrYes);
						}
					}
				}
			}
		}catch(...){}
		
		try
		{
			if(pHTMLBody!=NULL)
				pHTMLBody->Release();
			if(pHTMLElem!=NULL)
				pHTMLElem->Release();
			if(pHTMLDoc!=NULL)
				pHTMLDoc->Release();
			if(pDisp!=NULL)
				pDisp->Release();
		}catch(...){}
	}catch(...){}
}

void CMyIE9View::OnDocumentCompleteAll(LPCTSTR lpszUrl)
{
	SysDocumentCompleteAll(lpszUrl);
	DocShowedOK();
}

void CMyIE9View::SysDocumentCompleteAll(LPCTSTR lpszUrl)
{
	// make sure the main frame has the new URL.  This call also stops the animation
	try
	{

		CString strUrl;
		GetLocationURL(strUrl);
		if(strUrl.IsEmpty() && lpszUrl!=NULL)
			strUrl = lpszUrl;
		
		//the strUrl maybe empty
		if (strUrl.GetLength())
			m_lpszUrl = strUrl;
		
		pmf->AddToAddressList(strUrl, m_strLastTitle); //gfl
		
		CString strUrlNew = pmf->GetStandUrl(strUrl);

		//filter popup, //strUrlNew==m_lpszUrl => the url maybe the frame in the page
		if(g_bKillPop && !m_bToClose && !m_bIsPopUp && pmf!=NULL && g_bKillListWin && !strUrl.IsEmpty() && strUrlNew==m_lpszUrl) 
		{
			if (pmf->NeedPopFilter(strUrlNew))
			{
				if (pmf->ConfirmPopupFilter(strUrlNew, FALSE))
				{
					m_bIsPopUp = TRUE;
					m_strPossibleURL = strUrlNew;
					m_nProgress = -10;
					CChildFrame* tcf = ((CChildFrame*)GetParentFrame());
					if(tcf!=NULL)
					{
						m_bToClose = TRUE;
						tcf->PostMessage(WM_CLOSE);
						//add counter
						pmf->AddToPopList(strUrlNew, 2);
						return;
					}
				}
			}
		}
		
		
		if(m_pDisp == NULL) //means document complete
		{
			m_nProgress = -10;			
			if(m_bIsActive && pmf!=NULL && !strUrl.IsEmpty())
			{
				//((CMainFrame*)((CChildFrame*)GetParentFrame()->GetParentFrame()))->SetAddress(strUrl);
				BOOL bModified=FALSE;
				if (pmf->m_bBlankMonitorClip)
				{
					if (strUrl == "about:blank")
					{
						CString str = _GetClipString();
						if (str.GetLength())
						{
							//1:only url 2:url and text
							if (pmf->m_bBlankMonitorClip==2 || IS_URL(str))
							{
								pmf->SetAddressText(str);
								bModified = TRUE;
							}
						}
					}
				}
				if (!bModified)
					pmf->SetAddress(strUrl);
				g_nPercent= -1;	
			}
			//set icon
			if((m_nProgress>95 || m_nProgress<0))//3.2 && !strUrl.IsEmpty()
			{
				CChildFrame* tcf = (CChildFrame*)GetParentFrame();
				int nTabID = pmf->FindTab(tcf);
				int oi = m_nOldIcon;
				if(m_bIsActive)
				{
					if(tcf->m_bLock)
						pmf->SetTabIcon( TI_LOCK, nTabID, this);
					else if(tcf->m_bProtect)
						pmf->SetTabIcon( TI_PROTECT, nTabID, this);
					else if (m_nOldIcon!=TI_0)
						pmf->SetTabIcon( TI_NO, nTabID, this);
					//start to scroll
					if(!m_nScrollTimer && (m_bUseAutoScroll || pmf->m_bAllScroll) && m_nSrSpeed!=0)
					{
						if(m_nProgress==100 || m_nProgress<0 || (!m_bScrollAfterDL && m_nProgress>=0))
						{
							//						m_nScrollTimer = SetTimer(2132, (UINT)(1000*g_fSeed/m_nSrSpeed), NULL);
							if(pmf->m_bMouseCtrlScrSpeed)
								m_nLastScrSpd = STOP_TIMER;
							else
								m_nLastScrSpd = (UINT)((g_fSeed*100)/(m_nSrSpeed*m_nSrSpeed));
							m_nScrollTimer = SetTimer(2132, m_nLastScrSpd, NULL);
						}
					}
				}
				else if (m_lpszUrl != "about:blank")//3.3.6
					pmf->SetTabIcon( TI_100, nTabID, this );
				//update tab bar tab0
				if(pmf->m_nTabStyle==2)
					pmf->PostMessage(WM_UPDATE_TAB);
			}
			//restore homepage
			if(g_bLockHomepage)
			{
				CString strHomePage = pmf->GetHomePage();
				if (strHomePage != pmf->m_strHomePage)
				{
					if (strHomePage == "about:blank")//user may set blank in other place
						pmf->m_strHomePage = strHomePage;
					else
						pmf->SetHomePage(pmf->m_strHomePage);
				}
			}
			//better place here, here means the main page complete
//			//need run plugin on blank page,for example close right page
//			if (pmf->m_bAutoRunExternal)//gf if (pmf->m_bAutoRunExternal && !m_bExternalRunOK)
//			{
//				pmf->RunSelectedExternal((CChildFrame*)GetParentFrame(),TRUE);
//				m_bExternalRunOK = TRUE;
// 			}
			//better place here, some page reload image will cause OnProgrssChange
			if ( pmf->m_bReloadUnShownImage && (!_bRefreshing) && (m_lpszUrl.Find("baidu.")<0)&& (m_lpszUrl.Find("google")<0) && (m_lpszUrl!="about:blank") ) 
			{
				pmf->RunReloadUnloadImage((CChildFrame*)GetParentFrame());
			}

			if (m_nAutoRunCmd)
			{
				//must active the page, some command only run for active page
				pmf->MDIActivate(GetParentFrame());
				//if use post, user maybe active other page when reveive msg
				pmf->SendMessage(WM_COMMAND, m_nAutoRunCmd, 0);
				m_nAutoRunCmd = 0;
			}	

		}
		//scroll bar and background

		//gf 有这个会使baidu hi的网页完全加载后没有滚动条
//		if(m_bIsNewPopup &&(m_nProgress==100 || m_nProgress<0))
//			WBShowScrollBar(this);

		if(g_bAutoSetBG)
		{
			VARIANT color;
			color.vt = VT_BSTR;
			color.bstrVal = pmf->m_strBGColor.AllocSysString();
			pmf->SetBackground(this, color);
			SysFreeString(color.bstrVal);
		}
	}catch(...){}
}

void CMyIE9View::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD /*nFlags*/,
										  LPCTSTR lpszTargetFrameName, BOOL baPostedData,
										  LPCTSTR /*lpszHeaders*/, BOOL* pbCancel)
{
	LARGE_INTEGER liFinish;
	LARGE_INTEGER liFreq;
	BOOL bCanNew = FALSE;
	TCITEM TabCtrlItem;
	int nFilterType = -1;
	
	try{
		
		
		CString strUrl=lpszURL;

//		MYDEBUG(strUrl)
//		TRACE1("_bMaybeOpenNewWinByUser:%d", _bMaybeOpenNewWinByUser);

		CString strUrlLower = strUrl;
		strUrlLower.MakeLower();

		if (_bMaybeOpenNewWinByUser && IS_SCRIPT_STR(strUrlLower) ) //gfpop
		{
			_bScriptForFilter = TRUE;
		}

		//some script: first is "about:blank"
		if (_bMaybeOpenNewWinByUser && ("about:blank" == strUrlLower) ) //gfpop
		{
			_bScriptForFilter = TRUE;
		}

		_bMaybeOpenNewWinByUser = FALSE;

		if (pmf->m_bConfirmUnsafePage && ( !m_bStillOpenBadSite && CGFBusiness::IsBadSite(strUrlLower) ) )
		{		
			
			if(strUrl[0] == '2')
			{
				strUrl.SetAt(0, '1');
				
				CFilterSites::AddNotBadSite(strUrlLower);
				
			}
			
			
			if (!m_bPageOpenedInThisWindow && strUrl[0] != '1' )
			{
				
				CString strBlockedMsg = CGFBusiness::GetBlockedMsgFile(strUrl, CHelperStr::GetTopUrl(strUrl) );
				
				m_bStillOpenBadSite = FALSE;
				this->Navigate2( strBlockedMsg );
				
				*pbCancel=TRUE;
				return;
				
			}
			else if(strUrl[0] == '1')
			{
				CString strRealUrl = strUrl;
				strRealUrl = strRealUrl.Mid(1); 
				m_bStillOpenBadSite = TRUE;
				this->Navigate2( strRealUrl );
				
				
				*pbCancel=TRUE;
				return;
			}
			
		}
		
		
		if( 0 != strcmp(lpszURL, "about:blank") )
		{
			m_bPageOpenedInThisWindow = TRUE;
		}
		
		if (m_bForbidAutoNavigate) //auto refresh
		{
			*pbCancel=TRUE;
			return;
		}
		
		//down manager before ftp, for exam: ftp://123/asd.zip
		if (pmf->m_nManagerIndex)
		{
			if(pmf->IsDownloadAble(strUrl))
			{
				*pbCancel=TRUE;
				pmf->RunDownManagerScript(strUrl, (CChildFrame*)GetParentFrame());
				return;
			}
		}
		//must after down manager
		if (pmf->m_nFtpOpenType && !PRESS_CTRL && IS_FTP(strUrl))
		{
			_OpenFtpUrl(strUrl);
			*pbCancel=TRUE;
			return;
		}
		//open link in new window
		if (pmf->m_bOpenFolderOut)
		{
			if (IS_LOCAL_FOLDER(strUrl))
			{
				ShellExecute(NULL, "open", lpszURL , "", NULL, SW_SHOW );
				*pbCancel = TRUE;
				return;
			}
		}
		//init some var
		m_bExternalRunOK = FALSE;
		m_strFindKey = "";
		m_strHighLightKey = "";
		pmf->m_strLastNavUrl = lpszURL;
		
		//set prop
		CString strUrlNew = pmf->GetStandUrl(lpszURL);
		if ( pmf->m_nDownAll && m_bSpecialProperty==FALSE)
		{
			int oldType = pmf->GetUrlType(m_lpszUrl);
			int type = pmf->GetUrlType(lpszURL);
			//open other url in same window
			BOOL bSameUrl=false;
			if (m_lpszUrl==strUrlNew)
				bSameUrl = true;
			else
			{
				//!! http://abc and http://abc/
				int nLen1 = m_lpszUrl.GetLength();
				int nLen2 = strUrlNew.GetLength();
				if (nLen1 != nLen2)
				{
					nLen1 = min(nLen1,nLen2);
					if (m_lpszUrl.Left(nLen1) == strUrlNew.Left(nLen1) )
						bSameUrl = true;
				}
			}
			if ( !bSameUrl && (type!=0))//difficult dowith local file (%20 problem)
			{
				if (type == oldType)
					;
				else if (type==2)//internet file
				{
					if (pmf->m_nDownAll==1 && oldType==0)
						SetDefaultProperty();
					else if (pmf->m_nDownAll==2 && oldType<2)
						SetDefaultProperty();
					else
						;//do nothing
				}
				else if (type==1)//local net file
				{
					if (pmf->m_nDownAll==1 && oldType==0)
						SetDefaultProperty();
					else
						;//do nothing
				}
			}
			else if (type==3)//"about:blank"
				SetDefaultProperty();
			else if (pmf->m_nDownAll==1)
			{
				if (type==0)
					SetFullProperty();
				else
					SetDefaultProperty();
			}
			else if (pmf->m_nDownAll==2)
			{
				if (type==0 || type==1)
					SetFullProperty();
				else
					SetDefaultProperty();
			}
		}
		//
		CChildFrame* tcf = (CChildFrame*)GetParentFrame();
		BOOL m_bLock = tcf->m_bLock;
		
		//??
		if (m_strOpenNewUrl == lpszURL)
		{
			QueryPerformanceCounter(&liFinish);
			QueryPerformanceFrequency(&liFreq);
			double secs = (double)(liFinish.LowPart - m_tLastClick.LowPart)/(double)liFreq.LowPart;
			if(secs<POP_DELAY && secs>0)
			{
				m_strOpenNewUrl.Empty();
				*pbCancel = TRUE;
				return;
			}
		}

		m_strOpenNewUrl.Empty();
		//
		if(g_bLinkInNewWindow || m_bLock || m_nInNewWindow)
		{
			if(m_tLastClick.LowPart != 0 && !baPostedData)
			{
				QueryPerformanceCounter(&liFinish);
				QueryPerformanceFrequency(&liFreq);
				if(liFreq.LowPart!=0)
				{
					double secs = (double)(liFinish.LowPart - m_tLastClick.LowPart)/(double)liFreq.LowPart;
					if(secs<POP_DELAY && secs>0)
					{
						if (g_bKillPop && g_bKillDupWin)
						{
							int dup = pmf->FindDupURL(lpszURL, tcf);
							if(dup<0)
								bCanNew = TRUE;
							else
							{
								*pbCancel = TRUE;
								
								TabCtrlItem.mask = TCIF_PARAM;
								pmf->m_wndTab.GetItem(dup, &TabCtrlItem);
								CChildFrame* tcf2 = (CChildFrame*)TabCtrlItem.lParam;
								if(tcf2!=NULL && tcf2!=tcf)
									pmf->MDIActivate(tcf2);
							}
						}
						else
							bCanNew = TRUE;
					}
					else
						m_tLastClick.LowPart = 0;
				}
			}
			//
			if(!m_bFirst && pmf!=NULL && bCanNew)
			{
				char* pdest = strchr(lpszURL, '#');
				if(pdest!=NULL)
				{
					if(strncmp(lpszURL, m_lpszUrl, pdest - lpszURL)!=0)
						pdest = NULL;
				}
				BOOL bScript = IS_SCRIPT_STR(lpszURL);
				if ( !bScript && !IS_MAILTO_STR(lpszURL) && pdest==NULL )
				{
					CChildFrame *tcf = NULL;
					tcf = pmf->NewChildWindow(0,1, NULL, this);
					if(tcf != NULL)
					{
						tcf->m_pView->m_dwProperty = m_dwProperty;
						tcf->m_pView->m_bIsNewPopup = FALSE; //TRUE;
						tcf->m_pView->m_bFirst = TRUE;
						tcf->m_pView->m_nProgress = 0;
						//must before native
						tcf->m_nActive = m_nInNewWindow;
						tcf->m_bForceActive = m_nInNewWindow & INW_ACTIVE;
						//for that OnCreateClient will modify m_bIsActive, modify back at here
						tcf->m_pView->m_bIsActive = (CAN_ACTIVE(tcf) ? TRUE : FALSE);
						m_nInNewWindow = 0;
						//
						tcf->m_pView->ToNavigate(lpszURL);
						if(pbCancel!=NULL)
							*pbCancel = TRUE;
						m_strOpenNewUrl = lpszURL;
						return;
					}
				}
				//not clear script link, it will open in OnNewWindow2
				if (!bScript)
					m_nInNewWindow = 0;
			}
		}//end lock and open new
		
		m_bFirst = FALSE;
		m_bForceRefresh =  FALSE;
		

//		filter popup
				if(g_bKillPop && g_bKillListWin && lpszURL!=NULL && strcmp(lpszURL, "about:blank")!=0 && strUrlNew==m_lpszUrl)
				{
					if (pmf->NeedPopFilter(strUrlNew))
					{
						if (pmf->ConfirmPopupFilter(strUrlNew, FALSE))
						{
							if(pbCancel!=NULL)
								*pbCancel = TRUE;
							m_bIsPopUp = TRUE;
							m_strPossibleURL = strUrlNew;
							m_nProgress = -1;
							//add counter
							nFilterType = 2;
							pmf->AddToPopList(strUrlNew, nFilterType);
						}
					}
					else
					{
						m_nProgress = 0;
						if(m_bIsActive)
						{	
							//if "mailto:" then not chang g_nPercent
							if (strUrl.Left(5)=="http:" || strUrl.Left(5)=="file:" || strUrl.Find(':')==1)
							{
								g_nPercent = m_nProgress;
								//
								int nIcon = m_nProgress/34+1;
								int nTabID = pmf->FindTab((CChildFrame*)GetParentFrame());
								if(m_nProgress>=0)
									pmf->SetTabIcon( nIcon, nTabID, this );
							}
						}
					}
				}

		//filter duplicate url
		if(g_bKillPop && g_bKillDupWin && m_nFilterDup && !m_bIsPopUp && m_bIsNewPopup && pmf!=NULL && lpszURL!=NULL && strcmp(lpszURL, "about:blank")!=0 )//&& strUrlNew==m_lpszUrl)
		{
			//only filter once the window popup
			m_nFilterDup --;
			int dup = pmf->FindDupURL(strUrlNew, tcf);
			if(dup>=0)
			{
				//if (pmf->ConfirmPopupFilter(strUrlNew,1))
				{
					if(pbCancel!=NULL)
						*pbCancel = TRUE;
					m_bIsPopUp = TRUE;
					m_strPossibleURL = strUrlNew;
					m_nDupID = dup;
					m_nProgress = -1;
					m_nFilterDup = 0;
					
					//active that dup window
					if(tcf && pmf)
					{
						TabCtrlItem.mask = TCIF_PARAM;
						pmf->m_wndTab.GetItem(m_nDupID, &TabCtrlItem);
						CChildFrame* tcf2 = (CChildFrame*)TabCtrlItem.lParam;
						if(tcf2!=NULL && tcf2!=tcf)
						{
							pmf->MDIActivate(tcf2);
							//pmf->m_wndTab.SetCurSel(m_nDupID);
						}
						m_nDupID = -1;
					}
				}
			}
			else
			{
				m_nProgress = 0;
				if(m_bIsActive)
				{	
					//if "mailto:" then not chang g_nPercent
					if (strUrl.Left(5)=="http:" || strUrl.Left(5)=="file:" || strUrl.Find(':')==1)
					{
						g_nPercent = m_nProgress;
						//
						int nIcon = m_nProgress/34+1;
						int nTabID = pmf->FindTab((CChildFrame*)GetParentFrame());
						if(m_nProgress>=0)
							pmf->SetTabIcon( nIcon, nTabID, this );
					}
				}
			}
		}


		//if have filter not auto pop filter
		if (*pbCancel==TRUE)
		{
//gfpop del			m_bAutoPopWin = FALSE;

			//when not have page, list filter this, will make a error window
			//but not close dup window
			if (nFilterType==0 || nFilterType==2)
			{
				CChildFrame* tcf = ((CChildFrame*)GetParentFrame());
				if(tcf!=NULL)
				{
					m_bToClose = TRUE;
					tcf->PostMessage(WM_CLOSE);
				}
			}
 		}

		//gfpop del
//		
//		if (m_bAutoPopWin)
//		{
//			m_bAutoPopWin = FALSE;
//			BOOL bNeedFilter=pmf->NeedUnPopFilter(strUrlNew);
//			if (bNeedFilter && pmf->ConfirmPopupFilter(strUrlNew, TRUE))
//			{
//				if(pbCancel!=NULL)
//					*pbCancel = TRUE;
//				m_bIsPopUp = TRUE;
//				m_strPossibleURL = strUrlNew;
//				m_nProgress = -1;
//				//must close the tcf
//				CChildFrame* tcf = ((CChildFrame*)GetParentFrame());
//				if(tcf!=NULL)
//				{
//					m_bToClose = TRUE;
//					tcf->PostMessage(WM_CLOSE);
//				}
//				//
//				pmf->AddToPopList(strUrlNew, 0);
//				return;
//			}
//		}

		//secure info
		if(strncmp(lpszURL, "https", 5)!=0)
			m_bIsSecure = FALSE;
		if(m_bIsActive && pmf!=NULL)
			pmf->m_bIsSecure = m_bIsSecure;
		
		//reused window
		if(!m_bIsPopUp && m_bReUsed && tcf!=NULL && pmf!=NULL && CAN_ACTIVE(tcf))
		{
			int nTabID = pmf->FindTab(tcf);
			//active window
			pmf->MDIActivate(tcf);
			//select current tab
			pmf->m_wndTab.SetCurSel( nTabID );
			tcf->m_bForceActive = FALSE;
		}
		m_bReUsed = FALSE;
		
		//Add a tab to tabbar
		if(!m_bIsPopUp && m_bDelayAdd && tcf!=NULL && pmf!=NULL)
		{
			tcf->ShowWindow(SW_SHOW);
			m_bDelayAdd = FALSE;
			//add tab
			pmf->AddNewTab(tcf);
			int nTabID = pmf->FindTab(tcf);
			//active window
			if (CAN_ACTIVE(tcf))
			{
				try
				{
				
				if (GF_NO_DELAY_SHOW)
				{
					tcf->m_pView->ShowWindow(SW_SHOW);
				}

				pmf->MDIActivate(tcf);

				
				if (GF_NO_DELAY_SHOW)
				{
					tcf->ViewSetFocus();
				}

				}catch (...) {
					ASSERT(false);
				}

				//select current tab
				pmf->m_wndTab.SetCurSel( nTabID );
				tcf->m_bForceActive = FALSE;
			}
			//after use m_nActive, set back to 0, BeforeNavigate2 TargetFrameName will use this
			tcf->m_nActive = 0;
			//set icon
			pmf->SetTabIcon( TI_0, nTabID, this );
			//update tab bar
			if(pmf->m_nTabStyle==2)
				pmf->PostMessage(WM_UPDATE_TAB);
			pmf->PostMessage(WM_UPDATE_TAB_TIP);
		}
	}catch(...){}
}

// these are all simple one-liners to do simple controlling of the browser
void CMyIE9View::OnGoBack()
{
	try{

		if(m_bBack)
			GoBack();
		

//		if(m_bBack)
//			GoBack();
//		else //gfback
//		{
//			pmf->OnFileClose();
// 		}
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnGoForward()
{
	try{
		
		if(m_bForward)
			GoForward();
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel) 
{
	// TODO: Add your specialized code here and/or call the base class
	CChildFrame *tcf = NULL;
	if(ppDisp == NULL || Cancel == NULL)
		return;
	
	try{

//		m_pBrowserApp->put_RegisterAsBrowser(TRUE); //gfdb

		*Cancel = TRUE;
		*ppDisp = NULL;
		//auto filter popup window not opened by user
		if(g_bKillPop && g_bKillAutoWin && !PRESS_CTRL)
		{
			//this window is to be closed, so don't open any other windows from it.
			if(m_bToClose)
				return;

//gfpop del
//			LARGE_INTEGER liFinish;
//			LARGE_INTEGER liFreq;
//			QueryPerformanceCounter(&liFinish);
// 			QueryPerformanceFrequency(&liFreq);
			
			//gfpop del if(liFreq.LowPart!=0)
			{

				//gfpop del
// 				CString strUrl;
//				double secs = (double)(liFinish.LowPart - m_tLastClick.LowPart)/(double)liFreq.LowPart;
//				if((secs>POP_DELAY && secs<4.0 && m_bLClick) || m_nProgress==-1)
//				{
//					if(m_nProgress>=0)
//						strUrl = GetPointUrl(m_ptLClick);
//					else if(GetBusy())//whether browser is busy when m_mPorgress <0
//						m_nProgress=0;
// 				}
				//
				try{

					//gfpop del
//					if	(
//						!(	m_bLClick
//						&&
//						( (secs>=0 && secs<=POP_DELAY) || IS_URL(strUrl) || m_nProgress==-1 )
//						) 
// 						)
					{
						int bFilter = TRUE;
						//may click open in new window of pop menu
						if (m_bRClick)
						{
							if (m_bRClick == RC_NOT_FILTER)
							{
								bFilter = FALSE;//not filter
								m_bRClick = 0;
							}
							else
							{
								//CString strUrl = GetPointUrl(pmf->m_ptMouseDown);
								if (IsPointLink( GetPointInView(pmf->m_ptMouseDown) ) )
									bFilter = FALSE;//not filter
 							}
						}


						if ( (!pmf->m_bFilterRefresh) && _bRefeshedForFilter)
								bFilter = FALSE;

						//enable always. _bRefeshedForFilter = FALSE;

						if (_bScriptForFilter)
						{
							bFilter = FALSE;
						}

						_bScriptForFilter = FALSE;

						if (_bMaybeOpenNewWinByUser) //gfpop
						{
							bFilter = FALSE;
						}

						_bMaybeOpenNewWinByUser = FALSE;

						//for that here not know the url, have to get the url at OnBeforeNavigate2
						if (bFilter)
						{
							if ( (!pmf->IsInUnFilter(m_lpszUrl) ) &&(pmf->ConfirmPopupFilter(m_lpszUrl, FALSE)) )
							{
								//add counter
								pmf->AddToPopList(m_lpszUrl, 0);

								//5.5.27, use may use unfilter list
								/*if (!pmf->m_bOutputSaveFilter)
								{
								pmf->m_nPopFiltered++;
								if (pmf->m_bPlayPopupFilterSound)
								::sndPlaySound(theApp.m_strRoot+"Resource\\PopFilter.wav", SND_ASYNC);
								return;
								}
								else*/


//gfpop del								pmf->m_bAutoPopWin = TRUE;
								return;  //gfpop will get the url in onbeforenavigate
							}
						}
					}
			}catch(...){
				ASSERT(false);}
			}
		}
		m_bLClick = FALSE;
		
		//new child window from this
		tcf = pmf->NewChildWindow(0,1, NULL, this);
		
		//must do this, for pop in exist blank window
//gfpop del
//		if (pmf->m_bAutoPopWin)
//		{
//			tcf->m_pView->m_bAutoPopWin = TRUE;
//			pmf->m_bAutoPopWin = FALSE;
//		}

		if (m_nInNewWindow)
		{
			tcf->m_nActive = m_nInNewWindow;
			tcf->m_bForceActive = m_nInNewWindow & INW_ACTIVE;
			tcf->m_pView->m_bIsActive = (CAN_ACTIVE(tcf) ? TRUE : FALSE);
			m_nInNewWindow = 0;
		}
		//this maybe a blank window.
		if(((CChildFrame*)GetParentFrame())->m_pBlankChild != tcf)
		{
			((CChildFrame*)GetParentFrame())->m_pBlankChild = tcf;
			QueryPerformanceCounter(&((CChildFrame*)GetParentFrame())->m_tBlankChild);
		}
		else
			((CChildFrame*)GetParentFrame())->m_pBlankChild = NULL;
		tcf->m_pView->m_dwProperty = m_dwProperty;
		tcf->m_pView->m_bIsNewPopup = TRUE;
		tcf->m_pView->m_bIsPopUp = FALSE;
		tcf->m_pView->m_bToClose = FALSE;
		tcf->m_pView->m_nFilterDup = 1;
		//tcf->m_pView->m_bForceResize = TRUE;
		tcf->m_pView->m_nProgress = 0;
		//tcf->m_pView->g_bKillAutoWin = g_bKillAutoWin;
		*ppDisp = tcf->m_pView->GetApplication();
		if(*ppDisp != NULL)
			*Cancel = FALSE;
		
	}catch(...){}
	
	//CFixedHtmlView::OnNewWindow2(ppDisp, Cancel);
}

void CMyIE9View::OnProgressChange(long nProgress, long nProgressMax) 
{
	// TODO: Add your specialized code here and/or call the base class
	try{
		int oldp = m_nProgress;
		if(nProgressMax>0 && nProgress>=0 && nProgressMax/100>0)
		{
			m_nProgress  = nProgress/(nProgressMax/100);
			if(m_nProgress>100)
				m_nProgress=100;
		}
		else
		{
			m_nProgress = -10;
		}
		if(m_bIsActive)
			g_nPercent = m_nProgress;
		
		if(m_bIsActive && (m_bUseAutoScroll || pmf->m_bAllScroll) )
		{
			//start to scroll
			if(m_nScrollTimer==0 && (m_bUseAutoScroll || pmf->m_bAllScroll) && m_nSrSpeed!=0)
			{
				if(m_nProgress==100 || m_nProgress<0 || (!m_bScrollAfterDL && m_nProgress>=0))
				{
					if(pmf->m_bMouseCtrlScrSpeed)
						m_nLastScrSpd = STOP_TIMER;
					else
						m_nLastScrSpd = (UINT)((g_fSeed*100)/(m_nSrSpeed*m_nSrSpeed));
					m_nScrollTimer = SetTimer(2132, m_nLastScrSpd, NULL);
				}
			}
			else if(m_nScrollTimer && m_nProgress<20 && m_nProgress>0)
			{
				//stop the timer;
				if(m_nScrollTimer)
					KillTimer(m_nScrollTimer);
				m_nScrollTimer = 0;
			}
		}
		//set icon
		if(pmf!=NULL)
		{
			int nIcon = m_nProgress/34+1;
			int oi = m_nOldIcon;
			if(m_nProgress<0)
				nIcon=-1;
			if((m_nProgress>=0 && nIcon != oi) || m_nProgress<0)
			{
				int nTabID;
				nTabID = pmf->FindTab((CChildFrame*)GetParentFrame());
				if(m_nProgress>=0 && nIcon != oi)
					pmf->SetTabIcon( nIcon, nTabID, this );
				else if(m_nProgress<0)
				{
					if(((CChildFrame*)GetParentFrame())->m_bLock)
						pmf->SetTabIcon( TI_LOCK, nTabID, this );
					else if(((CChildFrame*)GetParentFrame())->m_bProtect)
						pmf->SetTabIcon( TI_PROTECT, nTabID, this );
					else
					{
						if(!m_bIsActive)
						{
							if (m_nOldIcon!=TI_0 && nProgress==0 && nProgressMax==0 && m_lpszUrl != "about:blank")//3.3.6
								pmf->SetTabIcon( TI_100, nTabID, this );
						}
						else
						{
							if (m_nOldIcon!=TI_0)
								pmf->SetTabIcon( TI_NO, nTabID, this );
						}
					}
				}
				//update tab bar
				if(m_nProgress*oldp<=0 && oi*m_nOldIcon<=0 && m_nOldIcon != oi && pmf->m_nTabStyle==2)
					pmf->PostMessage(WM_UPDATE_TAB);
				//complete
				if (nProgress==0 && nProgressMax==0)
				{
					m_bPageOpenedInThisWindow = FALSE;
					
					CChildFrame* tcf = (CChildFrame*)pmf->MDIGetActive();
					if (tcf && pmf->m_nState)
					{
						if (tcf->m_pView->m_lpszUrl == "about:blank" &&
							tcf == (CChildFrame*)GetParentFrame() )
						{
							pmf->m_wndAddress->SetFocus();
						}
						else
						{
							CWnd* pWnd = GetFocus();
							if ( pWnd == (CWnd*)(pmf->m_wndAddress->GetEditCtrl()) || pWnd == (CWnd*)(pmf->m_wndSearch->GetEditCtrl())	)
								;//do nothing
							else if (IsTopParentActive())
								tcf->ViewSetFocus();
						}
					}
					//run internal plugin,must place here, for that refresh not get ondocumentcomplete
					if (m_nOldIcon!=TI_0)// && m_lpszUrl!="about:blank")
					{
						//DocShowedOK();
					}
				}
			}
		}
	}catch(...){}
	
	//CFixedHtmlView::OnProgressChange(nProgress, nProgressMax);
}

void CMyIE9View::OnEditCopy() 
{
	try{
		
		// TODO: Add your command handler code here
		ExecWB(OLECMDID_COPY, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnEditCut() 
{
	try{
		
		// TODO: Add your command handler code here
		ExecWB(OLECMDID_CUT, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnEditFind() 
{
	try{
		
		// TODO: Add your command handler code here
		pmf->ExecCmdTarget(HTMLID_FIND, this);
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnEditPaste() 
{
	try{
		
		// TODO: Add your command handler code here
		ExecWB(OLECMDID_PASTE, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnEditSelectAll() 
{
	try{
		
		// TODO: Add your command handler code here
		ExecWB(OLECMDID_SELECTALL, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnStatusTextChange(LPCTSTR lpszText) 
{
	// TODO: Add your specialized code here and/or call the base class
	try{
		if(m_bIsActive && pmf!=NULL)
		{
			if(g_bDisScrollText && lpszText!=NULL && strlen(lpszText)!=0)
			{
				char* pdes = strstr(lpszText, "about:");
				if(pdes!=lpszText)
				{
					pdes = strstr(lpszText, "://");
					if(pdes == NULL)
					{
						pdes = strstr(lpszText, "mailto:");
						if(pdes == NULL)
						{
							pdes = strstr(lpszText, "javascript:");
						}
					}
				}
				int pos =0;
				if(pdes != NULL)
				{
					pos = pdes - lpszText;
				}
				if(strncmp(lpszText,"Shortcut", 8)!=0 && strstr(lpszText, "的快捷方式")==NULL && strstr(lpszText, "地址已")==NULL && strcmp(lpszText, "Done")!=0&& (pdes==NULL || pos>10 )
					&& strncmp(lpszText, "正在", 4)!=0 && strncmp(lpszText, "完成", 4)!=0 && strncmp(lpszText, "完毕", 4)!=0 && strstr(lpszText, "项剩余")==NULL 
					&& strncmp(lpszText, "Wait", 4)!=0 && strncmp(lpszText, "Start", 5)!=0 && strncmp(lpszText, "Open", 4)!=0 && strncmp(lpszText, "Connect", 7)!=0 && strncmp(lpszText,"Find",4)!=0 && strncmp(lpszText, "download", 8)!=0 && strncmp(lpszText, "Web", 3)!=0 && strstr(lpszText, "remain")==NULL)
					return;
			}
			if( !( pmf->m_bStatusBlank && lpszText!=NULL && strlen(lpszText)==0))
				pmf->SetMessageText(lpszText);
			if(strlen(lpszText)==0)
				pmf->m_bStatusBlank = TRUE;
			else
				pmf->m_bStatusBlank = FALSE;
			
		}
	}catch(...){}
}

BOOL CMyIE9View::OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar) 
{
	try{
		
		// TODO: Add your specialized code here and/or call the base class
		if (dispid == DISPID_AMBIENT_DLCONTROL)
		{
			pvar->vt = VT_I4;
			pvar->lVal = m_dwProperty; 
			if (pmf->m_bDisDownActivex)
				pvar->lVal |= DLCTL_NO_DLACTIVEXCTLS;
			return TRUE;
		}
		return CFixedHtmlView::OnAmbientProperty(pSite, dispid, pvar);
		
	}catch(...){ASSERT(false); return FALSE;}
}

void CMyIE9View::OnOptionsLoadimage() 
{
	try{
		
		// TODO: Add your command handler code here
		m_dwProperty ^= DLCTL_DLIMAGES;
		pmf->m_dwDefaultProperty ^= DLCTL_DLIMAGES;//gf
		ChangeProperty();
		if (!PRESS_CTRL)
			PowerRefresh();
		
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnUpdateOptions(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
		switch (pCmdUI->m_nID)
		{
		case ID_OPTIONS_LOADIMAGE:
			pmf->ToolBarSetCheck(pCmdUI, (m_dwProperty&DLCTL_DLIMAGES)!=0);
			break;
		case ID_OPTIONS_LOADVIDEOS:
			if(m_dwProperty&DLCTL_VIDEOS)
				pCmdUI->SetCheck(1);
			else
				pCmdUI->SetCheck(0);
			break;
		case ID_OPTIONS_LOADSOUNDS:
			if(m_dwProperty&DLCTL_BGSOUNDS)
				pCmdUI->SetCheck(1);
			else
				pCmdUI->SetCheck(0);
			break;
		case ID_OPTIONS_ALLOWSCRIPTS:
			if(m_dwProperty&DLCTL_NO_SCRIPTS)
				pCmdUI->SetCheck(0);
			else
				pCmdUI->SetCheck(1);
			break;
		case ID_OPTIONS_ALLOWJAVAAPPLET:
			if(m_dwProperty&DLCTL_NO_JAVA)
				pCmdUI->SetCheck(0);
			else
				pCmdUI->SetCheck(1);
			break;
		case ID_OPTIONS_ALLOWACTIVEX:
			if(m_dwProperty&DLCTL_NO_RUNACTIVEXCTLS)
				pCmdUI->SetCheck(0);
			else
				pCmdUI->SetCheck(1);
			break;
		}
	}catch(...){}
}

void CMyIE9View::OnOptionsAllowscripts() 
{
	try{
		
		// TODO: Add your command handler code here
		m_dwProperty ^= DLCTL_NO_SCRIPTS;
		pmf->m_dwDefaultProperty ^= DLCTL_NO_SCRIPTS;//gf
		ChangeProperty();
		if (!PRESS_CTRL)
			PowerRefresh();
		
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnOptionsLoadsounds() 
{
	try{
		
		// TODO: Add your command handler code here
		m_dwProperty ^= DLCTL_BGSOUNDS;
		pmf->m_dwDefaultProperty ^= DLCTL_BGSOUNDS;
		ChangeProperty();
		if (!PRESS_CTRL)
			PowerRefresh();
		
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnOptionsLoadvideos() 
{
	try{
		
		// TODO: Add your command handler code here
		m_dwProperty ^= DLCTL_VIDEOS;
		pmf->m_dwDefaultProperty ^= DLCTL_VIDEOS;
		ChangeProperty();
		if (!PRESS_CTRL)
			PowerRefresh();
		
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnOptionsUseWebp() 
{
	try{
		
		// TODO: Add your command handler code here
		if(m_bEnWebProxy)
			m_bEnWebProxy = FALSE;
		else
		{
			m_bEnWebProxy = TRUE;
			//auto refresh
			if (m_strWebProxy.IsEmpty())
			{
				if (!pmf->m_strDefaultWebProxy.IsEmpty())
					m_strWebProxy = pmf->m_strDefaultWebProxy;
				else
				{
					m_bEnWebProxy = FALSE;
					MSGBOX(IDS_NO_WEB_PROXY);
					return;
				}
			}
			if(!m_lpszUrl.IsEmpty() && m_lpszUrl!="about:blank")
			{
				ToNavigate(m_lpszUrl, 0, NULL, NULL, NULL, 0, TRUE);
			}
		}
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnUpdateOptionsUseWebp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try
	{
		pmf->ToolBarSetCheck(pCmdUI, m_bEnWebProxy);
	}catch(...){}
}

void CMyIE9View::OnOptionsAllowactivex() 
{
	try{
		
		// TODO: Add your command handler code here
		m_dwProperty ^= DLCTL_NO_RUNACTIVEXCTLS;
		pmf->m_dwDefaultProperty ^= DLCTL_NO_RUNACTIVEXCTLS;
		ChangeProperty();
		if (!PRESS_CTRL)
			PowerRefresh();
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnOptionsAllowjavaapplet() 
{
	try{
		
		// TODO: Add your command handler code here
		m_dwProperty ^= DLCTL_NO_JAVA;
		pmf->m_dwDefaultProperty ^= DLCTL_NO_JAVA;
		ChangeProperty();
		if (!PRESS_CTRL)
			PowerRefresh();
		
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnCommandStateChange(long nCommand, BOOL bEnable) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nCommand == 2)
		m_bBack = bEnable;
	else if(nCommand == 1)
		m_bForward = bEnable;
	
	//	CFixedHtmlView::OnCommandStateChange(nCommand, bEnable);
}

void CMyIE9View::OnUpdateGoBack(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
		pCmdUI->Enable(m_bBack);
//		if(pmf->m_wndTab.GetItemCount() == 1) 
//		{
//			pCmdUI->Enable(m_bBack);
//		}
//		else
//		{
//			pCmdUI->Enable(TRUE); //gfback m_bBack
//		}
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnUpdateGoForward(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
		pCmdUI->Enable(m_bForward);
	}catch(...){}
}

void ResizeSibling(CMyIE9View* pvw)
{
	try{
		g_bMax = 0;
		CChildFrame* tcf;
		CRect oldrect,rect;
		if(pmf!=NULL)
		{
			int n = pmf->m_wndTab.GetItemCount();
			TCITEM TabCtrlItem;
			TabCtrlItem.mask = TCIF_PARAM;
			for(int i=n-1;i>=0;i--)
			{
				pmf->m_wndTab.GetItem(i, &TabCtrlItem);
				tcf = ((CChildFrame*)TabCtrlItem.lParam);
				if(tcf != NULL && tcf->m_pView!=NULL)
				{
					if(tcf->m_pView != pvw)
					{
						tcf->GetClientRect(&rect);
						tcf->m_pView->m_wndBrowser.GetWindowRect(&oldrect);
						tcf->m_pView->ScreenToClient(&oldrect);
						if(oldrect.left != -BORDER && oldrect.top != -BORDER)
						{
							try{
								tcf->m_pView->m_wndBrowser.MoveWindow(-2,-2, rect.Width()+BORDER+BORDER, rect.Height()+BORDER+BORDER);
						}catch(...){}
						}
						else
						{
							try{
								if(oldrect.Width()!=rect.Width()+BORDER+BORDER && rect.Width()!=0)
									tcf->m_pView->SetWidth (rect.Width()+BORDER+BORDER);
								if(oldrect.Height()!=rect.Height()+BORDER+BORDER && rect.Height()!=0)
									tcf->m_pView->SetHeight (rect.Height()+BORDER+BORDER);
						}catch(...){}
						}
					}
				}
			}
		}
	}catch(...){}
}

void CMyIE9View::OnSize(UINT nType, int cx, int cy) 
{
	try
	{
		if(g_bMax==1)
		{
			if(GetParentFrame()->IsZoomed())
			{
				CRect oldrect;
				m_wndBrowser.GetWindowRect(&oldrect);
				ScreenToClient(&oldrect);
				if(oldrect.left != -BORDER && oldrect.top != -BORDER)
				{
					try{
						m_wndBrowser.MoveWindow(-2,-2, cx+BORDER+BORDER, cy+BORDER+BORDER);
					}catch(...){}
				}
				else
				{
					try{
						if(oldrect.Width()!=cx+BORDER+BORDER )
							SetWidth (cx+BORDER+BORDER);
						if(oldrect.Height()!=cy+BORDER+BORDER)
							SetHeight (cy+BORDER+BORDER);
					}catch(...){}
				}
			}
		}
		else
		{
			CRect oldrect;
			m_wndBrowser.GetWindowRect(&oldrect);
			ScreenToClient(&oldrect);
			if(oldrect.left != -BORDER && oldrect.top != -BORDER)
			{
				try{
					m_wndBrowser.MoveWindow(-2,-2, cx+BORDER+BORDER, cy+BORDER+BORDER);
				}catch(...){}
			}
			else
			{
				try{
					if(oldrect.Width()!=cx+BORDER+BORDER && cx!=0)
						SetWidth (cx+BORDER+BORDER);
					if(oldrect.Height()!=cy+BORDER+BORDER && cy!=0)
						SetHeight (cy+BORDER+BORDER);
				}catch(...){}
			}
			if(GetParentFrame()->GetStyle() & WS_MAXIMIZE)
				g_bMax = 1;
		}
		//size other views
		if(g_bMax == 3)
			ResizeSibling(this);
	}catch(...){}
}

void CMyIE9View::OnUpdateViewFonts(CCmdUI* pCmdUI) 
{ 
	// TODO: Add your command update UI handler code here
	if(pCmdUI->m_nID==ID_VIEW_FONTS_LARGEST+m_nFontSize)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CMyIE9View::ToNavigate(LPCTSTR URL, DWORD dwFlags, LPCTSTR lpszTargetFrameName, LPCTSTR lpszHeaders, LPVOID lpvPostData,  DWORD dwPostDataLen, BOOL bOnlyRefresh)
{
	try{
		CString str = URL;
		CString str2 = URL;

		str.TrimLeft();
		if (IS_SCRIPT(str))
		{
			pmf->RunScript(str);
			return;
		}

		m_lpszUrl=URL;
		m_bFirst = TRUE;
		
		if(m_bEnWebProxy)
		{
			if(m_strWebProxy.IsEmpty() && pmf!=NULL)
				m_strWebProxy = pmf->m_strDefaultWebProxy;
			
			if((str.Left(4)=="http" || str.Left(4)=="www.") && str.Find(m_strWebProxy)<0)
			{
				if(str.Left(4)=="www.")
					str2 = "http://" + str;
				if(m_strWebProxy.Find("%s")>0)
				{
					str2.Replace(":", "%3A");
					str2.Replace("/", "%2F");
					str = m_strWebProxy;
					str.Replace("%s", str2);
				}
				else if(m_strWebProxy.Find("%S")>0)
				{
					str = m_strWebProxy;
					str.Replace("%S", str2);
				}
				else
					str = m_strWebProxy+str2;
				Navigate2(str, dwFlags, lpszTargetFrameName, lpszHeaders,lpvPostData, dwPostDataLen);
			}
			else if(!bOnlyRefresh)
				Navigate2(str, dwFlags, lpszTargetFrameName, lpszHeaders,lpvPostData, dwPostDataLen);
		}
		else if(!bOnlyRefresh)
		{
			CString strGo = CGf_TranslateUrl::TranslatedToMy(str);
			Navigate2(strGo, dwFlags, lpszTargetFrameName, lpszHeaders,lpvPostData, dwPostDataLen);
		}
		
	}catch(...){}
	
	m_nProgress = -1;
}

void CMyIE9View::OnParentNotify(UINT message, LPARAM lParam) 
{
	try{
		
		CFixedHtmlView::OnParentNotify(message, lParam);
	}catch(...){ASSERT(false);}
	
	// TODO: Add your message handler code here
	try{
		CChildFrame* tcf = (CChildFrame*)GetParentFrame();
		//
		pmf->m_ptMouseDown.x = LOWORD((DWORD)lParam);
		pmf->m_ptMouseDown.y = HIWORD((DWORD)lParam);

		if (message==WM_LBUTTONDOWN && pmf->m_nLButtonDown)
		{
			if (MouseAction(pmf->m_nLButtonDown))
			{
				if (pmf->m_nMButtonDown>=ID_OPEN_LINK && pmf->m_nMButtonDown<=ID_OPEN_LINK_DEACTIVE)
				{
					//for that simu as shift+click 
					QueryPerformanceCounter(&m_tLastClick);
					m_bLClick = TRUE;
				}
				return;
			}
		}
		else if (message==WM_RBUTTONDOWN && pmf->m_nRButtonDown)
		{
			if (MouseAction(pmf->m_nRButtonDown))
			{
				if (pmf->m_nMButtonDown>=ID_OPEN_LINK && pmf->m_nMButtonDown<=ID_OPEN_LINK_DEACTIVE)
				{
					//for that simu as shift+click 
					QueryPerformanceCounter(&m_tLastClick);
					m_bLClick = TRUE;
				}
				SetCapture();
				m_bIsCapture = TRUE;
				pmf->m_bPopRightMenu=FALSE;
				return;
			}
		}
		//
		if((message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN) && (g_bLinkInNewWindow || tcf->m_bLock || g_bKillAutoWin) )
		{
			if(message == WM_LBUTTONDOWN )
			{
				QueryPerformanceCounter(&m_tLastClick);
				m_bRClick = FALSE;
				m_bLClick = TRUE;
				m_ptLClick.x = LOWORD((DWORD)lParam);
				m_ptLClick.y = HIWORD((DWORD)lParam);
			}
			else if(message == WM_RBUTTONDOWN )
			{
				QueryPerformanceCounter(&m_tLastRClick);
				m_bRClick = TRUE;
				m_bLClick = FALSE;
			}
			else
				m_bRClick = FALSE;
		}
		if  ( (message==WM_RBUTTONDOWN) && 
			(pmf->m_bRMouseDrag || pmf->m_bRMouseGesture)
			)
		{
			SetCapture();
			m_bIsCapture = TRUE;
			//m_bRClick = FALSE;
			m_ptRClick.x = LOWORD((DWORD)lParam);
			m_ptRClick.y = HIWORD((DWORD)lParam);
			m_ptPrev = m_ptRClick;
			g_nGestureHead = 0;
			g_nGestureTail = 0;
			pmf->m_bPopRightMenu=TRUE;
			
			//gfG
			if (pmf->m_bRMouseGesture)
			{
				CString str;
				LOADSTR(str, IDS_RIGHT_BUTTON_TIP_GESTURE);
				pmf->SetMessageText(str);
			}			
			else if (pmf->m_bRMouseDrag)
			{
				CString str;
				LOADSTR(str, IDS_RIGHT_BUTTON_TIP_DRAG);
				pmf->SetMessageText(str);
			}
			
		}
	}catch(...){}
}


void CMyIE9View::OnToolsAutoscrollAutoscrollingafterdownloading() 
{
	// TODO: Add your command handler code here
	m_bScrollAfterDL = !m_bScrollAfterDL;
	
	if(pmf!=NULL)
		pmf->m_bDefScrollAfterDL = m_bScrollAfterDL;
}

void CMyIE9View::OnUpdateToolsAutoscrollAutoscrollingafterdownloading(CCmdUI* pCmdUI) 
{
	try{
		
		// TODO: Add your command update UI handler code here
		pCmdUI->SetCheck(m_bScrollAfterDL);
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnUpdateToolsAutoscrollSpeed(CCmdUI* pCmdUI) 
{
	try{
		
		// TODO: Add your command update UI handler code here
		if(pmf->m_bMouseCtrlScrSpeed)
			pCmdUI->Enable(FALSE);
		else
			pCmdUI->Enable();
		
		if(pCmdUI->m_nID == ID_TOOLS_AUTOSCROLL_FAST)
			pCmdUI->SetCheck(m_nSrSpeed==3);
		else if(pCmdUI->m_nID == ID_TOOLS_AUTOSCROLL_MEDIUM)
			pCmdUI->SetCheck(m_nSrSpeed == 2);
		else
			pCmdUI->SetCheck(m_nSrSpeed == 1);
		
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnToolsAutoscrollUseautoscrolling() 
{
	try{
		
		// TODO: Add your command handler code here
		m_bUseAutoScroll = !m_bUseAutoScroll;
		if(pmf!=NULL)
			pmf->m_bDefUseAutoScroll = m_bUseAutoScroll;
		
		//start to scroll
		if(!m_nScrollTimer && (m_bUseAutoScroll || pmf->m_bAllScroll) && m_nSrSpeed!=0)
		{
			if(m_nProgress==100 || m_nProgress<0 || (!m_bScrollAfterDL && m_nProgress>=0))
			{
				if(pmf->m_bMouseCtrlScrSpeed)
					m_nLastScrSpd = STOP_TIMER;
				else
					m_nLastScrSpd = (UINT)((g_fSeed*100)/(m_nSrSpeed*m_nSrSpeed));
				m_nScrollTimer = SetTimer(2132, m_nLastScrSpd, NULL);
			}
		}
		else if(!(m_bUseAutoScroll || pmf->m_bAllScroll) && m_nScrollTimer)
		{
			if(m_nScrollTimer)
				KillTimer(m_nScrollTimer);
			m_nScrollTimer = 0;
		}
		
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnUpdateToolsAutoscrollUseautoscrolling(CCmdUI* pCmdUI) 
{
	
	// TODO: Add your command update UI handler code here
	try{
		pmf->ToolBarSetCheck(pCmdUI, m_bUseAutoScroll);
	}catch(...){}
}

void CMyIE9View::ChangeSpeed(int nSpeed)
{
	try{
		
		if(m_nScrollTimer)
		{
			KillTimer(m_nScrollTimer);
			m_nScrollTimer = 0;
		}
		
		//start to scroll
		if(!m_nScrollTimer && (m_bUseAutoScroll || pmf->m_bAllScroll) && nSpeed!=0)
		{
			if(m_nProgress==100 || m_nProgress<0 || (!m_bScrollAfterDL && m_nProgress>=0))
			{
				if(pmf->m_bMouseCtrlScrSpeed)
					m_nLastScrSpd = nSpeed;
				else
					m_nLastScrSpd = (UINT)((g_fSeed*100)/(m_nSrSpeed*m_nSrSpeed));
				m_nScrollTimer = SetTimer(2132, m_nLastScrSpd, NULL);
			}
		}
		
	}catch(...){ASSERT(false);}
}

void CMyIE9View::WBScroll()
{
	
	
	CMyIE9View* pvw = this;
	if(pvw->m_bToClose)
		return;
	
	LPDISPATCH pDisp = NULL;
	IHTMLDocument2 *pHTMLDoc = NULL;
	IHTMLWindow2 *pHTMLWnd = NULL;
	
	try{
		pDisp = pvw->GetHtmlDocument();
		if( pDisp )
		{
			if (SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
			{
				if(SUCCEEDED(pHTMLDoc->get_parentWindow( &pHTMLWnd )))
				{
					if (pmf->m_nAutoScrollPage)
					{
						CRect	rect;
						int	blockHeight;
						pvw->m_wndBrowser.GetWindowRect(&rect);
						blockHeight = (rect.bottom - rect.top) / pmf->m_nAutoScrollPage;
						m_nScrollSize++;
						if (m_nScrollSize>=blockHeight)
						{
							while(m_nScrollSize>0)
							{
								pHTMLWnd->scrollBy(0, 2);
								CGf_Win::Delay(2);
								m_nScrollSize -= 2;
							}
							m_nScrollSize=0;
						}
					}
					else
						pHTMLWnd->scrollBy(0, 1);
				}
			}
		}
	}catch(...){}
	
	try{
		if(pHTMLWnd!=NULL)	pHTMLWnd->Release();
		if(pHTMLDoc!=NULL)	pHTMLDoc->Release();
		if(pDisp!=NULL)		pDisp->Release();
	}catch(...){}
}

//scroll horizontal page
void CMyIE9View::WBScrollHPage(int bLeft)
{
	CMyIE9View* pvw = this;
	if(pvw->m_bToClose)
		return;
	
	LPDISPATCH pDisp = NULL;
	IHTMLDocument2 *pHTMLDoc = NULL;
	IHTMLWindow2 *pHTMLWnd = NULL;
	
	try{
		pDisp = pvw->GetHtmlDocument();
		if( pDisp )
		{
			if (SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
			{
				if(SUCCEEDED(pHTMLDoc->get_parentWindow( &pHTMLWnd )))
				{
					CRect	rect;
					pvw->m_wndBrowser.GetWindowRect(&rect);
					if (bLeft)
						pHTMLWnd->scrollBy(-(long)(rect.Width()*0.87),0);
					else
						pHTMLWnd->scrollBy((long)(rect.Width()*0.87),0);
				}
			}
		}
	}catch(...){}
	
	try{
		if(pHTMLWnd!=NULL)	pHTMLWnd->Release();
		if(pHTMLDoc!=NULL)	pHTMLDoc->Release();
		if(pDisp!=NULL)		pDisp->Release();
	}catch(...){}
}

void CMyIE9View::WBScrollBy(long xpos,long ypos)
{
	CMyIE9View* pvw = this;
	if(pvw->m_bToClose)
		return;
	
	LPDISPATCH pDisp = NULL;
	IHTMLDocument2 *pHTMLDoc = NULL;
	IHTMLWindow2 *pHTMLWnd = NULL;
	
	try{
		pDisp = pvw->GetHtmlDocument();
		if( pDisp )
		{
			if (SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
			{
				if(SUCCEEDED(pHTMLDoc->get_parentWindow( &pHTMLWnd )))
					pHTMLWnd->scrollBy(xpos,ypos);
			}
		}
	}catch(...){}
	
	try{
		if(pHTMLWnd!=NULL)	pHTMLWnd->Release();
		if(pHTMLDoc!=NULL)	pHTMLDoc->Release();
		if(pDisp!=NULL)		pDisp->Release();
	}catch(...){}
}

void CMyIE9View::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	try{
		if(nIDEvent == m_nScrollTimer && (m_bUseAutoScroll || pmf->m_bAllScroll))
		{
			if(m_nLastScrSpd<STOP_TIMER)
				WBScroll();
			//update scroll speed
			if(pmf->m_bMouseCtrlScrSpeed)
			{
				CPoint mousept;
				GetCursorPos(&mousept);
				ScreenToClient(&mousept);
				CRect rect;
				GetWindowRect(&rect);
				if(mousept.x<=rect.Width() && mousept.x>=rect.Width()-pmf->m_nScrollWidth)
				{
					if(mousept.y<=rect.Height()-pmf->m_nScrollArrowH && mousept.y>0)
					{
						int sp = 100 - mousept.y*100/(rect.Height()-pmf->m_nScrollArrowH)+1;
						if(abs(sp-m_nLastScrSpd)>5)
							ChangeSpeed(sp);
					}
					else
						ChangeSpeed(STOP_TIMER);
				}
				else
					ChangeSpeed(STOP_TIMER);
			}
			
		}
		else if(nIDEvent == m_nAutoRefreshTimer && m_bUseAutoRef)
		{
			if (!m_bIsActive || pmf->m_bAutoRefreshActive)
				PowerRefresh();
		}
		CFixedHtmlView::OnTimer(nIDEvent);
	}catch(...){}
}

void CMyIE9View::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	try{
		CFixedHtmlView::OnClose();
	}catch(...){}
}

void CMyIE9View::ChangeProperty()
{
	try{
		m_dwPropertyDefault = m_dwProperty;
		IOleControl* poc;
		LPDISPATCH pDisp = GetHtmlDocument();
		if(pDisp != NULL)
		{
			if(SUCCEEDED(pDisp->QueryInterface(IID_IOleControl, (void**)&poc)))
			{
				poc->OnAmbientPropertyChange(DISPID_AMBIENT_DLCONTROL);
				poc->Release();
			}
			pDisp->Release();
		}
	}catch(...){}
}

void CMyIE9View::OnToolsAutoref() 
{
	try{
		
		// TODO: Add your command handler code here
		m_bUseAutoRef = !m_bUseAutoRef;
		
		//start to scroll
		if(!m_nAutoRefreshTimer && m_bUseAutoRef && m_nRefreshInterval!=0)
		{
			m_nAutoRefreshTimer = SetTimer(3132+rand()/500, (UINT)(m_nRefreshInterval*1000), NULL);
		}
		else if(!m_bUseAutoRef && m_nAutoRefreshTimer)
		{
			if(m_nAutoRefreshTimer)
				KillTimer(m_nAutoRefreshTimer);
			m_nAutoRefreshTimer = 0;
		}	
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnUpdateToolsAutoref(CCmdUI* pCmdUI) 
{
	try{
		
		// TODO: Add your command update UI handler code here
		pCmdUI->SetCheck(m_bUseAutoRef);
		
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnToolsReffre() 
{
	try{
		
		// TODO: Add your command handler code here
		CSetRefIntervalDlg dlg;
		dlg.m_nRefInt = m_nRefreshInterval;
		if(dlg.DoModal() == IDOK)
		{
			m_nRefreshInterval = dlg.m_nRefInt;
			pmf->m_nDefRefreshInterval = m_nRefreshInterval;
			//change timer
			if(m_bUseAutoRef)
				ChangeRefresh();
			else
				OnToolsAutoref();//gf
		}
}catch(...){ASSERT(false);}

}

void CMyIE9View::ChangeRefresh()
{
	try{
		
		if(m_nAutoRefreshTimer)
		{		//stop the timer;
			KillTimer(m_nAutoRefreshTimer);
			m_nAutoRefreshTimer = 0;
		}
		//start to scroll
		if(!m_nAutoRefreshTimer && m_bUseAutoRef && m_nRefreshInterval!=0)
		{
			m_nAutoRefreshTimer = SetTimer(3132, (UINT)(m_nRefreshInterval*1000), NULL);
		}
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnNavigateComplete2(LPCTSTR lpszURL)
{
	try
	{
		CString strUrl;
		CChildFrame* tcf = ((CChildFrame*)GetParentFrame());
		GetLocationURL(strUrl);
		
		if(!m_bToClose && ((m_bIsPopUp && m_bIsNewPopup) || (tcf!=NULL && !tcf->IsWindowVisible())) && !strUrl.IsEmpty())
		{
			CString url2 = strUrl.Right(m_strPossibleURL.GetLength()+13);
			if((pmf->IsWindowVisible() && tcf!=NULL && !tcf->IsWindowVisible()) || url2 == "navcancl.htm#" + m_strPossibleURL /*for IE 5*/|| strUrl == "about:NavigationCanceled" /*for IE4*/ || strUrl==m_strPossibleURL)
			{
				if(tcf!=NULL) 
				{
					tcf->PostMessage(WM_CLOSE);
					m_bToClose = TRUE;
				}
				m_bIsPopUp = FALSE;
			}
		}
		//
		try
		{	
			//security flag
			if(!m_bToClose)
			{
				g_bVerbose = TRUE;
				if(strUrl.Left(5)=="https")
				{
					LPDISPATCH pDisp = NULL;
					IHTMLDocument2 *pHTMLDoc = NULL;
					BSTR p;
					
					try
					{
						pDisp = GetHtmlDocument();
						if( pDisp )
							pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc);
						else
							m_bIsSecure = FALSE;
						if(pHTMLDoc)
						{
							if(SUCCEEDED(pHTMLDoc->get_protocol( &p )))
							{
								CString s(p);
								if(s=="HyperText Transfer Protocol with Privacy" || s.Find("私人验证")>0 || s.Find("玂盞")>0)
									m_bIsSecure = TRUE;
								else
									m_bIsSecure = FALSE;
								SysFreeString(p);
							}
						}
						if(m_bIsActive && pmf!=NULL)
							pmf->m_bIsSecure = m_bIsSecure;
					}catch(...){}
					
					try
					{
						if(pHTMLDoc!=NULL)
							pHTMLDoc->Release();
						if(pDisp!=NULL)
							pDisp->Release();
					}catch(...){}
				}
			}
		}catch(...){}
	}catch(...){}
}


BOOL CMyIE9View::PreTranslateMessage(MSG* pMsg)
{
	ASSERT(pMsg != NULL);
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
	
	GF_TRY
	{
		if(pMsg == NULL || m_hWnd == NULL)
			return TRUE;
		//do key
		if( ( (pMsg->wParam==VK_RETURN) || (pMsg->wParam==VK_SPACE) )&& (pMsg->message == WM_KEYDOWN) )
		{
			LinkFired();
		}


		
		//gfpop
		if ( (WM_LBUTTONDOWN == pMsg->message) ) //&& g_bKillPop && g_bKillAutoWin
		{
			POINT ptLClick = pMsg->pt;

			MyScreenToClient(&ptLClick);

			if(IsPointLink(ptLClick) )
			{
				LinkFired();
				MYDEBUG("user clicked")
			}
		}		
		
		return CFixedHtmlView::PreTranslateMessage(pMsg);
	}
	GF_CATCH_TRUE
}


void CMyIE9View::OnFileSaveAs()
{
	try{
		
		// TODO: Add your command handler code here
		ExecWB(OLECMDID_SAVEAS, OLECMDEXECOPT_DODEFAULT,
			NULL, NULL);
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnFileSavehtml() 
{
	// TODO: Add your command handler code here
	ExecWB(OLECMDID_SAVEAS, OLECMDEXECOPT_DONTPROMPTUSER,
		NULL, NULL);
}

void CMyIE9View::OnFilePrintSetup() 
{
	try{
		
		// TODO: Add your command handler code here
		ExecCommand(OLECMDID_PAGESETUP);
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnFileProperty() 
{
	// TODO: Add your command handler code here
	ExecCommand(OLECMDID_PROPERTIES);
}

void CMyIE9View::ExecCommand(DWORD nCmdID)
{
	try{
		
		LPOLECOMMANDTARGET lpTarget = NULL;
		LPDISPATCH lpDisp = GetHtmlDocument();
		
		if (lpDisp != NULL)
		{
			// the control will handle all printing UI
			if (SUCCEEDED(lpDisp->QueryInterface(IID_IOleCommandTarget,
				(LPVOID*) &lpTarget)))
			{
				lpTarget->Exec(NULL, nCmdID, 0, NULL, NULL);
				lpTarget->Release();
			}
			lpDisp->Release();
		}	
		
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	try{
		
		if(m_bIsCapture)
		{
			int i =0;
			m_bIsCapture = FALSE;
			ReleaseCapture();
			//if have right button down action, then not pop menu
			if (pmf->m_nRButtonDown!=0)
				goto _PopRightMenu;
			//if mouse drag and not move then need pop menu
			if (pmf->m_bRMouseDrag)
				goto _PopRightMenu;
			
			g_cGestureString[0]= '\0';
			while(g_nGestureTail != g_nGestureHead)
			{
				g_cGestureString[i] = g_cGestureStack[g_nGestureHead];
				i++;
				g_cGestureString[i] = '\0';
				g_nGestureHead = (g_nGestureHead +1)%MG_LEN;		
			}
			
			if(i>0)
			{
				int command = pmf->m_GestureList.GetCmdFromName(g_cGestureString);
				if (command)
					MouseAction(command);
			}
			
_PopRightMenu:		
			if(i==0 && pmf->m_bPopRightMenu)
			{
				if (IsJumpImgMenu())
				{					
					pmf->DoWithImgUrl(3);
				}
				else
				{
					//popup right menu;
					POINT p2 = point;
					ClientToScreen(&p2);
					CWnd *pWnd = WindowFromPoint(p2);
					if(pWnd)
					{
						pWnd->ScreenToClient(&p2);
						pWnd->PostMessage(WM_RBUTTONUP, nFlags, MAKELONG(p2.x, p2.y));
					}
				}
				
			}
		}	
	}catch(...){}
	
	try{
		
		CFixedHtmlView::OnRButtonUp(nFlags, point);
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	try{
		//if right button down, then do nothing
		if(m_bIsCapture && pmf->m_nRButtonDown==0)
		{
			//if drag page then not mouse action
			if (pmf->m_bRMouseDrag)//drag page
			{
				if (m_ptPrev.x != point.x || m_ptPrev.y != point.y)
				{
					WBScrollBy(m_ptPrev.x - point.x, m_ptPrev.y - point.y);
					m_ptPrev = point;
					pmf->m_bPopRightMenu = FALSE;
					return;
				}
			}
			if (pmf->m_bRMouseGesture && nFlags == MK_RBUTTON )
			{
				char cDirection;
				if(MoveDirection(point, &cDirection))
				{
					PushMouseGesture(cDirection);
					m_ptRClick = point;
				}
			}
		}
	}catch(...){}
	
	try{
		
		CFixedHtmlView::OnMouseMove(nFlags, point);
	}catch(...){ASSERT(false);}
}

void CMyIE9View::PushMouseGesture(char cDirection)
{
	if(g_nGestureTail!=0 || g_nGestureHead !=0)
	{
		int pre = (g_nGestureTail -1 + MG_LEN)%MG_LEN;		
		if(g_cGestureStack[pre] == cDirection)
			return;
	}
	
	g_cGestureStack[g_nGestureTail] = cDirection;
	g_nGestureTail = (g_nGestureTail+1)%MG_LEN;
	if(g_nGestureHead == g_nGestureTail)
		g_nGestureHead = (g_nGestureHead + 1)%MG_LEN;
	
	//show gesture 
	if (pmf->m_bShowGesture)
	{
		int i=0;
		g_cGestureString[0]= '\0';
		int iMouseGS = g_nGestureHead;//not change g_nGestureHead
		while(g_nGestureTail != iMouseGS)
		{
			g_cGestureString[i] = g_cGestureStack[iMouseGS];
			i++;
			g_cGestureString[i] = '\0';
			iMouseGS = (iMouseGS +1)%MG_LEN;		
		}
		if(i>0)
		{
			CString strName;
			CString strMsg;
			LOADSTR(strMsg,IDS_GESTURE);
			strMsg += ": ";
			strMsg += g_cGestureString;
			if (theApp.m_bUseLngFile)
			{
				CString strUp,strDown,strLeft,strRight;
				//
				LOADSTR(strUp,IDS_UP);
				LOADSTR(strDown,IDS_DOWN);
				LOADSTR(strLeft,IDS_LEFT);
				LOADSTR(strRight,IDS_RIGHT);
				//
				strMsg.Replace("U",strUp);
				strMsg.Replace("D",strDown);
				strMsg.Replace("L",strLeft);
				strMsg.Replace("R",strRight);
			}
			strMsg += " => ";
			int command = pmf->m_GestureList.GetCmdFromName(g_cGestureString);
			_GetMenuItemString(strName,command);
			strMsg += strName;
			pmf->SetMessageText(strMsg);
		}
	}
}

BOOL CMyIE9View::MoveDirection(CPoint &point, char *Direction)
{
	int x = point.x - m_ptRClick.x;
	int y = point.y - m_ptRClick.y;
	int dist = x*x+y*y;
	if(dist>64)
	{
		if(x>abs(y) && x>0)
			*Direction = 'R';//right
		else if(abs(x)>abs(y) && x<0)
			*Direction = 'L';//left
		else if(y>abs(x) && y>0)
			*Direction = 'D';//down
		else if(abs(y)>abs(x) && y<0)
			*Direction = 'U';//up
		else
			return FALSE;
		return TRUE;
	}
	else
		return FALSE;
}

CString CMyIE9View::GetSortSaveFileName(CString &ext)
{
	CString filename;
	int l;
	
	GetLocationURL(filename);
	//
	filename.Replace('/', '\\');
	if(filename.Right(1) == "\\")
		filename = filename.Left(filename.GetLength()-1);
	
	l = filename.ReverseFind('\\');
	filename = filename.Mid(l+1);
	filename.Replace(':', '_');
	l = filename.Find('?');
	if(l>0)
		filename = filename.Left(l);
	l = filename.ReverseFind('.');
	if(l>0)
	{
		ext = filename.Right(filename.GetLength()-l);
		ext.MakeLower();
		filename = filename.Left(l);
	}
	if(ext != ".htm" && ext != ".html" && ext !=".shtml" && !pmf->IsImageType(ext))
	{
		ext = ".htm";
	}
	if (pmf->m_bNameByTitle)
	{
		CString strName;
		CChildFrame* tcf = ((CChildFrame*)GetParentFrame());
		GET_TAB_TITLE(tcf,strName);
		GET_URL_NAME(strName)
			REPLACE_ERR_PATH_CHAR(strName)
			int len = strName.GetLength();
		if (len)
		{
			if (len>FN_LEN)//cut to <len
			{
				strName = strName.Left(FN_LEN);
				if (strName.GetAt(FN_LEN-1) & 0x80)
				{
					int count=0;
					for (int i=0;i<FN_LEN;i++)
					{
						if (strName.GetAt(i) & 0x80)
							count++;
						else
							count = 0;
					}
					if (count%2)
						strName = strName.Left(FN_LEN-1);
				}
				strName += "_";
			}
			filename = strName;
		}
		// bag.jpg => bag
		if (filename.Right(ext.GetLength()) == ext)
		{
			filename = filename.Left(filename.GetLength()-ext.GetLength());
		}
	}
	
	return filename + ext;
}

void CMyIE9View::OnFileAutoSave() 
{
	// TODO: Add your command handler code here
	try{		
		IHTMLDocument2 * pDoc = NULL;
		IPersistFile *pPersistFile = NULL;
		CString filename, ext, htmlname,strNewFileName;
		int l;
		BSTR bfilename;
		char tmp[4]; 
		GetLocationURL(filename);
		htmlname.Empty();
		//
		strNewFileName = GetSortSaveFileName(ext);
		CString strSortSaveFolder;
		strSortSaveFolder.Empty();
		if (m_bSortSave)
		{
			m_bSortSave = FALSE;
			//not show the dlg
			if ( pmf->m_bShowSortSaveDlg && PRESS(VK_MENU) ||
				!pmf->m_bShowSortSaveDlg && !PRESS(VK_MENU))
			{
				if (pmf->m_strSortSaveFolder.GetLength())
				{
					strSortSaveFolder = pmf->m_strSortSaveFolder;
					goto _BeforeSave;
				}
			}
			//
			CSortSaveDlg dlg;
			CChildFrame* tcf = ((CChildFrame*)GetParentFrame());
			GET_TAB_TITLE(tcf,dlg.m_strTitle);
			//
			dlg.m_bType = pmf->m_bNameByTitle;
			dlg.m_strUrl = filename;
			dlg.m_strFileName = strNewFileName;
			dlg.m_strSortSaveFolder = pmf->m_strSortSaveFolder;
			if(dlg.DoModal()==IDOK)
			{
				strNewFileName = dlg.m_strFileName;
				REPLACE_ERR_PATH_CHAR(strNewFileName)
					pmf->m_strSortSaveFolder = strSortSaveFolder = dlg.m_strSortSaveFolder;
			}
			else
				return ;
		}
		else if (pmf->m_bSavePageByDate)
		{
			CString strTime;
			SYSTEMTIME time;
			::GetLocalTime(&time);
			strTime.Format("%04d_%02d_%02d",time.wYear,time.wMonth,time.wDay);
			strSortSaveFolder = strTime;
		}
_BeforeSave:
		//############################################################
		//select save path
		if (strSortSaveFolder.GetLength())
		{
			pmf->m_strAutoSavePath = theApp.m_strDefaultDirSort;
			pmf->m_strAutoSavePath += strSortSaveFolder;
			pmf->m_strAutoSavePath += "\\";
		}
		else
			pmf->m_strAutoSavePath = theApp.m_strDefaultDir;
		//
		if(pmf->m_bCateWithSite)
		{
			CString site;
			GetLocationURL(site);
			pmf->GetSiteName(site);
			//
			pmf->m_strAutoSavePath += site;
			pmf->m_strAutoSavePath += "\\";
		}
		//#############################################################
		//make sure the dir exist
		_CreateFullDir(pmf->m_strAutoSavePath);
		
		//#############################################################
		//before save
		if(	pmf->m_nAutoSave == 0 || 
			pmf->m_nAutoSave == 1 || 
			pmf->m_nAutoSave == 4 || 
			pmf->m_nAutoSave == 5 || 
			(pmf->m_nAutoSave == 3 && filename.Left(5)=="file:")
			)
		{
			keybd_event(VK_TAB, MapVirtualKey(VK_TAB, 0), 0, 0);
			if(pmf->m_nAutoSave==1)
			{
				keybd_event(VK_DOWN, MapVirtualKey(VK_DOWN, 0), 0, 0);
				keybd_event('A', MapVirtualKey('A', 0), 0, 0);
				keybd_event(VK_DOWN, MapVirtualKey(VK_DOWN, 0), 0, 0);
				keybd_event('A', MapVirtualKey('A', 0), 0, 0);
				keybd_event(VK_RETURN, MapVirtualKey(VK_RETURN, 0), 0, 0);
				keybd_event('A', MapVirtualKey('A', 0), 0, 0);
			}
			else if(pmf->m_nAutoSave==4)
			{
				keybd_event(VK_DOWN, MapVirtualKey(VK_DOWN, 0), 0, 0);
				keybd_event('A', MapVirtualKey('A', 0), 0, 0);
				keybd_event(VK_DOWN, MapVirtualKey(VK_DOWN, 0), 0, 0);
				keybd_event('A', MapVirtualKey('A', 0), 0, 0);
				keybd_event(VK_DOWN, MapVirtualKey(VK_DOWN, 0), 0, 0);
				keybd_event('A', MapVirtualKey('A', 0), 0, 0);
				keybd_event(VK_RETURN, MapVirtualKey(VK_RETURN, 0), 0, 0);
				keybd_event('A', MapVirtualKey('A', 0), 0, 0);
			}
			else if(pmf->m_nAutoSave==5)
			{
				keybd_event(VK_DOWN, MapVirtualKey(VK_DOWN, 0), 0, 0);
				keybd_event('A', MapVirtualKey('A', 0), 0, 0);
				keybd_event(VK_DOWN, MapVirtualKey(VK_DOWN, 0), 0, 0);
				keybd_event('A', MapVirtualKey('A', 0), 0, 0);
				keybd_event(VK_DOWN, MapVirtualKey(VK_DOWN, 0), 0, 0);
				keybd_event('A', MapVirtualKey('A', 0), 0, 0);
				keybd_event(VK_DOWN, MapVirtualKey(VK_DOWN, 0), 0, 0);
				keybd_event('A', MapVirtualKey('A', 0), 0, 0);
				keybd_event(VK_RETURN, MapVirtualKey(VK_RETURN, 0), 0, 0);
				keybd_event('A', MapVirtualKey('A', 0), 0, 0);
			}
			keybd_event(VK_RETURN, MapVirtualKey(VK_RETURN, 0), 0, 0);
			//set save directory
			HKEY hKey;
			if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Main"), &hKey) == ERROR_SUCCESS)
			{
				RegSetValueEx(hKey, _T("Save Directory"), 0, REG_SZ, 
					(LPBYTE)(LPCTSTR)pmf->m_strAutoSavePath,
					pmf->m_strAutoSavePath.GetLength()
					);
			}
			SendMessage(WM_COMMAND, ID_FILE_SAVEAS, 0);
		}
		else if(pmf->m_nAutoSave == 2)
		{
			filename = strNewFileName;
			l = filename.ReverseFind('.');
			if(l>0)
			{
				ext = filename.Right(filename.GetLength()-l);
				ext.MakeLower();
				filename = filename.Left(l);
			}
			filename = pmf->m_strAutoSavePath + filename;
			//
			l = 0;
			HFILE hf; OFSTRUCT of;
			htmlname = filename + ext;
			hf = OpenFile(htmlname, &of, OF_EXIST);
			while(hf!=HFILE_ERROR)
			{
				l++;
				htmlname = filename;
				htmlname += '[';
				itoa(l, tmp, 10);
				htmlname += tmp;
				htmlname += ']';
				htmlname += ext;
				hf = OpenFile(htmlname, &of, OF_EXIST);
			}
			if(!pmf->IsImageType(ext))
			{
				pDoc = (IHTMLDocument2*)GetHtmlDocument();
				bfilename = htmlname.AllocSysString();
				if (SUCCEEDED(pDoc->QueryInterface(IID_IPersistFile, (void**)&pPersistFile)))
					pPersistFile->Save(bfilename, FALSE);
				
				try{
					SysFreeString(bfilename);
					if(pPersistFile!=NULL)
						pPersistFile->Release();
					if(pDoc != NULL)
						pDoc->Release();
			}catch(...){}
			}
			else
			{
				//save image
				GetLocationURL(filename);
				DWORD dwEntrySize=0;
				LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry;
				if (!GetUrlCacheEntryInfo(filename,NULL,&dwEntrySize))
				{
					
					if (GetLastError()==ERROR_INSUFFICIENT_BUFFER)
					{
						
						lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) 
							new char[dwEntrySize];
						
						if (GetUrlCacheEntryInfo(filename,lpCacheEntry,&dwEntrySize))
						{
							CopyFile(lpCacheEntry->lpszLocalFileName, htmlname, FALSE);
						}
						delete lpCacheEntry;
						lpCacheEntry = NULL;
						dwEntrySize = 0;
					}
					else
					{
						//local image files
						pmf->DecodeEscap(filename);
						pmf->DecodeEscap(htmlname);
						if(filename.Left(8)=="file:///")
							filename = filename.Mid(8);
						CopyFile(filename, htmlname, FALSE);
					}
				}
			}
		}
		else if(pmf->m_nAutoSave == 3)
		{
			pmf->m_bIsAutoSave = TRUE;
			pmf->m_pCurView = this;
			pmf->SendMessage(WM_COMMAND, ID_FILE_QUICKSAVE,0);
			pmf->m_bIsAutoSave = FALSE;
		}
		
		l = pmf->FindTab((CChildFrame*)GetParentFrame());
		pmf->SetTabIcon(TI_SAVE, l, this);
		if(pmf->m_nTabStyle==2)
			pmf->PostMessage(WM_UPDATE_TAB);
		
		CString strMsg;
		LOADSTR(strMsg ,IDS_PAGE_SAVED);
		if (htmlname.GetLength()>0)
			strMsg+= htmlname;
		else
			strMsg+= pmf->m_strAutoSavePath;
		//add local url
		strMsg += " <= ";
		strMsg += m_lpszUrl;
		pmf->AddToCollector(strMsg,12);
	}catch(...){}	
}

void CMyIE9View::OnLButtonUp(UINT nFlags, CPoint point) 
{
	try
	{
		
		// TODO: Add your message handler code here and/or call default
		if(m_bIsCapture)
		{
			m_bIsCapture = FALSE;
			ReleaseCapture();
		}
	}catch(...){ASSERT(false);}
	
	try{
		
		CFixedHtmlView::OnLButtonUp(nFlags, point);
	}catch(...){ASSERT(false);}
}

int CMyIE9View::MouseAction(int kind) 
{
	if (kind)
	{
		g_bRTab = FALSE;
		//special
		if (kind==ID_OPEN_LINK)
			return OpenLinkUnderMouse(INW_NORMAL);
		else if (kind==ID_OPEN_LINK_ACTIVE)
			return OpenLinkUnderMouse(INW_ACTIVE);
		else if (kind==ID_OPEN_LINK_DEACTIVE)
			return OpenLinkUnderMouse(INW_INACTIVE);
		else
			pmf->PostMessage(WM_COMMAND, kind, 0);
	}
	return TRUE;
}

CString CMyIE9View::GetPointUrl(POINT pt)
{
	CString strUrl;
	strUrl.Empty();
	
	//gf LPDISPATCH pDisp; 
	//gf IHTMLDocument2* pHTMLDoc=NULL;
	CComPtr<IHTMLElement> pElem;
	CComPtr<IHTMLElement> pElemParent;
	
	try{
		//gf pDisp = GetHtmlDocument();
		//gf if(pDisp!=NULL)
		{
			//gf if(SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
			{
				//gf if(pHTMLDoc!=NULL)
				{
					//gf if(SUCCEEDED(pHTMLDoc->elementFromPoint(pt.x, pt.y, &pElem)))
					HRESULT hr = CGf_html::HitTest(m_pBrowserApp, pt, &(pElem.p));
					if(SUCCEEDED(hr))
					{
						BSTR bstrSrc;
						while(pElem)
						{
							pElem->get_tagName(&bstrSrc);
							strUrl = bstrSrc;
							SysFreeString(bstrSrc);
							if (strUrl!="A")
							{
								pElem->get_parentElement(&(pElemParent.p) );
								//gf pElem->Release();
								pElem = pElemParent;
							}
							else
							{
								pElem->toString(&bstrSrc);
								strUrl=bstrSrc;
								SysFreeString(bstrSrc);
								break;
							}
						}
					}
				}
			}
		}
	}catch(...){ASSERT(false);}
	
//	try{
//		RELEASE(pElem)
//			RELEASE(pHTMLDoc)
//			RELEASE(pDisp)
//		}catch(...){}
		
		return strUrl;
}

BOOL CMyIE9View::IsPointLink(POINT pt)
{
	BOOL bLink = TRUE;
		
	CComPtr<IHTMLElement> pElem;
	CComPtr<IHTMLElement> pElemParent;
	
	try{
		
		HRESULT hr = CGf_html::HitTest(m_pBrowserApp, pt, &(pElem.p) );
		
		//ASSERT(pElem != NULL);

		if (!pElem)
		{
			return FALSE;
		}

		if(SUCCEEDED(hr))
		{
			BSTR bstrSrc;
			while(pElem)
			{
				pElem->get_tagName(&bstrSrc);
				CString strUrl = bstrSrc;
				SysFreeString(bstrSrc);
				
				strUrl.MakeUpper();
				
				bLink = FALSE;
				
				CString strUrlContract = GF_STR_LINK_TAG_SEPERATE + strUrl + GF_STR_LINK_TAG_SEPERATE;
				
				if(CHelperStr::Contains( GF_STR_NOT_LINK_TAG, strUrlContract) )
				{
					break;
				}
				
				if ("INPUT" == strUrl) //text
				{
					CComBSTR bstrTypeKey = _T("type");
					
					VARIANT vAttribValue;
					VariantInit(&vAttribValue);
					
					HRESULT hrText = pElem->getAttribute(bstrTypeKey, 0, &vAttribValue);
					
					if (S_OK == hrText)
					{
						char buf[MAX_PATH];
						wcstombs(buf, vAttribValue.bstrVal, MAX_PATH);
						
						CString strType = buf;
						strType.MakeLower();
						
						if (_T("text") == strType)
						{
							break;
						}
					}
					
					VariantClear(&vAttribValue);
				}
				
				
				if (CHelperStr::Contains( GF_STR_LINK_TAG, strUrlContract) )
				{
					bLink = TRUE;
					break;
				}
				
				
				{
					pElem->get_parentElement(&(pElemParent.p) );
					
					pElem = pElemParent;
				}
				
				
			}
		}
		
	}catch(...){ASSERT(false);}
	
	return bLink;
}


int CMyIE9View::OpenLinkUnderMouse(int nActive)
{
	try{
		
		POINT pt;
		GetCursorPos(&pt);
		MyScreenToClient(&pt);
		CString strUrl = GetPointUrl(pt);
		//
		if (IS_URL(strUrl) || IS_SCRIPT_MAILTO(strUrl))
		{
			pmf->m_ptMouseDown = pt;
			pmf->ClickPoint(nActive);
			return TRUE;
		}
		return FALSE;
		
	}catch(...){ASSERT(false); return FALSE;}
}

void CMyIE9View::VScrollPageUp()
{
	this->PostMessage(WM_KEYDOWN,VK_PRIOR,1);
}

void CMyIE9View::VScrollPageDown()
{
	this->PostMessage(WM_KEYDOWN,VK_NEXT,1);
}

void CMyIE9View::OnUpdateToolsAutoscrollPage(CCmdUI* pCmdUI) 
{
	try{
		
		// TODO: Add your command update UI handler code here
		switch(pCmdUI->m_nID)
		{
		case ID_TOOLS_AUTOSCROLL_PAGE0:
			pCmdUI->SetCheck(pmf->m_nAutoScrollPage==0);
			break;
		case ID_TOOLS_AUTOSCROLL_PAGE1:
			pCmdUI->SetCheck(pmf->m_nAutoScrollPage==1);
			break;
		case ID_TOOLS_AUTOSCROLL_PAGE2:
			pCmdUI->SetCheck(pmf->m_nAutoScrollPage==2);
			break;
		case ID_TOOLS_AUTOSCROLL_PAGE4:
			pCmdUI->SetCheck(pmf->m_nAutoScrollPage==4);
			break;
		}
		
	}catch(...){ASSERT(false);}
}

void CMyIE9View::PowerRefresh()
{
	try{

		_bRefreshing = TRUE;

		if (!pmf->m_bFilterRefresh)
		{
			_bRefeshedForFilter = TRUE;
 		}
		
		CChildFrame* tcf = ((CChildFrame*)GetParentFrame());
		if (m_lpszUrl == "about:blank")
			return;
		int nTabID = pmf->FindTab(tcf);
		if (m_nOldIcon==-1 && pmf->m_nTabStyle==2)
			pmf->PostMessage(WM_UPDATE_TAB);
		pmf->SetTabIcon( TI_1, nTabID, this );


		if (IS_RES_URL(m_lpszUrl))
		{
			CString strNewUrl;// not use m_lpszUrl; user may modified the url
			pmf->m_wndAddress->GetEditCtrl()->GetWindowText(strNewUrl);
			DO_RES_URL(strNewUrl);
			CChildFrame* tcf = (CChildFrame*)pmf->MDIGetActive();
			if (tcf && tcf->m_pView == this)
				pmf->SetAddressText(strNewUrl);
			Navigate2(strNewUrl, 0, NULL);
			return;
		}
		//for http://www.ror.cn refresh
		m_bForbidAutoNavigate = 0;
		//gf Refresh2(REFRESH_NORMAL);//Refresh2(REFRESH_COMPLETELY);
		Refresh2(REFRESH_COMPLETELY);
		
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnViewPagesize50() 
{
	// TODO: Add your command handler code here
// 	pmf->m_bShowScriptResult = false;
//	pmf->RunScript("<script language=\"JavaScript\"> document.body.style.zoom=\"50%\"; </script>");
// 	pmf->m_bShowScriptResult = true;

	SetPageSize(50);
}

void CMyIE9View::OnViewPagesize100() 
{
	// TODO: Add your command handler code here
// 	pmf->m_bShowScriptResult = false;
//	pmf->RunScript("<script language=\"JavaScript\"> document.body.style.zoom=\"100%\"; </script>");
// 	pmf->m_bShowScriptResult = true;
	SetPageSize(100);
}

void CMyIE9View::OnViewPagesize200() 
{
	// TODO: Add your command handler code here
//	pmf->m_bShowScriptResult = false;
//	pmf->RunScript("<script language=\"JavaScript\"> document.body.style.zoom=\"150%\"; </script>");
// 	pmf->m_bShowScriptResult = true;

	SetPageSize(150);
}

void CMyIE9View::OnViewPagesizeAdd10() 
{
	// TODO: Add your command handler code here
//	pmf->m_bShowScriptResult = false;
//	pmf->RunScript("<script language=\"JavaScript\">	var i = parseInt(document.body.style.zoom);	if(isNaN(i))i=100; if(i<0)i=0; if(i==19)i=18; newZoom=i+10+'%'; document.body.style.zoom=newZoom; </script>");
// 	pmf->m_bShowScriptResult = true;

	
	_iPageSize += 10;

	SetPageSize(_iPageSize);
}

void CMyIE9View::OnViewPagesizeDec10() 
{
	// TODO: Add your command handler code here
//	pmf->m_bShowScriptResult = false;
//	pmf->RunScript("<script language=\"JavaScript\">	var i = parseInt(document.body.style.zoom);	if(isNaN(i))i=100; if(i<18)i=18; newZoom=i-10+'%'; document.body.style.zoom=newZoom; </script>");
// 	pmf->m_bShowScriptResult = true;

	_iPageSize -= 10;

	SetPageSize(_iPageSize);
}

void CMyIE9View::OnFileImportExport() 
{
	// TODO: Add your command handler code here
	try{
		CFindWnd FindIEWnd( m_wndBrowser.m_hWnd, "Shell DocObject View");
		::SendMessage( FindIEWnd.m_hWnd, WM_COMMAND, MAKEWPARAM(LOWORD(374), 0x0), 0 );
	}catch(...){}
}

void CMyIE9View::OnFileAutoSaveSort() 
{
	
	// TODO: Add your command handler code here
	m_bSortSave = TRUE;
	OnFileAutoSave();
}

//解决打开word后，点击一下外边的word，然后浏览器里面word的右键就出不来的问题。
void CMyIE9View::DispatchActiveMessage(BOOL bActive)
{
	try{
		
		if (m_pBrowserApp != NULL)
		{
			CComQIPtr<IOleInPlaceActiveObject> spInplace = m_pBrowserApp;
			
			if (spInplace)
			{
				spInplace->OnFrameWindowActivate(bActive);
			}
		}
		
	}catch(...){ASSERT(false);}
}

void CMyIE9View::OnActivateView(BOOL bActive, CView *pActiveView, CView *pDeactiveView)
{
	try
	{
		
		DispatchActiveMessage(bActive);
		
		CFixedHtmlView::OnActivateView(bActive, pActiveView, pDeactiveView);
		
	}catch(...){ASSERT(false);}
	
}

void CMyIE9View::SetPageSize(int iPageSize)
{
	if (iPageSize < 30)
	{
		return;
	}

	if ( CRegHelper::GetIEVer() > 6 )
	{
		CComVariant vaZoomFactor(iPageSize);
		m_pBrowserApp->ExecWB( (OLECMDID)63, OLECMDEXECOPT_DONTPROMPTUSER, &vaZoomFactor, NULL);
	}
	else
	{

		CString strScript = "";
		//strScript.Format("<script language=\"JavaScript\"> document.body.style.zoom=\"%i", iPageSize);
		strScript.Format("document.body.style.zoom=\"%i", iPageSize);
		strScript += "%\";"; // </script>";
			
		pmf->m_bShowScriptResult = false;
		pmf->RunScriptByFrame(strScript, "JavaScript", (CChildFrame*)GetParentFrame() );
		pmf->m_bShowScriptResult = true;

	}

	if (CGf_AppStatic::_instance._bKeepPageSize)
	{
		CGf_AppStatic::_instance._iPageSize = iPageSize;
	}
	
}

void CMyIE9View::SetPageSize()
{
	if (CGf_AppStatic::_instance._bKeepPageSize && (100 != CGf_AppStatic::_instance._iPageSize ) )
	{
		SetPageSize(CGf_AppStatic::_instance._iPageSize);
	}

}

void CMyIE9View::KillFly()
{

	//gffly not run when refreshed.					
	if ( _bRefreshing && !(pmf->m_bKillFlyWhenFresh) )//刷新时不killfly 且 正在刷新 
	{
		return;
	}
	
	//if click,then flow out
	CString strUrl = "";
	GetLocationURL(strUrl);	

// 	if ( (strUrl == _strUrlBefClick) )
// 	{
// 		return;						
// 	}

	if (GF_ALLOW_FLYING_FILTER && pmf->m_bUseFlyingFilter && (pmf->NeedFlyFilter( strUrl ) )  ) //gfc config
	{
		
		{ //avoid filter this: click a link, then jump a fly for input password and name. blog.sina
			
			CString strTmpOld=_strUrlBefClick;
			CString strTmpNow=strUrl;
			strTmpOld.Replace("#","");
			strTmpNow.Replace("#","");
			
			if( (strTmpOld == strTmpNow)&& !(_bRefreshing && pmf->m_bKillFlyWhenFresh) )
			{
				return;
			}
		}

		
		//_bClickedLinkForFly = FALSE;
		//_strOldKilledFlyUrl = strUrl;

		pmf->RunKillFlying((CChildFrame*)GetParentFrame());
	}


}

void CMyIE9View::DocShowedOK()
{
	//导致有时打不开吧	_bMaybeOpenNewWinByUser = FALSE;
	
	SetPageSize(); //gfsize
	
	KillFly();
	
	if (pmf->m_bKillBlank && pmf->m_bUseUrlFilter && GF_ALLOW_BLANK_FILTER)
		pmf->RunKillBlank((CChildFrame*)GetParentFrame());
	
	//need run plugin on blank page,for example close right page
	if (pmf->m_bAutoRunExternal)//gf if (pmf->m_bAutoRunExternal && !m_bExternalRunOK)
	{
		pmf->RunSelectedExternal((CChildFrame*)GetParentFrame(),TRUE);
		m_bExternalRunOK = TRUE;
	}
	
	
}


void CMyIE9View::LinkFired()
{

 	_bMaybeOpenNewWinByUser = TRUE;

	_bRefreshing = FALSE;

// 	_bClickedLinkForFly = TRUE;


	_strUrlBefClick=m_lpszUrl;
}

//gf onsize时往左上偏移了2，所以此时要去掉，否则取element有误

void CMyIE9View::MyScreenToClient(LPPOINT lpPoint) const
{

	lpPoint->x += BORDER;
	lpPoint->y += BORDER;

	ScreenToClient(lpPoint);
}


