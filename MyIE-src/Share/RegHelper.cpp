// RegHelper.cpp: implementation of the CRegHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RegHelper.h"

#include "HelperException.h"
#include "HelperStr.h"
#include "PathHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define USER_SHELL_FOLER _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\User Shell Folders")
#define KEY_FAVORITE _T("Favorites")

#define MIX_CONTENT_REG_PATH _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Zones\\3")  
#define MIX_CONTENT_REG_KEY "1609"

#define IE8MODE_REG_PATH _T("Software\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BROWSER_EMULATION")

#define IE8_MODE_VALUE 8000  
#define IE7_MODE_VALUE 7000

//open word. 
#define BROWSER_FLAG "BrowserFlags"
#define VALUE_OPEN_WORD_OUT_IE 0x00000008
#define VALUE_OPEN_WORD_IN_IE  0x00000024

//Max http persistent connections
#define MYIE9_MAX_HTTPS_VALUE 100

#define MAX_HTTPS_PATH_IE7 "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"
#define MAX_HTTPS_KEY_IE7_V11 "MaxConnectionsPerServer"
#define MAX_HTTPS_KEY_IE7_V10 "MaxConnectionsPer1_0Server"

#define MAX_HTTPS_PATH_IE8_V11 "SOFTWARE\\Microsoft\\Internet Explorer\\MAIN\\FeatureControl\\FEATURE_MAXCONNECTIONSPERSERVER"
#define MAX_HTTPS_PATH_IE8_V10 "SOFTWARE\\Microsoft\\Internet Explorer\\MAIN\\FeatureControl\\FEATURE_MAXCONNECTIONSPER1_0SERVER"

#define USERAGENT_BROWSER "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\User Agent\\Post Platform"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegHelper::CRegHelper()
{

}

CRegHelper::~CRegHelper()
{

}

BOOL CRegHelper::SetFavoriteFolder(const CString &strFav)
{

	return	RegSetExString(HKEY_CURRENT_USER, USER_SHELL_FOLER, KEY_FAVORITE, (LPCTSTR)strFav);

}

BOOL CRegHelper::GetFavoriteFolder(CString & strFav)
{
	TCHAR           sz[MAX_PATH] = {'\0'};
	TCHAR           szPath[MAX_PATH] = {'\0'};
	HKEY            hKey;
	DWORD           dwSize;

	// find out from the registry where the favorites are located.
	if(RegOpenKey(HKEY_CURRENT_USER, USER_SHELL_FOLER, &hKey) != ERROR_SUCCESS)
	{
		if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"), &hKey) != ERROR_SUCCESS)
		{
			CHelperException::ErrorOcurred("Favorites folder not found. 7133.62");
			return FALSE;
		}
	}
	
	dwSize = sizeof(sz);
	RegQueryValueEx(hKey, KEY_FAVORITE, NULL, NULL, (LPBYTE)sz, &dwSize);

	RegCloseKey(hKey);

	if(dwSize == 0 || strlen(sz)==0)
	{
		return FALSE;
	}
	else{
		ExpandEnvironmentStrings(sz, szPath, MAX_PATH);

		strFav.Format("%s", szPath);
		return TRUE;
	}
}

BOOL CRegHelper::RegSetExString(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName, LPCSTR lpValue)
{
	HKEY            hKey;

	if(RegOpenKey(hKeyRoot, _T(lpSubKey), &hKey) != ERROR_SUCCESS)
	{
	   if(RegCreateKey(hKeyRoot, _T(lpSubKey), &hKey) != ERROR_SUCCESS)
		if(RegOpenKey(hKeyRoot, _T(lpSubKey), &hKey) != ERROR_SUCCESS)
			return FALSE;
	}
	RegSetValueEx(hKey, _T(lpValueName), 0, REG_EXPAND_SZ, (LPBYTE)lpValue, strlen(lpValue));
	RegCloseKey(hKey);

	return TRUE;
}

void CRegHelper::DisableMixContentAlert()
{
	if (!IsMixContentAlert())
	{
		return;
	}

	_RegSetDword(HKEY_CURRENT_USER, MIX_CONTENT_REG_PATH, MIX_CONTENT_REG_KEY, 0 );
}

