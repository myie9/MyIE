// OrgGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "MainFrm.h"
#include "OrgGroupDlg.h"
#include "GroupUrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	INTERNET_MAX_PATH_LENGTH	2048
//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// COrgGroupDlg dialog


COrgGroupDlg::COrgGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COrgGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COrgGroupDlg)
	//}}AFX_DATA_INIT
	m_pDragImage = NULL;

}


void COrgGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrgGroupDlg)
	DDX_Control(pDX, IDC_CLOSE_ALL_BEFORE_NEW_GROUP, m_CloseAllBeforeNewGroup);
	DDX_Control(pDX, IDC_SHOW_GROUP_MEMBER, m_GroupShowMember);
	DDX_Control(pDX, IDC_INSERT, m_btnInsert);
	DDX_Control(pDX, IDC_URL_LIST, m_UrlList);
	DDX_Control(pDX, IDC_GROUP_LIST, m_GroupList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrgGroupDlg, CDialog)
	//{{AFX_MSG_MAP(COrgGroupDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_GROUP_LIST, OnSelChanging)
	ON_BN_CLICKED(IDC_NEW_GROUP, OnNewGroup)
	ON_BN_CLICKED(IDC_DEL_GROUP, OnDelGroup)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_GROUP_LIST, OnEndlabeleditGroupList)
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	ON_NOTIFY(NM_CLICK, IDC_GROUP_LIST, OnClickGroupList)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_URL_LIST, OnBegindragUrlList)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(NM_DBLCLK, IDC_URL_LIST, OnDblclkUrlList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrgGroupDlg message handlers

BOOL COrgGroupDlg::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogGroup");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogGroup");
	CDialog::OnInitDialog();

	m_bInit = TRUE;
	// TODO: Add extra initialization here
	m_GroupList.SetExtendedStyle(m_GroupList.GetExtendedStyle()|LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
	m_UrlList.SetExtendedStyle(m_UrlList.GetExtendedStyle() | LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);
	//setup list ctrl
	CRect rect;
	m_GroupList.InsertColumn(0,"");
	m_GroupList.GetWindowRect(rect);
	m_GroupList.SetColumnWidth(0, rect.Width()-25);

	CString str;
	LOADSTR(str ,IDS_NAME);
	m_UrlList.InsertColumn(0,str);
	m_UrlList.GetWindowRect(rect);
	m_UrlList.SetColumnWidth(0, 100);
	LOADSTR(str ,IDS_URL);
	m_UrlList.InsertColumn(1,str);
	m_UrlList.SetColumnWidth(1, rect.Width()-125);

	//load group list
	m_strStartGroup = pmf->m_strStartGroup;

	int i=0;
	CString strMenu;
	for (i=0; i<=pmf->m_astrGroup.GetUpperBound(); i++)
	{
		strMenu = pmf->m_astrGroup.GetAt(i);
		strMenu = strMenu.Left(strMenu.GetLength()-4);
		m_GroupList.InsertItem(i, strMenu);
		if(strMenu == m_strStartGroup)
		{
			m_GroupList.SetCheck(i);
		}
	}

	m_GroupList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	
	m_nLastSelItemID = -1;
	FillUrlList(0);
	if(i==0)
		m_btnInsert.EnableWindow(FALSE);

	m_bInit = FALSE;

	m_GroupShowMember.SetCheck( pmf->m_bGroupMenuShowMember );
	m_CloseAllBeforeNewGroup.SetCheck( pmf->m_bCloseAllBeforeNewGroup);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COrgGroupDlg::FillUrlList(int index)
{
	try{
	if(m_nLastSelItemID == index)
		return;
	if(m_nLastSelItemID>=0)
		SaveUrlList(m_nLastSelItemID);

	m_nLastSelItemID = index;
	//remove all first
	m_UrlList.DeleteAllItems();
	m_UrlList.SetItemCount(0);

	if(index<0)
		return;
	CString filename;
	filename = m_GroupList.GetItemText(index, 0);
	filename = theApp.m_strGroupPath + filename+".cgp";

	char state[10]="state",name[9]="name",url[8]="url",download[13]="download"; // x+5
	int i=0,nState;
	char tmp[INTERNET_MAX_PATH_LENGTH];
	BOOL r = TRUE;
	DWORD dwProperty;
	while(r)
	{
		itoa(i, state+5, 10);
		itoa(i, name+4, 10);
		itoa(i, url+3, 10);
		itoa(i, download+8, 10);
		nState = ::GetPrivateProfileInt("Group", state, 1, filename);
		r = ::GetPrivateProfileString("Group", name, NULL, tmp, INTERNET_MAX_PATH_LENGTH, filename);
		if (r)
		{
			m_UrlList.InsertItem(i, tmp);
			m_UrlList.SetCheck(i, nState);
			r = ::GetPrivateProfileString("Group", url, NULL, tmp, INTERNET_MAX_PATH_LENGTH, filename);
			if (r)
				m_UrlList.SetItemText(i,1,tmp);
			dwProperty = ::GetPrivateProfileInt("Group", download, DEFAULT_PROPERTY, filename);
			m_UrlList.SetItemData(i,dwProperty);
		}
		i++;
	}
	}catch(...){}
}

void COrgGroupDlg::SaveUrlList(int index)
{
	try{
	if(index<0)
		return;

	CString filename;
	filename = m_GroupList.GetItemText(index, 0);
	if(filename.IsEmpty())
		return;

	char state[10]="state",name[9]="name",url[8]="url",download[13]="download"; // x+5
	char num[15];
	filename = theApp.m_strGroupPath + filename+".cgp";
	int n = m_UrlList.GetItemCount();
	WritePrivateProfileSection("Group", NULL, filename);
	for( int i=0; i<n; i++)
	{
		itoa(i, state+5, 10);
		itoa(i, name+4, 10);
		itoa(i, url+3, 10);
		itoa(i, download+8, 10);
		if (m_UrlList.GetCheck(i)==FALSE)
		{
			itoa( 0, num, 10);
			::WritePrivateProfileString("Group", state, num, filename);
		}
		::WritePrivateProfileString("Group", name, m_UrlList.GetItemText(i,0), filename);
		::WritePrivateProfileString("Group", url, m_UrlList.GetItemText(i,1), filename);
		if (m_UrlList.GetItemData(i) != DEFAULT_PROPERTY)
		{
			ultoa( m_UrlList.GetItemData(i), num, 10);
			::WritePrivateProfileString("Group", download, num, filename);
		}
	}
	}catch(...){}
}

void COrgGroupDlg::OnSelChanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_bInit)
		return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int i = m_GroupList.GetSelectedCount();
	if(i>0)
	{
		POSITION pos = m_GroupList.GetFirstSelectedItemPosition();
		int item = m_GroupList.GetNextSelectedItem(pos);
		FillUrlList(item);
		m_btnInsert.EnableWindow(1);
	}
	else
	{
		FillUrlList(-1);
		m_btnInsert.EnableWindow(0);
	}

	*pResult = 0;
}

