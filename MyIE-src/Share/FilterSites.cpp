// FilterSites.cpp: implementation of the CFilterSites class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "FilterSites.h"

#include "HelperException.h"

#include "helperStr.h"

#include "Consts_gf.h"

#include "HelperFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CString CFilterSites::m_strBadSitesFile = "";
CString CFilterSites::m_strNotBadSitesFile = "";
CString CFilterSites::m_strNotBadSitesFileSystem = "";
CString CFilterSites::m_strUserBadSiteFile = "";

CMapStringToString CFilterSites::m_mapBadSites;

CMapStringToString CFilterSites::m_mapNotBadSites;

CMapStringToString CFilterSites::m_mapNotBadSitesSystem;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilterSites::CFilterSites()
{	

}

CFilterSites::~CFilterSites()
{

}

BOOL CFilterSites::IsBadSite(const CString& strUrl)
{

	CString strPure = CHelperStr::GetTopUrl(strUrl); //取得bar.baidu.com
	
	if (strPure.IsEmpty())
	{
		return FALSE;
	}

	if (CFilterSites::IsFullUrlBadSite(strPure) )
	{
		return TRUE;
	}

	CString strLevel2Url = CHelperStr::GetMainTopUrl(strUrl); //  google.com

	//2 level
	if (CFilterSites::IsGoodSite(strLevel2Url))
	{
		return FALSE;
	}

	//2 level
	if (CFilterSites::IsBigUrlBadSite(strLevel2Url) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CFilterSites::Initial(const CString& strBadSitesFile, const CString& strNotBadSitesFileSystem, const CString& strNotBadSitesFileUser, const CString& strUserBadSitesFile )
{
	CFilterSites::m_strBadSitesFile = strBadSitesFile;
	CFilterSites::m_strNotBadSitesFile = strNotBadSitesFileUser;

	CFilterSites::m_strNotBadSitesFileSystem = strNotBadSitesFileSystem;
	CFilterSites::m_strUserBadSiteFile = strUserBadSitesFile;
	//StartThread
	CWinThread* pThread = ::AfxBeginThread(LoadBadSitesThread, this);
}

BOOL CFilterSites::FilterSite(const CString &strUrl) 
{
	return IsBadSite(strUrl);
}

UINT CFilterSites::LoadBadSitesThread(LPVOID pParam)
{

	CHelperStr::LoadFileToMap(m_strNotBadSitesFile, m_mapNotBadSites);

	CHelperStr::LoadFileToMap(m_strNotBadSitesFileSystem, m_mapNotBadSitesSystem, GF_INIT_SIZE_MAP_NOTBAD);
	m_mapNotBadSitesSystem.SetAt("about:blank", "");

	CHelperStr::LoadFileToMap(m_strBadSitesFile, m_mapBadSites, GF_INIT_SIZE_MAP_BAD);
	CHelperStr::LoadFileToMap(m_strUserBadSiteFile, m_mapBadSites);


	return 0;
}


BOOL CFilterSites::IsGoodSite(const CString &strUrl) //只验证二级
{	

	LPCTSTR rKey;
	if(m_mapNotBadSites.LookupKey(strUrl, rKey) || m_mapNotBadSitesSystem.LookupKey(strUrl, rKey))
		return TRUE;
	else
		return FALSE;

}


BOOL CFilterSites::IsBigUrlBadSite(const CString &strUrl)
{
    LPCTSTR rKey;
	if(m_mapBadSites.LookupKey(strUrl, rKey) )
		return TRUE;
	else
		return FALSE;
}

BOOL CFilterSites::IsFullUrlBadSite(const CString &strUrl)
{
	if( -1 == CConsts::_saFullUrlBadSites.Find(strUrl) )
	{
		return FALSE; 
	}
	else
	{
		return TRUE;
	}
}

void CFilterSites::AddNotBadSite(const CString &strURL)
{
	CString strMainUrl = CHelperStr::GetMainTopUrl(strURL);
	strMainUrl.MakeLower();
	m_mapNotBadSites.SetAt( strMainUrl, "");
	
	CHelperFile::AppendLine(m_strNotBadSitesFile, strMainUrl);

}

//void CFilterSites::SaveNotBadSites()
//{
//	CHelperStr::SaveMapKeyToFile(m_mapNotBadSites, m_strNotBadSitesFile);
//
// }

void CFilterSites::AddUserBadSite(const CString &strUrl)
{
	CString strMainUrl = CHelperStr::GetMainTopUrl(strUrl);
	strMainUrl.MakeLower();

	strMainUrl.TrimLeft();

	if (strMainUrl.IsEmpty())
	{
		return;
	}

	m_mapBadSites.SetAt(strMainUrl, "");
	CHelperFile::AppendLine(m_strUserBadSiteFile, strMainUrl);
}
