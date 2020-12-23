// MenuData.cpp: implementation of the CMenuData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyIE9.h"
#include "MenuData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMenuData::CMenuData()
{
	m_nBitmap=-1;
	m_szMenuText = NULL;
	m_pImgList = NULL;
	m_szURL = NULL;
	m_bIni = TRUE;
}

CMenuData::~CMenuData()
{
	if(m_szMenuText!=NULL)
		delete[] m_szMenuText;
	if(m_szURL!=NULL)
		delete[] m_szURL;
}

BOOL SetMenuText(LPCSTR lpMenuText, CMenuData* md)
{
	if(lpMenuText==NULL || md == NULL)
		return FALSE;
	int i = strlen(lpMenuText);
	if(i>0)
	{
		if(md->m_szMenuText!=NULL)
			delete[] md->m_szMenuText;
		try
		{
			md->m_szMenuText = new char[i+1];
		}
		catch(...)
		{
			md->m_szMenuText = NULL;
			return FALSE;
		}

		if(md->m_szMenuText == NULL)
			return FALSE;
		strcpy(md->m_szMenuText, lpMenuText);
		return TRUE;
	}
	return FALSE;
}

BOOL SetURL(LPCSTR lpszURL, CMenuData* md)
{
	if(lpszURL==NULL || md == NULL)
		return FALSE;
	int i = strlen(lpszURL);
	if(i>0)
	{
		if(md->m_szURL!=NULL)
			delete[] md->m_szURL;
		try
		{
			md->m_szURL = new char[i+1];
		}
		catch(...)
		{
			md->m_szURL = NULL;
			return FALSE;
		}
		strcpy(md->m_szURL, lpszURL);
		return TRUE;
	}
	return FALSE;
}
