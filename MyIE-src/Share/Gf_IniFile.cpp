// Gf_IniFile.cpp: implementation of the CGf_IniFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "MyIE9.h"
#include "Gf_IniFile.h"
#include "PathHelper.h"
#include "DiskObject.h"
#include "RegHelper.h"
#include "urldecode.h"
#include "aes.h"
#include "gf_func.h"
#include "Gf_Win.h"
#include "macro.h"

#include <WININET.H>

//#include "macro.h"
// #include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//reserved section name: __MyIE9开头

#define ENC_NOENC_FLAG -1
#define MYIE9_NOENC_LEN_APPENDIX "__MyIE9NoEncLen"


#define MYIE9_ENC_APPENDIX "__MyIE9EncLen"

#define ENC_BUFFER_LEN 2048
#define ENC_GROUP_LEN 16

//以下2个值决定当前加密的密钥，无特别含义。 历史密钥留着，解析老数据时用
#define ENC_YES_FLAG 0x01
#define ENC_AES_FLAG 0x01

#define ENC_HEAD_LEN 3

#define ENC_YES_FLAG_POS 0
#define ENC_AES_FLAG_POS 1
#define ENC_DATA_PAD_LEN_POS 2



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_IniFile::CGf_IniFile()
{

}

CGf_IniFile::~CGf_IniFile()
{

}

BOOL CGf_IniFile::IsExistSection(const CString &section, const CString &filepath)
{
	char buf[20];
	if (GetPrivateProfileSection(section, buf, 20, filepath) > 0)
	{
		return TRUE;
	}

	return FALSE;

}

