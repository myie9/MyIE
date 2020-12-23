// HelperStr.cpp: implementation of the CHelperStr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "HelperStr.h"

#include "HelperException.h"

#include <ATLCONV.H>
#include "PathHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


static int s_iDomainsCouldWithAppend = 6;

	static CString s_strDomainsCouldWithAppend[] = {

".com",
".net",
".org",
".gov",
".org",
".edu"
	};



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHelperStr::CHelperStr()
{
	
}

CHelperStr::~CHelperStr()
{
	
}

void CHelperStr::Str2Map(const CString &strIn, CMapStringToString &mapOut, const CString& strSplit)
{
	
	int iPosBegin = 0;
	
	while (true)
	{
		int iPosEnd = strIn.Find(strSplit, iPosBegin);
		
		if (-1 == iPosEnd)
		{
			break;
		}
		
		CString strItem = strIn.Mid(iPosBegin, iPosEnd - iPosBegin);
		
		if ("" == strItem)
		{
			break;
		}
		
		mapOut.SetAt( (LPCTSTR)strItem, "");
		
		iPosBegin = iPosEnd + 2;
	}
}


CString CHelperStr::GetTopUrl(LPCTSTR lpszURL)
{
	
	CString strUrl = lpszURL;
	
	if (CHelperStr::IsNotUrl(lpszURL))
	{
		return strUrl;
	}
	
	
	{
		CString strTitle = "://";
		int iTitle = strUrl.Find(strTitle);
		
		if( -1 == iTitle)
			return strUrl;
		
		
		strUrl = strUrl.Mid(iTitle + strlen(strTitle) );
		
	}
	
	
	
	
	{
		
		int iPosSlash = strUrl.Find("/");
		if (-1 != iPosSlash)
		{
			strUrl =  strUrl.Left( iPosSlash );
		}
		
	}
	
	//remove :8080
	{
		int iMaoHao = strUrl.ReverseFind(':');
		
		if (-1 != iMaoHao)
		{
			strUrl =  strUrl.Left( iMaoHao );
		}
		
	}
	
	return strUrl;
}

CString CHelperStr::GetMainTopUrl(LPCTSTR lpszURL)
{
	CString strUrl = lpszURL;
	
	if(IsNotUrl(lpszURL) )
	{
		return strUrl;
	}
	
	strUrl = CHelperStr::GetTopUrl(lpszURL);
	
	int nDot = -1;	
	
	if (IS_2URL(nDot,strUrl))
	{
		CString strTemp = strUrl.Left(nDot - 1);
		int iTop = strTemp.ReverseFind('.' );
		
		strUrl = strUrl.Mid(iTop+1);
	}
	else
	{
		int iLast1 = strUrl.ReverseFind('.');
		
		CString strTemp = strUrl.Left(iLast1 - 1);
		
		int iLast2 = strTemp.ReverseFind('.');
		
		strUrl = strUrl.Mid(iLast2+1);
		
	}
	
	return strUrl;
	
}

