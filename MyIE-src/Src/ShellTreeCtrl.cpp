// ShellTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "ShellTreeCtrl.h"
#include "MainFrm.h"
#include "setscdlg.h"
#include "inputalias.h"
#include "ShellTree.h"
#include "ShellTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CShellTreeCtrl

CShellTreeCtrl::CShellTreeCtrl()
{
	m_bDispFile = TRUE;
	m_strFilter = AfxGetApp()->GetProfileString("Settings", "FileTypes", "*.htm;*.html;*.shtm;*.shtml;*.mht;*.txt;");
	if(m_strFilter.Right(1) != ";")
		m_strFilter += ";";
}

CShellTreeCtrl::~CShellTreeCtrl()
{
	try{
	AfxGetApp()->WriteProfileString("Settings", "FileTypes", m_strFilter);
	}catch(...){}
}


BEGIN_MESSAGE_MAP(CShellTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CShellTreeCtrl)
	ON_NOTIFY_REFLECT(NM_RCLICK, GetContextMenu)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnFolderExpanding)
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnDeleteShellItem)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShellTreeCtrl message handlers


/****************************************************************************
*
*    FUNCTION: PopulateTree()
*
*    PURPOSE:  Processes the File.Fill/RefreshTree command
*
****************************************************************************/
void CShellTreeCtrl::PopulateTree() 
{
    LPSHELLFOLDER lpsf=NULL;
    LPITEMIDLIST  lpi=NULL;
    HRESULT hr;
    TV_SORTCB      tvscb;
   
    // Get a pointer to the desktop folder.
    hr=SHGetDesktopFolder(&lpsf);

    if (SUCCEEDED(hr))
    {
       // Initialize the tree view to be empty.
       DeleteAllItems();

       // Fill in the tree view from the root.
       FillTreeView(lpsf, NULL, TVI_ROOT);
       //TunnelFillTree(lpsf, NULL, TVI_ROOT);
       // Release the folder pointer.
       lpsf->Release();
    }
    tvscb.hParent     = TVI_ROOT;
    tvscb.lParam      = 0;
    tvscb.lpfnCompare = TreeViewCompareProc;

    // Sort the items in the tree view
	SortChildrenCB(&tvscb/*, FALSE*/);
    
	HTREEITEM hItem;
	hItem = GetRootItem();
	Expand(hItem,TVE_EXPAND);
	Select(GetRootItem(),TVGN_CARET);
}

