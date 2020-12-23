// SelectLang.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "SelectLang.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectLang dialog


CSelectLang::CSelectLang(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectLang::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectLang)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelectLang::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectLang)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectLang, CDialog)
	//{{AFX_MSG_MAP(CSelectLang)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectLang message handlers

BOOL CSelectLang::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogSelectLang");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogSelectLang");
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_list.SetExtendedStyle(m_list.GetExtendedStyle()| LVS_EX_FULLROWSELECT);
	CRect rect;
	CString strName;
	LOADSTR(strName,IDS_FILENAME);
	m_list.InsertColumn(0,strName);
	m_list.SetColumnWidth(0, 110);
	LOADSTR(strName,IDS_LANGUAGE);
	m_list.InsertColumn(1,strName);
	m_list.GetWindowRect(rect);
	m_list.SetColumnWidth(1, 100);

	//gf
//	LOADSTR(strName,IDS_INTERNAL_LANG);
//	m_list.InsertItem(0, strName);

	//
	if (theApp.m_bUseLngFile)
	{
		strName = theApp.m_strLngFile;
		strName = strName.Mid(strName.ReverseFind('\\')+1);
	}
	int nSelect=0;
	char buf[256];
	//
	CString psz = theApp.m_strRoot+"Language\\*.ini";
	WIN32_FIND_DATA wfd;
	HANDLE          h;
	h = FindFirstFile(psz, &wfd);
	if(h != INVALID_HANDLE_VALUE)
	{
		int i=0;//gf i=1;
		do{
			m_list.InsertItem(i,wfd.cFileName);
			if (theApp.m_bUseLngFile)
			{
				if (strName == wfd.cFileName)
					nSelect = i;
			}
			if (::GetPrivateProfileString("Information", "Language", NULL, buf, 256, ((CMyIE9App*)AfxGetApp())->m_strRoot+"Language\\"+wfd.cFileName))
				m_list.SetItemText(i,1,buf);
			i++;
		}
		while(FindNextFile(h, &wfd) && i<255);
	} 

	FindClose(h);

	LIST_FOCUS_POS(m_list,nSelect);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectLang::OnOK() 
{
	// TODO: Add extra validation here
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int current = m_list.GetNextSelectedItem(pos);
/* //gf	if (current==0)
	{
		theApp.m_bUseLngFile = FALSE;
		theApp.m_strLngFile = "";
	}
	else if (current>0)*/
	if (current>-1) //gf -1
	{
		theApp.m_bUseLngFile = TRUE;
		theApp.m_strLngFile = theApp.m_strRoot+"Language\\" + m_list.GetItemText(current,0);
	}
	
	if (current>=0)
	{
		_InitLanguageFile(theApp.m_strLngFile);
		theApp.WriteProfileInt("Settings", "UseLngFile", theApp.m_bUseLngFile );
		theApp.WriteProfileString("Settings", "LngFile", m_list.GetItemText(current,0) );
	}
	CDialog::OnOK();
}

void CSelectLang::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if(pos!=NULL)
		OnOK();

	*pResult = 0;
}