void COrgGroupDlg::OnNewGroup() 
{
	// TODO: Add your control notification handler code here
	CString def, str;
	LOADSTR(def ,IDS_NEW_GROUP);
	str = def;
	
	char si[5]; int i=0;
	LVFINDINFO info;
	info.flags = LVFI_STRING;
	info.psz = str;
	
	while (m_GroupList.FindItem(&info) != -1)
	{
		i++;
		itoa(i, si, 10);
		str = def + " (";
		str += si;
		str += ")";
		info.psz = str;
	}
	
	i = m_GroupList.GetItemCount();
	m_GroupList.InsertItem(i, str);
	m_GroupList.SetFocus();
	m_GroupList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);

	CString filename;
	filename = theApp.m_strGroupPath + str+".cgp";

	HANDLE hfile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	CloseHandle(hfile);

	m_GroupList.EnsureVisible(i,FALSE);
	m_GroupList.EditLabel(i);
}

void COrgGroupDlg::OnDelGroup() 
{
	// TODO: Add your control notification handler code here
	int i = m_GroupList.GetSelectedCount();
	if(i>0)
	{
		POSITION pos = m_GroupList.GetFirstSelectedItemPosition();
		int item = m_GroupList.GetNextSelectedItem(pos);
		if(MSGBOX(IDS_CONFIRM_DEL, MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			BOOL rem = FALSE;
			CString filename;
			filename = m_GroupList.GetItemText(item, 0);
			if(filename == m_strStartGroup)
				rem = TRUE;
			filename = theApp.m_strGroupPath + filename+".cgp";
			if(DeleteFile(filename))
			{
				m_nLastSelItemID = -1;
				m_GroupList.DeleteItem(item);
				m_UrlList.DeleteAllItems();
				if(rem)
					m_strStartGroup = "";
			}
			//
			LIST_FOCUS_POS(m_GroupList, item);
		}
	}
}

void COrgGroupDlg::OnEndlabeleditGroupList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString str;
	m_GroupList.GetEditControl()->GetWindowText(str);
	str.TrimLeft();
	str.TrimRight();

	//rename file
	CString filename;
	BOOL ren = FALSE;
	filename = m_GroupList.GetItemText(pDispInfo->item.iItem, 0);
	if(filename == m_strStartGroup)
		ren = TRUE;
	filename = theApp.m_strGroupPath + filename+".cgp";
	if(MoveFile(filename, theApp.m_strGroupPath + str + ".cgp"))
	{
		m_GroupList.SetItemText(pDispInfo->item.iItem,0,str);
		if(ren)
			m_strStartGroup = str;
	}
	
	*pResult = 0;
}

