// RegHelper.h: interface for the CRegHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGHELPER_H__6AE500FE_D4FE_4D10_80E8_760FE06F4029__INCLUDED_)
#define AFX_REGHELPER_H__6AE500FE_D4FE_4D10_80E8_760FE06F4029__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MYIE_KEY _T("MyIE9.exe")

//########################################################
DWORD	_RegGetDword(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName);
BOOL	_RegSetDword(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName, DWORD dwValue);
CString _RegGetString(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName);
BOOL	_RegSetString(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName, LPCSTR lpValue, BOOL bCreate = TRUE);
void	_RegDeletePath(HKEY hKey, CString strPath);
void	_RegDeleteValue(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName);


class CRegHelper  
{
public:
	static void SetUserAgent();
	static void DelUserAgent();
	static void SetDefaultBrowserForWin7(const CString& strExe);
	static CString GetPCID();
	static void SetPCID();
	static int GetIEVer();
	static void SetDefaultBrowser(LPCTSTR lpszModule);
	static BOOL IsIE8();
	static void AllowActiveX(BOOL bNotAllow);
	static void SetOpenWordOutIE(BOOL bOutOfIE);
	static void SetMaxHttp();
	static BOOL IsMixContentAlert();
	static void EnableMixContentAlert();
	static void DisableMixContentAlert();
	static BOOL RegSetExString(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName, LPCSTR lpValue);
	static BOOL SetFavoriteFolder(const CString& strFav);
	
	static BOOL GetFavoriteFolder(CString & strFav);

	static BOOL IsIE8Mode();
	static void DisableIE8Mode();
	static void EnableIE8Mode();

	static BOOL IsMaxHttpSetted();

	CRegHelper();
	virtual ~CRegHelper();

private:
	static void SetOpenWord(DWORD dwValue);

};

#endif // !defined(AFX_REGHELPER_H__6AE500FE_D4FE_4D10_80E8_760FE06F4029__INCLUDED_)
