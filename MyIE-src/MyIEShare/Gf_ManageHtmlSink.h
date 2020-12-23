// Gf_ManageHtmlSink.h: interface for the CGf_ManageHtmlSink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_MANAGEHTMLSINK_H__44637117_CBDD_4905_AF21_787A75430FB2__INCLUDED_)
#define AFX_GF_MANAGEHTMLSINK_H__44637117_CBDD_4905_AF21_787A75430FB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mshtml.h>

#include <iostream> 
#include <deque>


enum ConnectType { ConnType_Advise, ConnType_Unadvise };

class CGf_ManageHtmlSink  
{
public:
	void AddEventSink(IDispatch* pdispWebBrowser);
	void UnSinkAll();
	CGf_ManageHtmlSink();
	virtual ~CGf_ManageHtmlSink();
	
	
private:
	
	class CConnInfo
	{
	public:
		CConnInfo(IDispatch* pWBDisp) :
		  m_spWBDisp(pWBDisp),
			  m_dwDocumentCookie(0),
			  m_dwWindowCookie(0)
		  {
		  }
		  
		  CComPtr<IDispatch> m_spWBDisp;
		  DWORD m_dwDocumentCookie;
		  DWORD m_dwWindowCookie;
	};
	
	
	HRESULT ManageEventHandlers(CConnInfo* pConnInfo, ConnectType eConnectType);
	
	private:
		//	// Handle to IE main window
		//	HWND m_hwndIE;			
		//
		//	// Keyboard hook handle
		//	HHOOK m_hHook;		
		//
		//	// Hotkey toggle
		//	BOOL m_bHotkeysEnabled;	
		//
		//	// TRUE if blocking new windows
		//	BOOL m_bBlockNewWindow;
		
		// TRUE if the user clicked a link
		BOOL m_bUserClickedLink;
		
		//	// TRUE if the page is about to be unloaded
		//	BOOL m_bUnloading;
		
		// Connection Tokens - used for Advise and Unadvise
		// 	DWORD m_dwBrowserCookie;   
		DWORD m_dwDocumentCookie;
		DWORD m_dwWindowCookie;
		// 	DWORD m_dwPropNotifyCookie;
		
		// Smart pointer to browser
	// 	CComQIPtr<IWebBrowser2, &IID_IWebBrowser2> m_spWebBrowser2;
		
		// Smart pointer to the document
		CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2> m_spHTMLDocument;
		
		// Smart pointer to the document window
		IHTMLWindow2* m_pHTMLWindow;
		
		// Top-level web browser IDispatch*
		LPDISPATCH m_pWBDisp; 
		
		// Smart pointer to IDispatch of object handling 
		CComQIPtr<IDispatch> m_spExtDispatch;
		
		// Smart pointer to IDispatch of HTMLDocumentEvent handler
		CComQIPtr<IDispatch> m_spDocEventDisp;
		
		// Smart pointer to IDispatch of HTMLWindowEvent handler
		CComQIPtr<IDispatch> m_spWindowEventDisp;
		
		std::deque<CConnInfo*> m_deqEventHandlers;
		
		
};

#endif // !defined(AFX_GF_MANAGEHTMLSINK_H__44637117_CBDD_4905_AF21_787A75430FB2__INCLUDED_)
