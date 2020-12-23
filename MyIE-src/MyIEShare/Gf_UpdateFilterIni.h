// Gf_UpdateFilterIni.h: interface for the CGf_UpdateFilterIni class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_UPDATEFILTERINI_H__ADAF3573_5C07_4D05_9AD2_49EFC626A444__INCLUDED_)
#define AFX_GF_UPDATEFILTERINI_H__ADAF3573_5C07_4D05_9AD2_49EFC626A444__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGf_UpdateFilterIni  
{
private:
	CString _strUserDir;
	CString _strSysDir;	
	void DoFly();
public:
	void Execute();
	
	void DoPopUn();
	
	void Combine( const CString&  strFile, const CString&  strSec, const CString&  key );
	
	void DoPop();
	
	void DoUrlUn();
	
	CGf_UpdateFilterIni(const CString& strUserDir, const CString& strSysDir);
	CGf_UpdateFilterIni();
	virtual ~CGf_UpdateFilterIni();

};

#endif // !defined(AFX_GF_UPDATEFILTERINI_H__ADAF3573_5C07_4D05_9AD2_49EFC626A444__INCLUDED_)
