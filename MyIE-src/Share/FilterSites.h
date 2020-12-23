// FilterSites.h: interface for the CFilterSites class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILTERSITES_H__960D3D08_2129_454F_BE45_BCFD51E24B28__INCLUDED_)
#define AFX_FILTERSITES_H__960D3D08_2129_454F_BE45_BCFD51E24B28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFilterSites  
{
public:
	static void AddUserBadSite(const CString& strUrl);
	//static void SaveNotBadSites();
	static void AddNotBadSite(const CString& strURL);

	//for user:strNotBadSitesFile
	void Initial(const CString& strBadSitesFile, const CString& strNotBadSitesFileSystem, const CString& strNotBadSitesFileUser, const CString& strUserBadSitesFile); //not static inorder to have this pointer
	static BOOL FilterSite(const CString &strUrl); // True if is bad site and jump box
	
	CFilterSites();
	virtual ~CFilterSites();

	
private:
	
	static CMapStringToString m_mapBadSites;

	static UINT LoadBadSitesThread(LPVOID pParam);


	static BOOL IsFullUrlBadSite(const CString &strUrl); //  ≈‰3º∂
	static BOOL IsBigUrlBadSite(const CString& strUrl); //  ≈‰2º∂
	static BOOL IsGoodSite(const CString &strUrl);

	
	static BOOL IsBadSite(const CString& strUrl); //strUrl: http://www.ca.cn/

	static CString m_strBadSitesFile;
	static CString m_strNotBadSitesFile; //for user
	static CString m_strUserBadSiteFile;

	static CString m_strNotBadSitesFileSystem;


	static CMapStringToString m_mapNotBadSites; //for user
	static CMapStringToString m_mapNotBadSitesSystem;
};

#endif // !defined(AFX_FILTERSITES_H__960D3D08_2129_454F_BE45_BCFD51E24B28__INCLUDED_)
