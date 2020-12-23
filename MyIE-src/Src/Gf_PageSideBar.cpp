// Gf_PageSideBar.cpp: implementation of the CGf_PageSideBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Resource.h"
#include "Gf_PageSideBar.h"
#include "Gf_AppStatic.h"
#include "HelperFile.h"
#include "GfPathManager.h"
#include "HelperStr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define GF_SEC_PAGESIDE "PageSideBar"
#define GF_SEC_PAGESIDE_NAME "PageSideBarName"

#define GF_KEY_PAGESIDE "url"
#define GF_KEY_PAGESIDE_NAME "name"
#define ID_PAGESIDEBAR_COMBO 103

#define GF_SHOW_SPACE "  "   //名称和url之间的分割符
#define GF_PAD_CHAR ' '
#define GF_MAX_NAME_LEN 0 //0:not gu ding width  为12  
#define GF_NULL _T("::") //ini文件中不能取空。所以用此替代

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_PageSideBar::CGf_PageSideBar()
:_bDirt(FALSE)
,_filepath(_T(""))
{
	
}

CGf_PageSideBar::~CGf_PageSideBar()
{
	GF_TRY
	{
		SaveAdressList();

		m_typeFont.DeleteObject();
		_page.DestroyWindow();
	}GF_CATCH	
	
}

BEGIN_MESSAGE_MAP(CGf_PageSideBar, CWnd)
//{{AFX_MSG_MAP(CGf_PageSideBar)
ON_WM_CREATE()
ON_WM_SIZE()
ON_COMMAND(IDOK, OnRefresh)
//}}AFX_MSG_MAP
ON_CBN_SELCHANGE(ID_PAGESIDEBAR_COMBO, OnComboSelChange)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGf_PageSideBar message handlers

int CGf_PageSideBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	_page.Create(NULL, "", WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN, CRect(0,0,0,0), this, IDS_TITLE_RESOURCE,NULL);

	if (!m_fType.Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWN| CBS_AUTOHSCROLL|CBES_EX_NOSIZELIMIT, CRect(0,0,0,0), this, ID_PAGESIDEBAR_COMBO))
	{
		return -1;      // fail to create
	}

	{
		
		m_typeFont.DeleteObject();
		
		LOGFONT lFont;
		ZeroMemory ((PVOID) &lFont,sizeof (LOGFONT));
		NONCLIENTMETRICS nm;
		nm.cbSize = sizeof (NONCLIENTMETRICS);
		VERIFY (SystemParametersInfo(SPI_GETNONCLIENTMETRICS,nm.cbSize,&nm,0)); 
		lFont =  nm.lfMenuFont;
		
		lFont.lfWeight = 0;
		lFont.lfItalic = 0;
		lFont.lfUnderline = 0;
		m_typeFont.CreateFontIndirect(&lFont);
	
		m_fType.SetFont(&m_typeFont);
	}
	
	//
	CImageList img;
	m_wndRef.CreateEx(this);
	img.Create(13, 15, ILC_COLORDDB|ILC_MASK, 1, 1);
	HBITMAP hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_REFRESH));
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);
	m_wndRef.GetToolBarCtrl().SetImageList(&img);
	img.Detach();
	
	m_wndRef.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT|TBSTYLE_TOOLTIPS  );
	m_wndRef.SetButtonInfo(0, IDOK, TBSTYLE_BUTTON, 0);
	m_wndRef.GetToolBarCtrl().GetToolTips()->UpdateTipText(IDS_REFRESH, this, IDOK);
	
	InitAddressList();
	Populate();
	
	ToNavigate(GetFirstAddress() );

	return 0;
}

void CGf_PageSideBar::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	int nSize= 23;//button size
	m_fType.MoveWindow(0, 1, cx-nSize+2, 150);//nSize-2);
	m_wndRef.MoveWindow(cx-nSize+2, 1, nSize, nSize);
	_page.MoveWindow(0,nSize-2,cx,cy-nSize+2);
}

void CGf_PageSideBar::OnRefresh()
{
	CString str;
	m_fType.GetWindowText(str);

	str.TrimLeft();
	str.TrimRight();

	CString name=GF_NULL;
	CString url=str;
	if (CHelperStr::Contains(str, GF_SHOW_SPACE))
	{
		url=CHelperStr::GetSubStr(str, GF_SHOW_SPACE, "");

		name=str;
		name.Replace(url,"");
		name.TrimRight();

		url.TrimLeft();
	}
	
	int n=_lstPageSiderBarAddress.FindInt(url);
	//POSITION pos=_lstPageSiderBarAddress.Find(str);
	if (n>=0)
	{
		_lstPageSiderBarAddress.Remove(n);

		CString oldName=_lstNames.GetAt(n);
		if (name==GF_NULL)
		{
			name=oldName;
		}
		_lstNames.Remove(n);

	}
	
	_lstPageSiderBarAddress.AddHead(url);
	_lstNames.AddHead(name);

	_bDirt=TRUE;
	Populate();
	ToNavigate(url);
	
	
}
void CGf_PageSideBar::OnComboSelChange()
{
	
	int n=m_fType.GetCurSel();
	//m_fType.GetLBText(m_fType.GetCurSel(), str); 	
	if (n<0)
	{
		return;
	}
	
	CString str=_lstPageSiderBarAddress.GetAt(n);
	
	ToNavigate(str);
	
//	m_fType.SetWindowText(str);
}

