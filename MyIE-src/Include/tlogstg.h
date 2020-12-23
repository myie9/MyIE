
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0279 */
/* at Thu Dec 13 20:16:45 2001
 */
/* Compiler settings for .\tlogstg.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32 (32b run), ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __tlogstg_h__
#define __tlogstg_h__

/* Forward Declarations */ 

#ifndef __ITravelLogEntry_FWD_DEFINED__
#define __ITravelLogEntry_FWD_DEFINED__
typedef interface ITravelLogEntry ITravelLogEntry;
#endif 	/* __ITravelLogEntry_FWD_DEFINED__ */


#ifndef __IEnumTravelLogEntry_FWD_DEFINED__
#define __IEnumTravelLogEntry_FWD_DEFINED__
typedef interface IEnumTravelLogEntry IEnumTravelLogEntry;
#endif 	/* __IEnumTravelLogEntry_FWD_DEFINED__ */


#ifndef __ITravelLogStg_FWD_DEFINED__
#define __ITravelLogStg_FWD_DEFINED__
typedef interface ITravelLogStg ITravelLogStg;
#endif 	/* __ITravelLogStg_FWD_DEFINED__ */


/* header files for imported files */
#include "objidl.h"
#include "oleidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_tlogstg_0000 */
/* [local] */ 

//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//

#pragma comment(lib,"uuid.lib")

//---------------------------------------------------------------------------=
// ITravelLogStg Interface.

#define SID_STravelLogCursor IID_ITravelLogStg 


extern RPC_IF_HANDLE __MIDL_itf_tlogstg_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_tlogstg_0000_v0_0_s_ifspec;

#ifndef __ITravelLogEntry_INTERFACE_DEFINED__
#define __ITravelLogEntry_INTERFACE_DEFINED__

