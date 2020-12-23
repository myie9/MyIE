// QuickSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "QuickSearchDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on )

#define INVALID_SEARCH_ENGINE(url,alias)		(url.Find("%s")<0 && alias!="--" && url.Find("%u")<0)
/////////////////////////////////////////////////////////////////////////////
// CQuickSearchDlg dialog


CQuickSearchDlg::CQuickSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQuickSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQuickSearchDlg)
	//}}AFX_DATA_INIT
}


void CQuickSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuickSearchDlg)
	DDX_Control(pDX, IDC_ICON_PATH, m_cIcon);
	DDX_Control(pDX, IDC_SEARCHENGINE, m_cSearchEngine);
	DDX_Control(pDX, IDC_NAME, m_cName);
	DDX_Control(pDX, IDC_SEACHLIST, m_EngineList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQuickSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CQuickSearchDlg)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SEACHLIST, OnItemchangedSeachlist)
	ON_BN_CLICKED(IDC_CLEAN, OnClean)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnDeltaposSpin)
	ON_BN_CLICKED(IDC_GET_ICON, OnGetIcon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuickSearchDlg message handlers

BOOL CQuickSearchDlg::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogSearch");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogSearch");
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_EngineList.SetExtendedStyle(m_EngineList.GetExtendedStyle()| LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);

	//setup list ctrl
	CRect rect;
	int i=0;
	m_bUpdating = FALSE;
	CString str;
	LOADSTR(str ,IDS_NAME);
	m_EngineList.InsertColumn(0,str);
	m_EngineList.GetWindowRect(rect);
	m_EngineList.SetColumnWidth(0, 120);
	//
	LOADSTR(str ,IDS_SEARCH_ENGINE);
	m_EngineList.InsertColumn(1,str);
	m_EngineList.SetColumnWidth(1, rect.Width()-200);
	//
	LOADSTR(str ,IDS_ICON);
	m_EngineList.InsertColumn(2,str);
	m_EngineList.SetColumnWidth(2, 50);

	CSearch *item;
	for(i=0;i<=pmf->m_SearchList.m_Searchs.GetUpperBound();i++)
	{
		item = pmf->m_SearchList.m_Searchs.GetAt(i);
		m_EngineList.InsertItem(i, item->name);
		m_EngineList.SetItemText(i, 1, item->url);
		m_EngineList.SetItemText(i, 2, item->icon);
		m_EngineList.SetCheck(i,item->state);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CQuickSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	OnUpdate();
	//
	pmf->m_SearchList.Clean();
	//
	CSearch *ps;
	int n = m_EngineList.GetItemCount();
	pmf->m_SearchList.m_nIconCount = 0;
	for( int i=0; i<n; i++)
	{
		ps = new CSearch;
		ps->state = m_EngineList.GetCheck(i);
		ps->name = m_EngineList.GetItemText(i,0);
		ps->url  = m_EngineList.GetItemText(i,1);
		ps->icon = m_EngineList.GetItemText(i,2);
		pmf->m_SearchList.m_Searchs.Add(ps);
		if (ps->icon.GetLength())
			pmf->m_SearchList.m_nIconCount++;
	}

	CDialog::OnOK();
}

void CQuickSearchDlg::OnUpdate()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_EngineList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int i = m_EngineList.GetNextSelectedItem(pos);
		CString alias;
		m_cName.GetWindowText(alias);
		alias.TrimLeft(); alias.TrimRight();
		CString url;
		m_cSearchEngine.GetWindowText(url);
		url.TrimLeft();
		if (alias.GetLength()<1 || url.GetLength()<2)
			return;
		CString icon;
		m_cIcon.GetWindowText(icon);

		if(INVALID_SEARCH_ENGINE(url,alias))
		{
			MSGBOX(IDS_NEEDKEYSTRING);
			return;
		}

		if(!alias.IsEmpty() && !url.IsEmpty() || alias=="--" )
		{
			m_bUpdating = TRUE;
			m_EngineList.SetItemText(i, 0, alias);
			m_EngineList.SetItemText(i, 1, url);
			m_EngineList.SetItemText(i, 2, icon);
		}
	}	
}