/****************************************************************************
*
*  FUNCTION: FillTreeView( LPSHELLFOLDER lpsf,
*                          LPITEMIDLIST  lpifq,
*                          HTREEITEM     hParent)
*
*  PURPOSE: Fills a branch of the TreeView control.  Given the
*           shell folder, enumerate the subitems of this folder,
*           and add the appropriate items to the tree.
*
*  PARAMETERS:
*    lpsf         - Pointer to shell folder that we want to enumerate items 
*    lpifq        - Fully qualified item id list to the item that we are enumerating
*                   items for.  In other words, this is the PIDL to the item
*                   identified by the lpsf parameter.
*    hParent      - Parent node
*
*  COMMENTS:
*    This function enumerates the items in the folder identifed by lpsf.
*    Note that since we are filling the left hand pane, we will only add
*    items that are folders and/or have sub-folders.  We *could* put all
*    items in here if we wanted, but that's not the intent.
*
****************************************************************************/
void CShellTreeCtrl::FillTreeView(LPSHELLFOLDER lpsf, LPITEMIDLIST  lpifq, HTREEITEM     hParent)
{
    TV_ITEM         tvi;                          // TreeView Item.
    TV_INSERTSTRUCT tvins;                        // TreeView Insert Struct.
    HTREEITEM       hPrev = NULL;                 // Previous Item Added.
    LPSHELLFOLDER   lpsf2=NULL;
    LPENUMIDLIST    lpe=NULL;
    LPITEMIDLIST    lpi=NULL, lpiTemp=NULL, lpifqThisItem=NULL;
    LPTVITEMDATA    lptvid=NULL;
    LPMALLOC        lpMalloc=NULL;
    ULONG           ulFetched;
    UINT            uCount=0;
    HRESULT         hr;
    char            szBuff[256];
	char			szFileName[2048];
    HWND            hwnd=::GetParent(m_hWnd);

    // Allocate a shell memory object. 
    hr=::SHGetMalloc(&lpMalloc);
    if (FAILED(hr))
       return;

    if (SUCCEEDED(hr))
    {
        // Get the IEnumIDList object for the given folder.
        hr=lpsf->EnumObjects(hwnd, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS | SHCONTF_INCLUDEHIDDEN , &lpe);

        if (SUCCEEDED(hr))
        {
            // Enumerate throught the list of folder and non-folder objects.
            while (S_OK==lpe->Next(1, &lpi, &ulFetched))
            {
                //Create a fully qualified path to the current item
                //The SH* shell api's take a fully qualified path pidl,
                //(see GetIcon above where I call SHGetFileInfo) whereas the
                //interface methods take a relative path pidl.
                ULONG ulAttrs = SFGAO_HASSUBFOLDER | SFGAO_FOLDER;

                // Determine what type of object we have.
                lpsf->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lpi, &ulAttrs);

                if (m_bDispFile || (ulAttrs & (SFGAO_HASSUBFOLDER | SFGAO_FOLDER)))
                {
                   //We need this next if statement so that we don't add things like
                   //the MSN to our tree.  MSN is not a folder, but according to the
                   //shell it has subfolders.
                   if (m_bDispFile || (ulAttrs & SFGAO_FOLDER ))
                   {
                      tvi.mask= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

                      if (ulAttrs & (SFGAO_FOLDER|SFGAO_HASSUBFOLDER) )//
                      {
                         //This item has sub-folders, so let's put the + in the TreeView.
                         //The first time the user clicks on the item, we'll populate the
                         //sub-folders.
                         tvi.cChildren=1;
                         tvi.mask |= TVIF_CHILDREN;
                      }
                        
                      //OK, let's get some memory for our ITEMDATA struct
                      lptvid = (LPTVITEMDATA)lpMalloc->Alloc(sizeof(TVITEMDATA));
                      if (!lptvid)
                         goto Done;  // Error - could not allocate memory.
   
                      //Now get the friendly name that we'll put in the treeview.
                      if (!GetName(lpsf, lpi, SHGDN_NORMAL, szBuff))
                         goto Done; // Error - could not get friendly name.

					//  if (!GetName(lpsf, lpi, SHGDN_FORPARSING, szFileName))
					  if(SHGetPathFromIDList(lpi,szFileName))
					  {		
		 				  if(!(ulAttrs & SFGAO_FOLDER) && m_bDispFile)
							  if(!MatchExtensionFilter(szFileName))
								  continue;
					  }
					  else
		 				  if(!(ulAttrs & SFGAO_FOLDER) && m_bDispFile)
							  if(!MatchExtensionFilter(szBuff))
								  continue;

                      tvi.pszText    = szBuff;
                      tvi.cchTextMax = MAX_PATH;
    
                      lpifqThisItem=ConcatPidls(lpifq, lpi);
      
                      //Now, make a copy of the ITEMIDLIST
                      lptvid->lpi=CopyITEMID(lpMalloc, lpi);
   
                      GetNormalAndSelectedIcons(lpifqThisItem, &tvi);
   
                      lptvid->lpsfParent=lpsf;    //Store the parent folders SF
                      lpsf->AddRef();

                      lptvid->lpifq=ConcatPidls(lpifq, lpi);
   
                      tvi.lParam = (LPARAM)lptvid;
   
                      // Populate the TreeVeiw Insert Struct
                      // The item is the one filled above.
                      // Insert it after the last item inserted at this level.
                      // And indicate this is a root entry.
                      tvins.item         = tvi;
                      tvins.hInsertAfter = hPrev;
                      tvins.hParent      = hParent;
   
                      // Add the item to the tree
                      hPrev = InsertItem(&tvins);
                   }
                   // Free this items task allocator.
                   lpMalloc->Free(lpifqThisItem);  
                   lpifqThisItem=0;
                }
                lpMalloc->Free(lpi);  //Free the pidl that the shell gave us.
                lpi=0;
            }
        }
    }
    else
       return;

