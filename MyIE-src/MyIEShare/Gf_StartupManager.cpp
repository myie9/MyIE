// Gf_StartupManager.cpp: implementation of the CGf_StartupManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gf_StartupManager.h"
#include "Gf_IniFile.h"
#include "GfPathManager.h"
#include "HelperStr.h"
#include "Gf_IE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define GF_START_MODE STARTUP_URL //1 //2:ie

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_StartupManager::CGf_StartupManager()
{

}

CGf_StartupManager::~CGf_StartupManager()
{

}

int CGf_StartupManager::GetStartMode( const CString &strIniFile)
{

	CString strStartMode=CGf_IniFile::DecodeJust("Settings", "StartupMode2", strIniFile );
	if (strStartMode.IsEmpty())
	{
		return STARTUP_URL;

	}
	
	int bGoHome = atoi( strStartMode);

	return bGoHome;

}

BOOL CGf_StartupManager::GetStartURLs(CStringArray &saUrl, const CString &strIniFile)
{
	CString strDec=GetStartupUrlStr(strIniFile);

	if (strDec.IsEmpty() )
	{
		return FALSE;
	}

	CHelperStr::SplitStr(saUrl, strDec, "\r\n");

	return TRUE;

}

CString CGf_StartupManager::GetStartupUrlStr( const CString &strIniFile)
{
	return CGf_IniFile::DecodeJust("Settings", "StartupURLs", strIniFile );
}

BOOL CGf_StartupManager::SaveStartMode(int nMode, const CString& strFilename)
{
	CString strMode;
	strMode.Format("%d", nMode);

	if (nMode == GetStartMode(strFilename))
	{
		return TRUE;
	}

	return CGf_IniFile::EncAndWriteString(INI_SETTINGS, "StartupMode2", strMode, strFilename, ENC_KEY_HOME1_0, ENC_KEY_HOME1_1);

}

BOOL CGf_StartupManager::SaveStartURL(const CString &strUrl, const CString &strIniFile)
{
	if (strUrl == GetStartupUrlStr(strIniFile) )
	{
		return TRUE;
	}

	return CGf_IniFile::EncAndWriteString(INI_SETTINGS, "StartupURLs", strUrl, strIniFile, ENC_KEY_HOME1_0, ENC_KEY_HOME1_1);
}


