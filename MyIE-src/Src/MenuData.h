// MenuData.h: interface for the CMenuData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUDATA_H__D27B31C3_4B00_11D4_9BDF_0000E85300AE__INCLUDED_)
#define AFX_MENUDATA_H__D27B31C3_4B00_11D4_9BDF_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMenuData  
{
public:
	BOOL m_bIni;
	CImageList* m_pImgList;
	CMenuData();
	virtual ~CMenuData();
	int m_nBitmap;
	char *m_szMenuText;
	char *m_szURL;
	DWORD	m_dwProperty;
};

BOOL SetMenuText(LPCSTR lpMenuText, CMenuData* md);
BOOL SetURL(LPCSTR lpszURL, CMenuData* md);

#endif // !defined(AFX_MENUDATA_H__D27B31C3_4B00_11D4_9BDF_0000E85300AE__INCLUDED_)
