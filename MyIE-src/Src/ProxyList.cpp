// ProxyCategories.cpp: implementation of the CProxyCategories class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyIE9.h"
#include "ProxyList.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProxyItem::CProxyItem()
{
	index = -1;
}

CProxyItem::~CProxyItem()
{

}

CProxyList::CProxyList()
{
	m_bIsTransProxy = FALSE;
}

CProxyList::~CProxyList()
{
	CProxyItem* pi;
	for (int i=0;i <= m_Proxies.GetUpperBound();i++)
	{
	   pi = m_Proxies.GetAt(i);
	   delete pi;
	}
	m_Proxies.RemoveAll();
}

CProxyCategory::CProxyCategory()
{

}

CProxyCategory::CProxyCategory(CProxyCategory& pc)
{
	Copy(pc);
}


CProxyCategory::~CProxyCategory()
{
	RemoveAll();
}

void CProxyCategory::RemoveAll()
{
	CProxyList* pl;
	for (int i=0;i <= m_ProxyCategory.GetUpperBound();i++)
	{
	   pl = m_ProxyCategory.GetAt(i);
	   delete pl;
	}
	m_ProxyCategory.RemoveAll();
}

CProxyList* CProxyCategory::GetWebProxyList(BOOL bTrans)
{
	CProxyList* pl;
	for (int i=0;i <= m_ProxyCategory.GetUpperBound();i++)
	{
	   pl = m_ProxyCategory.GetAt(i);
	   if((!bTrans && pl->m_bIsWebProxy && !pl->m_bIsTransProxy) || (bTrans && pl->m_bIsTransProxy))
		   return pl;
	}

	return NULL;
}

CProxyList* CProxyCategory::GetProxyServerList()
{
	CProxyList* pl;
	for (int i=0;i <= m_ProxyCategory.GetUpperBound();i++)
	{
	   pl = m_ProxyCategory.GetAt(i);
	   if(!pl->m_bIsWebProxy && !pl->m_bIsTransProxy)
		   return pl;
	}
	return NULL;
}

int CProxyList::Find(LPCSTR proxy)
{
	for (int i=0;i <= m_Proxies.GetUpperBound();i++)
	{
	   if(m_Proxies.ElementAt(i)->m_strProxy == proxy)
		   return i;
	}
	return -1;
}

void CProxyList::Add(LPCSTR proxy, LPCSTR name, int speed)
{
	try{
	CProxyItem* pi;
	pi = new CProxyItem;
	pi->m_nProxySpeed = speed;
	pi->m_strProxy = proxy;
	pi->m_strProxyName = name;
	m_Proxies.Add(pi);
	}catch(...){}
}

