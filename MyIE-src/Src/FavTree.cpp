// FavTree.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "FavTree.h"
#include "MainFrm.h"
#include "MenuOrder.h"
#include "Gf_IE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CFavTree

CFavTree::CFavTree()
{
	m_bFirstInitial = TRUE;
}

CFavTree::~CFavTree()
{
	try{
	m_wndFav.m_FavTreeCtrl.DeleteAllItems();
	m_wndFav.m_FavTreeCtrl.m_astrFav.RemoveAll();
	}catch(...){}
}

/////////////////////////////////////////////////////////////////////////////
// CFavTree message handlers

LPCSTR CFavTree::GetTitle()
{
	LOADSTR(strTitle ,IDS_TITLE_FAVORITE);
	return (LPCSTR)strTitle;
}

BOOL CFavTree::Initialize()
{
	// TODO: Add your command handler code here
	CWaitCursor	wc;
	CString		szPath;

	// find out from the registry where the favorites are located.
	if(!pmf->GetFavoriteFolder(szPath))
		return FALSE;

	if(m_bFirstInitial)
	{
		m_bFirstInitial = FALSE;
	}

	HTREEITEM hRoot=NULL;
	BuildFavorites(szPath,0, hRoot);
	m_wndFav.m_FavTreeCtrl.Expand(hRoot,TVE_EXPAND);
	if(szPath == pmf->m_strStartFavFolder)
	{
		m_wndFav.m_FavTreeCtrl.SetItemState(hRoot,TVIS_BOLD,TVIS_BOLD);
		m_wndFav.m_FavTreeCtrl.m_hStartFolder = hRoot;
	}
	m_wndFav.m_FavTreeCtrl.m_bDragging = FALSE;

	return TRUE;
}

BOOL CFavTree::Create(CWnd* pParent)
{
	m_wndFav.Create(NULL,NULL, WS_CHILD|WS_VISIBLE,CRect(0,0,0,0), pParent, 100);
	m_wndFav.m_FavTreeCtrl.m_pFavTree = this;
	return TRUE;
}

CWnd* CFavTree::GetWindow()
{
	return (CWnd*)&m_wndFav;
}

void CFavTree::Destroy()
{
	try{
	m_wndFav.m_FavTreeCtrl.DeleteAllItems();
	m_wndFav.m_FavTreeCtrl.m_astrFav.RemoveAll();
	}catch(...){}
}

BOOL CFavTree::Update(int nLevel)
{
	m_wndFav.LockWindowUpdate();
	m_wndFav.m_FavTreeCtrl.DeleteAllItems();
	m_wndFav.m_FavTreeCtrl.m_astrFav.RemoveAll();
	Initialize();
	m_wndFav.UnlockWindowUpdate();
	m_wndFav.m_FavTreeCtrl.m_bDragging = FALSE;

	return TRUE;
}

