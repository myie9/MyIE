// UrlFilterProp.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "UrlFilterProp.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUrlFilterProp property page

IMPLEMENT_DYNCREATE(CUrlFilterProp, CSAPrefsSubDlg)

CUrlFilterProp::CUrlFilterProp() : CSAPrefsSubDlg(CUrlFilterProp::IDD)
{
	//{{AFX_DATA_INIT(CUrlFilterProp)
	m_bUseUrlFilter = FALSE;
	m_strBlankContent = _T("");
	m_bKillBlank = FALSE;
	//}}AFX_DATA_INIT
}

void CUrlFilterProp::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUrlFilterProp)
	DDX_Control(pDX, IDC_UN_URL_LIST, m_cURLUnList);
	DDX_Control(pDX, IDC_URL_LIST, m_cURLList);
	DDX_Check(pDX, IDC_URL_FILTER, m_bUseUrlFilter);
	DDX_Text(pDX, IDC_BLANK_CONTENT, m_strBlankContent);
	DDX_Check(pDX, IDC_KILL_BLANK, m_bKillBlank);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUrlFilterProp, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CUrlFilterProp)
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_URL_LIST, OnDblclkUrlList)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_URL_LIST, OnEndlabeleditUrlList)
	ON_BN_CLICKED(IDC_URL_FILTER, OnUrlFilter)
	ON_BN_CLICKED(IDC_UN_INSERT, OnUnInsert)
	ON_BN_CLICKED(IDC_UN_DELETE, OnUnDelete)
	ON_BN_CLICKED(IDC_UN_EDIT, OnUnEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_UN_URL_LIST, OnDblclkUnUrlList)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_UN_URL_LIST, OnEndlabeleditUnUrlList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUrlFilterProp message handlers

void CUrlFilterProp::OnInsert() 
{
	// TODO: Add your control notification handler code here
	int i = m_cURLList.GetItemCount();
	m_cURLList.InsertItem(i, "http://");
	m_cURLList.SetFocus();
	m_cURLList.EnsureVisible(i,FALSE);
	m_cURLList.EditLabel(i);
}

void CUrlFilterProp::OnEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_cURLList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nItem = m_cURLList.GetNextSelectedItem(pos);
		m_cURLList.SetFocus();
		m_cURLList.EditLabel(nItem);
	}	
}

void CUrlFilterProp::OnDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_cURLList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nItem = m_cURLList.GetNextSelectedItem(pos);
		m_cURLList.DeleteItem(nItem);
		m_cURLList.SetItemState(nItem, LVIS_SELECTED,LVIS_SELECTED);
	}
}

BOOL CUrlFilterProp::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogContentFilter");
#endif
	LOADDLG("DialogContentFilter");
	CSAPrefsSubDlg::OnInitDialog();

	// TODO: Add extra initialization here
	int i=0;
	CRect rect;
	m_cURLList.InsertColumn(0,"");
	m_cURLList.GetWindowRect(rect);
	m_cURLList.SetColumnWidth(0, rect.Width()-25);
	for(i=0; i<=pmf->m_astrUrlFilter.GetUpperBound( ) ; i++)
	{
		m_cURLList.InsertItem(i, pmf->m_astrUrlFilter.GetAt(i));
	}
	m_cURLUnList.InsertColumn(0,"");
	m_cURLUnList.GetWindowRect(rect);
	m_cURLUnList.SetColumnWidth(0, rect.Width()-25);
	for(i=0; i<=pmf->m_astrUrlUnFilter.GetUpperBound( ) ; i++)
	{
		m_cURLUnList.InsertItem(i, pmf->m_astrUrlUnFilter.GetAt(i));
	}
	//
	SetUrlGroupEnable(m_bUseUrlFilter);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUrlFilterProp::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	int n,i;
	CString url;
	//
	pmf->m_astrUrlFilter.RemoveAll();
	n = m_cURLList.GetItemCount();
	for( i=0; i<n; i++)
	{
		url = pmf->GetStandUrl(m_cURLList.GetItemText(i,0));
		pmf->m_astrUrlFilter.Add(url);
	}
	//
	pmf->m_astrUrlUnFilter.RemoveAll();
	n = m_cURLUnList.GetItemCount();
	for( i=0; i<n; i++)
	{
		url = pmf->GetStandUrl(m_cURLUnList.GetItemText(i,0));
		pmf->m_astrUrlUnFilter.Add(url);
	}
	//
	pmf->m_bUrlFilterDirty = TRUE;
	pmf->InitialUrlFilter();
	pmf->SaveFilterList2File(1);

	CSAPrefsSubDlg::OnOK();
}

void CUrlFilterProp::OnDblclkUrlList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEdit();
	
	*pResult = 0;
}

void CUrlFilterProp::OnEndlabeleditUrlList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString str;
	m_cURLList.GetEditControl()->GetWindowText(str);

	m_cURLList.SetItemText(pDispInfo->item.iItem,0,str);
	
	*pResult = 0;
}

void CUrlFilterProp::OnUrlFilter() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd;
	int state; 
	pWnd = GetDlgItem(IDC_URL_FILTER);
	state = ((CButton*)pWnd)->GetCheck();
	//
	SetUrlGroupEnable(state);
}

void CUrlFilterProp::SetUrlGroupEnable(BOOL state)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_URL_LIST);
	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDC_INSERT);
	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDC_DELETE);
	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDC_EDIT);
	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDC_UN_STATIC);
	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDC_UN_URL_LIST);
	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDC_UN_INSERT);
	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDC_UN_DELETE);
	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDC_UN_EDIT);
	pWnd->EnableWindow(state);
}

void CUrlFilterProp::OnUnInsert() 
{
	// TODO: Add your control notification handler code here
	int i = m_cURLUnList.GetItemCount();
	m_cURLUnList.InsertItem(i, "http://");
	m_cURLUnList.SetFocus();
	m_cURLUnList.EnsureVisible(i,FALSE);
	m_cURLUnList.EditLabel(i);
}

void CUrlFilterProp::OnUnDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_cURLUnList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nItem = m_cURLUnList.GetNextSelectedItem(pos);
		m_cURLUnList.DeleteItem(nItem);
		m_cURLUnList.SetItemState(nItem, LVIS_SELECTED,LVIS_SELECTED);
	}	
}

void CUrlFilterProp::OnUnEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_cURLUnList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nItem = m_cURLUnList.GetNextSelectedItem(pos);
		m_cURLUnList.SetFocus();
		m_cURLUnList.EditLabel(nItem);
	}
}

void CUrlFilterProp::OnDblclkUnUrlList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CUrlFilterProp::OnEndlabeleditUnUrlList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString str;
	m_cURLUnList.GetEditControl()->GetWindowText(str);

	m_cURLUnList.SetItemText(pDispInfo->item.iItem,0,str);
	
	*pResult = 0;
}
