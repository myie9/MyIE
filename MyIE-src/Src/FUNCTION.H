#if !defined(_FUNCTION_H_)
#define _FUNCTION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "gf_func.h"

#define MAX_CL	8//max content of url, except http://....



//########################################################
CString _GetPlugInSection(CString strFileName, CString strSection);
int		_GetPlugInType(CString strFileName);
CString _GetPlugInIconPath(CString strFileName);
CString _GetPlugInName(CString strFileName);
CString _GetPlugInComments(CString strFileName);
CString _GetPlugInFileName(CString strFileName);

//########################################################
BOOL	_BrowserPath(CWnd* pParentWnd, CString& m_strPath);


CString _GetSameFormUrl(const CString& strUrl, BOOL bMain=FALSE);

//########################################################
BOOL	_GroupAddItem(CString strName, CString strUrl, BOOL bState, DWORD dwProperty, CString filename);


//########################################################
#ifdef _WRITE_LNG_FILE_
	void _WriteDlgString(CWnd *pDlg, CString strDomain);
	void _WriteStringTable();
	void _WriteAllMenuString();
	void _WriteDlgComboBox(CWnd *pDlg, CString strDomain);
#endif

void _InitLanguageFile(LPCSTR lpFileName);
void _LoadDlgString(CWnd *pDlg, CString strDomain);
void _LoadString(CString &str, UINT id);
void _LoadMenuString(CMenu *pMenu, CString strDomain);
void _LoadMenuStringOwnerDraw(CMenu *pMenu, CString strDomain,CImageList *imgList);
DWORD _GetPrivateProfileString(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName);

//
int MSGBOX(UINT nIDPrompt, UINT nType = MB_OK, UINT nIDHelp = (UINT)-1);
int MSGBOX(LPCTSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0);
//########################################################
void _LoadMenuString2ComboBox(CMenu *pMenu, CComboBox *pCb);
void _GetMenuItemString(CString &strName, int nID);

//########################################################
BOOL _OpenFtpUrl(CString strUrl);
//########################################################

//########################################################


#endif

