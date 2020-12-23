// Gf_TearSession.h: interface for the CGf_TearSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_TEARSESSION_H__BE3498F3_CE12_4665_A96B_B4E9D217F729__INCLUDED_)
#define AFX_GF_TEARSESSION_H__BE3498F3_CE12_4665_A96B_B4E9D217F729__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <afx.h>
#include <afxwin.h>
#include <afxinet.h>

#include <iostream.h>
#include <stdlib.h>

class CTearSession : public CInternetSession
{
public:
	CTearSession(LPCTSTR pszAppName, int nMethod);
	virtual void OnStatusCallback(DWORD dwContext, DWORD dwInternetStatus,
		LPVOID lpvStatusInfomration, DWORD dwStatusInformationLen);
};


class CTearException : public CException
{
	DECLARE_DYNCREATE(CTearException)

public:
	CTearException(int nCode = 0);
	~CTearException() { }

	int m_nErrorCode;
};


#endif // !defined(AFX_GF_TEARSESSION_H__BE3498F3_CE12_4665_A96B_B4E9D217F729__INCLUDED_)
