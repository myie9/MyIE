// HelperStr.h: interface for the CHelperStr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELPERSTR_H__CD606BAC_3F05_4C9F_9448_EE1E54215F8A__INCLUDED_)
#define AFX_HELPERSTR_H__CD606BAC_3F05_4C9F_9448_EE1E54215F8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#######################################################################



class CHelperStr  
{
public:
	static CString GetN(const CString& strPad, int nPad);
	static void Trim(CString& strIn);
	static void SplitStr(CStringArray& stlList, const CString& strToSplit, const CString& strToken);
	static int ReverseFindStr(const CString& strAll, const CString& strToFind);
	static bool IsEnglish(char cChar);
	static bool IsEnglish(const CString& strAll);
	static BOOL EndsWith(const CString& strIn, const CString& strEnds);
	static CString GetSubStr(const CString& strIn, const CString& strBegin, const CString& strEnd);
	static BOOL IsTitleBad(const CString &strTitle);
	static CString GetStrFromList(const CStringList& sl, int iPos);
	static CString GetSubStr(const CString& strIn, int iSubNum, CString strAppendix = _T("...") );
	static BOOL IsTitleOK(const CString &strTitle);
	static BOOL Contains(const CString& strFull, LPCTSTR subStr);

// 	static void GetTopAndMainURL(LPCTSTR lpszURL, CString& strMainTopUrl, CString& strTopUrl);

	
	//input: www.google.com or google.com ; return: google.com
	// http://login.bj.yahoo.com/  => yahoo.com
	static CString GetMainTopUrl(LPCTSTR lpszURL);

	static BOOL StartsWith(const CString &strIn, const CString &strSub); 

	static void Str2Map(const CString& strIn, CMapStringToString& mapOut, const CString& strSplit = "\r\n");

	CHelperStr();
	virtual ~CHelperStr();

	static CString GetTopUrl(LPCTSTR lpszURL);
	static void LoadFileToMap(const CString& lpszFilePath, CMapStringToString& maps, const int& iInitMap = 10);

	static BOOL IS_1URL(const CString& strUrl);
	static BOOL IS_2URL(int& nDot, const CString& strUrl);


private:
	static BOOL IsNotUrl(LPCTSTR lpszURL);
};

#endif // !defined(AFX_HELPERSTR_H__CD606BAC_3F05_4C9F_9448_EE1E54215F8A__INCLUDED_)
