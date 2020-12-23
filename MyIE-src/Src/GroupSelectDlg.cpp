// GroupSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "GroupSelectDlg.h"
#include "mainfrm.h"

#include "GroupUrlDlg.h"
#include <wininet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroupSelectDlg dialog


CGroupSelectDlg::CGroupSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGroupSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroupSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGroupSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroupSelectDlg)
	DDX_Control(pDX, IDC_NAME, m_name);
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroupSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CGroupSelectDlg)
	ON_BN_CLICKED(IDC_GET_TIME, OnGetTime)
	ON_BN_CLICKED(IDC_SELECT_ALL, OnSelectAll)
	ON_BN_CLICKED(IDC_SELECT_NONE, OnSelectNone)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupSelectDlg message handlers


BOOL CGroupSelectDlg::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogGroupSelect");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogGroupSelect");
	CDialog::OnInitDialog();

	m_list.SetExtendedStyle(m_list.GetExtendedStyle()| LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES);
	m_list.SetBkColor(GetSysColor(COLOR_WINDOW));
	//setup list ctrl
	CRect rect;
	m_list.GetWindowRect(&rect);
	CString str;
	LOADSTR(str ,IDS_NAME);
	m_list.InsertColumn(0, str);
	m_list.SetColumnWidth(0, 150);
	LOADSTR(str ,IDS_URL);
	m_list.InsertColumn(1, str);
	m_list.SetColumnWidth(1, rect.Width()-180);
	//
	m_list.SetFocus();

	// TODO: Add extra initialization here
	try{
	CChildFrame* tcf;
	int n = pmf->m_wndTab.GetItemCount();
	pmf->m_nWin = n;
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;

	for( int i=0; i<n; i++)
	{
		pmf->m_wndTab.GetItem(i, &TabCtrlItem);
		tcf = ((CChildFrame*)TabCtrlItem.lParam);
		if(tcf != NULL && tcf->m_pView!=NULL)
		{
			if(!tcf->m_pView->m_lpszUrl.IsEmpty() && !tcf->m_pView->m_bToClose)
			{
				m_list.InsertItem(i,tcf->m_strTitle);
				m_list.SetItemText(i,1,tcf->m_pView->m_lpszUrl);
				m_list.SetItemData(i,tcf->m_pView->m_dwProperty);//DEFAULT_PROPERTY);
			}
		}
	}
	OnSelectAll();
	
	//load group list
	CString strMenu;
	for (i=0; i<=pmf->m_astrGroup.GetUpperBound(); i++)
	{
		strMenu = pmf->m_astrGroup.GetAt(i);
		strMenu = strMenu.Left(strMenu.GetLength()-4);
		m_name.InsertString(i, strMenu);
	}
	
	OnGetTime();

	m_name.SetFocus();
	}catch(...){}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGroupSelectDlg::OnGetTime() 
{
	// TODO: Add your control notification handler code here
	CString strTime;
	SYSTEMTIME time;
	::GetLocalTime(&time);
	strTime.Format("%02d_%02d-%02d.%02d.%02d",time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);
	m_name.SetWindowText(strTime);
	m_name.SetEditSel(0,-1);
	m_name.SetFocus();
}

