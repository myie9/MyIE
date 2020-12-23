// Gf_StartPageMgr.h: interface for the CGf_StartPageMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_STARTPAGEMGR_H__9EC3BA26_8946_4A86_BC74_6F30AA2EF953__INCLUDED_)
#define AFX_GF_STARTPAGEMGR_H__9EC3BA26_8946_4A86_BC74_6F30AA2EF953__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Gf_StringList.h"

class CGf_StartPageMgr  
{
public:
	void LoadNames();
	void AddFav(LPCSTR title, const CString& url);
	void Write();
	CString _all;
	void AddTyped(LPCSTR url);
	void AddLastVisit(const CString& name, const CString& url);
	void EnableWrite();
	static CGf_StartPageMgr _ins;
	CGf_StartPageMgr();
	virtual ~CGf_StartPageMgr();

	BOOL _bWrite;
private:
	CGf_StringList _slFavUrl,_slFavName;
	CString GetTitleShowStr(const CString& str);
	void SaveFavData();
	void SetFavData();
	int _nTyped;
	int _maxTyped;
	void AddBlockLastVisit(const CString &block);
	void Write(const CString& all);
	void AddBlock(const CString &block, const CString &titleName);
	CString GetName(const CString& name);
	CString _form;
	CString _fav;
	CString _typed;
	CString _lastVisit;
	
};

#endif // !defined(AFX_GF_STARTPAGEMGR_H__9EC3BA26_8946_4A86_BC74_6F30AA2EF953__INCLUDED_)
