// ExternalUtils.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "ExternalUtils.h"
#include "MainFrm.h"
//#include "InputUtil.h"
#include "PathHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on)

/////////////////////////////////////////////////////////////////////////////
// CExternalUtils dialog


CExternalUtils::CExternalUtils(CWnd* pParent /*=NULL*/)
	: CDialog(CExternalUtils::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExternalUtils)
	//}}AFX_DATA_INIT
}


void CExternalUtils::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExternalUtils)
	DDX_Control(pDX, IDC_PATH, m_cPath);
	DDX_Control(pDX, IDC_PARAM, m_cPara);
	DDX_Control(pDX, IDC_AUTO_RUN, m_cRun);
	DDX_Control(pDX, IDC_STARTUP, m_cStartup);
	DDX_Control(pDX, IDC_CLOSEUTIL, m_cClose);
	DDX_Control(pDX, IDC_NAME, m_cName);
	DDX_Control(pDX, IDC_COMMAND, m_cCommand);
	DDX_Control(pDX, IDC_EXTERN_UTIL, m_UtilList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExternalUtils, CDialog)
	//{{AFX_MSG_MAP(CExternalUtils)
	ON_BN_CLICKED(IDC_ADD_UTIL, OnAddUtil)
	ON_BN_CLICKED(IDC_MOD_UTIL, OnModUtil)
	ON_BN_CLICKED(IDC_DEL_UTIL, OnDelUtil)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_EXTERN_UTIL, OnEndlabeleditExternUtil)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_EXTERN_UTIL, OnItemchangedExternUtil)
	ON_NOTIFY(NM_DBLCLK, IDC_EXTERN_UTIL, OnDblclkExternUtil)
	ON_BN_CLICKED(IDC_BROWSER_FILE, OnBrowserFile)
	ON_BN_CLICKED(IDC_IMPORT_PLUG_IN, OnImportPlugIn)
	ON_BN_CLICKED(IDC_BROWSER_PATH, OnBrowserPath)
	ON_BN_CLICKED(IDC_CLEAN, OnClean)
	ON_BN_CLICKED(IDC_AUTO_RUN, OnAutoRun)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExternalUtils message handlers

BOOL CExternalUtils::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogExternal");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogExternal");
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_UtilList.SetExtendedStyle(m_UtilList.GetExtendedStyle()| LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);

	int i=0;
	CRect rect;
	CString str;
	LOADSTR(str ,IDS_NAME);
	m_UtilList.InsertColumn(0,str);
	m_UtilList.GetWindowRect(rect);
	m_UtilList.SetColumnWidth(0, 150);
	LOADSTR(str ,IDS_COMMAND);
	m_UtilList.InsertColumn(1,str);
	m_UtilList.SetColumnWidth(1, rect.Width()-180);

	m_ExternalList.Clone(&pmf->m_ExternalUtilList);

	m_bSkip = TRUE;
	CExternalUtilItem* item;
	for (i=0;i <= m_ExternalList.m_UtilList.GetUpperBound();i++)
	{
		item = m_ExternalList.m_UtilList.GetAt(i);
		m_UtilList.InsertItem(i, item->m_strUtilName);
		m_UtilList.SetItemText(i, 1, item->m_strUtilCmd);
		m_UtilList.SetCheck(i,item->m_bUtilRun);
	}

	m_bSkip = FALSE;

	m_bAutoRunExternal = pmf->m_bAutoRunExternal;
	m_cRun.SetCheck( pmf->m_bAutoRunExternal); //gf

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExternalUtils::OnOK() 
{
	//gf
	pmf->m_bAutoRunExternal = m_bAutoRunExternal;

	// TODO: Add extra validation here
	OnModUtil();
	//
	int n = m_UtilList.GetItemCount();
	CExternalUtilItem* item;
	for( int i=0; i<n; i++)
	{
		item = m_ExternalList.m_UtilList.GetAt(i);
		item->m_bUtilRun = m_UtilList.GetCheck(i);
	}
	//
	pmf->m_ExternalUtilList.Clone(&m_ExternalList);
	
	CDialog::OnOK();
}

void CExternalUtils::OnAddUtil() 
{
	// TODO: Add your control notification handler code here
	CString Name;
	m_cName.GetWindowText(Name);
	CString Command;
	m_cCommand.GetWindowText(Command);

	CString Para;
	m_cPara.GetWindowText(Para);
	CString Path;
	m_cPath.GetWindowText(Path);

	if(!Command.IsEmpty() || Name.Left(2)=="--")
	{
		if(Name.IsEmpty())
			Name = Command;

		m_bSkip = TRUE;

		int i = m_UtilList.GetItemCount();//!!must do like this
		i = m_UtilList.InsertItem(i, Name);
		m_UtilList.SetItemText(i, 1, Command);
		//gf m_UtilList.SetCheck(i,m_cRun.GetCheck());
		m_UtilList.SetCheck(i, FALSE);
		LIST_FOCUS_POS (m_UtilList, i);

		CExternalUtilItem* eui = new CExternalUtilItem;
		eui->m_strUtilName = Name;
		eui->m_strUtilCmd = Command;
		eui->m_strUtilPara = Para;
		eui->m_strUtilPath = Path;

		eui->m_bUtilStart = m_cStartup.GetCheck();
		eui->m_bUtilClose = m_cClose.GetCheck();
		eui->m_bUtilRun = m_cRun.GetCheck();
		eui->m_iProcess = 0;
		m_ExternalList.m_UtilList.Add(eui);
		m_bSkip = FALSE;

		OnClean();
	}
}

