// Gf_TearSession.cpp: implementation of the CGf_TearSession class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gf_TearSession.h"

static BOOL    bProgressMode = FALSE;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CTearSession object

// TEAR wants to use its own derivative of the CInternetSession class
// just so it can implement an OnStatusCallback() override.

CTearSession::CTearSession(LPCTSTR pszAppName, int nMethod)
	: CInternetSession(pszAppName, 1, nMethod)
{
}

void CTearSession::OnStatusCallback(DWORD /* dwContext */, DWORD dwInternetStatus,
	LPVOID /* lpvStatusInfomration */, DWORD /* dwStatusInformationLen */)
{
	if (!bProgressMode)
		return;

	if (dwInternetStatus == INTERNET_STATUS_CONNECTED_TO_SERVER)
		cerr << _T("Connection made!") << endl;
}

/////////////////////////////////////////////////////////////////////////////
// CTearException -- used if something goes wrong for us

// TEAR will throw its own exception type to handle problems it might
// encounter while fulfilling the user's request.

IMPLEMENT_DYNCREATE(CTearException, CException)

CTearException::CTearException(int nCode)
	: m_nErrorCode(nCode)
{
}


