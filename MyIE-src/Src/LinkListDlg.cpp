// LinkListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "LinkListDlg.h"

#include "ProxyList.h"
#include "mainfrm.h"
#include "InputDlg.h"
#include "Gf_Win.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLinkListDlg dialog
//#pragma optimize( "s", on)


CLinkListDlg::CLinkListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLinkListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLinkListDlg)
	//}}AFX_DATA_INIT
}


void CLinkListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLinkListDlg)
	DDX_Control(pDX, IDC_NAME, m_name);
	DDX_Control(pDX, IDC_URL, m_url);
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLinkListDlg, CDialog)
	//{{AFX_MSG_MAP(CLinkListDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_CBN_SELCHANGE(IDC_URL, OnSelChangeUrl)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST, OnRdblclkList)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_LL_SELECT_ALL, OnSelectAll)
	ON_COMMAND(ID_LL_SELECT_NONE, OnSelectNone)
	ON_COMMAND(ID_LL_SELECT_UP, OnSelectUp)
	ON_COMMAND(ID_LL_SELECT_DOWN, OnSelectDown)
	ON_COMMAND(ID_LL_SELECT_ODD, OnSelectOdd)
	ON_COMMAND(ID_LL_SELECT_EVEN, OnSelectEven)

	ON_COMMAND(ID_LL_OPEN_SELECT, OnOpenSelect)
	ON_COMMAND(ID_LL_CLOSE_SELECT, OnCloseSelect)
	ON_COMMAND(ID_LL_COPY_URL, OnCopyUrl)
	ON_COMMAND(ID_LL_EDIT_URL, OnEditUrl)

	ON_COMMAND(ID_LL_REFRESH, OnRefreshLinkList)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLinkListDlg message handlers

//#pragma optimize( "s", off)

