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
#pragma once
#include "resource.h"       // main symbols

//
///* interface IDocEventHandler */
///* [unique][helpstring][dual][uuid][object] */ 
//
//
//EXTERN_C const IID IID_IDocEventHandler;
//
//
//    
//    MIDL_INTERFACE("B1E68D42-02C4-465B-8368-5ED9B732E22D")
//    IDocEventHandler : public IDispatch
//    {
//    public:
//    };
//
//
//// IDocEventHandler
//[
//	object,
//	uuid("B1E68D42-02C4-465B-8368-5ED9B732E22D"),
//	dual,	helpstring("IDocEventHandler Interface"),
//	pointer_default(unique)
//]
//__interface IDocEventHandler : IDispatch
//{
//};
//
//
//
//// CDocEventHandler
//
//[
//	coclass,
//	threading("apartment"),
//	aggregatable("never"),
//	vi_progid("OsbornTech.PubDocEventHandler"),
//	progid("OsbornTech.PubDocEventHandler.1"),
//	version(1.0),
//	uuid("F9668ADA-FC6B-47F4-8381-DE861DBA5115"),
//	helpstring("OsbornTech DocEventHandler Class")
//  ]
class ATL_NO_VTABLE CDocEventHandler : public IDispatch
	//public IDispatchImpl<IDocEventHandler, &__uuidof(IDocEventHandler)>
{
public:
	CDocEventHandler() :
		m_pbUserClickedLink(NULL)
	{
	}


//	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:

	//
	// IDispatch Methods
	//
	STDMETHOD(Invoke)(DISPID dispidMember,REFIID riid, LCID lcid, WORD wFlags,
		DISPPARAMS * pdispparams, VARIANT * pvarResult,
		EXCEPINFO * pexcepinfo, UINT * puArgErr);

public:
	BOOL*	m_pbUserClickedLink;
};

