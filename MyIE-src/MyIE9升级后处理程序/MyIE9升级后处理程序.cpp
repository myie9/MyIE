// MyIE9������������.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MyIE9������������.h"

#include "PathHelper.h"
#include "RegHelper.h"
#include "Gf_StartupManager.h"
#include "Gf_IE.h"
#include "Gf_IniFile.h"
#include "GfPathManager.h"
#include "Gf_UpdateFilterIni.h"
#include "HelperFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _countof(array) (sizeof(array)/sizeof(array[0]))


/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;



static CString GetIniFileString(LPCSTR lpszSection, LPCSTR lpszEntry, LPCSTR lpszDefault,LPCSTR pszProfileName)
{

		TCHAR szT[4096];
		DWORD dw = ::GetPrivateProfileString(lpszSection, lpszEntry,
			lpszDefault, szT, _countof(szT), pszProfileName);

		return szT;

}

static BOOL WriteProfileStringIfEqual(const CString& strSection, const CString& strKey,  const CString& strOldCompare, const CString& strFile, const CString& strNew)
{
					CString strOld = GetIniFileString(strSection, strKey, "", strFile);
				if ( strOldCompare == strOld)
				{					
					::WritePrivateProfileString(strSection, strKey, strNew, strFile);
					return TRUE;
				}
				return FALSE;

}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	try
	{
		
		int nRetCode = 0;
		
		// initialize MFC and print and error on failure
		if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			cerr << _T("Fatal Error: MFC initialization failed") << endl;
			nRetCode = 1;
			return nRetCode;
		}
		else
		{
			// TODO: code your application's behavior here.
			//		CString strHello;
			//		strHello.LoadString(IDS_HELLO);
			//cout << (LPCTSTR)strHello << endl;
		}
		
		//CGfPathManager::Init();
		CString strCurExeDir = CPathHelper::GetExeDir();
		
		CString strUserConfigPath = strCurExeDir + _T("..\\User\\SearchEngine.ini");
		CString strSearch=CHelperFile::Read(strUserConfigPath);
		strSearch.Replace("&prog=aff&client=pub-1430257349410019", "");
		strSearch.Replace("client=pub-1430257349410019&", "");
		CHelperFile::Write(strUserConfigPath, strSearch);

		CRegHelper::SetUserAgent();


		return nRetCode;
		
		
	}
	catch (...) {
		ASSERT(false);
		return 3;
	}
}


