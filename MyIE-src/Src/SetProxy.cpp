// SetProxy.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "SetProxy.h"
#include "MainFrm.h"
#include "ProxyInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CSetProxy dialog

//0. invalid, 1. slower, 2. slow, 3. normal, 4. fast, 5. faster

CSetProxy::CSetProxy(CWnd* pParent /*=NULL*/)
	: CDialog(CSetProxy::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetProxy)
	m_bUseWebProxy = FALSE;
	m_bUseProxy = FALSE;
	//}}AFX_DATA_INIT

	m_pDragImage = NULL;
}


void CSetProxy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetProxy)
	DDX_Control(pDX, IDC_BYPASS, m_ByPass);
	DDX_Control(pDX, IDC_CATES, m_CateList);
	DDX_Control(pDX, IDC_PROXY_LIST, m_ProxyList);
	DDX_Check(pDX, IDC_USE_WEB_PROXY, m_bUseWebProxy);
	DDX_Check(pDX, IDC_USER_PROXY, m_bUseProxy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetProxy, CDialog)
	//{{AFX_MSG_MAP(CSetProxy)
	ON_BN_CLICKED(IDC_PROXY_NEW, OnProxyNew)
	ON_BN_CLICKED(IDC_PROXY_DEL, OnProxyDel)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_PROXY_LIST, OnEndlabeleditProxyList)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	ON_BN_CLICKED(IDC_MODIFYPROXY, OnModifyproxy)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CATES, OnItemchangedCates)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_CATES, OnEndlabeleditCates)
	ON_BN_CLICKED(IDC_NEWCATE, OnNewcate)
	ON_EN_CHANGE(IDC_BYPASS, OnChangeBypass)
	ON_BN_CLICKED(IDC_DELCATE, OnDelcate)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PROXY_LIST, OnItemchangedProxyList)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_PROXY_LIST, OnBegindragProxyList)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(NM_DBLCLK, IDC_PROXY_LIST, OnDblclkProxyList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetProxy message handlers

BOOL CSetProxy::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogProxy");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogProxy");
	CDialog::OnInitDialog();

	m_ProxyList.SetExtendedStyle(m_ProxyList.GetExtendedStyle()| LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ProxyList.SetBkColor(GetSysColor(COLOR_WINDOW));

	m_tmpProxyCategory = new CProxyCategory(pmf->m_aProxyCategories);

	CString str;
	CRect rect;
	int i=0;
	m_ProxyList.GetWindowRect(rect);
	LOADSTR(str ,IDS_NAME);
	m_ProxyList.InsertColumn(0,str);
	m_ProxyList.SetColumnWidth(0, 110);
	LOADSTR(str ,IDS_URL);
	m_ProxyList.InsertColumn(1, str);
	m_ProxyList.SetColumnWidth(1, rect.Width()-180);
	LOADSTR(str ,IDS_SPEED);
	m_ProxyList.InsertColumn(2, str);
	m_ProxyList.SetColumnWidth(2, 50);

	//image
	CImageList img;
	img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 2, 1);
	HBITMAP hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP10));
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);

	m_CateList.SetExtendedStyle(m_CateList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_CateList.InsertColumn(0,"");
	m_CateList.GetWindowRect(rect);
	m_CateList.SetColumnWidth(0, rect.Width()-5);

	m_CateList.SetImageList(&img, LVSIL_SMALL);
	img.Detach();

	//fill with the default proxy list
	//category list
	m_nList = m_nSelProxyList;
	int icon;
    for(i=0; i<=m_tmpProxyCategory->m_ProxyCategory.GetUpperBound(); i++)
	{	
		if(m_tmpProxyCategory->m_ProxyCategory.ElementAt(i)->m_bIsWebProxy)
		{
			icon=0;
			m_CateList.InsertItem(i, m_tmpProxyCategory->m_ProxyCategory.ElementAt(i)->m_strCategoryName, icon);
		}
		
	}
//gf
//	{	
//		if(m_tmpProxyCategory->m_ProxyCategory.ElementAt(i)->m_bIsWebProxy)
//			icon=0;
//		else
//			icon=1;
//		m_CateList.InsertItem(i, m_tmpProxyCategory->m_ProxyCategory.ElementAt(i)->m_strCategoryName, icon);
// 	}
	CProxyList* pl=NULL;
	if(m_nList>=0 && m_nList<i)
		pl=m_tmpProxyCategory->m_ProxyCategory.GetAt(m_nList);
	if(pl!=NULL)
	{
		for(i=0; i<=pl->m_Proxies.GetUpperBound(); i++)
		{
			m_ProxyList.InsertItem(i, pl->m_Proxies.ElementAt(i)->m_strProxyName);

			m_ProxyList.SetItemText(i, 1, pl->m_Proxies.ElementAt(i)->m_strProxy);
			if(m_SelProxy == pl->m_Proxies.ElementAt(i)->m_strProxy)
				m_ProxyList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);

			LOADSTR(str ,IDS_SP_INVALID + pl->m_Proxies.ElementAt(i)->m_nProxySpeed);
			m_ProxyList.SetItemText(i, 2, str);

		}
		m_ByPass.SetWindowText(pl->m_strProxyByPass);
	}

	m_CateList.SetItemState(m_nList, LVIS_SELECTED, LVIS_SELECTED);

	m_bUnUpdateProxy = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetProxy::OnOK() 
{
	// TODO: Add extra validation here
	try{	 
	pmf->m_aProxyCategories.Copy(*m_tmpProxyCategory);
	delete m_tmpProxyCategory;
	}catch(...){}

	CDialog::OnOK();
}

