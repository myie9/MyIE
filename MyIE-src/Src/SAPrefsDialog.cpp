/*********************************************************************

   SAPrefsDialog

   Copyright (C) 2002 Smaller Animals Software, Inc.

   This software is provided 'as-is', without any express or implied
   warranty.  In no event will the authors be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

   3. This notice may not be removed or altered from any source distribution.

   http://www.smalleranimals.com
   smallest@smalleranimals.com

**********************************************************************/

// SAPrefsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SAPrefsDialog.h"
#include "mainfrm.h"
#include "MyIE9.h"
#include "GfAppString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSAPrefsDialog dialog


CSAPrefsDialog::CSAPrefsDialog(CWnd* pParent /*=NULL*/)
: CDialog(CSAPrefsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSAPrefsDialog)
	//}}AFX_DATA_INIT
	m_iCurPage = -1;
	m_pages.RemoveAll();
	m_iStartPage = 0;
	m_bValidateBeforeChangingPage = false;
}

/////////////////////////////////////////////////////////////////////////////

CSAPrefsDialog::~CSAPrefsDialog()
{
	GF_TRY{
	
	// clean up
	for (int i=0;i<m_pages.GetSize();i++)
	{
		pageStruct *pPS = (pageStruct *)m_pages.GetAt(i);
		if (pPS)
			delete pPS;
	}
	}GF_CATCH
}

/////////////////////////////////////////////////////////////////////////////

void CSAPrefsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSAPrefsDialog)
	DDX_Control(pDX, IDC_DLG_FRAME, m_boundingFrame);
	DDX_Control(pDX, IDC_PAGE_TREE, m_pageTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSAPrefsDialog, CDialog)
