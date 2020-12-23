// HistoryTree.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "HistoryTree.h"
#include <Wininet.h>
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CHistoryTree

CHistoryTree::CHistoryTree()
{
   m_pMalloc = NULL;
   SHGetMalloc(&m_pMalloc);
   m_bIE4 = FALSE;
}

CHistoryTree::~CHistoryTree()
{
	m_wndHisTree.DeleteAllItems();
	m_wndHisTree.m_astrHist.RemoveAll();
    m_pMalloc->Release();
}

/////////////////////////////////////////////////////////////////////////////
// CHistoryTree message handlers

LPCSTR CHistoryTree::GetTitle()
{
	LOADSTR(strTitle ,IDS_TITLE_HIS);
	return (LPCSTR)strTitle;
}

BOOL CHistoryTree::Initialize()
{
	// TODO: Add your command handler code here
	
	m_wndHisTree.DeleteAllItems();
	m_wndHisTree.m_astrHist.RemoveAll();
	
	LPITEMIDLIST pidlHistory = NULL;	
	HRESULT hr = SHGetSpecialFolderLocation(NULL, CSIDL_HISTORY, &pidlHistory);
	if (NOERROR != hr)
	{
	   m_bIE4=TRUE;
	   //for ie4 or win95
	   CWaitCursor wc;
	   
	   IUrlHistoryStg2* pUrlHistoryStg2 = NULL;
	   HRESULT hr = CoCreateInstance(CLSID_CUrlHistory,
		   NULL, CLSCTX_INPROC, IID_IUrlHistoryStg2,
		   (void**)&pUrlHistoryStg2);
	   if (SUCCEEDED(hr))
	   {     
		   IEnumSTATURL* pEnum;
		   hr = pUrlHistoryStg2->EnumUrls(&pEnum);
		   ULONG size;
		   STATURL staturl;
		   int ind=1;
		   hr = pEnum->Next(-1,&staturl, &size);
		   if(SUCCEEDED(hr))
		   {
			   m_wndHisTree.DeleteAllItems();
			   m_wndHisTree.m_astrHist.RemoveAll();
		   }
		   CTime ttime = CTime::GetCurrentTime();
		   today = ttime.GetDayOfWeek();
		   int t;
		   if(today==1)
			   t=8;
		   else
			   t=today;
		   CTimeSpan tt(t-2,0,0,0);
		   ttime =ttime - tt;
		   CTime t2(ttime.GetYear(), ttime.GetMonth(), ttime.GetDay(),0,0,0);
		   montime = t2;
		   while(size>0)
		   {
			   if(insertUrl(&staturl,ind))
			   {
				   m_wndHisTree.m_astrHist.Add(staturl.pwcsUrl);
				   ind++;
			   }
			   pEnum->Next(-1,&staturl, &size);
		   }
		   pEnum->Release();
		   pUrlHistoryStg2->Release();
	   }
	   return FALSE;
   }
   
   IShellFolder* psfDesktop;
   IShellFolder* psfHistory;
   hr = SHGetDesktopFolder(&psfDesktop);
   if (NOERROR != hr)
   {
	   return FALSE;
   }
   hr = psfDesktop->BindToObject(pidlHistory, NULL, IID_IShellFolder,
	   (LPVOID*)&psfHistory);
   psfDesktop->Release();
   if (NOERROR != hr)
   {
	   return FALSE;
   }
   
   AddHistory(psfHistory, NULL);
   m_pMalloc->Free(pidlHistory);
   psfHistory->Release();
   
   return TRUE;
}