void CRegHelper::EnableMixContentAlert()
{
	
	if (IsMixContentAlert())
	{
		return;
	}

	_RegSetDword(HKEY_CURRENT_USER, MIX_CONTENT_REG_PATH, MIX_CONTENT_REG_KEY, 1);

}

BOOL CRegHelper::IsMixContentAlert()
{
	DWORD dw = NULL;

	dw = _RegGetDword(HKEY_CURRENT_USER, MIX_CONTENT_REG_PATH, MIX_CONTENT_REG_KEY);

	if ( (NULL == dw) || (0 == dw) )
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}

}

BOOL CRegHelper::IsIE8Mode()
{
	DWORD dw = NULL;

	dw = _RegGetDword(HKEY_CURRENT_USER, IE8MODE_REG_PATH, MYIE_KEY);

	if ( (NULL == dw) || (IE7_MODE_VALUE == dw) )
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}

}


void CRegHelper::DisableIE8Mode()
{
	if (!IsIE8Mode())
	{
		return;
	}

	_RegDeleteValue(HKEY_CURRENT_USER, IE8MODE_REG_PATH, MYIE_KEY );
}

void CRegHelper::EnableIE8Mode()
{
	
	if (IsIE8Mode())
	{
		return;
	}

	_RegSetDword(HKEY_CURRENT_USER, IE8MODE_REG_PATH, MYIE_KEY, IE8_MODE_VALUE);

}
//##############################################################
DWORD _RegGetDword(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName)
{
	DWORD           sz;
	HKEY            hKey;
	DWORD           dwSize = sizeof(sz);

	if(RegOpenKey(hKeyRoot, _T(lpSubKey), &hKey) != ERROR_SUCCESS)
		return NULL;
	if (RegQueryValueEx(hKey, _T(lpValueName), NULL, NULL, (LPBYTE)&sz, &dwSize) != ERROR_SUCCESS)
		sz = NULL;
	RegCloseKey(hKey);

	return sz;
}

BOOL _RegSetDword(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName, DWORD dwValue)
{
	BOOL			bRet=FALSE;
	HKEY            hKey;
	DWORD           dwSize = sizeof(dwValue);

	if(RegOpenKey(hKeyRoot, _T(lpSubKey), &hKey) != ERROR_SUCCESS)
	{
		if(RegCreateKey(hKeyRoot, _T(lpSubKey), &hKey) != ERROR_SUCCESS)
			return bRet;
	}
	if (RegSetValueEx(hKey, _T(lpValueName), NULL, REG_DWORD, (LPBYTE)(&dwValue), dwSize) == ERROR_SUCCESS)
		bRet = TRUE;
	RegCloseKey(hKey);

	return bRet;
}

CString _RegGetString(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName)
{
	CString			strRet;
	HKEY            hKey;
	TCHAR           sz[MAX_PATH];
	DWORD           dwSize = sizeof(sz);

	strRet.Empty();
	memset(sz,0,MAX_PATH);
	if(RegOpenKey(hKeyRoot, _T(lpSubKey), &hKey) != ERROR_SUCCESS)
		return strRet;
	if (RegQueryValueEx(hKey, _T(lpValueName), NULL, NULL, (LPBYTE)sz, &dwSize) == ERROR_SUCCESS)
		strRet = sz;
	RegCloseKey(hKey);

	return strRet;
}

BOOL _RegSetString(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName, LPCSTR lpValue, BOOL bCreate)
{
	HKEY            hKey;

	if(RegOpenKey(hKeyRoot, _T(lpSubKey), &hKey) != ERROR_SUCCESS)
	{
		if (!bCreate)
		{
			return FALSE;
		}

		if(RegCreateKey(hKeyRoot, _T(lpSubKey), &hKey) != ERROR_SUCCESS)
			return FALSE;
	}
	RegSetValueEx(hKey, _T(lpValueName), 0, REG_SZ, (LPBYTE)lpValue, strlen(lpValue));
	RegCloseKey(hKey);

	return TRUE;
}

