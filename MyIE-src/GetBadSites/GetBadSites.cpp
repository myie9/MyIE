// GetBadSites.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GetBadSites.h"
#include "GetTopUrl.h"
#include "HelperStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		CString strHello;
		strHello.LoadString(IDS_HELLO);
		cout << (LPCTSTR)strHello << endl;
	}

	ASSERT(CHelperStr::StartsWith(".cn", ".") == TRUE);
	ASSERT(CHelperStr::StartsWith("w.cn", ".") == FALSE);

	//CGetTopUrl::GetUrlFromDirToFile( _T("D:\\MyIE9\\¶ñÒâÍøÕ¾\\autoDir" ), "D:\\rawbadSites.txt");
	
	CGetTopUrl::GetUrlFromDirToFile( _T("D:\\MyIE9\\notBadSites\\autoDeal") , "D:\\rawNotBadSites.txt");

	return nRetCode;
}


