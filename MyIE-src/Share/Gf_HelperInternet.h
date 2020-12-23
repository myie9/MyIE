// Gf_HelperInternet.h: interface for the CGf_HelperInternet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_HELPERINTERNET_H__0B058758_9EC1_4D55_8B98_250ACAE91836__INCLUDED_)
#define AFX_GF_HELPERINTERNET_H__0B058758_9EC1_4D55_8B98_250ACAE91836__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGf_HelperInternet  
{
public:
	CString GetRedirectedUrl(const CString& strUrl);
	int DownLoadHttpFile(const CString& strUrl, const CString& strLocalFilePath);
	CGf_HelperInternet();
	CString ReadFileContent(const CString &strUrl);
	virtual ~CGf_HelperInternet();

private:
	BOOL _bReadContent;
	CString _strReadContent;
	CString _strNewUrl;
	BOOL _bJustGetRedirectUrl;
	int DoCommunicate();

	LPCTSTR _pszURL;

	CString _strLocalFileName;
};

#endif // !defined(AFX_GF_HELPERINTERNET_H__0B058758_9EC1_4D55_8B98_250ACAE91836__INCLUDED_)
