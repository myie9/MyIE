// Gf_UpdateControl.h: interface for the CGf_UpdateControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_UPDATECONTROL_H__16705B1A_DA9D_472F_AD87_E909C597367C__INCLUDED_)
#define AFX_GF_UPDATECONTROL_H__16705B1A_DA9D_472F_AD87_E909C597367C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afx.h>

class CGf_UpdateControl  
{
public:
	static BOOL IsLastedVersion(BOOL bGetTestVer=FALSE);
	static CString GetLocalVersion();
	BOOL Execute();
	CGf_UpdateControl();
	virtual ~CGf_UpdateControl();

private:
	BOOL DownPatch(const CString& strId, const CString& strLocalFileName);
//	CString GetRandomDownloadUrl();
};

#endif // !defined(AFX_GF_UPDATECONTROL_H__16705B1A_DA9D_472F_AD87_E909C597367C__INCLUDED_)
