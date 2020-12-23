// Gf_IE.cpp: implementation of the CGf_IE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gf_IE.h"

#include <WININET.H>

//#include "ProxyList.h"
#include "gf_func.h"
#include "macro.h"
#include "RegHelper.h"
#include "PathHelper.h"
#include "Consts_gf.h"

#pragma comment(lib, "Wininet.lib")

#define SWEEP_BUFFER_SIZE			10000
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_IE::CGf_IE()
{

}

CGf_IE::~CGf_IE()
{

}

void CGf_IE::DelTempFilesInThread()
{
	AfxBeginThread(CGf_IE::DelTempFilesThreadApi, NULL); //(LPVOID)(&iType)
}

UINT CGf_IE::DelTempFilesThreadApi(LPVOID param)
{	
	//DelTempType iType = (DelTempType)(*((int*)(param)));

	return CGf_IE::DelTempFiles(COOKIE_CACHE_ENTRY, FALSE);

}

BOOL CGf_IE::DelTempFiles(DWORD CacheType, BOOL op)
{
    BOOL bResult = FALSE;
    BOOL bDone = FALSE;
    LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;  
    DWORD  dwTrySize, dwEntrySize = 4096; // start buffer size    
    HANDLE hCacheDir = NULL;    
    DWORD  dwError = ERROR_INSUFFICIENT_BUFFER;
    
    do
	{                               
        switch (dwError)
        {
            // need a bigger buffer
            case ERROR_INSUFFICIENT_BUFFER: 
                delete [] lpCacheEntry;            
                lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
                lpCacheEntry->dwStructSize = dwEntrySize;
                dwTrySize = dwEntrySize;
                BOOL bSuccess;
                if (hCacheDir == NULL)
				{                  
                    bSuccess = (hCacheDir 
                      = FindFirstUrlCacheEntry(NULL, lpCacheEntry,
                      &dwTrySize)) != NULL;
				}
                else
                    bSuccess = FindNextUrlCacheEntry(hCacheDir, lpCacheEntry, &dwTrySize);
                if (bSuccess)
                    dwError = ERROR_SUCCESS;    
                else
                {
                    dwError = GetLastError();
                    dwEntrySize = dwTrySize; // use new size returned
                }
                break;

            case ERROR_NO_MORE_ITEMS:// we are done
                bDone = TRUE;
                bResult = TRUE;          
                break;

            case ERROR_SUCCESS:// we have got an entry
                // if CacheType=0 then delete all, else only delete CacheType
				if(op)
				{
					if (CacheType==0 || (lpCacheEntry->CacheEntryType & CacheType))                
                          DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
				}
				else
				{
					if (CacheType==0 || !(lpCacheEntry->CacheEntryType & CacheType))                
                          DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
				}
                // get ready for next entry
                dwTrySize = dwEntrySize;
                if (FindNextUrlCacheEntry(hCacheDir, lpCacheEntry, &dwTrySize))
                    dwError = ERROR_SUCCESS;          
                else
                {
                    dwError = GetLastError();
                    dwEntrySize = dwTrySize; // use new size returned
                }                    
                break;

            default:// unknown error
                bDone = TRUE;                
                break;
        }
        if (bDone)
        {   
            delete [] lpCacheEntry; 
            if (hCacheDir)
                FindCloseUrlCache(hCacheDir);         
        }
    }while (!bDone);
	return bResult;
}


CString CGf_IE::GetCacheFilePath(CString strUrl)
{
	CString strPath;
	strPath.Empty();
	
	DWORD dwEntrySize=0;
	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry;
	if (!GetUrlCacheEntryInfo(strUrl,NULL,&dwEntrySize))
	{
		if (GetLastError()==ERROR_INSUFFICIENT_BUFFER)
		{
			lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
			if (GetUrlCacheEntryInfo(strUrl,lpCacheEntry,&dwEntrySize))
			{
				strPath = lpCacheEntry->lpszLocalFileName;						
			}
			delete lpCacheEntry;
			lpCacheEntry = NULL;
		}
	}
	return strPath;
}

