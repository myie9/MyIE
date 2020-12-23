// FixedHtmlView.cpp : implementation file
//
#include "stdafx.h"
#include "MyIE9.h"
#include "FixedHtmlView.h"
#include "MainFrm.h"
#include <Mshtmhst.h>

#include "Gf_html.h"
#include "Gf_AppStatic.h"

//##############################################################
#ifdef AFX_HTML_SEG
#pragma code_seg(AFX_HTML_SEG)
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//##############################################################
#define IDC_BROWSER_CONTROL 100

/////////////////////////////////////////////////////////////////////////////
// CFixedHtmlView

IMPLEMENT_DYNCREATE(CFixedHtmlView, CFormView)

CFixedHtmlView::CFixedHtmlView() : CFormView((LPCTSTR) NULL)
{
	_dwRCTimeLast = 0;
	m_pBrowserApp = NULL;	
	m_pDisp = NULL;
	m_tLastClick.LowPart = 0;
	m_tLastRClick.LowPart = 0;
	m_bRClick = FALSE;
	m_pITravelLog = NULL;
	m_bIniTravelLog = FALSE;
	//
	m_strTarget.Empty();

	m_nMapMode = MM_TEXT; //gfsd solve debug problem
}

CFixedHtmlView::~CFixedHtmlView()
{
	try{
		if (m_pBrowserApp != NULL)
		{
			m_pBrowserApp->Release();
			m_pBrowserApp=NULL;
		}
	}catch(...){}
	
	try{
		if(m_pITravelLog!=NULL)
		{
			m_pITravelLog->Release();
			m_pITravelLog = NULL;
		}
	}catch(...){}
}

