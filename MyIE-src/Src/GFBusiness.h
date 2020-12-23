// GFBusiness.h: interface for the CGFBusiness class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GFBUSINESS_H__E69D6ED1_E4A0_441A_9AA6_6D9339A19BEE__INCLUDED_)
#define AFX_GFBUSINESS_H__E69D6ED1_E4A0_441A_9AA6_6D9339A19BEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGFBusiness  
{
public:
	static BOOL IsNeedFill(const CString& strName, const CString& strFieldValue, const CString& strValue);
	static BOOL IsValidAdressList(const CString& strUrl);
	static CString GetAddressShowStr(const CString& strUrl, const CString& strTitle);
	static CString GetBlockedMsgFile(const CString& strUrl, const CString& strUrlShow);
	static BOOL IsBadSite(LPCTSTR lpszURL);
	CGFBusiness();
	virtual ~CGFBusiness();

};

#endif // !defined(AFX_GFBUSINESS_H__E69D6ED1_E4A0_441A_9AA6_6D9339A19BEE__INCLUDED_)