BOOL CLinkListDlg::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogLinkList");
#endif
	LOADDLG("DialogLinkList");
	CDialog::OnInitDialog();
	//
	CImageList img;
	if (!m_ToolBar.CreateEx(this))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 10, 1);
	HBITMAP hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_LINKLIST));
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);
	m_ToolBar.GetToolBarCtrl().SetImageList(&img);
	img.Detach();

	m_ToolBar.SetButtons(NULL, 13);
	m_ToolBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT| TBSTYLE_TOOLTIPS);
	m_ToolBar.ModifyStyleEx(0, WS_EX_TOOLWINDOW);	
	m_ToolBar.SetBarStyle(m_ToolBar.GetBarStyle() |	CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);

	CString str;

	m_ToolBar.SetButtonInfo(0, ID_LL_SELECT_ALL, TBSTYLE_BUTTON, 0);
	LOADSTR(str ,ID_LL_SELECT_ALL);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_LL_SELECT_ALL);

	m_ToolBar.SetButtonInfo(1, ID_LL_SELECT_NONE, TBSTYLE_BUTTON, 1);
	LOADSTR(str ,ID_LL_SELECT_NONE);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_LL_SELECT_NONE);

	m_ToolBar.SetButtonInfo(2, ID_LL_SELECT_UP, TBSTYLE_BUTTON, 2);
	LOADSTR(str ,ID_LL_SELECT_UP);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_LL_SELECT_UP);

	m_ToolBar.SetButtonInfo(3, ID_LL_SELECT_DOWN, TBSTYLE_BUTTON, 3);
	LOADSTR(str ,ID_LL_SELECT_DOWN);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_LL_SELECT_DOWN);

	m_ToolBar.SetButtonInfo(4, ID_LL_SELECT_ODD, TBSTYLE_BUTTON, 4);
	LOADSTR(str ,ID_LL_SELECT_ODD);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_LL_SELECT_ODD);

	m_ToolBar.SetButtonInfo(5, ID_LL_SELECT_EVEN, TBSTYLE_BUTTON, 5);
	LOADSTR(str ,ID_LL_SELECT_EVEN);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_LL_SELECT_EVEN);
	m_ToolBar.SetButtonInfo(6, 0, TBBS_SEPARATOR, 0);

	m_ToolBar.SetButtonInfo(7, ID_LL_OPEN_SELECT, TBSTYLE_BUTTON, 6);
	LOADSTR(str ,ID_LL_OPEN_SELECT);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_LL_OPEN_SELECT);
	m_ToolBar.SetButtonInfo(8, ID_LL_CLOSE_SELECT, TBSTYLE_BUTTON, 7);
	LOADSTR(str ,ID_LL_CLOSE_SELECT);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_LL_CLOSE_SELECT);
	m_ToolBar.SetButtonInfo(9, ID_LL_COPY_URL, TBSTYLE_BUTTON, 8);
	LOADSTR(str ,ID_LL_COPY_URL);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_LL_COPY_URL);
	m_ToolBar.SetButtonInfo(10, ID_LL_EDIT_URL, TBSTYLE_BUTTON, 9);
	LOADSTR(str ,ID_LL_EDIT_URL);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_LL_EDIT_URL);
	m_ToolBar.SetButtonInfo(11, 0, TBBS_SEPARATOR, 0);

	m_ToolBar.SetButtonInfo(12, ID_LL_REFRESH, TBSTYLE_BUTTON, 10);
	LOADSTR(str ,ID_LL_REFRESH);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_LL_REFRESH);
	m_ToolBar.SetButtonInfo(13, 0, TBBS_SEPARATOR, 0);

	//m_ToolBar.GetToolBarCtrl().SetButtonSize(CSize(16,16));

	CRect rect;
	m_ToolBar.GetToolBarCtrl().GetItemRect(m_ToolBar.GetToolBarCtrl().GetButtonCount()-1,&rect);
	m_nToolbarWidth = rect.right;
	m_nToolbarHeight= rect.Height();

	m_ToolBar.SetWindowPos(NULL, 0,0, m_nToolbarWidth+2, 36,0);
	int x;
	x = m_nToolbarWidth+5;
	//y = (m_nToolbarHeight-16)/2;
	CWnd *p;
	p = GetDlgItem(IDC_STATIC_NAME);
	p->SetWindowPos(NULL, x, 5, 40, 14,0);
	x += 40;
	p = GetDlgItem(IDC_NAME);
	p->SetWindowPos(NULL, x, 1, 80, 20,0);

	x += 80;
	p = GetDlgItem(IDC_STATIC_URL);
	p->SetWindowPos(NULL, x, 5, 40, 14,0);
	x += 40;
	p = GetDlgItem(IDC_URL);
	p->SetWindowPos(NULL, x, 1, 80, 20,0);

	// TODO: Add extra initialization here
	m_list.SetExtendedStyle(m_list.GetExtendedStyle()| LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES);
	m_list.SetBkColor(GetSysColor(COLOR_WINDOW));
	m_list.GetWindowRect(&rect);
	LOADSTR(str ,IDS_TITLE);
	m_list.InsertColumn(0, str);
	m_list.SetColumnWidth(0, 150);
	LOADSTR(str ,IDS_URL);
	m_list.InsertColumn(1, str);
	m_list.SetColumnWidth(1, rect.Width()-180);
	//
	m_list.SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLinkListDlg::OnUpdate() 
{
	// TODO: Add your control notification handler code here
	int i,j=0,type;
	BOOL bNotFilter=FALSE;
	CLink *item;
	CString strType;
	CWnd* p = GetFocus();
	if (p->m_hWnd == m_name.m_hWnd)
	{
		m_name.GetWindowText(strType);
		type = 0;
	}
	else if (p->GetParent()->m_hWnd == m_url.m_hWnd)
	{
		m_url.GetWindowText(strType);
		type = 1;
	}
	else
		return;
	//
	if (strType.GetLength()==0)
		bNotFilter = TRUE;
	else
		bNotFilter = FALSE;
	m_list.DeleteAllItems();
	for(i=0;i<=pmf->m_LinkList.m_Links.GetUpperBound();i++)
	{
		item = pmf->m_LinkList.m_Links.GetAt(i);
		if( bNotFilter || 
			(type==0 && item->name.Find(strType)!=-1) ||
			(type==1 && item->url.Find(strType)!=-1)
		  )
		{
			m_list.InsertItem(j, item->name);
			m_list.SetItemText(j, 1, item->url);
			j++;
		}
	}
	m_list.AddSortIndexData();
	//
	UpdateTitleInfo();
}