void CSetProxy::OnProxyNew() 
{
	// TODO: Add your control notification handler code here
	if(m_nList<0)
		return;

	POSITION pos = m_CateList.GetFirstSelectedItemPosition();
	if(pos==NULL)
	{
		m_bUnUpdateProxy = TRUE;
		m_CateList.SetItemState(m_nList, LVIS_SELECTED, LVIS_SELECTED);
	}

	CProxyInfo proxyinfo;
	CProxyList* pl = m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList);
	proxyinfo.m_Proxy = "http://";

	if(proxyinfo.DoModal() == IDOK)
	{
		proxyinfo.m_Proxy.TrimLeft();
		proxyinfo.m_Proxy.TrimRight();

		if(proxyinfo.m_ProxyName.IsEmpty())
			proxyinfo.m_ProxyName = proxyinfo.m_Proxy;

		int i; LIST_GET_INSERT_POS(m_ProxyList,&i);
		m_ProxyList.InsertItem(i, proxyinfo.m_ProxyName);
		m_ProxyList.SetItemText(i, 1, proxyinfo.m_Proxy);
		CString str;
		LOADSTR(str ,IDS_SP_INVALID + proxyinfo.m_Speed);
		m_ProxyList.SetItemText(i, 2, str);
		m_ProxyList.EnsureVisible(i, FALSE);
		m_ProxyList.SetFocus();

		CProxyItem* pi;
		pi = new CProxyItem;
		pi->m_nProxySpeed = proxyinfo.m_Speed;
		pi->m_strProxy = proxyinfo.m_Proxy;
		pi->m_strProxyName = proxyinfo.m_ProxyName;
		pl->m_Proxies.InsertAt(i, pi);
	}
}

void CSetProxy::OnProxyDel() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ProxyList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nItem = m_ProxyList.GetNextSelectedItem(pos);
		m_ProxyList.DeleteItem(nItem);
		delete m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.GetAt(nItem);
		m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.RemoveAt(nItem);
		CProxyList* pl=m_tmpProxyCategory->m_ProxyCategory.GetAt(m_nList);

		if(nItem<m_ProxyList.GetItemCount())
		{
			m_ProxyList.SetItemState(nItem, LVIS_SELECTED,LVIS_SELECTED);
			if(!pl->m_bIsWebProxy)
				m_SelProxy = m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(nItem)->m_strProxy;
			else
				m_SelWebProxy = m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(nItem)->m_strProxy;
		}
		else
		{
			if(!pl->m_bIsWebProxy)
				m_SelProxy = "";
			else
				m_SelWebProxy = "";
		}
	}
	
}

void CSetProxy::OnEndlabeleditProxyList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString str;
	m_ProxyList.GetEditControl()->GetWindowText(str);
	str.TrimLeft();
	str.TrimRight();

	m_ProxyList.SetItemText(pDispInfo->item.iItem,0,str);
	
	m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(pDispInfo->item.iItem)->m_strProxyName = str;

	*pResult = 0;
}

