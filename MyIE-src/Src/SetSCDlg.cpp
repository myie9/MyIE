// SetSCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "SetSCDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CSetSCDlg dialog
typedef HRESULT (CALLBACK* LPFNDLLFUNC1)(HWND ,DWORD);


CSetSCDlg::CSetSCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetSCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetSCDlg)
	m_strEdit = _T("");
	m_bUseIeKey = FALSE;
	//}}AFX_DATA_INIT
}


void CSetSCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetSCDlg)
	DDX_Control(pDX, IDC_KEYLIST, m_keylist);
	DDX_Control(pDX, IDC_EDIT1, m_url);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_strEdit);
	DDX_Check(pDX, IDC_CHECK1, m_bUseIeKey);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetSCDlg, CDialog)
	//{{AFX_MSG_MAP(CSetSCDlg)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_CBN_SELCHANGE(IDC_KEYLIST, OnSelchangeKeylist)
	ON_BN_CLICKED(IDC_APPEND, OnAppend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetSCDlg message handlers

BOOL CSetSCDlg::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogKey");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogKey");
	CDialog::OnInitDialog();
	
	m_list.SetExtendedStyle(m_list.GetExtendedStyle()| LVS_EX_FULLROWSELECT);
	// TODO: Add extra initialization here
	CRect rect;
	CString str;
	LOADSTR(str ,IDS_KEY);
	m_list.InsertColumn(0,str);
	m_list.GetWindowRect(rect);
	m_list.SetColumnWidth(0, 40);
	LOADSTR(str ,IDS_URL);
	m_list.InsertColumn(1,str);
	m_list.SetColumnWidth(1, rect.Width()-58);

	char name[5] = "F";
	for(int i=0;i<11; i++)
	{
		itoa(i+2, name+1,10);
		m_list.InsertItem(i, name);
		m_list.SetItemText(i, 1, pmf->m_strShortCuts[i]);
	}
	m_bUseIeKey = pmf->m_bUseIeKey;
	UpdateData(FALSE);
	//auto complete
	HINSTANCE hIns = LoadLibrary("shlwapi.dll");
	if(hIns != NULL)
	{
		LPFNDLLFUNC1 lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hIns, "SHAutoComplete");
		if(lpfnDllFunc1!=NULL)
			lpfnDllFunc1(m_url.m_hWnd, 0xe);
		FreeLibrary(hIns);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetSCDlg::OnOK() 
{
	// TODO: Add extra validation here
	CWinApp* app = AfxGetApp();
	char key[9]="F";
	for( int i=0; i<11; i++)
	{
		pmf->m_strShortCuts[i]=m_list.GetItemText(i,1);
		itoa(i+2, key+1, 10);
		app->WriteProfileString("Shortcuts", key, pmf->m_strShortCuts[i]);
	}
	UpdateData();
	SAVEINT(m_bUseIeKey, pmf->m_bUseIeKey,"Shortcuts","UseIeKey");
	CDialog::OnOK();
}

void CSetSCDlg::OnUpdate() 
{
	// TODO: Add your control notification handler code here
	int i = m_keylist.GetCurSel();
	if(i<0)
		return;
	CString url;
	m_url.GetWindowText(url);
	m_list.SetItemText(i, 1, url);
}

void CSetSCDlg::OnAppend() 
{
	// TODO: Add your control notification handler code here
	int i = m_keylist.GetCurSel();
	if(i<0)
		return;
	CString url;
	m_url.GetWindowText(url);
	CString strOld;
	CString strNew;
	strOld = m_list.GetItemText(i,1);
	if (strOld.GetLength())
	{
		if (strOld.Find(url)>=0)
			return;
		strNew = strOld + " | ";
		strNew += url;
	}
	else
		strNew = url;
	m_list.SetItemText(i, 1, strNew);
}

//#pragma optimize( "s", off )

void CSetSCDlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	int i=-1, nIndex = pNMListView->iItem;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		i = m_list.GetNextSelectedItem(pos);
	}
	if(nIndex==i)
	{
		m_keylist.SetCurSel(nIndex);
		m_url.SetWindowText(m_list.GetItemText(nIndex,1));
	}
	else
	{
		m_keylist.SetCurSel(-1);
		m_url.SetWindowText("");
	}

	*pResult = 0;
}

void CSetSCDlg::OnSelchangeKeylist() 
{
	// TODO: Add your control notification handler code here
	CString url;
	m_url.GetWindowText(url);
	if(url.IsEmpty())
	{
		int i;
		i = m_keylist.GetCurSel();
		if(i>=0)
		{
			url = m_list.GetItemText(i, 1);
			m_url.SetWindowText(url);
		}
	}
}