Done:
 
    if (lpe)  
        lpe->Release();

    //The following 2 if statements will only be TRUE if we got here on an
    //error condition from the "goto" statement.  Otherwise, we free this memory
    //at the end of the while loop above.
    if (lpi && lpMalloc)           
        lpMalloc->Free(lpi);
    if (lpifqThisItem && lpMalloc) 
        lpMalloc->Free(lpifqThisItem);  

    if (lpMalloc) 
        lpMalloc->Release();
}

/****************************************************************************
*
*    FUNCTION: GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem)
*
*    PURPOSE:  Gets the index for the normal and selected icons for the current item.
*
*    PARAMETERS:
*    lpifq    - Fully qualified item id list for current item.
*    lptvitem - Pointer to treeview item we are about to add to the tree.
*
****************************************************************************/
void CShellTreeCtrl::GetNormalAndSelectedIcons(LPITEMIDLIST lpifq,
                               LPTV_ITEM lptvitem)
{
   //Note that we don't check the return value here because if GetIcon()
   //fails, then we're in big trouble...

   lptvitem->iImage = GetItemIcon(lpifq, SHGFI_PIDL | 
                              SHGFI_SYSICONINDEX | 
                              SHGFI_SMALLICON);
   
   lptvitem->iSelectedImage = GetItemIcon(lpifq, SHGFI_PIDL | 
                                      SHGFI_SYSICONINDEX | 
                                      SHGFI_SMALLICON |
                                      SHGFI_OPENICON);
   return;
}

/****************************************************************************
*
*    FUNCTION: TreeViewCompareProc(LPARAM, LPARAM, LPARAM)
*
*    PURPOSE:  Callback routine for sorting the tree 
*
****************************************************************************/
int CALLBACK CShellTreeCtrl::TreeViewCompareProc(LPARAM lparam1, 
    LPARAM lparam2, LPARAM lparamSort)
{
    LPTVITEMDATA lptvid1=(LPTVITEMDATA)lparam1;
    LPTVITEMDATA lptvid2=(LPTVITEMDATA)lparam2;
    HRESULT   hr;

    hr = lptvid1->lpsfParent->CompareIDs(0,lptvid1->lpi,lptvid2->lpi);

    if (FAILED(hr))
       return 0;

    return (short)SCODE_CODE(GetScode(hr));
}

/////////////////////////////////////////////////////////////////////////////
// CShellTreeCtrl message handlers

/****************************************************************************
*
*	FUNCTION:	OnFolderExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
*
*	PURPOSE:	Reponds to an TVN_ITEMEXPANDING message in order to fill up
*				subdirectories. Pass the parameters from OnItemExpanding() to 
*				this function. You need to do that or your folders won't
*				expand.
*
*	OTHER:		It can also be used to update a corresponding listview. Seem MFCENUM
*
*	MESSAGEMAP:	TVN_ITEMEXPANDING
*
****************************************************************************/
void CShellTreeCtrl::OnFolderExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPTVITEMDATA   lptvid;  //Long pointer to TreeView item data
	HRESULT        hr;
	LPSHELLFOLDER  lpsf2=NULL;
	static char    szBuff[MAX_PATH];
	TV_SORTCB      tvscb;

	NM_TREEVIEW* pnmtv = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
    if ((pnmtv->itemNew.state & TVIS_EXPANDEDONCE))
	{
         return;
	}

    lptvid=(LPTVITEMDATA)pnmtv->itemNew.lParam;
    if (lptvid)
       {
            hr=lptvid->lpsfParent->BindToObject(lptvid->lpi,
                0, IID_IShellFolder,(LPVOID *)&lpsf2);

            if (SUCCEEDED(hr))
            {
                FillTreeView(lpsf2,
                       lptvid->lpifq,
                       pnmtv->itemNew.hItem);
            }

            tvscb.hParent     = pnmtv->itemNew.hItem;
            tvscb.lParam      = 0;
            tvscb.lpfnCompare = TreeViewCompareProc;

            SortChildrenCB(&tvscb /*, FALSE*/);
    }	

	*pResult = 0;
}

