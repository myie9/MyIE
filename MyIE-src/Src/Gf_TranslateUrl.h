// CGf_TranslateUrl.h: interface for the CGf_TranslateUrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGf_TranslateUrl_H__B20CFA23_FD49_45C1_B3A2_0F2D25D33EAE__INCLUDED_)
#define AFX_CGf_TranslateUrl_H__B20CFA23_FD49_45C1_B3A2_0F2D25D33EAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGf_TranslateUrl  
{
public:
	CGf_TranslateUrl();
	virtual ~CGf_TranslateUrl();

	
	static CString DealUrlToShowToUser(const CString & strUrlIn);
	static CString TranslatedToMy(const CString& strUrlIn);

};

#endif // !defined(AFX_CGf_TranslateUrl_H__B20CFA23_FD49_45C1_B3A2_0F2D25D33EAE__INCLUDED_)
