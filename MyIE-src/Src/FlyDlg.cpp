// FlyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "FlyDlg.h"
#include "MainFrm.h"
#include "HelperStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CFlyDlg dialog
IMPLEMENT_DYNCREATE(CFlyDlg, CSAPrefsSubDlg)

CFlyDlg::CFlyDlg() : CSAPrefsSubDlg(CFlyDlg::IDD)
{
	//{{AFX_DATA_INIT(CFlyDlg)

	m_bKillFly = FALSE;

	//}}AFX_DATA_INIT
}


void CFlyDlg::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlyDlg)
	DDX_Control(pDX, IDD_UN_FLYLIST, m_UnFlyList);
	DDX_Control(pDX, IDD_FLYLIST, m_FlyList);

	DDX_Check(pDX, IDC_ENABLE_FLY_CONTENT, m_bKillFly);
	DDX_Check(pDX, IDC_ENABLE_FLY_WHEN_REFRESH, m_bKillFlyWhenFresh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFlyDlg, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CFlyDlg)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDD_FLYLIST, OnEndlabeleditFlylist)
	ON_NOTIFY(NM_DBLCLK, IDD_FLYLIST, OnDblclkFlylist)
	
	ON_BN_CLICKED(IDC_ENABLE_FLY_CONTENT, OnEnableFlyWin)
	
	ON_BN_CLICKED(IDC_UN_INSERT, OnUnInsert)
	ON_BN_CLICKED(IDC_UN_DELETE, OnUnDelete)
	ON_BN_CLICKED(IDC_UN_EDIT, OnUnEdit)
	ON_NOTIFY(NM_DBLCLK, IDD_UN_FLYLIST, OnDblclkUnFlylist)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDD_UN_FLYLIST, OnEndlabeleditUnFlylist)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlyDlg message handlers

BOOL CFlyDlg::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogFlyFilter");
#endif
	LOADDLG("DialogFlyFilter");
	CSAPrefsSubDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	try{
	POSITION pos;
	int i;
	CRect rect;
	//
	pos = pmf->m_astrFly.GetHeadPosition();
	i=0;
	m_FlyList.InsertColumn(0,"");
	m_FlyList.GetWindowRect(rect);
	m_FlyList.SetColumnWidth(0, rect.Width()-25);
	while(pos!=NULL)
	{
		m_FlyList.InsertItem(i, pmf->m_astrFly.GetAt(pos));
		pmf->m_astrFly.GetNext(pos);
		i++;
	}
	//un filter
	pos = pmf->m_astrUnFly.GetHeadPosition();
	i=0;
	m_UnFlyList.InsertColumn(0,"");
	m_UnFlyList.GetWindowRect(rect);
	m_UnFlyList.SetColumnWidth(0, rect.Width()-25);
	while(pos!=NULL)
	{
		m_UnFlyList.InsertItem(i, pmf->m_astrUnFly.GetAt(pos));
		pmf->m_astrUnFly.GetNext(pos);
		i++;
	}
	//
	SetFlyGroupEnable(m_bKillFly);
	}catch(...){}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFlyDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString url;
	int i;
	int n ;

	pmf->m_astrFly.RemoveAll();
	n = m_FlyList.GetItemCount();
	for( i=0; i<n; i++)
	{
		url = pmf->GetStandUrl(m_FlyList.GetItemText(i,0));
		pmf->m_astrFly.AddTail(url);
	}
	//
	pmf->m_astrUnFly.RemoveAll();
	n = m_UnFlyList.GetItemCount();
	for( i=0; i<n; i++)
	{
		url = pmf->GetStandUrl(m_UnFlyList.GetItemText(i,0));
		pmf->m_astrUnFly.AddTail(url);
	}
	pmf->SaveFilterFlyList2File();

	CSAPrefsSubDlg::OnOK();
}

void CFlyDlg::OnDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_FlyList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nItem = m_FlyList.GetNextSelectedItem(pos);
		m_FlyList.DeleteItem(nItem);
		m_FlyList.SetItemState(nItem, LVIS_SELECTED,LVIS_SELECTED);
	}
}

void CFlyDlg::OnEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_FlyList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nItem = m_FlyList.GetNextSelectedItem(pos);
		m_FlyList.SetFocus();
		m_FlyList.EditLabel(nItem);
	}
}

void CFlyDlg::OnInsert() 
{
	// TODO: Add your control notification handler code here
	int i = m_FlyList.GetItemCount();
	m_FlyList.InsertItem(i, "http://");
	m_FlyList.SetFocus();
	m_FlyList.EnsureVisible(i,FALSE);
	m_FlyList.EditLabel(i);
}

void CFlyDlg::OnEndlabeleditFlylist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString str;
	m_FlyList.GetEditControl()->GetWindowText(str);

	str.MakeLower();
	m_FlyList.SetItemText(pDispInfo->item.iItem,0,str);
	*pResult = 0;
}

void CFlyDlg::OnDblclkFlylist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEdit();
	
	*pResult = 0;
}

//#pragma optimize( "s", off)

void CFlyDlg::OnEnableFlyWin() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd;
	int state;
	pWnd = GetDlgItem(IDC_ENABLE_FLY_CONTENT);
	state = ((CButton*)pWnd)->GetCheck();
	//
	SetFlyGroupEnable(state);
}

void CFlyDlg::OnUnInsert() 
{
	// TODO: Add your control notification handler code here
	int i = m_UnFlyList.GetItemCount();
	m_UnFlyList.InsertItem(i, "http://");
	m_UnFlyList.SetFocus();
	m_UnFlyList.EnsureVisible(i,FALSE);
	m_UnFlyList.EditLabel(i);	
}

void CFlyDlg::OnUnDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_UnFlyList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nItem = m_UnFlyList.GetNextSelectedItem(pos);
		m_UnFlyList.DeleteItem(nItem);
		m_UnFlyList.SetItemState(nItem, LVIS_SELECTED,LVIS_SELECTED);
	}	
}

void CFlyDlg::OnUnEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_UnFlyList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nItem = m_UnFlyList.GetNextSelectedItem(pos);
		m_UnFlyList.SetFocus();
		m_UnFlyList.EditLabel(nItem);
	}
}

void CFlyDlg::OnDblclkUnFlylist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnUnEdit();

	*pResult = 0;
}

void CFlyDlg::OnEndlabeleditUnFlylist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString str;
	m_UnFlyList.GetEditControl()->GetWindowText(str);

	str.MakeLower();

	m_UnFlyList.SetItemText(pDispInfo->item.iItem,0,str);
	*pResult = 0;
}


void CFlyDlg::SetFlyGroupEnable(BOOL state)
{
	
	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_ENABLE_FLY_WHEN_REFRESH);
	pWnd->EnableWindow(state);
	//
	
	SetListGroupEnable(state);
}

void CFlyDlg::SetListGroupEnable(BOOL state)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(IDD_FLYLIST);
	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDC_INSERT);
	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDC_DELETE);
	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDC_EDIT);
	pWnd->EnableWindow(state);


	pWnd = GetDlgItem(IDC_UN_FLY_STATIC);
	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDD_UN_FLYLIST);
 	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDC_UN_INSERT);
	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDC_UN_DELETE);
	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDC_UN_EDIT);
 	pWnd->EnableWindow(state);
}
