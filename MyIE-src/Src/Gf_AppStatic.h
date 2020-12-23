// Gf_AppStatic.h: interface for the CGf_AppStatic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_APPSTATIC_H__8A9EC750_3110_4CD3_A28D_ED5AA02C9C17__INCLUDED_)
#define AFX_GF_APPSTATIC_H__8A9EC750_3110_4CD3_A28D_ED5AA02C9C17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGf_AppStatic  
{
public:
	BOOL _bDelayLoaded;
	int _nMenuWidthOther;
	
	CString _strUserAppTitle;
	DWORD _dwStart;

	BOOL _bShowXIconInTab;
	CWnd* _curChild;
	
	BOOL _bStartFirstTime;
	//static CGf_AppStatic Instance();

	BOOL _bForbidAutoUpdate;
	BOOL _bKeepPageSize;
	int _iPageSize;

	BOOL _bEnableMixContentAlert;
	BOOL _bEnableIE8Mode;

	BOOL _bOpenWordOutOfIE;
	BOOL _bAutoHideFavSideBar;

	BOOL _bAlwaysIEMenuRightClickImg;

	BOOL _bDirectOpenForOneWord;
	BOOL _bRestoreConfig;
								
	static CGf_AppStatic _instance;

	CGf_AppStatic();
	virtual ~CGf_AppStatic();

};

#endif // !defined(AFX_GF_APPSTATIC_H__8A9EC750_3110_4CD3_A28D_ED5AA02C9C17__INCLUDED_)