BOOL CHistoryTree::Create(CWnd* pParent)
{
	if (!m_wndHisTree.Create(WS_CHILD|WS_VISIBLE|TVS_SHOWSELALWAYS|TVS_LINESATROOT|
		TVS_FULLROWSELECT |TVS_TRACKSELECT ,
		CRect(0,0,0,0), pParent, 100))
	{
		TRACE0("Failed to create instant bar child\n");
		return 0;		// fail to create
	}
	m_wndHisTree.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	// bring the tooltips to front
	CWnd* pTT = m_wndHisTree.FromHandle((HWND) m_wndHisTree.SendMessage(TVM_GETTOOLTIPS));
    if (pTT != NULL)
		pTT->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,
			SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);

	CImageList img;
	img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 4, 1);
	HBITMAP hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_HISTORY));
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);
	m_wndHisTree.SetImageList(&img, TVSIL_NORMAL);
	img.Detach();

	m_wndHisTree.m_pHisTree = this;

	return TRUE;
}

CWnd* CHistoryTree::GetWindow()
{
	return (CWnd*)&m_wndHisTree;
}

void CHistoryTree::Destroy()
{
	m_wndHisTree.DeleteAllItems();
	m_wndHisTree.m_astrHist.RemoveAll();
}

//for ie4 and win95
BOOL CHistoryTree::insertUrl(LPSTATURL pSUrl, int ind)
{
	//get host
	CString url = pSUrl->pwcsUrl;
	CString host;
	CString l = url.Left(5);
	int i;
	if(l=="https")
	{
		host = url.Right(url.GetLength()-8);
		i=host.Find('/');
		if(i>0)
			host=host.Left(i);
	}
	if(l=="http:")
	{
		host = url.Right(url.GetLength()-7);
		i=host.Find('/');
		if(i>0)
			host=host.Left(i);
	}
	else if(l=="ftp:/")
	{
		host = url.Right(url.GetLength()-6);
		i=host.Find('/');
		if(i>0)
			host=host.Left(i);
	}
	else if(l=="file:")
	{
		LOADSTR(host ,IDS_MY_COMPUTER);
	}
	else
		return FALSE;
	//get date
	CTime ttime(pSUrl->ftLastVisited);
	CTime ttime2(pSUrl->ftLastUpdated);
	if( ttime < ttime2)
		ttime=ttime2;

	CTime filetime(ttime.GetYear(), ttime.GetMonth(), ttime.GetDay(),0,0,0);
	int d;
	int week = 0, data;
	CString root;
	CTimeSpan ts = montime-filetime;
	if(filetime < montime)
	{
		week=(ts.GetDays()+6)/7;
	}

	if(week>1)
	{
		char tw[4];
		itoa(week, tw,10);
		LOADSTR(root ,IDS_WEEKS_AGO);
		root = tw+root;
		data = -week-7;
	}
	else if(week==1)
	{
		LOADSTR(root ,IDS_LAST_WEEK);
		data = -8;
	}
	else
	{
		d=filetime.GetDayOfWeek();
		if(d==today)
			{LOADSTR(root ,IDS_TODAY);}
		else
			{LOADSTR(root ,IDS_SUNDAY+d-1);}
		data = -9+d;
		if(d==1)
			data=-1;
	}
	
	HTREEITEM hitem;
	hitem = FindAndInsert(NULL, root, 0,0, data);
	hitem = FindAndInsert(hitem, host, 1,3);

	CString temp;
	temp = pSUrl->pwcsTitle;
	if(temp.IsEmpty())
	{
		temp=pSUrl->pwcsUrl;
		i=temp.ReverseFind('/');
		temp=temp.Right(temp.GetLength()-i-1);
	}
	HTREEITEM hnode = m_wndHisTree.InsertItem(temp, 2, 2, hitem);
	m_wndHisTree.SetItemData(hnode, ind);

	return TRUE;
}

