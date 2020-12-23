// Gf_StartPageMgr.cpp: implementation of the CGf_StartPageMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyIE9.h"
#include "Gf_StartPageMgr.h"
#include "HelperStr.h"
#include "GfPathManager.h"
#include "Resource.h"
#include "GfAppString.h"
#include "HelperFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define GF_FRAME "<div id=\"aside\"><div class=\"div_border\"  style=\"zoom:1\">%s</div></div>"

#define GF_INSERT "<!--myie9insert-->"

#define GF_TABLE "<table>%s%s</table>"
#define GF_TABLE_LASTVISIT "<table id=\"lastClose\">%s%s</table>"

#define GF_TITLE_LASTVISIT "<tr><th>%s(<a href=\"###\" onclick=\"javascript:openall();return false;\">%s</a>)%s</th></tr>"
#define GF_TITLE "<tr><th>%s</th></tr>"

#define GF_LINE "<tr><td><a href=\"%s\">%s</a></td></tr>"

#define MAX_FAV 7
#define MAX_LINE_LEN 20
#define MAX_TITLE_LINE_LEN 23
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_StartPageMgr CGf_StartPageMgr::_ins;

CGf_StartPageMgr::CGf_StartPageMgr()
:_bWrite(FALSE)
,_lastVisit("")
,_typed("")
,_form("")
,_fav("")

,_maxTyped(5)
,_nTyped(0)
{

}

CGf_StartPageMgr::~CGf_StartPageMgr()
{

}

void CGf_StartPageMgr::EnableWrite()
{
	_bWrite=TRUE;

}

void CGf_StartPageMgr::AddLastVisit(const CString& name, const CString& url)
{
	CString str;
	str.Format(GF_LINE, url, GetName(name) );

	_lastVisit+=str;

}


void CGf_StartPageMgr::AddTyped(LPCSTR url)
{
	return;

	if (_nTyped>_maxTyped)
	{
		return;
	}

	CString strUrl;
	strUrl.Format("%s", url);

	if (CHelperStr::StartsWith(strUrl, "http://") )
	{
		strUrl=strUrl.Right(strUrl.GetLength() - 7);
	}
	
	CString str;
	str.Format(GF_LINE, url, GetName(strUrl) );
	
	_typed+=str;

	_nTyped++;
	
}

CString CGf_StartPageMgr::GetName(const CString &name)
{
	
	CString strUrl;
	strUrl.Format("%s", name);
	
	if (CHelperStr::StartsWith(strUrl, "http://") )
	{
		strUrl=strUrl.Right(strUrl.GetLength() - 7);

		if (strUrl.Right(1) == "/")
		{
			strUrl=strUrl.Left( strUrl.GetLength() -1);
		}
	}

	return CHelperStr::GetSubStr(strUrl, MAX_LINE_LEN, "..");

}

void CGf_StartPageMgr::Write()
{
	SetFavData();

	AddBlock(_fav, CGfAppString::GetString(IDS_PAGE_FAV) );

	//AddBlock(_form, CGfAppString::GetString(IDS_PAGE_FORM) );
	//AddBlock(_typed, CGfAppString::GetString(IDS_PAGE_TYPED) );
	AddBlockLastVisit(_lastVisit);

	CString total;
	total.Format(GF_FRAME, _all);

	Write(total);

	SaveFavData();
}

void CGf_StartPageMgr::AddBlock(const CString &block, const CString &titleName)
{
	if (block.IsEmpty())
	{
		return;
	}

	CString title;
	title.Format(GF_TITLE, GetTitleShowStr(titleName) );

	CString str;
	str.Format(GF_TABLE, title, block);

	_all+=str;
}

void CGf_StartPageMgr::Write(const CString &all)
{
	CString old=CHelperFile::Read(CGfPathManager::GetStartPageLeftBak() );

	if (old.IsEmpty())
	{		
		return;
	}

	old.Replace(GF_INSERT, all);

	CHelperFile::Write(CGfPathManager::GetStartPageLeft(), old);

}

CString CGf_StartPageMgr::GetTitleShowStr(const CString& str)
{
	return str;

	
// 	int nPad=MAX_TITLE_LINE_LEN-str.GetLength();
// 
// 	CString strPad=CHelperStr::GetN("&nbsp;", nPad);
// 	return str+strPad;
	

}

void CGf_StartPageMgr::AddBlockLastVisit(const CString &block)
{
	if (block.IsEmpty())
	{
		return;
	}

	
	//int nPad=MAX_TITLE_LINE_LEN-CGfAppString::GetString(IDS_PAGE_LASTVISIT).GetLength()-2-CGfAppString::GetString(IDS_PAGE_OPEN).GetLength();	

	CString strPad="";//CHelperStr::GetN("&nbsp;", nPad);

	CString title;
	title.Format(GF_TITLE_LASTVISIT, CGfAppString::GetString(IDS_PAGE_LASTVISIT), CGfAppString::GetString(IDS_PAGE_OPEN), strPad);

	CString str;
	str.Format(GF_TABLE_LASTVISIT, title, block);
	
	_all+=str;
}

void CGf_StartPageMgr::AddFav(LPCSTR title, const CString &url)
{
	if (url.IsEmpty())
	{
		return;
	}

	int iPos=_slFavUrl.FindInt(url);
	if (iPos>-1)
	{
		_slFavUrl.Remove(iPos);
		_slFavName.Remove(iPos);
	}

	CString strTitle=title;

	if (strTitle.IsEmpty())
	{
		strTitle=url;

	}

	_slFavName.AddHead( GetName(strTitle) );
	_slFavUrl.AddHead(url);

}

void CGf_StartPageMgr::SetFavData()
{

	if (_slFavUrl.GetCount()<MAX_FAV)
	{
		CHelperFile::AddToStrList( CGfPathManager::GetFavName(), _slFavName);
		CHelperFile::AddToStrList( CGfPathManager::GetFavUrl(), _slFavUrl);
	}

	for (int i=0; (i<_slFavUrl.GetCount())&&(i<MAX_FAV);i++)
	{
		
		CString str;
		str.Format(GF_LINE, _slFavUrl.GetAt(i), _slFavName.GetAt(i));
		
		_fav+=str;

	}
	
}

void CGf_StartPageMgr::SaveFavData()
{
	CHelperFile::Write(CGfPathManager::GetFavName(), _slFavName);
	CHelperFile::Write(CGfPathManager::GetFavUrl(), _slFavUrl);

}

void CGf_StartPageMgr::LoadNames()
{

}
