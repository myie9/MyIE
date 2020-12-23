#include "stdafx.h"
#include "PerfMon.h"
#include "rasSpdM.h"


BOOL glo_fFromDynReg=TRUE, glo_fSuc = FALSE, glo_fInitialized = FALSE;
BOOL glo_fInitializing = FALSE;
CPerfMon m_PerfMon;
int m_nRAS;

/*
在9x下通过dyn_data注册表得到速度
*/
DWORD GetTotalRecv(void)
{
	DWORD dwValue=(DWORD)-999;

	HKEY hKey;
	if(0==RegOpenKey(HKEY_DYN_DATA,"PerfStats\\StatData",&hKey))
	{
		DWORD dwType,dwLen=sizeof(dwValue);
		RegQueryValueEx(hKey,"Dial-Up Adapter\\TotalBytesRecvd",NULL,&dwType,(BYTE*)&dwValue,&dwLen);
		RegCloseKey(hKey);
	}

	return dwValue;
}

/*****************************************************************
在NT下通过per Data得到
 *****************************************************************/
/*****************************************************************
得到下RAS速度
 *****************************************************************/
inline void GetRASSpeedOn9X(/*DWORD* pTPS,DWORD* pRPS,DWORD* pTT,*/ DWORD* pRT)
{
	*pRT=GetTotalRecv();
}

inline void GetRASSpeedOnNT(/*DWORD* pTPS,DWORD* pRPS,DWORD* pTT,*/ DWORD* pRT)
{
	//参数：每秒发送，每秒接收，总发送，总接受
	try{
	if(glo_fSuc)
	{
		if (!m_PerfMon.CollectQueryData())
		{
			*pRT = (DWORD)-999;
			return;
		}
		*pRT = m_PerfMon.GetCounterValue(m_nRAS);
	}
	else
		*pRT = (DWORD)-999;
	}catch(...){}
}

void GetRASSpeed(/*DWORD* pTPS,DWORD* pRPS,DWORD* pTT,*/ DWORD* pRT)
{
	//参数：每秒发送，每秒接收，总发送，总接受
	if(glo_fFromDynReg==FALSE )
	{
		if(glo_fSuc)
			GetRASSpeedOnNT(/*pTPS,pRPS, pTT,*/ pRT);
		else
			*pRT = -999;
	}
	else
	{
		GetRASSpeedOn9X(/*pTPS,pRPS, pTT,*/ pRT);
	}
}

void InitSpeedMonitor(void)
{
	if(glo_fInitialized || glo_fInitializing)
		return;
	glo_fInitializing = TRUE;
	DWORD dwVersion = GetVersion();
	// Get major and minor version numbers of Windows
	DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
	DWORD dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));
	// Get build numbers for Windows NT or Win32s
	if (dwVersion>>31 ==0)                // Windows NT
	{
		glo_fFromDynReg=FALSE;
		//add conter here
		if (!m_PerfMon.Initialize())
		{
			glo_fFromDynReg = TRUE;
		}
		else
		{
			m_nRAS = m_PerfMon.AddCounter(CNTR_RAS_RECEIVBYTES);
			if(m_nRAS==-1)
				glo_fSuc = FALSE;
			else
				glo_fSuc = TRUE;
		}
	}
	else         // Windows 95 - 98
		glo_fFromDynReg=TRUE;

	glo_fInitialized = TRUE;
}

void ClearSpeedMonitor(void)
{
	if(!glo_fFromDynReg && glo_fInitialized)
	{
		if(glo_fSuc)
			m_PerfMon.RemoveCounter(0);
		glo_fSuc = FALSE;
		m_PerfMon.Uninitialize();
	}
	glo_fInitialized = FALSE;
}
