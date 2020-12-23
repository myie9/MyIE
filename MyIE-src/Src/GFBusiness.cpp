// GFBusiness.cpp: implementation of the CGFBusiness class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GFBusiness.h"

#include "FilterSites.h"
#include "PathHelper.h"

#include "GfAppString.h"
#include "resource.h"

#include "Consts_gf.h"
#include "HelperStr.h"
#include "Gf_Config.h"
#include "macro.h"
#include "function.h"

#define MAX_SHOW_IN_UI_STR 250

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGFBusiness::CGFBusiness()
{
	
}

CGFBusiness::~CGFBusiness()
{
	
}

BOOL CGFBusiness::IsBadSite(LPCTSTR lpszURL)
{
	if (CFilterSites::FilterSite(lpszURL))
	{
		return TRUE;
		
		
	}	
	else
	{
		return FALSE;
	}
	
}

CString CGFBusiness::GetBlockedMsgFile(const CString& strUrl, const CString& strUrlShow)
{
	
	
	CString strTmpFile = CPathHelper::GetUniqueTmpFilePath();
	
	try
	{
		CFile flShow;
		flShow.Open(strTmpFile, CFile::modeCreate| CFile::modeWrite);
		
		CString strMsg;
		strMsg.Format( CGfAppString::GetString(IDS_BLOCKED_MSG_PAGE), strUrl, strUrlShow, strUrl);
		flShow.Write(strMsg, strMsg.GetLength() );
		flShow.Close();
	}
	catch (...) {
	}
	
	
	return strTmpFile;
}

CString CGFBusiness::GetAddressShowStr(const CString &strUrl, const CString &strTitle)
{
	if (!GF_ADD_ADRRESS_TITLE)
	{
		return strUrl;
	}

	CString str = "";
	if ( CHelperStr::IsTitleOK(strTitle) )
	{
		str = CHelperStr::GetSubStr(strTitle, CConsts::_iTitleShow) + CConsts::_strSpaceBetweenTitleAndUrl + strUrl;
	}
	else
	{
		str = strUrl;
	}

	str.TrimLeft();

	return str;
}


BOOL CGFBusiness::IsValidAdressList(const CString &strUrl)
{
	if (IS_HTTP(strUrl))
	{
		return TRUE;
	}

	return FALSE;

}

/************************************************************************/
/* 是否要填表单中的这个值
strValue:new
strFieldValue:old
/************************************************************************/
BOOL CGFBusiness::IsNeedFill(const CString &strName, const CString &strFieldValue, const CString &strValue)
{
	if ( strFieldValue==strValue || strValue.IsEmpty() )
	{
		return FALSE;
	}

	
	BOOL bFillField = TRUE;

	if (!strFieldValue.IsEmpty() )
	{		

		CString strMsg;
		strMsg.Format(CGfAppString::GetString(IDS_OVERWRITE_FIELD), strName, strFieldValue.Left(MAX_SHOW_IN_UI_STR), strValue.Left(MAX_SHOW_IN_UI_STR));
		
		if (IDYES != MSGBOX(strMsg, MB_YESNOCANCEL|MB_ICONQUESTION) )
		{
			bFillField = FALSE;
		}
		
	}
	
	return bFillField;
	
}
