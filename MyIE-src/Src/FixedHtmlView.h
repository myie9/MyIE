#if !defined(AFX_FIXEDHTMLVIEW_H__8BCE18F1_197D_11D4_9BCF_0000E85300AE__INCLUDED_)
#define AFX_FIXEDHTMLVIEW_H__8BCE18F1_197D_11D4_9BCF_0000E85300AE__INCLUDED_

#ifndef __AFXDISP_H__
	#include <afxdisp.h>
#endif

#ifndef __exdisp_h__
	#include <exdisp.h>
#endif

#include <mshtml.h>

#ifdef _AFX_MINREBUILD
#pragma component(minrebuild, off)
#endif
#ifndef _AFX_FULLTYPEINFO
#pragma component(mintypeinfo, on)
#endif

#include <tlogstg.h>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _AFXDLL
#undef AFX_DATA
#define AFX_DATA AFX_OLE_DATA
#endif

/////////////////////////////////////////////////////////////////////////////
// CFixedHtmlView view

class CFixedHtmlView : public CFormView
{
protected:
	CFixedHtmlView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFixedHtmlView)
	DECLARE_EVENTSINK_MAP()

// Attributes
public:
	POINT GetPointInView(const POINT& ptIn);

	HRESULT GetHitElement(const POINT &pt, IHTMLElement **ppElement);
	BOOL GetBusy();
	
	void SetLeft(long nNewValue);
	void SetTop(long nNewValue);
	void SetHeight(long nNewValue);
	void SetWidth(long nNewValue);
	void GetLocationName(CString& strLocName);
	void GetLocationURL(CString& strLocURL);

	int  GetFocusType();

	long GetLeft();
	long GetTop();
	long GetHeight();
	long GetWidth();
	
	READYSTATE GetReadyState();
	LPDISPATCH GetApplication();
	LPDISPATCH GetHtmlDocument();
	OLECMDF QueryStatusWB(OLECMDID cmdID) ;

// Operations
public:
	IWebBrowser2* GetWebBrowser();
	BOOL m_b01InUrl;//01 in page
	BOOL m_bIniTravelLog;
	BOOL m_bToClose;
	BOOL m_bRClick, m_bLClick;
	BOOL m_bForward;
	BOOL m_bBack;

	int m_nHistoryLength;
	int m_nCurHistory;
	int m_nProgress;
	int m_nBackSteps;

	CString m_strTarget;
	CString m_strLastTitle, m_strTempLastTitle, m_strLastURL;

	CStringArray m_astrHistory, m_astrHistoryURL;

	CWnd m_wndBrowser;//CWebBrowser2 m_wndBrowser;

	ITravelLogStg * m_pITravelLog;
	LARGE_INTEGER m_tLastClick, m_tLastRClick;
	
	//###################################
	BOOL LoadFromResource(LPCTSTR lpszResource);
	BOOL LoadFromResource(UINT nRes);

	int SetHistory(LPCSTR strCurUrl);
	
	void GoBack(int dist=1);
	void GoForward(int dist=1);
	void GoHome();
	void Navigate(LPCTSTR lpszURL, DWORD dwFlags = 0,
		LPCTSTR lpszTargetFrameName = NULL,
		LPCTSTR lpszHeaders = NULL,
		LPVOID lpvPostData = NULL,
		DWORD dwPostDataLen = 0
		);
	void Navigate2(LPCTSTR lpszURL, DWORD dwFlags = 0,
		LPCTSTR lpszTargetFrameName = NULL,
		LPCTSTR lpszHeaders = NULL,
		LPVOID lpvPostData = NULL,
		DWORD dwPostDataLen = 0
		);
	void Refresh();
	void Refresh2(int nLevel);
	void Stop();
	void ExecWB(OLECMDID cmdID, OLECMDEXECOPT cmdexecopt, VARIANT* pvaIn,
		VARIANT* pvaOut);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFixedHtmlView)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	LPDISPATCH m_pDisp;
	virtual ~CFixedHtmlView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CString m_strUrl;
	COleSafeArray vPostData;
	IWebBrowser2* m_pBrowserApp;

	// Events
	virtual void OnNavigateComplete2(LPCTSTR strURL);
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags,
		LPCTSTR lpszTargetFrameName, BOOL baPostedData,
		LPCTSTR lpszHeaders, BOOL* pbCancel);
	virtual void OnStatusTextChange(LPCTSTR lpszText);
	virtual void OnProgressChange(long nProgress, long nProgressMax);
	virtual void OnCommandStateChange(long nCommand, BOOL bEnable);
	virtual void OnTitleChange(LPCTSTR lpszText);
	virtual void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel);
	virtual void OnDocumentComplete(LPCTSTR lpszURL);
	virtual void OnDocumentCompleteAll(LPCTSTR lpszURL); //gf 每个及所有frame打开完时. 

// Event reflectors (not normally overridden)
protected:
	virtual void NavigateComplete2(LPDISPATCH pDisp, VARIANT* URL);
	virtual void BeforeNavigate2(LPDISPATCH pDisp, VARIANT* URL,
		VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData,
		VARIANT* Headers,   BOOL* Cancel);
	virtual void DocumentComplete(LPDISPATCH pDisp, VARIANT* URL);

	// Generated message map functions
protected:
	BOOL IsJumpImgMenu();
	//{{AFX_MSG(CFixedHtmlView)
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//BOOL IsDownloadable(CString& URL);

	DWORD _dwRCTimeLast;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIXEDHTMLVIEW_H__8BCE18F1_197D_11D4_9BCF_0000E85300AE__INCLUDED_)