void COrgGroupDlg::OnOK() 
{
	// TODO: Add extra validation here
	int i = m_GroupList.GetSelectedCount();
	if(i>0)
	{
		POSITION pos = m_GroupList.GetFirstSelectedItemPosition();
		int item = m_GroupList.GetNextSelectedItem(pos);
		SaveUrlList(item);
	}
	pmf->m_strStartGroup = m_strStartGroup;
	pmf->m_bGroupMenuShowMember = m_GroupShowMember.GetCheck();
	pmf->m_bCloseAllBeforeNewGroup = m_CloseAllBeforeNewGroup.GetCheck();

	//get order
	CString strItem;
	CString strOrder;
	for (i=0;i<m_GroupList.GetItemCount();i++)
	{
		strItem = m_GroupList.GetItemText(i,0);
		strOrder += strItem;
		strOrder += ".cgp\r\n";
	}
	//write order
	CFile f;
	if(f.Open(theApp.m_strGroupPath+"order.txt", CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
	{
		f.Write((void*)(LPCSTR)strOrder, strOrder.GetLength());
		f.Close();
	}
	//
	CDialog::OnOK();
}

void COrgGroupDlg::OnDelete() 
{
	// TODO: Add your control notification handler code here
	int i = m_UrlList.GetSelectedCount();
	if(i>0)
	{
		POSITION pos = m_UrlList.GetFirstSelectedItemPosition();
		int item = m_UrlList.GetNextSelectedItem(pos);
		m_UrlList.DeleteItem(item);
		//
		LIST_FOCUS_POS(m_UrlList,item);
	}
}

void COrgGroupDlg::OnInsert() 
{
	// TODO: Add your control notification handler code here
	if(m_nLastSelItemID<0)
		return;

	CGroupUrlDlg dlg;
	dlg.m_bNotEmpty = FALSE;
	if(dlg.DoModal() == IDOK)
	{
		CString str = dlg.m_strName;
		str.TrimLeft(); str.TrimRight();
		if(str.IsEmpty())
			GetUnqBlankName(str);
		dlg.m_strUrl.TrimLeft();

		int item; LIST_GET_INSERT_POS(m_UrlList,&item);
		m_UrlList.InsertItem(item, str);
		m_UrlList.SetItemText(item, 1, dlg.m_strUrl);
		m_UrlList.SetCheck(item,dlg.m_bState);
		m_UrlList.SetItemData(item, dlg.m_dwProperty);
		LIST_FOCUS_POS (m_UrlList, item);
	}
}

void COrgGroupDlg::OnUpdate() 
{
	// TODO: Add your control notification handler code here
	int item;
	int i = m_UrlList.GetSelectedCount();
	if(i>0)
	{
		POSITION pos = m_UrlList.GetFirstSelectedItemPosition();
		item = m_UrlList.GetNextSelectedItem(pos);
	}
	else
		return;

	CGroupUrlDlg dlg;
	dlg.m_strName = m_UrlList.GetItemText(item, 0);
	dlg.m_strUrl = m_UrlList.GetItemText(item,1);
	dlg.m_bState = m_UrlList.GetCheck(item);
	dlg.m_dwProperty = m_UrlList.GetItemData(item);
	if(dlg.DoModal() == IDOK)
	{
		CString str = dlg.m_strName;
		str.TrimLeft();
		str.TrimRight();
		if(str.IsEmpty())
			GetUnqBlankName(str);
		m_UrlList.SetItemText(item, 0, str);
		dlg.m_strUrl.TrimLeft();
		m_UrlList.SetItemText(item, 1, dlg.m_strUrl);
		m_UrlList.SetCheck(item, dlg.m_bState);
		m_UrlList.SetItemData(item,dlg.m_dwProperty);
	}	
}

void COrgGroupDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	POSITION pos = m_GroupList.GetFirstSelectedItemPosition();
	int nItem = -1, newItem;
	if(pos!=NULL)
	{
		nItem = m_GroupList.GetNextSelectedItem(pos);
		if(pNMUpDown->iDelta<0)
			newItem = nItem -1;
		else
			newItem = nItem + 1;
		int n = m_GroupList.GetItemCount();
		if(newItem>=0 && newItem<n)
		{
			CString str = m_GroupList.GetItemText(nItem,0);
			BOOL	bState = m_GroupList.GetCheck(nItem);
			m_GroupList.DeleteItem(nItem);

			m_GroupList.InsertItem(newItem, str);
			m_GroupList.SetItemState(newItem, LVIS_SELECTED, LVIS_SELECTED);
			m_GroupList.SetCheck(newItem, bState);
			m_GroupList.EnsureVisible(newItem, FALSE);
		}
	}
	
	*pResult = 0;
}

void COrgGroupDlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	POSITION pos = m_UrlList.GetFirstSelectedItemPosition();
	int nItem = -1, newItem;
	if(pos!=NULL)
	{
		nItem = m_UrlList.GetNextSelectedItem(pos);
		if(pNMUpDown->iDelta<0)
			newItem = nItem -1;
		else
			newItem = nItem + 1;
		int n = m_UrlList.GetItemCount();
		if(newItem>=0 && newItem<n)
		{
			CString str = m_UrlList.GetItemText(nItem,0);
			CString str2 = m_UrlList.GetItemText(nItem, 1);
			BOOL	bState = m_UrlList.GetCheck(nItem);
			DWORD	dwProperty= m_UrlList.GetItemData(nItem);
			m_UrlList.DeleteItem(nItem);

			m_UrlList.InsertItem(newItem, str);
			m_UrlList.SetItemText(newItem, 1, str2);
			m_UrlList.SetItemState(newItem, LVIS_SELECTED, LVIS_SELECTED);
			m_UrlList.SetCheck(newItem, bState);
			m_UrlList.SetItemData(newItem, dwProperty);
			m_UrlList.EnsureVisible(newItem, FALSE);

		}
	}
	
	*pResult = 0;
}

