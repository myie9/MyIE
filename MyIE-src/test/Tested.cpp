// Tested.cpp: implementation of the Tested class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "test.h"
#include "Tested.h"

#include "HelperStr.h"

#include "HelperException.h"
#include "Gf_ShareConfig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Tested::Tested()
{

}

Tested::~Tested()
{

}

void Tested::Execute()
{

	{

			Tested::TestGetURL();

    CString strRaw = "D:\\rawNotBadSites.txt";

	CString toFile = "D:\\notBadSites.txt";
	ChuliNotBadSites(strRaw, toFile);

 	}

		//
	{

			Tested::TestGetURL();

    CString strRaw = "D:\\rawbadSites.txt";

	CString toFile = "D:\\MyIE9\\发布\\最新\\MyIE9Data\\Resource\\b001.wav";

	CString strNotBad = "D:\\notBadSites.txt";

	ChuliBadSites(strRaw, strNotBad, toFile);

 	}
//


	TestSubStr();

	TestSiteFilter();

	
}

void Tested::TestGetURL()
{
	ASSERT(CHelperStr::GetMainTopUrl("http://www.huawei.com:8080/") == "huawei.com");
	ASSERT(CHelperStr::GetMainTopUrl("http://www.huawei.com:8080") == "huawei.com");
	ASSERT(CHelperStr::GetMainTopUrl("http://www.huawei.com.cn:8080/") == "huawei.com.cn");

	ASSERT(CHelperStr::GetMainTopUrl("http://www.huawei.com") == "huawei.com");

	ASSERT(CHelperStr::GetMainTopUrl("http://121.23.34.53") == "34.53");

	ASSERT(CHelperStr::GetMainTopUrl("http://sasad") == "sasad");

	ASSERT(CHelperStr::GetMainTopUrl("http://www.huawei.com.cn") == "huawei.com.cn");

	{
			ASSERT(CHelperStr::GetMainTopUrl("http://www.huawei.com/") == "huawei.com");

	ASSERT(CHelperStr::GetMainTopUrl("http://121.23.34.53/") == "34.53");

	ASSERT(CHelperStr::GetMainTopUrl("http://sasad/") == "sasad");

	ASSERT(CHelperStr::GetMainTopUrl("http://www.huawei.com.cn/") == "huawei.com.cn");

	}

	{
			ASSERT(CHelperStr::GetMainTopUrl("http://www.huawei.com/asd/asdsa/adw.html") == "huawei.com");

	ASSERT(CHelperStr::GetMainTopUrl("http://121.23.34.53/assa/aw") == "34.53");

	ASSERT(CHelperStr::GetMainTopUrl("http://sasad/adw/wq") == "sasad");

	ASSERT(CHelperStr::GetMainTopUrl("http://www.huawei.com.cn/acomasa.acn/as.com.cn") == "huawei.com.cn");

	}

		ASSERT(CHelperStr::GetMainTopUrl("ftp://www.huawei.com") == "huawei.com");

	ASSERT(CHelperStr::GetMainTopUrl("https://121.23.34.53") == "34.53");
	{
			ASSERT(CHelperStr::GetMainTopUrl("https://www.huawei.com") == "huawei.com");

	ASSERT(CHelperStr::GetMainTopUrl("https://121.23.34.53") == "34.53");

	ASSERT(CHelperStr::GetMainTopUrl("https://sasad") == "sasad");

	ASSERT(CHelperStr::GetMainTopUrl("https://www.huawei.com.cn") == "huawei.com.cn");

	}

	{
			ASSERT(CHelperStr::GetMainTopUrl("\\www.huawei.com") == "\\www.huawei.com");

	ASSERT(CHelperStr::GetMainTopUrl("file:///C:") == "file:///C:");

	ASSERT(CHelperStr::GetMainTopUrl("\\\\sasad\\ds") == "\\\\sasad\\ds");


	}

	{
					ASSERT(CHelperStr::GetMainTopUrl("www.huawei.cn") == "huawei.cn");
					ASSERT(CHelperStr::GetMainTopUrl("www.com.cn.huawei.cn") == "huawei.cn");
					ASSERT(CHelperStr::GetMainTopUrl("huawei.cn") == "huawei.cn");
	}

}

