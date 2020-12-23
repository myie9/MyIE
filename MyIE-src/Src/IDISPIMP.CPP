/*
 * idispimp.CPP
 * IDispatch for Extending Dynamic HTML Object Model
 *
 * Copyright (c)1995-1999 Microsoft Corporation, All Rights Reserved
 */

#include "stdafx.h"
#include "idispimp.h"
#include "MyIE9.h"
#include "MainFrm.h"
#include "resource.h"       // main symbols

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Hardcoded information for extending the Object Model 
// Typically this would be supplied through a TypeInfo
// In this case the name "xxyyzz" maps to DISPID_Extend 
const WCHAR addFavorite[]		=L"addFavorite";
const WCHAR addProxy[]			=L"addProxy";
const WCHAR gbExec[]			=L"gbExec";
const WCHAR gbIsAdContent[]		=L"gbIsAdContent";

#define DISPID_addFavorite		20
#define DISPID_addProxy			21
#define DISPID_gbExec			22
#define DISPID_gbIsAdContent	23

//##################################################################
#define DOWITH_IMG(type)	\
	{	\
		if (type==1) pmf->CollectorDrop(&DataObj, *pdwEffect);	\
		else if (type==2) goto _returnInvalid;	\
		else if (type>=3) pmf->DoWithImgUrl(type);	\
	}

#define DOWITH_LINK(type)	\
	{	\
		if (type==1) pmf->CollectorDrop(&DataObj,*pdwEffect);	\
		else if (type>=2) pmf->DoWithLinkUrl(type,strUrl,strText);	\
	}

#define DOWITH_TXT(type)	\
	{	\
		if (type==1) pmf->CollectorDrop(&DataObj,*pdwEffect);	\
		else if (type>=2) pmf->DoWithText(type,strUrl);	\
	}
//##################################################################
/*
 * CImpIDispatch::CImpIDispatch
 * CImpIDispatch::~CImpIDispatch
 *
 * Parameters (Constructor):
 *  pSite           PCSite of the site we're in.
 *  pUnkOuter       LPUNKNOWN to which we delegate.
 */

CImpIDispatch::CImpIDispatch( void )
{
    m_cRef = 0;
}

CImpIDispatch::~CImpIDispatch( void )
{
	ASSERT( m_cRef == 0 );
}


/*
 * CImpIDispatch::QueryInterface
 * CImpIDispatch::AddRef
 * CImpIDispatch::Release
 *
 * Purpose:
 *  IUnknown members for CImpIDispatch object.
 */