HTREEITEM CHistoryTree::FindAndInsert(HTREEITEM  hRoot, LPCSTR text, int nImg, int nOImg,int data)
{
	HTREEITEM hit, hnode = NULL, pre=TVI_FIRST;
	BOOL found=FALSE;
	hit = m_wndHisTree.GetChildItem(hRoot);
	if(data>=0)
	{
		while(hit!=NULL && !found)
		{
			if(m_wndHisTree.GetItemText(hit).CompareNoCase(text)==1)
			{
				hnode = m_wndHisTree.InsertItem(text, nImg, nOImg, hRoot,TVI_SORT);
				found = TRUE;
			}
			else if(m_wndHisTree.GetItemText(hit).CompareNoCase(text)==0)
			{
				found = TRUE;
				hnode = hit;
			}
			else
				hit = m_wndHisTree.GetNextSiblingItem(hit);
		}
	}
	else //compare with data
	{
		while(hit!=NULL && !found)
		{
			if((signed)m_wndHisTree.GetItemData(hit)>data)
			{
				hnode = m_wndHisTree.InsertItem(text, nImg, nOImg, hRoot,pre);
				found = TRUE;
			}
			else if((signed)m_wndHisTree.GetItemData(hit)==data)
			{
				found = TRUE;
				hnode = hit;
			}
			else
			{
				pre=hit;
				hit = m_wndHisTree.GetNextSiblingItem(hit);
			}
		}
	}

	if(!found)
		hnode = m_wndHisTree.InsertItem(text, nImg, nOImg, hRoot,TVI_LAST);

	m_wndHisTree.SetItemData(hnode, data);

	return hnode;
}

BOOL CHistoryTree::Update(int nLevel)
{
	if(m_bIE4)
	{
		HTREEITEM hit, pre=NULL;
		BOOL found=FALSE;
		int d;
		CString text;
		LOADSTR(text ,IDS_TODAY);
		//now only update today.
		if (nLevel == 0)
		{
			//get today item
			hit = m_wndHisTree.GetChildItem(NULL);
			while(hit!=NULL && !found)
			{
				if(m_wndHisTree.GetItemText(hit).CompareNoCase(text)==0)
				{
					found = TRUE;
				}
				else
				{
					pre = hit;
					hit = m_wndHisTree.GetNextSiblingItem(hit);
				}
			}
			int day;
			if(found)
			{
				d = (int)m_wndHisTree.GetItemData(hit);
				day=d+9;
				if(d==-1)
					day=1;

				//delete today
				m_wndHisTree.DeleteItem(hit);
				m_wndHisTree.Expand(pre, TVE_COLLAPSE);
			}
			else
				day=today;

			//insert history after that day
			CWaitCursor wc;
			IUrlHistoryStg2* pUrlHistoryStg2 = NULL;
			HRESULT hr = CoCreateInstance(CLSID_CUrlHistory,
				NULL, CLSCTX_INPROC, IID_IUrlHistoryStg2,
				(void**)&pUrlHistoryStg2);
			if (SUCCEEDED(hr))
			{     
				IEnumSTATURL* pEnum;
				hr = pUrlHistoryStg2->EnumUrls(&pEnum);
				ULONG size;
				STATURL staturl;
				int ind=m_wndHisTree.m_astrHist.GetUpperBound()+2;
				hr = pEnum->Next(-1,&staturl, &size);
				if(!SUCCEEDED(hr))
				{
					return FALSE;
				}
				CTime ttime = CTime::GetCurrentTime();
				today = ttime.GetDayOfWeek();
				CTime lastday;
				if(today=day)
				{
					CTime t1(ttime.GetYear(), ttime.GetMonth(), ttime.GetDay(),0,0,0);
					lastday = t1;
				}
				else
				{
					CTimeSpan tsp(1,0,0,0);
					ttime = ttime-tsp;
					CTime t1(ttime.GetYear(), ttime.GetMonth(), ttime.GetDay(),0,0,0);
					lastday = t1;
					ttime = ttime +tsp;
				}
				int t;
				if(today==1)
					t=8;
				else
					t=today;
				CTimeSpan tt(t-2,0,0,0);
				ttime =ttime - tt;

				CTime t2(ttime.GetYear(), ttime.GetMonth(), ttime.GetDay(),0,0,0);
				montime = t2;

				while(size>0)
				{
					CTime t3(staturl.ftLastVisited);
					if(t3>=lastday)
					{
						if(insertUrl(&staturl,ind))
						{
							m_wndHisTree.m_astrHist.Add(staturl.pwcsUrl);
							ind++;
						}
					}
					pEnum->Next(-1,&staturl, &size);
				}
				pEnum->Release();
				pUrlHistoryStg2->Release();
			}
			return TRUE;
		}
		return TRUE;
	}

	//following is for ie5 and win98
	//remove the children of Today
	HTREEITEM hit;
	BOOL found=FALSE;
	CString text;
	LOADSTR(text ,IDS_TODAY);
	//get today item
	hit = m_wndHisTree.GetChildItem(NULL);
	while(hit!=NULL && !found)
	{
		if(m_wndHisTree.GetItemText(hit).CompareNoCase(text)==0)
			found = TRUE;
		else
			hit = m_wndHisTree.GetNextSiblingItem(hit);
	}
	if(found)
	{
		//delete today
		m_wndHisTree.DeleteItem(hit);
		//m_wndHisTree.Expand(pre, TVE_COLLAPSE);
	}
	LPITEMIDLIST pidlHistory = NULL;	
	HRESULT hr = SHGetSpecialFolderLocation(NULL, CSIDL_HISTORY, &pidlHistory);
	if (NOERROR != hr)
		return FALSE;
	
	IShellFolder* psfDesktop;
	IShellFolder* psfHistory;	
	hr = SHGetDesktopFolder(&psfDesktop);
	if (NOERROR != hr)
		return FALSE;	
	hr = psfDesktop->BindToObject(pidlHistory, NULL, IID_IShellFolder,
							 (LPVOID*)&psfHistory);
	psfDesktop->Release();	
	if (NOERROR != hr)
		return FALSE;
	
	AddHistory(psfHistory, NULL,0,0,TRUE);	
	m_pMalloc->Free(pidlHistory);	
	psfHistory->Release();
	
	return TRUE;
}

