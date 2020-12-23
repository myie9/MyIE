// SimpleSAH.cpp: implementation of the CSimpleSAH class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyIE9.h"
#include "SimpleSAH.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//#pragma optimize( "s", on )

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimpleSAH::CSimpleSAH()
{

}

CSimpleSAH::~CSimpleSAH()
{
	file.Close();
}

//tag should be lowercase
BOOL CSimpleSAH::GetNextTags(CStringList& astrTagNames)
{
	int i, j, k;
	BOOL found = FALSE;
	CString tagp;
	long len = strHTML.GetLength();

	try{
	while(!found)
	{
		if(!strCurTagBuf.IsEmpty())
		{
			if(m_bUpdate)
				file.WriteString(strCurTagBuf);
		}

		i = strHTML.Find('<', nStartPoint);
		if(i<0)
		{
			//end of file
			if(m_bUpdate)
			{
				file.Write(strHTML.GetBuffer(8) + nStartPoint, len - nStartPoint);
				strHTML.ReleaseBuffer();
			}
			return FALSE;
		}
		else
		{
			if(m_bUpdate)
			{
				file.Write(strHTML.GetBuffer(8) + nStartPoint, i - nStartPoint);
				strHTML.ReleaseBuffer();
			}
		}
		if(strHTML.Mid(i+1, 2) == "!-") //is comment
		{
			j = strHTML.Find("-->", i);
			if(j<0)
			{
				//not a real comment, just some text
				if(m_bUpdate)
				{
					file.Write(strHTML.GetBuffer(8) + i, 3);
					strHTML.ReleaseBuffer();
				}
				nStartPoint = i + 3;
			}
			else
			{
				//is comment, output it
				if(m_bUpdate)
				{
					file.Write(strHTML.GetBuffer(8) + i, j - i+3);
					strHTML.ReleaseBuffer();
				}	
				nStartPoint = j + 3;
			}
			strCurTagBuf.Empty();
		}
		else
		{		
			j = strHTML.Find('>', i);
			k = strHTML.Find('<', i);

			if(j<0)
			{
				//end of file
				if(m_bUpdate)
				{
					file.Write(strHTML.GetBuffer(8) + i, len - i);
					strHTML.ReleaseBuffer();
				}
				return FALSE;
			}

			if(j>k)
			{
				//output
				if(m_bUpdate)
				{
					file.Write(strHTML.GetBuffer(8) + i, k-i);
					strHTML.ReleaseBuffer();
				}
				i = k;
			}
			strCurTagBuf = strHTML.Mid(i, j-i+1);
			nStartPoint = j+1;
			
			//if is the needed tag
			k=strCurTagBuf.FindOneOf(" >\r\n");
			if(k>0)
				tagp = strCurTagBuf.Mid(1, k-1);
			else
				tagp = strCurTagBuf.Mid(1, strCurTagBuf.GetLength()-2);
			tagp.MakeLower();
			if(astrTagNames.Find(tagp))
			{
				//find the tag;
				found = TRUE;
			}
		}
	}
	}catch(...){}

	return found;
}

CString CSimpleSAH::GetAttribute(LPCSTR lpAttriName)
{
	CString tmp;
	CString tag(strCurTagBuf);
	tag.MakeLower();
	CString attri(lpAttriName);
	attri.MakeLower();

	try{
	CString s;
	int i=0, j=0;
	BOOL found = FALSE;
	while(!found)
	{
		i = tag.Find(attri, i);
		if(i<0)
			break;

		j = tag.Find('=', i);
		s = tag.Mid(i+attri.GetLength(), j-i-attri.GetLength());
		s.TrimLeft();
		if(s.IsEmpty())
		{
			found=TRUE;
			tmp = strCurTagBuf.Mid(j+1);
			if(tmp.GetAt(0) == '\"' || tmp.GetAt(0) == '\'') //quoted string
			{
				tmp = tmp.Right(tmp.GetLength()-1);
				i = tmp.FindOneOf("\'\"\r\n>");
			}
			else
				i = tmp.FindOneOf(" \r\n>\'\"");
			tmp = tmp.Left(i);
			tmp.TrimLeft();
			tmp.TrimRight();
		}
		i += attri.GetLength();
	}
	}catch(...){}

	return tmp;
}

BOOL CSimpleSAH::SetAttribute(LPCSTR lpAttriName, LPCSTR lpNewVal)
{
	CString tmp;
	CString tag(strCurTagBuf);
	tag.MakeLower();
	CString attri(lpAttriName);
	attri.MakeLower();

	BOOL found = FALSE;
	try{
	CString s;
	int i=0, j=0;
	while(!found)
	{
		i = tag.Find(attri, i);
		if(i<0)
			break;

		j = tag.Find('=', i);
		s = tag.Mid(i+attri.GetLength(), j-i-attri.GetLength());
		s.TrimLeft();
		if(s.IsEmpty())
		{
			found=TRUE;
			tmp = strCurTagBuf.Mid(j+1); tmp.TrimLeft();
			tag = strCurTagBuf.Left(j+1); //XXX=
			tag += "\"";
			tag += lpNewVal;
			tag += "\"";  //XXX="VVV" 

			if(tmp.GetAt(0) == '\"' || tmp.GetAt(0) == '\'') //quated string
			{
				tmp = tmp.Right(tmp.GetLength()-1);
				i = tmp.FindOneOf("\'\"\r\n>");
				if(tmp.GetAt(i) == '\"' || tmp.GetAt(i) == '\'')
					i++;
			}
			else
				i = tmp.FindOneOf(" \r\n>\'\"");
			tag += strCurTagBuf.Right(tmp.GetLength() - i);
			strCurTagBuf = tag;
		}
		i += attri.GetLength();
	}
	}catch(...){}

	return found;
}

BOOL CSimpleSAH::LoadHTML(LPCSTR filename, BOOL bUpdate)
{
	try{
	CString tmp;
	file.Open(filename, CFile::modeRead | CFile::shareDenyWrite);
	while(file.ReadString(tmp))
	{
		strHTML += tmp;
		strHTML += "\n";
	}
	file.Close();
	if(bUpdate)
		file.Open(filename, CFile::modeCreate | CFile::modeWrite);
	m_bUpdate = bUpdate;
	nStartPoint = 0;
	}catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

void CSimpleSAH::Close()
{
	file.Close();
	strHTML.Empty();
	strCurTagBuf.Empty();
}

BOOL CSimpleSAH::AddText(CString& text)
{
	strHTML = text + strHTML;
	return TRUE;
}

//#pragma optimize( "s", off )