void CProxyCategory::LoadProxies()
{
	char *tmp = NULL;
	m_ProxyCategory.RemoveAll();
	CString filename = theApp.m_strUser+"Proxy.ini";
	TCHAR buf[1024];
	CProxyList* pl;
	CProxyItem *pi;
	CString wp;
	CString wpn;

	int rlens = ::GetPrivateProfileString("Proxy", "CategoryName", _T(""), buf, 1024, filename);
	if(rlens>0)
	{
		//load new one
		char Keyname1[11] = "proxy", Keyname2[11] = "speed", Keyname3[11] = "alias";
		int sp, i, l=1;
		char Section[10]="Proxy";
		do{
			i=1;
			sp = ::GetPrivateProfileInt(Section, "speed1", 3, filename);
			::GetPrivateProfileString(Section, "alias1", _T(""), buf, 1024, filename);
			wpn = buf;
			::GetPrivateProfileString(Section, "proxy1", _T(""), buf, 1024, filename);
			wp = buf;
			pl = new CProxyList;
			while(wp != "" && !wp.IsEmpty())
			{
				pi = new CProxyItem;
				pi->m_nProxySpeed = sp;
				pi->m_strProxy = wp;
				pi->m_strProxyName = wpn;
				pl->m_Proxies.Add(pi);
				i++;
				itoa(i, Keyname1+5, 10);
				itoa(i, Keyname2+5, 10);
				itoa(i, Keyname3+5, 10);
				sp = ::GetPrivateProfileInt(Section, Keyname2, 3, filename);
				::GetPrivateProfileString(Section, Keyname3, _T(""), buf, 1024, filename);
				wpn = buf;
				::GetPrivateProfileString(Section, Keyname1, _T(""), buf, 1024, filename);
				wp = buf;
			}
			::GetPrivateProfileString(Section, "ProxyByPass",	pmf->m_strProxyByPass, buf, 1024, filename);
			pl->m_strProxyByPass = buf;
			pl->m_bIsWebProxy = ::GetPrivateProfileInt(Section, "ProxyType", 0, filename);
			pl->m_bIsTransProxy = ::GetPrivateProfileInt(Section, "Translation", 0, filename);
			::GetPrivateProfileString(Section, "CategoryName", NULL,buf, 1024, filename);
			pl->m_strCategoryName = buf;

			m_ProxyCategory.Add(pl);

			itoa(l, Section+5, 10);
			l++;
			rlens = ::GetPrivateProfileString(Section, "CategoryName", NULL,buf, 1024, filename);

		}while(rlens>0);
	}

	if(GetWebProxyList()==NULL)
	{
		CString str;
		LOADSTR(str ,IDS_WEB_PROXY);

		pl = new CProxyList;
		pl->m_strProxyByPass = "";
		pl->m_bIsWebProxy = TRUE;
		pl->m_strCategoryName = str;
		m_ProxyCategory.Add(pl);
	}
	if(GetWebProxyList(TRUE)==NULL)
	{
		CString str;
		LOADSTR(str ,IDS_TRANS_PROXY);

		pl = new CProxyList;
		pl->m_strProxyByPass = "";
		pl->m_bIsWebProxy = TRUE;
		pl->m_bIsTransProxy = TRUE;
		pl->m_strCategoryName = str;
		m_ProxyCategory.Add(pl);
	}
	if(GetProxyServerList()==NULL)
	{
		CString str;
		LOADSTR(str ,IDS_PROXY_SERVER);
		
		pl = new CProxyList;
		pl->m_strProxyByPass = "<local>";
		pl->m_bIsWebProxy = FALSE;
		pl->m_bIsTransProxy = FALSE;
		pl->m_strCategoryName = str;
		m_ProxyCategory.Add(pl);
	}
}

void CProxyCategory::SaveProxies()
{
	int i = 1;
	char Keyname1[11]="proxy", Keyname2[11]="speed", Keyname3[11]="alias";
	CProxyItem *pi;
	CProxyList* pl;

	CString filename = theApp.m_strUser+"Proxy.ini";
	remove(filename);//!!

	char num[10];
	char Section[10]="Proxy";
	int l=0;
	for (i=0;i <= m_ProxyCategory.GetUpperBound();i++)
	{
	   pl = m_ProxyCategory.GetAt(i);
	   if(pl!=NULL)
	   {
			if(l>0)
			   itoa(l, Section+5, 10);
			WritePrivateProfileSection(Section, NULL, filename);
			for(int ii=0; ii<=pl->m_Proxies.GetUpperBound(); ii++)
			{
				pi = pl->m_Proxies.ElementAt(ii);
				itoa(ii+1, Keyname1+5, 10);
				//app->WriteProfileString(Section, Keyname1, pi->m_strProxy);
				::WritePrivateProfileString(Section, Keyname1, pi->m_strProxy, filename);
				itoa(ii+1, Keyname2+5, 10);
				itoa(pi->m_nProxySpeed, num, 10);
				::WritePrivateProfileString(Section, Keyname2, num, filename);
				itoa(ii+1, Keyname3+5, 10);
				::WritePrivateProfileString(Section, Keyname3, pi->m_strProxyName, filename);
			}
			::WritePrivateProfileString(Section, "CategoryName", pl->m_strCategoryName, filename);
			::WritePrivateProfileString(Section, "ProxyByPass", pl->m_strProxyByPass, filename);
			itoa(pl->m_bIsWebProxy, num, 10);
			::WritePrivateProfileString(Section, "ProxyType",  num, filename);
			itoa(pl->m_bIsTransProxy, num, 10);
			::WritePrivateProfileString(Section, "Translation",  num, filename);
			l++;
		}
	}
}

