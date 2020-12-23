// GfPathManager.cpp: implementation of the CGfPathManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GfPathManager.h"
#include "PathHelper.h"

#include "Gf_ShareConfig.h"

#define INI_USER_CONFIG "UserConfig.ini"
#define INI_FILTER "Filter.ini"
#define INI_FLY "FilterFly.ini"

CString CGfPathManager::m_strRoot = "";
CString CGfPathManager::m_strResourceDir = "";
CString CGfPathManager::m_strUserDir = "";


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGfPathManager::CGfPathManager()
{
	
}

CGfPathManager::~CGfPathManager()
{
	
}

CString CGfPathManager::GetBadSiteFile()
{
	return m_strResourceDir + "b001.wav";
	
}

CString CGfPathManager::GetNotBadSiteFileSystem()
{
	return m_strResourceDir + "n001.wav";	
}

CString CGfPathManager::GetNotBadSiteFileUser()
{
	return m_strUserDir + "GoodWebSites.txt";	
}

void CGfPathManager::Init()
{
	
	m_strRoot = CPathHelper::GetExeDir();	
	
	m_strUserDir	= m_strRoot + "User\\";
	
	m_strResourceDir = m_strRoot+"Resource\\";
	
	
}

CString CGfPathManager::GetTempleLinksFolder()
{
	return m_strResourceDir + "TempleLinks";
	
}

CString CGfPathManager::GetUpdateExeFilePath()
{

	return 	m_strRoot + GF_UPDATE_EXE;
}

CString CGfPathManager::GetInternalVersion()
{

	CString strVersionIniFile = GetVersionFilePath();
	
	char tmp[MAX_PATH]={0};
	::GetPrivateProfileString("Version", "update", NULL, tmp, MAX_PATH, strVersionIniFile);

	return tmp;

}

CString CGfPathManager::GetUserConfig()
{
	static CString strRet = m_strUserDir + INI_USER_CONFIG;
	return strRet;
}

CString CGfPathManager::GetMyIE9StartPage(const CString& strFile)
{
	return GetResStartPageDir()+strFile;

}

CString CGfPathManager::GetLanguageFile()
{
	static CString strRet=m_strRoot + "Language\\" + CGfPathManager::GetLanguageName();
	return strRet;

}

CString CGfPathManager::GetLanguageName()
{
	//Init();
	CString strValue;
	::GetPrivateProfileString("Settings", "LngFile", "English.ini", strValue.GetBuffer(256), 256, CGfPathManager::GetUserConfig() );
	strValue.ReleaseBuffer();

	return strValue;

}

CString CGfPathManager::GetStartPageLeftBak()
{

	return m_strResourceDir+"startPage\\leftbak.htm";
}

CString CGfPathManager::GetStartPageLeft()
{
	return m_strUserDir+"left.htm";
}

CString CGfPathManager::GetFavName()
{
	return m_strUserDir+"favName.txt";

}

CString CGfPathManager::GetFavUrl()
{

	return m_strUserDir+"favUrl.txt";
}

CString CGfPathManager::GetResStartPageDir()
{
	return m_strResourceDir+"startPage\\";

}


CString CGfPathManager::GetVersionFilePath()
{

	return m_strRoot + GF_VERSION_INI_FILE_PATH;

}

CString CGfPathManager::GetUpdateDir()
{
	return m_strRoot + GF_UPDATE_FOLDER;

}

CString CGfPathManager::GetPluginData()
{
	return m_strUserDir +"PluginData.ini";
}

CString CGfPathManager::GetPluginDataSys()
{
	return m_strResourceDir +"PluginData.ini";
}

void CGfPathManager::RestoreUserConfig()
{
	CopyFile(m_strResourceDir+INI_USER_CONFIG, GetUserConfig(), FALSE);
	CopyFile(m_strResourceDir+INI_FILTER, m_strUserDir+INI_FILTER, FALSE);
	CopyFile(m_strResourceDir+INI_FLY, m_strUserDir+INI_FLY, FALSE);
}

CString CGfPathManager::GetUserBadSiteFile()
{
	return m_strUserDir+"BadWebSites.txt";
}