BEGIN_MESSAGE_MAP(CFixedHtmlView, CFormView)
//{{AFX_MSG_MAP(CFixedHtmlView)
ON_WM_PARENTNOTIFY()
ON_WM_CREATE()
ON_WM_SIZE()
ON_WM_DESTROY()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CFixedHtmlView, CFormView)
ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 102 /* StatusTextChange */, OnStatusTextChange, VTS_BSTR)
ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 108 /* ProgressChange */, OnProgressChange, VTS_I4 VTS_I4)
ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 105 /* CommandStateChange */, OnCommandStateChange, VTS_I4 VTS_BOOL)
ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 113 /* TitleChange */, OnTitleChange, VTS_BSTR)
ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 252 /* NavigateComplete2 */, NavigateComplete2, VTS_DISPATCH VTS_PVARIANT)
ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 250 /* BeforeNavigate2 */, BeforeNavigate2, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 251 /* NewWindow2 */, OnNewWindow2, VTS_PDISPATCH VTS_PBOOL)
ON_EVENT(CFixedHtmlView, IDC_BROWSER_CONTROL, 259 /* DocumentComplete */, DocumentComplete, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFixedHtmlView drawing

void CFixedHtmlView::OnDraw(CDC* pDC)
{
}

void CFixedHtmlView::OnDestroy()
{
	try{
		if (m_pBrowserApp != NULL)
		{
			m_pBrowserApp->Release();
			m_pBrowserApp = NULL;
		}
	}catch(...){}
}

/////////////////////////////////////////////////////////////////////////////
// CFixedHtmlView diagnostics

#ifdef _DEBUG
void CFixedHtmlView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFixedHtmlView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFixedHtmlView message handlers

long CFixedHtmlView::GetLeft() 
{
	long result=0;
	try{
		m_pBrowserApp->get_Left(&result);
	}catch(...){}
	return result;
}

long CFixedHtmlView::GetTop() 
{
	long result=0;
	try{
		m_pBrowserApp->get_Top(&result);
	}catch(...){}
	return result;
}

long CFixedHtmlView::GetHeight() 
{
	long result=0;
	try{
		m_pBrowserApp->get_Height(&result);
	}catch(...){}
	return result;
}

long CFixedHtmlView::GetWidth() 
{
	long result=0;
	try{
		m_pBrowserApp->get_Width(&result);
	}catch(...){}
	return result;
}

void CFixedHtmlView::SetTop(long nNewValue)
{ 
	try{
		m_pBrowserApp->put_Top(nNewValue);
	}catch(...){}
}

void CFixedHtmlView::SetLeft(long nNewValue)
{ 
	try{
		m_pBrowserApp->put_Left(nNewValue);
	}catch(...){}
}

void CFixedHtmlView::SetHeight(long nNewValue)
{ 
	try{
		m_pBrowserApp->put_Height(nNewValue);
	}catch(...){}
}

void CFixedHtmlView::SetWidth(long nNewValue)
{ 
	try{
		m_pBrowserApp->put_Width(nNewValue);
	}catch(...){}
}

void CFixedHtmlView::GetLocationName(CString& strLocName) 
{
	BSTR bstr=NULL;
	try{
		m_pBrowserApp->get_LocationName(&bstr);
	}catch(...){}
	strLocName = bstr;
}

void CFixedHtmlView::GetLocationURL(CString& strLocURL) 
{
	if (m_b01InUrl)
	{
		strLocURL = ((CMyIE9View*)this)->m_lpszUrl;
		return;
	}
	BSTR bstr=NULL;
	try{
		m_pBrowserApp->get_LocationURL(&bstr);
	}catch(...){}
	strLocURL = bstr;
}

READYSTATE CFixedHtmlView::GetReadyState() 
{
	READYSTATE result;
	try{
		m_pBrowserApp->get_ReadyState(&result);
	}catch(...){}
	return result;
}

LPDISPATCH CFixedHtmlView::GetApplication() 
{
	LPDISPATCH result=NULL;
	try{
		m_pBrowserApp->get_Application(&result);
	}catch(...){}
	return result;
}

LPDISPATCH CFixedHtmlView::GetHtmlDocument() 
{
	LPDISPATCH result=NULL;
	try{
		m_pBrowserApp->get_Document(&result);
	}catch(...){}
	return result;
}

OLECMDF CFixedHtmlView::QueryStatusWB(OLECMDID cmdID) 
{
	OLECMDF result;
	try{
		m_pBrowserApp->QueryStatusWB(cmdID, &result);
	}catch(...){}
	return result;
}

void CFixedHtmlView::ExecWB(OLECMDID cmdID, OLECMDEXECOPT cmdexecopt,
							VARIANT* pvaIn, VARIANT* pvaOut)
{
	try{
		m_pBrowserApp->ExecWB(cmdID, cmdexecopt, pvaIn, pvaOut);
	}catch(...){}
}

/////////////////////////////////////////////////////////////////////////////
// CFixedHtmlView operations

BOOL CFixedHtmlView::LoadFromResource(LPCTSTR lpszResource)
{
	HINSTANCE hInstance = AfxGetResourceHandle();
	ASSERT(hInstance != NULL);
	
	CString strResourceURL;
	BOOL bRetVal = TRUE;
	LPTSTR lpszModule = new TCHAR[_MAX_PATH];
	
	if (GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
	{
		strResourceURL.Format(_T("res://%s/%s"), lpszModule, lpszResource);
		Navigate(strResourceURL, 0, 0, 0);
	}
	else
		bRetVal = FALSE;
	
	delete [] lpszModule;
	return bRetVal;
}

BOOL CFixedHtmlView::LoadFromResource(UINT nRes)
{
	HINSTANCE hInstance = AfxGetResourceHandle();
	ASSERT(hInstance != NULL);
	
	CString strResourceURL;
	BOOL bRetVal = TRUE;
	LPTSTR lpszModule = new TCHAR[_MAX_PATH];
	
	if (GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
	{
		strResourceURL.Format(_T("res://%s/%d"), lpszModule, nRes);
		Navigate(strResourceURL, 0, 0, 0);
	}
	else
		bRetVal = FALSE;
	
	delete [] lpszModule;
	return bRetVal;
}

void CFixedHtmlView::Navigate(LPCTSTR lpszURL, DWORD dwFlags /* = 0 */,
							  LPCTSTR lpszTargetFrameName /* = NULL */ ,
							  LPCTSTR lpszHeaders /* = NULL */, LPVOID lpvPostData /* = NULL */,
							  DWORD dwPostDataLen /* = 0 */)
{
	try
	{
		
		CString m_strUrl(lpszURL);
		BSTR bstrURL = m_strUrl.AllocSysString();
		
		COleSafeArray vPostData;
		if (lpvPostData != NULL)
		{
			if (dwPostDataLen == 0)
				dwPostDataLen = lstrlen((LPCTSTR) lpvPostData);
			
			vPostData.CreateOneDim(VT_UI1, dwPostDataLen, lpvPostData);
		}
		
		m_pBrowserApp->Navigate(bstrURL,
			COleVariant((long) dwFlags, VT_I4),
			COleVariant(lpszTargetFrameName, VT_BSTR),
			vPostData,
			COleVariant(lpszHeaders, VT_BSTR));
		
	}catch(...){ASSERT(false);}
}


void CFixedHtmlView::Navigate2(LPCTSTR lpszURL, DWORD dwFlags /* = 0 */,
							   LPCTSTR lpszTargetFrameName /* = NULL */,
							   LPCTSTR lpszHeaders /* = NULL */,
							   LPVOID lpvPostData /* = NULL */, DWORD dwPostDataLen /* = 0 */)
{
	try
	{
		
		
		ASSERT(m_pBrowserApp != NULL);
		
		if ((CFrameWnd*)GetParentFrame() == (CFrameWnd*)pmf->MDIGetActive())
			pmf->SetAddressText(lpszURL);
		
		COleSafeArray vPostData;
		if (lpvPostData != NULL)
		{
			if (dwPostDataLen == 0)
				dwPostDataLen = lstrlen((LPCTSTR) lpvPostData);
			
			vPostData.CreateOneDim(VT_UI1, dwPostDataLen, lpvPostData);
		}
		
		COleVariant vURL(lpszURL, VT_BSTR);
		COleVariant vHeaders(lpszHeaders, VT_BSTR);
		COleVariant vTargetFrameName(lpszTargetFrameName, VT_BSTR);
		COleVariant vFlags((long) dwFlags, VT_I4);
		
		m_pBrowserApp->Navigate2(vURL,
			vFlags, vTargetFrameName, vPostData, vHeaders);
		//
		//		try{
		//	static BYTE parms[] =
		//		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
		//	InvokeHelper(0x1f4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		//		 vURL, vFlags, vTargetFrameName, vPostData, vHeaders);
		//
		//	}catch(...)
		//	{
		//	} 
		
	}catch(...){ASSERT(false);}
}

void CFixedHtmlView::GoHome()
{
	try{
		m_pBrowserApp->GoHome();
	}catch(...){}
}

void CFixedHtmlView::Stop()
{
	try{
		m_pBrowserApp->Stop();
	}catch(...){}
}

void CFixedHtmlView::GoBack(int dist)
{
	try{
		for(int i=0; i<dist; i++)
			m_pBrowserApp->GoBack();
		m_nBackSteps = dist;
	}catch(...){}
}

void CFixedHtmlView::GoForward(int dist)
{
	try{
		for(int i=0; i<dist; i++)
			m_pBrowserApp->GoForward();
		m_nBackSteps = -dist;
	}catch(...){}
}

void CFixedHtmlView::Refresh()
{
	try{
		m_pBrowserApp->Refresh();
		m_nProgress = 0;
	}catch(...){}
}

void CFixedHtmlView::Refresh2(int nLevel)
{
	try{
		m_pBrowserApp->Refresh2(COleVariant((long) nLevel, VT_I4));
		m_nProgress = 0;
	}catch(...){}
}

/////////////////////////////////////////////////////////////////////////////
// CFixedHtmlView event reflectors
void CFixedHtmlView::NavigateComplete2(LPDISPATCH  pDisp , VARIANT* URL)
{
	ASSERT(V_VT(URL) == VT_BSTR);
	
	try
	{
		m_bLClick = FALSE;
		m_bRClick = FALSE;
		
		USES_CONVERSION;
		
		m_strUrl = OLE2T(V_BSTR(URL));
		
		//successfully set history,
		//this can get the real url of this navigation
		//but not fit for history list, because the back and forward doesn't 
		//get the same url as navigation
		if(m_pITravelLog==NULL)
		{
			SetHistory(m_strUrl);
			m_strLastURL = m_strUrl;
		}
		
		if (!m_pDisp)
		{
			m_pDisp = pDisp;
		}
		//在http://download.pchome.net/输入回车搜索的问题.
		//如果已经变大了,post的消息,会造成rect重新变回去
		//4.7.1 Create 的时候就已经传了rect
		/*if(m_bForceResize)
		{
		CRect rect;
		GetWindowRect(&rect);
		PostMessage(WM_SIZE,  0, ((rect.Height())<<16)+rect.Width());
		m_bForceResize = FALSE;
	}*/
		OnNavigateComplete2(m_strUrl);
	}catch(...){ASSERT(false);}
}

void CFixedHtmlView::BeforeNavigate2(LPDISPATCH  pDisp , VARIANT* URL,
									 VARIANT* Flags, VARIANT* TargetFrameName,
									 VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	ASSERT(V_VT(URL) == VT_BSTR);
	ASSERT(V_VT(TargetFrameName) == VT_BSTR);
	ASSERT(V_VT(PostData) == (VT_VARIANT | VT_BYREF));
	ASSERT(V_VT(Headers) == VT_BSTR);
	ASSERT(Cancel != NULL);
	
	try
	{
		USES_CONVERSION;
		m_strUrl = V_BSTR(URL);
		if (m_strUrl.Find( 0x01 )>=0 && m_strUrl.Find( '@' )>=0)
		{
			//gf
//			if (pmf->m_bConfirmUnsafePage)
//			{				
//				CString strTip,strTip2;
//				LOADSTR(strTip ,IDS_TIP_01URL)
//					LOADSTR(strTip2 ,IDS_TIP_BROWSE);
//				strTip = m_strUrl + "\r\n\r\n" + strTip+strTip2;
//				if(MSGBOX(strTip, MB_YESNO|MB_ICONQUESTION)==IDNO)
//				{
//					CChildFrame* tcf = (CChildFrame*)GetParentFrame();
//					*Cancel=TRUE;
//					m_bToClose = TRUE;
//					tcf->PostMessage(WM_CLOSE);
//					return;
//				}
// 			}

			m_b01InUrl = TRUE;
			((CMyIE9View*)this)->m_lpszUrl = m_strUrl;
		}
		else
			m_b01InUrl = FALSE;
		
		//do with target frame information
		if (pmf->m_bUseTargetFrameName)
		{
			CString strTargetFrameName(V_BSTR(TargetFrameName));
			if (strTargetFrameName.GetLength() && strTargetFrameName.Find("No__Name")<0)//{"_No__Name:4" VT_BSTR}
			{
				CChildFrame* tcf = (CChildFrame*)GetParentFrame();
				CChildFrame* tcfTarget = pmf->GetChildFrameByTarget(tcf,strTargetFrameName);
				if( VALID_TCF(tcfTarget) && 
					//tcf->m_nActive means drag link
					!tcf->m_nActive && 
					//must do this, IsEmpty means the page have open in a new window
					((CMyIE9View*)this)->m_lpszUrl.IsEmpty()
					)
				{
					*Cancel=TRUE;
					m_bToClose = TRUE;
					tcf->PostMessage(WM_CLOSE);
					tcfTarget->m_pView->Navigate2(m_strUrl);
					return;
				}
				else
					m_strTarget = strTargetFrameName;
			}
		}
		//
		VARIANT* vtPostedData = V_VARIANTREF(PostData);
		BOOL array = FALSE;
		if (V_VT(vtPostedData) & VT_ARRAY)
		{
			// must be a vector of bytes
			ASSERT(vtPostedData->parray->cDims == 1 && vtPostedData->parray->cbElements == 1);
			vtPostedData->vt |= VT_UI1;
			COleSafeArray safe(vtPostedData);
			DWORD dwSize = safe.GetOneDimSize();
			array = dwSize>0?TRUE:FALSE;
		}
		// make real parameters out of the notification
		DWORD nFlags = V_I4(Flags);
		// notify the user's class
		OnBeforeNavigate2(m_strUrl, nFlags, "", array, "", Cancel);
		
	}catch(...){ASSERT(false);}
}

void CFixedHtmlView::DocumentComplete(LPDISPATCH pDisp, VARIANT* URL)
{
	try
	{
		//	UNUSED_ALWAYS(pDisp);
		ASSERT(V_VT(URL) == VT_BSTR);
		//is download complete
		if (m_pDisp && m_pDisp == pDisp)
		{
			// if the LPDISPATCH are same, that means
			// it is the final DocumentComplete. Reset glpDisp
			TRACE("Document is done all downloading");
			m_pDisp = NULL;

			OnDocumentCompleteAll(m_strUrl); //gf
		}
		m_strUrl = V_BSTR(URL);
		OnDocumentComplete(m_strUrl);
	}catch(...){}
}

/////////////////////////////////////////////////////////////////////////////
// CFixedHtmlView Events

void CFixedHtmlView::OnProgressChange(long lProgress, long lProgressMax)
{
	// user will override to handle this notification
	UNUSED_ALWAYS(lProgress);
	UNUSED_ALWAYS(lProgressMax);
}

void CFixedHtmlView::OnCommandStateChange(long lCommand, BOOL bEnable)
{
	// user will override to handle this notification
	UNUSED_ALWAYS(lCommand);
	UNUSED_ALWAYS(bEnable);
}

void CFixedHtmlView::OnTitleChange(LPCTSTR lpszText)
{
	// user will override to handle this notification
	UNUSED_ALWAYS(lpszText);
}

void CFixedHtmlView::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* bCancel)
{
	// default to continuing
	*bCancel = FALSE;
	// user will override to handle this notification
	UNUSED_ALWAYS(ppDisp);
}

void CFixedHtmlView::OnDocumentComplete(LPCTSTR lpszURL)
{
	// user will override to handle this notification
	UNUSED_ALWAYS(lpszURL);
}


void CFixedHtmlView::OnDocumentCompleteAll(LPCTSTR lpszURL)
{
	// user will override to handle this notification
	UNUSED_ALWAYS(lpszURL);
}

void CFixedHtmlView::OnNavigateComplete2(LPCTSTR lpszURL)
{
	// user will override to handle this notification
	UNUSED_ALWAYS(lpszURL);
}

void CFixedHtmlView::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags,
									   LPCTSTR lpszTargetFrameName, BOOL baPostData,
									   LPCTSTR lpszHeaders, BOOL* bCancel)
{
	// default to continuing
	*bCancel = FALSE;
	
	// user will override to handle this notification
	UNUSED_ALWAYS(lpszURL);
	UNUSED_ALWAYS(nFlags);
	UNUSED_ALWAYS(lpszTargetFrameName);
	UNUSED_ALWAYS(baPostData);
	UNUSED_ALWAYS(lpszHeaders);
}

void CFixedHtmlView::OnStatusTextChange(LPCTSTR pszText)
{
}

BOOL CFixedHtmlView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	try{
		
		if (!CView::Create(lpszClassName, lpszWindowName,dwStyle, rect, pParentWnd,  nID, pContext))
			return FALSE;
		
		RECT rectClient;
		if(g_bMax==1)
		{
			::GetClientRect(pmf->m_hWndMDIClient, &rectClient);
			rectClient.top -= 2;
			rectClient.left -= 2;
			rectClient.bottom += 2;
			rectClient.right += 2;
		}
		else
		{
			rectClient.top = rect.top - 2;
			rectClient.left = rect.left - 2;
			rectClient.bottom = rect.bottom + 2;
			rectClient.right = rect.right + 2;
		}
		
		// create the control window
		// AFX_IDW_PANE_FIRST is a safe but arbitrary ID
		if (!m_wndBrowser.CreateControl(CLSID_WebBrowser, lpszWindowName,
			WS_VISIBLE | WS_CHILD , rectClient, this, IDC_BROWSER_CONTROL))
		{
			DestroyWindow();
			return FALSE;
		}
		
		LPUNKNOWN lpUnk = m_wndBrowser.GetControlUnknown();
		HRESULT hr = lpUnk->QueryInterface(IID_IWebBrowser2, (void**) &m_pBrowserApp);
		if (!SUCCEEDED(hr))
		{
			m_pBrowserApp = NULL;
			m_wndBrowser.DestroyWindow();
			DestroyWindow();
			return FALSE;
		}
		
	}catch(...){ASSERT(false); return FALSE;}
	
	return TRUE;
}

