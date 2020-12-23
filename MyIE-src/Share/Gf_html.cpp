// Gf_html.cpp: implementation of the CGf_html class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <WININET.h>

#include "Gf_html.h"

#include "ComUtils.h"
#include "HelperStr.h"
#include "macro.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_html::CGf_html()
{
	
}

CGf_html::~CGf_html()
{
	
}



static HRESULT GetElementOffset( IHTMLElement *pElement, SIZE &offset )
{
    if( !pElement )
    {
        return E_POINTER;
    }
    
	//
	// as we're zeroing the offset structure here, be sure
	// to perform the recursive step before adding the offset
	// of the current element
	//
    ::ZeroMemory( &offset, sizeof( SIZE ) );
	
    ThrowHResult    hr;
	
    try
    {
		//
		// do we need to recurse up the element hierarchy?
		//
		// thanks to dchris_med (christig@home.ro) for the
		// fix when the link is in a text container (e.g. something
		// like the TEXTAREA tag
		//
        CComPtr< IHTMLElement > pParent;
		
        hr = pElement->get_offsetParent( &pParent );
        if( pParent )
        {
            hr = GetElementOffset( pParent, offset );
        }
		
        POINT   ptScroll = { 0 };
        CComQIPtr< IHTMLTextContainer > pTC = pElement;
        if( pTC )
        {
            pTC->get_scrollLeft( &ptScroll.x );
            pTC->get_scrollTop( &ptScroll.y );
        }
		
        POINT   ptClient = { 0 };
        CComQIPtr< IHTMLControlElement >    pCE = pElement;
        if( pCE )
        {
            pCE->get_clientLeft( &ptClient.x );
            pCE->get_clientTop( &ptClient.y );
        }
		
        long    cx, cy;
        hr = pElement->get_offsetLeft( &cx );
        hr = pElement->get_offsetTop( &cy );
		
        offset.cx = offset.cx + cx - ptScroll.x + ptClient.x;
        offset.cy = offset.cy + cy - ptScroll.y + ptClient.y;
    }
    catch( HRESULT )
    {
    }
	
    return hr;
}

/**
* Ascend the frame hierarchy (if any) until we find the first non-frame HTML 
* element at the specified position
**/
HRESULT CGf_html::HitTest( IWebBrowser2 *pWB, const POINT &pt, IHTMLElement **ppElement )
{
	TRACE("\n\nIn CGf_html::HitTest\n\n");

	*ppElement = NULL;
	
    if( pWB == NULL )
    {
		
        return E_INVALIDARG;
    }
	
    if( !ppElement )
    {
		
        return E_POINTER;
    }
	
    
	
    ThrowHResult    hr;
	
    try
    {
		//
		// get the document associated with this web browser object
		//
        CComPtr< IDispatch >    pDisp;
        hr = pWB->get_Document( &pDisp );
		
		//
		// is it an HTML document?
		//
        CComQIPtr< IHTMLDocument2 >   pDoc = pDisp;
		
        if( pDoc == NULL )
        {
			//
			// no - nothing more doing
			//
            return -1;
        }
		
		//
		// get the element at the specified point
		//
        CComPtr< IHTMLElement > pElement;
        hr = pDoc->elementFromPoint( pt.x, pt.y, &pElement );
		
        if( pElement == NULL )
        {
//			ASSERT(false);
            return -1;
        }
		
		
		POINT   ptInFrame = pt;
		
		for (int i=0; (i< 30) ; i++)
		{
			
			CComBSTR bsTag;
			HRESULT hrTag = pElement->get_tagName(&bsTag);
			
			CString strTag = bsTag;
			strTag.MakeUpper();

			if ("IFRAME" == strTag)  //因无法处理，当作正常元素返回，作为image 和 link 处理
			{
				break;
			}
			
			if ( ("FRAME" != strTag) && ("FRAMESET" != strTag) )
			{
				break;
			}
			
			//			{
			CComQIPtr< IWebBrowser2 >   pFrame = pElement;

			if (!pFrame)
			{
				return -1;
			}
			//
			// get the document associated with this web browser object
			//
			CComPtr< IDispatch >    pDispFrame;
			hr = pFrame->get_Document( &pDispFrame );
			
			//
			// is it an HTML document?
			// 
			CComQIPtr< IHTMLDocument2 >   pDocFrame = pDispFrame;
			
			if( pDocFrame == NULL )
			{
				//
				// no - nothing more doing
				//
				break;
			}
			
			//			}
			
			{
				
				//
				// how far is the frame nested within the document?
				//
				SIZE    offset;
				hr = GetElementOffset( pElement, offset );
				
				ptInFrame.x =  ptInFrame.x - offset.cx;
				ptInFrame.y =  ptInFrame.y - offset.cy;
				
			}
			
			CComPtr< IHTMLElement > pElementTry;
			hr = pDocFrame->elementFromPoint( ptInFrame.x, ptInFrame.y, &pElementTry );
			
			if (!SUCCEEDED(hr) || (!pElementTry))
			{
				break;
			}
			
			pElement = pElementTry;
			
		}
		
		//
		// return the element we've found to the user
		//
        hr = pElement->QueryInterface( 
			IID_IHTMLElement,
			reinterpret_cast< void ** >( ppElement )
			);
    }
    catch(...)
    {
		ASSERT(false);
		return -1;
    }
	
    return hr;
}


