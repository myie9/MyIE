// SelectURL.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "SelectURL.h"
#include "mainfrm.h"
#include "wininet.h"//for INTERNET_MAX_PATH_LENGTH
#include "InputDlg.h"
#include "RegHelper.h"
#include "Consts_gf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectURL dialog


CSelectURL::CSelectURL(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectURL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectURL)
	//}}AFX_DATA_INIT
	m_nUrlType = 0;
}


void CSelectURL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectURL)
	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectURL, CDialog)
	//{{AFX_MSG_MAP(CSelectURL)
	ON_BN_CLICKED(IDC_SELECT_ALL, OnSelectAll)
	ON_BN_CLICKED(IDC_SELECT_NONE, OnSelectNone)
	ON_BN_CLICKED(IDC_OFFLINE, OnOffline)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_BN_CLICKED(IDC_DEL_CURRENT, OnDelCurrent)
	ON_BN_CLICKED(IDC_DEL_URL, OnDelUrl)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab)
	ON_BN_CLICKED(IDC_EDIT_TITLE, OnEditTitle)
	ON_BN_CLICKED(IDC_EDIT_URL, OnEditUrl)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST, OnRdblclkList)
	ON_BN_CLICKED(IDC_OPEN_CURRENT, OnOpenCurrent)
	ON_BN_CLICKED(IDC_OPEN_SELECTED, OnOpenSelected)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectURL message handlers
void CSelectURL::LoadLastVisiting()
{
	char state[10]="state",name[9]="name",url[8]="url"; // x+5
	CString filename = theApp.m_strUser+"LastVisit.ini";
	int i=0,nState;
	char tmp[INTERNET_MAX_PATH_LENGTH];
	BOOL r = TRUE;
	CString strUrl;
	while(r)
	{
		itoa(i, state+5, 10);
		itoa(i, name+4, 10);
		itoa(i, url+3, 10);
		nState = ::GetPrivateProfileInt("LastVisit", state, 0, filename);
		r = ::GetPrivateProfileString("LastVisit", url, NULL, tmp, INTERNET_MAX_PATH_LENGTH, filename);
		if (r)
		{
			strUrl = tmp;
			::GetPrivateProfileString("LastVisit", name, NULL, tmp, INTERNET_MAX_PATH_LENGTH, filename);
			m_list.InsertItem(i, tmp, nState);//the name maybe null
			m_list.SetItemText(i,1,strUrl);
		}
		i++;
	}
	m_list.AddSortIndexData();
}

void CSelectURL::LoadLastClosed()
{
	char name[9]="name",url[8]="url"; // x+5
	CString filename = theApp.m_strUser+"LastClose.ini";
	int i=0;
	char tmp[INTERNET_MAX_PATH_LENGTH];
	BOOL r = TRUE;
	while(r)
	{
		itoa(i, name+4, 10);
		itoa(i, url+3, 10);
		r = GetPrivateProfileString("LastClose", name, NULL, tmp, INTERNET_MAX_PATH_LENGTH, filename);
		if (r)
		{
			m_list.InsertItem(0, tmp,TI_NO);
			r = ::GetPrivateProfileString("LastClose", url, NULL, tmp, INTERNET_MAX_PATH_LENGTH, filename);
			if (r)
				m_list.SetItemText(0,1,tmp);
		}
		i++;
	}
	m_list.AddSortIndexData();
}

void CSelectURL::LoadVisting()
{
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
				if(tcf->m_strStickName.GetLength()>0)
					m_list.InsertItem(i, tcf->m_strStickName, tcf->m_pView->m_nOldIcon);
				else
					m_list.InsertItem(i, tcf->m_strTitle, tcf->m_pView->m_nOldIcon);
				m_list.SetItemText(i,1,tcf->m_pView->m_lpszUrl);
			}
		}
	}
	m_list.AddSortIndexData();
}

void CSelectURL::LoadClosed()
{
	//fill undo close
	for (int i =pmf->m_strUndoName.GetCount()-1; i>=0 ; i--)
	{
		m_list.InsertItem(0, pmf->m_strUndoName.GetAt(i),TI_NO);
		m_list.SetItemText(0,1,pmf->m_strUndoURL.GetAt(i));
	}
	m_list.AddSortIndexData();
}

