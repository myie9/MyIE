// MyIE9NSHandle.cpp : Implementation of CMyIE9App and DLL registration.

#include "stdafx.h"
#include "MyIE9.h"
#include "MyIE9NSHandle.h"
#include "afxmt.h"
#include "MainFrm.h"

/////////////////////////////////////////////////////////////////////////////
//
BYTE bytePlaceHold[] = {0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x01, 0x00,
						0x01, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
						0xFF, 0xFF, 0xFF, 0x21, 0xF9, 0x04, 0x01, 0x00,
						0x00, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x00,
						0x01, 0x00, 0x01, 0x00, 0x00, 0x02, 0x01, 0x44,
						0x00, 0x3B
};

CMyIE9NSHandle::CMyIE9NSHandle()
{
}

STDMETHODIMP CMyIE9NSHandle::Start(
        LPCWSTR szUrl,
        IInternetProtocolSink *pIProtSink,
        IInternetBindInfo *pIBindInfo,
        DWORD grfSTI,
        DWORD dwReserved)
{
	// Should we do the following or always fail? How can we continue if parsing failed?
	try{

	//dialog filter
	if (pmf->m_bUseDialogFilter && !PRESS_CTRL)
	{
		HRESULT hr;
		DWORD m_bindf;
		BINDINFO m_bindinfo;
		m_bindinfo.cbSize = sizeof(BINDINFO);
		hr = pIBindInfo->GetBindInfo(&m_bindf, &m_bindinfo);
		if (m_bindf == 0x100083)
		{
			CString strUrl(szUrl);
			//m_strLastNavUrl = view-source:http://127.0.0.1/forum/includes/default_javascript.js
			if (pmf->m_strLastNavUrl.Find(strUrl)==-1)
			{
				BOOL bFilter = TRUE;
				//must use pop list, for that ConfirmPopupFilter may add to pop list
				if (!pmf->NeedUnPopFilter(strUrl))
					bFilter = FALSE;
				//some special url
				else if (strUrl.Right(4)==".xsl" || strUrl.Right(4)==".htc")
					bFilter = FALSE;
				if (pmf->m_bNotFilterClickDialog)
				{
					//now MDIGetActive may not current child frame
					int nCurSel = pmf->m_wndTab.GetCurSel();
					if (nCurSel>=0)
					{
						TCITEM TabCtrlItem;
						TabCtrlItem.mask = TCIF_PARAM;
						pmf->m_wndTab.GetItem(nCurSel, &TabCtrlItem);
						if(TabCtrlItem.lParam)
						{
							CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
							if (tcf && tcf->m_pView)
							{
								LARGE_INTEGER liFinish;
								LARGE_INTEGER liFreq;
								QueryPerformanceCounter(&liFinish);
								QueryPerformanceFrequency(&liFreq);
								double secs = (double)(liFinish.LowPart - tcf->m_pView->m_tLastClick.LowPart)/(double)liFreq.LowPart;
								if (secs < pmf->m_nPopDelay)
									bFilter = FALSE;
							}
						}
					}
				}
				//confirm
				if (bFilter)
				{
					if (pmf->ConfirmPopupFilter(strUrl, TRUE))
					{
						pmf->AddToPopList(strUrl, 3);
						pmf->m_nDialogFilterCount++;
						//must only do this, do more will have quit problem
						if (pIProtSink)
							pIProtSink->ReportResult(S_OK, 0, 0);
						return S_OK;
					}
				}
			}
		}
	}
	//better place after dialog filter
	//if some dialog url filter by this, will pop a blank window
	if(pmf->m_bUseUrlFilter && !pmf->m_bUrlFilterDirty)
	{
		BOOL bFilter = FALSE;
		CString strUrl(szUrl);
		strUrl.MakeLower();

		bFilter = pmf->NeedUrlFilter(strUrl);
		if(bFilter)
		{
			if (pmf->m_bOutputSaveUrlFilter)
			{
				CString strMsg;
				LOADSTR(strMsg ,IDS_URL_FILTER);
				strMsg += strUrl;
				pmf->AddToCollector(strMsg,15);
			}
			if(pIProtSink!=NULL)
			{
				m_pProtSink = pIProtSink;
				m_pProtSink->AddRef();
				m_pProtSink->ReportData(BSCF_FIRSTDATANOTIFICATION || BSCF_LASTDATANOTIFICATION || BSCF_DATAFULLYAVAILABLE, 42, 42);
				m_pProtSink->ReportResult(S_OK, 0, 0);
				m_pProtSink->Release();
				m_pProtSink = NULL;
			}
			return S_OK;
		}
	}

	}catch(...){}

	return INET_E_USE_DEFAULT_PROTOCOLHANDLER;
}

STDMETHODIMP CMyIE9NSHandle::Continue(PROTOCOLDATA *pStateInfo)
{
	return S_OK;
}

STDMETHODIMP CMyIE9NSHandle::Abort(HRESULT hrReason,DWORD dwOptions)
{
	return E_NOTIMPL;
}