void CSetProxy::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	POSITION pos = m_ProxyList.GetFirstSelectedItemPosition();
	int nItem = -1, newItem;
	if(pos!=NULL)
	{
		nItem = m_ProxyList.GetNextSelectedItem(pos);
		if(pNMUpDown->iDelta<0)
			newItem = nItem -1;
		else
			newItem = nItem + 1;
		int n = m_ProxyList.GetItemCount();
		if(newItem>=0 && newItem<n)
		{
			CString str = m_ProxyList.GetItemText(nItem,0);
			CString str2 = m_ProxyList.GetItemText(nItem,1);
			m_ProxyList.DeleteItem(nItem);

			m_ProxyList.InsertItem(newItem, str);
			m_ProxyList.SetItemText(newItem, 1, str2);
			LOADSTR(str ,IDS_SP_INVALID + m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(nItem)->m_nProxySpeed);
			m_ProxyList.SetItemText(newItem, 2, str);
			m_ProxyList.SetItemState(newItem, LVIS_SELECTED, LVIS_SELECTED);
			m_ProxyList.EnsureVisible(newItem, FALSE);

			int tmp;
			CString strtmp;
			strtmp = m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(nItem)->m_strProxyName;
			m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(nItem)->m_strProxyName = m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(newItem)->m_strProxyName;
			m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(newItem)->m_strProxyName = strtmp;

			strtmp = m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(nItem)->m_strProxy;
			m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(nItem)->m_strProxy = m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(newItem)->m_strProxy;
			m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(newItem)->m_strProxy = strtmp;
	
			tmp = m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(nItem)->m_nProxySpeed;
			m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(nItem)->m_nProxySpeed = m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(newItem)->m_nProxySpeed;
			m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(newItem)->m_nProxySpeed = tmp;
		}
	}

	*pResult = 0;
}

void CSetProxy::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	POSITION pos = m_CateList.GetFirstSelectedItemPosition();
	int nItem = -1, newItem;
	if(pos!=NULL)
	{
		nItem = m_CateList.GetNextSelectedItem(pos);
		if(pNMUpDown->iDelta<0)
			newItem = nItem -1;
		else
			newItem = nItem + 1;
		int n = m_CateList.GetItemCount();
		if(newItem>=0 && newItem<n)
		{
			CString str = m_CateList.GetItemText(nItem,0);
			CProxyList* pl;
			pl = m_tmpProxyCategory->m_ProxyCategory.GetAt(nItem);
			m_tmpProxyCategory->m_ProxyCategory.SetAt(nItem, m_tmpProxyCategory->m_ProxyCategory.GetAt(newItem));
			m_tmpProxyCategory->m_ProxyCategory.SetAt(newItem, pl);

			int icon;
			if(pl->m_bIsWebProxy)
				icon=0;
			else
				icon=1;
			m_CateList.DeleteItem(nItem);

			m_bUnUpdateProxy = TRUE;
			m_CateList.InsertItem(newItem, str, icon);
			m_CateList.SetItemState(newItem, LVIS_SELECTED, LVIS_SELECTED);
			m_CateList.EnsureVisible(newItem, FALSE);
			m_nList = newItem;
		}
	}

	*pResult = 0;
}

void CSetProxy::OnModifyproxy() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ProxyList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nItem = m_ProxyList.GetNextSelectedItem(pos);
		CProxyInfo proxyinfo;
		proxyinfo.m_ProxyName = m_ProxyList.GetItemText(nItem, 0);
		proxyinfo.m_Proxy = m_ProxyList.GetItemText(nItem, 1);
		proxyinfo.m_Speed = m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(nItem)->m_nProxySpeed;
		if(proxyinfo.DoModal() == IDOK)
		{
			proxyinfo.m_Proxy.TrimLeft();
			proxyinfo.m_Proxy.TrimRight();
			if(proxyinfo.m_ProxyName.IsEmpty())
				proxyinfo.m_ProxyName = proxyinfo.m_Proxy;

			m_ProxyList.SetItemText(nItem, 0, proxyinfo.m_ProxyName);
			m_ProxyList.SetItemText(nItem, 1, proxyinfo.m_Proxy);

			CString str;
			LOADSTR(str ,IDS_SP_INVALID + proxyinfo.m_Speed);
			m_ProxyList.SetItemText(nItem, 2, str);

			m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(nItem)->m_strProxyName = proxyinfo.m_ProxyName;
			m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(nItem)->m_strProxy = proxyinfo.m_Proxy;
			m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_Proxies.ElementAt(nItem)->m_nProxySpeed = proxyinfo.m_Speed;
		}	
	}	
}


