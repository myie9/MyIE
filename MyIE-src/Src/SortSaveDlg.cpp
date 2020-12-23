// SortSaveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "SortSaveDlg.h"
#include "inputdlg.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSortSaveDlg dialog


CSortSaveDlg::CSortSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSortSaveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSortSaveDlg)
	m_strFileName = _T("");
	m_bType = FALSE;
	//}}AFX_DATA_INIT
	m_bUrlReplaced = FALSE;
}


void CSortSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSortSaveDlg)
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_FILENAME, m_strFileName);
	DDX_Check(pDX, IDC_TYPE, m_bType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSortSaveDlg, CDialog)
	//{{AFX_MSG_MAP(CSortSaveDlg)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnClickList1)
	ON_BN_CLICKED(IDC_NOT_SORT, OnNotSort)
	ON_BN_CLICKED(IDC_NEW, OnNew)
	ON_BN_CLICKED(IDC_OPEN, OnOpen)
	ON_BN_CLICKED(IDC_RENAME, OnRename)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_BN_CLICKED(IDC_DATE_SAVE, OnDateSave)
	ON_BN_CLICKED(IDC_TYPE, OnType)
	ON_BN_CLICKED(IDC_SHOW_SORT_DLG, OnShowSortDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSortSaveDlg message handlers

BOOL CSortSaveDlg::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogSortSave");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogSortSave");
	CDialog::OnInitDialog();
	
	CRect rect;
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_list.InsertColumn(0,"");
	m_list.GetWindowRect(rect);
	m_list.SetColumnWidth(0, rect.Width()-5);

	try{
	static char cur[MAX_PATH];
	static char tmp[5]; 
	//load bmp names
	CString strTemp = ((CMyIE9App*)AfxGetApp())->m_strDefaultDirSort;
	cur[0]=0;
	GetCurrentDirectory(MAX_PATH, cur);
	if(!SetCurrentDirectory(strTemp))
		CreateDirectory(strTemp, NULL);
	else
		SetCurrentDirectory(cur);

	WIN32_FIND_DATA wfd;
	HANDLE          h;
	strTemp += "*.*";

	int i=0,selected=0;//,iIcon;
	h = FindFirstFile(strTemp, &wfd);
	if(h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
			{
				if(lstrcmp(wfd.cFileName, _T(".")) == 0 || lstrcmp(wfd.cFileName, _T("..")) == 0)
					continue;
				m_list.InsertItem(i,wfd.cFileName);
				if(lstrcmp(wfd.cFileName, m_strSortSaveFolder) == 0)
					selected = i;
				//
				i++;
			}
		}
		while(FindNextFile(h, &wfd) && i<511);
	}
	FindClose(h);
	LIST_FOCUS_POS(m_list,selected);
	//
	CButton *p = (CButton*)GetDlgItem(IDC_SHOW_SORT_DLG);
	p->SetCheck(pmf->m_bShowSortSaveDlg);

	}catch(...){}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSortSaveDlg::OnOK() 
{
	// TODO: Add extra validation here
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int current = m_list.GetNextSelectedItem(pos);
	if (current<0)
		return;
	m_strSortSaveFolder = m_list.GetItemText(current,0);
	CDialog::OnOK();
}

void CSortSaveDlg::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if(pos!=NULL)
		OnOK();

	*pResult = 0;
}

void CSortSaveDlg::OnNotSort() 
{
	// TODO: Add your control notification handler code here
	m_strSortSaveFolder.Empty();
	CDialog::OnOK();
}
void CSortSaveDlg::NewDir(CString strNew)
{
	CString strItem;
	int count = m_list.GetItemCount();
	for (int i=0;i<count;i++)
	{
		strItem = m_list.GetItemText(i,0);
		strItem.MakeLower();
		strNew.MakeLower();
		if (strItem==strNew)
			break;
	}
	if (i==count)
	{
		m_list.InsertItem(i, strNew);
		strNew = theApp.m_strDefaultDirSort + strNew;
		_CreateFullDir(strNew);
	}
	LIST_FOCUS_POS(m_list,i);
}
void CSortSaveDlg::OnNew() 
{
	// TODO: Add your control notification handler code here
	CInputDlg dlg;
	CString str;
	LOADSTR(str ,IDS_NEW_DIR);
	dlg.m_strMsg = str;
	if (dlg.DoModal()==IDOK)
	{
		CString strName;
		strName = dlg.m_strOutput;
		if (strName.GetLength())
		{
			REPLACE_ERR_PATH_CHAR(strName)
			if (strName.GetLength()<1)
				return;
			NewDir(strName);
		}
	}
}