BOOL CFixedHtmlView::PreTranslateMessage(MSG* pMsg)
{
	ASSERT(pMsg != NULL);
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
	
	GF_TRY
	{
		if(pMsg == NULL || m_hWnd == NULL)
			return TRUE;
		//do key
		if (pMsg->wParam==VK_RETURN)
		{
			//initialization value must be WM_KEYUP
			static int nCharState=WM_KEYUP;
			//when keyup, ctrl key maybe have release, so remember when keydown
			static BOOL bPressCtrl=0;
			if(pMsg->message == WM_KEYDOWN)
			{
				nCharState = WM_KEYDOWN;
				bPressCtrl = PRESS_CTRL;
				//
				QueryPerformanceCounter(&m_tLastClick);
				m_bRClick = FALSE;
				m_bLClick = TRUE;
			}
			else if(pMsg->message == WM_CHAR)
				nCharState = WM_CHAR;
			else if(pMsg->message == WM_KEYUP)
			{
				//the right order is down,char,up, but sometime maybe no char, add one
				if (nCharState==WM_KEYDOWN && !bPressCtrl)
					pMsg->message = WM_CHAR;
				//
				nCharState = WM_KEYUP;
			}
		}

		if (pMsg->message == WM_KEYUP)
		{
			if (pMsg->wParam==VK_ESCAPE)
			{
				pmf->PostMessage(WM_COMMAND, ID_VIEW_STOP, 0);				
			}
		}
		else if (pMsg->message == WM_KEYDOWN)
		{
			switch(pMsg->wParam)
			{
			case VK_LEFT:
				if (PRESS_CTRL && GetFocusType()==0)
				{
					pmf->PostMessage(WM_COMMAND, ID_TAB_PRE, 0);
					return TRUE;
				}
				break;
				
			case VK_RIGHT:
				if (PRESS_CTRL && GetFocusType()==0)
				{
					pmf->PostMessage(WM_COMMAND, ID_TAB_NEXT, 0);
					return TRUE;
				}
				break;
			}
		}
		//do mouse
		else if (pMsg->message == WM_RBUTTONUP)
		{
			if (pmf->m_nLeftRight && LOWORD(pMsg->wParam)==MK_LBUTTON)
			{
				m_tLastRClick.LowPart = 0;//??
				((CMyIE9View*)this)->MouseAction(pmf->m_nLeftRight);
				return TRUE;
			}

			if (pmf->m_nRDbClick)
			{				
				DWORD dwGap=pMsg->time - _dwRCTimeLast;

				UINT sysDbGap = GetDoubleClickTime();

				if (sysDbGap > 250)
				{
					sysDbGap=250;
				}

				DWORD dwOldRCTimeLast = _dwRCTimeLast;

				_dwRCTimeLast = pMsg->time;

				if ( (dwGap<sysDbGap) && (dwGap>0) && (dwOldRCTimeLast>0) )
				{
					((CMyIE9View*)this)->MouseAction(pmf->m_nRDbClick);
					return TRUE;	
				}

			}
			
			if (pmf->m_bNotShowRightMenu)
			{
				pmf->m_bNotShowRightMenu = FALSE;
				return TRUE;
			}

		}
		else if ( pMsg->message==WM_RBUTTONDOWN) //gf
		{
			if (IsJumpImgMenu() )
			{				
				return TRUE;
			}
			else if (CGf_html::IsPointImage(GetWebBrowser(),  GetPointInView(pmf->m_ptMouseDown) ) == CGf_html::GF_FLASH_TYPE )
			{
				//gf 便宜flash右键菜单， 以便出现右双击事件
				pMsg->lParam=MAKELONG(LOWORD(pMsg->lParam)+2, HIWORD(pMsg->lParam));
			}
 		}
		else if (pMsg->message == WM_LBUTTONUP)
		{
			//长时间按下链接后松开,新开的链接会被过滤
			QueryPerformanceCounter(&m_tLastClick);
			m_bLClick = TRUE;
			((CMyIE9View*)this)->m_strOpenNewUrl.Empty();
			if (pmf->m_nRightLeft && LOWORD(pMsg->wParam)==MK_RBUTTON)
			{
				m_tLastClick.LowPart = 0;//??
				((CMyIE9View*)this)->MouseAction(pmf->m_nRightLeft);
				if (!pmf->m_bNotShowRightMenu)
				{
					((CMyIE9View*)this)->m_bIsCapture = FALSE;
					ReleaseCapture();
					pmf->m_bNotShowRightMenu = TRUE;
				}
				return TRUE;
			}

			if (PRESS_CTRL)
			{
				if (((CMyIE9View*)this)->MouseAction(ID_OPEN_LINK_DEACTIVE) )
				{
					QueryPerformanceCounter(&m_tLastClick);
					m_bLClick = TRUE;

					return TRUE;
				}
			}

		}
		else if(pMsg->message==WM_MOUSEWHEEL)
		{
			if (PRESS_CTRL)
			{
				short zDelta = HIWORD(pMsg->wParam);
				if ( zDelta > 0)
					((CMyIE9View*)this)->MouseAction(ID_VIEW_PAGESIZE_ADD10);
				else
					((CMyIE9View*)this)->MouseAction(ID_VIEW_PAGESIZE_DEC10);
				
				return TRUE;
			}

			if (pmf->m_bWheelTab && LOWORD(pMsg->wParam)==MK_RBUTTON)
			{
				if ( (pMsg->time - pmf->m_dwLastTimeWheel) >300 || 0 == pmf->m_dwLastTimeWheel )
				{
					pmf->m_dwLastTimeWheel = pMsg->time;
						
					short zDelta = HIWORD(pMsg->wParam);
					if ( zDelta > 0)
						((CMyIE9View*)this)->MouseAction(ID_TAB_PRE);
					else
						((CMyIE9View*)this)->MouseAction(ID_TAB_NEXT);
					//
					if (!pmf->m_bNotShowRightMenu)
					{
						((CMyIE9View*)this)->m_bIsCapture = FALSE;
						ReleaseCapture();
						pmf->m_bNotShowRightMenu = TRUE;
					}

					return TRUE;
				}
				
			}
		}
		//这里不要用 up 消息,因为down时光标就变了
		else if(pMsg->message == WM_MBUTTONDOWN && pmf->m_nMButtonDown)//??
		{
			if ( ((CMyIE9View*)this)->MouseAction(pmf->m_nMButtonDown))
			{
				if (pmf->m_nMButtonDown>=ID_OPEN_LINK && pmf->m_nMButtonDown<=ID_OPEN_LINK_DEACTIVE)
				{
					//因为模拟的是 shift+click 
					QueryPerformanceCounter(&m_tLastClick);
					m_bLClick = TRUE;
				}
				return TRUE;
			}
		}
		else if(pMsg->message == WM_LBUTTONDBLCLK && pmf->m_nLDbClick)
		{
			((CMyIE9View*)this)->MouseAction(pmf->m_nLDbClick);
			return TRUE;
		}
	}GF_CATCH_TRUE
	
	GF_TRY
	{
		if (CFormView::PreTranslateMessage(pMsg))
			return TRUE;
		
	}GF_CATCH_TRUE
	
	return FALSE;
}


