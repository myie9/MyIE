// Gf_ManageHtmlSink.cpp: implementation of the CGf_ManageHtmlSink class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gf_ManageHtmlSink.h"
#include "DocEventHandler.h"
#include <mshtml.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_ManageHtmlSink::CGf_ManageHtmlSink()
{
	
}

CGf_ManageHtmlSink::~CGf_ManageHtmlSink()
{
	
}


//
// Set event handlers for HTMLDocumentEvents2 and HTMLWindowEvents2
//
HRESULT CGf_ManageHtmlSink::ManageEventHandlers(CConnInfo* pConnInfo, ConnectType eConnectType)
{
	HRESULT hr = E_FAIL;
	
	CComQIPtr<IWebBrowser2, &IID_IWebBrowser2> spWB(pConnInfo->m_spWBDisp);
	if (spWB)
	{
		CComPtr<IDispatch> spDocDisp;
		hr = spWB->get_Document(&spDocDisp);
		if (SUCCEEDED(hr) && spDocDisp)
		{
			CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2> spDoc(spDocDisp);
			if (spDoc)
			{
				IHTMLWindow2* spWnd = NULL;
				hr = spDoc->get_parentWindow(&spWnd);
				if (SUCCEEDED(hr) && spWnd)
				{
					if (eConnectType == ConnType_Advise)
					{
						DWORD dwCookie = 0;
						
						CComObject<CDocEventHandler>* pDocHandler = NULL;
						hr = CComObject<CDocEventHandler>::CreateInstance (&pDocHandler);
						ATLASSERT(SUCCEEDED(hr));
						if (SUCCEEDED(hr) && pDocHandler)
						{
							pDocHandler->m_pbUserClickedLink = &m_bUserClickedLink;
							
							CComQIPtr<IDispatch> spDocHandlerDisp(pDocHandler);
							ATLASSERT(spDocHandlerDisp);
							
							ATLASSERT(pConnInfo->m_dwDocumentCookie == 0);
							dwCookie = 0;
							hr = AtlAdvise(spDoc, spDocHandlerDisp, DIID_HTMLDocumentEvents2, &dwCookie);
							ATLASSERT(SUCCEEDED(hr));
							if (SUCCEEDED(hr))
								pConnInfo->m_dwDocumentCookie = dwCookie;
						}
						
//						CComObject<CWindowEventHandler>* pWndHandler = NULL;
//						hr = CComObject<CWindowEventHandler>::CreateInstance (&pWndHandler);
//						ATLASSERT(SUCCEEDED(hr));
//						if (SUCCEEDED(hr) && pWndHandler)
//						{
//							pWndHandler->m_pbUnloading = &m_bUnloading;
//							
//							CComQIPtr<IDispatch> spWndHandlerDisp(pWndHandler);
//							ATLASSERT(spWndHandlerDisp);
//							
//							ATLASSERT(pConnInfo->m_dwWindowCookie == 0);
//							dwCookie = 0;
//							hr = AtlAdvise(spWnd, spWndHandlerDisp, DIID_HTMLWindowEvents2, &dwCookie);
//							ATLASSERT(SUCCEEDED(hr));
//							if (SUCCEEDED(hr))
//								pConnInfo->m_dwWindowCookie = dwCookie;
//						}
					}
					else
					{
						ATLASSERT(pConnInfo->m_dwWindowCookie != 0);
						hr = AtlUnadvise(spWnd, DIID_HTMLWindowEvents2, pConnInfo->m_dwWindowCookie);
						ATLASSERT(SUCCEEDED(hr) || hr == CONNECT_E_NOCONNECTION);
						pConnInfo->m_dwWindowCookie = 0;
						
						ATLASSERT(pConnInfo->m_dwDocumentCookie != 0);
						hr = AtlUnadvise(spDoc, DIID_HTMLDocumentEvents2, pConnInfo->m_dwDocumentCookie);
						ATLASSERT(SUCCEEDED(hr));
						pConnInfo->m_dwDocumentCookie = 0;
					}
				}
			}
		}
	}
	
	ATLASSERT(SUCCEEDED(hr));
	return hr;
}

void CGf_ManageHtmlSink::UnSinkAll()
{
	while (m_deqEventHandlers.size() > 0)
	{
		CConnInfo* pConnInfo = m_deqEventHandlers.front();
		ManageEventHandlers(pConnInfo, ConnType_Unadvise);
		delete pConnInfo;
		m_deqEventHandlers.pop_front();
	}
}

//called in DISPID_DOCUMENTCOMPLETE
void CGf_ManageHtmlSink::AddEventSink(IDispatch *pdispWebBrowser)
{
	CConnInfo* pConnInfo = new CConnInfo(pdispWebBrowser);
	if (pConnInfo)
	{
		m_deqEventHandlers.push_back(pConnInfo);
		ManageEventHandlers(pConnInfo, ConnType_Advise);
	}
}