void CSortSaveDlg::OnOpen() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int current = m_list.GetNextSelectedItem(pos);
	CString strName;
	if (current>=0)
		strName = m_list.GetItemText(current,0);
	//
	CString strTemp = ((CMyIE9App*)AfxGetApp())->m_strDefaultDirSort;
	strTemp += strName;
	ShellExecute(NULL, "open", strTemp , "", NULL, SW_SHOW );
}

void CSortSaveDlg::OnRename() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int current = m_list.GetNextSelectedItem(pos);
	CString strNameOld;
	if (current>=0)
		strNameOld = m_list.GetItemText(current,0);
	//
	CInputDlg dlg;
	CString str;
	LOADSTR(str ,IDS_NEW_DIR);
	dlg.m_strMsg = str;
	dlg.m_strOutput = strNameOld;
	if (dlg.DoModal()==IDOK)
	{
		CString strName;
		strName = dlg.m_strOutput;
		if (strName.GetLength())
		{
			REPLACE_ERR_PATH_CHAR(strName)
			if (strName.GetLength()<1)
				return;
			m_list.SetItemText(current,0,strName);
			//		
			CString strOldPath = ((CMyIE9App*)AfxGetApp())->m_strDefaultDirSort + strNameOld;
			CString strNewPath = ((CMyIE9App*)AfxGetApp())->m_strDefaultDirSort + strName;
			rename(strOldPath, strNewPath);
		}
	}	
}

void CSortSaveDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if(pos!=NULL)
		OnOK();

	*pResult = 0;
}

void CSortSaveDlg::OnDateSave() 
{
	// TODO: Add your control notification handler code here
	CString strTime;
	SYSTEMTIME time;
	::GetLocalTime(&time);
	strTime.Format("%04d_%02d_%02d",time.wYear,time.wMonth,time.wDay);
	NewDir(strTime);
	OnOK();
}

void CSortSaveDlg::GetUrl() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString filename = m_strFileName;
	CString ext;
	int l = filename.ReverseFind('.');
	if(l>0)
	{
		ext = filename.Right(filename.GetLength()-l);
		filename = filename.Left(l);
	}
	//
	if (!m_bUrlReplaced)
	{
		m_strUrl.Replace('/', '\\');
		if(m_strUrl.Right(1) == "\\")
			m_strUrl = m_strUrl.Left(m_strUrl.GetLength()-1);

		int l = m_strUrl.ReverseFind('\\');
		m_strUrl = m_strUrl.Mid(l+1);
		m_strUrl.Replace(':', '_');
		l = m_strUrl.Find('?');
		if(l>0)
			m_strUrl = m_strUrl.Left(l);
		l = m_strUrl.ReverseFind('.');
		if(l>0)
			m_strUrl = m_strUrl.Left(l);
		//
		m_bUrlReplaced = TRUE;
	}
	m_strFileName = m_strUrl + ext;
	UpdateData(FALSE);
}

void CSortSaveDlg::GetTitle() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString filename = m_strFileName;
	CString ext;
	int l = filename.ReverseFind('.');
	if(l>0)
	{
		ext = filename.Right(filename.GetLength()-l);
		filename = filename.Left(l);
	}
	if (IS_URL(m_strTitle))
		m_strFileName = m_strTitle ;
	else
		m_strFileName = m_strTitle + ext;
	UpdateData(FALSE);
}

void CSortSaveDlg::OnType() 
{
	// TODO: Add your control notification handler code here
	if (m_bType)
		GetUrl();
	else
		GetTitle();
}

void CSortSaveDlg::OnShowSortDlg() 
{
	// TODO: Add your control notification handler code here
	pmf->m_bShowSortSaveDlg = !pmf->m_bShowSortSaveDlg;
}
