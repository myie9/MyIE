// Gf_HelperInternet.cpp: implementation of the CGf_HelperInternet class.
//
//////////////////////////////////////////////////////////////////////

#include <afx.h>
#include <afxwin.h>
#include <afxinet.h>

#include <iostream.h>


#include "Gf_HelperInternet.h"
#include "gf_TearSession.h"

#define MAX_REDIRECT_TIMES 10

static BOOL GF_NO_INPUT_PASSWORD = TRUE;

//LPCTSTR pszURL = "http://img.tongji.linezing.com/1465618/tongji.gif";
//"http://ftpmyie9.n76.nicdns.cn/a.html";// "http://tainfo.onlinedown.net/down/myie9.zip"; //OK NULL;
static BOOL    bStripMode = FALSE;
static BOOL    bProgressMode = FALSE;
static DWORD   dwAccessType = PRE_CONFIG_INTERNET_ACCESS;

static DWORD dwHttpRequestFlags =
INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT | INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_PRAGMA_NOCACHE|INTERNET_FLAG_RELOAD;

static const TCHAR szHeaders[] =
_T("Accept: */*\r\nUser-Agent: Maxthon2\r\n");

void ThrowTearException(int nCode)
{
	CTearException* pEx = new CTearException(nCode);
	throw pEx;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_HelperInternet::CGf_HelperInternet()
:_bReadContent(FALSE)
,_strReadContent("")
{
	
}

CGf_HelperInternet::~CGf_HelperInternet()
{
	
}

int CGf_HelperInternet::DoCommunicate()
{
	
	int nRetCode = 0;
	
	CTearSession session(_T("SafeBrowser Download"), dwAccessType);
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
	try
	{
		// check to see if this is a reasonable URL
		
		CString strServerName;
		CString strObject;
		INTERNET_PORT nPort;
		DWORD dwServiceType;
		
		//test http://img.tongji.linezing.com/1465618/tongji.gif //http://img.tongji.linezing.com/1192897/tongji.gif
//		if (!AfxParseURL("http://myie9.googlecode.com/files/MyIE9_For_Huawei_Portable.zip", dwServiceType, strServerName, strObject, nPort) ||
// 			dwServiceType != INTERNET_SERVICE_HTTP) //_pszURL
		if (!AfxParseURL(_pszURL, dwServiceType, strServerName, strObject, nPort) ||
			dwServiceType != INTERNET_SERVICE_HTTP) 
		{
			cerr << _T("Error: can only use URLs beginning with http://") << endl;
			ThrowTearException(1);
		}
		
		if (bProgressMode)
		{
			cerr << _T("Opening Internet...");
			VERIFY(session.EnableStatusCallback(TRUE));
		}
		
		pServer = session.GetHttpConnection(strServerName, nPort);
		
		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
			strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
		pFile->AddRequestHeaders(szHeaders);
		pFile->SendRequest();
		
		DWORD dwRet;
		pFile->QueryInfoStatusCode(dwRet);
		
		for (int iRedirectTime=0; iRedirectTime< MAX_REDIRECT_TIMES; iRedirectTime++)
		{
			
			
			// if access was denied, prompt the user for the password
			
			if (dwRet == HTTP_STATUS_DENIED)
			{
				if (GF_NO_INPUT_PASSWORD)
				{
					cerr << _T("Access denied: Need password\n");
					ThrowTearException(1);
				}				
				
				DWORD dwPrompt;
				dwPrompt = pFile->ErrorDlg(NULL, ERROR_INTERNET_INCORRECT_PASSWORD,
					FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, NULL);
				
				// if the user cancelled the dialog, bail out
				
				if (dwPrompt != ERROR_INTERNET_FORCE_RETRY)
				{
					cerr << _T("Access denied: Invalid password\n");
					ThrowTearException(1);
				}
				
				pFile->SendRequest();
				pFile->QueryInfoStatusCode(dwRet);
			}
			
			//CString strNewLocation;
			//pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strNewLocation);
			
			// were we redirected?
			// these response status codes come from WININET.H
			
			if (dwRet == HTTP_STATUS_MOVED ||
				dwRet == HTTP_STATUS_REDIRECT ||
				dwRet == HTTP_STATUS_REDIRECT_METHOD)
			{
				CString strNewLocationAll;
				pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strNewLocationAll);
				
				int nPlace = strNewLocationAll.Find(_T("Location: "));
				if (nPlace == -1)
				{
					cerr << _T("Error: Site redirects with no new location") << endl;
					ThrowTearException(2);
				}
				
				CString strNewLocation = strNewLocationAll.Mid(nPlace + 10);
				nPlace = strNewLocation.Find('\n');
				if (nPlace > 0)
					strNewLocation = strNewLocation.Left(nPlace);
				
				// close up the redirected site
				
				pFile->Close();
				delete pFile;
				pFile = NULL;
				
				pServer->Close();
				delete pServer;
				pServer = NULL;
				
				
				
				if (bProgressMode)
				{
					cerr << _T("Caution: redirected to ");
					cerr << (LPCTSTR) strNewLocation << endl;
				}
				
				if (_bJustGetRedirectUrl)
				{
					_strNewUrl = strNewLocation;
					ThrowTearException(31);
				}
				
				
				// figure out what the old place was
				if (!AfxParseURL(strNewLocation, dwServiceType, strServerName, strObject, nPort))
				{
					cerr << _T("Error: the redirected URL could not be parsed.") << endl;
					ThrowTearException(2);
				}
				
				if (dwServiceType != INTERNET_SERVICE_HTTP)
				{
					cerr << _T("Error: the redirected URL does not reference a HTTP resource.") << endl;
					ThrowTearException(2);
				}
				
				// try again at the new location
				pServer = session.GetHttpConnection(strServerName, nPort);
				pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
					strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
				pFile->AddRequestHeaders(szHeaders);
				pFile->SendRequest();
				
				pFile->QueryInfoStatusCode(dwRet);
// 				if (dwRet != HTTP_STATUS_OK)
// 				{
// 					cerr << _T("Error: Got status code ") << dwRet << endl;
// 					ThrowTearException(2);
// 				}
				
				cerr << _T("Status Code of Download is ") << dwRet << endl;
			}
			else if (HTTP_STATUS_OK != dwRet)
			{
				cerr << _T("Status Code of Download is ") << dwRet << endl;
				ThrowTearException(33);
			}
			else
			{	
				if (_bJustGetRedirectUrl)
				{
					ThrowTearException(32);
					
				}
				
				break;
			}
		}
		//gf
		
		
		CFile flTolocal;

		if (!_bReadContent)
		{
			BOOL bOpenFile = flTolocal.Open(_strLocalFileName, CFile::modeCreate| CFile::modeWrite);
			
			if (!bOpenFile)
			{
				ASSERT(false);
				ThrowTearException(4);
			}
		}
		
		
		TCHAR sz[1025];
		int nRead = 1;
		
		nRetCode = FALSE;

		while (nRead > 0)
		{
			ZeroMemory(sz, 1025);
			
			nRead = pFile->Read(sz, 1024);
			
			if (nRead>0)
			{
				if (!_bReadContent)
					flTolocal.Write(sz, nRead);
				else
					_strReadContent+=sz;

				nRetCode = TRUE;
			}
		}
		
		if (!_bReadContent)
			flTolocal.Close();
		
		//		pFile->Close();
		//		pServer->Close();
	}
	catch (CInternetException* pEx)
	{
		// catch errors from WinINet
		
		TCHAR szErr[1024];
		pEx->GetErrorMessage(szErr, 1024);
		
		cerr << _T("Error: (") << pEx->m_dwError << _T(") ");
		cerr << szErr << endl;
		
		nRetCode = 2;
		pEx->Delete();

		nRetCode = FALSE;
	}
	catch (CTearException* pEx)
	{
		// catch things wrong with parameters, etc
		
		nRetCode = pEx->m_nErrorCode;
		TRACE1("Error: Exiting with CTearException(%d)\n", nRetCode);
		pEx->Delete();

		nRetCode = FALSE;
	}
	catch(...)
	{
		//nRetCode = FALSE;
	}
	
	if (pFile != NULL)
	{
		pFile->Close();
		delete pFile;
	}
	
	if (pServer != NULL)
	{
		pServer->Close();
		delete pServer;
	}
	
	session.Close();
	
	
	//	ShellExecute(NULL, "open", strLocalFileName, NULL, NULL, SW_SHOW);
	//ShellExecute(NULL, "open", "rar.exe", "x " + strLocalFileName +" " + "D:\", NULL, SW_HIDE);
	
	return nRetCode;
	
}

int CGf_HelperInternet::DownLoadHttpFile(const CString &strUrl, const CString &strLocalFilePath)
{
	_pszURL = strUrl;

	_strLocalFileName = strLocalFilePath;

	_bJustGetRedirectUrl = FALSE;

	return DoCommunicate();
	
}

CString CGf_HelperInternet::GetRedirectedUrl(const CString &strUrl)
{
	
	_pszURL = strUrl;


	_bJustGetRedirectUrl = TRUE;

	DoCommunicate();

	return _strNewUrl;
	
}

CString CGf_HelperInternet::ReadFileContent(const CString &strUrl)
{
	
	_pszURL = strUrl;	
	
	_bJustGetRedirectUrl = FALSE;

	_bReadContent=TRUE;
	_strReadContent="";
	
	DoCommunicate();
	
	return _strReadContent;
	
}