void CGf_PageSideBar::InitAddressList()
{
	_filepath=CGfPathManager::GetPluginData();

	if (_lstPageSiderBarAddress.GetCount()>0)
	{
		return;
	}

	CHelperFile::AddToStrList(_filepath, GF_SEC_PAGESIDE, GF_KEY_PAGESIDE, _lstPageSiderBarAddress );
	CHelperFile::AddToStrList(_filepath, GF_SEC_PAGESIDE_NAME, GF_KEY_PAGESIDE_NAME, _lstNames, FALSE );

	PadNames();
	
	
	if (_lstPageSiderBarAddress.GetCount()>0)
	{
		return;
	}

	CHelperFile::AddToStrList(CGfPathManager::GetPluginDataSys(), GF_SEC_PAGESIDE, GF_KEY_PAGESIDE, _lstPageSiderBarAddress );
	CHelperFile::AddToStrList(CGfPathManager::GetPluginDataSys(), GF_SEC_PAGESIDE_NAME, GF_KEY_PAGESIDE_NAME, _lstNames, FALSE );

	PadNames();
}

void CGf_PageSideBar::SaveAdressList()
{
	if (!_bDirt)
	{
		return;
	}

	CHelperFile::Write(_filepath, GF_SEC_PAGESIDE, GF_KEY_PAGESIDE, _lstPageSiderBarAddress );
	CHelperFile::Write(_filepath, GF_SEC_PAGESIDE_NAME, GF_KEY_PAGESIDE_NAME, _lstNames );


	_bDirt=FALSE;
	
}

void CGf_PageSideBar::Populate()
{
	m_fType.ResetContent();
	POSITION pos=_lstPageSiderBarAddress.GetHeadPosition();
	int nposName=0;
	while(pos)
	{
		CString name=_lstNames.GetAt(nposName);
		nposName++;

		CString url=_lstPageSiderBarAddress.GetNext(pos);
		

		CString strShow;
		CString strPad="";

		name.TrimLeft();
		name.Replace(GF_NULL, "");
		if (!name.IsEmpty())
		{
			if (name.GetLength()<GF_MAX_NAME_LEN)
			{
				CString strPadtmp(GF_PAD_CHAR,2*( GF_MAX_NAME_LEN-name.GetLength() )); //2个空格是一个字母
				strPad=strPadtmp;
			}

			strPad+=GF_SHOW_SPACE;

		}

		

		strShow.Format("%s%s%s", CHelperStr::GetSubStr(name,GF_MAX_NAME_LEN, ""), strPad, url);
		m_fType.AddString(strShow);
	}
	
	m_fType.SetWindowText(GetFirstAddress());
	
}

BOOL CGf_PageSideBar::PreTranslateMessage(MSG* pMsg)
{
	GF_TRY
	{
		
		if(  (pMsg->message == WM_KEYDOWN) )
		{
// 			if (pMsg->wParam==VK_RETURN)
// 			{
// 				OnRefresh();
// 				return TRUE;
// 			} 
// 			else 
				
			if(pMsg->wParam==VK_DELETE)
			{
				if (DeleteCurSel())
				{
					return TRUE;
				}
				
				
			}
		}
		
	}
	GF_CATCH

	return CWnd::PreTranslateMessage(pMsg);
}

BOOL CGf_PageSideBar::DeleteCurSel()
{
	int n=m_fType.GetCurSel();

	if (n<0)
	{
		return FALSE;
	}
	
	CString str;
	m_fType.GetWindowText(str);
	
	
	m_fType.DeleteString(n);

	_lstPageSiderBarAddress.Remove(n);
	_lstNames.Remove(n);

	m_fType.SetWindowText(str);
	_bDirt=TRUE;

	return TRUE;
}

CString CGf_PageSideBar::GetFirstAddress()
{
	return _lstPageSiderBarAddress.GetCount()>0?_lstPageSiderBarAddress.GetHead():"about:blank";
}

void CGf_PageSideBar::ToNavigate(LPCTSTR lpszURL, DWORD dwFlags /* = 0 */,
					   LPCTSTR lpszTargetFrameName /* = NULL */,
					   LPCTSTR lpszHeaders /* = NULL */,
					   LPVOID lpvPostData /* = NULL */, DWORD dwPostDataLen /* = 0 */)
{
	COleSafeArray vPostData;
	if (lpvPostData != NULL)
	{
		if (dwPostDataLen == 0)
			dwPostDataLen = lstrlen((LPCTSTR) lpvPostData);
		
		vPostData.CreateOneDim(VT_UI1, dwPostDataLen, lpvPostData);
	}
	
	COleVariant vURL(lpszURL, VT_BSTR);
	COleVariant vHeaders(lpszHeaders, VT_BSTR);
	COleVariant vTargetFrameName(lpszTargetFrameName, VT_BSTR);
	COleVariant vFlags((long) dwFlags, VT_I4);
	
	_page.Navigate2(vURL,
	vFlags, vTargetFrameName, vPostData, vHeaders);
}

void CGf_PageSideBar::PadNames()
{
	int gap=_lstPageSiderBarAddress.GetCount()-_lstNames.GetCount();
	for (int i=0;i<gap; i++)
	{
		_lstNames.AddTail(GF_NULL);
	}

}