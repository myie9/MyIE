// AddressBarEx.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "ComboBar.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on)

/////////////////////////////////////////////////////////////////////////////
// CComboBar

CComboBar::CComboBar()
{
}

CComboBar::~CComboBar()
{
}


BEGIN_MESSAGE_MAP(CComboBar, CToolBar)
	//{{AFX_MSG_MAP(CComboBar)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboBar message handlers

void CComboBar::OnSize(UINT nType, int cx, int cy) 
{
	CToolBar::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	int w;
	if(pmf->m_bShowSearchBarButton)
	{
		int bc = GetToolBarCtrl().GetButtonCount();
		RECT rect1,rect2;
		GetItemRect(1,&rect1);
		GetItemRect(bc-1,&rect2);
		w= cx - (rect2.right-rect1.left);
	}
	else
		w = cx;

	if (::IsWindow(m_wndComboBox))
	{
		SetButtonInfo(0, 0, TBBS_SEPARATOR, w);
		m_wndComboBox.SetWindowPos(NULL,  0, 0, w, cy, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOCOPYBITS);
		if(!m_wndComboBox.IsWindowVisible())
			m_wndComboBox.ShowWindow(SW_SHOW);
	}
}

void CComboBar::Init() 
{
	// TODO: Add your specialized creation code here
	if (!m_wndComboBox.Create(CBS_DROPDOWN | CBS_AUTOHSCROLL | WS_CHILD, CRect(0, 0, 200, 450), this, ID_TOOLBAR_SEARCH))
	{
		TRACE0("Failed to create combobox\n");
		return ;      // fail to create
	}
	m_wndComboBox.SetExtendedStyle(0, m_wndComboBox.GetExtendedStyle()|CBES_EX_NOSIZELIMIT);

	InitImage();

	if (!ms_dropTarget.Register(this)) {
		TRACE("Fail in registing drop target\n");
	}
	return;
}
void CComboBar::InitImage()
{
	CImageList *oldimg;
	oldimg = m_wndComboBox.GetImageList();
	if(oldimg!=NULL)
		oldimg->DeleteImageList();

	CImageList img;
	HBITMAP hbmp;

	if (pmf->m_bComboShowIcon)
	{
		DWORD dwStyle = m_wndComboBox.GetExtendedStyle();
		dwStyle &= ~(CBES_EX_NOEDITIMAGE|CBES_EX_NOEDITIMAGEINDENT) ;
		m_wndComboBox.SetExtendedStyle(0,dwStyle);
		//
		hbmp = pmf->GetBitmap("SearchBar.bmp");
		img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 2, 1);
		ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
		DeleteObject(hbmp);
		m_wndComboBox.SetImageList(&img);
		img.Detach();
	}
	else
	{
		DWORD dwStyle = m_wndComboBox.GetExtendedStyle();
		dwStyle |= (CBES_EX_NOEDITIMAGE|CBES_EX_NOEDITIMAGEINDENT) ;
		m_wndComboBox.SetExtendedStyle(0,dwStyle);
	}

	oldimg = GetToolBarCtrl().GetImageList();
	if(oldimg!=NULL)
		oldimg->DeleteImageList();

	int nIconCount = pmf->m_SearchList.m_nIconCount;
	//
	hbmp = pmf->GetBitmap("SearchBar.bmp");
	img.Create(16, 16, ILC_COLOR32|ILC_MASK, nIconCount ? 4+nIconCount+1 : 4, 1);
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);
	//
	if (nIconCount)
	{
		CSearch *item;
		HICON hIconSm;
		CString strIcon;
		for(int i=0;i<=pmf->m_SearchList.m_Searchs.GetUpperBound();i++)
		{
			item = pmf->m_SearchList.m_Searchs.GetAt(i);
			if (item->icon.GetLength())
			{
				strIcon = item->icon;
				DO_RELATIVE_URL(strIcon);
				hIconSm = (HICON)LoadImage(NULL, 
					strIcon,
					IMAGE_ICON,
					16, 16,
					LR_LOADFROMFILE
					);
				if (hIconSm==NULL)
				{
					item->icon = "";//must do this, cancel add the button
					pmf->m_SearchList.m_nIconCount--;
				}
				else
				{
					img.Add(hIconSm);
					DestroyIcon(hIconSm);
				}
			}
		}
	}
	//
	SendMessage(TB_SETIMAGELIST, 0, (LPARAM)img.m_hImageList);
	img.Detach();
}

void CComboBar::InitButton()
{
	int b = pmf->m_SearchBar.GetToolBarCtrl().GetButtonCount();
	for(int i = 0; i<b;i++)
		pmf->m_SearchBar.GetToolBarCtrl().DeleteButton(0);
	//
	int nIconCount = pmf->m_SearchList.m_nIconCount;
	pmf->m_SearchBar.SetButtons(NULL, nIconCount ? 5+nIconCount+1 : 5);
	//
	int nIndex=0;
	pmf->m_SearchBar.SetButtonInfo(nIndex++, 0, TBBS_SEPARATOR, 0);
	pmf->m_SearchBar.SetButtonInfo(nIndex++, ID_SEARCH_OK, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, 0);
	//
	if (nIconCount)
	{
		pmf->m_SearchBar.SetButtonInfo(nIndex++, 0, TBBS_SEPARATOR, 0);
		CSearch *item;
		for(int i=0;i<=pmf->m_SearchList.m_Searchs.GetUpperBound();i++)
		{
			item = pmf->m_SearchList.m_Searchs.GetAt(i);
			if (item->icon.GetLength())
			{
				pmf->m_SearchBar.SetButtonInfo(nIndex, B_SEARCH+i, TBSTYLE_BUTTON, nIndex+1);
				pmf->m_SearchBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(item->name, &pmf->m_SearchBar, B_SEARCH+i);
				nIndex++;
			}
		}
	}
	pmf->m_SearchBar.SetButtonInfo(nIndex++, 0, TBBS_SEPARATOR, 0);
	pmf->m_SearchBar.SetButtonInfo(nIndex++, ID_HIGH_LIGHT_SEARCH_BAR, TBSTYLE_BUTTON, 2);
	pmf->m_SearchBar.SetButtonInfo(nIndex++, ID_FIND_SEARCH_BAR, TBSTYLE_BUTTON, 3);
}

//#pragma optimize( "s", off)

void CComboBar::GetSelText(int index, CString & str)
{
	if(index<0)
		return;

	try{
	static TCHAR           sz[MAX_PATH];
	COMBOBOXEXITEM item;
	item.mask = CBEIF_TEXT;
	item.pszText = (LPTSTR)sz;
	item.cchTextMax = MAX_PATH;

	item.iItem = index;
	m_wndComboBox.GetItem(&item);
	str = sz;
	}catch(...){}
}

int CComboBar::FindStringExact(int start, CString & str)
{
	static TCHAR	sz[MAX_PATH];

	try{
	COMBOBOXEXITEM item;
	item.mask = CBEIF_TEXT;
	item.pszText = (LPTSTR)sz;
	item.cchTextMax = MAX_PATH;
	int nc = m_wndComboBox.SendMessage(CB_GETCOUNT, 0, 0);
	for(register int i=start+1;i<nc;i++)
	{
		item.iItem = i;
		m_wndComboBox.GetItem(&item);
		if(str.CompareNoCase(sz)==0)
			return i;
	}
	}catch(...){}

	return CB_ERR;
}