BOOL CSelectURL::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogPageManager");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogPageManager");
	CDialog::OnInitDialog();

	try{
	m_list.SetExtendedStyle(m_list.GetExtendedStyle()| LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES|LVS_EX_HEADERDRAGDROP);
	//setup list ctrl
	CRect rect;
	m_list.GetWindowRect(&rect);
	CString str;
	LOADSTR(str ,IDS_TITLE);
	m_list.InsertColumn(0, str);
	m_list.SetColumnWidth(0, 200);
	LOADSTR(str ,IDS_URL);
	m_list.InsertColumn(1, str);
	m_list.SetColumnWidth(1, rect.Width()-230);
	//
	CImageList img;
	img.Create(pmf->m_wndTab.GetImageList());
	m_list.SetImageList(&img, LVSIL_SMALL);
	img.Detach();

	//load last visit
	LOADSTR(str ,IDS_LAST_VISITING_PAGES);
	m_tab.InsertItem(0, str);
	LOADSTR(str ,IDS_LAST_CLOSED_PAGES);
	m_tab.InsertItem(1, str);
	LOADSTR(str ,IDS_VISITING_PAGES);
	m_tab.InsertItem(2, str);
	LOADSTR(str ,IDS_CLOSED_PAGES);
	m_tab.InsertItem(3, str);
	//
	m_tab.SetCurSel(m_nUrlType);
	LoadNewUrl();
	//
	if (m_nUrlType==0)
		OnSelectAll();
	DWORD dwRet = _RegGetDword(HKEY_CURRENT_USER,
		"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings",
		"GlobalUserOffline"
		);
	CheckDlgButton(IDC_OFFLINE, dwRet);

	}catch(...){}
	// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
	return TRUE;
}

void CSelectURL::OkClosed()
{
		//gf no use
//	int i,count = m_list.GetItemCount();

//	pmf->m_nUndoStart = 0;
//	pmf->m_nUndoEnd = 0;
//	CString strTitle;
//	CString strUrl;
//	for (i=count-1;i>=0;i--)
//	{
//		strTitle = m_list.GetItemText(i,0);
//		strUrl = m_list.GetItemText(i,1);
//		pmf->m_strUndoName[pmf->m_nUndoEnd] = strTitle;
//		pmf->m_strUndoURL[pmf->m_nUndoEnd] = strUrl;
//		pmf->m_nUndoEnd = (pmf->m_nUndoEnd+1)%MAX_UNDO;
//		if(pmf->m_nUndoStart == pmf->m_nUndoEnd)
//			pmf->m_nUndoStart = (pmf->m_nUndoStart + 1)%MAX_UNDO;
//	}
	pmf->UpdateUndoMenu();
}

void CSelectURL::OnOK() 
{
	// TODO: Add extra validation here
	m_nUrlType = m_tab.GetCurSel();
	if (m_nUrlType!=2)
		OpenSelected();
	switch (m_nUrlType)
	{
	case 2:	pmf->SaveLastVisit(1); break;
	case 3: OkClosed();	break;
	}
	//
	CDialog::OnOK();
}

void CSelectURL::OnSelectNone() 
{
	// TODO: Add your control notification handler code here
	int i,count = m_list.GetItemCount();
	for(i=0;i<count;i++)
		m_list.SetCheck(i,FALSE);
}

void CSelectURL::OnSelectAll() 
{
	// TODO: Add your control notification handler code here
	int i,count = m_list.GetItemCount();
	for(i=0;i<count;i++)
		m_list.SetCheck(i,TRUE);
}

void CSelectURL::OnOffline() 
{
	// TODO: Add your control notification handler code here
	pmf->SendMessage(WM_COMMAND,	ID_FILE_WORKOFFLINE);
}

void CSelectURL::OnEditTitle() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int current = m_list.GetNextSelectedItem(pos);
	if (current<0)
		return;
	CInputDlg dlg;
	CString strMsg;
	LOADSTR(strMsg ,IDS_INPUT_TITLE);
	dlg.m_strMsg = strMsg;
	CString strData = m_list.GetItemText(current,0);
	dlg.m_strOutput = strData;
	
	if(dlg.DoModal() == IDOK)
		m_list.SetItemText(current,0,dlg.m_strOutput);
}

void CSelectURL::OnEditUrl() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int current = m_list.GetNextSelectedItem(pos);
	if (current<0)
		return;
	CInputDlg dlg;
	CString strMsg;
	LOADSTR(strMsg ,IDS_INPUT_URL);
	dlg.m_strMsg = strMsg;
	CString strData = m_list.GetItemText(current,1);
	dlg.m_strOutput = strData;
	
	if(dlg.DoModal() == IDOK)
		m_list.SetItemText(current,1,dlg.m_strOutput);
}