CProxyItem* CProxyCategory::FindIndex(int index, CProxyList** plist, int* item)
{
	CProxyList* pl;
	CProxyItem* pi;
	int l;
	for (int i=0;i <= m_ProxyCategory.GetUpperBound();i++)
	{
	   pl = m_ProxyCategory.GetAt(i);
	   if((l=pl->FindIndex(index))>=0)
	   {
			pi = pl->m_Proxies.GetAt(l);
			break;
	   }
	}
	*plist = pl;
	*item = i;
	return pi;
}

int CProxyList::FindIndex(int index)
{
	for (int i=0;i <= m_Proxies.GetUpperBound();i++)
	{
	   if(m_Proxies.ElementAt(i)->index == index)
		   return i;
	}
	return -1;
}


void CProxyCategory::Copy(CProxyCategory &pc)
{
	RemoveAll();

	CProxyList* pl, *plcopy;
	CProxyItem *pi;
	for (int i=0;i <= pc.m_ProxyCategory.GetUpperBound();i++)
	{
	   pl = pc.m_ProxyCategory.GetAt(i);
	   if(pl!=NULL)
	   {
		   plcopy= new CProxyList;
		   plcopy->m_bIsWebProxy = pl->m_bIsWebProxy;
		   plcopy->m_bIsTransProxy = pl->m_bIsTransProxy;
		   plcopy->m_strCategoryName = pl->m_strCategoryName;
		   plcopy->m_strProxyByPass = pl->m_strProxyByPass;
		   for(int l=0; l<=pl->m_Proxies.GetUpperBound(); l++)
		   {
			   pi = new CProxyItem;
			   pi->index = pl->m_Proxies.GetAt(l)->index;
			   pi->m_nProxySpeed = pl->m_Proxies.GetAt(l)->m_nProxySpeed;
			   pi->m_strProxy = pl->m_Proxies.GetAt(l)->m_strProxy;
			   pi->m_strProxyName = pl->m_Proxies.GetAt(l)->m_strProxyName;
			   plcopy->m_Proxies.Add(pi);
		   }
		   m_ProxyCategory.Add(plcopy);
	   }
	}
}

//#######################################################################
CSearchList::~CSearchList()
{
	Clean();
}

void CSearchList::Clean()
{
	CSearch* item;
	for (int i=0;i <= m_Searchs.GetUpperBound();i++)
	{
	   item = m_Searchs.GetAt(i);
	   delete item;
	}
	m_Searchs.RemoveAll();
}

int CSearchList::Lookup(LPCTSTR key, CString& rValue)
{
	for (int i=0;i <= m_Searchs.GetUpperBound();i++)
	{
	   if(m_Searchs.ElementAt(i)->name == key)
	   {
		   rValue = m_Searchs.ElementAt(i)->url;
		   return i;
	   }
	}
	return -1;
}

int CSearchList::GetIconIndex(int nIndex)
{
	int nCount = 0;
	for (int i=0;i <= m_Searchs.GetUpperBound();i++)
	{
		if(m_Searchs.ElementAt(i)->icon.GetLength())
		{
			if (nCount==nIndex)
				return i;
			nCount++;
		}
	}
	return -1;
}

BOOL CSearchList::LoadSearchList()
{
	m_nIconCount = 0;
	//
	CSearch *item;
	TCHAR key[10];
	CString url, alias, icon;
	int r = TRUE;
	int i = 0,rlens;
	int state ;
	TCHAR buf[1024];
	CString filename = theApp.m_strUser+"SearchEngine.ini";
	while(r)
	{
		key[0] = 'N';
		itoa(i, key+1, 10);
		rlens = ::GetPrivateProfileString("SearchEngine", key, _T(""), buf, 1024, filename);
		if(rlens==0)
			r=FALSE;
		else
		{
			alias = buf;
			//
			key[0] = 'S';
			itoa(i, key+1, 10);
			rlens = ::GetPrivateProfileString("SearchEngine", key, _T(""), buf, 1024, filename);
			url = buf;
			//
			key[0] = 'C';//selected
			state = ::GetPrivateProfileInt("SearchEngine", key, 0, filename);
			//
			key[0] = 'I';//icon
			itoa(i, key+1, 10);
			rlens = ::GetPrivateProfileString("SearchEngine", key, _T(""), buf, 1024, filename);
			icon = buf;
			//
			item = new CSearch;
			item->state = state;
			item->name = alias;
			item->url = url;
			item->icon = icon;
			m_Searchs.Add(item);
			if (icon.GetLength())
				m_nIconCount++;
		}
		i++;
	}
	
	return TRUE;
}

