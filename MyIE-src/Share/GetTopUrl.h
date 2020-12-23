// GetTopUrl.h: interface for the CGetTopUrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GETTOPURL_H__F34D18A6_E2B4_4D33_BCED_1D353D2A52B2__INCLUDED_)
#define AFX_GETTOPURL_H__F34D18A6_E2B4_4D33_BCED_1D353D2A52B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGetTopUrl  
{
public:
	
	static CString GetTopUrlFromLine(const CString& strLine);
	static void GetUrlFromDirToFile(const CString& strDir, const CString& strToFilePath);
	CGetTopUrl();
	virtual ~CGetTopUrl();

private:
	static bool IsStop(const CString& strLine, int iPos);
	static int GetTailPos(const CString& strLine, int iDot);
	static int GetHeadPos(const CString& strLine, int iDot);
};

#endif // !defined(AFX_GETTOPURL_H__F34D18A6_E2B4_4D33_BCED_1D353D2A52B2__INCLUDED_)
