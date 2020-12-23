// GfPathManager.h: interface for the CGfPathManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GFPATHMANAGER_H__CBC9082D_6891_4A31_862F_23BE808B1DA9__INCLUDED_)
#define AFX_GFPATHMANAGER_H__CBC9082D_6891_4A31_862F_23BE808B1DA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGfPathManager  
{
public:
	static CString GetUserBadSiteFile();
	static void RestoreUserConfig();
	static CString GetPluginDataSys();
	static CString GetPluginData();
	static CString GetUpdateDir();
	static CString GetVersionFilePath();
	
	static CString GetResStartPageDir();
	static CString GetFavUrl();
	static CString GetFavName();
	static CString GetStartPageLeft();
	static CString GetStartPageLeftBak();
	
	static CString GetLanguageFile();
	static CString GetMyIE9StartPage(const CString& strFile);
	static CString GetUserConfig();
	static CString GetInternalVersion();

	static CString GetUpdateExeFilePath();
	static CString GetTempleLinksFolder();
	static void Init();
	static CString GetBadSiteFile();
	static CString GetNotBadSiteFileSystem();
	static CString GetNotBadSiteFileUser();

	CGfPathManager();
	virtual ~CGfPathManager();

	static CString m_strUserDir;
	static CString m_strResourceDir;
	static CString m_strRoot;
private:

	static CString GetLanguageName();


};

#endif // !defined(AFX_GFPATHMANAGER_H__CBC9082D_6891_4A31_862F_23BE808B1DA9__INCLUDED_)