void CSelectURL::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnOpenCurrent();
	*pResult = 0;
}

void CSelectURL::OnRdblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*) pNMHDR;
	if (pNMListView->iSubItem==0)
		OnEditTitle();
	else
		OnEditUrl();
	*pResult = 0;
}

void CSelectURL::OnDelCurrent() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int current = m_list.GetNextSelectedItem(pos);
	if (current<0)
		return;
	//
	if (m_nUrlType==2)
	{
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		CChildFrame* tcf;
		pmf->m_wndTab.GetItem(current, &TabCtrlItem);
		tcf = ((CChildFrame*)TabCtrlItem.lParam);
		CLOSE_PAGE(tcf);
	}
	//
	m_list.DeleteItem(current);
	LIST_FOCUS_POS(m_list, current);
}

void CSelectURL::OnDelUrl() 
{
	// TODO: Add your control notification handler code here
	int i,count = m_list.GetItemCount();
	if (count<=0)
		return;
	//
	if (m_nUrlType==2)
	{
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		CChildFrame* tcf;

		int i,count = m_list.GetItemCount();
		int index=0;
		for (i=0;i<count;i++)
		{
			if (m_list.GetCheck(i))
			{
				pmf->m_wndTab.GetItem(i, &TabCtrlItem);
				tcf = ((CChildFrame*)TabCtrlItem.lParam);
				CLOSE_PAGE(tcf);
			}
		}
	}
	//
	for (i=count-1;i>=0;i--)
	{
		if (m_list.GetCheck(i))
			m_list.DeleteItem(i);
	}
}

void CSelectURL::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_nUrlType = m_tab.GetCurSel();
	LoadNewUrl();
	//
	*pResult = 0;
}

void CSelectURL::LoadNewUrl()
{
	m_list.DeleteAllItems();
	switch(m_nUrlType)
	{
	case 0:	LoadLastVisiting(); break;
	case 1:	LoadLastClosed(); break;
	case 2:	LoadVisting(); break;
	case 3:	LoadClosed(); break;
	}
}

void CSelectURL::OnOpenCurrent() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int current = m_list.GetNextSelectedItem(pos);
	if (current<0)
		return;
	if (m_nUrlType==2)//active current
	{
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		CChildFrame* tcf;
		pmf->m_wndTab.GetItem(current, &TabCtrlItem);
		tcf = ((CChildFrame*)TabCtrlItem.lParam);
		if(tcf != NULL)
		{
			pmf->MDIActivate(tcf);
			pmf->m_wndTab.SetCurSel(current);
		}
	}
	else//open
	{
		CString url = m_list.GetItemText(current,1);
		CChildFrame *tcf = pmf->NewChildWindow(0);
		if(tcf && tcf->m_pView)
		{
			tcf->m_pView->ToNavigate(url, 0, NULL);
			//
			LVITEMA item;
			item.mask = LVIF_IMAGE ;
			item.iSubItem = 0;
			item.iItem = current;
			m_list.GetItem(&item);
			if (item.iImage ==4)
				tcf->m_bLock = TRUE;
		}
	}
}

void CSelectURL::OpenSelected()
{
	BOOL bTemp = pmf->m_nNewSequence;
	pmf->m_nNewSequence = 0;
	CChildFrame *tcf;
	CString url;
	//
	LVITEMA item;
	item.mask = LVIF_IMAGE ;
	item.iSubItem = 0;
	//
	int count = m_list.GetItemCount();
	for (int i=0;i<count;i++)
	{
		if (m_list.GetCheck(i))
		{
			url = m_list.GetItemText(i,1);
			tcf = pmf->NewChildWindow(0);
			if(tcf && tcf->m_pView)
			{
				tcf->m_pView->ToNavigate(url, 0, NULL);
				//
				item.iItem = i;
				m_list.GetItem(&item);
				if (item.iImage ==4)
					tcf->m_bLock = TRUE;
			}
		}
	}
	pmf->m_nNewSequence  = bTemp;
}

void CSelectURL::OnOpenSelected() 
{
	// TODO: Add your control notification handler code here
	if (m_nUrlType==2)
		return;
	OpenSelected();
	OnCancel();	
}
