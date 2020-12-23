// Gf_UpdateControl.cpp: implementation of the CGf_UpdateControl class.
//
//////////////////////////////////////////////////////////////////////


#include <afx.h>


#include "Gf_UpdateControl.h"

#include "gf_ShareConfig.h"
#include "Gf_HelperInternet.h"

#include "HelperStr.h"
#include "PathHelper.h"

#include "GfPathManager.h"
#include <SHELLAPI.H>

#include "Gf_Language.h"
#include "Gf_Win.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_UpdateControl::CGf_UpdateControl()
{

}

CGf_UpdateControl::~CGf_UpdateControl()
{

}

BOOL CGf_UpdateControl::Execute()
{	

	

	CString strLocalFileName = CPathHelper::GetUniqueTmpFilePath();
	strLocalFileName = strLocalFileName + GF_DONWLOADFILE_EXT;

	int iRetDown = FALSE;
	{
		iRetDown = DownPatch(CGf_Win::GetFullAbbriLangID(), strLocalFileName);
	}

	if (iRetDown == FALSE)
	{
		iRetDown = DownPatch(CGf_Win::GetPrimaryLangID(), strLocalFileName);

		if (iRetDown == FALSE)
		{
			iRetDown = DownPatch("", strLocalFileName);
			
			if (iRetDown == FALSE)
			{
				return FALSE;
			}
		}
		
	}

	CString strUpdateDir = CPathHelper::GetExeDir() + GF_UPDATE_FOLDER;

	CPathHelper::CreateFullDir(strUpdateDir);

	CString strRarPath  = CPathHelper::GetExeDir() + GF_RAR_PATH;

	CString strRarParam;
	strRarParam.Format("x -o+ -y \"%s\" \"%s\"", strLocalFileName, strUpdateDir );

	HINSTANCE hiRet = ShellExecute(NULL, "open", strRarPath, strRarParam, NULL, SW_HIDE);
	
	TRACE1("\r\nrar:%d\r\n", hiRet);

	UINT iRet = (UINT)hiRet;
	ASSERT(iRet ==2);


	return iRetDown;

}

CString CGf_UpdateControl::GetLocalVersion()
{
	return CGfPathManager::GetInternalVersion();
	
}

//-1:error
BOOL CGf_UpdateControl::IsLastedVersion(BOOL bGetTestVer)
{

// 	{
// 		CString strTongji="http://img.tongji.linezing.com/1465618/tongji.gif";
// 		CGf_HelperInternet oHInet;
// 		oHInet.ReadFileContent( strTongji);
// 	}
// 	{
// 		CString strTongji="http://webimg.51.la:82/go.asp?svid=58&id=3292847&style=0&vpage=&106.312.gif";
// 		CGf_HelperInternet oHInet;
// 		oHInet.ReadFileContent( strTongji);
// 	}

	CString strNowVer = GetLocalVersion();

	CGf_HelperInternet oHInet;
	CString strNewUrl = oHInet.ReadFileContent( bGetTestVer?GF_URL_TEST_VERSION:GF_URL_VERSION);

	if (strNewUrl.IsEmpty())
	{
		strNewUrl = oHInet.ReadFileContent( bGetTestVer?GF_URL_TEST_VERSION2:GF_URL_VERSION2);
		
		if (strNewUrl.IsEmpty())
		{
			return -1;
		}
	}

	CString strVersionFromUrl = CHelperStr::GetSubStr(strNewUrl, GF_URL_VERSION_BEGIN, GF_URL_VERSION_END);

	strNowVer.TrimRight();
	strVersionFromUrl.TrimRight();

	if (strVersionFromUrl.IsEmpty())
	{
		return -1;
	}

	if ( "" != strNowVer && atoi(strNowVer) >= atoi(strVersionFromUrl) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// CString CGf_UpdateControl::GetRandomDownloadUrl()
// {
// 
// 	srand( (unsigned)time( NULL ) );
// 
// 	int i = rand() % GF_URL_DOWNLOAD_NUMBER;
// 
// 	CString strUrl;
// 	strUrl.Format(GF_URL_DOWNLOAD, i);
// 	return strUrl;
// }

BOOL CGf_UpdateControl::DownPatch(const CString& strId, const CString& strLocalFileName)
{
	int iRetDown = FALSE;
	{
		CString strUrl = "http://myie9.googlecode.com/files/pat" + strId +".rar";
		
		CGf_HelperInternet oHInet;
		iRetDown = oHInet.DownLoadHttpFile(strUrl, strLocalFileName);
	}
	
	if (iRetDown == FALSE)
	{
		CString strUrl = "http://myie9.sourceforge.net/d/u" + strId +".jpg";
		CGf_HelperInternet oHInet;
		iRetDown = oHInet.DownLoadHttpFile(strUrl, strLocalFileName);
		
		if (iRetDown == FALSE)
		{
			return FALSE;
		}
		
	}

	return TRUE;

}