BOOL CFixedHtmlView::PreCreateWindow(CREATESTRUCT& cs)
{
	try
	{
		
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs
		
		cs.lpszClass = AfxRegisterWndClass(
			CS_DBLCLKS,                       // if you need double-clicks
			NULL,                             // no cursor (use default)
			NULL,                             // no background brush
			NULL); // app icon
		ASSERT(cs.lpszClass);
		
		if( !CFormView::PreCreateWindow(cs) )
			return FALSE;
		
	}catch(...){ASSERT(false);}
	
	return TRUE;
}

void CFixedHtmlView::OnParentNotify(UINT message, LPARAM lParam) 
{
	
	
	// TODO: Add your message handler code here
	try{
		CFormView::OnParentNotify(message, lParam);
		
		if(message == WM_DESTROY)
		{
			CFrameWnd* pFrame = GetParentFrame();
			if (pFrame!=NULL && pFrame!=pmf)
			{
				m_bToClose = TRUE;
				pFrame->PostMessage(WM_CLOSE);
			}
		}
	}catch(...){}
}

int CFixedHtmlView::SetHistory(LPCSTR strCurUrl)
{
	if(!m_bIniTravelLog)
	{
		//not initialize at startup
		if(strcmp(strCurUrl, "about:blank")==0)
			return 0;
		
		//load ITravelLogStg for IE 5.5
		m_pITravelLog = NULL;
		IServiceProvider* pISP = NULL;
		LPDISPATCH pDisp = GetApplication();
		if(pDisp!=NULL)
		{
			HRESULT hr = pDisp->QueryInterface(IID_IServiceProvider, (void**)&pISP);
			if(SUCCEEDED(hr) && pISP!=NULL) 
			{
				hr = pISP->QueryService(SID_STravelLogCursor , IID_ITravelLogStg, (void**)&m_pITravelLog);
				if(FAILED(hr))
					m_pITravelLog=NULL;
				pISP->Release();
			}
			pDisp->Release();
		}
		m_bIniTravelLog = TRUE;
		if(m_pITravelLog!=NULL)
			return 0;
	}
	
	short p = m_nHistoryLength;
	int seth = 0;
	LPDISPATCH pDisp = NULL;
	IHTMLDocument2 *pHTMLDoc = NULL;
	IHTMLWindow2 *pHTMLWnd = NULL;
	IOmHistory  *pHistory = NULL;
	
	try{
		//get history length
		pDisp = GetHtmlDocument();
		if( pDisp )
		{
			if(SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
			{
				if(SUCCEEDED(pHTMLDoc->get_parentWindow( &pHTMLWnd )))
				{
					if(SUCCEEDED(pHTMLWnd->get_history( &pHistory )))
					{
						if(pHistory->get_length(&p)==S_OK)
						{
							if(p>0 && p!=m_nHistoryLength)
							{
								if(m_strLastTitle.IsEmpty())
									m_strLastTitle = m_strLastURL;
								if(p>m_nHistoryLength)
								{
									m_astrHistory.SetAtGrow(p-1, m_strLastTitle);
									m_astrHistoryURL.SetAtGrow(p-1, m_strLastURL);
								}
								else
								{
									m_astrHistory.SetAt(p-1, m_strLastTitle);
									m_astrHistory.SetSize(p);
									if(m_nHistoryLength - p >MAX_HISTORY)
										m_astrHistory.FreeExtra();
									
									m_astrHistoryURL.SetAt(p-1, m_strLastURL);
									m_astrHistoryURL.SetSize(p);
									if(m_nHistoryLength - p >MAX_HISTORY)
										m_astrHistoryURL.FreeExtra();
								}
								m_nCurHistory = p;
								seth = 1;
							}
							else if(p==0)
								m_nCurHistory = 0;
							//adjust cur history
							m_nCurHistory -= m_nBackSteps;
							if(!m_bBack)
								m_nCurHistory = 0;
							else if(!m_bForward && seth && m_nBackSteps>=0)
								m_nCurHistory = p;
							else if(m_nCurHistory>=0 && m_nCurHistory<p && strCurUrl == m_astrHistoryURL.GetAt(m_nCurHistory))
								m_nCurHistory = m_nCurHistory;
							else if(m_nCurHistory-1>=0 && strCurUrl == m_astrHistoryURL.GetAt(m_nCurHistory-1))
								m_nCurHistory --;
							else if(m_nCurHistory-2>=0 && strCurUrl  == m_astrHistoryURL.GetAt(m_nCurHistory-2))
								m_nCurHistory -=2;
							else if(m_nCurHistory+1<p && strCurUrl == m_astrHistoryURL.GetAt(m_nCurHistory+1))
								m_nCurHistory ++;
							else if(m_nCurHistory+2<p && strCurUrl  == m_astrHistoryURL.GetAt(m_nCurHistory+2))
								m_nCurHistory +=2;
							
							if(m_bForward && m_nCurHistory>=p-1)
								m_nCurHistory = p-2;
							if(m_bBack && m_nCurHistory==0)
								m_nCurHistory =1;
							
							m_nHistoryLength = p;
						}
					}
				}
			}
		}
	}catch(...){}
	
	try{
		if(pHistory != NULL)
			pHistory->Release();
		if(pHTMLWnd != NULL)
			pHTMLWnd->Release();
		if(pHTMLDoc != NULL)
			pHTMLDoc->Release();
		if(pDisp != NULL)
			pDisp->Release();
	}catch(...){}
	
	m_nBackSteps = 0;
	return seth;
}

/////////////////////////////////////////////////////////////////////////////
// Pre-startup code

/*
#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
*/

//return value means 0: not edit 1:single line 2:mult-line
int CFixedHtmlView::GetFocusType()
{
	int nFocusType=0;
	LPDISPATCH pDisp = NULL;
    IHTMLDocument2 *pHTMLDoc=NULL;
	IHTMLElement *focus=NULL;
	IHTMLTextElement *text=NULL;
	IHTMLTextAreaElement *textarea=NULL;
	
	try
	{
		pDisp = GetHtmlDocument();
		if( pDisp )
		{
			if(SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
			{
				if (SUCCEEDED(pHTMLDoc->get_activeElement(&focus)))
				{
					if (SUCCEEDED(focus->QueryInterface(IID_IHTMLInputTextElement,(void**)&text)))
						nFocusType = 1;
					else if (SUCCEEDED(focus->QueryInterface(IID_IHTMLTextAreaElement,(void**)&textarea)))
						nFocusType = 2;
				}
			}
		}
	}catch(...){}
	//
	try{
		RELEASE(text)
			RELEASE(textarea)
			RELEASE(focus)
			RELEASE(pHTMLDoc)
			RELEASE(pDisp)
		}catch(...){}
		//
		return nFocusType;
}

BOOL CFixedHtmlView::GetBusy()
{
	VARIANT_BOOL result;
	try{
		m_pBrowserApp->get_Busy(&result);
	}catch(...){}
	return result;
}

HRESULT CFixedHtmlView::GetHitElement(const POINT &pt, IHTMLElement **ppElement)
{
	return CGf_html::HitTest(m_pBrowserApp, pt, ppElement);
}

IWebBrowser2* CFixedHtmlView::GetWebBrowser()
{
	return m_pBrowserApp;
}

POINT CFixedHtmlView::GetPointInView(const POINT& ptIn)
{

	POINT pt;
	pt.x = ptIn.x + BORDER;	
	pt.y = ptIn.y + BORDER;

	return pt;
}

BOOL CFixedHtmlView::IsJumpImgMenu()
{

	if ( CGf_html::IsPointImage(GetWebBrowser(),  GetPointInView(pmf->m_ptMouseDown)  ) )
	{
		//正常时按下ctl
		if (CGf_AppStatic::_instance._bAlwaysIEMenuRightClickImg && PRESS_CTRL)
		{
			return TRUE;
		}
	
		
		if (!CGf_AppStatic::_instance._bAlwaysIEMenuRightClickImg && (!PRESS_CTRL ) )
		{	
			//启用特殊图片菜单，且没有按下ctl
			return TRUE;
		}
	}

	return FALSE;
}
