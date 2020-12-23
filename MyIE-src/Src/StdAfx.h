// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__19E497BF_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_)
#define AFX_STDAFX_H__19E497BF_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//########################################################
#define DWORD_PTR DWORD
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

//write new language file
//#define _WRITE_LNG_FILE_

//########################################################
#ifdef NDEBUG
	#pragma comment(linker,"/ALIGN:4096")
#endif

//########################################################
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxhtml.h>			// MFC HTML view support

#define _SCB_REPLACE_MINIFRAME
#define _SCB_MINIFRAME_CAPTION
#include "sizecbar.h"
#include "scbarg.h"
#include "scbarcf.h"
#define baseCMyBar CSizingControlBarCF

#define _ATL_APARTMENT_THREADED
#include <atlbase.h>

//########################################################
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class CMyIE9Module : public CComModule
{
public:
	LONG Unlock();
	LONG Lock();
	LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2);
	DWORD dwThreadID;
};
extern CMyIE9Module _Module;
#include <atlcom.h>

//open it when test: #define _GF_NOT_CATCH

#ifdef _GF_NOT_CATCH
#define GF_TRY 
#define GF_CATCH_TRUE 
#define GF_CATCH
#else
#define GF_TRY try
#define GF_CATCH_TRUE catch (...) {ASSERT(false); return TRUE; }
#define GF_CATCH catch (...) {ASSERT(false); }
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__19E497BF_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_)