void CSetProxy::OnItemchangedCates(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	POSITION pos = m_CateList.GetFirstSelectedItemPosition();
	if(pos!=NULL && !m_bUnUpdateProxy)
	{
		int nItem = m_CateList.GetNextSelectedItem(pos);
		if(nItem!=m_nList)
			FillProxyList(nItem);
	}
	
	m_bUnUpdateProxy = FALSE;
	*pResult = 0;
}

void CSetProxy::FillProxyList(int index)
{
	m_nList = index;

	//remove all first
	m_ProxyList.DeleteAllItems();
	m_ProxyList.SetItemCount(0);

	CProxyList* pl;
	CString str;
	pl=m_tmpProxyCategory->m_ProxyCategory.GetAt(index);
    for(int i=0; i<=pl->m_Proxies.GetUpperBound(); i++)
	{
		m_ProxyList.InsertItem(i, pl->m_Proxies.ElementAt(i)->m_strProxyName);

		m_ProxyList.SetItemText(i, 1, pl->m_Proxies.ElementAt(i)->m_strProxy);
		if(m_SelProxy == pl->m_Proxies.ElementAt(i)->m_strProxy || m_SelWebProxy == pl->m_Proxies.ElementAt(i)->m_strProxy)
			m_ProxyList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);

		LOADSTR(str ,IDS_SP_INVALID + pl->m_Proxies.ElementAt(i)->m_nProxySpeed);
		m_ProxyList.SetItemText(i, 2, str);

	}

	if(!pl->m_bIsWebProxy)
	{
		m_ByPass.SetWindowText(pl->m_strProxyByPass);
		m_ByPass.EnableWindow(TRUE);
	}
	else
	{
		m_ByPass.SetWindowText("");
		m_ByPass.EnableWindow(FALSE);
	}

}

void CSetProxy::OnEndlabeleditCates(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString str;
	m_CateList.GetEditControl()->GetWindowText(str);
	str.TrimLeft();
	str.TrimRight();

	m_CateList.SetItemText(pDispInfo->item.iItem,0,str);
	m_tmpProxyCategory->m_ProxyCategory.ElementAt(pDispInfo->item.iItem)->m_strCategoryName = str;
	
	*pResult = 0;
}

void CSetProxy::OnNewcate() 
{
	// TODO: Add your control notification handler code here
	CString def;
	LOADSTR(def ,IDS_PROXY_SERVER);
	CString str(def);

	char si[5]; int i=0;
	LVFINDINFO info;
	info.flags = LVFI_STRING;
	info.psz = str;

	while (m_CateList.FindItem(&info) != -1)
	{
		i++;
		itoa(i, si, 10);
		str = def;
		str += si;
		info.psz = str;
	}

	CProxyList* pl = new CProxyList;
	pl->m_bIsWebProxy = FALSE;
	pl->m_strCategoryName = str;
	pl->m_strProxyByPass = "<local>";
	m_tmpProxyCategory->m_ProxyCategory.Add(pl);
	
	i = m_CateList.GetItemCount();
	m_CateList.InsertItem(i, str,1);
	m_CateList.SetFocus();
	m_CateList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);

	m_ByPass.SetWindowText(pl->m_strProxyByPass);
	m_CateList.EnsureVisible(i,FALSE);
	m_CateList.EditLabel(i);
}

void CSetProxy::OnChangeBypass() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString str;
	m_ByPass.GetWindowText(str);
	m_tmpProxyCategory->m_ProxyCategory.ElementAt(m_nList)->m_strProxyByPass = str;
}

