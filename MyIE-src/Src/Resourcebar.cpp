// .cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "Resourcebar.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )


/////////////////////////////////////////////////////////////////////////////
// CResourcebarTree

CResourcebar::CResourcebar()
{
}

CResourcebar::~CResourcebar()
{
}



/////////////////////////////////////////////////////////////////////////////
// CHistoryTree message handlers

LPCSTR CResourcebar::GetTitle()
{
	LOADSTR(strTitle ,IDS_TITLE_RESOURCE);
	return (LPCSTR)strTitle;
}

BOOL CResourcebar::Initialize()
{
	return TRUE;
//     HINSTANCE hInstance = AfxGetResourceHandle();
//     ASSERT(hInstance != NULL);
// 
//     CString strResourceURL;
//     BOOL bRetVal = TRUE;
// 	LPTSTR lpszModule = NULL;
// 	try
// 	{
// 		 lpszModule = new TCHAR[_MAX_PATH];
// 	}
// 	catch(...)
// 	{
// 		return FALSE;
// 	}
// 
//     if (GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
//     {
// 		//check if resource.htm exists
// 		CString strSearchRes(lpszModule);
// 		int i = strSearchRes.ReverseFind('\\');
// 		strSearchRes = strSearchRes.Left(i+1);
// 		strSearchRes += "Resource\\resource.htm";
// 		OFSTRUCT fst;
// 		if(OpenFile(strSearchRes,&fst ,OF_EXIST)!=HFILE_ERROR)
// 			m_wndResource.Navigate(strSearchRes, 0, 0, 0,NULL);
// 		else
// 			m_wndResource.Navigate("about:blank", 0, 0, 0,NULL);
//     }
//     else
//        bRetVal = FALSE;
// 
//     delete [] lpszModule;
//     return bRetVal;
}

BOOL CResourcebar::Create(CWnd* pParent)
{
		m_wndResource.Create(NULL,NULL, WS_CHILD|WS_VISIBLE,CRect(0,0,0,0), pParent, 100);
//	m_wndResource.Create(NULL, "", WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN, CRect(0,0,0,0), pParent, IDS_TITLE_RESOURCE,NULL);
	return TRUE;
}


CWnd* CResourcebar::GetWindow()
{
	return (CWnd*)&m_wndResource;
}

void CResourcebar::Destroy()
{
	return;
}

BOOL CResourcebar::Update(int nLevel)
{
	return TRUE;
}

//#pragma optimize( "s", off)
