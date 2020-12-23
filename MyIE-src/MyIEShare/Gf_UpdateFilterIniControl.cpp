// Gf_UpdateFilterIniControl.cpp: implementation of the CGf_UpdateFilterIniControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyIE9.h"
#include "Gf_UpdateFilterIniControl.h"
#include "Gf_UpdateFilterIni.h"
#include "GfPathManager.h"
#include "Gf_ShareConfig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_UpdateFilterIniControl::CGf_UpdateFilterIniControl()
{

}

CGf_UpdateFilterIniControl::~CGf_UpdateFilterIniControl()
{

}


//要求事先有了CGfPathManager：：Ini（）
void CGf_UpdateFilterIniControl::Do(BOOL bFromMyIEQuit)
{
	CString verFile="";
	if (bFromMyIEQuit)
	{
		verFile=CGfPathManager::GetVersionFilePath();
	}
	else
	{
		verFile=CGfPathManager::GetUpdateDir() +GF_VERSION_INI_FILE_PATH;

	}

	int bCombineFilter = ::GetPrivateProfileInt( "Version", "combineFilter", 0, verFile );


	if (bCombineFilter>0)
	{
		CString sysDir="";

		if (bFromMyIEQuit)
		{
			sysDir=CGfPathManager::m_strResourceDir;
		}
		else
		{
			sysDir=CGfPathManager::GetUpdateDir()+"Resource\\";
		}

		CGf_UpdateFilterIni gf_UpdateFilterIni(CGfPathManager::m_strUserDir, sysDir);
		gf_UpdateFilterIni.Execute();
		
		if (bFromMyIEQuit)
		{			
			::WritePrivateProfileString( "Version", "combineFilter", "0", CGfPathManager::GetVersionFilePath() );
		}
	}
}
