// GetTopUrl.cpp: implementation of the CGetTopUrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "GetTopUrl.h"
#include "DiskObject.h"
#include "HelperFile.h"
#include "HelperStr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGetTopUrl::CGetTopUrl()
{
	
}

CGetTopUrl::~CGetTopUrl()
{
	
}

void CGetTopUrl::GetUrlFromDirToFile(const CString &strDir, const CString &strToFilePath)
{
	CStringArray files;
	CDiskObject oDisk;
	
	BOOL bRetFiles = oDisk.EnumAllFiles(strDir, files);
	
	for (int i = 0; i < files.GetSize(); i++)
	{
		CString strAFilePath = files.GetAt(i);
		
		CStdioFile file;
		try
		{
			file.Open(strAFilePath, CFile::modeRead);
			CString strLine ="";
			while (file.ReadString(strLine))
			{
				CString strOkSite = GetTopUrlFromLine(strLine);
				
				if ("" != strOkSite)
				{
					CHelperFile::AppendLine(strToFilePath, strOkSite);
				}
			}
			
		}
		catch (...) {
			ASSERT(false);
		}
		file.Close();
	}
}

CString CGetTopUrl::GetTopUrlFromLine(const CString &strLine)
{
	int iDot = strLine.Find(".");
	
	if ( (-1 == iDot) || (0 == iDot) )
	{
		return "";
	}
	
	int iMoveHead = GetHeadPos(strLine, iDot);
	
	int iMoveTail = GetTailPos(strLine, iDot);
	
	if (iMoveHead >= iMoveTail)
	{
		return "";
	}
	
	CString strOk = strLine.Mid(iMoveHead, iMoveTail - iMoveHead + 1);

	if (CHelperStr::Contains(strOk, "@"))
	{
		return "";
	}
	
	
	if (CHelperStr::Contains(strOk, "&"))
	{
		return "";
	}

	int iTmp;
	if (!CHelperStr::EndsWith(strOk, ".cn") && !CHelperStr::IS_1URL(strOk) && !CHelperStr::IS_2URL(iTmp, strOk) )
	{
		return "";
	}
	
	{
		CString strExcept = ".mozilla.org";
		
		ASSERT(CHelperStr::EndsWith("org", strExcept) == FALSE);
		ASSERT(CHelperStr::EndsWith("123.mozilla.org", strExcept) == TRUE);
		
		if (CHelperStr::EndsWith(strOk, strExcept) )
		{
			return "";
		}
		
	}
	
	if (CHelperStr::EndsWith(strOk, ".w3.org") )
	{
		return "";
	}

	if (CHelperStr::StartsWith(strOk, ".") )
	{
		return "";
	}

	
	if (CHelperStr::EndsWith(strOk, ".taobao.com") )
	{
		return "";
	}
	
	return strOk;
	
}

bool CGetTopUrl::IsStop(const CString &strLine, int iPos)
{
	char cChar = strLine[iPos];

	bool bIsEnglish = CHelperStr::IsEnglish(cChar);

	return ( !bIsEnglish || (' ' == cChar) || (':' == cChar) || ('"' == cChar)  || (';' == cChar) || ('?' == cChar) || ('<' == cChar) || ('!' == cChar)|| ('/' == cChar) || ('>' == cChar) );
}

int CGetTopUrl::GetHeadPos(const CString &strLine, int iDot)
{

	int iMoveHead = iDot - 1;

	while( (iMoveHead >= 0) && !IsStop(strLine, iMoveHead) )
	{
		iMoveHead--;
	}

	return iMoveHead+1;
}

int CGetTopUrl::GetTailPos(const CString &strLine, int iDot)
{


	int iMoveHead = iDot + 1;

	while( (iMoveHead <= strLine.GetLength() - 1) && !IsStop(strLine, iMoveHead) )
	{
		iMoveHead++;
	}

	return iMoveHead-1;
}