/****************************************************************************
*
*	FUNCTION:	GetContextMenu(NMHDR* pNMHDR, LRESULT* pResult) 
*
*	PURPOSE:	Diplays a popup menu for the folder selected. Pass the
*				parameters from Rclick() to this function.
*
*	MESSAGEMAP:	NM_RCLICK;
*
****************************************************************************/
void CShellTreeCtrl::GetContextMenu(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POINT			pt;
	LPTVITEMDATA	lptvid;  //Long pointer to TreeView item data
	LPSHELLFOLDER	lpsf2=NULL;
	static char		szBuff[MAX_PATH];
	TV_HITTESTINFO	tvhti;
	TV_ITEM			tvi;

	// TODO: Add your control notification handler code here
	::GetCursorPos((LPPOINT)&pt);
	ScreenToClient(&pt);
	tvhti.pt=pt;
    HitTest(&tvhti);
    SelectItem(tvhti.hItem);
	if (tvhti.flags & (TVHT_ONITEMLABEL|TVHT_ONITEMICON))
	{
		ClientToScreen(&pt);
		tvi.mask=TVIF_PARAM;
		tvi.hItem=tvhti.hItem;
		
        if (!GetItem(&tvi)){
			return;
		}
		
		lptvid=(LPTVITEMDATA)tvi.lParam;
		
		DoTheMenuThing(::GetParent(m_hWnd),
		lptvid->lpsfParent, lptvid->lpi, &pt);
	}	
	
	*pResult = 0;
}

/****************************************************************************
*
*	FUNCTION:	OnFolderSelected(NMHDR* pNMHDR, LRESULT* pResult, CString &szFolderPath) 
*
*	PURPOSE:	Call this function if for example you want to put the path of the folder
*				selected inside a combobox or an edit window. You would pass the
*				parameters from OnSelChanged() to this function along with a CString object
*				that will hold the folder path. If the path is not
*				in the filesystem(eg MyComputer) it returns false.
*
*	MESSAGEMAP:	TVN_SELCHANGED
*
****************************************************************************/
BOOL CShellTreeCtrl::OnFolderSelected(NMHDR* pNMHDR, LRESULT* pResult, CString &szFolderPath) 
{
	// TODO: Add your control notification handler code here
	LPTVITEMDATA	lptvid;  //Long pointer to TreeView item data
	LPSHELLFOLDER	lpsf2=NULL;
	static char		szBuff[MAX_PATH];
	HRESULT			hr;
	BOOL			bRet=false;
	TV_SORTCB		tvscb;
	HTREEITEM		hItem=NULL;

	if((hItem = GetSelectedItem()))
	{
		lptvid=(LPTVITEMDATA)GetItemData(hItem);

		if (lptvid && lptvid->lpsfParent && lptvid->lpi)
		{
			hr=lptvid->lpsfParent->BindToObject(lptvid->lpi,
					 0,IID_IShellFolder,(LPVOID *)&lpsf2);

			if (SUCCEEDED(hr))
			{
				ULONG ulAttrs = SFGAO_FILESYSTEM;

				// Determine what type of object we have.
				lptvid->lpsfParent->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lptvid->lpi, &ulAttrs);

				if (ulAttrs & (SFGAO_FILESYSTEM))
				{
					if(SHGetPathFromIDList(lptvid->lpifq,szBuff)){
						szFolderPath = szBuff;
						bRet = true;
					}
				}
				//non standard from here(NEW CODE)
				NM_TREEVIEW* pnmtv = (NM_TREEVIEW*)pNMHDR;
				if ((pnmtv->itemNew.cChildren == 1) && !(pnmtv->itemNew.state & TVIS_EXPANDEDONCE)){
					FillTreeView(lpsf2,lptvid->lpifq,pnmtv->itemNew.hItem);

					tvscb.hParent     = pnmtv->itemNew.hItem;
					tvscb.lParam      = 0;
					tvscb.lpfnCompare = TreeViewCompareProc;
					SortChildrenCB(&tvscb);
					
					pnmtv->itemNew.state |= TVIS_EXPANDEDONCE;
					pnmtv->itemNew.stateMask |= TVIS_EXPANDEDONCE;
					pnmtv->itemNew.mask |= TVIF_STATE;
					SetItem(&pnmtv->itemNew);
				}
			}
		}
		if(lpsf2)
			lpsf2->Release();
		
	}	
	*pResult = 0;
	return bRet;
}

