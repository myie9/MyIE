//
// Popup Blocker Version 2.0
// Copyright (c) 2003 Osborn Technologies, Inc.  All rights reserved.
// Web: http://www.osborntech.com
// Email: mailto:info@osborntech.com
//
// This software is being provided to you, the LICENSEE, by 
// Osborn Technologies under the following license.  By obtaining, 
// using and/or copying this software, you agree that you have read, 
// understood, and will comply with these terms and conditions:  
//
// Permission to use, copy, modify and distribute this software and 
// its documentation for any purpose and without fee or royalty is 
// hereby granted, provided that you agree to comply with the 
// following copyright notice and statements, including the 
// disclaimer, and that the same appear on ALL copies of the 
// software and documentation, including modifications that you make 
// for internal use or for distribution.  
//  
// Popup Blocker Version 2.0 Copyright © 2003 by Osborn Technologies.  All rights reserved.  
//  
// THIS SOFTWARE IS PROVIDED "AS IS" AND OSBORN TECHNOLOGIES MAKES 
// NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.  BY WAY OF 
// EXAMPLE, BUT NOT LIMITATION, OSBORN TECHNOLOGIES MAKES NO 
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR 
// ANY PARTICULAR PURPOSE, OR THAT THE FUNCTIONS CONTAINED IN THIS 
// SOFTWARE WILL MEET YOUR REQUIREMENTS OR EXPECTATIONS, OR THAT THE 
// OPERATION OF THIS SOFTWARE WILL BE ENTIRELY ERROR FREE OR APPEAR 
// PRECISELY AS DESCRIBED IN THE SOFTWARE DOCUMENTATION, OR THAT THE 
// USE OF THE LICENSED SOFTWARE OR DOCUMENTATION WILL NOT INFRINGE 
// ANY THIRD PARTY PATENTS, COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS.  
//  
// The name of Osborn Technologies may not be used in advertising or 
// publicity pertaining to distribution of this software.  Title to 
// copyright in this software and any associated documentation shall 
// at all times remain with Osborn Technologies and LICENSEE agrees 
// to preserve same.  
//

// DocEventHandler.cpp : Implementation of CDocEventHandler
// Handles HTMLDocumentEvents2

#include "stdafx.h"
#include <mshtmdid.h>
#include "DocEventHandler.h"
#include <mshtml.h>

// CDocEventHandler