void CQuickSearchDlg::OnInsert() 
{
	// TODO: Add your control notification handler code here
	CString alias;
	m_cName.GetWindowText(alias);
	CString url;
	m_cSearchEngine.GetWindowText(url);
	CString icon;
	m_cIcon.GetWindowText(icon);
	
	if(INVALID_SEARCH_ENGINE(url,alias))
	{
		MSGBOX(IDS_NEEDKEYSTRING);
		return;
	}

	alias.TrimLeft(); alias.TrimRight();
	url.TrimLeft();

	int count = m_EngineList.GetItemCount();
	CString existAlias;
	for (int i=0;i<count;i++)
	{
		existAlias = m_EngineList.GetItemText(i,0);
		if (existAlias==alias && alias!="--")
		{
			MSGBOX(IDS_NAME_EXISTED);
			return;
		}
	}
	//
	if(!alias.IsEmpty() && !url.IsEmpty() || alias=="--")
	{
		int item; LIST_GET_INSERT_POS(m_EngineList,&item);
		m_EngineList.InsertItem(item, alias);
		m_EngineList.SetItemText(item, 1, url);
		m_EngineList.SetItemText(item, 2, icon);
		LIST_FOCUS_POS(m_EngineList, item);
		//
		m_cName.SetWindowText("");
		m_cSearchEngine.SetWindowText("");
		m_cIcon.SetWindowText("");
	}
}

void CQuickSearchDlg::OnDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_EngineList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int i = m_EngineList.GetNextSelectedItem(pos);
		m_EngineList.DeleteItem(i);
		//
		LIST_FOCUS_POS(m_EngineList,i);
	}
}

void CQuickSearchDlg::OnItemchangedSeachlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(m_bUpdating)
	{
		m_bUpdating = FALSE;
		return;
	}

	int i, nIndex = pNMListView->iItem;
	POSITION pos = m_EngineList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		i = m_EngineList.GetNextSelectedItem(pos);
	}
	if(nIndex==i)
	{
		m_cName.SetWindowText(m_EngineList.GetItemText(nIndex,0));
		m_cSearchEngine.SetWindowText(m_EngineList.GetItemText(nIndex,1));
		m_cIcon.SetWindowText(m_EngineList.GetItemText(nIndex,2));
	}
	else
	{
		m_cName.SetWindowText("");
		m_cSearchEngine.SetWindowText("");
		m_cIcon.SetWindowText("");
	}

	*pResult = 0;
}

//#pragma optimize( "s", off)

void CQuickSearchDlg::OnClean() 
{
	// TODO: Add your control notification handler code here
	m_cName.SetWindowText("");
	m_cSearchEngine.SetWindowText("");
	m_cIcon.SetWindowText("");
}

void CQuickSearchDlg::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	POSITION pos = m_EngineList.GetFirstSelectedItemPosition();
	int nItem = -1, newItem;
	if(pos!=NULL)
	{
		nItem = m_EngineList.GetNextSelectedItem(pos);
		if(pNMUpDown->iDelta<0)
			newItem = nItem -1;
		else
			newItem = nItem + 1;
		int n = m_EngineList.GetItemCount();
		if(newItem>=0 && newItem<n)
		{
			CString alias = m_EngineList.GetItemText(nItem,0);
			CString url = m_EngineList.GetItemText(nItem, 1);
			CString icon = m_EngineList.GetItemText(nItem, 2);
			BOOL	bState = m_EngineList.GetCheck(nItem);
			m_EngineList.DeleteItem(nItem);

			m_EngineList.InsertItem(newItem, alias);
			m_EngineList.SetItemText(newItem, 1, url);
			m_EngineList.SetItemText(newItem, 2, icon);
			m_EngineList.SetCheck(newItem, bState);
			//
			LIST_FOCUS_POS(m_EngineList,newItem);
		}
	}
	
	*pResult = 0;
}

void CQuickSearchDlg::OnGetIcon() 
{
	// TODO: Add your control notification handler code here
	CString str;
	LOADSTR(str ,IDS_TYPE_ICON);
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, str);
	if(fileDlg.DoModal() == IDOK)
		m_cIcon.SetWindowText(fileDlg.GetPathName());
}
