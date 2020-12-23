// Gf_Func.cpp: implementation of the Gf_Func class.
//
//////////////////////////////////////////////////////////////////////


#include "Gf_Func.h"
#include "stdafx.h"
#include "macro.h"
#include "wininet.h"		//for INTERNET_MAX_PATH_LENGTH

#include "HelperStr.h"
//
#include <sys\stat.h>	//for struct stat
#include <direct.h>		//for mkdir

//##############################################################
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//##############################################################
//file common function
char* _FileGetBuffer(CString filename)
{
	CFile f;
	if( f.Open(filename, CFile::modeRead|CFile::shareDenyNone) == FALSE)
		return NULL;
	DWORD len = f.GetLength();
	if (len<=0)
		return NULL;
	char* tmp = (char*)malloc(len+1);
	f.Read(tmp, len);
	tmp[len]=0;
	f.Close();
	return tmp;
}

//##############################################################
BOOL _FileIsExist(CString strPath)
{
	//必须这样做,因为 末尾带 \ 的 FindFirstFile stat 中不能用
	int nSlash = strPath.ReverseFind('\\');
	if( nSlash == strPath.GetLength()-1)
		strPath = strPath.Left(nSlash);
	/*
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile( strPath, &fd );
	if ( hFind != INVALID_HANDLE_VALUE )
		FindClose( hFind );
	return hFind != INVALID_HANDLE_VALUE;
	*/
	struct stat stat_buf;
	if (stat(strPath, &stat_buf) >= 0)
		return TRUE;
	else
		return FALSE;
}

void _CreateFullDir(const char *dir)
{
    struct stat stat_buf;
	char *p;
	char parent[MAX_PATH];
	int length;

	if (_FileIsExist(dir))
		return;
    if ( strlen(dir) == 0) 
		return;
	p = strchr(dir,'\\');
	while(p)
	{
		length = p-dir;
		STRNCPY(parent,dir,length);
		//make dir
		if (stat(parent, &stat_buf) < 0 && strlen(parent) > 2)//can not mkdir("c:")
		{
			if ( mkdir(parent)!=0 )
				return;
		}
		//move to next
		if (*(p+1)==0)
			return;
		p = strchr(p+1,'\\');
	}
	//for c:\\aa\bb\cc\\dd,mow create dd
	if (stat(dir, &stat_buf) < 0 && strlen(dir) > 2)//can not mkdir("c:")
	{
		if ( mkdir(dir)!=0 )
			return;
	}
}
//
////#######################################################################
////国际 URL
//#define IS_1URL(strUrl)		  ( strUrl.Right(5)==".com/" ||		\
//								strUrl.Right(5)==".net/" ||		\
//								strUrl.Right(5)==".org/" ||		\
//								strUrl.Right(5)==".edu/" ||		\
//								strUrl.Right(4)==".gov/" ||		\
//								strUrl.Right(4)==".com"  ||		\
//								strUrl.Right(4)==".net"  ||		\
//								strUrl.Right(4)==".org"  ||		\
//								strUrl.Right(4)==".gov"  ||		\
//								strUrl.Right(4)==".edu" )
////2级 URL
//#define IS_2URL(nDot,strUrl)  ( (nDot=strUrl.Find(".com."))>0 ||	\
//								(nDot=strUrl.Find(".net."))>0 ||	\
//								(nDot=strUrl.Find(".org."))>0 ||	\
//								(nDot=strUrl.Find(".gov."))>0 ||	\
//								(nDot=strUrl.Find(".edu."))>0 )
//
//##############################################################

//http://sda.baidu.com 得到http://www.baidu.com
CString _StringGetUpUrl(CString strUrl)
{
	if (!strUrl || strUrl.GetLength()<=0)
		return strUrl;
	if (strUrl=="about:blank")
		return strUrl;
	//if (strUrl.Left(6)=="res://")
	//	return strUrl;
	// must have / and \\ 
	int c,b;
	c = strUrl.Find('/');
	if (c==5 || c==6 || c==4) // file:///   http://   https:// ftp://
	{
		c = '/'; 
		b = 8; // https://
	}
	else if (strUrl.Find('\\')>=0)
	{
		c = '\\';
		if (strUrl.GetAt(1)==':')
			b = 3; // c:\a\b
		else
			b = 2; // \\server1
	}
	else
		return strUrl;
	//
	int len =  strUrl.GetLength();
	int pos1 = strUrl.Find(c,b);	//get first c
	int pos2 = strUrl.ReverseFind(c);

	if ( (pos1<0) || (pos1+1==len && b!=3) )// \\server1   \\server1\  ,must except c:\temp\ 
	{
		if (c=='/')
		{
			// http://mail.qwe.com/ , http://ror.cn/, http://mail.qwe.com.cn/
			if(	(strUrl.Left(7)=="http://" && strUrl.Left(11)!="http://www.") || 
				(strUrl.Left(8)=="https://" && strUrl.Left(12)!="https://www.")
				)
			{
				int nDot=0;
				CString strTemp;
				if ( CHelperStr::IS_1URL(strUrl) )
				{
					// http://login.bj.yahoo.com/
					nDot = strUrl.ReverseFind('.');
					strTemp = strUrl.Left(nDot);
					nDot = strTemp.ReverseFind('.');
				}
				else if ( CHelperStr::IS_2URL(nDot, strUrl) )
				{
					strTemp = strUrl.Left(nDot);
					nDot = strTemp.ReverseFind('.');
				}
				else if ( strUrl.Find('.') < strUrl.ReverseFind('.')) // http://news.sh.cn/
				{
					int pos1 = strUrl.Find("://");
					pos1 += 3;
					strTemp = strUrl.Mid(pos1);
					if (strTemp.FindOneOf("abcdefghijklmnopqrstuvwxyz")>=0) //http://127.0.0.1/
						nDot = strUrl.Find('.',pos1);
				}
				//replace str as www
				if (nDot>0)
				{
					// http://news.newscn.com/ => http://www.newscn.com/
					int pos1 = strUrl.Find("://");
					pos1 += 3;
					strTemp  = strUrl.Left(pos1);
					strTemp += "www";
					strTemp += strUrl.Mid(nDot);
					strUrl = strTemp;
				}
			}
		}
		return strUrl;
	}
	else if (pos2+1==len)// \\server1\mail\xxx\ 
	{
		strUrl = strUrl.Left(pos2);
		pos2   = strUrl.ReverseFind(c);
	}
	strUrl = strUrl.Left(pos2+1); // \\server1\mail\xxx 
	return strUrl;
}