//
// IDispatch Methods
//
STDMETHODIMP CDocEventHandler::Invoke(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags,
                                         DISPPARAMS* pDispParams, VARIANT* pvarResult,
                                         EXCEPINFO*  pExcepInfo,  UINT* puArgErr)
{
	if (!pDispParams)
		return E_INVALIDARG;
	
	if (!pvarResult)
		return E_POINTER;

	switch (dispidMember)
	{
	case DISPID_HTMLDOCUMENTEVENTS2_ONCLICK:
		ATLTRACE(_T("HTMLDocumentEvents::onclick fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONMOUSEDOWN:
		ATLTRACE(_T("HTMLDocumentEvents::onmousedown fired\n"));
		{
			CComPtr<IDispatch> spDisp;
			spDisp = pDispParams->rgvarg[0].pdispVal;
			if (spDisp)
			{
				CComQIPtr<IHTMLEventObj, &IID_IHTMLEventObj> spEventObj(spDisp);
				if (spEventObj)
				{
					long nMouseBtn = 0;
					HRESULT hr = spEventObj->get_button(&nMouseBtn);
					if (SUCCEEDED(hr) && nMouseBtn == 0)
					{
						ATLTRACE(_T("HTMLDocumentEvents::onmousedown fired -- simulated by script\n"));
						*m_pbUserClickedLink = FALSE;
						break;
					}
					
					CComPtr<IHTMLElement> spElem;
					hr = spEventObj->get_srcElement(&spElem);
					if (SUCCEEDED(hr) && spElem)
					{
						CComBSTR bsTagName;

						while (1)
						{
							spElem->get_tagName(&bsTagName);
							bsTagName.ToUpper();

							if (bsTagName == L"BODY")
								break;	// did not click a link

							if (bsTagName == L"A" ||
								bsTagName == L"AREA" ||
								bsTagName == L"INPUT" ||
								bsTagName == L"IMG")
							{
								ATLTRACE(_T("HTMLDocumentEvents::onclick fired -- user clicked a link\n"));
								*m_pbUserClickedLink = TRUE;
								break;
							}

							CComPtr<IHTMLElement> spParentElem;
							hr = spElem->get_parentElement(&spParentElem);
							if (FAILED(hr) || !spParentElem)
								break;
							spElem = spParentElem;
						}
					}
				}
			}
		}
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONPROPERTYCHANGE:
		ATLTRACE(_T("HTMLDocumentEvents::onpropertychange fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONREADYSTATECHANGE:
		ATLTRACE(_T("HTMLDocumentEvents::onreadystatechange fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONBEFOREUPDATE:
		ATLTRACE(_T("HTMLDocumentEvents::onbeforeupdate fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONAFTERUPDATE:
		ATLTRACE(_T("HTMLDocumentEvents::onafterupdate fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONROWEXIT:
		ATLTRACE(_T("HTMLDocumentEvents::onrowexit fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONROWENTER:
		ATLTRACE(_T("HTMLDocumentEvents::onrowenter fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONDRAGSTART:
		ATLTRACE(_T("HTMLDocumentEvents::ondragstart fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONSELECTSTART:
		ATLTRACE(_T("HTMLDocumentEvents::onselectstart fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONERRORUPDATE:
		ATLTRACE(_T("HTMLDocumentEvents::onerrorupdate fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONCONTEXTMENU:
		ATLTRACE(_T("HTMLDocumentEvents::oncontextmenu fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONSTOP:
		ATLTRACE(_T("HTMLDocumentEvents::onstop fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONROWSDELETE:
		ATLTRACE(_T("HTMLDocumentEvents::onrowsdelete fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONROWSINSERTED:
		ATLTRACE(_T("HTMLDocumentEvents::onrowsinserted fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONCELLCHANGE:
		ATLTRACE(_T("HTMLDocumentEvents::oncellchange fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONDATASETCHANGED:
		ATLTRACE(_T("HTMLDocumentEvents::ondatasetchanged fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONDATAAVAILABLE:
		ATLTRACE(_T("HTMLDocumentEvents::ondataavailable fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONDATASETCOMPLETE:
		ATLTRACE(_T("HTMLDocumentEvents::ondatasetcomplete fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONBEFOREEDITFOCUS:
		ATLTRACE(_T("HTMLDocumentEvents::onbeforeeditfocus fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONSELECTIONCHANGE:
		ATLTRACE(_T("HTMLDocumentEvents::onselectionchange fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONCONTROLSELECT:
		ATLTRACE(_T("HTMLDocumentEvents::oncontrolselect fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONFOCUSIN:
		ATLTRACE(_T("HTMLDocumentEvents::onfocusin fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONFOCUSOUT:
		ATLTRACE(_T("HTMLDocumentEvents::onfocusout fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONACTIVATE:
		ATLTRACE(_T("HTMLDocumentEvents::onactivate fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONDEACTIVATE:
		ATLTRACE(_T("HTMLDocumentEvents::ondeactivate fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONBEFOREACTIVATE:
		ATLTRACE(_T("HTMLDocumentEvents::onbeforeactivate fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONBEFOREDEACTIVATE:
		ATLTRACE(_T("HTMLDocumentEvents::onbeforedeactivate fired\n"));
		return DISP_E_MEMBERNOTFOUND;

	case DISPID_HTMLDOCUMENTEVENTS2_ONDBLCLICK:
	case DISPID_HTMLDOCUMENTEVENTS2_ONKEYDOWN:
	case DISPID_HTMLDOCUMENTEVENTS2_ONKEYUP:
	case DISPID_HTMLDOCUMENTEVENTS2_ONKEYPRESS:
	case DISPID_HTMLDOCUMENTEVENTS2_ONHELP:
	case DISPID_HTMLDOCUMENTEVENTS2_ONMOUSEMOVE:
	case DISPID_HTMLDOCUMENTEVENTS2_ONMOUSEUP:
	case DISPID_HTMLDOCUMENTEVENTS2_ONMOUSEOUT:
	case DISPID_HTMLDOCUMENTEVENTS2_ONMOUSEOVER:
	case DISPID_HTMLDOCUMENTEVENTS2_ONMOUSEWHEEL:
		return DISP_E_MEMBERNOTFOUND;

	default:
		ATLTRACE(_T("HTMLDocumentEvents2 event %ld (0x%x) fired\n"), dispidMember, dispidMember);
		return DISP_E_MEMBERNOTFOUND;
	}
	return S_OK;
}
