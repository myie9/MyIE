// AddressBarEx.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "AddressBarEx.h"
#include "MainFrm.h"

#include "Consts_gf.h"
#include "HelperStr.h"
#include "Gf_StartPageMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on)

/////////////////////////////////////////////////////////////////////////////
// CAddressBarEx

CAddressBarEx::CAddressBarEx()
{
}

CAddressBarEx::~CAddressBarEx()
{
}


BEGIN_MESSAGE_MAP(CAddressBarEx, CToolBar)
	//{{AFX_MSG_MAP(CAddressBarEx)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddressBarEx message handlers

void CAddressBarEx::OnSize(UINT nType, int cx, int cy) 
{
	CToolBar::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rect2;
	GetItemRect(1, rect2);
	int w;
	if(pmf->m_bShowAddressBarButton)
		w= cx-rect2.Width()-2;
	else
		w = cx;
		
	if (::IsWindow(m_wndAddress))
	{
		SetButtonInfo(0, 0, TBBS_SEPARATOR, w);
		m_wndAddress.SetWindowPos(NULL,  0, 0, w, cy, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOCOPYBITS);
		if(!m_wndAddress.IsWindowVisible())
			m_wndAddress.ShowWindow(SW_SHOW);
	}
}

CComboBoxEx* CAddressBarEx::GetAddressBox()
{
	return &m_wndAddress;
}

void CAddressBarEx::Init() 
{
	// TODO: Add your specialized creation code here  //gfl list of address. 450   124/6 155/8 220/12 
	if (!m_wndAddress.Create(CBS_DROPDOWN | CBS_AUTOHSCROLL | WS_CHILD, CRect(0, 0, 200, CConsts::_iAdressListUIHight), this, ID_TOOLBAR_ADDRESS))
	{
		TRACE0("Failed to create combobox\n");
		return ;      // fail to create
	}
	m_wndAddress.SetExtendedStyle(0, m_wndAddress.GetExtendedStyle()|CBES_EX_NOSIZELIMIT);
	InitImage();
	if (!ms_dropTarget.Register(this)) {
		TRACE("Fail in registing drop target\n");
	}
}

void CAddressBarEx::InitImage() 
{
	CImageList *oldimg;
	oldimg = m_wndAddress.GetImageList();
	if(oldimg!=NULL)
		oldimg->DeleteImageList();
	if (pmf->m_bComboShowIcon)
	{
		DWORD dwStyle = m_wndAddress.GetExtendedStyle();
		dwStyle &= ~(CBES_EX_NOEDITIMAGE|CBES_EX_NOEDITIMAGEINDENT) ;
		m_wndAddress.SetExtendedStyle(0,dwStyle);
		CImageList img;
		HBITMAP hbmp = pmf->GetBitmap("FavBar.bmp");
		img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 2, 1);
		ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
		DeleteObject(hbmp);
		m_wndAddress.SetImageList(&img);
		img.Detach();
	}
	else
	{
		DWORD dwStyle = m_wndAddress.GetExtendedStyle();
		dwStyle |= (CBES_EX_NOEDITIMAGE|CBES_EX_NOEDITIMAGEINDENT) ;
		m_wndAddress.SetExtendedStyle(0,dwStyle);
	}
}

/*************************************************************
CAddDropTarget
**************************************************************/

DROPEFFECT CAddDropTarget::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point)
{
	return DROPEFFECT_SCROLL | DROPEFFECT_COPY | DROPEFFECT_LINK;
}

DROPEFFECT CAddDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, 
                                      DWORD dwKeyState, CPoint point)
{
	DROPEFFECT dwEffect;
	dwEffect=DROPEFFECT_COPY;
    return dwEffect;
}

void CAddDropTarget::OnDragLeave(CWnd* pWnd)
{
}

DROPEFFECT CAddDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, 
                                     DWORD dwKeyState, CPoint point)
{
	DROPEFFECT dwEffect;
	dwEffect=DROPEFFECT_COPY;
	return dwEffect;
}