//#######################################################################
CLinkList::~CLinkList()
{
	Clean();
}

void CLinkList::Clean()
{
	CLink* item;
	for (int i=0;i <= m_Links.GetUpperBound();i++)
	{
	   item = m_Links.GetAt(i);
	   delete item;
	}
	m_Links.RemoveAll();
}

//#######################################################################
CGestureList::~CGestureList()
{
	Clean();
}

void CGestureList::Clean()
{
	CGesture* item;
	for (int i=0;i <= m_Gestures.GetUpperBound();i++)
	{
	   item = m_Gestures.GetAt(i);
	   delete item;
	}
	m_Gestures.RemoveAll();
}

int CGestureList::GetCmdFromName(LPCSTR name)
{
	CGesture *item;
	for (int i=0;i <= m_Gestures.GetUpperBound();i++)
	{
		item = m_Gestures.ElementAt(i);
		if( item->gesture == name)
		   return item->command;
	}
	return 0;
}

void CGestureList::SaveGestureList()
{
	CGesture *item;
	TCHAR key[10];
	for (int i=0;i <= m_Gestures.GetUpperBound();i++)
	{
		item = m_Gestures.ElementAt(i);
		key[0] = 'N';
		itoa(i, key+1, 10);
		theApp.WriteProfileString("MouseGesture", key, item->gesture);
		//
		key[0] = 'C';
		itoa(i, key+1, 10);
		theApp.WriteProfileInt("MouseGesture", key, item->command);
	}
	//
	key[0] = 'N';
	itoa(i, key+1, 10);
	theApp.WriteProfileString("MouseGesture", key, "");
}

BOOL CGestureList::LoadGestureList()
{
	CGesture *item;
	TCHAR key[10];
	CString strName;
	int command;
	int i=0;
	
	while(1)
	{
		key[0] = 'N';
		itoa(i, key+1, 10);
		strName = theApp.GetProfileString("MouseGesture", key, "");
		if (strName.GetLength()<1)
			break;
		key[0] = 'C';
		itoa(i, key+1, 10);
		command = theApp.GetProfileInt("MouseGesture", key, 0);
		//
		item = new CGesture;
		item->gesture = strName;
		item->command = command;
		m_Gestures.Add(item);
		i++;
	}

	return TRUE;
}
//#######################################################################
CFavIconList::~CFavIconList()
{
	Clean();
}

void CFavIconList::Clean()
{
	CFavIcon* item;
	for (int i=0;i <= m_FavIcons.GetUpperBound();i++)
	{
		item = m_FavIcons.GetAt(i);
		delete item;
	}
	m_FavIcons.RemoveAll();
}

int CFavIconList::Find(LPCSTR strRoot)
{
	CFavIcon* item;
	int nCount = m_FavIcons.GetSize();
	for (int i=0;i < nCount; i++)
	{
		item = m_FavIcons.GetAt(i);
		if (item->strRoot==strRoot)
			return item->nIcon;
	}
	return FI_NOT_IN_LIST;
}

void CFavIconList::Insert(LPCSTR strRoot, int nIcon)
{
	CFavIcon* item;
	int nCount = m_FavIcons.GetSize();
	for (int i=0;i < nCount; i++)
	{
		item = m_FavIcons.GetAt(i);
		if (item->strRoot==strRoot)
		{
			item->nIcon = nIcon;
			return;
		}
	}
	//inser new
	try{
	item = new CFavIcon;
	item->strRoot = strRoot;
	item->nIcon = nIcon;
	m_FavIcons.Add(item);
	}catch(...){}
}