

// HelperFile.cpp: implementation of the CHelperFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HelperFile.h"

#include "HelperException.h"
#include "PathHelper.h"
#include <wininet.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHelperFile::CHelperFile()
{

}

CHelperFile::~CHelperFile()
{

}

void CHelperFile::AppendLine(const CString &strFile, const CString strLine)
{
	//打开文件
	CStdioFile file;
	
	try
	{
		if (!CPathHelper::IsExist(strFile))
		{
			file.Open(strFile, CFile::modeWrite | CFile::modeCreate);
		}
		else
		{
			file.Open(strFile, CFile::modeWrite);
			
			file.SeekToEnd();
			file.WriteString( "\n" );
		}		
		
		file.WriteString( strLine );
			
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
	
	file.Close();

}

void CHelperFile::SaveMapKeyToFile(const CMapStringToString &maps, const CString &strFile)
{
		
	//打开文件
	CStdioFile file;
	
	try
	{
		
		file.Open(strFile, CFile::modeCreate | CFile::modeWrite);
		
		POSITION pos = maps.GetStartPosition();
		
		CString rKey = "";
		CString rValue;
		
		
		//逐行读取字符串
		while(NULL != pos )
		{
			maps.GetNextAssoc(pos, rKey,  rValue);
			
			file.WriteString( rKey );
			file.WriteString( "\n" );
		}
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
	
	file.Close();


}

//url1 url2
void CHelperFile::AddToStrList(const CString& path, const CString& section, const CString& key, CStringList& slIn, BOOL bUnique)
{
	
	if (!CPathHelper::IsExist(path))
	{
		//ASSERT(false);
		return;
	}
	
	TCHAR buf[INTERNET_MAX_URL_LENGTH]={0};
	DWORD rlens;
		
	int i = 1;
	BOOL r = TRUE;
	while(r)
	{
		CString strKey;
		strKey.Format("%s%d", key, i);
		rlens = ::GetPrivateProfileString(section, strKey, _T(""), buf, INTERNET_MAX_URL_LENGTH, path);
		if(rlens==0)
		{
			r=FALSE;
		}
		else
		{
			if (bUnique && slIn.Find(buf)==NULL)
			{
				slIn.AddTail(buf);
			}
			else if (!bUnique)
			{
				slIn.AddTail(buf);
			}
	
		}
		i++;
	}

}

void CHelperFile::AddToStrList(const CString& path, CStringList& slIn)
{

	if (!CPathHelper::IsExist(path))
		return;

	CStdioFile file(path, CFile::modeRead|CFile::shareDenyNone);
	CString buf;
	while(file.ReadString(buf))
	{
		if (slIn.Find(buf)==NULL)
		{
			slIn.AddTail(buf);
		}

	}

	file.Close();

}

void CHelperFile::AddToStrArray(const CString& path, CStringArray& slIn)
{
	
	if (!CPathHelper::IsExist(path))
		return;
	
	CStdioFile file(path, CFile::modeRead|CFile::shareDenyNone);
	CString buf;
	while(file.ReadString(buf))
	{
		if (!buf.IsEmpty())
		{
			slIn.Add(buf);
		}
		
	}
	
	file.Close();
	
}

void CHelperFile::Write(const CString& path, const CStringList& slIn)
{

	CStdioFile file(path, CFile::modeCreate|CFile::modeWrite);
	for (POSITION pos=slIn.GetHeadPosition(); pos!=NULL;)
	{
		file.WriteString(slIn.GetNext(pos)+"\n");
	}

	file.Close();
}

//ini 里从1开始url1 url2
void CHelperFile::Write(const CString& path, const CString& section, const CString& key, const CStringList& slIn)
{
	
	int	i = 1;

	//pop filter
	POSITION pos = slIn.GetHeadPosition();
	WritePrivateProfileSection(section, NULL, path);
	while(pos!=NULL)
	{
		CString strKey;
		strKey.Format("%s%d", key, i);
		::WritePrivateProfileString(section, strKey, slIn.GetAt(pos), path);
		slIn.GetNext(pos);
		i++;
	}

}

CString CHelperFile::Read(const CString &path)
{
	if (!CPathHelper::IsExist(path))
		return "";
	
	CString old="";
	
	CFile f;
	if (f.Open(path, CFile::modeRead|CFile::shareDenyNone))
	{
		LPTSTR lpBuf=old.GetBuffer(f.GetLength()+1);
		f.Read(lpBuf, f.GetLength());
		
		*(lpBuf+f.GetLength())='\0';
		old.ReleaseBuffer();
		f.Close();
	}
	else
	{
		ASSERT(false);
	}
	
	return old;
}

void CHelperFile::Write(const CString &path, const CString &all)
{
	CFile f;
	if (f.Open(path, CFile::modeCreate|CFile::modeWrite))
	{
		f.Write( (void*)(LPCSTR)all, all.GetLength());
		
		f.Close();
		
	}
	
}