BOOL CAddDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
                           DROPEFFECT dropEffect, CPoint point)
{
	try{
    HGLOBAL hData=pDataObject->GetGlobalData(CF_TEXT);
	LPCSTR lpcszData;
	CString strUrl;
    if (hData)
	{
		lpcszData=(LPCSTR)GlobalLock(hData);
		strUrl = lpcszData;
		//for link only search the text
		if (dropEffect==DROPEFFECT_LINK && *pWnd == pmf->m_SearchBar)
		{
			HGLOBAL hLinkData=pDataObject->GetGlobalData(RegisterClipboardFormat("FileGroupDescriptor"));
			if(hLinkData)
			{
				strUrl = ((LPCSTR)GlobalLock(hLinkData)) + 76;
				strUrl = strUrl.Left(strUrl.GetLength()-4);//cut .url of abc.url
				GlobalUnlock(hLinkData);
			}
		}
    }
	else
	{
		hData=pDataObject->GetGlobalData(CF_HDROP);
		if (!hData) {
			TRACE("Fail in getting data\n");
			return FALSE;
		}
		HDROP hdrop =(HDROP)GlobalLock(hData);
		DragQueryFile(hdrop, 0, g_szFile, 1024);
		strUrl = g_szFile;
	}
    GlobalUnlock(hData);

	short ks=GetKeyState(VK_CONTROL), ss=GetKeyState(VK_SHIFT);
	if (*pWnd == pmf->m_AddressBar)
	{
		//if the protocol is javascript then ignore
		if ( IS_SCRIPT_MAILTO(strUrl) )
			return FALSE;

		//check alias
		CString strRet;
		if(pmf->m_bUseAlias && pmf->m_mapAlias.Lookup(strUrl, strRet))
			strUrl = strRet;

		//only ctrl:	search string
		//only shift:	set search string
		//ctrl+shift: highlight
		//no ctrl,shift: open string
		if (ks<0 && ss>=0)
		{
			pmf->m_wndSearch->GetEditCtrl()->SetWindowText(strUrl);
			pmf->m_nSearchBarAction = 1;
			SendMessage(pmf->GetSafeHwnd(), WM_COMMAND, ID_SEARCH_OK, 0);
		}
		else if (ks>=0 && ss<0)
			pmf->m_wndSearch->GetEditCtrl()->SetWindowText(strUrl);
		else if (ks<0 && ss<0)
		{
			pmf->m_wndSearch->GetEditCtrl()->SetWindowText(strUrl);
			pmf->m_nSearchBarAction = 4;
			SendMessage(pmf->GetSafeHwnd(), WM_COMMAND, ID_SEARCH_OK, 0);
		}
		else
			pmf->NewChildWindow(1,2,strUrl);
	}
	else
	{
		//only ctrl:  find next
		//only shift:	set search string
		//ctrl+shift: highlight
		//no ctrl,shift: search
		pmf->m_wndSearch->GetEditCtrl()->SetWindowText(strUrl);
		if (ks<0 && ss>=0)
		{
			pmf->m_nSearchBarAction = 2;
			SendMessage(pmf->GetSafeHwnd(), WM_COMMAND, ID_SEARCH_OK, 0);
		}
		else if (ks>=0 && ss<0)
			;
		else if (ks<0 && ss<0)
		{
			pmf->m_nSearchBarAction = 4;
			SendMessage(pmf->GetSafeHwnd(), WM_COMMAND, ID_SEARCH_OK, 0);
		}
		else
		{
			int nButton = pmf->m_SearchBar.GetToolBarCtrl().HitTest(&point);
			int nIconCount = pmf->m_SearchList.m_nIconCount;
			//
			if (nIconCount && nButton>2 && nButton<nIconCount+3)
				SendMessage(pmf->GetSafeHwnd(), WM_COMMAND, B_SEARCH+pmf->m_SearchList.GetIconIndex(nButton-3), 0);
			else
			{
				int nBegin = nIconCount ? nIconCount+4 : 3;//high light pos
				if (nButton==nBegin)
					pmf->m_nSearchBarAction = 4;
				else if (nButton==nBegin+1)
					pmf->m_nSearchBarAction = 2;
				else
					pmf->m_nSearchBarAction = 1;
				SendMessage(pmf->GetSafeHwnd(), WM_COMMAND, ID_SEARCH_OK, 0);
			}
		}
	}

	}catch(...){}

    return TRUE;
}

//#pragma optimize( "s", off)

void CAddressBarEx::GetSelText(int index, CString & str)
{
	if(index<0)
		return;

	try{
	static TCHAR	sz[MAX_PATH];
	COMBOBOXEXITEM	item;
	item.mask = CBEIF_TEXT;
	item.pszText = (LPTSTR)sz;
	item.cchTextMax = MAX_PATH;

	item.iItem = index;
	m_wndAddress.GetItem(&item);
	CString strItem = sz;

	CString strUrl = pmf->GetAddressTypedUrlFromList(index);

	if (strUrl == "" || !CHelperStr::Contains(strItem, strUrl) )
	{
		ASSERT(false);
		int iPosSpace = strItem.Find( CConsts::_strSpaceBetweenTitleAndUrl);
		int iPosComma = strItem.Find("://");

		if ( (-1 == iPosSpace) || (iPosComma < iPosSpace) )
		{
			str = strItem;
			return;
		}

		str = strItem.Mid(iPosSpace);
		str.TrimLeft();
	}
	else
	{
	
		str = strUrl;
	
	}

	CGf_StartPageMgr::_ins.AddFav(str, str);

	}catch(...){}
}

int CAddressBarEx::FindStringExact(int start, CString & str)
{
	static TCHAR           sz[MAX_PATH];

	try{
	COMBOBOXEXITEM item;

	item.mask = CBEIF_TEXT;
	item.pszText = (LPTSTR)sz;
	item.cchTextMax = MAX_PATH;

	int nc = m_wndAddress.SendMessage(CB_GETCOUNT, 0, 0);
	for(register int i=start+1;i<nc;i++)
	{
		item.iItem = i;
		m_wndAddress.GetItem(&item);
		if(str.CompareNoCase(sz)==0)
			return i;
	}
	}catch(...){}

	return CB_ERR;
}