void CGroupSelectDlg::OnOK() 
{
	// TODO: Add extra validation here
	int oldCount=0;
	BOOL r = TRUE;
	CString filename;
	m_name.GetWindowText(filename);
	int pos = filename.FindOneOf(STR_ERR_PATH_CHAR);
	if (pos>=0)
	{
		MSGBOX(IDS_GROUPNAME_ERROR);
		return;
	}
	//
	char state[10]="state",name[9]="name",url[8]="url",download[13]="download"; // x+5
	char num[15];
	filename = theApp.m_strGroupPath + filename+".cgp";
	//get old count 
	char tmp[INTERNET_MAX_PATH_LENGTH];
	while(r)
	{
		itoa(oldCount, name+4, 10);
		itoa(oldCount, url+3, 10);
		r = ::GetPrivateProfileString("Group", name, NULL, tmp, INTERNET_MAX_PATH_LENGTH, filename);
		if (!r)
			break;
		oldCount++;
	}
	//
	if (oldCount<1)
	{
		r = WritePrivateProfileSection("Group", NULL, filename);
		if (!r)
		{
			if (!_FileIsExist(theApp.m_strGroupPath))
				MSGBOX(IDS_GROUP_NOT_EXIST);
			else
				MSGBOX(IDS_GROUPNAME_ERROR);
			return;
		}
	}
	//write new item
	int i,count = m_list.GetItemCount();
	int index=0;
	for (i=0;i<count;i++)
	{
		if (m_list.GetCheck(i))
		{
			itoa(index+oldCount, state+5, 10);
			itoa(index+oldCount, name+4, 10);
			itoa(index+oldCount, url+3, 10);
			itoa(index+oldCount, download+8, 10);
			//::WritePrivateProfileString("Group", state, "1", filename);
			::WritePrivateProfileString("Group", name, m_list.GetItemText(i,0), filename);
			::WritePrivateProfileString("Group", url, m_list.GetItemText(i,1), filename);
			if (m_list.GetItemData(i) != DEFAULT_PROPERTY)
			{
				ultoa( m_list.GetItemData(i), num, 10);
				::WritePrivateProfileString("Group", download, num, filename);
			}
			//
			index++;
		}
	}

	CDialog::OnOK();
}

void CGroupSelectDlg::OnSelectAll() 
{
	// TODO: Add your control notification handler code here
	int i,count = m_list.GetItemCount();
	for(i=0;i<count;i++)
		m_list.SetCheck(i,TRUE);
}

void CGroupSelectDlg::OnSelectNone() 
{
	// TODO: Add your control notification handler code here
	int i,count = m_list.GetItemCount();
	for(i=0;i<count;i++)
		m_list.SetCheck(i,FALSE);
}

void CGroupSelectDlg::OnUpdate() 
{
	// TODO: Add your control notification handler code here
	int item;
	int i = m_list.GetSelectedCount();
	if(i>0)
	{
		POSITION pos = m_list.GetFirstSelectedItemPosition();
		item = m_list.GetNextSelectedItem(pos);
	}
	else
		return;
	
	CGroupUrlDlg dlg;
	dlg.m_strName = m_list.GetItemText(item, 0);
	dlg.m_strUrl = m_list.GetItemText(item,1);
	dlg.m_bState = m_list.GetCheck(item);
	dlg.m_dwProperty = m_list.GetItemData(item);
	if(dlg.DoModal() == IDOK)
	{
		CString str = dlg.m_strName;
		str.TrimLeft();
		str.TrimRight();
		if(str.IsEmpty())
			GetUnqBlankName(str);
		m_list.SetItemText(item, 0, str);
		dlg.m_strUrl.TrimLeft();
		m_list.SetItemText(item, 1, dlg.m_strUrl);
		m_list.SetCheck(item, dlg.m_bState);
		m_list.SetItemData(item,dlg.m_dwProperty);
	}	
}

void CGroupSelectDlg::GetUnqBlankName(CString &newblnk)
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
	}while(m_list.FindItem(&info) != -1);
}

void CGroupSelectDlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int nItem = -1, newItem;
	if(pos!=NULL)
	{
		nItem = m_list.GetNextSelectedItem(pos);
		if(pNMUpDown->iDelta<0)
			newItem = nItem -1;
		else
			newItem = nItem + 1;
		int n = m_list.GetItemCount();
		if(newItem>=0 && newItem<n)
		{
			CString str = m_list.GetItemText(nItem,0);
			CString str2 = m_list.GetItemText(nItem, 1);
			BOOL	bState = m_list.GetCheck(nItem);
			DWORD	dwProperty= m_list.GetItemData(nItem);
			m_list.DeleteItem(nItem);

			m_list.InsertItem(newItem, str);
			m_list.SetItemText(newItem, 1, str2);
			m_list.SetItemState(newItem, LVIS_SELECTED, LVIS_SELECTED);
			m_list.SetCheck(newItem, bState);
			m_list.SetItemData(newItem, dwProperty);
			m_list.EnsureVisible(newItem, FALSE);

		}
	}
	
	*pResult = 0;
}

void CGroupSelectDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int item;
	int i = m_list.GetSelectedCount();
	if(i>0)
	{
		POSITION pos = m_list.GetFirstSelectedItemPosition();
		item = m_list.GetNextSelectedItem(pos);
	}
	else
		return;
	OnUpdate();

	*pResult = 0;
}
