
// Gf_Win.cpp: implementation of the CGf_Win class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gf_Win.h"


int GetMacAddress(CStringArray &LanMAC)  
{ 
  #define MAX_ADAPTER_NAME_LENGTH 256
  #define MAX_ADAPTER_DESCRIPTION_LENGTH 128
  #define MAX_ADAPTER_ADDRESS_LENGTH 8
  typedef struct _NET_IP_ADDRESS_STRING{
    char String[16];
  } NET_IP_ADDRESS_STRING, *PNET_IP_ADDRESS_STRING, NET_IP_MASK_STRING, *PNET_IP_MASK_STRING;
  typedef struct _NET_IP_ADDR_STRING {
    struct _NET_IP_ADDR_STRING* Next;
    NET_IP_ADDRESS_STRING IpAddress;
    NET_IP_MASK_STRING IpMask;
    DWORD Context;
  } NET_IP_ADDR_STRING, *PNET_IP_ADDR_STRING;
  typedef struct _NET_IP_ADAPTER_INFO {
    struct _NET_IP_ADAPTER_INFO* Next;
    DWORD ComboIndex;
    char AdapterName[MAX_ADAPTER_NAME_LENGTH+4];
    char Description[MAX_ADAPTER_DESCRIPTION_LENGTH+4];
    UINT AddressLength;
    BYTE Address[MAX_ADAPTER_ADDRESS_LENGTH];
    DWORD Index;
    UINT Type;
    UINT DhcpEnabled;
    PNET_IP_ADDR_STRING CurrentIpAddress;
    NET_IP_ADDR_STRING IpAddressList;
    NET_IP_ADDR_STRING GatewayList;
    NET_IP_ADDR_STRING DhcpServer;
    BOOL HaveWins;
    NET_IP_ADDR_STRING PrimaryWinsServer;
    NET_IP_ADDR_STRING SecondaryWinsServer;
    time_t LeaseObtained;
    time_t LeaseExpires;
  } NET_IP_ADAPTER_INFO, *PNET_IP_ADAPTER_INFO;

  LanMAC.RemoveAll();

  HINSTANCE hLib = 0;
  if ( hLib = LoadLibrary( _T("iphlpapi.dll") ) ) //加载动态库
  {
    DWORD  (WINAPI*pGetAdaptersInfo ) (PNET_IP_ADAPTER_INFO,PULONG) =
      ( DWORD (WINAPI*) (PNET_IP_ADAPTER_INFO,PULONG) )GetProcAddress(hLib, "GetAdaptersInfo");//获取地址
    if ( pGetAdaptersInfo != 0  )
    {
      DWORD AdapterInfoSize = 0;
      if( pGetAdaptersInfo(0, &AdapterInfoSize) == ERROR_BUFFER_OVERFLOW) //获取长度
      {
        PNET_IP_ADAPTER_INFO pAdapterInfo = (PNET_IP_ADAPTER_INFO) new BYTE [AdapterInfoSize]; //分配内存

        if ( pGetAdaptersInfo(pAdapterInfo,   &AdapterInfoSize) == ERROR_SUCCESS) //获取信息
        {
          PNET_IP_ADAPTER_INFO pAdapter = pAdapterInfo;
          while(pAdapter)
          {
            CString strMac;
            //gf strMac.Format( _T("%02X-%02X-%02X-%02X-%02X-%02X"),
			strMac.Format( _T("%02X%02X%02X%02X%02X%02X"),
              pAdapter->Address[0],pAdapter->Address[1],pAdapter->Address[2],
              pAdapter->Address[3],pAdapter->Address[4],pAdapter->Address[5]);
            LanMAC.Add(strMac);

			break;
            pAdapter = pAdapter->Next; //下一个区域
          }
        }

        delete [] pAdapterInfo; //释放内存
      }
    }

    FreeLibrary( hLib ); //释放库
  }

  return LanMAC.GetSize(); //返回个数
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_Win::CGf_Win()
{

}

CGf_Win::~CGf_Win()
{

}

BOOL CGf_Win::OpenNormal(const CString &strToOpen)
{
	ShellExecute(NULL, "open", strToOpen , NULL, NULL, SW_NORMAL);

	return TRUE;
}

BOOL CGf_Win::OpenHide(const CString &strExeToOpen, LPCSTR lpParameters)
{
	ShellExecute(NULL, "open", strExeToOpen , lpParameters, NULL, SW_HIDE);

	return TRUE;
}


CString CGf_Win::GetMac()
{

	CStringArray lanMAC;
	int nRet= GetMacAddress(lanMAC);
	if (nRet ==0)
	{
		return "";
	}

	if (lanMAC.GetSize()>0)
	{
		return lanMAC.GetAt(0);
	}

	return "";
}


BOOL MyPreTranslateMessage(MSG* pMsg)
{
// 	ASSERT_VALID(this);
// 	
// 	// if this is a thread-message, short-circuit this function
// 	if (pMsg->hwnd == NULL && DispatchThreadMessageEx(pMsg))
// 		return TRUE;
	
	// walk from target to main window
	CWnd* pMainWnd = AfxGetMainWnd();
	if (CWnd::WalkPreTranslateTree(pMainWnd->GetSafeHwnd(), pMsg))
		return TRUE;
	
	// in case of modeless dialogs, last chance route through main
	//   window's accelerator table
	if (pMainWnd != NULL)
	{
		CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);
		if (pWnd->GetTopLevelParent() != pMainWnd)
			return pMainWnd->PreTranslateMessage(pMsg);
	}
	
	return FALSE;   // no special processing
}