void CExternalUtils::OnModUtil() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_UtilList.GetFirstSelectedItemPosition();
	int nItem;
	if(pos!=NULL)
	{
		nItem = m_UtilList.GetNextSelectedItem(pos);
		m_bSkip = TRUE;
		CString Name;
		m_cName.GetWindowText(Name);
		CString Command;
		m_cCommand.GetWindowText(Command);
		//
		CString Para;
		m_cPara.GetWindowText(Para);
		CString Path;
		m_cPath.GetWindowText(Path);
		//
		if(!Command.IsEmpty() || Name.Left(2)=="--")
		{
			if(Name.IsEmpty())
				Name = Command;
	
			m_UtilList.SetItemText(nItem, 0, Name);
			m_UtilList.SetItemText(nItem, 1, Command);
			//gf m_UtilList.SetCheck(nItem,m_cRun.GetCheck());

			CExternalUtilItem* eui = m_ExternalList.m_UtilList.GetAt(nItem);
			eui->m_strUtilName = Name;
			eui->m_strUtilCmd = Command;
			eui->m_strUtilPara = Para;
			eui->m_strUtilPath = Path;
			//
			eui->m_bUtilStart = m_cStartup.GetCheck();
			eui->m_bUtilClose = m_cClose.GetCheck();
			//gf eui->m_bUtilRun = m_cRun.GetCheck();
		}
		m_bSkip = FALSE;
	}
	else
		return;
	
}

void CExternalUtils::OnDelUtil() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_UtilList.GetFirstSelectedItemPosition();
	int nItem;
	if(pos!=NULL)
	{
		nItem = m_UtilList.GetNextSelectedItem(pos);
		m_UtilList.DeleteItem(nItem);
		delete m_ExternalList.m_UtilList.GetAt(nItem);
		m_ExternalList.m_UtilList.RemoveAt(nItem);
		//
		LIST_FOCUS_POS(m_UtilList, nItem);
	}
	else
		return;
}

void CExternalUtils::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	POSITION pos = m_UtilList.GetFirstSelectedItemPosition();
	int nItem = -1, newItem;
	if(pos!=NULL)
	{
		nItem = m_UtilList.GetNextSelectedItem(pos);
		if(pNMUpDown->iDelta<0)
			newItem = nItem -1;
		else
			newItem = nItem + 1;
		int n = m_UtilList.GetItemCount();
		if(newItem>=0 && newItem<n)
		{
			CString str = m_UtilList.GetItemText(nItem,0);
			CString str1 = m_UtilList.GetItemText(nItem,1);
			BOOL bTemp = m_UtilList.GetCheck(nItem);
			m_UtilList.DeleteItem(nItem);

			m_UtilList.InsertItem(newItem, str);
			m_UtilList.SetItemText(newItem, 1, str1);
			m_UtilList.SetCheck(newItem, bTemp);
			//
			LIST_FOCUS_POS(m_UtilList,newItem);

			CExternalUtilItem* eui = m_ExternalList.m_UtilList.GetAt(nItem);
			m_ExternalList.m_UtilList.SetAt(nItem, m_ExternalList.m_UtilList.GetAt(newItem));
			m_ExternalList.m_UtilList.SetAt(newItem, eui);
		}
	}	
	*pResult = 0;
}

void CExternalUtils::OnEndlabeleditExternUtil(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString str;
	m_UtilList.GetEditControl()->GetWindowText(str);
	str.TrimLeft();
	str.TrimRight();
	m_UtilList.SetItemText(pDispInfo->item.iItem,0,str);
	
	m_ExternalList.m_UtilList.GetAt(pDispInfo->item.iItem)->m_strUtilName = str;

	*pResult = 0;
}

void CExternalUtils::OnItemchangedExternUtil(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	if(!m_bSkip)
	{
		m_bSkip = FALSE;

		int i, nIndex = pNMListView->iItem;
		POSITION pos = m_UtilList.GetFirstSelectedItemPosition();
		if(pos!=NULL)
		{
			i = m_UtilList.GetNextSelectedItem(pos);
		}
		if(nIndex==i)
		{
			m_cName.SetWindowText(m_UtilList.GetItemText(nIndex,0));
			m_cCommand.SetWindowText(m_UtilList.GetItemText(nIndex,1));
			m_cPara.SetWindowText(m_ExternalList.m_UtilList.GetAt(pNMListView->iItem)->m_strUtilPara);
			m_cPath.SetWindowText(m_ExternalList.m_UtilList.GetAt(pNMListView->iItem)->m_strUtilPath);
			//
			m_cStartup.SetCheck(m_ExternalList.m_UtilList.GetAt(pNMListView->iItem)->m_bUtilStart);
			m_cClose.SetCheck(m_ExternalList.m_UtilList.GetAt(pNMListView->iItem)->m_bUtilClose);
			//
			m_cRun.SetCheck(m_UtilList.GetCheck(nIndex));
		}
		else
		{
			OnClean();
		}
	}

	*pResult = 0;
}