BOOL CGf_html::IsPointImage(IWebBrowser2 *pWB, const POINT &pt)
{
	CString strUrl = "";
	
	return IsPointImage(pWB, pt, strUrl);
}

BOOL CGf_html::IsPointImage(IWebBrowser2 *pWB, const POINT &pt, CString &strUrl, BOOL bHide)
{
	try{
		
		strUrl = "";
		
		if (NULL == pWB)
		{
			return FALSE;
		}
		
		
		CComPtr< IHTMLElement> pElem ;
		
		if(! SUCCEEDED(CGf_html::HitTest(pWB, pt,  &pElem) ) )
		{
			return FALSE;
		}
		
		if (!pElem)
		{
			ASSERT(false);// 不可能
			return FALSE;
		}
		
		BSTR bstrSrc;
		pElem->get_tagName(&bstrSrc);
		CString strTag = bstrSrc;
		SysFreeString(bstrSrc);
		
		BOOL bImage = FALSE;
		
		strTag.MakeUpper();
		
		if( strTag == "IMG") 
		{
			CComQIPtr<IHTMLImgElement, &IID_IHTMLImgElement> spImg(pElem);
			
			if (spImg)
			{
				CComBSTR bsValue;
				HRESULT hr = spImg->get_src(&bsValue);
				if (SUCCEEDED(hr) && (bsValue) )
				{
					strUrl = bsValue;
				}
			}
			
			bImage = TRUE;
		}
		else if("IFRAME" == strTag) 
		{
			CComQIPtr<IHTMLFrameBase, &IID_IHTMLFrameBase> spFrm(pElem);
			
			if (spFrm)
			{
				CComBSTR bsValue;
				HRESULT hr = spFrm->get_src(&bsValue);
				if (SUCCEEDED(hr) && (bsValue) )
				{
					strUrl = bsValue;
				}
			}
			
			bImage = TRUE;
		}
		else if (strTag == "OBJECT")
		{
			CComBSTR bsAttrib = L"movie";
			CComVariant varValue;
			if(SUCCEEDED(pElem->getAttribute(bsAttrib, 0, &varValue) ) )
			{
				CComBSTR bsValue = varValue.bstrVal;
				
				strUrl = bsValue;
				
				bImage = GF_FLASH_TYPE;
			}
			
		}
		else if (strTag == "EMBED")
		{
			CComQIPtr<IHTMLEmbedElement, &IID_IHTMLEmbedElement> spEmbed(pElem);
			
			if (spEmbed)
			{
				CComBSTR bsValue;
				HRESULT hr = spEmbed->get_src(&bsValue);
				if (SUCCEEDED(hr))
				{
					CString strValue = bsValue;
					strValue.MakeUpper();
					if ( CHelperStr::Contains(strValue, ".SWF") || CHelperStr::Contains(strValue, ".GIF") || CHelperStr::Contains(strValue, ".JPG") )
					{
						bImage = TRUE;

						if (CHelperStr::Contains(strValue, ".SWF") )
						{
							bImage = GF_FLASH_TYPE;
						}
						
						strUrl = bsValue;
					}
				}
				
			}
		}
		
		if (bImage && bHide)
		{
			CComPtr<IHTMLStyle> spStyle;
			pElem->get_style(&spStyle);
			spStyle->put_visibility(L"hidden");
			
		}
		
		
		//getFullAddress
		try{
			
			if (bImage && (!strUrl.IsEmpty() )&& IsRelativeUrl(strUrl) ) 
			{
				CString strLocationURL = "";
				
				CComBSTR bsWBLocationURL;
				pWB->get_LocationURL(&bsWBLocationURL);
				
				if (bsWBLocationURL)
				{
					strLocationURL = bsWBLocationURL;
				}
				
				
				//get Element url
				{
					CComPtr<IDispatch> spDispForDoc;
					
					pElem->get_document(&spDispForDoc);//
					
					CComQIPtr<IHTMLDocument2> spDocDisp(spDispForDoc);
					
					
					CComBSTR bsElemLocation;
					spDocDisp->get_URL(&bsElemLocation);
					
					if (bsElemLocation) //get url ok
					{
						strLocationURL = bsElemLocation;
					}
					
				}
				
				if (!strLocationURL.IsEmpty())
				{
					strUrl = GetFullUrl(strLocationURL, strUrl);
				}
				
			}
			
		}catch (...) {ASSERT(false);}
		
		
		return bImage;
		
	}catch(...){ASSERT(false);}
	
	
	return FALSE;
	
}

BOOL CGf_html::IsRelativeUrl(const CString &strUrl)
{
	
	if (strUrl.IsEmpty())
	{
		return FALSE;
	}
	
	if ( (strUrl.Left(1) == ".") || (strUrl.Left(1) == "/") )
	{
		return TRUE;
	}

	if (!IS_URL(strUrl) )
	{
		return TRUE;

	}
	
	return FALSE;
}

CString CGf_html::GetFullUrl(const CString &strLocationURL, const CString &strUrl)
{
	
	TCHAR buf [INTERNET_MAX_PATH_LENGTH];
	buf[0] = '\0';
	
	DWORD ulLen = INTERNET_MAX_PATH_LENGTH;
	
	BOOL bRetUrl = InternetCombineUrl(strLocationURL, strUrl, buf, &ulLen,ICU_BROWSER_MODE);
	if (bRetUrl)
	{
		return buf;
	}
	
	return strUrl;
	
}