void CSetProxy::OnDelcate() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_CateList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		if(MSGBOX(IDS_COMFIRM_CATE_DEL, MB_ICONQUESTION|MB_YESNO)==IDNO)
			return;

		int nItem = m_CateList.GetNextSelectedItem(pos);
		m_CateList.DeleteItem(nItem);
		CProxyList* pl=m_tmpProxyCategory->m_ProxyCategory.GetAt(nItem);
		if(pl->m_bIsWebProxy)
			m_SelWebProxy = "";
		else if(nItem == m_nSelProxyList)
			m_SelProxy="";
		delete pl;
		m_tmpProxyCategory->m_ProxyCategory.RemoveAt(nItem);

		if(nItem<m_CateList.GetItemCount())
		{
			m_CateList.SetItemState(nItem, LVIS_SELECTED,LVIS_SELECTED);
			m_nList = nItem;
			FillProxyList(nItem);
		}
		else if(m_CateList.GetItemCount()>0)
		{
			m_CateList.SetItemState(0, LVIS_SELECTED,LVIS_SELECTED);
			m_nList = 0;
		}
		else
		{
			m_nSelProxyList = m_nList = -1;
			m_ProxyList.DeleteAllItems();
			m_ProxyList.SetItemCount(0);
		}
		if(m_nList>=0)
		{
			pl=m_tmpProxyCategory->m_ProxyCategory.GetAt(nItem);
			if(!pl->m_bIsWebProxy)
				m_nSelProxyList = m_nList;
		}
	}	
}

void CSetProxy::OnItemchangedProxyList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	POSITION pos = m_ProxyList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		CProxyList* pl;
		pl=m_tmpProxyCategory->m_ProxyCategory.GetAt(m_nList);
		int nItem = m_ProxyList.GetNextSelectedItem(pos);
		if(nItem>=0)
		{
			if(pl->m_bIsWebProxy)
				m_SelWebProxy = pl->m_Proxies.ElementAt(nItem)->m_strProxy;
			else
			{
				m_SelProxy = pl->m_Proxies.ElementAt(nItem)->m_strProxy;
				m_nSelProxyList = m_nList;
			}
		}
	}	

	*pResult = 0;
}

void CSetProxy::OnBegindragProxyList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	// save the index of the item being dragged in m_nDragIndex
	m_nDragIndex = pNMListView->iItem;
	POINT pt;
	pt.x = 8;
	pt.y = 8;

	if(m_pDragImage)
		delete m_pDragImage;
	
	m_pDragImage = m_ProxyList.CreateDragImage (m_nDragIndex, &pt);
	ASSERT (m_pDragImage);
	// changes the cursor to the drag image (DragMove() is still required in 
	// OnMouseMove())
	VERIFY (m_pDragImage->BeginDrag (0, CPoint (8, 8)));
	VERIFY (m_pDragImage->DragEnter (GetDesktopWindow (), pNMListView->ptAction));
	// set dragging flag
	m_bDragging = TRUE;
	m_nDropIndex = -1;
	m_pDropWnd = &m_CateList;
	// capture all mouse messages
	SetCapture ();
	
	*pResult = 0;
}

void CSetProxy::OnLButtonUp(UINT nFlags, CPoint point) 
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
		m_CateList.SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);
		// redraw previous item
		m_CateList.RedrawItems (m_nDropIndex, m_nDropIndex);

		CPoint pt (point);
		ClientToScreen (&pt);
		// get the CWnd pointer of the window that is under the mouse cursor
		// if window is CTreeCtrl
		CWnd* pDropWnd = WindowFromPoint (pt);
		if (pDropWnd == m_pDropWnd && m_nDropIndex>=0 && m_nDropIndex!=m_nList)
		{
			CProxyList* pl, *pl2;
			pl = m_tmpProxyCategory->m_ProxyCategory.GetAt(m_nList);
			pl2 = m_tmpProxyCategory->m_ProxyCategory.GetAt(m_nDropIndex);

			if(pl->m_bIsWebProxy != pl2->m_bIsWebProxy)
				MSGBOX(IDS_CANT_MOVE_PROXY, MB_ICONEXCLAMATION);
			else
			{
				//add to new list
				CProxyItem* pi;
				pi = new CProxyItem;
				pi->m_nProxySpeed = pl->m_Proxies.ElementAt(m_nDragIndex)->m_nProxySpeed;
				pi->m_strProxy = pl->m_Proxies.ElementAt(m_nDragIndex)->m_strProxy;
				pi->m_strProxyName = pl->m_Proxies.ElementAt(m_nDragIndex)->m_strProxyName;
				pl2->m_Proxies.Add(pi);
				if(!PRESS_CTRL)
					OnProxyDel();
			}
		}
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CSetProxy::OnMouseMove(UINT nFlags, CPoint point) 
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

//#pragma optimize( "s", off )

void CSetProxy::OnDblclkProxyList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnModifyproxy();
	*pResult = 0;
}