int CGf_IE::FindFavLinkIcon(CString strLinkFullPath, CImageList* pImg, int nIconDefault)
{
	int nRet = nIconDefault;

	static TCHAR buf [INTERNET_MAX_PATH_LENGTH];
	buf[0] = '\0';
			
	if (IS_FAVURL(strLinkFullPath))
		::GetPrivateProfileString(_T("InternetShortcut"), _T("URL"), _T(""), buf, INTERNET_MAX_PATH_LENGTH, strLinkFullPath);


	CString strUrl = buf;

	if ( IS_HTTP(strUrl))
	{
		CString strRoot = _StringGetTopUrl(strUrl);
		
		//get favicon from cache
		int nIcon = CGf_IE::FindCacheFavIcon(strRoot, pImg );
		if (FI_NOT_EXIST != nIcon )
		{
			nRet = nIcon;
		}

	}

	return nRet;
}

int CGf_IE::FindCacheFavIcon(CString strRoot, CImageList* pImg)
{
	//get favicon from cache
	CString strIcon = GetCacheFilePath( strRoot+"favicon.ico");
	if (strIcon.IsEmpty())
		return FI_NOT_EXIST;
	//
	HICON hIconSm;
	hIconSm = (HICON)LoadImage(NULL, 
		strIcon,
		IMAGE_ICON,
		16, 16,
		LR_LOADFROMFILE
		);
	if (hIconSm)
	{
		//gf CImageList* pImg = m_wndTab.GetImageList();
		int nIcon = pImg->Add(hIconSm);
		DestroyIcon(hIconSm);
		return nIcon;
	}
	
	return FI_NOT_EXIST;
}

void CGf_IE::DeleteAutoForms()
{
	// 清除表单自动完成历史记录
      CString sKey;
//      DWORD dwRet;

	if (IsWindows2k() || IsWindowsNT())//先判断系统
		{
			CString sBaseKey;
			SECURITY_DESCRIPTOR NewSD;
			BYTE* pOldSD;
			PACL pDacl = NULL;
			PSID pSid = NULL;
			TCHAR szSid[256];
			if (GetUserSid(&pSid))
			{
				//get the hiden key name
				GetSidString(pSid, szSid);

				sKey = _T("Software\\Microsoft\\Protected Storage System Provider\\");
				sKey += szSid;

				//get old SD
				sBaseKey = sKey;
				GetOldSD(HKEY_CURRENT_USER, sBaseKey, &pOldSD);

				//set new SD and then clear
				if (CreateNewSD(pSid, &NewSD, &pDacl))
				{
					RegSetPrivilege(HKEY_CURRENT_USER, sKey, &NewSD, FALSE);

					sKey += _T("\\Data");
					RegSetPrivilege(HKEY_CURRENT_USER, sKey, &NewSD, FALSE);

					sKey += _T("\\e161255a-37c3-11d2-bcaa-00c04fd929db");
					RegSetPrivilege(HKEY_CURRENT_USER, sKey, &NewSD, TRUE);

					//gf dwRet = SHDeleteKey(HKEY_CURRENT_USER, sKey);
					_RegDeletePath(HKEY_CURRENT_USER, sKey);
				}

				if (pDacl != NULL)
					HeapFree(GetProcessHeap(), 0, pDacl);

				//restore old SD
				if (pOldSD)
				{
					RegSetPrivilege(HKEY_CURRENT_USER, sBaseKey, 
						(SECURITY_DESCRIPTOR*)pOldSD, FALSE);
					delete pOldSD;
				}
			}
			if (pSid)
				HeapFree(GetProcessHeap(), 0, pSid);
		}

		//win9x
		DWORD dwSize = MAX_PATH;
		TCHAR szUserName[MAX_PATH];
		GetUserName(szUserName, &dwSize);

		sKey = _T("Software\\Microsoft\\Protected Storage System Provider\\");
		sKey += szUserName;
		sKey += _T("\\Data\\e161255a-37c3-11d2-bcaa-00c04fd929db");
		_RegDeletePath(HKEY_LOCAL_MACHINE, sKey);

}

//IE6, IE7
void CGf_IE::ClearPasswords()
{
	// 清除自动密码历史记录	
	_RegDeletePath(HKEY_CURRENT_USER, 
			_T("Software\\Microsoft\\Internet Explorer\\IntelliForms"));
}

