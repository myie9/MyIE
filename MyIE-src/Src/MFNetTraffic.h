/*******************************************

	MFTrafficButton

	Version:	1.0
	Date:		31.10.2001
	Author:		Michael Fatzi
	Mail:		Michael_Fatzi@hotmail.com
	Copyright 1996-1997, Keith Rule

	You may freely use or modify this code provided this
	Copyright is included in all derived versions.
	
	History: 10.2001 Startup

	Handy little button control to display current 
	nettraffic as graph in a button.

********************************************/

// MFNetTraffic.h: interface for the MFNetTraffic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFNETTRAFFIC_H__9CA9C41F_F929_4F26_BD1F_2B5827090494__INCLUDED_)
#define AFX_MFNETTRAFFIC_H__9CA9C41F_F929_4F26_BD1F_2B5827090494__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>


class MFNetTraffic  
{
public:
	enum TrafficType 
	{
		AllTraffic		= 388,
		IncomingTraffic	= 264,
		OutGoingTraffic	= 506
	};

	void SetTrafficType(int trafficType);
	DWORD	GetInterfaceTotalTraffic(int index);
	BOOL	GetNetworkInterfaceName(CString *InterfaceName, int index);
	int		GetNetworkInterfacesCount();
	double	GetTraffic(int interfaceNumber);

	DWORD	GetInterfaceBandwidth(int index);
	MFNetTraffic();
	virtual ~MFNetTraffic();
private:
	BOOL		GetInterfaces();
	double		lasttraffic;
	CStringList Interfaces;
	CList < DWORD, DWORD &>		Bandwidths;
	CList < DWORD, DWORD &>		TotalTraffics;
	int CurrentInterface;
	int CurrentTrafficType;

	DWORD m_size;//gf buffer
	unsigned char *data;

};

#endif // !defined(AFX_MFNETTRAFFIC_H__9CA9C41F_F929_4F26_BD1F_2B5827090494__INCLUDED_)