void _RegDeletePath(HKEY hKey, CString strPath)
{
	TCHAR   sz[MAX_PATH];
	HKEY	hKeySub;
	DWORD	dwSize = MAX_PATH;
	LONG	lRet;

	if (RegOpenKey(hKey, strPath, &hKeySub) != ERROR_SUCCESS)
		return ;
	//not i++, for that have delete one
	while(RegEnumKey(hKeySub, 0, sz, dwSize) == ERROR_SUCCESS)
	{
		lRet = RegDeleteKey(hKeySub,sz);
		if (lRet != ERROR_SUCCESS)
			_RegDeletePath(hKeySub, sz);
	}
	RegCloseKey(hKeySub);
	//delete the path
	RegDeleteKey(hKey,strPath);
}

void _RegDeleteValue(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName)
{
	HKEY	hKeySub;
	
	if (RegOpenKey(hKeyRoot, lpSubKey, &hKeySub) != ERROR_SUCCESS)
		return ;
	
	RegDeleteValue(hKeySub, lpValueName);
	
	RegCloseKey(hKeySub);
}


void CRegHelper::SetMaxHttp()
{
	if (!IsMaxHttpSetted() )
	{
		_RegSetDword (HKEY_CURRENT_USER, MAX_HTTPS_PATH_IE7, MAX_HTTPS_KEY_IE7_V11, MYIE9_MAX_HTTPS_VALUE) ;
		_RegSetDword (HKEY_CURRENT_USER, MAX_HTTPS_PATH_IE7, MAX_HTTPS_KEY_IE7_V10, MYIE9_MAX_HTTPS_VALUE) ;

		_RegSetDword (HKEY_CURRENT_USER, MAX_HTTPS_PATH_IE8_V11, MYIE_KEY, MYIE9_MAX_HTTPS_VALUE) ;
		_RegSetDword (HKEY_CURRENT_USER, MAX_HTTPS_PATH_IE8_V10, MYIE_KEY, MYIE9_MAX_HTTPS_VALUE) ;
	}

}

BOOL CRegHelper::IsMaxHttpSetted()
{
	DWORD dw = NULL;

	dw = _RegGetDword(HKEY_CURRENT_USER, MAX_HTTPS_PATH_IE8_V11, MYIE_KEY);

	if ( MYIE9_MAX_HTTPS_VALUE != dw) 
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}

}

/*
microsoft default: ie6,ie8 open in IE, ie7 open out of IE

*/
void CRegHelper::SetOpenWordOutIE(BOOL bOutOfIE)
{
	if (bOutOfIE)
	{
		SetOpenWord(VALUE_OPEN_WORD_OUT_IE);
	}
	else
	{
		SetOpenWord(VALUE_OPEN_WORD_IN_IE);
	}

	
}

void CRegHelper::SetOpenWord(DWORD dwValue)
{
	_RegSetDword (HKEY_CURRENT_USER, "SOFTWARE\\Classes\\Word.Document.8", BROWSER_FLAG, dwValue) ;
	_RegSetDword (HKEY_CURRENT_USER, "SOFTWARE\\Classes\\Word.RTF.8", BROWSER_FLAG, dwValue) ;
	_RegSetDword (HKEY_CURRENT_USER, "SOFTWARE\\Classes\\Word.Document.12", BROWSER_FLAG, dwValue) ;
	_RegSetDword (HKEY_CURRENT_USER, "SOFTWARE\\Classes\\Word.DocumentMacroEnabled.12", BROWSER_FLAG, dwValue) ;
}