void CLinkListDlg::FillLinkList()
{
	CLink *item;
	m_list.DeleteAllItems();
	for(int i=0;i<=pmf->m_LinkList.m_Links.GetUpperBound();i++)
	{
		item = pmf->m_LinkList.m_Links.GetAt(i);
		m_list.InsertItem(i, item->name);
		m_list.SetItemText(i, 1, item->url);
	}
	m_list.AddSortIndexData();
	UpdateTitleInfo();
}

void CLinkListDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int current = m_list.GetNextSelectedItem(pos);
	if (current<0)
		return;
	CString url = m_list.GetItemText(current,1);
	pmf->NewChildWindow(1, 2, url);

	*pResult = 0;
}

void CLinkListDlg::OnSelChangeUrl() 
{
	// TODO: Add your control notification handler code here
	CString str;
	m_url.GetLBText(m_url.GetCurSel(), str); 
	m_url.SetWindowText(str);
	OnUpdate();
}

void CLinkListDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	m_list.SetWindowPos(&CWnd::wndTop, 0,m_nToolbarHeight+1, cx,cy-(m_nToolbarHeight+1), 0);
	m_list.SetColumnWidth(1, cx-30-m_list.GetColumnWidth(0));
}

int CLinkListDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_LINKLIST)),FALSE);
	return 0;
}
void CLinkListDlg::OnSelectAll() 
{
	// TODO: Add your control notification handler code here
	int i,count = m_list.GetItemCount();
	for(i=0;i<count;i++)
		m_list.SetCheck(i,1);
}

void CLinkListDlg::OnSelectNone() 
{
	// TODO: Add your control notification handler code here
	int i,count = m_list.GetItemCount();
	for(i=0;i<count;i++)
		m_list.SetCheck(i,0);
}

void CLinkListDlg::OnSelectUp() 
{
	// TODO: Add your control notification handler code here
	int i,current;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	current = m_list.GetNextSelectedItem(pos);
	for(i=0;i<=current;i++)
		m_list.SetCheck(i,1);
}

void CLinkListDlg::OnSelectDown() 
{
	// TODO: Add your control notification handler code here
	int i,current,count = m_list.GetItemCount();
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	current = m_list.GetNextSelectedItem(pos);
	for(i=current; i<count; i++)
		m_list.SetCheck(i,1);
}
void CLinkListDlg::OnSelectOdd() 
{
	// TODO: Add your control notification handler code here
	int i,count = m_list.GetItemCount();
	for(i=0;i<count;i++)
	{
		if (i%2)
			m_list.SetCheck(i,0);
		else
			m_list.SetCheck(i,1);
	}
}