BOOL CHelperStr::IsNotUrl(LPCTSTR lpszURL)
{
	CString strUrl = lpszURL;
	
	if(strUrl.Find("\\") != -1 || strUrl.Find("file://") != -1)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CHelperStr::LoadFileToMap(const CString& lpszFilePath, CMapStringToString& maps, const int& iInitMap)
{
	if (!CPathHelper::IsExist(lpszFilePath))
	{
		return;
	}

	//打开文件
	CStdioFile file;
	
	try
	{
		if (maps.GetCount() == 0)
		{
			maps.InitHashTable(iInitMap);
		}		
		
		file.Open(lpszFilePath, CFile::modeRead);
		
		CString szLine = "";
		//逐行读取字符串
		while( file.ReadString( szLine ) )
		{
			szLine.TrimRight();

			if (szLine.GetLength() > 0)
			{
				maps.SetAt(szLine, "");
			}
			
		}
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
	
	file.Close();
	
}




BOOL CHelperStr::Contains(const CString &strFull, LPCTSTR subStr)
{
	if (strFull.Find(subStr) != -1 )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

BOOL CHelperStr::IsTitleOK(const CString &strTitle)
{
	if (IsTitleBad(strTitle) \
		|| Contains(strTitle, _T("://") ) \
		\
		)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}


}

BOOL CHelperStr::IsTitleBad(const CString &strTitle)
{
	if (   Contains(strTitle, _T("没有可以显示的页面") ) \
		|| Contains(strTitle, _T("找不到服务器") ) \
		|| Contains(strTitle, _T("无法找到该页") )  \
		|| Contains(strTitle, _T("No page to display") )  \
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}


}


CString CHelperStr::GetSubStr(const CString &strIn, int iLeft, CString strAppendix)
{

	try
	{
		
		if ( (strIn.GetLength() <= iLeft) || (iLeft<=0) )
		{
			return strIn;
		}

		int nMaxLeft= iLeft-strAppendix.GetLength();

		int nEnglish=0, nOther=0;

		{

		
		for (int i=0;i<iLeft;i++)
		{
			if (IsEnglish(strIn[i]))
			{
				nEnglish++;
			} 
			else
			{
				nOther++;
			}
		}
		}

		int nRealLeft=nEnglish+nOther/2;
		
		USES_CONVERSION;
		CString strSrc = strIn;
		LPWSTR pUtfStr = A2W( (LPTSTR)(LPCTSTR)strSrc);

		LPWSTR pBeginUtfSrc = pUtfStr;

		for (int i = 0; (i < nRealLeft) && ('\0' != (*pUtfStr)); i++)
		{
			pUtfStr++;
		}

		if ('\0' == (*pUtfStr))
		{
			return strSrc;
		}

		*pUtfStr = '\0';

		pUtfStr = pBeginUtfSrc;

		char* pStr = W2A(pUtfStr);

		CString strOK = "";
		strOK.Format("%s", pStr);

		strOK += strAppendix;

		return strOK;
		
	}
	catch (...) {
		ASSERT(false);
		return strIn;
	}
	
	
}

CString CHelperStr::GetStrFromList(const CStringList& sl, int iPos)
{

	POSITION pos = sl.FindIndex(iPos);
	if (NULL == pos)
	{
		return "";
	}

	return sl.GetAt(pos);
}

//找不到begin返回"". 找不到end或end为空则返回begin后面所有的
CString CHelperStr::GetSubStr(const CString &strIn, const CString &strBegin, const CString &strEnd)
{
	CString strRet = "";
	int iBegin = strIn.Find(strBegin);

	if (-1 == iBegin)
	{
		return "";
	}

	int iEnd = strEnd.IsEmpty()?-1:strIn.Find(strEnd, iBegin + strBegin.GetLength() );

	if (-1 == iEnd)
	{
		strRet = strIn.Mid(iBegin);
	}
	else
	{
		strRet = strIn.Mid(iBegin, iEnd - iBegin);
	}

	int iB = strRet.Find(strBegin);

	return strRet.Mid(iB + strBegin.GetLength() );

}


BOOL CHelperStr::StartsWith(const CString &strIn, const CString &strSub)
{
	
	if (strSub.GetLength() > strIn.GetLength() )
	{
		return FALSE;
	}
	
	
	if (strIn.Left(strSub.GetLength() ) == strSub)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
}

BOOL CHelperStr::EndsWith(const CString &strIn, const CString &strEnds)
{
	if (strEnds.GetLength() > strIn.GetLength() )
	{
		return FALSE;
	}
	
	
	if (strIn.Right(strEnds.GetLength() ) == strEnds)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
}

bool CHelperStr::IsEnglish(const CString& strAll)
{
	for (int i = 0; i< strAll.GetLength(); i++)
	{
		if (!IsEnglish(strAll[i] ))
		{
			return false;
		}
	}

	return true;
}

bool CHelperStr::IsEnglish(char cChar)
{
	if ( (cChar > 31 ) && (cChar < 127 ))
	{
		return true;
	}
	else
	{
		return false;
	}

}

BOOL CHelperStr::IS_1URL(const CString& strUrl)
{
//	//国际 URL
//#define IS_1URL(strUrl)		  ( strUrl.Right(5)==".com/" ||		\
//	strUrl.Right(5)==".net/" ||		\
//	strUrl.Right(5)==".org/" ||		\
//	strUrl.Right(5)==".edu/" ||		\
//	strUrl.Right(4)==".cn/" ||		\
//	strUrl.Right(6)==".mobi/" ||		\
//	strUrl.Right(5)==".biz/" ||		\
//	strUrl.Right(5)==".gov/" ||		\
//	strUrl.Right(6)==".info/" ||		\
//	strUrl.Right(6)==".name/" ||		\	
//	strUrl.Right(4)==".com"  ||		\
//	strUrl.Right(4)==".net"  ||		\
//	strUrl.Right(4)==".org"  ||		\
//	strUrl.Right(3)==".cn" ||		\
//	strUrl.Right(5)==".mobi" ||		\
//	strUrl.Right(4)==".biz" ||		\
//	strUrl.Right(4)==".gov" ||		\
//	strUrl.Right(5)==".info" ||		\
//	strUrl.Right(5)==".name" ||		\	
//	strUrl.Right(4)==".edu" )

	for (int i =0; i< s_iDomainsCouldWithAppend; i++)
	{
		if ( EndsWith(strUrl, s_strDomainsCouldWithAppend[i]) )
		{
			return TRUE;
		}

		
		if ( EndsWith(strUrl, s_strDomainsCouldWithAppend[i] + "/") )
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CHelperStr::IS_2URL(int& nDot, const CString& strUrl)
{
	
//2级 URL
//#define IS_2URL(nDot,strUrl)  ( (nDot=strUrl.Find(".com."))>0 ||	\
//	(nDot=strUrl.Find(".net."))>0 ||	\
//	(nDot=strUrl.Find(".org."))>0 ||	\
//	(nDot=strUrl.Find(".gov."))>0 ||	\	
//	(nDot=strUrl.Find(".org."))>0 ||	\
//(nDot=strUrl.Find(".edu."))>0 )

	{
		ASSERT( 9 == ReverseFindStr("com.cn.as.com", ".com"));

		ASSERT( 3 == ReverseFindStr("com.cn.as.com", ".cn"));
		ASSERT( 0 == ReverseFindStr(".com.cn.as.com", ".com.cn"));
		ASSERT( -1 == ReverseFindStr("m", ".cn"));

		ASSERT( -1 == ReverseFindStr("masdaseew.dsdd", ".cn"));
	}


	for (int i =0; i< s_iDomainsCouldWithAppend; i++)
	{
		CString strToFind = s_strDomainsCouldWithAppend[i] + ".";

		nDot=ReverseFindStr(strUrl, strToFind );
		if ( nDot >0 )
		{
			int iTailDot = strUrl.Find(".", nDot + strToFind.GetLength() );

			if (iTailDot < 0 )
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}

		}
	} 

	return FALSE;

}

int CHelperStr::ReverseFindStr(const CString &strAll, const CString &strToFind)
{
	int nPos = strAll.Find(strToFind);

	while (nPos > -1)
	{
		if (nPos + strToFind.GetLength() - 1 > strAll.GetLength() -1 )
		{
			break;

		}

		int iLastPos = nPos;

		nPos = strAll.Find(strToFind, nPos + strToFind.GetLength() -1 );

		if (nPos < 0)
		{
			return iLastPos;
		}

	}

	return nPos;
}

void CHelperStr::SplitStr(CStringArray& stlList, const CString& strToSplit, const CString& strToken)
{

	try
	{
		
		for (int iToken=0; iToken<strToSplit.GetLength(); )
		{
			int iTokenPosTail = strToSplit.Find(strToken, iToken);

			if (-1 == iTokenPosTail)
			{
				CString strGot = strToSplit.Mid(iToken, strToSplit.GetLength() - iToken );
				stlList.Add(strGot);
				return;
			}

			CString strGot = strToSplit.Mid(iToken, iTokenPosTail - iToken);
			stlList.Add(strGot);
			iToken = iTokenPosTail + strToken.GetLength();
		}


	}
	catch (...) {
		ASSERT(false);
		stlList.RemoveAll();
		return;

	}
}

void CHelperStr::Trim(CString& strIn)
{
	strIn.TrimLeft();
	strIn.TrimRight();
}

CString CHelperStr::GetN( const CString &strPad, int nPad)
{
	
	if (nPad<=0)
	{
		return "";
	}

	CString tmp="";

	for (int i=0;i<nPad;i++)
	{

		tmp+=strPad;
	}
	
	return tmp;
}
