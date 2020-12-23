// Gf_AppStatic.cpp: implementation of the CGf_AppStatic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gf_AppStatic.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_AppStatic CGf_AppStatic::_instance ;

CGf_AppStatic::CGf_AppStatic()
:_strUserAppTitle(_T("MyIE9"))
,_bDelayLoaded(FALSE)
,_bRestoreConfig(FALSE)
,_nMenuWidthOther(50)
,_bForbidAutoUpdate(FALSE)
,_bKeepPageSize(FALSE)
,_iPageSize(100)
,_bShowXIconInTab(TRUE)
,_bStartFirstTime(FALSE)
,_bEnableMixContentAlert(FALSE)
,_bEnableIE8Mode(FALSE)
,_bAutoHideFavSideBar(FALSE)
,_bAlwaysIEMenuRightClickImg(TRUE)
,_bDirectOpenForOneWord(TRUE)
{
}

CGf_AppStatic::~CGf_AppStatic()
{

}

//CGf_AppStatic CGf_AppStatic::Instance()
//{
//	return _instance;
//}
//