BOOL CHistoryTree::StrRetToStr(STRRET StrRet, LPTSTR *str, LPITEMIDLIST pidl)
{
	HRESULT hr = S_OK;
	int cch;
	LPSTR strOffset;	
	*str = NULL;  // Assume failure
	
	switch (StrRet.uType)
	{
	case STRRET_WSTR: 
		cch = WideCharToMultiByte(CP_OEMCP, 0, StrRet.pOleStr, -1, NULL, 0, NULL, NULL); 
		*str = (LPTSTR)m_pMalloc->Alloc(cch * sizeof(TCHAR));		
		if (*str != NULL)
			WideCharToMultiByte(CP_OEMCP, 0, StrRet.pOleStr, -1, *str, cch, NULL, NULL); 
		else
			hr = E_FAIL;
		break;
		
	case STRRET_OFFSET: 
		strOffset = (((char *) pidl) + StrRet.uOffset);		
		cch = strlen(strOffset) + 1; // NULL terminator
		*str = (LPTSTR)m_pMalloc->Alloc(cch * sizeof(TCHAR));		
		if (*str != NULL)
			strcpy(*str, strOffset);
		else
			hr = E_FAIL;
		break;
		
	case STRRET_CSTR: 
		cch = strlen(StrRet.cStr) + 1; // NULL terminator
		*str = (LPTSTR)m_pMalloc->Alloc(cch * sizeof(TCHAR));		
		if (*str != NULL)
			strcpy(*str, StrRet.cStr);
		else
			hr = E_FAIL;		
		break;
	} 
	
	return TRUE;
}

