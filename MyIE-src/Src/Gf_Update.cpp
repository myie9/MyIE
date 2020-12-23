// Gf_Update.cpp: implementation of the CGf_Update class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gf_Update.h"
#include "GfPathManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_Update::CGf_Update()
{

}

CGf_Update::~CGf_Update()
{

}

void CGf_Update::Execute()
{	

	if (!IsOKToUpdate() )
	{
		return;

	}

	CString strExe = CGfPathManager::GetUpdateExeFilePath();

	ShellExecute(NULL, "open", strExe, "-d d -k k", NULL, SW_HIDE);
}

BOOL CGf_Update::IsOKToUpdate()
{
	if (1 == rand()%3 )
	{
		return FALSE;
	}

	{
	
	CTime tcur = CTime::GetCurrentTime();
	int ih = tcur.GetHour();

	if (8 == ih || 9 == ih )
	{
		return FALSE;
	}

	}

	return TRUE;
}