//判断系统类型
BOOL CGf_IE::IsWindowsNT()
{
	BOOL bRet = FALSE;
	BOOL bOsVersionInfoEx;
	OSVERSIONINFOEX osvi;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure,
	// If that fails, try using the OSVERSIONINFO structure.
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return bRet;
	}

	if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion <= 4)
	{
		bRet = TRUE;
	}

	return bRet;
}

BOOL CGf_IE::IsWindows2k()
{
	BOOL bRet = FALSE;
	BOOL bOsVersionInfoEx;
	OSVERSIONINFOEX osvi;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure,
	// If that fails, try using the OSVERSIONINFO structure.
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return bRet;
	}

	if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion >= 5)
	{
		bRet = TRUE;
	}

	return bRet;
}











BOOL CGf_IE::GetUserSid(PSID* ppSid)
{
	HANDLE hToken;
	BOOL bRes;
	DWORD cbBuffer, cbRequired;
	PTOKEN_USER pUserInfo;

	// The User's SID can be obtained from the process token
	bRes = OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);
	if (FALSE == bRes)
	{
		return FALSE;
	}

	// Set buffer size to 0 for first call to determine
	// the size of buffer we need.
	cbBuffer = 0;
	bRes = GetTokenInformation(hToken, TokenUser, NULL, cbBuffer, &cbRequired);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		return FALSE;
	}

	// Allocate a buffer for our token user data
	cbBuffer = cbRequired;
	pUserInfo = (PTOKEN_USER) HeapAlloc(GetProcessHeap(), 0, cbBuffer);
	if (NULL == pUserInfo)
	{
		return FALSE;
	}

	// Make the "real" call
	bRes = GetTokenInformation(hToken, TokenUser, pUserInfo, cbBuffer, &cbRequired);
	if (FALSE == bRes) 
	{
		return FALSE;
	}

	// Make another copy of the SID for the return value
	cbBuffer = GetLengthSid(pUserInfo->User.Sid);

	*ppSid = (PSID) HeapAlloc(GetProcessHeap(), 0, cbBuffer);
	if (NULL == *ppSid)
	{
		return FALSE;
	}

	bRes = CopySid(cbBuffer, *ppSid, pUserInfo->User.Sid);
	if (FALSE == bRes)
	{
	    HeapFree(GetProcessHeap(), 0, *ppSid);
		return FALSE;
	}

	bRes = HeapFree(GetProcessHeap(), 0, pUserInfo);

	return TRUE;
}





void CGf_IE::GetSidString(PSID pSid, LPTSTR szBuffer)
{
	//convert SID to string
	SID_IDENTIFIER_AUTHORITY *psia = ::GetSidIdentifierAuthority( pSid );
	DWORD dwTopAuthority = psia->Value[5];
	_stprintf(szBuffer, _T("S-1-%lu"), dwTopAuthority);

	TCHAR szTemp[32];
	int iSubAuthorityCount = *(GetSidSubAuthorityCount(pSid));
	for (int i = 0; i<iSubAuthorityCount; i++) 
	{
		DWORD dwSubAuthority = *(GetSidSubAuthority(pSid, i));
		_stprintf(szTemp, _T("%lu"), dwSubAuthority);
		_tcscat(szBuffer, _T("-"));
		_tcscat(szBuffer, szTemp);
	}
}









BOOL CGf_IE::GetOldSD(HKEY hKey, LPCTSTR pszSubKey, BYTE** pSD)
{
	BOOL bRet = FALSE;
	HKEY hNewKey = NULL;
	DWORD dwSize = 0;
    LONG lRetCode;
	*pSD = NULL;

	lRetCode = RegOpenKeyEx(hKey, pszSubKey, 0, READ_CONTROL, &hNewKey);
	if(lRetCode != ERROR_SUCCESS)
		goto cleanup;

	lRetCode = RegGetKeySecurity(hNewKey, 
		(SECURITY_INFORMATION)DACL_SECURITY_INFORMATION, *pSD, &dwSize);
	if (lRetCode == ERROR_INSUFFICIENT_BUFFER)
	{
		*pSD = new BYTE[dwSize];
		lRetCode = RegGetKeySecurity(hNewKey, 
			(SECURITY_INFORMATION)DACL_SECURITY_INFORMATION, *pSD, &dwSize);
		if(lRetCode != ERROR_SUCCESS)
		{
			delete *pSD;
			*pSD = NULL;
			goto cleanup;
		}
	}
	else if (lRetCode != ERROR_SUCCESS)
		goto cleanup;

	bRet = TRUE; // indicate success

cleanup:
	if (hNewKey)
	{
		RegCloseKey(hNewKey);
	}
    return bRet;
}