void CLinkListDlg::OnSelectEven() 
{
	// TODO: Add your control notification handler code here
	int i,count = m_list.GetItemCount();
	for(i=0;i<count;i++)
	{
		if (i%2)
			m_list.SetCheck(i,1);
		else
			m_list.SetCheck(i,0);
	}
}
void CLinkListDlg::OnOpenSelect() 
{
	// TODO: Add your control notification handler code here
	int i,count = m_list.GetItemCount();
	if (count<1)
		return;
	CString url;
	url.Empty();
	for (i=0;i<count;i++)
	{
		if (NULL == pmf)
		{
			return;
		}

		if (m_list.GetCheck(i))
		{
			url = m_list.GetItemText(i,1);
			//pmf->NewChildWindow(1, 2, url);
			pmf->NewChildWindowInBack(url);

			CGf_Win::Delay(4000);
		}
	}
	if (url.GetLength()<1)//if not check one, copy current item url
	{
		POSITION pos = m_list.GetFirstSelectedItemPosition();
		int current = m_list.GetNextSelectedItem(pos);
		if (current>=0)
		{
			url = m_list.GetItemText(current,1);
			pmf->NewChildWindow(1, 2, url);
		}
	}
}
void CLinkListDlg::OnCloseSelect() 
{
	// TODO: Add your control notification handler code here
	int i,count = m_list.GetItemCount();
	if (count<=0)
		return;
	for (i=count-1;i>=0;i--)
	{
		if (m_list.GetCheck(i))
			m_list.DeleteItem(i);
	}
	//if not check one
	if (count == m_list.GetItemCount())
	{
		POSITION pos = m_list.GetFirstSelectedItemPosition();
		int current = m_list.GetNextSelectedItem(pos);
		if (current>=0)
			m_list.DeleteItem(current);
		else
			return;
	}
	UpdateTitleInfo();
}
void CLinkListDlg::UpdateTitleInfo()
{
	char current[6],total[6];
	CString strState;
	LOADSTR(strState ,IDS_LINK_LIST_TITLE);
	itoa(m_list.GetItemCount(), current,10);
	itoa(pmf->m_LinkList.m_Links.GetUpperBound()+1, total,10);
	strState += current;
	strState += "/";
	strState += total;
	SetWindowText(strState);
}
void CLinkListDlg::OnCopyUrl() 
{
	// TODO: Add your control notification handler code here
	int i,count = m_list.GetItemCount();
	if (count<1)
		return;
	short ks;
	ks =GetKeyState(VK_CONTROL);

	CString url ;
	CString name; 
	CString strRet;
	strRet.Empty();

	for (i=0;i<count;i++)
	{
		if (m_list.GetCheck(i))
		{
			if (ks<0)
			{
				name = m_list.GetItemText(i,0);
				strRet += name;
				strRet += "          ";
			}
			url = m_list.GetItemText(i,1);
			strRet += url;
			strRet += "\r\n";
		}
	}

	int len = strRet.GetLength();
	if (len<1)//if not check one, copy current item url
	{
		POSITION pos = m_list.GetFirstSelectedItemPosition();
		int current = m_list.GetNextSelectedItem(pos);
		if (current>=0)
		{
			if (ks<0)
			{
				name = m_list.GetItemText(current,0);
				strRet += name;
				strRet += "          ";
			}
			url = m_list.GetItemText(current,1);
			strRet += url;
			//
			len = strRet.GetLength();
		}
		else
			return;
	}

	_SetClipString(strRet);
}

void CLinkListDlg::OnEditUrl() 
{
	// TODO: Add extra validation here
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int current = m_list.GetNextSelectedItem(pos);
	if (current<0)
		return;
	CInputDlg dlg;
	CString strMsg;
	LOADSTR(strMsg ,IDS_INPUT_URL);
	dlg.m_strMsg = strMsg;
	CString strData = m_list.GetItemText(current, 1);
	dlg.m_strOutput = strData;
	dlg.DoModal();
}

void CLinkListDlg::OnRefreshLinkList()
{
	pmf->SendMessage(WM_COMMAND, ID_ALL_PAGE_LINKS, 0);
}

void CLinkListDlg::OnOK() 
{
	// TODO: Add extra validation here
	CWnd* p = GetFocus();
	if( (p->m_hWnd == m_name.m_hWnd) ||
		(p->GetParent()->m_hWnd == m_url.m_hWnd)
	  )
	{
		OnUpdate();
		return;
	}
	pmf->m_LinkList.Clean();
	CDialog::OnOK();
}

void CLinkListDlg::OnRdblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int current = m_list.GetNextSelectedItem(pos);
	if (current<0)
		return;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*) pNMHDR;
	BOOL bName = (pNMListView->iSubItem==0 ? TRUE : FALSE);
	//
	CInputDlg dlg;
	CString strMsg;
	LOADSTR(strMsg ,bName ? IDS_INPUT_TITLE : IDS_INPUT_URL);
	dlg.m_strMsg = strMsg;
	CString strData = m_list.GetItemText(current, bName ? 0 : 1);
	dlg.m_strOutput = strData;
	dlg.DoModal();

	*pResult = 0;
}