void CGf_Win::ReleaseCtrl()
{
	MSG msg;
	while(::PeekMessage(&msg, NULL, 0,0,PM_REMOVE) &&!MyPreTranslateMessage(&msg))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

}

void CGf_Win::Delay(DWORD dwSeconds)
{
	DWORD dwTick=::GetTickCount();
	while(GetTickCount()<(dwTick+dwSeconds))
	{
		ReleaseCtrl();
	}

}

CString CGf_Win::GetFullLangID()
{
	LANGID id=GetUserDefaultLangID();
	TCHAR d1[MAX_PATH]={0};
	TCHAR d2[6]={0};

	GetLocaleInfo( MAKELCID(id, SORT_DEFAULT), LOCALE_SENGLANGUAGE, d1, sizeof(d1)/sizeof(d1[0]));//Chinese
	GetLocaleInfo( MAKELCID(id, SORT_DEFAULT), LOCALE_SISO3166CTRYNAME, d2, sizeof(d2)/sizeof(d2[0]));//CN

	CString str;
	str.Format("%s%s", d1, d2);
	//str.MakeLower();
	return str;

}

//zh
CString CGf_Win::GetPrimaryLangID()
{

	LANGID id=GetUserDefaultLangID();
	TCHAR d1[MAX_PATH]={0};
	TCHAR d2[6]={0};
	
	GetLocaleInfo( MAKELCID(id, SORT_DEFAULT), LOCALE_SISO639LANGNAME, d1, sizeof(d1)/sizeof(d1[0]));//zh
	
	
	CString str;
	str.Format("%s", d1);
	str.MakeLower();
	return str;

}

//zhcn
CString CGf_Win::GetFullAbbriLangID()
{
	
	LANGID id=GetUserDefaultLangID();
	TCHAR d1[MAX_PATH]={0};
	TCHAR d2[6]={0};
	
	GetLocaleInfo( MAKELCID(id, SORT_DEFAULT), LOCALE_SISO639LANGNAME, d1, sizeof(d1)/sizeof(d1[0]));//zh
	
	GetLocaleInfo( MAKELCID(id, SORT_DEFAULT), LOCALE_SISO3166CTRYNAME, d2, sizeof(d2)/sizeof(d2[0]));//CN
	
	CString str;
	str.Format("%s%s", d1, d2);
	str.MakeLower();
	return str;

}