void CExternalUtils::InsertApp(CString strPath)
{
	try{

	if (_GetPlugInType(strPath))
	{
		CString strName = _GetPlugInName(strPath);
		m_cName.SetWindowText(strName);
	}
	else
	{
		int pos1 = strPath.ReverseFind('\\');
		int pos2 = strPath.ReverseFind('.');
		int count = pos2-pos1-1;
		if (count>0)
		{
			CString strName = strPath.Mid(pos1+1, count);
			m_cName.SetWindowText(strName);
		}
	}
	//
	//gf
	strPath.Replace(CPathHelper::GetExeDir(), ".\\");
	strPath.Replace("\\\\", "\\");

	strPath = "\""+strPath;
	strPath += "\"";
	m_cCommand.SetWindowText(strPath);

	}catch(...){}
}

void CExternalUtils::OnBrowserFile() 
{
	// TODO: Add your control notification handler code here
	CString str;
	LOADSTR(str ,IDS_TYPE_EXTOOL);
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str,this);
	if(dlg.DoModal()==IDOK)
	{
		CString str = dlg.GetPathName();
		if (str.GetLength())
			InsertApp(str);
	}
}

//#pragma optimize( "s", off)

void CExternalUtils::OnImportPlugIn() 
{

	SetCurrentDirectory(CPathHelper::GetExeDir() + _T("Plugin") );
	
	CString strPath;
	if (_BrowserPath(this,strPath) == FALSE)
		return;
	if(strPath.Right(1) != "\\")
		strPath += "\\";

	CString strRootPath = strPath;

	try{
		//gf
		{
			CString strJustTheDestIni = strRootPath + _T("plugin.ini");
			if (_FileIsExist(strJustTheDestIni) && _GetPlugInType(strJustTheDestIni))
			{
				InsertApp(strJustTheDestIni);
				OnAddUtil();			
			}
		}

	static char cur[MAX_PATH];
	cur[0]=0;
	GetCurrentDirectory(MAX_PATH, cur);
	if(!SetCurrentDirectory(strPath))
		CreateDirectory(strPath, NULL);
	else
		SetCurrentDirectory(cur);

	WIN32_FIND_DATA wfd;
	HANDLE          h;
	strPath += "*.*";

	CString strIniPath;
	int i=0;
	h = FindFirstFile(strPath, &wfd);
	if(h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
			{
				if(lstrcmp(wfd.cFileName, _T(".")) == 0 || lstrcmp(wfd.cFileName, _T("..")) == 0 || lstrcmpi(wfd.cFileName, _T("default")) == 0)
					continue;
				strIniPath = strRootPath + wfd.cFileName + "\\plugin.ini";
				if (_FileIsExist(strIniPath) && _GetPlugInType(strIniPath))
				{
					InsertApp(strIniPath);
					OnAddUtil();
					i++;
				}
			}
		}
		while(FindNextFile(h, &wfd) && i<250);
	}
	FindClose(h);

	}catch(...){}	
}

void CExternalUtils::OnBrowserPath() 
{
	// TODO: Add your control notification handler code here
	CString strPath;
	if (_BrowserPath(this,strPath) == FALSE)
		return;
	if(strPath.Right(1) != "\\")
		strPath += "\\";

	m_cPath.SetWindowText(strPath);
}

void CExternalUtils::OnClean()
{
	// TODO: Add your control notification handler code here
	m_cName.SetWindowText("");
	m_cCommand.SetWindowText("");
	m_cPara.SetWindowText("");
	m_cPath.SetWindowText("");
	m_cStartup.SetCheck(FALSE);
	m_cClose.SetCheck(FALSE);
	m_cRun.SetCheck(FALSE);
}

void CExternalUtils::OnAutoRun() 
{

	m_bAutoRunExternal = !m_bAutoRunExternal;
	//gf
//	// TODO: Add your control notification handler code here
//	POSITION pos = m_UtilList.GetFirstSelectedItemPosition();
//	if(pos==NULL)
//		return;
//	int nItem;
//	nItem = m_UtilList.GetNextSelectedItem(pos);
// 	m_UtilList.SetCheck(nItem, !m_UtilList.GetCheck(nItem));
}

void CExternalUtils::OnDblclkExternUtil(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_UtilList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nItem = m_UtilList.GetNextSelectedItem(pos);
		CString strCmd = m_UtilList.GetItemText(nItem,1);
		strCmd.Remove('"');
		strCmd = _StringGetUpUrl(strCmd);
		pmf->NewChildWindow(1,2,strCmd);
	}
	//
	*pResult = 0;
}
