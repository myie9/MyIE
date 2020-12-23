// HelperException.cpp: implementation of the CHelperException class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HelperException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHelperException::CHelperException()
{

}

CHelperException::~CHelperException()
{

}

void CHelperException::ErrorOcurred(const CString& strMsg)
{
	ASSERT(!strMsg);

}