STDMETHODIMP CMyIE9NSHandle::Terminate(DWORD dwOptions)
{
	return  S_OK;
}

STDMETHODIMP CMyIE9NSHandle::Suspend()
{
ATLTRACENOTIMPL("Suspend\n");

	return E_NOTIMPL;
}

STDMETHODIMP CMyIE9NSHandle::Resume()
{
ATLTRACENOTIMPL("Resume\n");
	
	return E_NOTIMPL;
}

// IInternetProtocol::Read
// -----------------------

// This is how URLMON asks us for data. As we report data on the protocol sink,
// URLMON will periodically ask us for data via this method.

// How URLMON asks for data will not always correlate directly to what data is
// reported.

// Do not expect to see 1 and only 1 Read method for each time data is reported
// to the protocol sink (give URLMON what it asks for if you can.)

// Expect that extra read calls may come in after all the data in the buffer has
// been read (continue to return S_FALSE.)

// The amount requested may sometimes be much more than what was reported. URLMON
// typically looks for data in 4k buffers, but do not depend on this.

STDMETHODIMP CMyIE9NSHandle::Read(void *pv, ULONG cb, ULONG *pcbRead)
{
	try{

	if(cb>=42 && pv!=NULL)
	{
		memcpy(pv, bytePlaceHold, 42);
		*pcbRead =	42;
	}
	else
		*pcbRead = 0;

	}catch(...){}

	return S_FALSE;
}

STDMETHODIMP CMyIE9NSHandle::Seek(
        LARGE_INTEGER dlibMove,
        DWORD dwOrigin,
        ULARGE_INTEGER *plibNewPosition)
{
ATLTRACENOTIMPL("Seek\n");
	
	return E_NOTIMPL;
}

STDMETHODIMP CMyIE9NSHandle::LockRequest(DWORD dwOptions)
{
ATLTRACE(_T("LockRequest\n"));


	return S_OK;
}

STDMETHODIMP CMyIE9NSHandle::UnlockRequest()
{
ATLTRACE(_T("UnlockRequest\n"));

	return S_OK;
}


STDMETHODIMP CMyIE9NSHandle::CombineUrl(LPCWSTR pwzBaseUrl, LPCWSTR pwzRelativeUrl, DWORD dwCombineFlags,
	LPWSTR pwzResult, DWORD cchResult, DWORD *pcchResult, DWORD dwReserved)
{
ATLTRACE(_T("CombineURL\n"));

	return INET_E_DEFAULT_ACTION;
}

STDMETHODIMP CMyIE9NSHandle::CompareUrl(LPCWSTR pwzUrl1, LPCWSTR pwzUrl2, DWORD dwCompareFlags)
{
ATLTRACE(_T("CompareUrl\n"));

	// Ignoring dwCompareFlags
	if (0 == wcscmp(pwzUrl1, pwzUrl2))
		return S_OK;
	else
		return S_FALSE;
}

// IInternetProtocolInfo::ParseUrl
// -------------------------------
//
// Unlike the other methods of IIPI, ParseUrl is worthy of note:
//
// 1. ParseUrl is used in many HTML-based security checks. If you seem to be having a security
//    problem with your own protocol, it is probably due to your ParseUrl implementation.
//
// 2. ParseUrl is called many many times, even when the protocol is not doing a bind. Be aware
//    that this is used as a "general utility" function by IE for parsing and checking on URLs.
//    For example, mousing over a hyperlink to a X-DB2XML URL will cause MSHTML to call
//    CoInternetParseUrl 2 or more times. Every call will cocreate a new instance of this protocol
//    handler object, call IIPI::ParseUrl, and destroy the protocol handler instance.

// Our protocol handler "cheats". We pretend that the URL we were passed as the UDL file path is
// actually our URL. This allows us to easily map ourselves to the HTTP(S) protocol so that HTML
// pages served by HTTP(S) don't continually run into security problems with our protocol. Normally
// it is Against The Rules for web pages to access data across domains or between different protocols.

STDMETHODIMP CMyIE9NSHandle::ParseUrl(LPCWSTR pwzUrl, PARSEACTION ParseAction, DWORD dwParseFlags,
	LPWSTR pwzResult, DWORD cchResult, DWORD *pcchResult, DWORD dwReserved)
{
ATLTRACE(_T("ParseUrl\n"));

//	HRESULT hr = CoInternetParseUrl(m_url, ParseAction, dwParseFlags, pwzResult, dwUrlLen, pcchResult, dwReserved);

	return INET_E_DEFAULT_ACTION;
}

// TODO: Implement queryinfo against xml result set... not UDL file
STDMETHODIMP CMyIE9NSHandle::QueryInfo( LPCWSTR pwzUrl, QUERYOPTION QueryOption, DWORD dwQueryFlags,
	LPVOID pBuffer, DWORD cbBuffer, DWORD *pcbBuf, DWORD dwReserved)
{
ATLTRACE(_T("QueryInfo\n"));

	return INET_E_DEFAULT_ACTION;
}