//{{AFX_MSG_MAP(CSAPrefsDialog)
	ON_WM_CREATE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_PAGE_TREE, OnSelchangedPageTree)
	ON_NOTIFY(TVN_GETDISPINFO, IDC_PAGE_TREE, OnGetdispinfoPageTree)
	ON_NOTIFY(TVN_SELCHANGING, IDC_PAGE_TREE, OnSelchangingPageTree)
	ON_BN_CLICKED(IDC_RESTORE, OnRestore)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CHANGE_PAGE, OnChangePage)
	ON_MESSAGE(WM_SET_FOCUS_WND, OnSetFocusWnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSAPrefsDialog message handlers

/////////////////////////////////////////////////////////////////////////////

BOOL CSAPrefsDialog::PreTranslateMessage(MSG* pMsg) 
{
	ASSERT(pMsg != NULL);
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
	
	// Don't let CDialog process the Escape key.
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
	{
		OnCancel();
		return TRUE;
	}
	
	if (CWnd::PreTranslateMessage(pMsg))
		return TRUE;
	
	// don't translate dialog messages when 
	// application is in help mode
	CFrameWnd* pFrameWnd = GetTopLevelFrame();
	if (pFrameWnd != NULL && pFrameWnd->m_bHelpMode)
		return FALSE;
	
	// ensure the dialog messages will not
	// eat frame accelerators
	pFrameWnd = GetParentFrame();
	while (pFrameWnd != NULL)
	{
		if (pFrameWnd->PreTranslateMessage(pMsg))
			return TRUE;
		pFrameWnd = pFrameWnd->GetParentFrame();
	}
	
	return PreTranslateInput(pMsg);
	
}

/////////////////////////////////////////////////////////////////////////////

int CSAPrefsDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CSAPrefsDialog::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogOptions");
#endif
	LOADDLG("DialogOptions");
	CDialog::OnInitDialog();
	
	long l = GetWindowLong(m_pageTree.m_hWnd, GWL_STYLE);
#if (_MSC_VER > 1100)
	l = l | TVS_TRACKSELECT ;
#else
	//#define TVS_TRACKSELECT         0x0200
	l = l | 0x0200;
#endif
	SetWindowLong(m_pageTree.m_hWnd, GWL_STYLE, l);
	
	// where will the dlgs live?
	m_boundingFrame.GetWindowRect(m_frameRect);
	ScreenToClient(m_frameRect);
	m_frameRect.DeflateRect(2,2);
	m_frameRect.top = 2;
	
	// fill the tree. we'll create the pages as we need them
	for (int i=0;i<m_pages.GetSize();i++)
	{
		pageStruct *pPS = (pageStruct *)m_pages.GetAt(i);
		ASSERT(pPS);
		ASSERT(pPS->pDlg);
		if (pPS)
		{
			TV_INSERTSTRUCT tvi;
			
			// find this node's parent...
			tvi.hParent = FindHTREEItemForDlg(pPS->pDlgParent);
			
			tvi.hInsertAfter = TVI_LAST;
			tvi.item.cchTextMax = 0;
			tvi.item.pszText = LPSTR_TEXTCALLBACK;
			tvi.item.lParam = (long)pPS;
			tvi.item.mask = TVIF_PARAM | TVIF_TEXT | TVIF_STATE;
			tvi.item.state = TVIS_EXPANDED;
			
			HTREEITEM hTree = m_pageTree.InsertItem(&tvi);
			
			// keep track of the dlg's we've added (for parent selection)
			if (hTree)
			{
				DWORD dwTree = (DWORD)hTree;
				m_dlgMap.SetAt(pPS->pDlg, dwTree);
			}
		}
	}

	//expand all
	HTREEITEM hCurrent = TreeView_GetRoot(m_pageTree);
	UINT uFlag = NULL;
	uFlag = TVE_EXPAND;
	while (hCurrent)
	{
		TreeView_Expand(m_pageTree, hCurrent,uFlag);
		hCurrent = TreeView_GetNextVisible(m_pageTree, hCurrent);
	}

	// start with page 0
	m_iCurPage = m_iStartPage ;
	ShowPage(m_iCurPage);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////

HTREEITEM CSAPrefsDialog::FindHTREEItemForDlg(CSAPrefsSubDlg *pParent)
{
	// if you didn't specify a parent in AddPage(...) , the
	// dialog becomes a root-level entry
	if (pParent==NULL)
	{
		return TVI_ROOT;
	}
	else
	{
		DWORD dwHTree;
		if (m_dlgMap.Lookup(pParent, dwHTree))
		{
			return (HTREEITEM)dwHTree;
		}
		else
		{
			// you have specified a parent that has not 
			// been added to the tree - can't do that.
			ASSERT(FALSE);
			return TVI_ROOT;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

bool CSAPrefsDialog::AddPage(CSAPrefsSubDlg &dlg, const char *pCaption, CSAPrefsSubDlg* pDlgParent /*=NULL*/)
{
	if (m_hWnd)
	{
		// can't add once the window has been created
		ASSERT(0);
		return false;
	}
	
	pageStruct *pPS = new pageStruct;
	pPS->pDlg = &dlg;
	pPS->id = dlg.GetID();
	pPS->csCaption = pCaption;
	pPS->pDlgParent = pDlgParent;
	
	m_pages.Add(pPS);
	
	return true;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CSAPrefsDialog::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;
	
	cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS, NULL, NULL, NULL);
	cs.style |= WS_CLIPCHILDREN;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

bool CSAPrefsDialog::ShowPage(CSAPrefsSubDlg * pPage)
{
	// find that page
	for (int i=0;i<m_pages.GetSize();i++)
	{
		pageStruct *pPS = (pageStruct *)m_pages.GetAt(i);
		ASSERT(pPS);
		if (pPS)
		{
			ASSERT(pPS->pDlg);
			if (pPS->pDlg == pPage)
			{
				ShowPage(i);
				m_iCurPage = i;
				return true;
			}
		}
	}
	
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool CSAPrefsDialog::ShowPage(int iPage)
{
	// turn off the current page
	if ((m_iCurPage >= 0) && (m_iCurPage < m_pages.GetSize()))
	{
		pageStruct *pPS = (pageStruct *)m_pages.GetAt(m_iCurPage);
		ASSERT(pPS);
		if (pPS)
		{
			ASSERT(pPS->pDlg);
			if (pPS->pDlg)
			{
				if (::IsWindow(pPS->pDlg->m_hWnd))
				{
					pPS->pDlg->ShowWindow(SW_HIDE);
				}
			}
		}
		else
		{
			return false;
		}
	}
	
	// show the new one
	if ((iPage >= 0) && (iPage < m_pages.GetSize()))
	{
		pageStruct *pPS = (pageStruct *)m_pages.GetAt(iPage);
		ASSERT(pPS);
		
		if (pPS)
		{
			ASSERT(pPS->pDlg);
			if (pPS->pDlg)
			{
				// if we haven't already, Create the dialog
				if (!::IsWindow(pPS->pDlg->m_hWnd))
				{
					pPS->pDlg->Create(pPS->pDlg->GetID(), this);
				}
				
				// move, show, focus
				if (::IsWindow(pPS->pDlg->m_hWnd))
				{
					pPS->pDlg->MoveWindow(m_frameRect.left, m_frameRect.top, m_frameRect.Width(), m_frameRect.Height());
					pPS->pDlg->ShowWindow(SW_SHOW);
					//pPS->pDlg->SetFocus();
				}
				
				// find this in our map
				HTREEITEM hItem = FindHTREEItemForDlg(pPS->pDlg);
				if (hItem)
				{
					// select it
					m_pageTree.SelectItem(hItem);
					m_pageTree.SetFocus();
				}
				return true;
			}
		}
	}
	
	return false;
}

/////////////////////////////////////////////////////////////////////////////

void CSAPrefsDialog::OnOK() 
{
	// if EndOK returns true, all of the UpdateData(TRUE)'s succeeded
	if (EndOK())
	{
		CDialog::OnOK();
	}
}

/////////////////////////////////////////////////////////////////////////////

bool CSAPrefsDialog::EndOK()
{
	bool bOK = true;
	
	CSAPrefsSubDlg * pPage = NULL;
	
	// first, UpdateData...
	for (int i=0;i<m_pages.GetSize();i++)
	{
		pageStruct *pPS = (pageStruct *)m_pages.GetAt(i);
		ASSERT(pPS);
		if (pPS)
		{
			ASSERT(pPS->pDlg);
			if (pPS->pDlg)
			{
				if (::IsWindow(pPS->pDlg->m_hWnd))
				{
					if (!pPS->pDlg->UpdateData(TRUE))
					{
						bOK = false;
						pPage = pPS->pDlg;
						break;
					}
				}
			}
		}
	}
	
	// were there any UpdateData errors?
	if ((!bOK) && (pPage!=NULL))
	{
		ShowPage(pPage);
		return false;
	}
	
	// tell all of the sub-dialogs "OK"
	for (i=0;i<m_pages.GetSize();i++)
	{
		pageStruct *pPS = (pageStruct *)m_pages.GetAt(i);
		ASSERT(pPS);
		if (pPS)
		{
			ASSERT(pPS->pDlg);
			if (pPS->pDlg)
			{
				if (::IsWindow(pPS->pDlg->m_hWnd))
				{
					pPS->pDlg->OnOK();
				}
			}
		}
	}
	
	return true;
}

/////////////////////////////////////////////////////////////////////////////

void CSAPrefsDialog::OnCancel() 
{
	// tell all of the sub-dialogs "Cancel"
	for (int i=0;i<m_pages.GetSize();i++)
	{
		pageStruct *pPS = (pageStruct *)m_pages.GetAt(i);
		ASSERT(pPS);
		
		if (pPS)
		{
			ASSERT(pPS->pDlg);
			if (pPS->pDlg)
			{
				if (::IsWindow(pPS->pDlg->m_hWnd))
				{
					pPS->pDlg->OnCancel();
				}
			}
		}
	}
	
	CDialog::OnCancel();
}


/////////////////////////////////////////////////////////////////////////////

void CSAPrefsDialog::EndSpecial(UINT res, bool bOk)
{
	if (bOk)
	{
		EndOK();
	}
	
	EndDialog(res);
}

/////////////////////////////////////////////////////////////////////////////

void CSAPrefsDialog::OnSelchangingPageTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	// do we need to validate this page?
	if (m_bValidateBeforeChangingPage)
	{
		// is the current page a real page?
		if ((m_iCurPage >= 0) && (m_iCurPage < m_pages.GetSize()))
		{
			pageStruct *pPS = (pageStruct *)m_pages.GetAt(m_iCurPage);
			if (pPS)
			{
				ASSERT(pPS->pDlg);
				if (pPS->pDlg)
				{
					if (::IsWindow(pPS->pDlg->m_hWnd))
					{
						// now we know we have a valid window. let's see if the data on it is OK
						if (!pPS->pDlg->UpdateData())
						{  
							// bad data!
							
							// when we leave here, the tree wil grab focus again.
							// this is bad, because we want the control that cause the UpdateData
							// to fail to have focus (so the user knows what to fix). so, we'll
							// do some trickery...

							// find out who has focus
							CWnd *pLost = pPS->pDlg->GetFocus();
							
							if (pLost)
							{
								// post a message back to this window with the HWND of the
								// window that had focus (the one that broke the UpdateData).
								// the handler for WM_SET_FOCUS_WND will set the focus back to 
								// that control
								PostMessage(WM_SET_FOCUS_WND, (UINT)pLost->m_hWnd);
							}

							// don't switch pages
							*pResult = 1;

							return;
						}

						// if we're here, the dialog validated OK. we can switch pages.
					}
				}
			}
		}
	}

	// ok to switch pages
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////

void CSAPrefsDialog::OnSelchangedPageTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	if (pNMTreeView->itemNew.lParam)
	{
		// find out which page was selected
		int iIdx = -1;
		for (int i=0;i<m_pages.GetSize();i++)
		{
			if (m_pages.GetAt(i)==(pageStruct *)pNMTreeView->itemNew.lParam)
			{
				iIdx = i;
				break;
			}
		}
		
		// show that page
		if ((iIdx >= 0) && (iIdx < m_pages.GetSize()))
		{
			pageStruct *pPS = (pageStruct *)m_pages.GetAt(iIdx);
			if (m_iCurPage!=iIdx)
			{
				PostMessage(WM_CHANGE_PAGE, iIdx);
			}
		}
	}
	
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////

void CSAPrefsDialog::OnGetdispinfoPageTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	
	// return the caption of the appropriate dialog
	if (pTVDispInfo->item.lParam)
	{
		if (pTVDispInfo->item.mask & TVIF_TEXT)
		{
			pageStruct *pPS = (pageStruct *)pTVDispInfo->item.lParam;
			strcpy(pTVDispInfo->item.pszText, pPS->csCaption);
		}
	}
	
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////

LONG CSAPrefsDialog::OnChangePage(UINT u, LONG l)
{
	if (ShowPage(u))
	{
		m_iCurPage = u;   
	}
	
	return 0L;
}

/////////////////////////////////////////////////////////////////////////////

LONG CSAPrefsDialog::OnSetFocusWnd(UINT u, LONG l)
{
	if (::IsWindow((HWND)u))
	{
		::SetFocus((HWND)u);
	}

	return 0L;
}

void CSAPrefsDialog::OnRestore() 
{
	if(MSGBOX(CGfAppString::GetString(IDS_RESTORE_CONFIG), MB_YESNOCANCEL|MB_ICONQUESTION)==IDYES)
	{
		CGf_AppStatic::_instance._bRestoreConfig=TRUE;
		AfxMessageBox(CGfAppString::GetString(IDS_RESTORED));
	}
	else
	{
		CGf_AppStatic::_instance._bRestoreConfig=FALSE;

	}
}