void COrgGroupDlg::GetUnqBlankName(CString &newblnk)
{
	CString def = "blank";
	newblnk = def;
	
	char si[5]; int i=0;
	LVFINDINFO info;

	info.flags = LVFI_STRING;
	
	do{
		i++;
		itoa(i, si, 10);
		newblnk = def;
		newblnk += si;
		info.psz = newblnk;
	}while(m_UrlList.FindItem(&info) != -1);

}

void COrgGroupDlg::OnClickGroupList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint pt;
	UINT flag;
	GetCursorPos(&pt);
	m_GroupList.ScreenToClient(&pt);
	int iItem = m_GroupList.HitTest(pt, &flag);
	if(flag != LVHT_ONITEMSTATEICON  || iItem<0)
		return;

	BOOL bcheck = m_GroupList.GetCheck(iItem);

	LVFINDINFO info;
	info.flags = LVFI_STRING;
	info.psz = m_strStartGroup;
	int item = m_GroupList.FindItem(&info);

	if(!bcheck)
	{
		m_GroupList.SetCheck(item, 0);
		//m_GroupList.SetCheck(iItem);
		m_strStartGroup = m_GroupList.GetItemText(iItem, 0);
	}
	else if(iItem == item)
	{
		m_strStartGroup = "";
	}
	
	*pResult = 0;
}