BOOL CGf_IE::CreateNewSD(PSID pSid, SECURITY_DESCRIPTOR* pSD, PACL* ppDacl)
{
	BOOL bRet = FALSE;
    PSID pSystemSid = NULL;
    SID_IDENTIFIER_AUTHORITY sia = SECURITY_NT_AUTHORITY;
	ACCESS_ALLOWED_ACE* pACE = NULL;
    DWORD dwAclSize;
	DWORD dwAceSize;

    // prepare a Sid representing local system account
    if(!AllocateAndInitializeSid(&sia, 1, SECURITY_LOCAL_SYSTEM_RID,
        0, 0, 0, 0, 0, 0, 0, &pSystemSid))
	{
        goto cleanup;
    }

    // compute size of new acl
    dwAclSize = sizeof(ACL) + 2 * (sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD)) + 
		GetLengthSid(pSid) + GetLengthSid(pSystemSid);

    // allocate storage for Acl
    *ppDacl = (PACL)HeapAlloc(GetProcessHeap(), 0, dwAclSize);
    if(*ppDacl == NULL)
		goto cleanup;

    if(!InitializeAcl(*ppDacl, dwAclSize, ACL_REVISION))
        goto cleanup;

//    if(!AddAccessAllowedAce(pDacl, ACL_REVISION, KEY_WRITE, pSid))
//		goto cleanup;

    // add current user
	dwAceSize = sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD) + GetLengthSid(pSid); 
	pACE = (ACCESS_ALLOWED_ACE *)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, dwAceSize);

	pACE->Mask = KEY_READ | KEY_WRITE | KEY_ALL_ACCESS;
	pACE->Header.AceType = ACCESS_ALLOWED_ACE_TYPE;
	pACE->Header.AceFlags = CONTAINER_INHERIT_ACE | OBJECT_INHERIT_ACE;
	pACE->Header.AceSize = dwAceSize;
	
	memcpy(&pACE->SidStart, pSid, GetLengthSid(pSid));
	if (!AddAce(*ppDacl, ACL_REVISION, MAXDWORD, pACE, dwAceSize))
		goto cleanup;
    
    // add local system account
	HeapFree(GetProcessHeap(), 0, pACE);
	pACE = NULL;
	dwAceSize = sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD) + GetLengthSid(pSystemSid);
	pACE = (ACCESS_ALLOWED_ACE *)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, dwAceSize);

	pACE->Mask = KEY_READ | KEY_WRITE | KEY_ALL_ACCESS;
	pACE->Header.AceType = ACCESS_ALLOWED_ACE_TYPE;
	pACE->Header.AceFlags = CONTAINER_INHERIT_ACE | OBJECT_INHERIT_ACE;
	pACE->Header.AceSize = dwAceSize;
	
	memcpy(&pACE->SidStart, pSystemSid, GetLengthSid(pSystemSid));
	if (!AddAce(*ppDacl, ACL_REVISION, MAXDWORD, pACE, dwAceSize))
		goto cleanup;

	if(!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION))
        goto cleanup;

    if(!SetSecurityDescriptorDacl(pSD, TRUE, *ppDacl, FALSE))
        goto cleanup;
	
	bRet = TRUE; // indicate success

cleanup:
	if(pACE != NULL)
		HeapFree(GetProcessHeap(), 0, pACE);
    if(pSystemSid != NULL)
        FreeSid(pSystemSid);

    return bRet;
}








