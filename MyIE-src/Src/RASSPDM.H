// Rasspdmon.h : interface of the RAS speed monitor
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RASSPDMON_H__19E497C1_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_)
#define AFX_RASSPDMON_H__19E497C1_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_


#ifndef __RASSPDMON_H_
#define __RASSPDMON_H_

extern BOOL glo_fInitialized, glo_fInitializing;

void InitSpeedMonitor(void);
void ClearSpeedMonitor(void);
void GetRASSpeed(/*DWORD* pTPS,DWORD* pRPS,DWORD* pTT,*/ DWORD* pRT);
////参数：每秒发送，每秒接收，总发送，总接受
UINT _cdecl TInitialMonitor(LPVOID con);


#endif



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RASSPDMON_H__19E497C1_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_)
