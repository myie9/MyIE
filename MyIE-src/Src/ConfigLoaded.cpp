// CConfigLoaded.cpp: implementation of the CCConfigLoaded class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyIE9.h"
#include "ConfigLoaded.h"
#include "Gf_AppStatic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConfigLoaded::CConfigLoaded()
{

}

CConfigLoaded::~CConfigLoaded()
{
	CGf_AppStatic::_instance._bDelayLoaded=TRUE;

}
