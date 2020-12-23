// PopupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "PopupDlg.h"
#include "MainFrm.h"
#include "HelperStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CPopupDlg dialog
IMPLEMENT_DYNCREATE(CPopupDlg, CSAPrefsSubDlg)

CPopupDlg::CPopupDlg() : CSAPrefsSubDlg(CPopupDlg::IDD)
{
	//{{AFX_DATA_INIT(CPopupDlg)
	m_bKillAutoWin = FALSE;
	m_bKillDupWin = FALSE;
	m_bKillListWin = FALSE;
	m_bKillPop = FALSE;
	m_bNotConfirmPopupFilter = FALSE;
	m_bPlayPopupFilterSound = FALSE;
	m_nPopDelay = 0;
	m_bFilterRefresh = FALSE;
	m_bNotFilterClickDialog = FALSE;
	//}}AFX_DATA_INIT
}


void CPopupDlg::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPopupDlg)
	DDX_Control(pDX, IDD_UN_POPUPLIST, m_UnPopupList);
	DDX_Control(pDX, IDD_POPUPLIST, m_PopupList);
	DDX_Check(pDX, IDC_KILL_AUTO_WIN, m_bKillAutoWin);
	DDX_Check(pDX, IDC_KILL_DUP_WIN, m_bKillDupWin);
	DDX_Check(pDX, IDC_KILL_LIST_WIN, m_bKillListWin);
	DDX_Check(pDX, IDC_ENABLE_POP_WIN, m_bKillPop);
	DDX_Check(pDX, IDC_NOT_CONFIRM_POPUP, m_bNotConfirmPopupFilter);
	DDX_Check(pDX, IDC_PLAY_POPUP_SOUND, m_bPlayPopupFilterSound);
	DDX_Slider(pDX, IDC_POP_DELAY, m_nPopDelay);
	DDX_Check(pDX, IDC_FILTER_REFRESH, m_bFilterRefresh);
	DDX_Check(pDX, IDC_NOT_FILTER_CLICK_DIALOG, m_bNotFilterClickDialog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPopupDlg, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CPopupDlg)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDD_POPUPLIST, OnEndlabeleditPopuplist)
	ON_NOTIFY(NM_DBLCLK, IDD_POPUPLIST, OnDblclkPopuplist)
	ON_BN_CLICKED(IDC_ENABLE_POP_WIN, OnEnablePopWin)
	ON_BN_CLICKED(IDC_UN_INSERT, OnUnInsert)
	ON_BN_CLICKED(IDC_UN_DELETE, OnUnDelete)
	ON_BN_CLICKED(IDC_UN_EDIT, OnUnEdit)
	ON_NOTIFY(NM_DBLCLK, IDD_UN_POPUPLIST, OnDblclkUnPopuplist)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDD_UN_POPUPLIST, OnEndlabeleditUnPopuplist)
	ON_BN_CLICKED(IDC_KILL_LIST_WIN, OnKillListWin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopupDlg message handlers

BOOL CPopupDlg::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogPopupFilter");
#endif
	LOADDLG("DialogPopupFilter");
	CSAPrefsSubDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	try{
	POSITION pos;
	int i;
	CRect rect;
	//
	pos = pmf->m_astrPopup.GetHeadPosition();
	i=0;
	m_PopupList.InsertColumn(0,"");
	m_PopupList.GetWindowRect(rect);
	m_PopupList.SetColumnWidth(0, rect.Width()-25);
	while(pos!=NULL)
	{
		m_PopupList.InsertItem(i, pmf->m_astrPopup.GetAt(pos));
		pmf->m_astrPopup.GetNext(pos);
		i++;
	}
	//un filter
	pos = pmf->m_astrUnPopup.GetHeadPosition();
	i=0;
	m_UnPopupList.InsertColumn(0,"");
	m_UnPopupList.GetWindowRect(rect);
	m_UnPopupList.SetColumnWidth(0, rect.Width()-25);
	while(pos!=NULL)
	{
		m_UnPopupList.InsertItem(i, pmf->m_astrUnPopup.GetAt(pos));
		pmf->m_astrUnPopup.GetNext(pos);
		i++;
	}
	//
	SetPopGroupEnable(m_bKillPop);
	//set pop delay
	CSliderCtrl *pWnd = (CSliderCtrl*)GetDlgItem(IDC_POP_DELAY);
	pWnd->SetRange(80,160,TRUE);
	pWnd->SetLineSize(20);
	pWnd->SetPageSize(20);
	pWnd->SetTicFreq(20);
	pWnd->SetPos(m_nPopDelay);
	//
	}catch(...){}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPopupDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString url;
	int i;
	int n ;

	pmf->m_astrPopup.RemoveAll();
	n = m_PopupList.GetItemCount();
	for( i=0; i<n; i++)
	{
		url = pmf->GetStandUrl(m_PopupList.GetItemText(i,0));
		pmf->m_astrPopup.AddTail(url);
	}
	//
	pmf->m_astrUnPopup.RemoveAll();
	n = m_UnPopupList.GetItemCount();
	for( i=0; i<n; i++)
	{
		url = pmf->GetStandUrl(m_UnPopupList.GetItemText(i,0));
		pmf->m_astrUnPopup.AddTail(url);
	}
	pmf->SaveFilterList2File(0);

	CSAPrefsSubDlg::OnOK();
}