BOOL CGf_IniFile::IsEncValue(const CString &base, const CString &strName, const CString &filename)
{

	DWORD ret =::GetPrivateProfileInt(base, strName + MYIE9_ENC_APPENDIX, 0, filename);
	if (ret>0 || ENC_NOENC_FLAG == ret)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CGf_IniFile::Decode(CString &strValue, const CString &base, const CString &strName, const CString &filename)
{
	if (ENC_NOENC_FLAG == ::GetPrivateProfileInt(base, strName+MYIE9_ENC_APPENDIX, 0, filename))
	{
		UINT nAllDataLen=::GetPrivateProfileInt(base, strName+MYIE9_NOENC_LEN_APPENDIX, ENC_BUFFER_LEN, filename);

		int nBuf=nAllDataLen+1; //last is \0
		
		DWORD dwret=::GetPrivateProfileString(base, strName, _T(""), strValue.GetBuffer(nBuf), nBuf, filename);
		strValue.ReleaseBuffer(); //update the string struct
		return (dwret>0?TRUE:FALSE);
	}

	char encbuf[ENC_BUFFER_LEN]={'\0'
	};

	UINT nAllDataLen = ::GetPrivateProfileInt(base, strName + MYIE9_ENC_APPENDIX, 0, filename);

	BOOL bGetEncData = GetPrivateProfileStruct(base, strName, encbuf, nAllDataLen, filename);

	if (!bGetEncData || nAllDataLen==0)
	{
		return FALSE;
	}


	CString strMiYao = GetPCIDMiYao(encbuf[0], encbuf[1]); //more than 256bit
	AES myenc(AES::BIT256, (BYTE*)(LPTSTR)(LPCTSTR)strMiYao);

	BYTE decbuf[ENC_BUFFER_LEN];
	memset(decbuf, '\0', ENC_BUFFER_LEN);


	int padLen = encbuf[ENC_DATA_PAD_LEN_POS];

	for (size_t pos=ENC_HEAD_LEN, despos=0; pos<nAllDataLen; pos+=ENC_GROUP_LEN, despos+=ENC_GROUP_LEN)
	{
		myenc.InvCipher( (BYTE*)(encbuf+pos), decbuf+despos);
	}

	decbuf[strlen( (const char*)decbuf) - padLen] = '\0';

	//
	char strBuf[ENC_BUFFER_LEN]={'\0'
	};

	URL_UTF8_Decode( (const char*)decbuf, strBuf);
	strValue.Format("%s", strBuf);
	


	return TRUE;

}

/************************************************************************/
/* strValue长度超过时，不加密，可全部存储，到时可全部读取
/************************************************************************/
BOOL CGf_IniFile::EncAndWriteString(const CString &base, const CString &strName, const CString &strValue, const CString &filename, BYTE btEncType0, BYTE btEncType1)
{
	CString strCvt = strValue;
	_Ascii2Utf8(strCvt);

	if (! (strlen( (LPCTSTR)strCvt) + ENC_HEAD_LEN < ENC_BUFFER_LEN))
	{
		CString strNoEnc;
		strNoEnc.Format("%d", ENC_NOENC_FLAG);
		::WritePrivateProfileString(base, strName+MYIE9_ENC_APPENDIX, strNoEnc, filename);

		CString strLen;
		strLen.Format("%d", strCvt.GetLength());
		::WritePrivateProfileString(base, strName+MYIE9_NOENC_LEN_APPENDIX, strLen, filename);

		::WritePrivateProfileString(base, strName, strValue, filename);

		return FALSE;
	}

	BYTE buf[ENC_BUFFER_LEN]={'\0'
	};
	buf[ENC_YES_FLAG_POS]=btEncType0;
	buf[ENC_AES_FLAG_POS]=btEncType1;

	int nPad=ENC_GROUP_LEN - strlen( (LPCTSTR)strCvt)% ENC_GROUP_LEN;

	if (ENC_GROUP_LEN != nPad)
	{
		CString strPad(' ', nPad);
		CString tmp = strCvt;
		strCvt.Format("%s%s", tmp, strPad);
	}
	else
	{
		nPad=0;
	}

	
	CString strMiYao = GetPCIDMiYao(btEncType0, btEncType1); //more than 256bit
	AES myenc(AES::BIT256, (BYTE*)(LPTSTR)(LPCTSTR)strMiYao);

	buf[ENC_DATA_PAD_LEN_POS] =nPad;
	for (size_t pos=0, despos=ENC_HEAD_LEN; pos<strlen( (LPCTSTR)strCvt); pos+=ENC_GROUP_LEN, despos+=ENC_GROUP_LEN)
	{
		myenc.Cipher( (BYTE*)(LPSTR)(LPCTSTR)strCvt + pos, (buf+despos));
	}

	size_t szAllLen = strlen( (LPCTSTR)strCvt ) + ENC_HEAD_LEN;

	CString strAllLen;
	strAllLen.Format("%u", szAllLen);
	
	::WritePrivateProfileStruct(base, strName, buf, szAllLen, filename );
	::WritePrivateProfileString(base, strName + MYIE9_ENC_APPENDIX, strAllLen, filename );
	
	return TRUE;
}

CString CGf_IniFile::GetPCIDMiYao(BYTE flag1, BYTE flag2)
{
	static CString strMac=CGf_Win::GetMac();

	if (ENC_KEY_HOME1_0 == flag1 && ENC_KEY_HOME1_1 == flag2)
	{
		CString strID="";
		strID.Format(".%s+*$dIwkdoqgsa24ga5eaf34n.;]a31`345d-=pyga", strMac);

		strID.SetAt(4, '*');

		return strID;

	}
	else if (ENC_KEY_DATA1_0 == flag1 && ENC_KEY_DATA1_1 == flag2)
	{
		static CString strRegID = CRegHelper::GetPCID();

		static DWORD ulVol = CDiskObject::GetVolumnSerialNumber( CPathHelper::GetExeDir());

		CString strVol;
		strVol.Format("%x", ulVol);

		CString strID="";
		strID.Format(".%s%sD%s*$dImde3rgsa24ga5eaf34n.;]a31`345d-=pyga", strVol, strMac, strRegID.Left(7));

		strID.SetAt(4, '-');

		return strID;

	}
 	else if (ENC_YES_FLAG == flag1 && ENC_AES_FLAG == flag2)
	{
	
		CString strRegID = CRegHelper::GetPCID();

		DWORD ulVol = CDiskObject::GetVolumnSerialNumber( CPathHelper::GetExeDir());

		CString strVol;
		strVol.Format("%x", ulVol);
		
		//MYDEBUG(strVol);

		CString strID="";
		strID.Format("re%sfdd+-2%sds*$dIwmfkrgsa24ga5eaf34n.;]a31`345d-=pyga", strVol, strRegID);

		strID.SetAt(4, '.');

		//MYDEBUG(strID);

		return strID;
		
	}

	ASSERT(false);
	return ".ud+-2s*$dIwl2mdkea24ga5eaf34n.;]a31`345d-=pyga";

}

/************************************************************************/
/* 当某个url内的字段不存在时，取general中的字段。
/************************************************************************/
BOOL CGf_IniFile::GetPrivateProfileStringJinSi(BOOL bIsJinSiDomain, LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName)
{
	BOOL bFieldExist = FALSE;

	if (::GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, lpFileName) > 0)
	{
		bFieldExist=TRUE;
	}
	else if (bIsJinSiDomain)
	{
		if (::GetPrivateProfileString(GENERAL_SECTION, lpKeyName, lpDefault, lpReturnedString, nSize, lpFileName) > 0)
		{
			bFieldExist=TRUE;
		}
	}

	return bFieldExist;
}

CString CGf_IniFile::DecodeJust(const CString &base, const CString &strName, const CString &filename)
{

	if (IsEncValue(base, strName, filename))
	{
		CString strValue;

		BOOL bRet = Decode(strValue, base, strName, filename);
		if (bRet && !strValue.IsEmpty() && (strValue.GetLength() < (ENC_BUFFER_LEN - ENC_HEAD_LEN) ) )
		{
			return strValue;
		}
	}

	return "";
}

BOOL CGf_IniFile::GetSections(const CString &filename, CStringArray &saSections)
{
	char* tmp = _FileGetBuffer(filename);
	if (tmp==NULL)
		return FALSE;
	//
	char *head,*tail;
	char pUrl[INTERNET_MAX_URL_LENGTH +1];
	head = tmp;
	int i=0;
	DWORD len = strlen(tmp);
	while(1)
	{
		if (*head == NULL)
			break;
		else if( (*head == '[') &&( (head==tmp)||('\n'==*(head-1) ) ) )
		{
			tail = strstr(head,"]");
			len = tail-(head+1);
			STRNCPY(pUrl,head+1,len );
			//pCtrl->InsertItem(i,pUrl);
			saSections.Add(pUrl);
			i++;
			head = tail+ 3;// ]\r\n
		}
		else
		{
			tail = strstr(head,"\r\n");
			if (!tail)
				break;
			head = tail+ 2;// \r\n
		}
	}
	free(tmp);

	return TRUE;
}