//http://sda.baidu.com/sa/w 得到http://www.baidu.com
CString _StringGetRootUrl(CString strUrl)
{
	
	{
		CString strTmp2 = strUrl;
	CString strG= _StringGetUpUrl(strTmp2);
//	AfxMessageBox(strG);
	}

	if (!strUrl || strUrl.GetLength()<1)
		return strUrl;

	if (!IS_URL(strUrl))
		return strUrl;
	int nIndex = 0;
	CString strUrlUp;
	while(nIndex < 100)
	{
		strUrlUp = _StringGetUpUrl(strUrl);
		if (strUrlUp == strUrl)
			break;
		strUrl = strUrlUp;
		//
		nIndex++;
	}
//	AfxMessageBox("2:" + strUrl);
	return strUrl;
}

//only for http:// and https://
CString _StringGetTopUrl(CString strUrl)
{
	CString strTop;
	strTop.Empty();

	if (!IS_HTTP(strUrl))
		return strTop;
	
	int p1 = strUrl.Find("://");
	p1+=3;
	int p2 = strUrl.Find("/",p1);
	if (p2<0)
	{
		strTop = strUrl + "/";
		return strTop;
	}
	strTop = strUrl.Left(p2+1);
	return strTop;
}


//##############################################################
void _SetClipString(CString str)
{
	int len = str.GetLength();

	if ( !::OpenClipboard(NULL) )
		return;
	// Remove the current Clipboard contents
	if( !EmptyClipboard() )
		return;
	HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, len+1);
	if (hData == NULL)
		return;
	strcpy((LPSTR)GlobalLock(hData), (LPCSTR)str);
	GlobalUnlock(hData);
	// For the appropriate data formats...
	SetClipboardData( CF_TEXT, hData );
	CloseClipboard();
	//GlobalFree(hData);
}

CString _GetClipString()
{
	CString strUrl;
	strUrl.Empty();

	if ( !::OpenClipboard(NULL) )
		return strUrl;
	// For the appropriate data formats...
	HGLOBAL hData = ::GetClipboardData( CF_TEXT);
	if ( hData == NULL )
	{
		CloseClipboard();
		return strUrl;
	}
	// ...
	DWORD nSize = GlobalSize(hData);
	if (nSize > 0)
	{
		char *str = (char*)GlobalLock(hData);
		strUrl = str;
	}
	GlobalUnlock(hData);
	CloseClipboard();

	return strUrl;
}
//##############################################################



//##############################################################
char NUMS[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

#define IS_09azAZ(c)	( (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') )

//# => %23
void _CharFormat(char *p)
{
	char c;
	while(*p)
	{	
		if ( IS_09azAZ(*p) )
			p++;//do nothing
		else
		{
			c = *p;
			memmove(p+3,p+1,strlen(p+1)+1);
			*p = '%';
			*(p+1) = NUMS[(c & 0xf0)>>4];
			*(p+2) = NUMS[ c & 0x0f];
			p+=3;
		}
	}
}

void _AsciiFormat(CString& strAscii)
{
	int nLen = strAscii.GetLength();

	for (int i=0; i<nLen; i++)
	{
		if ( !IS_09azAZ(strAscii.GetAt(i)) )
			break;
	}
	if (i == nLen)
		return;//no use format

	LPSTR pAsciiBuf = strAscii.GetBuffer(nLen*3+1);
	_CharFormat(pAsciiBuf);
	strAscii.ReleaseBuffer();
}

void _Ascii2Utf8(CString& strAscii)
{
	int nAsciiLen,nTransLen,length;
	LPSTR pAsciiBuf;
	LPSTR pTransBuf;

	nAsciiLen = strAscii.GetLength();
	nTransLen = nAsciiLen*9 + 1; //3->9

	pAsciiBuf = strAscii.GetBuffer(nTransLen);
	pTransBuf = (LPSTR)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,nTransLen);

	length = MultiByteToWideChar(CP_ACP,0,pAsciiBuf,nAsciiLen, (LPWSTR)pTransBuf,nTransLen);
	length = WideCharToMultiByte(CP_UTF8,0,(LPWSTR)pTransBuf,length,   pAsciiBuf,nTransLen,NULL,NULL);
	*(pAsciiBuf+length) = 0;
	
	//# => %23
	_CharFormat(pAsciiBuf);
	strAscii.ReleaseBuffer();
	//
	GlobalFree(pTransBuf);
}
