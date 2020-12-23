// Gf_Win.h: interface for the CGf_Win class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_WIN_H__13AC439F_B703_403D_9982_C6A982E5F6C2__INCLUDED_)
#define AFX_GF_WIN_H__13AC439F_B703_403D_9982_C6A982E5F6C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGf_Win  
{
public:
	static CString GetFullAbbriLangID();
	static CString GetPrimaryLangID();
	static CString GetFullLangID();
	static void Delay(DWORD dwSeconds);
	static void ReleaseCtrl();
	static CString GetMac();
	static BOOL OpenNormal(const CString& strToOpen);
	CGf_Win();
	virtual ~CGf_Win();

	static BOOL OpenHide(const CString &strExeToOpen, LPCSTR lpParameters = NULL);

};

#endif // !defined(AFX_GF_WIN_H__13AC439F_B703_403D_9982_C6A982E5F6C2__INCLUDED_)