void CHistoryTree::AddHistory(IShellFolder *pFolder, HTREEITEM root, int nImg, int nOImg, BOOL bNeedCmp)
{
	if(pFolder == NULL)
		return;
	
	//enum history
	IEnumIDList* pItems = NULL;
	LPITEMIDLIST pidlNext = NULL;
	STRRET StrRetName;
	LPTSTR lpszName = NULL;
	LPTSTR lpszURL = NULL;
	DWORD dwEntrySize=0;
	
	// Enumerate all object in the given folder
	HRESULT hr = pFolder->EnumObjects(NULL, SHCONTF_FOLDERS|SHCONTF_NONFOLDERS|SHCONTF_INCLUDEHIDDEN , &pItems);
	while (NOERROR == hr)
	{
		hr = pItems->Next(1, &pidlNext, NULL);
		if (NOERROR == hr)
		{
			pFolder->GetDisplayNameOf(pidlNext, SHGDN_NORMAL, &StrRetName);
			StrRetToStr(StrRetName, &lpszName, pidlNext);
			//add folder
			BOOL found = FALSE;
			if(bNeedCmp)
			{
				HTREEITEM hit;
				//get today item
				hit = m_wndHisTree.GetChildItem(root);
				while(hit!=NULL && !found)
				{
					if(m_wndHisTree.GetItemText(hit).CompareNoCase(lpszName)==0)
						found = TRUE;
					else
						hit = m_wndHisTree.GetNextSiblingItem(hit);
				}
			}
			if(!found)
			{
				HTREEITEM hItem;
				if(nImg==0)
					hItem = m_wndHisTree.InsertItem(lpszName, nImg, nOImg, root ,TVI_LAST);
				else
					hItem = m_wndHisTree.InsertItem(lpszName, nImg, nOImg, root ,TVI_SORT);
				if(nImg==2)
					ResolveHistory(pFolder, pidlNext, &lpszURL);
				if (lpszURL)
				{
					// Add the URL to the array and free lpszURL
					// since it was created with IMalloc::Alloc
					m_wndHisTree.m_astrHist.Add(lpszURL);
					m_wndHisTree.SetItemData(hItem, m_wndHisTree.m_astrHist.GetSize());
					//is the url cached?
					dwEntrySize = 0;
					if(strncmp(lpszURL, "file:", 5) != 0)
					{
						if (!GetUrlCacheEntryInfoEx(lpszURL,NULL,&dwEntrySize, NULL, NULL, NULL, 0))
						{
							if (GetLastError()==ERROR_FILE_NOT_FOUND)
							{
								m_wndHisTree.SetItemState(hItem, TVIS_CUT, TVIS_CUT);
							}
						}
					}
					m_pMalloc->Free(lpszURL);
					lpszURL = NULL;
				}
			}
			if (lpszName)
				m_pMalloc->Free(lpszName);
			if (pidlNext)
				m_pMalloc->Free(pidlNext);
		}
	}
	if (pItems)
		pItems->Release();
}

BOOL CHistoryTree::ResolveHistory(IShellFolder* pFolder, LPCITEMIDLIST pidl, LPTSTR* lpszURL)
{
	IShellLink* pShellLink;
	*lpszURL = NULL;  // Assume failure
	
	// Get a pointer to the IShellLink interface from the given folder
	HRESULT hr = pFolder->GetUIObjectOf(NULL, 1, &pidl, IID_IShellLink, NULL, (LPVOID*)&pShellLink);
	if (SUCCEEDED(hr))
	{
		WIN32_FIND_DATA wfd;      
		hr = pShellLink->Resolve(AfxGetApp()->m_pMainWnd->m_hWnd, SLR_NO_UI); 
		if (NOERROR == hr)
		{
			// Get the path to the link target. 
			*lpszURL = (LPTSTR)m_pMalloc->Alloc(MAX_PATH);  // Must remember to Free later
			hr = pShellLink->GetPath(*lpszURL, MAX_PATH - 1, (WIN32_FIND_DATA*)&wfd, SLGP_UNCPRIORITY);
		}
		pShellLink->Release();
	}
	
	if(SUCCEEDED(hr))
		return TRUE;
	else
		return FALSE;
}

//#pragma optimize( "s", off)
