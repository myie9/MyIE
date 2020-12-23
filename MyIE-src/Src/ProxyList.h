// ProxyCategories.h: interface for the CProxyCategories class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROXYCATEGORIES_H__04AF675F_3E8C_4216_8158_4B138DE19CBC__INCLUDED_)
#define AFX_PROXYCATEGORIES_H__04AF675F_3E8C_4216_8158_4B138DE19CBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Consts_gf.h"

class CProxyItem
{
public:
	int index;
	CProxyItem();
	virtual ~CProxyItem();
	CString m_strProxyName;
	CString m_strProxy;
	int m_nProxySpeed;
};

class CProxyList
{
public:
	int FindIndex(int index);
	void Add(LPCSTR proxy, LPCSTR name, int speed);
	int Find(LPCSTR proxy);
	CProxyList();
	virtual ~CProxyList();
	CArray<CProxyItem*, CProxyItem*> m_Proxies;
	CString m_strProxyByPass;
	CString m_strCategoryName;
	BOOL m_bIsWebProxy, m_bIsTransProxy;
};

class CProxyCategory
{
public:
	void Copy(CProxyCategory& pc);
	CProxyItem* FindIndex(int index, CProxyList** plist, int* item);
	void SaveProxies();
	void LoadProxies();
	CProxyList* GetWebProxyList(BOOL bTrans = FALSE);
	CProxyList* GetProxyServerList();
	void RemoveAll();
	CProxyCategory();
	CProxyCategory(CProxyCategory& pc);
	virtual ~CProxyCategory();
	CArray<CProxyList*, CProxyList*> m_ProxyCategory;
};
//########################################################
class CSearch
{
public:
	int	state;
	CString name;
	CString url;
	CString icon;
};

class CSearchList
{
public:
	void Clean();
	int Lookup(LPCTSTR key, CString& rValue);
	BOOL LoadSearchList();
	int  GetIconIndex(int nIndex);
	int  m_nIconCount;
	virtual ~CSearchList();
	CArray<CSearch*, CSearch*> m_Searchs;
};
//########################################################
class CLink
{
public:
	CString name;
	CString url;
};

class CLinkList
{
public:
	void Clean();
	virtual ~CLinkList();
	CArray<CLink*, CLink*> m_Links;
};

//#######################################################
class CGesture
{
public:
	CString gesture;
	int command;
};

class CGestureList
{
public:
	void Clean();
	BOOL LoadGestureList();
	void SaveGestureList();
	int GetCmdFromName(LPCSTR name);
	virtual ~CGestureList();
	CArray<CGesture*, CGesture*> m_Gestures;
};

//#######################################################
class CFavIcon
{
public:
	int nIcon;
	CString strRoot;
};


class CFavIconList
{
public:
	void Clean();
	int  Find(LPCSTR strRoot);
	void Insert(LPCSTR strRoot, int nIcon);
	virtual ~CFavIconList();
	CArray<CFavIcon*, CFavIcon*> m_FavIcons;
};

#endif // !defined(AFX_PROXYCATEGORIES_H__04AF675F_3E8C_4216_8158_4B138DE19CBC__INCLUDED_)
