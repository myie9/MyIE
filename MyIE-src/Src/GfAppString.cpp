// GfAppString.cpp: implementation of the CGfAppString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GfAppString.h"

#include "MyIE9.h"
#include "function.h"
#include "macro.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGfAppString::CGfAppString()
{

}

CGfAppString::~CGfAppString()
{

}

CString CGfAppString::GetString(int id)
{

	
	CString str;
	LOADSTR(str, id);

	return str;

}