int CFavTree::BuildFavorites(LPCTSTR pszPath, int nStartPos, HTREEITEM hItem, int nLevel)
{
	CString         strPath(pszPath);
	CString         strPath2;
	CString         str,str2;
	WIN32_FIND_DATA wfd;
	HANDLE          h;
	int             nPos;
	int             nEndPos;
	int             nLastDir;
	TCHAR           buf[2048];
	CStringArray    astrFavorites;
	CArray<int, int>	anFavID;
	CStringArray    astrDirs;
	HTREEITEM		hnode;
	BOOL			bFolderOnly = m_wndFav.m_bFolderOnly;

	// make sure there's a trailing backslash
	if(strPath[strPath.GetLength() - 1] != _T('\\'))
		strPath += _T('\\');
	strPath2 = strPath;
	strPath += "*.*";

	nLastDir = 0;
	// now scan the directory, first for .URL files and then for subdirectories
	// that may also contain .URL files
	h = FindFirstFile(strPath, &wfd);
	DWORD fileattr;
	if(pmf->m_bShowHiddenFav == 1)
		fileattr = FILE_ATTRIBUTE_DIRECTORY;
	else
		fileattr = FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_HIDDEN;
	if(h != INVALID_HANDLE_VALUE)
	{
		nEndPos = nStartPos;
		do
		{
			if((wfd.dwFileAttributes & fileattr)==0)
			{
				str = wfd.cFileName;str2=str;
				if (IS_FAVURL(str))
				{
					// an .URL file is formatted just like an .INI file, so we can
					// use GetPrivateProfileString() to get the information we want
					::GetPrivateProfileString(_T("InternetShortcut"), _T("URL"),
											  _T(""), buf, 2048,
											  strPath2 + str);
					if(!bFolderOnly)
					{
						nPos = nEndPos;
						astrFavorites.Add(str2);
						anFavID.Add(nPos);
						m_wndFav.m_FavTreeCtrl.m_astrFav.AddTail(buf);
						++nEndPos;
					}
				}
				else  if(!bFolderOnly && str.Right(4) == _T(".lnk"))
				{
					nPos = nEndPos;
					m_wndFav.m_FavTreeCtrl.m_astrFav.AddTail(strPath2 + str);
					astrFavorites.Add(str2);
					anFavID.Add(nPos);
					++nEndPos;
				}
			}
			else if((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (pmf->m_bShowHiddenFav==1 || (wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)==0) )
			{
				// ignore the current and parent directory entries
				if(lstrcmp(wfd.cFileName, _T(".")) == 0 || lstrcmp(wfd.cFileName, _T("..")) == 0)
					continue;
				astrDirs.Add(wfd.cFileName);
				++nLastDir;
			}
		} while(FindNextFile(h, &wfd));
		FindClose(h);

		//sort and add to tree
		CMenuOrder mo;
		HTREEITEM hLastDir=TVI_LAST;
		BOOL found;
		int j;
		WORD type;
		if(pmf->GetFavoriteFolder(strPath))
		{
			strPath = strPath2.Mid(strPath.GetLength()+1);
			if(mo.LoadMenuOrder(strPath))
			{
				for(int i = 0; i<mo.len; i++)
				{
					str = mo.mois[mo.index[i]].longname;
					type = mo.mois[mo.index[i]].filetype;
					//find in url first
					found = FALSE;
					for(j = 0; j<astrFavorites.GetSize() && !found && (type==0x20 || type==0); j++)
					{
						if(astrFavorites[j].CompareNoCase(str)==0)
						{
							found = TRUE;
							//add
							str = astrFavorites[j];
							str = astrFavorites[j].Left(str.GetLength()-4);
							if(!bFolderOnly)
							{
								int nIconDefault = 1;

								CString strLinkFullPath = strPath2 + astrFavorites[j];
								nIconDefault = CGf_IE::FindFavLinkIcon( strLinkFullPath, m_wndFav.m_FavTreeCtrl.GetImageList(TVSIL_NORMAL), nIconDefault);
								hnode = m_wndFav.m_FavTreeCtrl.InsertItem(str, nIconDefault, nIconDefault, hItem,TVI_LAST);
							}

							m_wndFav.m_FavTreeCtrl.SetItemData(hnode, anFavID[j]);
							astrFavorites[j].Empty();
						}
					}
					for(j = 0; j<astrDirs.GetSize() && !found && (type==0x10 || type==0x14 || type==0); j++)
					{
						if(astrDirs[j].CompareNoCase(str)==0)
						{
							//add
							hnode = m_wndFav.m_FavTreeCtrl.InsertItem(astrDirs[j], 0, 2, hItem,hLastDir);
							m_wndFav.m_FavTreeCtrl.SetItemData(hnode, -1);
							if(strPath2+astrDirs[j]+"\\" == pmf->m_strStartFavFolder)
							{
								m_wndFav.m_FavTreeCtrl.m_hStartFolder = hnode;
								m_wndFav.m_FavTreeCtrl.SetItemState(hnode,TVIS_BOLD,TVIS_BOLD);
							}
							astrDirs[j].Empty();
						}
					}
				}
			}
		}

		//sort dirs and favs
		CString tmp;
		int n = astrDirs.GetSize();
		int i, ch, m;
		ch = n;
		while(ch>0)
		{
			m = ch-1; ch = 0;
			for(i=1; i<=m; i++)
			{
				if(lstrcmp(astrDirs[i-1], astrDirs[i])>0)
				{
					tmp = astrDirs[i-1];
					astrDirs[i-1] = astrDirs[i];
					astrDirs[i] = tmp;
					ch = i;
				}
			}
		}
		
		n = astrFavorites.GetSize();
		ch = n;
		int ntmp;
		while(ch>0)
		{
			m = ch-1; ch = 0;
			for(i=1; i<=m; i++)
			{
				if(lstrcmpiA(astrFavorites[i-1], astrFavorites[i])>0)
				{
					tmp = astrFavorites[i-1];
					astrFavorites[i-1] = astrFavorites[i];
					astrFavorites[i] = tmp;
					ntmp = anFavID[i-1];
					anFavID[i-1] = anFavID[i];
					anFavID[i] = ntmp;
					ch = i;
				}
			}
		}		
		
		for(i = 0; i<astrDirs.GetSize(); i++)
		{
			if(astrDirs[i].IsEmpty())
				continue;

			hnode = m_wndFav.m_FavTreeCtrl.InsertItem(astrDirs[i], 0, 2, hItem,hLastDir);
			m_wndFav.m_FavTreeCtrl.SetItemData(hnode, -1);
			if(strPath2+astrDirs[i]+"\\" == pmf->m_strStartFavFolder)
			{
				m_wndFav.m_FavTreeCtrl.m_hStartFolder = hnode;
				m_wndFav.m_FavTreeCtrl.SetItemState(hnode,TVIS_BOLD,TVIS_BOLD);
			}
		}
		
		for(i = 0; i<astrFavorites.GetSize(); i++)
		{
			if(astrFavorites[i].IsEmpty())
				continue;

			str = astrFavorites[i];
			str = astrFavorites[i].Left(str.GetLength()-4);
			if(!bFolderOnly)
			{
				int nIconDefault = 1;


				CString strLinkFullPath = strPath2 + astrFavorites[i];


				nIconDefault = CGf_IE::FindFavLinkIcon( strLinkFullPath, m_wndFav.m_FavTreeCtrl.GetImageList(TVSIL_NORMAL), nIconDefault);

				hnode = m_wndFav.m_FavTreeCtrl.InsertItem(str, nIconDefault, nIconDefault, hItem,TVI_LAST);
			}

			m_wndFav.m_FavTreeCtrl.SetItemData(hnode, anFavID[i]);
		}
	}
	return nEndPos;
}

//#pragma optimize( "s", off)

