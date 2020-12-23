// CGf_TranslateUrl.cpp: implementation of the CGf_TranslateUrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gf_TranslateUrl.h"

#include "HelperStr.h"
#include "Gf_ShareConfig.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_TranslateUrl::CGf_TranslateUrl()
{

}

CGf_TranslateUrl::~CGf_TranslateUrl()
{

}

CString CGf_TranslateUrl::TranslatedToMy(const CString &strUrlIn)
{
	return strUrlIn;
}

CString CGf_TranslateUrl::DealUrlToShowToUser(const CString & strUrlIn)
{
	return strUrlIn;

}
