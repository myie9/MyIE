// Gf_IE.h: interface for the CGf_IE class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_IE_H__BFE29B4F_5449_4879_BD5D_42A81A020389__INCLUDED_)
#define AFX_GF_IE_H__BFE29B4F_5449_4879_BD5D_42A81A020389__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGf_IE  
{
public:
	static CString GetIEHomePage();
	static CString GetIEFilePath();
	static void ClearPasswords();
	static void DeleteAutoForms();

//	enum DelTempType
//	{
//		CACHE,
//		COOKIE,
//		ALL,
// 	};

	static int FindFavLinkIcon(CString strLinkFullPath, CImageList* pImg, int nIconDefault);
	static int FindCacheFavIcon(CString strRoot, CImageList* pImg); //gf
	static CString GetCacheFilePath(CString strUrl);

	static void DelTempFilesInThread();

	static BOOL DelTempFiles(DWORD CacheType, BOOL op = TRUE);


	static UINT DelTempFilesThreadApi(LPVOID param);
	


private:

	CGf_IE();
	virtual ~CGf_IE();


	enum DEL_CACHE_TYPE //要删除的类型。
	 {
	 File,//表示internet临时文件
	 Cookie //表示Cookie
	 };

//以下为一些，辅助函数.
  static BOOL DeleteUrlCache(DEL_CACHE_TYPE type);
  static BOOL EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni = FALSE,   BOOL bWipeIndexDat = FALSE);
  static BOOL IsWindowsNT();//判断系统
  static BOOL IsWindows2k();
  static BOOL GetUserSid(PSID* ppSid);
  static void GetSidString(PSID pSid, LPTSTR szBuffer);
  static BOOL GetOldSD(HKEY hKey, LPCTSTR pszSubKey, BYTE** pSD);
  static BOOL CreateNewSD(PSID pSid, SECURITY_DESCRIPTOR* pSD, PACL* ppDacl);
  static BOOL RegSetPrivilege(HKEY hKey, LPCTSTR pszSubKey, SECURITY_DESCRIPTOR* pSD, BOOL bRecursive);
  static BOOL WipeFile(LPCTSTR szDir, LPCTSTR szFile);


};

#endif // !defined(AFX_GF_IE_H__BFE29B4F_5449_4879_BD5D_42A81A020389__INCLUDED_)
