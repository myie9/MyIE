// tear.cpp : implements the TEAR console application
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1997-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include <afx.h>
#include <afxwin.h>
#include <afxinet.h>
#include "tear.h"

#include <iostream.h>
#include <stdlib.h>

#include "PathHelper.h"
#include "Gf_UpdateControl.h"
#include "gf_ShareConfig.h"
#include "GfPathManager.h"
#include "gf_language.h"
#include "Gf_Lang.h"
#include "HelperStr.h"
#include "Gf_UpdateFilterIniControl.h"

/////////////////////////////////////////////////////////////////////////////
// Globals



static BOOL g_bShowMsg=FALSE;

void println(int id)
{
	if (!g_bShowMsg)
	{
		return;
	}
	
	CString strMsg=CGf_Lang::GetStr(id);
	
	CStringArray stlList;
	CHelperStr::SplitStr(stlList,strMsg, "\\n");
	
	for (int i=0; i< stlList.GetSize(); i++)
	{
		cerr<<stlList.GetAt(i)<<endl;
	}
	
	
}


/////////////////////////////////////////////////////////////////////////////
// Routines

void ShowBanner()
{
	
	println( GF_LINE_1_NOT_CONCERN);
	println(GF_WAITING);
}

void ShowUsage()
{
	cerr << _T("Usage:  TEAR [options] <URL>") << endl << endl;
	cerr << _T("\t<URL> points at a HTTP resource") << endl;
	cerr << _T("\t[options] are any of:") << endl;
	cerr << _T("\t\t/F force reload of requested page") << endl;
	cerr << _T("\t\t/P show detailed progress information") << endl;
	cerr << _T("\t\t/S strip HTML tags from stream") << endl << endl;
	cerr << _T("\t\t/L use local Internet access") << endl;
	cerr << _T("\t\t/D use pre-configured Internet access (default)") << endl;
	
	cerr << endl;
	//exit(1);
}

int waitQuit()
{
	if (!g_bShowMsg)
	{
		return 1;
	}

	println(GF_PRESS_ENTER);
	getchar();
	return 1;

}

/////////////////////////////////////////////////////////////////////////////
// The main() Thang

int main(int argc, char* argv[])
{
	try
	{
		CGfPathManager::Init();

		if (!(argc > 1 ))
		{
			g_bShowMsg = TRUE;
		}

		//g_bShowMsg = FALSE;//test
		
		ShowBanner();
		
		if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
		{
			cerr << _T("MFC Failed to initialize.\n");
			cerr << GF_FAILED;
			return 1;
		}
		
		//	if (argc < 2 || !ParseOptions(argc, argv) || pszURL == NULL)
		//		ShowUsage();
		
		CGf_UpdateControl oUpdateControl;
		BOOL bRetOfVer = oUpdateControl.IsLastedVersion(g_bShowMsg);
		if (TRUE == bRetOfVer)
		{
			if (!(argc > 1 ))
			{
				CString strMsg;
				strMsg.Format(CGf_Lang::GetStr(GF_MSG_ALREADY_LATEST_VERSION), CGf_UpdateControl::GetLocalVersion());
				cerr<<strMsg<<endl;
			}
			
			return waitQuit();
		}
		else if (-1 ==bRetOfVer)
		{
			println(GF_FAILED);
			return waitQuit();
		}
		else
		{
			println(8); //not newest
		}
		
		BOOL bRetCode = oUpdateControl.Execute();
		
		
		if (!bRetCode)
		{
			println(GF_FAILED); //strShowEnd = GF_FAILED;
			return waitQuit();
		}

		
		//cerr << strShowEnd << endl;
		
		println(GF_FINISHED);
		

		for (int i=0; i<5; i++)
		{
			Sleep(1000);
			

			if (CPathHelper::IsExist(CGfPathManager::GetUpdateDir()+GF_VERSION_INI_FILE_PATH ) )
			{
				if (g_bShowMsg)
				{					
					CGf_UpdateFilterIniControl::Do(FALSE);
				}

				
				CString strExeAfterUnpack = CGfPathManager::GetUpdateDir() + GF_NAME_EXE_AFTER_UNPACK;
				if (CPathHelper::IsExist(strExeAfterUnpack))
				{
					ShellExecute(NULL, "open", strExeAfterUnpack, "", NULL, SW_HIDE);
				}

				break;
			}
		}

		println(GF_UPGRADE_OK);

		return waitQuit();
		
	}
	catch (...) {
		ASSERT(false);
		return 3;
	}
}
