// MyIE9ÂèÂè°®ÉÏÍøä¯ÀÀÆ÷.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MyIE9ÂèÂè°®ÉÏÍøä¯ÀÀÆ÷.h"

#include "gf_shareConfig.h"
#include "PathHelper.h"
#include "DiskObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyIE9App
BEGIN_MESSAGE_MAP(CMyIE9App, CWinApp)
//{{AFX_MSG_MAP(CMyIE9App)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG
ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyIE9App construction

CMyIE9App::CMyIE9App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMyIE9App object

CMyIE9App theApp;

/////////////////////////////////////////////////////////////////////////////
// CMyIE9App initialization

BOOL CMyIE9App::InitInstance()
{
	try
	{
		
		// Standard initialization
		// If you are not using these features and wish to reduce the size
		//  of your final executable, you should remove from the following
		//  the specific initialization routines you do not need.
		
		CString strCurExeDir = CPathHelper::GetExeDir();
		
		CString strAppDir = strCurExeDir + GF_REAL_APP_FOLDER;
		
		CString strUpdateDir = strAppDir + GF_UPDATE_FOLDER;
		
		if (CPathHelper::IsExist(strUpdateDir))
		{
			CDiskObject oDisk;
			
			if (oDisk.CopyDirectories(strUpdateDir, strAppDir))
			{
				oDisk.RemoveDirectories(strUpdateDir);

				oDisk.RemoveFile(strAppDir + "MyIE9PostUpgrade.exe");
			}
			else
			{
				ASSERT(false);
			}
		}
		
		CString strRealAppExe = strAppDir + GF_REAL_APP_EXE;
		
		if( CPathHelper::IsExist(strRealAppExe) )
		{
			ShellExecute(NULL, "open", strRealAppExe, "", NULL, SW_SHOW);
		}
		else
		{
			CString strMsg;
			strMsg.Format(IDS_NOT_EXIST, strRealAppExe);
			AfxMessageBox(strMsg);
		}
		
		
		//	CMyIE9Dlg dlg;
		//	m_pMainWnd = &dlg;
		//	int nResponse = dlg.DoModal();
		//	if (nResponse == IDOK)
		//	{
		//		// TODO: Place code here to handle when the dialog is
		//		//  dismissed with OK
		//	}
		//	else if (nResponse == IDCANCEL)
		//	{
		//		// TODO: Place code here to handle when the dialog is
		//		//  dismissed with Cancel
		//	}
		//
		// Since the dialog has been closed, return FALSE so that we exit the
		//  application, rather than start the application's message pump.
		return FALSE;
		
	}
	catch (...) {
		ASSERT(false);
		return FALSE;
	}
}