STDMETHODIMP CImpIDispatch::QueryInterface( REFIID riid, void **ppv )
{
    *ppv = NULL;

	try{
    if ( IID_IDispatch == riid )
	{
        *ppv = this;
	}
	if ( NULL != *ppv )
    {
        ((LPUNKNOWN)*ppv)->AddRef();
        return NOERROR;
    }
	}catch(...){}

	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CImpIDispatch::AddRef(void)
{
    return ++m_cRef;
}

STDMETHODIMP_(ULONG) CImpIDispatch::Release(void)
{
    return --m_cRef;
}

//IDispatch
STDMETHODIMP CImpIDispatch::GetTypeInfoCount(UINT* /*pctinfo*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CImpIDispatch::GetTypeInfo(/* [in] */ UINT /*iTInfo*/,
            /* [in] */ LCID /*lcid*/,
            /* [out] */ ITypeInfo** /*ppTInfo*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CImpIDispatch::GetIDsOfNames(
            /* [in] */ REFIID riid,
            /* [size_is][in] */ OLECHAR** rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID* rgDispId)
{
	HRESULT hr;
	UINT	i;

	// Assume some degree of success
	hr = NOERROR;
	try{
	// Hardcoded mapping for this sample
	// A more usual procedure would be to use a TypeInfo
	for ( i=0; i < cNames; i++)
	{
		if(wcscmp(gbIsAdContent, rgszNames[i])==0)
			rgDispId[i] = DISPID_gbIsAdContent;
		else if(wcscmp(gbExec, rgszNames[i])==0)
			rgDispId[i] = DISPID_gbExec;
		else if(wcsicmp(addFavorite, rgszNames[i])==0)
			rgDispId[i] = DISPID_addFavorite;
		else if(wcscmp(addProxy, rgszNames[i])==0)
			rgDispId[i] = DISPID_addProxy;
		else
		{
			// One or more are unknown so set the return code accordingly
			hr = ResultFromScode(DISP_E_UNKNOWNNAME);
			rgDispId[i] = DISPID_UNKNOWN;
		}
	}
	}catch(...){}

	return hr;
}

STDMETHODIMP CImpIDispatch::Invoke(
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID /*riid*/,
            /* [in] */ LCID /*lcid*/,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS* pDispParams,
            /* [out] */ VARIANT* pVarResult,
            /* [out] */ EXCEPINFO* /*pExcepInfo*/,
            /* [out] */ UINT* puArgErr)
{

	if ( pmf!=NULL)
	{
		try
		{
			switch (dispIdMember)
			{
			case DISPID_gbIsAdContent:
				{
					CString strUrl(pDispParams->rgvarg[0].bstrVal);
					pVarResult->vt = VT_BOOL;
					pVarResult->boolVal = pmf->NeedUrlFilter(strUrl) ? VARIANT_TRUE : VARIANT_FALSE;
				}
				break;
			case DISPID_gbExec:
				{
					UINT id = pDispParams->rgvarg[0].lVal;
					if (id==ID_TOOLS_COOKIE ||
						id==ID_TOOLS_HISTORY ||
						id==ID_TOOLS_CACHE ||
						id==ID_TOOL_CLEANMyIE9FORM ||
						id==ID_TOOLS_CLEANADDR ||
						id==ID_CLEAN_TYPED_SEARCH ||
						id==ID_TOOLS_CLEAN_UNDO_CLOSE
						)
					{
						if (!pmf->PromptMenuCommand(id))
							break;
					}
					pmf->SendMessage(WM_COMMAND, id, 0);
				}
				break;
			case DISPID_addFavorite:
				if(pDispParams->cArgs == 2)
				   pmf->AddtoFavorite(pDispParams->rgvarg[1].bstrVal, &pDispParams->rgvarg[0]);
				else if(pDispParams->cArgs == 1)
				   pmf->AddtoFavorite(pDispParams->rgvarg[0].bstrVal, &pDispParams->rgvarg[0]);
				break;
			case DISPID_addProxy:
				if(pDispParams->cArgs == 4)
				{
					int webproxy =pDispParams->rgvarg[0].iVal;
					int speed = pDispParams->rgvarg[1].iVal;
					CString proxy(pDispParams->rgvarg[2].bstrVal);
					CString proxyname(pDispParams->rgvarg[3].bstrVal);
					if(webproxy > 0)
					{
						g_bProxyDirty = TRUE;
						pmf->m_bSaveConfig = TRUE;
						CProxyList * pl;
						if(webproxy == 1)
							pl = pmf->m_aProxyCategories.GetWebProxyList();
						else
							pl = pmf->m_aProxyCategories.GetWebProxyList(TRUE);
						if(pl!=NULL && pl->Find(proxy)==-1)
						{
							pl->Add(proxy, proxyname, speed);
							MSGBOX(IDS_PROXY_ADDED);
						}
					}
				}
				break;
			}
		}catch(...){}
	}

	return S_OK;
}

//#############################################################################
//DropTarget
CImpIDropTarget::CImpIDropTarget( void )
{
    m_cRef = 0;
}

CImpIDropTarget::~CImpIDropTarget( void )
{
	ASSERT( m_cRef == 0 );
}

/*
 * CImpIDropTarget::QueryInterface
 * CImpIDropTarget::AddRef
 * CImpIDropTarget::Release
 *
 * Purpose:
 *  IUnknown members for CImpIDispatch object.
 */

STDMETHODIMP CImpIDropTarget::QueryInterface( REFIID riid, void **ppv )
{
    *ppv = NULL;

	try{
    if ( IID_IDropTarget == riid )
	{
        *ppv = this;
	}
	
	if ( NULL != *ppv )
    {
        ((LPUNKNOWN)*ppv)->AddRef();
        return NOERROR;
    }
	}catch(...){}

	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CImpIDropTarget::AddRef(void)
{
    return ++m_cRef;
}

STDMETHODIMP_(ULONG) CImpIDropTarget::Release(void)
{
    return --m_cRef;
}

STDMETHODIMP CImpIDropTarget::DragEnter(/* [unique][in] */IDataObject __RPC_FAR *pDataObj,
            /* [in] */ DWORD grfKeyState,
            /* [in] */ POINTL pt,
            /* [out][in] */ DWORD __RPC_FAR *pdwEffect)
{
	try{
	if (*pdwEffect == 7)
		return S_OK;
	m_nKeyState = grfKeyState;
	if (grfKeyState&MK_RBUTTON)
	{
		if (pmf->m_bRMouseDrag || pmf->m_bRMouseGesture)
		{
			COleDataObject DataObj;
			HGLOBAL hData = NULL;
			DataObj.Attach(pDataObj);
			hData = DataObj.GetGlobalData(CF_TEXT);
			DataObj.Detach();
			m_bImage = hData? FALSE : TRUE;
			if (m_bImage)
				*pdwEffect = DROPEFFECT_LINK;
			else
				*pdwEffect = DROPEFFECT_NONE;
			return S_OK;
		}
	}
	}catch(...){}

    return S_OK;
}

STDMETHODIMP CImpIDropTarget::DragOver(/* [in] */ DWORD grfKeyState,
            /* [in] */ POINTL pt,
            /* [out][in] */ DWORD __RPC_FAR *pdwEffect)
{
	if (*pdwEffect == 7)
		return S_OK;
	m_nKeyState = grfKeyState;
	if (grfKeyState&MK_RBUTTON)
	{
		if (pmf->m_bRMouseDrag || pmf->m_bRMouseGesture)
		{
			CChildFrame* tcf = (CChildFrame*)pmf->MDIGetActive();
			POINT point;
			point.x = pt.x;
			point.y = pt.y;
			tcf->m_pView->ScreenToClient(&point);
			tcf->m_pView->SendMessage(WM_MOUSEMOVE, grfKeyState, MAKELONG(point.x, point.y));
			//
			if (m_bImage)
				*pdwEffect = DROPEFFECT_LINK;
			else
				*pdwEffect = DROPEFFECT_NONE;
			return S_OK;
		}
	}
	return S_OK;
}

STDMETHODIMP CImpIDropTarget::DragLeave()
{
	if (m_nKeyState&MK_RBUTTON && pmf->m_bRMouseGesture)
	{
		CChildFrame* tcf = (CChildFrame*)pmf->MDIGetActive();
		POINT point;
		GetCursorPos(&point);
		tcf->m_pView->ScreenToClient(&point);
		tcf->m_pView->SendMessage(WM_RBUTTONUP, m_nKeyState, MAKELONG(point.x, point.y));
	}
	return S_OK;
}
STDMETHODIMP CImpIDropTarget::Drop(/* [unique][in] */ IDataObject __RPC_FAR *pDataObj,
            /* [in] */ DWORD grfKeyState,
            /* [in] */ POINTL pt,
            /* [out][in] */ DWORD __RPC_FAR *pdwEffect)
{
	COleDataObject DataObj;
	BOOL bImage=FALSE;

	try{
	if (*pdwEffect == 7)
		return E_INVALIDARG;
	grfKeyState = m_nKeyState;//!!
	//
	HGLOBAL hData = NULL;
	CString strUrl,strText;
	DataObj.Attach(pDataObj);
	hData = DataObj.GetGlobalData(CF_TEXT);
	//DataObj.Detach();
    if (hData)
	{
		LPCSTR lpcszData=(LPCSTR)GlobalLock(hData);
		strUrl = lpcszData;
		GlobalUnlock(hData);
		if (*pdwEffect==DROPEFFECT_LINK)//link
		{
			strText.Empty();
			HGLOBAL hLinkData=DataObj.GetGlobalData(RegisterClipboardFormat("FileGroupDescriptor"));
			if(hLinkData)
			{
				strText = ((LPCSTR)GlobalLock(hLinkData)) + 76;
				strText = strText.Left(strText.GetLength()-4);//cut .url of abc.url
				GlobalUnlock(hLinkData);
			}
		}
    }
	else
	{
		HGLOBAL hData2 = NULL;
		hData2=DataObj.GetGlobalData(CF_HDROP);
		if (hData2) 
		{
			HDROP hdrop =(HDROP)GlobalLock(hData2);
			DragQueryFile(hdrop, 0, g_szFile, 1024);
			GlobalUnlock(hData2);
			bImage = pmf->IsImageType(g_szFile);
		}
		else
		{
			CString strMsg;
			LOADSTR(strMsg ,IDS_NOT_DROP_IMG);
			pmf->SetMessageText(strMsg);// MSGBOX(IDS_NOT_DROP_IMG);
		}
	}

	//Get direction
	int nDirection=1;
	CChildFrame* tcf = (CChildFrame*)pmf->MDIGetActive();
	POINT point;
	GetCursorPos(&point);
	tcf->m_pView->ScreenToClient(&point);
	int x = point.x - pmf->m_ptMouseDown.x;
	int y = point.y - pmf->m_ptMouseDown.y;
	if(x>abs(y) && x>0)
		nDirection = 1;//right
	else if(abs(x)>abs(y) && x<0)
		nDirection = 2;//left
	else if(y>abs(x) && y>0)
		nDirection = 3;//down
	else if(abs(y)>abs(x) && y<0)
		nDirection = 4;//up
	//###########################################################
	//ex drag
	BOOL bExDrag = FALSE;
	LARGE_INTEGER liFinish;
	LARGE_INTEGER liFreq;
	QueryPerformanceCounter(&liFinish);
	QueryPerformanceFrequency(&liFreq);
	if(liFreq.LowPart!=0)
	{
		CChildFrame* tcf = (CChildFrame*)pmf->MDIGetActive();
		if (tcf)
		{
			double secs = (double)(liFinish.LowPart - tcf->m_pView->m_tLastClick.LowPart)/(double)liFreq.LowPart;
			if (secs > 3)
				bExDrag = TRUE;
		}
	}
	//###########################################################
	
	if (hData==NULL)//image and drop file		
	{
		//if drag drop from ie
		if (bExDrag)
			pmf->CollectorDrop(&DataObj, *pdwEffect);
		//当image没有显示出时bImage为FALSE,但是不要_returnInvalid,因为还可能要看image地址
		//if (bImage==FALSE)
		//	goto _returnInvalid;
		else if (nDirection==1)
			DOWITH_IMG(pmf->m_nImgRight)
		else if (nDirection==2)
			DOWITH_IMG(pmf->m_nImgLeft)
		else if (nDirection==3)
			DOWITH_IMG(pmf->m_nImgDown)
		else
			DOWITH_IMG(pmf->m_nImgUp)
	}
	//###########################################################
	else if (*pdwEffect==DROPEFFECT_LINK)//link
	{
		if (bExDrag)
			pmf->DoWithText(3,strUrl);
		else if (nDirection==1)
			DOWITH_LINK(pmf->m_nLinkRight)
		else if (nDirection==2)
			DOWITH_LINK(pmf->m_nLinkLeft)
		else if (nDirection==3)
			DOWITH_LINK(pmf->m_nLinkDown)
		else
			DOWITH_LINK(pmf->m_nLinkUp)
	}
	//###########################################################
	else//text
	{
		if (bExDrag)
			pmf->DoWithText(3,strUrl);
		else if (nDirection==1)
			DOWITH_TXT(pmf->m_nTextRight)
		else if (nDirection==2)
			DOWITH_TXT(pmf->m_nTextLeft)
		else if (nDirection==3)
			DOWITH_TXT(pmf->m_nTextDown)
		else
			DOWITH_TXT(pmf->m_nTextUp)
	}
	}catch(...){}

	DataObj.Detach();
    return S_OK;

_returnInvalid:
	DataObj.Detach();
	return E_INVALIDARG;
}
