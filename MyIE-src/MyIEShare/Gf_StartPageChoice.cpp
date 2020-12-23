// Gf_StartPageChoice.cpp: implementation of the CGf_StartPageChoice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gf_StartPageChoice.h"
#include "GfPathManager.h"
#include "Gf_inifile.h"
#include "HelperFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define INI_START_PAGE_CHOICE "StartPageChoice"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_StartPageChoice::CGf_StartPageChoice()
{

}

CGf_StartPageChoice::~CGf_StartPageChoice()
{

}

CGf_StartPageChoice& CGf_StartPageChoice::GetIns()
{

	static CGf_StartPageChoice ins;
	return ins;

}

int CGf_StartPageChoice::GetNamesSize()
{
	return _names.GetSize();

}

CString CGf_StartPageChoice::GetName(int i)
{
	return _names.GetAt(i);
}

int CGf_StartPageChoice::GetOldSel()
{
	return ::GetPrivateProfileInt(INI_SETTINGS, INI_START_PAGE_CHOICE, 0, CGfPathManager::GetUserConfig());

}

void CGf_StartPageChoice::SetSel(int npos)
{
	int nOld=GetOldSel();
	if (nOld==npos)
	{
		return;
	}

	if (npos<0)
	{
		return;
	}


	CString str;
	str.Format("%d", npos);
	::WritePrivateProfileString(INI_SETTINGS, INI_START_PAGE_CHOICE, str, CGfPathManager::GetUserConfig());

}

CString CGf_StartPageChoice::GetMyIE9StartPage()
{
	LoadFiles();

	int old=GetOldSel();

	if (old>=_files.GetSize())
	{
		return "";
	}


	return CGfPathManager::GetMyIE9StartPage(_files.GetAt(old));

}

void CGf_StartPageChoice::LoadFiles()
{
	if (_files.GetSize()>0)
	{
		return;
	}

	CHelperFile::AddToStrArray(CGfPathManager::GetResStartPageDir()+ "user_startPageChoiceFiles.txt", _files);
	CHelperFile::AddToStrArray(CGfPathManager::GetResStartPageDir()+ "startPageChoiceFiles.txt", _files);

}

void CGf_StartPageChoice::LoadNames()
{
	
	if (_names.GetSize()>0)
	{
		return;
	}
	
	CHelperFile::AddToStrArray(CGfPathManager::GetResStartPageDir()+ "user_startPageChoiceNames.txt", _names);

	CHelperFile::AddToStrArray(CGfPathManager::GetResStartPageDir()+ "startPageChoiceNames.txt", _names);

}