/****************************************************************************
*
*	FUNCTION:	OnDeleteShellItem(NMHDR* pNMHDR, LRESULT* pResult)
*
*	PURPOSE:	Releases the memory allocated by the shell folders
*
*	MESSAGEMAP:	TVN_DELETEITEM
*
*	MISC:		failure to call this function will result in a memory leak
*
****************************************************************************/
void CShellTreeCtrl::OnDeleteShellItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPTVITEMDATA lptvid=NULL;
	HRESULT hr;
	LPMALLOC lpMalloc;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	//Let's free the memory for the TreeView item data...
	hr=SHGetMalloc(&lpMalloc);
	if (FAILED(hr))
		return;
		
	lptvid=(LPTVITEMDATA)pNMTreeView->itemOld.lParam;
	lptvid->lpsfParent->Release();
	lpMalloc->Free(lptvid->lpi);  
	lpMalloc->Free(lptvid->lpifq);  
	lpMalloc->Free(lptvid);  
	lpMalloc->Release();
}

/****************************************************************************
*
*	FUNCTION:	EnableImages()
*
*	PURPOSE:	Obtains a handle to the system image list and attaches it
*				to the tree control. DO NOT DELETE the imagelist
*
*	MESSAGEMAP:	NONE
*
****************************************************************************/
void CShellTreeCtrl::EnableImages()
{
	// Get the handle to the system image list, for our icons
    HIMAGELIST  hImageList;
    SHFILEINFO    sfi;

    hImageList = (HIMAGELIST)SHGetFileInfo((LPCSTR)"C:\\", 
                                           0,
                                           &sfi, 
                                           sizeof(SHFILEINFO), 
                                           SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
    // Attach ImageList to TreeView
    if (hImageList)
        ::SendMessage(m_hWnd, TVM_SETIMAGELIST, (WPARAM) TVSIL_NORMAL,
            (LPARAM)hImageList);
}

/****************************************************************************
*
*	FUNCTION:	SearchTree(	HTREEITEM treeNode,
*							CString szSearchName )
*
*	PURPOSE:	Too crude to explain, just use it
*
*	WARNING:	Only works if you use the default PopulateTree()
*				Not guaranteed to work on any future or existing
*				version of windows. Use with caution. Pretty much
*				ok if you're using on local drives
*
****************************************************************************/
bool CShellTreeCtrl::SearchTree(HTREEITEM treeNode,
							CString szSearchName,
							FindAttribs attr)
{
	LPTVITEMDATA	lptvid;  //Long pointer to TreeView item data
	LPSHELLFOLDER	lpsf2=NULL;
	char	drive[_MAX_DRIVE];
	char	dir[_MAX_DIR];
	char	fname[_MAX_FNAME];
	char	ext[_MAX_EXT];
	bool	bRet=false;
	HRESULT	hr;
	CString	szCompare;

	szSearchName.MakeUpper();
	while(treeNode && bRet==false)
	{
		lptvid=(LPTVITEMDATA)GetItemData(treeNode);
		if (lptvid && lptvid->lpsfParent && lptvid->lpi)
		{
			hr=lptvid->lpsfParent->BindToObject(lptvid->lpi,
					 0,IID_IShellFolder,(LPVOID *)&lpsf2);
			if (SUCCEEDED(hr))
			{
				ULONG ulAttrs = SFGAO_FILESYSTEM;
				lptvid->lpsfParent->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lptvid->lpi, &ulAttrs);
				if (ulAttrs & (SFGAO_FILESYSTEM))
				{
					if(SHGetPathFromIDList(lptvid->lpifq,szCompare.GetBuffer(MAX_PATH)))
					{
						switch(attr)
						{
						case type_drive:
							_splitpath(szCompare,drive,dir,fname,ext);
							szCompare=drive;
							break;
						case type_folder:
							szCompare = GetItemText(treeNode);
							break;
						}
						szCompare.MakeUpper();
						if(szCompare == szSearchName)
						{
							EnsureVisible(treeNode);
							SelectItem(treeNode);
							Expand(treeNode, TVE_EXPAND);
							bRet=true;
						}
					}
				}
				lpsf2->Release();
			}
		}
		treeNode = GetNextSiblingItem(treeNode);
	}
	return bRet;
}