void CRegHelper::AllowActiveX(BOOL bNotAllow)
{

	{
		DWORD dwGet = _RegGetDword (HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_HTTP_USERNAME_PASSWORD_DISABLE", MYIE_KEY) ; 

		if (dwGet == 1 )
		{
			return;
		}
	}

	//上面的信息栏。 0：不启用
	_RegSetDword (HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_SECURITYBAND", MYIE_KEY, 1) ;

	_RegSetDword (HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_HTTP_USERNAME_PASSWORD_DISABLE", MYIE_KEY, 1) ;

	/*IE7 http://msdn.microsoft.com/en-us/library/ms649488.aspx Security and Compatibility in Internet Explorer 7
	FEATURE_BLOCK_LMZ_IMG can block images that try to load from the user's local file system. To opt in, add your process name and set the value to 0x00000001, as shown below.
FEATURE_BLOCK_LMZ_OBJECT can block objects that try to load from the user's local file system. To opt in, add your process name and set the value to 0x00000001, as shown below.
FEATURE_BLOCK_LMZ_SCRIPT can block script access from the user's local file system. To opt in, add your process name and set the value to 0x00000001, as shown below.
	*/

//_RegSetDword (HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BLOCK_LMZ_IMG", MYIE_KEY, bNotAllow) ; //xp

//_RegSetDword (HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BLOCK_LMZ_OBJECT", MYIE_KEY, bNotAllow) ; //xp

		//0:允许 Activex 允许MIME 为了让“允许活动内容在本机上的文件中运行”成立的情况下也能在IE7中直接打开网络中的基于ActiveX活动内容如swf
		_RegSetDword (HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BLOCK_LMZ_SCRIPT", MYIE_KEY, bNotAllow) ; //xp

		_RegSetDword (HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_LMZSCRIPTFAIL_IN_INTERNET", MYIE_KEY, bNotAllow) ; //win2003
		
			
		//“允许活动内容在本机上的文件中运行”  0: 打勾		IE:1
		_RegSetDword (HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_LOCALMACHINE_LOCKDOWN", MYIE_KEY, bNotAllow) ;
//
//		//http://msdn.microsoft.com/en-us/library/ms537641.aspx Compatibility in Internet Explorer 6 for Windows XP Service Pack 2
//		_RegSetDword (HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_OBJECT_CACHING", MYIE_KEY, 1) ;
//		
//_RegSetDword (HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_MIME_HANDLING", MYIE_KEY, 1) ;
//
//_RegSetDword (HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_MIME_SNIFFING", MYIE_KEY, 1) ;
//
///*It is disabled by default for Internet Explorer (iexplore.exe)*/
//_RegSetDword (HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_PROTOCOL_LOCKDOWN", MYIE_KEY, 0) ;
//
////1: .exe not accessible
//_RegSetDword (HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_RESTRICT_FILEDOWNLOAD", MYIE_KEY, 0) ;
//
//_RegSetDword (HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_RESTRICT_ACTIVEXINSTALL", MYIE_KEY, 0) ;
//
///*can't block a lot of
//*/
//_RegSetDword (HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_WEBOC_POPUPMANAGEMENT", MYIE_KEY, 0) ;
//
//_RegSetDword (HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_ZONE_ELEVATION", MYIE_KEY, 1) ;
//
//_RegSetDword (HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BEHAVIORS", MYIE_KEY, 1) ;


}

BOOL CRegHelper::IsIE8()
{

	if ( GetIEVer() >= 8 )
	{
		return TRUE;
	}

	return FALSE;

}

void CRegHelper::SetDefaultBrowser(LPCTSTR lpszModule)
{
			//HKEY_CLASSES_ROOT\CLSID\{0002DF01-0000-0000-C000-000000000046}\LocalServer32
			_RegSetString(HKEY_CLASSES_ROOT, "CLSID\\{0002DF01-0000-0000-C000-000000000046}\\LocalServer32", "", lpszModule );

			//HKEY_LOCAL_MACHINE\SOFTWARE\Classes\CLSID\{0002DF01-0000-0000-C000-000000000046}\LocalServer32
			_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes\\CLSID\\{0002DF01-0000-0000-C000-000000000046}\\LocalServer32", "", lpszModule);

			/*
			[HKEY_LOCAL_MACHINE\SOFTWARE\Tencent\Traveler]
"exe"="D:\\Program Files\\TheWorld 2.0\\TheWorld.exe"

[HKEY_LOCAL_MACHINE\SOFTWARE\Tencent\TTraveler]
			*/
//gf			_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Tencent\\Traveler", "exe", lpszModule, TRUE);

			//_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Tencent\\Traveler", "Install", lpszModule, FALSE);

			//_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Tencent\\TTraveler", "exe", lpszModule, FALSE);
}

int CRegHelper::GetIEVer()
{
	int ret = 6;

	static CString strIEVer = _RegGetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Internet Explorer", "Version");

	if (strIEVer.IsEmpty())
	{
		strIEVer = _RegGetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Microsoft\\Internet Explorer", "Version");
	}

	if ( strIEVer.GetLength() > 1 )
	{
		strIEVer.SetAt(1,'\0');
		ret = atoi(strIEVer) ;
	}

	return ret;

}

void CRegHelper::SetPCID()
{
	CString strID = _RegGetString(HKEY_CURRENT_USER, "Control Panel\\Colors", "InactiveMenuText");

	if (strID.IsEmpty())
	{
		//set it
		
		srand( (unsigned)time( NULL ) );
		
		int i = rand() % 255;
		int j = rand() % 255;
		
		
		strID.Format("%d %d 255", i,j);

		
		_RegSetString(HKEY_CURRENT_USER, "Control Panel\\Colors", "InactiveMenuText", strID );

	}

				

}


CString CRegHelper::GetPCID()
{
	
	CString strID = _RegGetString(HKEY_CURRENT_USER, "Control Panel\\Colors", "InactiveMenuText");
	
	if (strID.IsEmpty())
	{
		SetPCID();
		
		strID = _RegGetString(HKEY_CURRENT_USER, "Control Panel\\Colors", "InactiveMenuText");
	}
	
	strID.Replace(" ", "2");
	
	return strID;
}


/************************************************************************/
/* Windows Registry Editor Version 5.00

  [HKEY_CURRENT_USER\Software\Classes\MyIE9.Association.HTML]
  @="MyIE9 HTML"
  
	[HKEY_CURRENT_USER\Software\Classes\MyIE9.Association.HTML\DefaultIcon]
	@="C:\\Windows\\System32\\url.dll,0"
	;这个是htm类文件的图标，MyIE9的图标我觉得有点难看没用，用的系统的图标，可自行修改。
	
	  [HKEY_CURRENT_USER\Software\Classes\MyIE9.Association.HTML\Shell]
	  
		[HKEY_CURRENT_USER\Software\Classes\MyIE9.Association.HTML\Shell\open]
		
		  [HKEY_CURRENT_USER\Software\Classes\MyIE9.Association.HTML\Shell\open\command]
		  @="\"E:\\MyIE9\\MyIE9.exe\" \"%1\""
		  ;MyIE9的完整路径下面还有注意修改完全
		  
			[HKEY_CURRENT_USER\Software\Microsoft\Windows\Shell\Associations\UrlAssociations]
			
			  [HKEY_CURRENT_USER\Software\Microsoft\Windows\Shell\Associations\UrlAssociations\http]
			  
				[HKEY_CURRENT_USER\Software\Microsoft\Windows\Shell\Associations\UrlAssociations\http\UserChoice]
				"Progid"="MyIE9.Association.HTML"
				
				  [HKEY_CURRENT_USER\Software\Microsoft\Windows\Shell\Associations\UrlAssociations\https]
				  
					[HKEY_CURRENT_USER\Software\Microsoft\Windows\Shell\Associations\UrlAssociations\https\UserChoice]
					"Progid"="MyIE9.Association.HTML"
					
					  
						[HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9]
						@="MyIE9 Web Browser"
						
						  [HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9\Capabilities]
						  "ApplicationDescription"="MyIE9 Web Browser"
						  "ApplicationName"="MyIE9 Web Browser"
						  "ApplicationIcon"="E:\\MyIE9\\MyIE9.exe,0 "
						  
							[HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9\Capabilities\FileAssociations]
							".htm"="MyIE9.Association.HTML"
							".html"="MyIE9.Association.HTML"
							".mht"="MyIE9.Association.HTML"
							".mhtm"="MyIE9.Association.HTML"
							".mhtml"="MyIE9.Association.HTML"
							
							  [HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9\Capabilities\Startmenu]
							  "StartmenuInternet"="MyIE9 Web Browse"
							  
								[HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9\Capabilities\UrlAssociations]
								"http"="MyIE9.Association.HTML"
								"https"="MyIE9.Association.HTML"
								
								  [HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9\DefaultIcon]
								  @="E:\\MyIE9\\MyIE9.exe,0"
								  
									[HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9\InstallInfo]
									"ReinstallCommand"="E:\\MyIE9\\MyIE9.exe"
									"ShowIconsCommand"="E:\\MyIE9\\MyIE9.exe"
									"HideIconsCommand"="E:\\MyIE9\\MyIE9.exe"
									"IconsVisible"=dword:00000001
									;这里是安装信息键值是随便填的糊弄系统的，好像没什么用。
									
									  [HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9\Shell]
									  
										[HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9\Shell\open]
										
										  [HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9\Shell\open\command]
										  @="E:\\MyIE9\\MyIE9.exe"
										  
											[HKEY_LOCAL_MACHINE\SOFTWARE\RegisteredApplications]
"MyIE9"="Software\\Clients\\StartMenuInternet\\MyIE9\\Capabilities"                                                                     */
/************************************************************************/
void CRegHelper::SetDefaultBrowserForWin7(const CString& strExe)
{
// 	[HKEY_CURRENT_USER\Software\Classes\MyIE9.Association.HTML]
// 		@="MyIE9 HTML"
_RegSetString(HKEY_CURRENT_USER, "Software\\Classes\\MyIE9.Association.HTML", _T(""), "MyIE9 HTML");
		
// 		[HKEY_CURRENT_USER\Software\Classes\MyIE9.Association.HTML\DefaultIcon]
// 		@="C:\\Windows\\System32\\url.dll,0"
// 		;这个是htm类文件的图标，MyIE9的图标我觉得有点难看没用，用的系统的图标，可自行修改。

_RegSetString(HKEY_CURRENT_USER, "Software\\Classes\\MyIE9.Association.HTML\\DefaultIcon", _T(""), CPathHelper::GetSysRootDir() + ":\\Windows\\System32\\url.dll,0");
		
// 		[HKEY_CURRENT_USER\Software\Classes\MyIE9.Association.HTML\Shell]
// 		
// 		[HKEY_CURRENT_USER\Software\Classes\MyIE9.Association.HTML\Shell\open]
// 		
// 		[HKEY_CURRENT_USER\Software\Classes\MyIE9.Association.HTML\Shell\open\command]
// 		@="\"E:\\MyIE9\\MyIE9.exe\" \"%1\""
// 		;MyIE9的完整路径下面还有注意修改完全
_RegSetString(HKEY_CURRENT_USER, "Software\\Classes\\MyIE9.Association.HTML\\Shell\\open\\command", _T(""), "\"" + strExe+"\" \"%1\"");
		
// 		[HKEY_CURRENT_USER\Software\Microsoft\Windows\Shell\Associations\UrlAssociations]
// 		
// 		[HKEY_CURRENT_USER\Software\Microsoft\Windows\Shell\Associations\UrlAssociations\http]
// 		
// 		[HKEY_CURRENT_USER\Software\Microsoft\Windows\Shell\Associations\UrlAssociations\http\UserChoice]
// 		"Progid"="MyIE9.Association.HTML"
_RegSetString(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\Shell\\Associations\\UrlAssociations\\http\\UserChoice", _T("Progid"), "MyIE9.Association.HTML");
		
// 		[HKEY_CURRENT_USER\Software\Microsoft\Windows\Shell\Associations\UrlAssociations\https]
// 		
// 		[HKEY_CURRENT_USER\Software\Microsoft\Windows\Shell\Associations\UrlAssociations\https\UserChoice]
//		"Progid"="MyIE9.Association.HTML"

_RegSetString(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\Shell\\Associations\\UrlAssociations\\https\\UserChoice", _T("Progid"), "MyIE9.Association.HTML");
		
		
// 		[HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9]
// 		@="MyIE9 Web Browser"
_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet\\MyIE9", _T(""), "MyIE9 Web Browser");
		
// 						  [HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9\Capabilities]
// 						  "ApplicationDescription"="MyIE9 Web Browser"
// 						  "ApplicationName"="MyIE9 Web Browser"
// 						  "ApplicationIcon"="E:\\MyIE9\\MyIE9.exe,0 "
_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet\\MyIE9\\Capabilities", _T("ApplicationDescription"), "MyIE9 Web Browser");
_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet\\MyIE9\\Capabilities", _T("ApplicationName"), "MyIE9 Web Browser");
_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet\\MyIE9\\Capabilities", _T("ApplicationIcon"), strExe+",0");
						  
// 						  [HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9\Capabilities\FileAssociations]
// 						  ".htm"="MyIE9.Association.HTML"
// 						  ".html"="MyIE9.Association.HTML"
// 						  ".mht"="MyIE9.Association.HTML"
// 						  ".mhtm"="MyIE9.Association.HTML"
// 						  ".mhtml"="MyIE9.Association.HTML"
_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet\\MyIE9\\Capabilities\\FileAssociations", _T(".htm"), "MyIE9.Association.HTML");
_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet\\MyIE9\\Capabilities\\FileAssociations", _T(".html"), "MyIE9.Association.HTML");
_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet\\MyIE9\\Capabilities\\FileAssociations", _T(".mht"), "MyIE9.Association.HTML");
_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet\\MyIE9\\Capabilities\\FileAssociations", _T(".mhtm"), "MyIE9.Association.HTML");
_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet\\MyIE9\\Capabilities\\FileAssociations", _T(".mhtml"), "MyIE9.Association.HTML");


// 						  [HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9\Capabilities\Startmenu]
// 						  "StartmenuInternet"="MyIE9 Web Browse"
_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet\\MyIE9\\Capabilities\\Startmenu", _T("StartmenuInternet"), "MyIE9 Web Browse");
 						  
// 						  [HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9\Capabilities\UrlAssociations]
// 						  "http"="MyIE9.Association.HTML"
// 						  "https"="MyIE9.Association.HTML"
 						  _RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet\\MyIE9\\Capabilities\\UrlAssociations", _T("http"), "MyIE9.Association.HTML");
						  _RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet\\MyIE9\\Capabilities\\UrlAssociations", _T("https"), "MyIE9.Association.HTML");

// 						  [HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9\DefaultIcon]
// 						  @="E:\\MyIE9\\MyIE9.exe,0"
					_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet\\MyIE9\\Capabilities\\UrlAssociations", _T(""), strExe+",0");
						  
// 						  [HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9\InstallInfo]
// 						  "ReinstallCommand"="E:\\MyIE9\\MyIE9.exe"
// 						  "ShowIconsCommand"="E:\\MyIE9\\MyIE9.exe"
// 						  "HideIconsCommand"="E:\\MyIE9\\MyIE9.exe"
// 						  "IconsVisible"=dword:00000001
// 						  ;这里是安装信息键值是随便填的糊弄系统的，好像没什么用。
					_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet\\MyIE9\\InstallInfo", _T("ReinstallCommand"), "\"" +strExe+"\"");
				_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet\\MyIE9\\InstallInfo", _T("ShowIconsCommand"), "\"" +strExe+"\"");
				_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet\\MyIE9\\InstallInfo", _T("HideIconsCommand"), "\"" +strExe+"\"");
				_RegSetDword(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet\\MyIE9\\InstallInfo", _T("IconsVisible"), 1);

// 						  [HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9\Shell]
// 						  
// 						  [HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9\Shell\open]
// 						  
// 						  [HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\MyIE9\Shell\open\command]
//						  @="E:\\MyIE9\\MyIE9.exe"
	_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet\\MyIE9\\Shell\\open\\command", _T(""), "\"" +strExe+"\"");
						  
// 						  [HKEY_LOCAL_MACHINE\SOFTWARE\RegisteredApplications]
// "MyIE9"="Software\\Clients\\StartMenuInternet\\MyIE9\\Capabilities"                                                                  
	_RegSetString(HKEY_LOCAL_MACHINE, "SOFTWARE\\RegisteredApplications", _T("MyIE9"), "Software\\Clients\\StartMenuInternet\\MyIE9\\Capabilities");

}

void CRegHelper::DelUserAgent()
{
	_RegDeleteValue(HKEY_LOCAL_MACHINE, USERAGENT_BROWSER, "MyIE9");
}

void CRegHelper::SetUserAgent()
{
	_RegSetString(HKEY_LOCAL_MACHINE, USERAGENT_BROWSER, "MyIE9", _T("IEAK"), TRUE );
}
