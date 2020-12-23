// CustomizeToolbar.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "CustomizeToolbar.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
#define BUTTON_COUNT	44

/////////////////////////////////////////////////////////////////////////////
extern  UINT g_nToolBarID[];
extern  UINT g_nToolBarText[];
extern  UINT g_nToolBarImg[];

/////////////////////////////////////////////////////////////////////////////
// CCustomizeToolbar dialog
CCustomizeToolbar::CCustomizeToolbar(CWnd* pParent /*=NULL*/)
	: CDialog(CCustomizeToolbar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCustomizeToolbar)
	m_bColorIcon = FALSE;
	m_nTextOpt = -1;
	m_bSmallToolBar = -1;
	//}}AFX_DATA_INIT
}


void CCustomizeToolbar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomizeToolbar)
	DDX_Control(pDX, IDC_CURBTN, m_curbtn);
	DDX_Control(pDX, IDC_AVABTN, m_avabtn);
	DDX_Check(pDX, IDC_COLOR_ICON, m_bColorIcon);
	DDX_CBIndex(pDX, IDC_TEXT_OPT, m_nTextOpt);
	DDX_CBIndex(pDX, IDC_ICON_OPT, m_bSmallToolBar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCustomizeToolbar, CDialog)
	//{{AFX_MSG_MAP(CCustomizeToolbar)
	ON_BN_CLICKED(IDC_MOVWDOWN, OnMoveDown)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveUp)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_NOTIFY(NM_DBLCLK, IDC_AVABTN, OnDblclkAvabtn)
	ON_NOTIFY(NM_DBLCLK, IDC_CURBTN, OnDblclkCurbtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomizeToolbar message handlers

BOOL CCustomizeToolbar::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogCustomize");
#endif
	CDialog::OnInitDialog();
#ifdef _WRITE_LNG_FILE_
	_WriteDlgComboBox(this,"DialogCustomize");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogCustomize");

	m_avabtn.SetExtendedStyle(m_avabtn.GetExtendedStyle()| LVS_EX_FULLROWSELECT);
	m_curbtn.SetExtendedStyle(m_curbtn.GetExtendedStyle()| LVS_EX_FULLROWSELECT);

	int n=0;
	CImageList img;
	img.Create(24, 24, ILC_COLORDDB|ILC_MASK, 40, 1);
	HBITMAP hbmp = pmf->GetBitmap("MainTool24.bmp");
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);
	m_avabtn.SetImageList(&img, LVSIL_SMALL);
	img.Detach();

	LOADSTR(str ,IDS_SEPARATOR);
	CRect rect;
	m_avabtn.InsertColumn(0,"");
	m_avabtn.GetWindowRect(rect);
	m_avabtn.SetColumnWidth(0, rect.Width()-25);
	m_avabtn.InsertItem(0, str, -1);
	m_avabtn.SetItemData(0, 1);

	int nMaxBtn = BUTTON_COUNT;//sizeof(g_nToolBarID)/sizeof(UINT);
	n=1;
	for(int i=0; i<nMaxBtn; i++)
	{
		if(g_nToolBarID[i]!=0)
		{
			LOADSTR(str ,g_nToolBarText[i]);
			m_avabtn.InsertItem(n, str.Right(str.GetLength()-str.Find('\n')-1), g_nToolBarImg[i]);
			m_avabtn.SetItemData(n, i);
			n++;
		}
	}
	
	img.Create(24, 24, ILC_COLORDDB|ILC_MASK, 40, 1);
	hbmp = pmf->GetBitmap("MainTool24.bmp");
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);
	m_curbtn.SetImageList(&img, LVSIL_SMALL);
	img.Detach();

	m_curbtn.InsertColumn(0,"");
	m_curbtn.GetWindowRect(rect);
	m_curbtn.SetColumnWidth(0, rect.Width()-25);

	int ids[50], idx=0, id;
	char tmp[6]; tmp[0]='B'; 

	id=0;idx=0;
	while(idx<nMaxBtn && id!=-1)
	{
		itoa(idx,tmp+1,10);
		id = (int)AfxGetApp()->GetProfileInt("ToolBar", tmp, -5);
		if(id==-5)
			ids[idx] = idx;
		else if(id>=nMaxBtn)
			ids[idx] = 1;//add a seprate
		else
			ids[idx]=id;
		idx++;
	}
	LVFINDINFO info;
	int index;
	n=0;
	for(i=0; i<idx; i++)
	{
		itoa(i,tmp+1,10);
		id = ids[i];
		if(id>=0 && id<nMaxBtn)
		{
			if(g_nToolBarText[id]>0)
			{
				LOADSTR(str ,g_nToolBarText[id]);
				m_curbtn.InsertItem(n, str.Right(str.GetLength()-str.Find('\n')-1), g_nToolBarImg[id]);
				m_curbtn.SetItemData(n, id);
				//delete from available list
				info.flags = LVFI_PARAM;
				info.lParam = id;
				index=m_avabtn.FindItem(&info);
				if(index != -1)
				{
					m_avabtn.DeleteItem(index);
				}
			}
			else
			{
				LOADSTR(str ,IDS_SEPARATOR);
				m_curbtn.InsertItem(n, str, -1);
				m_curbtn.SetItemData(n, 1);
			}
			n++;
		}
		else if(id>=nMaxBtn)
			continue;
		else
			break;
	}
	m_curbtn.InsertItem(n, "                 ", -1);
	m_curbtn.SetItemData(n, -1);
	LIST_FOCUS_POS(m_curbtn,n);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCustomizeToolbar::OnOK() 
{
	// TODO: Add extra validation here
	int id;
	char tmp[6]; tmp[0]='B'; 

	id=0;
	int n = m_curbtn.GetItemCount();
	for( int i=0; i<n; i++)
	{
		itoa(i,tmp+1,10);
		id = m_curbtn.GetItemData(i);
		AfxGetApp()->WriteProfileInt("ToolBar", tmp, id);
	}

	CDialog::OnOK();
}