//from to bText
void Tested::ChuliBadSites(CString m_strBadSitesFile, CString strNotBadSitesFile, CString strToOKFile, BOOL bIsText)
{

	CMapStringToString mapNotBadSites;
	{
		CHelperStr::LoadFileToMap(strNotBadSitesFile, mapNotBadSites, 10000);
	}

	static CMapStringToString m_mapBadSites;

		//打开文件
	CStdioFile file;

	try
	{
		m_mapBadSites.InitHashTable(300);

		file.Open(m_strBadSitesFile, CFile::modeRead);

		CString szLine = "";
		//逐行读取字符串
		while( file.ReadString( szLine ) )
		{
			szLine = CHelperStr::GetMainTopUrl(szLine);

			
			LPCTSTR rKey;
			if (!(mapNotBadSites.LookupKey(szLine, rKey) ) )
			{
				m_mapBadSites.SetAt(szLine, "");
			}
			else
			{
			//	ASSERT(false); //ok test
			}
			
		}

	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}

	file.Close();

	{

	
//	try
//	{
		if (bIsText)
		{
			file.Open( strToOKFile, CFile::modeCreate| CFile::modeWrite| CFile::typeText);
		}
		else
		{
			file.Open( strToOKFile, CFile::modeCreate| CFile::modeWrite| CFile::typeBinary);
		}
		

		POSITION pos = m_mapBadSites.GetStartPosition();

		while (pos != NULL)
		{
			CString strKey, strValue;
			m_mapBadSites.GetNextAssoc(pos, strKey, strValue);

			if (strKey != "")
			{
				strKey.TrimLeft();
				strKey.TrimRight();
				file.WriteString(strKey + "\n");
			}
			
		}
		
//	}
//	catch (...) {
//		CHelperException::ErrorOcurred();
// 	}

	file.Close();

	}

}

//from to bText
void Tested::ChuliNotBadSites(CString m_strBadSitesFile, CString strToOKFile, BOOL bIsText)
{

	static CMapStringToString m_mapBadSites;


		//打开文件
	CStdioFile file;

	try
	{
		m_mapBadSites.InitHashTable(300);

		file.Open(m_strBadSitesFile, CFile::modeRead);

		CString szLine = "";
		//逐行读取字符串
		while( file.ReadString( szLine ) )
		{
			szLine = CHelperStr::GetMainTopUrl(szLine);
			m_mapBadSites.SetAt(szLine, "");
		}

	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}

	file.Close();

	{

	
//	try
//	{
		if (bIsText)
		{
			file.Open( strToOKFile, CFile::modeCreate| CFile::modeWrite| CFile::typeText);
		}
		else
		{
			file.Open( strToOKFile, CFile::modeCreate| CFile::modeWrite| CFile::typeBinary);
		}
		

		POSITION pos = m_mapBadSites.GetStartPosition();

		while (pos != NULL)
		{
			CString strKey, strValue;
			m_mapBadSites.GetNextAssoc(pos, strKey, strValue);

			if (strKey != "")
			{
				strKey.TrimLeft();
				strKey.TrimRight();
				file.WriteString(strKey + "\n");
			}
			
		}
		
//	}
//	catch (...) {
//		CHelperException::ErrorOcurred();
// 	}

	file.Close();

	}

}


#include "FilterSites.h"

void Tested::TestSiteFilter()
{
		CString m_strBadSitesFile = _T("D:\\MyIE9\\发布\\最新\\MyIE9Data\\Resource\\b001.wav");

		CString m_strNotBadSitesFile = _T("D:\\MyIE9\\发布\\最新\\MyIE9Data\\Resource\\n001.wav");

		CString m_strNotBadSitesFileUser = _T("D:\\MyIE9\\发布\\最新\\MyIE9Data\\Resource\\n001.wav");

	CFilterSites oF;
	oF.Initial(m_strBadSitesFile, m_strNotBadSitesFile, m_strNotBadSitesFileUser);

	Sleep(200);

	ASSERT(CFilterSites::FilterSite("http://www-taobaovip.cn/") == TRUE);
	ASSERT(CFilterSites::FilterSite("http://www.baidu.com/") == FALSE);
	
	ASSERT(CFilterSites::FilterSite("http://bar.baidu.com/") == FALSE);
}

void Tested::TestSubStr()
{
	{
		CString strIn = "http://dfa.com/version/v1/";

		CString strSub = CHelperStr::GetSubStr(strIn, GF_URL_VERSION_BEGIN, GF_URL_VERSION_END);
	
		ASSERT(strSub == "1");
	}
	
	
	{
		CString strIn = "http://dfa.com/version/v11/";

		CString strSub = CHelperStr::GetSubStr(strIn, GF_URL_VERSION_BEGIN, GF_URL_VERSION_END);
	
		ASSERT(strSub == "11");
	}

	
	{
		CString strIn = "http://dfa.com/version/1/";

		CString strSub = CHelperStr::GetSubStr(strIn, "version/", GF_URL_VERSION_END);
	
		ASSERT(strSub == "1");
	}

}