void COrgGroupDlg::OnBegindragUrlList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_nDragIndex = pNMListView->iItem;
	POINT pt;
	pt.x = 8;
	pt.y = 8;
	// create a drag image

	if(m_pDragImage)
		delete m_pDragImage;
	
	m_pDragImage = m_UrlList.CreateDragImage (m_nDragIndex, &pt);
	ASSERT (m_pDragImage);
	// changes the cursor to the drag image (DragMove() is still required in 
	// OnMouseMove())
	VERIFY (m_pDragImage->BeginDrag (0, CPoint (8, 8)));
	VERIFY (m_pDragImage->DragEnter (GetDesktopWindow (), pNMListView->ptAction));
	// set dragging flag
	m_bDragging = TRUE;
	m_nDropIndex = -1;
	m_pDropWnd = &m_GroupList;
	// capture all mouse messages
	SetCapture ();
	
	*pResult = 0;
}

void COrgGroupDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDragging)
	{
		// release mouse capture
		VERIFY (::ReleaseCapture ());
		m_bDragging = FALSE;
		// end dragging
		VERIFY (m_pDragImage->DragLeave (GetDesktopWindow ()));
		m_pDragImage->EndDrag ();

		// turn off hilight for previous drop target
		m_GroupList.SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);
		// redraw previous item
		m_GroupList.RedrawItems (m_nDropIndex, m_nDropIndex);

		CPoint pt (point);
		ClientToScreen (&pt);
		// get the CWnd pointer of the window that is under the mouse cursor
		// if window is CTreeCtrl
		CWnd* pDropWnd = WindowFromPoint (pt);
		if (pDropWnd == m_pDropWnd && m_nDropIndex>=0 && m_nDropIndex!=m_nLastSelItemID)
		{
			CString name = m_UrlList.GetItemText(m_nDragIndex,0);
			CString url = m_UrlList.GetItemText(m_nDragIndex,1);
			BOOL	bState = m_UrlList.GetCheck(m_nDragIndex);
			DWORD	dwProperty= m_UrlList.GetItemData(m_nDragIndex);

			//add to new list
			CString filename;
			filename = m_GroupList.GetItemText(m_nDropIndex, 0);
			filename = theApp.m_strGroupPath + filename+".cgp";

			//::WritePrivateProfileString("CaptorGroup", name, url, filename);
			_GroupAddItem(name,url,bState,dwProperty,filename);

			if(!PRESS_CTRL)
				OnDelete();
		}
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

void COrgGroupDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDragging)
	{
		CPoint pt (point);
		ClientToScreen (&pt);
		// move the drag image
		VERIFY (m_pDragImage->DragMove (pt));
		// unlock window updates
		VERIFY (m_pDragImage->DragShowNolock (FALSE));

		// get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT (pDropWnd);

		// convert from screen coordinates to drop target client coordinates
		pDropWnd->ScreenToClient (&pt);

		// if window is CTreeCtrl
		if (pDropWnd == m_pDropWnd)
		{			
			UINT uFlags;
			CListCtrl* pList = (CListCtrl*)pDropWnd;

			// turn off hilight for previous drop target
			pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);
			// redraw previous item
			pList->RedrawItems (m_nDropIndex, m_nDropIndex);
			
			// get the item that is below cursor
			m_nDropIndex = ((CListCtrl*)pDropWnd)->HitTest (pt, &uFlags);
			// highlight it
			if(m_nDropIndex>=0)
			{
				pList->SetItemState (m_nDropIndex, LVIS_DROPHILITED, LVIS_DROPHILITED);
				// redraw item
				pList->RedrawItems (m_nDropIndex, m_nDropIndex);
				pList->UpdateWindow ();
			}
		}
		// lock window updates
		VERIFY (m_pDragImage->DragShowNolock (TRUE));
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void COrgGroupDlg::OnDblclkUrlList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int item;
	int i = m_UrlList.GetSelectedCount();
	if(i>0)
	{
		POSITION pos = m_UrlList.GetFirstSelectedItemPosition();
		item = m_UrlList.GetNextSelectedItem(pos);
	}
	else
		return;

	pmf->NewChildWindow(1,2, m_UrlList.GetItemText(item,1)); //OnUpdate(); jump edit

	*pResult = 0;
}

//#pragma optimize( "s", off)