void CCustomizeToolbar::OnMoveDown() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_curbtn.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int i = m_curbtn.GetNextSelectedItem(pos);
		int id;
		id = m_curbtn.GetItemData(i);
		if(id>=0 && i<m_curbtn.GetItemCount()-2)
		{
			m_curbtn.DeleteItem(i);
			if (g_nToolBarText[id]>0)
			{
				LOADSTR(str ,g_nToolBarText[id]);
				m_curbtn.InsertItem(i+1, str.Right(str.GetLength()-str.Find('\n')-1), g_nToolBarImg[id]);
				m_curbtn.SetItemData(i+1, id);
			}
			else
			{
				LOADSTR(str ,IDS_SEPARATOR);
				m_curbtn.InsertItem(i+1, str, -1);
				m_curbtn.SetItemData(i+1, id);
			}
			LIST_FOCUS_POS(m_curbtn,i+1);
		}
	}		
}

void CCustomizeToolbar::OnMoveUp() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_curbtn.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int i = m_curbtn.GetNextSelectedItem(pos);
		int id;
		id = m_curbtn.GetItemData(i);
		if(id>=0 && i>0)
		{
			m_curbtn.DeleteItem(i);
			if (g_nToolBarText[id]>0)
			{
				LOADSTR(str ,g_nToolBarText[id]);
				m_curbtn.InsertItem(i-1, str.Right(str.GetLength()-str.Find('\n')-1), g_nToolBarImg[id]);
				m_curbtn.SetItemData(i-1, id);
			}
			else
			{
				LOADSTR(str ,IDS_SEPARATOR);
				m_curbtn.InsertItem(i-1, str, -1);
				m_curbtn.SetItemData(i-1, id);
			}
			LIST_FOCUS_POS(m_curbtn,i-1);
		}
	}		
}

void CCustomizeToolbar::OnRemove() 
{
	// TODO: Add your control notification handler code here
	int j;
	POSITION pos = m_curbtn.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int i = m_curbtn.GetNextSelectedItem(pos);
		int id;
		id = m_curbtn.GetItemData(i);
		if(id>=0)
		{
			j = m_avabtn.GetItemCount();
			if(id!=1) //not a separator
			{
				LOADSTR(str ,g_nToolBarText[id]);
				m_avabtn.InsertItem(j, str.Right(str.GetLength()-str.Find('\n')-1), g_nToolBarImg[id]);
				m_avabtn.SetItemData(j, id);
				m_curbtn.DeleteItem(i);
			}
			else
			{
				m_curbtn.DeleteItem(i);
			}
			m_curbtn.EnsureVisible(i, TRUE);
		}
		//
		LIST_FOCUS_POS(m_curbtn,i);
	}
}

void CCustomizeToolbar::OnAdd() 
{
	// TODO: Add your control notification handler code here
	int j;
	POSITION pos = m_avabtn.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int i = m_avabtn.GetNextSelectedItem(pos);
		int id;
		id = m_avabtn.GetItemData(i);
		//get current selected item in curbtns
		POSITION pos1 = m_curbtn.GetFirstSelectedItemPosition();
		if(pos1!=NULL)
		{
			j = m_curbtn.GetNextSelectedItem(pos1);
		}
		else
			j = m_curbtn.GetItemCount()-1;

		if(i>0)
		{
			LOADSTR(str ,g_nToolBarText[id]);
			m_curbtn.InsertItem(j, str.Right(str.GetLength()-str.Find('\n')-1), g_nToolBarImg[id]);
			m_curbtn.SetItemData(j, id);
			m_avabtn.DeleteItem(i);
		}
		else
		{
			LOADSTR(str ,IDS_SEPARATOR);
			m_curbtn.InsertItem(j, str, -1);
			m_curbtn.SetItemData(j, 1);
		}
		m_curbtn.EnsureVisible(j, TRUE);
		//
		LIST_FOCUS_POS(m_avabtn,i);
	}	
}

void CCustomizeToolbar::OnDblclkAvabtn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnAdd();
	*pResult = 0;
}

void CCustomizeToolbar::OnDblclkCurbtn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnRemove();
	*pResult = 0;
}

//#pragma optimize( "s", off)