/****************************************************************************
*
*	FUNCTION:	TunnelTree(CString szFindPath)
*
*	PURPOSE:	Too crude to explain, just use it
*
*	WARNING:	Only works if you use the default PopulateTree()
*				Not guaranteed to work on any future or existing
*				version of windows. Use with caution. Pretty much
*				ok if you're using on local drives
*
****************************************************************************/
void CShellTreeCtrl::TunnelTree(CString szFindPath)
{
	HTREEITEM subNode = GetRootItem();
	CString szPathHop;
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	char delimiter[]="\\";
	LPSHELLFOLDER  lpsf2=NULL;
	static char    szBuff[MAX_PATH];

	if ( !_FileIsExist(szFindPath) )
	{
		MessageBox(szFindPath,"Folder not found",MB_ICONERROR);
		return;
	}
		
	if(szFindPath.ReverseFind('\\') != szFindPath.GetLength()-1)
	{
		szFindPath += "\\";
	}

	_splitpath(szFindPath,drive,dir,fname,ext);

	//search the drive first
	bool found = false;
	szPathHop=drive;
	subNode=GetChildItem(subNode);
	if(subNode)
	{
		found = SearchTree(subNode,szPathHop, CShellTreeCtrl::type_drive);
		if(!found)
		{
			subNode = GetRootItem();
			subNode = GetNextSiblingItem(subNode);
			Expand(subNode, TVE_EXPAND);
			subNode = GetChildItem(subNode);
			if(subNode)
				found = SearchTree(subNode,szPathHop, CShellTreeCtrl::type_drive);
		}
		if(found)
		{
			//break down subfolders and search
			char *p=strtok(dir,delimiter);
			while(p)
			{
				subNode = GetSelectedItem();
				//expand it
				Expand(subNode, TVE_EXPAND);
				subNode = GetChildItem(subNode);
				if(SearchTree(subNode,p,CShellTreeCtrl::type_folder))
					p=strtok(NULL,delimiter);
				else
					p=NULL;
			}
		}
	}
}

BOOL CShellTreeCtrl::MatchExtensionFilter(LPSTR lpFileName)
{
	if(lpFileName == NULL)
		return FALSE;

	if(m_strFilter == "" || m_strFilter.Find("*.*")>=0)
		return TRUE;

	CString str(lpFileName);
	str.MakeLower();
	int i = str.ReverseFind('.');
	if(i<0)
		return FALSE;
	str = str.Mid(i);
	str += ";";
	if(m_strFilter.Find(str)>=0)
		return TRUE;
	else
		return FALSE;
}

void CShellTreeCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPTVITEMDATA	lptvid;
	char		szBuff[MAX_PATH];
	HTREEITEM		hItem=NULL;

	CPoint pt;
	UINT uFlags;

	GetCursorPos(&pt);
	ScreenToClient(&pt);
	hItem = HitTest(pt, &uFlags);
	if(hItem == NULL || !(TVHT_ONITEM & uFlags))
		return;

	lptvid=(LPTVITEMDATA)GetItemData(hItem);
	if(SHGetPathFromIDList(lptvid->lpifq,szBuff))
	{
		OFSTRUCT ofs;
		if(OpenFile(szBuff, &ofs, OF_EXIST) != HFILE_ERROR)
		{
			//is exe?
			CString s = szBuff;
			if(s.Right(4)==".exe" || s.Right(4)==".EXE")
				ShellExecute(NULL, "open", szBuff , "", NULL, SW_SHOW );
			else
			{
				CMyIE9View* pvw = GetCurrentView(1, g_bFileInNewWindow);
				if(pvw!=NULL)
				{
					pvw->ToNavigate(szBuff,0,NULL);
				}
			}
		}
		else
			Expand(hItem, TVE_TOGGLE);
	}
}

BOOL CShellTreeCtrl::GetCurFilePath(CString & str)
{
	LPTVITEMDATA	lptvid;
	char		szBuff[MAX_PATH];
	HTREEITEM		hItem=NULL;

	hItem = GetSelectedItem();
	lptvid=(LPTVITEMDATA)GetItemData(hItem);
	if(SHGetPathFromIDList(lptvid->lpifq,szBuff))
	{
		str = szBuff;
		return TRUE;
	}
	else
		return FALSE;
}

//#pragma optimize( "s", off )