/* interface ITravelLogEntry */
/* [local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ITravelLogEntry;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7EBFDD87-AD18-11d3-A4C5-00C04F72D6B8")
    ITravelLogEntry : public IUnknown
    {
    public:
        virtual /* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE GetTitle( 
            /* [out] */ LPOLESTR __RPC_FAR *ppszTitle) = 0;
        
        virtual /* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE GetURL( 
            /* [out] */ LPOLESTR __RPC_FAR *ppszURL) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITravelLogEntryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITravelLogEntry __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITravelLogEntry __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITravelLogEntry __RPC_FAR * This);
        
        /* [helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTitle )( 
            ITravelLogEntry __RPC_FAR * This,
            /* [out] */ LPOLESTR __RPC_FAR *ppszTitle);
        
        /* [helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetURL )( 
            ITravelLogEntry __RPC_FAR * This,
            /* [out] */ LPOLESTR __RPC_FAR *ppszURL);
        
        END_INTERFACE
    } ITravelLogEntryVtbl;

    interface ITravelLogEntry
    {
        CONST_VTBL struct ITravelLogEntryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITravelLogEntry_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITravelLogEntry_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITravelLogEntry_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITravelLogEntry_GetTitle(This,ppszTitle)	\
    (This)->lpVtbl -> GetTitle(This,ppszTitle)

#define ITravelLogEntry_GetURL(This,ppszURL)	\
    (This)->lpVtbl -> GetURL(This,ppszURL)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE ITravelLogEntry_GetTitle_Proxy( 
    ITravelLogEntry __RPC_FAR * This,
    /* [out] */ LPOLESTR __RPC_FAR *ppszTitle);


void __RPC_STUB ITravelLogEntry_GetTitle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE ITravelLogEntry_GetURL_Proxy( 
    ITravelLogEntry __RPC_FAR * This,
    /* [out] */ LPOLESTR __RPC_FAR *ppszURL);


void __RPC_STUB ITravelLogEntry_GetURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITravelLogEntry_INTERFACE_DEFINED__ */


#ifndef __IEnumTravelLogEntry_INTERFACE_DEFINED__
#define __IEnumTravelLogEntry_INTERFACE_DEFINED__

/* interface IEnumTravelLogEntry */
/* [local][helpcontext][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IEnumTravelLogEntry;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7EBFDD85-AD18-11d3-A4C5-00C04F72D6B8")
    IEnumTravelLogEntry : public IUnknown
    {
    public:
        virtual /* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Next( 
            /* [in] */ ULONG cElt,
            /* [length_is][size_is][out] */ ITravelLogEntry __RPC_FAR *__RPC_FAR *rgElt,
            /* [out] */ ULONG __RPC_FAR *pcEltFetched) = 0;
        
        virtual /* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG cElt) = 0;
        
        virtual /* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual /* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumTravelLogEntry __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumTravelLogEntryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEnumTravelLogEntry __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEnumTravelLogEntry __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEnumTravelLogEntry __RPC_FAR * This);
        
        /* [helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Next )( 
            IEnumTravelLogEntry __RPC_FAR * This,
            /* [in] */ ULONG cElt,
            /* [length_is][size_is][out] */ ITravelLogEntry __RPC_FAR *__RPC_FAR *rgElt,
            /* [out] */ ULONG __RPC_FAR *pcEltFetched);
        
        /* [helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Skip )( 
            IEnumTravelLogEntry __RPC_FAR * This,
            /* [in] */ ULONG cElt);
        
        /* [helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reset )( 
            IEnumTravelLogEntry __RPC_FAR * This);
        
        /* [helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IEnumTravelLogEntry __RPC_FAR * This,
            /* [out] */ IEnumTravelLogEntry __RPC_FAR *__RPC_FAR *ppEnum);
        
        END_INTERFACE
    } IEnumTravelLogEntryVtbl;

    interface IEnumTravelLogEntry
    {
        CONST_VTBL struct IEnumTravelLogEntryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumTravelLogEntry_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumTravelLogEntry_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumTravelLogEntry_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumTravelLogEntry_Next(This,cElt,rgElt,pcEltFetched)	\
    (This)->lpVtbl -> Next(This,cElt,rgElt,pcEltFetched)

#define IEnumTravelLogEntry_Skip(This,cElt)	\
    (This)->lpVtbl -> Skip(This,cElt)

#define IEnumTravelLogEntry_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumTravelLogEntry_Clone(This,ppEnum)	\
    (This)->lpVtbl -> Clone(This,ppEnum)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE IEnumTravelLogEntry_Next_Proxy( 
    IEnumTravelLogEntry __RPC_FAR * This,
    /* [in] */ ULONG cElt,
    /* [length_is][size_is][out] */ ITravelLogEntry __RPC_FAR *__RPC_FAR *rgElt,
    /* [out] */ ULONG __RPC_FAR *pcEltFetched);


void __RPC_STUB IEnumTravelLogEntry_Next_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE IEnumTravelLogEntry_Skip_Proxy( 
    IEnumTravelLogEntry __RPC_FAR * This,
    /* [in] */ ULONG cElt);


void __RPC_STUB IEnumTravelLogEntry_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE IEnumTravelLogEntry_Reset_Proxy( 
    IEnumTravelLogEntry __RPC_FAR * This);


void __RPC_STUB IEnumTravelLogEntry_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE IEnumTravelLogEntry_Clone_Proxy( 
    IEnumTravelLogEntry __RPC_FAR * This,
    /* [out] */ IEnumTravelLogEntry __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IEnumTravelLogEntry_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumTravelLogEntry_INTERFACE_DEFINED__ */


#ifndef __ITravelLogStg_INTERFACE_DEFINED__
#define __ITravelLogStg_INTERFACE_DEFINED__

/* interface ITravelLogStg */
/* [local][unique][object][uuid] */ 


enum __MIDL_ITravelLogStg_0001
    {	TLEF_RELATIVE_INCLUDE_CURRENT	= 0x1,
	TLEF_RELATIVE_BACK	= 0x10,
	TLEF_RELATIVE_FORE	= 0x20,
	TLEF_INCLUDE_UNINVOKEABLE	= 0x40,
	TLEF_ABSOLUTE	= 0x31
    };
typedef DWORD TLENUMF;


EXTERN_C const IID IID_ITravelLogStg;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7EBFDD80-AD18-11d3-A4C5-00C04F72D6B8")
    ITravelLogStg : public IUnknown
    {
    public:
        virtual /* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE CreateEntry( 
            /* [in] */ LPCOLESTR pszUrl,
            /* [in] */ LPCOLESTR pszTitle,
            /* [in] */ ITravelLogEntry __RPC_FAR *ptleRelativeTo,
            /* [in] */ BOOL fPrepend,
            /* [out] */ ITravelLogEntry __RPC_FAR *__RPC_FAR *pptle) = 0;
        
        virtual /* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE TravelTo( 
            /* [in] */ ITravelLogEntry __RPC_FAR *ptle) = 0;
        
        virtual /* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE EnumEntries( 
            /* [in] */ TLENUMF flags,
            /* [out] */ IEnumTravelLogEntry __RPC_FAR *__RPC_FAR *ppenum) = 0;
        
        virtual /* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE FindEntries( 
            /* [in] */ TLENUMF flags,
            /* [in] */ LPCOLESTR pszUrl,
            /* [out] */ IEnumTravelLogEntry __RPC_FAR *__RPC_FAR *ppenum) = 0;
        
        virtual /* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE GetCount( 
            /* [in] */ TLENUMF flags,
            /* [out] */ DWORD __RPC_FAR *pcEntries) = 0;
        
        virtual /* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE RemoveEntry( 
            /* [in] */ ITravelLogEntry __RPC_FAR *ptle) = 0;
        
        virtual /* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE GetRelativeEntry( 
            /* [in] */ int iOffset,
            /* [out] */ ITravelLogEntry __RPC_FAR *__RPC_FAR *ptle) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITravelLogStgVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITravelLogStg __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITravelLogStg __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITravelLogStg __RPC_FAR * This);
        
        /* [helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateEntry )( 
            ITravelLogStg __RPC_FAR * This,
            /* [in] */ LPCOLESTR pszUrl,
            /* [in] */ LPCOLESTR pszTitle,
            /* [in] */ ITravelLogEntry __RPC_FAR *ptleRelativeTo,
            /* [in] */ BOOL fPrepend,
            /* [out] */ ITravelLogEntry __RPC_FAR *__RPC_FAR *pptle);
        
        /* [helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TravelTo )( 
            ITravelLogStg __RPC_FAR * This,
            /* [in] */ ITravelLogEntry __RPC_FAR *ptle);
        
        /* [helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumEntries )( 
            ITravelLogStg __RPC_FAR * This,
            /* [in] */ TLENUMF flags,
            /* [out] */ IEnumTravelLogEntry __RPC_FAR *__RPC_FAR *ppenum);
        
        /* [helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindEntries )( 
            ITravelLogStg __RPC_FAR * This,
            /* [in] */ TLENUMF flags,
            /* [in] */ LPCOLESTR pszUrl,
            /* [out] */ IEnumTravelLogEntry __RPC_FAR *__RPC_FAR *ppenum);
        
        /* [helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCount )( 
            ITravelLogStg __RPC_FAR * This,
            /* [in] */ TLENUMF flags,
            /* [out] */ DWORD __RPC_FAR *pcEntries);
        
        /* [helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveEntry )( 
            ITravelLogStg __RPC_FAR * This,
            /* [in] */ ITravelLogEntry __RPC_FAR *ptle);
        
        /* [helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRelativeEntry )( 
            ITravelLogStg __RPC_FAR * This,
            /* [in] */ int iOffset,
            /* [out] */ ITravelLogEntry __RPC_FAR *__RPC_FAR *ptle);
        
        END_INTERFACE
    } ITravelLogStgVtbl;

    interface ITravelLogStg
    {
        CONST_VTBL struct ITravelLogStgVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITravelLogStg_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITravelLogStg_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITravelLogStg_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITravelLogStg_CreateEntry(This,pszUrl,pszTitle,ptleRelativeTo,fPrepend,pptle)	\
    (This)->lpVtbl -> CreateEntry(This,pszUrl,pszTitle,ptleRelativeTo,fPrepend,pptle)

#define ITravelLogStg_TravelTo(This,ptle)	\
    (This)->lpVtbl -> TravelTo(This,ptle)

#define ITravelLogStg_EnumEntries(This,flags,ppenum)	\
    (This)->lpVtbl -> EnumEntries(This,flags,ppenum)

#define ITravelLogStg_FindEntries(This,flags,pszUrl,ppenum)	\
    (This)->lpVtbl -> FindEntries(This,flags,pszUrl,ppenum)

#define ITravelLogStg_GetCount(This,flags,pcEntries)	\
    (This)->lpVtbl -> GetCount(This,flags,pcEntries)

#define ITravelLogStg_RemoveEntry(This,ptle)	\
    (This)->lpVtbl -> RemoveEntry(This,ptle)

#define ITravelLogStg_GetRelativeEntry(This,iOffset,ptle)	\
    (This)->lpVtbl -> GetRelativeEntry(This,iOffset,ptle)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE ITravelLogStg_CreateEntry_Proxy( 
    ITravelLogStg __RPC_FAR * This,
    /* [in] */ LPCOLESTR pszUrl,
    /* [in] */ LPCOLESTR pszTitle,
    /* [in] */ ITravelLogEntry __RPC_FAR *ptleRelativeTo,
    /* [in] */ BOOL fPrepend,
    /* [out] */ ITravelLogEntry __RPC_FAR *__RPC_FAR *pptle);


void __RPC_STUB ITravelLogStg_CreateEntry_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE ITravelLogStg_TravelTo_Proxy( 
    ITravelLogStg __RPC_FAR * This,
    /* [in] */ ITravelLogEntry __RPC_FAR *ptle);


void __RPC_STUB ITravelLogStg_TravelTo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE ITravelLogStg_EnumEntries_Proxy( 
    ITravelLogStg __RPC_FAR * This,
    /* [in] */ TLENUMF flags,
    /* [out] */ IEnumTravelLogEntry __RPC_FAR *__RPC_FAR *ppenum);


void __RPC_STUB ITravelLogStg_EnumEntries_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE ITravelLogStg_FindEntries_Proxy( 
    ITravelLogStg __RPC_FAR * This,
    /* [in] */ TLENUMF flags,
    /* [in] */ LPCOLESTR pszUrl,
    /* [out] */ IEnumTravelLogEntry __RPC_FAR *__RPC_FAR *ppenum);


void __RPC_STUB ITravelLogStg_FindEntries_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE ITravelLogStg_GetCount_Proxy( 
    ITravelLogStg __RPC_FAR * This,
    /* [in] */ TLENUMF flags,
    /* [out] */ DWORD __RPC_FAR *pcEntries);


void __RPC_STUB ITravelLogStg_GetCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE ITravelLogStg_RemoveEntry_Proxy( 
    ITravelLogStg __RPC_FAR * This,
    /* [in] */ ITravelLogEntry __RPC_FAR *ptle);


void __RPC_STUB ITravelLogStg_RemoveEntry_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE ITravelLogStg_GetRelativeEntry_Proxy( 
    ITravelLogStg __RPC_FAR * This,
    /* [in] */ int iOffset,
    /* [out] */ ITravelLogEntry __RPC_FAR *__RPC_FAR *ptle);


void __RPC_STUB ITravelLogStg_GetRelativeEntry_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITravelLogStg_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