void CPopupDlg::OnDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_PopupList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nItem = m_PopupList.GetNextSelectedItem(pos);
		m_PopupList.DeleteItem(nItem);
		m_PopupList.SetItemState(nItem, LVIS_SELECTED,LVIS_SELECTED);
	}
}

void CPopupDlg::OnEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_PopupList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nItem = m_PopupList.GetNextSelectedItem(pos);
		m_PopupList.SetFocus();
		m_PopupList.EditLabel(nItem);
	}
}

void CPopupDlg::OnInsert() 
{
	// TODO: Add your control notification handler code here
	int i = m_PopupList.GetItemCount();
	m_PopupList.InsertItem(i, "http://");
	m_PopupList.SetFocus();
	m_PopupList.EnsureVisible(i,FALSE);
	m_PopupList.EditLabel(i);
}

void CPopupDlg::OnEndlabeleditPopuplist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString str;
	m_PopupList.GetEditControl()->GetWindowText(str);

	m_PopupList.SetItemText(pDispInfo->item.iItem,0,str);
	*pResult = 0;
}

void CPopupDlg::OnDblclkPopuplist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEdit();
	
	*pResult = 0;
}

//#pragma optimize( "s", off)

void CPopupDlg::OnEnablePopWin() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd;
	int state;
	pWnd = GetDlgItem(IDC_ENABLE_POP_WIN);
	state = ((CButton*)pWnd)->GetCheck();
	//
	SetPopGroupEnable(state);
}

void CPopupDlg::OnUnInsert() 
{
	// TODO: Add your control notification handler code here
	int i = m_UnPopupList.GetItemCount();
	m_UnPopupList.InsertItem(i, "http://bb.aaaa.com");
	m_UnPopupList.SetFocus();
	m_UnPopupList.EnsureVisible(i,FALSE);
	m_UnPopupList.EditLabel(i);	
}

void CPopupDlg::OnUnDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_UnPopupList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nItem = m_UnPopupList.GetNextSelectedItem(pos);
		m_UnPopupList.DeleteItem(nItem);
		m_UnPopupList.SetItemState(nItem, LVIS_SELECTED,LVIS_SELECTED);
	}	
}

void CPopupDlg::OnUnEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_UnPopupList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nItem = m_UnPopupList.GetNextSelectedItem(pos);
		m_UnPopupList.SetFocus();
		m_UnPopupList.EditLabel(nItem);
	}
}

void CPopupDlg::OnDblclkUnPopuplist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnUnEdit();

	*pResult = 0;
}

void CPopupDlg::OnEndlabeleditUnPopuplist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString str;
	m_UnPopupList.GetEditControl()->GetWindowText(str);

	str = CHelperStr::GetTopUrl(str); //gfpop
	str.MakeLower();

	m_UnPopupList.SetItemText(pDispInfo->item.iItem,0,str);
	*pResult = 0;
}

void CPopupDlg::OnKillListWin() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd;
	int state; 
	pWnd = GetDlgItem(IDC_KILL_LIST_WIN);
	state = ((CButton*)pWnd)->GetCheck();
	//
	SetListGroupEnable(state);
}

void CPopupDlg::SetPopGroupEnable(BOOL state)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_KILL_AUTO_WIN);
	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDC_KILL_DUP_WIN);
	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDC_KILL_LIST_WIN);
	pWnd->EnableWindow(state);
	//
	SetListGroupEnable(m_bKillListWin);
}

void CPopupDlg::SetListGroupEnable(BOOL state)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(IDD_POPUPLIST);
	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDC_INSERT);
	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDC_DELETE);
	pWnd->EnableWindow(state);
	pWnd = GetDlgItem(IDC_EDIT);
	pWnd->EnableWindow(state);

	//gfpop	del
//	pWnd = GetDlgItem(IDC_UN_STATIC);
//	pWnd->EnableWindow(state);
//  pWnd = GetDlgItem(IDD_UN_POPUPLIST);
// 	pWnd->EnableWindow(state);
//	pWnd = GetDlgItem(IDC_UN_INSERT);
//	pWnd->EnableWindow(state);
//	pWnd = GetDlgItem(IDC_UN_DELETE);
//	pWnd->EnableWindow(state);
//	pWnd = GetDlgItem(IDC_UN_EDIT);
// 	pWnd->EnableWindow(state);
}
