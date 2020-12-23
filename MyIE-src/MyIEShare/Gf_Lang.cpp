// Gf_Lang.cpp: implementation of the CGf_Lang class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gf_Lang.h"
#include "GfPathManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_Lang::CGf_Lang()
{

}

CGf_Lang::~CGf_Lang()
{

}

CString CGf_Lang::GetStr(int id)
{
	CString strId;
	strId.Format("%d", id);
	
	CString strValue;
	::GetPrivateProfileString("UpgradeExe", strId, strId, strValue.GetBuffer(256), 256, CGfPathManager::GetLanguageFile() );
	strValue.ReleaseBuffer();

	return strValue;
}
