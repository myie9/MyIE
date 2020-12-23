// Gf_StartupManager.h: interface for the CGf_StartupManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_STARTUPMANAGER_H__802BE15B_8216_45A2_A1B8_E3E2B05B93DA__INCLUDED_)
#define AFX_GF_STARTUPMANAGER_H__802BE15B_8216_45A2_A1B8_E3E2B05B93DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


enum enmStartupMode{
	STARTUP_BLANK=0,
	STARTUP_IE=1,
	STARTUP_LAST_JUMP=2,
	STARTUP_LAST=3,
	STARTUP_FAV=4,
	STARTUP_GROUP=5,
	STARTUP_NONE=6,
	STARTUP_URL=7,
	STARTUP_MYIE9_START=8,
};

class CGf_StartupManager  
{
public:
	
	static BOOL SaveStartURL(const CString& strUrl, const CString &strIniFile);
	static BOOL SaveStartMode(int nMode, const CString& strFilename);
	static CString GetStartupUrlStr( const CString &strIniFile);
	static BOOL GetStartURLs(CStringArray& saUrl, const CString &strIniFile);
	static int GetStartMode( const CString &strIniFile);
	CGf_StartupManager();
	virtual ~CGf_StartupManager();

};

#endif // !defined(AFX_GF_STARTUPMANAGER_H__802BE15B_8216_45A2_A1B8_E3E2B05B93DA__INCLUDED_)
