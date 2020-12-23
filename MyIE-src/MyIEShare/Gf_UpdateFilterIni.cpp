// Gf_UpdateFilterIni.cpp: implementation of the CGf_UpdateFilterIni class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gf_UpdateFilterIni.h"

#include "Gf_StringList.h"
#include "HelperFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_UpdateFilterIni::CGf_UpdateFilterIni()
{
	
}

CGf_UpdateFilterIni::~CGf_UpdateFilterIni()
{
	
}

CGf_UpdateFilterIni::CGf_UpdateFilterIni(const CString& strUserDir, const CString& strSysDir)
{
	_strUserDir=strUserDir;
	_strSysDir=strSysDir;
	
}

void CGf_UpdateFilterIni::DoFly()
{
	CGf_StringList slContent;
	{
		CString strFile="FilterFly.ini";
		CString strSec="FlyFilter";
		CString key="url";
		//update content filter
		{
			
			CString strContentFilterIni=_strSysDir+strFile;
			
			CHelperFile::AddToStrList(strContentFilterIni, strSec,key, slContent);
			
			slContent.Remove("*");
		}
		
		{
			
			CString strContentFilterIni=_strUserDir+strFile;
			
			CHelperFile::AddToStrList(strContentFilterIni, strSec,key, slContent);
		}
		
		CHelperFile::Write(_strUserDir+strFile, strSec, key, slContent);
		
	}
}

void CGf_UpdateFilterIni::Execute()
{
	{
		CString strFile="Filter.ini";
		CString strSec="UrlFilter";
		CString key="url";
		
		Combine(strFile, strSec, key);

	}
	
	DoUrlUn();

	DoPop();
	DoPopUn();

	DoFly();
	{
		
		CString strFile="FilterFly.ini";
		CString strSec="FlyUnFilter";
		CString key="url";

		Combine(strFile, strSec, key);
			
	
	}
	
}

void CGf_UpdateFilterIni::DoUrlUn()
{
	
	CString strFile="Filter.ini";
	CString strSec="UrlUnFilter";
	CString key="url";
		
	Combine(strFile, strSec, key);

}

void CGf_UpdateFilterIni::DoPop()
{	
	CString strFile="Filter.ini";
	CString strSec="PopFilter";
	CString key="pop";
	
	
	Combine(strFile, strSec,  key);
}

void CGf_UpdateFilterIni::DoPopUn()
{
	
	CString strFile="Filter.ini";
	CString strSec="PopUnFilter";
	CString key="pop";
	
	
	Combine(strFile, strSec, key);
	
}

void CGf_UpdateFilterIni::Combine( const CString&  strFile, const CString&  strSec, const CString&  key )
{
	CGf_StringList slContent;
	CString strContentFilterIni=_strSysDir+strFile;
	
	CHelperFile::AddToStrList(strContentFilterIni, strSec,key, slContent);
	
	
	strContentFilterIni=_strUserDir+strFile;
	
	CHelperFile::AddToStrList(strContentFilterIni, strSec, key, slContent);
	
	
	CHelperFile::Write(_strUserDir+strFile, strSec, key, slContent);
}