BOOL CGf_IE::RegSetPrivilege(HKEY hKey, LPCTSTR pszSubKey, 
							  SECURITY_DESCRIPTOR* pSD, BOOL bRecursive)
{
	BOOL bRet = FALSE;
    HKEY hSubKey = NULL;
    LONG lRetCode;
	LPTSTR pszKeyName = NULL;;
	DWORD dwSubKeyCnt;
	DWORD dwMaxSubKey;
	DWORD dwValueCnt;
	DWORD dwMaxValueName;
	DWORD dwMaxValueData;
	DWORD i;

	if (!pszSubKey)
		goto cleanup;

	// open the key for WRITE_DAC access
	lRetCode = RegOpenKeyEx(hKey, pszSubKey, 0, WRITE_DAC, &hSubKey);
	if(lRetCode != ERROR_SUCCESS)
		goto cleanup;

	// apply the security descriptor to the registry key
	lRetCode = RegSetKeySecurity(hSubKey, 
		(SECURITY_INFORMATION)DACL_SECURITY_INFORMATION, pSD);
	if( lRetCode != ERROR_SUCCESS )
		goto cleanup;

	if (bRecursive)
	{
		// reopen the key for KEY_READ access
		RegCloseKey(hSubKey);
		hSubKey = NULL;
		lRetCode = RegOpenKeyEx(hKey, pszSubKey, 0, KEY_READ, &hSubKey);
		if(lRetCode != ERROR_SUCCESS)
			goto cleanup;

		// first get an info about this subkey ...
		lRetCode = RegQueryInfoKey(hSubKey, 0, 0, 0, &dwSubKeyCnt, &dwMaxSubKey,
			0, &dwValueCnt, &dwMaxValueName, &dwMaxValueData, 0, 0);
		if( lRetCode != ERROR_SUCCESS )
			goto cleanup;

		// enumerate the subkeys and call RegTreeWalk() recursivly
		pszKeyName = new TCHAR [MAX_PATH + 1];
		for (i=0 ; i<dwSubKeyCnt; i++)
		{
			lRetCode = RegEnumKey(hSubKey, i, pszKeyName, MAX_PATH + 1);
			if(lRetCode == ERROR_SUCCESS)
			{
				RegSetPrivilege(hSubKey, pszKeyName, pSD, TRUE);
			}
			else if(lRetCode == ERROR_NO_MORE_ITEMS)
			{
				break;
			}
		}
		delete [] pszKeyName ;
	}

	bRet = TRUE; // indicate success

cleanup:
	if (hSubKey)
	{
		RegCloseKey(hSubKey);
	}
    return bRet;
}







BOOL CGf_IE::WipeFile(LPCTSTR szDir, LPCTSTR szFile)
{
	CString sPath;
	HANDLE	hFile;
	DWORD	dwSize;
	DWORD	dwWrite;
	char	sZero[SWEEP_BUFFER_SIZE];
	memset(sZero, 0, SWEEP_BUFFER_SIZE);

	sPath = szDir;
	sPath += _T('\\');
	sPath += szFile;

	hFile = CreateFile(sPath, GENERIC_WRITE, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	dwSize = GetFileSize(hFile, NULL);

	//skip file header (actually, I don't know the file format of index.dat)
	dwSize -= 64;
	SetFilePointer(hFile, 64, NULL, FILE_BEGIN);

	while (dwSize > 0)
	{
		if (dwSize > SWEEP_BUFFER_SIZE)
		{
			WriteFile(hFile, sZero, SWEEP_BUFFER_SIZE, &dwWrite, NULL);
			dwSize -= SWEEP_BUFFER_SIZE;
		}
		else
		{
			WriteFile(hFile, sZero, dwSize, &dwWrite, NULL);
			break;
		}
	}

	CloseHandle(hFile);
	return TRUE;
}

CString CGf_IE::GetIEFilePath()
{
			//get ie path
		CString strIePath = _RegGetString(HKEY_LOCAL_MACHINE,
			"SOFTWARE\\Microsoft\\Windows\\CurrentVersion",
			"ProgramFilesDir");
		if (strIePath.GetLength())
		{
			if(strIePath.Right(1) != "\\")
				strIePath += "\\";
			strIePath += "Internet Explorer\\iexplore.exe";
		}

		if (!CPathHelper::IsExist(strIePath))
		{
			strIePath.SetAt(0, 'C');

		}

		return strIePath;

}

CString CGf_IE::GetIEHomePage()
{
	return _RegGetString(HKEY_CURRENT_USER,
		"Software\\Microsoft\\Internet Explorer\\Main",
		"Start Page");
}
