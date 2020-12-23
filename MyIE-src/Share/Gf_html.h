// Gf_html.h: interface for the CGf_html class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_HTML_H__1DC2020B_3C76_42AA_8D07_984886D78285__INCLUDED_)
#define AFX_GF_HTML_H__1DC2020B_3C76_42AA_8D07_984886D78285__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mshtml.h>


class CGf_html  
{
public:
	enum
	{
		GF_FLASH_TYPE=2
	};
	
	static CString GetFullUrl(const CString& strLocationURL, const CString& strUrl );
	static BOOL IsRelativeUrl(const CString& strUrl);
	static BOOL IsPointImage(IWebBrowser2 *pWB, const POINT &pt, CString& strUrl, BOOL bHide = FALSE);
	static BOOL IsPointImage(IWebBrowser2 *pWB, const POINT &pt);

	//out:ppElement that mouse click on pt in pWB
	static HRESULT HitTest( IWebBrowser2 *pWB, const POINT &pt, IHTMLElement **ppElement );


private:
	CGf_html();
	virtual ~CGf_html();

};

#endif // !defined(AFX_GF_HTML_H__1DC2020B_3C76_42AA_8D07_984886D78285__INCLUDED_)
