////////////////////////////////////////////////////////////////
// Copyright 1998 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
// CMenuBar implements menu bar for MFC. See MenuBar.h for how
// to use, and also the MBTest sample application.
//
#include "StdAfx.h"
#include "MenuBar.h"
#include "ToolMenu.h"
#include "MyIE9.h"
#include "function.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//##############################################################
#define countof(x)	(sizeof(x)/sizeof(x[0]))

//##############################################################
BOOL	g_bNoHook = FALSE;
//give WM_NCHITTEST a chance to handle before WM_MOUSEMOVE
int		g_nDelay = 10;

//##############################################################
extern	BOOL g_bRTab;

//##############################################################

IMPLEMENT_DYNAMIC(CMenuBar, CToolBar)

BEGIN_MESSAGE_MAP(CMenuBar, CToolBar)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_UPDATE_COMMAND_UI_RANGE(0, 256, OnUpdateMenuButton)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

CMenuBar::CMenuBar()
{
	m_iTrackingState = TRACK_NONE;		 // initial state: not tracking 
	m_iPopupTracking = m_iNewPopup = -1; // invalid
	m_hmenu = NULL;
	m_nMenuItems = 0;
	//
	g_bNoHook = FALSE;
	g_nDelay = 10;
}

CMenuBar::~CMenuBar()
{
	DestroyMenuData(m_hmenu,0);
}

//////////////////
// Menu bar was created: install hook into owner window
//
int CMenuBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CToolBar::OnCreate(lpCreateStruct)==-1)
		return -1;
	UpdateFont();
	return 0; // OK
}

//////////////////
// Load a different menu. The HMENU must not belong to any CMenu,
// and you must free it when you're done. Returns old menu.
//
HMENU CMenuBar::LoadMenu(HMENU hmenu)
{
	UINT iPrevID=(UINT)-1;
	ASSERT(::IsMenu(hmenu));
	ASSERT_VALID(this);

	HMENU hOldMenu = m_hmenu;
	m_hmenu = hmenu;

	// delete existing buttons
	int nCount = GetToolBarCtrl().GetButtonCount();
	while (nCount--) {
		VERIFY(GetToolBarCtrl().DeleteButton(0));
	}

	GetToolBarCtrl().SetImageList(NULL);

	DWORD dwStyle = GetStyle();
	BOOL bModifyStyle = ModifyStyle(0, TBSTYLE_FLAT|TBSTYLE_TRANSPARENT);

	// add text buttons
	UINT nMenuItems = hmenu ? ::GetMenuItemCount(hmenu) : 0;
	for (UINT i=0; i < nMenuItems; i++)
	{
		TCHAR name[64];
		memset(name, 0, sizeof(name)); // guarantees double-0 at end
		if (theApp.m_bUseLngFile)
		{
			char num[10];
			sprintf(num,"X%d",i);
			if (_GetPrivateProfileString("MenuMain", num, NULL, name, countof(name)-1, theApp.m_strLngFile)==FALSE)
				::GetMenuString(hmenu, i, name, countof(name)-1, MF_BYPOSITION);
		}
		else
			::GetMenuString(hmenu, i, name, countof(name)-1, MF_BYPOSITION);
		//
		TBBUTTON tbb;
		memset(&tbb, 0, sizeof(tbb));
		tbb.idCommand = ::GetMenuItemID(hmenu, i);

		// Because the toolbar is too brain-damaged to know if it already has
		// a string, and is also too brain-dead to even let you delete strings,
		// I have to determine if each string has been added already. Otherwise
		// in a MDI app, as the menus are repeatedly switched between doc and
		// no-doc menus, I will keep adding strings until somebody runs out of
		// memory. Sheesh!
		// 
		int iString = -1;
		for (int j=0; j<m_arStrings.GetSize(); j++) {
			if (m_arStrings[j] == name) {
				iString = j; // found it
				break;
			}
		}
		if (iString <0) {
			// string not found: add it
			iString = GetToolBarCtrl().AddStrings(name);
			m_arStrings.SetAtGrow(iString, name);
		}

		tbb.iString = iString;
		tbb.fsState = TBSTATE_ENABLED;
		tbb.fsStyle = TBSTYLE_AUTOSIZE;
		tbb.iBitmap = -1;
		tbb.idCommand = i;
		VERIFY(GetToolBarCtrl().AddButtons(1, &tbb));
	}

	if (bModifyStyle)
		SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	
	if (hmenu) {
		GetToolBarCtrl().AutoSize();// size buttons
		m_nMenuItems = GetToolBarCtrl().GetButtonCount();
	}
	return hOldMenu;
}

//////////////////
// Load menu from resource
//
HMENU CMenuBar::LoadMenu(LPCSTR lpszMenuName)
{
	return LoadMenu(::LoadMenu(AfxGetResourceHandle(), lpszMenuName));
}

//////////////////
// Set menu bar font from current system menu font
//
void CMenuBar::UpdateFont()
{
	static CFont font;
	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
	if ((HFONT)font)
		font.DeleteObject();
	VERIFY(font.CreateFontIndirect(&info.lfMenuFont));
	SetFont(&font);
}

//////////////////
// The reason for having this is so MFC won't automatically disable
// the menu buttons. Assumes < 256 top-level menu items. The ID of
// the ith menu button is i. IOW, the index and ID are the same.
//
void CMenuBar::OnUpdateMenuButton(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	if (IsValidButton(pCmdUI->m_nID))
		pCmdUI->Enable(TRUE);
}

//////////////////
// Set tracking state: none, button, or popup
//
void CMenuBar::SetTrackingState(TRACKINGSTATE iState, int iButton)
{
	ASSERT_VALID(this);
	if (iState != m_iTrackingState)
	{
		if (iState == TRACK_NONE)
			iButton = -1;
		GetToolBarCtrl().SetHotItem(iButton);// could be none (-1)
		if (iState==TRACK_POPUP)
		{
			// set related state stuff
			m_bEscapeWasPressed = FALSE;	 // assume Esc key not pressed
			m_bProcessRightArrow =			 // assume left/right arrow..
				m_bProcessLeftArrow = TRUE; // ..will move to prev/next popup
			m_iPopupTracking = iButton;	 // which popup I'm tracking
		}
		m_iTrackingState = iState;
	}
}

//////////////////
// Toggle state from home state to button-tracking and back
//
void CMenuBar::ToggleTrackButtonMode()
{
	ASSERT_VALID(this);
	if (m_iTrackingState == TRACK_NONE || m_iTrackingState == TRACK_BUTTON)
	{
		SetTrackingState(m_iTrackingState == TRACK_NONE ?
			TRACK_BUTTON : TRACK_NONE, 0);
	}
}

//////////////////
// Get button index before/after a given button
//
int CMenuBar::GetNextOrPrevButton(int iButton, BOOL bPrev)
{
	ASSERT_VALID(this);
	if (bPrev)
	{
		iButton--;
		if (iButton <0)
			iButton = m_nMenuItems - 1;//GetToolBarCtrl().GetButtonCount()
	}
	else
	{
		iButton++;
		if (iButton >= m_nMenuItems)//GetToolBarCtrl().GetButtonCount()
			iButton = 0;
	}
	return iButton;
}

/////////////////
// This is to correct a bug in the system toolbar control: TB_HITTEST only
// looks at the buttons, not the size of the window. So it returns a button
// hit even if that button is totally outside the size of the window!
//
int CMenuBar::HitTest(CPoint p) const
{
	int iHit = GetToolBarCtrl().HitTest(&p);
	if (iHit>0)
	{
		CRect rc;
		GetClientRect(&rc);
		if (!rc.PtInRect(p)) // if point is outside window
			iHit = -1;			// can't be a hit!
	}
	return iHit;
}

//////////////////
// Handle mouse click: if clicked on button, press it
// and go into main menu loop.
//
void CMenuBar::OnLButtonDown(UINT nFlags, CPoint pt)
{
	try{
	ASSERT_VALID(this);
	int iButton = HitTest(pt);
	if (iButton >= 0 && iButton<m_nMenuItems) //GetToolBarCtrl().GetButtonCount() if mouse is over a button:
		TrackPopup(iButton);								 //   track it
	else														 // otherwise:
		CToolBar::OnLButtonDown(nFlags, pt);	 //   pass it on...
	}catch(...){}
}

//////////////////
// Handle mouse movement
//
void CMenuBar::OnMouseMove(UINT nFlags, CPoint pt)
{
	ASSERT_VALID(this);

	if (m_iTrackingState==TRACK_BUTTON)
	{
		// In button-tracking state, ignore mouse-over to non-button area.
		// Normally, the toolbar would de-select the hot item in this case.
		// 
		// Only change the hot item if the mouse has actually moved.
		// This is necessary to avoid a bug where the user moves to a different
		// button from the one the mouse is over, and presses arrow-down to get
		// the menu, then Esc to cancel it. Without this code, the button will
		// jump to wherever the mouse is--not right.

		int iHot = HitTest(pt);
		if (IsValidButton(iHot) && pt != m_ptMouse)
			GetToolBarCtrl().SetHotItem(iHot);
		return;// don't let toolbar get it
	}
	m_ptMouse = pt; // remember point
	CToolBar::OnMouseMove(nFlags, pt);
}

/////////////////
// When user selects a new menu item, note whether it has a submenu
// and/or parent menu, so I know whether right/left arrow should
// move to the next popup.
//
void CMenuBar::OnMenuSelect(HMENU hmenu, UINT iItem)
{
	if (m_iTrackingState > 0)
	{
		// process right-arrow iff item is NOT a submenu
		m_bProcessRightArrow = (::GetSubMenu(hmenu, iItem) == NULL);
		// process left-arrow iff curent menu is one I'm tracking
		//m_bProcessLeftArrow = hmenu==m_hMenuTracking;
		//only when first level pop menu, left key can process by cmenubar
		m_bProcessLeftArrow = (::GetSubMenu(m_hmenu, m_iPopupTracking)==hmenu ? 1 : 0);
	}
}

// globals--yuk! But no other way using windows hooks.
//
static CMenuBar*	g_pMenuBar = NULL;
static HHOOK		g_hMsgHook = NULL;

////////////////
// Menu filter hook just passes to virtual CMenuBar function
//
LRESULT CALLBACK
CMenuBar::MenuInputFilter(int code, WPARAM wp, LPARAM lp)
{
	return (code==MSGF_MENU && g_pMenuBar &&
		g_pMenuBar->OnMenuInput(*((MSG*)lp))) ? TRUE
		: CallNextHookEx(g_hMsgHook, code, wp, lp);
}

//////////////////
// Handle menu input event: Look for left/right to change popup menu,
// mouse movement over over a different menu button for "hot" popup effect.
// Returns TRUE if message handled (to eat it).
//
BOOL CMenuBar::OnMenuInput(MSG& m)
{
	ASSERT_VALID(this);
	ASSERT(m_iTrackingState == TRACK_POPUP); // sanity check
	int msg = m.message;

	if (msg==WM_KEYDOWN)
	{
		// handle left/right-arow.
		TCHAR vkey = m.wParam;
		if ((vkey == VK_LEFT  && m_bProcessLeftArrow) || (vkey == VK_RIGHT && m_bProcessRightArrow))
		{
			CancelMenuAndTrackNewOne( GetNextOrPrevButton(m_iPopupTracking, vkey==VK_LEFT));
			return TRUE; // eat it
		}
		else if (vkey == VK_ESCAPE)
		{
			m_bEscapeWasPressed = TRUE;	 // (menu will abort itself)
		}
	}
	else if (msg==WM_MOUSEMOVE || msg==WM_LBUTTONDOWN)
	{
		// handle mouse move or click
		CPoint pt = m.lParam;
		ScreenToClient(&pt);

		if((msg==WM_MOUSEMOVE || msg==WM_LBUTTONDOWN) && g_nDelay>=0)
			g_nDelay--;
		if (msg == WM_MOUSEMOVE && !g_bNoHook && g_nDelay<0) 
		{
			if (pt != m_ptMouse ) 
			{
				int iButton = HitTest(pt);
				if (IsValidButton(iButton) && iButton != m_iPopupTracking) 
				{
					if (::WindowFromPoint((CPoint)m.lParam)==this->m_hWnd)
					{
						// user moved mouse over a different button: track its popup
						CancelMenuAndTrackNewOne(iButton);
						m_iPopupTracking = iButton;
					}
				}
				m_ptMouse = pt;
			}
		}
		else if (msg == WM_LBUTTONDOWN && !g_bNoHook && g_nDelay<0) 
		{
			if (HitTest(pt) == m_iPopupTracking) 
			{
				if (::WindowFromPoint((CPoint)m.lParam)==this->m_hWnd)
				{
					// user clicked on same button I am tracking: cancel menu
					CancelMenuAndTrackNewOne(-1);
					return TRUE; // eat it
				}
			}
		}
	}
	return FALSE; // not handled
}

//////////////////
// Cancel the current popup menu by posting WM_CANCELMODE, and track a new
// menu. iNewPopup is which new popup to track (-1 to quit).
//
void CMenuBar::CancelMenuAndTrackNewOne(int iNewPopup)
{
	ASSERT_VALID(this);
	if (iNewPopup != m_iPopupTracking)
	{
		AfxGetApp()->m_pMainWnd->PostMessage(WM_CANCELMODE); // quit menu loop
		m_iNewPopup = iNewPopup;					 // go to this popup (-1 = quit)
	}
}

//////////////////
// Track the popup submenu associated with the i'th button in the menu bar.
// This fn actually goes into a loop, tracking different menus until the user
// selects a command or exits the menu.
//
void CMenuBar::TrackPopup(int iButton)
{
	ASSERT_VALID(this);
	ASSERT(m_hmenu);

	g_bRTab = FALSE;
	CMenu menu;
	menu.Attach(m_hmenu);
	int nMenuItems = menu.GetMenuItemCount();
	
	while (iButton >= 0)// while user selects another menu
	{					 
		m_iNewPopup = -1;						 // assume quit after this
		GetToolBarCtrl().PressButton(iButton, TRUE);		 // press the button
		UpdateWindow();						 // and force repaint now

		// post a simulated arrow-down into the message stream
		// so TrackPopupMenu will read it and move to the first item
		AfxGetApp()->m_pMainWnd->PostMessage(WM_KEYDOWN, VK_DOWN, 1);
		AfxGetApp()->m_pMainWnd->PostMessage(WM_KEYUP, VK_DOWN, 1);

		SetTrackingState(TRACK_POPUP, iButton); // enter tracking state

		// Need to install a hook to trap menu input in order to make
		// left/right-arrow keys and "hot" mouse tracking work.
		//
		g_pMenuBar = this;
		g_hMsgHook = SetWindowsHookEx(WH_MSGFILTER,
			MenuInputFilter, NULL, ::GetCurrentThreadId());
		if(g_hMsgHook==NULL)
			g_hMsgHook = SetWindowsHookEx(WH_MSGFILTER,
				MenuInputFilter, NULL, ::GetCurrentThreadId());
		// get submenu and display it beneath button
		TPMPARAMS tpm;
		CRect rcButton;
		GetToolBarCtrl().GetRect(iButton, rcButton);
		ClientToScreen(&rcButton);
		CPoint pt = ComputeMenuTrackPoint(rcButton, tpm);
		HMENU hMenuPopup = ::GetSubMenu(m_hmenu, iButton);
		ASSERT(hMenuPopup);

		//CToolMenu submenu;
		submenu.Detach();
		submenu.m_pImg = &m_ImgList;
		submenu.Attach(hMenuPopup);
		BOOL bRet = submenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_VERTICAL,pt.x, pt.y, AfxGetApp()->m_pMainWnd);

		// uninstall hook.
		::UnhookWindowsHookEx(g_hMsgHook);
		g_hMsgHook = NULL;
		g_pMenuBar = NULL;
		
		GetToolBarCtrl().PressButton(iButton, FALSE);	 // un-press button
		UpdateWindow();					 // and force repaint now

		// If the user exited the menu loop by pressing Escape,
		// return to track-button state; otherwise normal non-tracking state.
		SetTrackingState(m_bEscapeWasPressed ?
			TRACK_BUTTON : TRACK_NONE, iButton);

		// If the user moved mouse to a new top-level popup (eg from File to
		// Edit button), I will have posted a WM_CANCELMODE to quit
		// the first popup, and set m_iNewPopup to the new menu to show.
		// Otherwise, m_iNewPopup will be -1 as set above.
		// So just set iButton to the next popup menu and keep looping...
		iButton = m_iNewPopup;
		//submenu.Detach();
	}
	menu.Detach();
}

//////////////////
// Given button rectangle, compute point and "exclude rect" for
// TrackPopupMenu, based on current docking style, so that the menu will
// appear always inside the window.
//
CPoint CMenuBar::ComputeMenuTrackPoint(const CRect& rcButn, TPMPARAMS& tpm)
{
	tpm.cbSize = sizeof(tpm);
	DWORD dwStyle = m_dwStyle;
	CPoint pt;
	CRect& rcExclude = (CRect&)tpm.rcExclude;
	rcExclude = rcButn;
	::GetWindowRect(::GetDesktopWindow(), &rcExclude);

	return CPoint(rcButn.left, rcButn.bottom);
}

//////////////////
// This function translates special menu keys and mouse actions.
// You must call it from your frame's PreTranslateMessage.
//
BOOL CMenuBar::TranslateFrameMessage(MSG* pMsg)
{
	ASSERT_VALID(this);
	ASSERT(pMsg);

	try{
	UINT msg = pMsg->message;
	if (WM_LBUTTONDOWN <= msg && msg <= WM_MOUSELAST)
	{
		if (pMsg->hwnd != m_hWnd && m_iTrackingState > 0)
		{
			// user clicked outside menu bar: exit tracking mode
			SetTrackingState(TRACK_NONE);
		}
	}
	else if (msg==WM_SYSKEYDOWN || msg==WM_SYSKEYUP || msg==WM_KEYDOWN)
	{
		BOOL bAlt = HIWORD(pMsg->lParam) & KF_ALTDOWN; // Alt key down
		TCHAR vkey = pMsg->wParam;							  // get virt key
		
		// key is VK_MENU or F10 with no alt/ctrl/shift: toggle menu mode
		if (vkey==VK_MENU )
		{
			// key is VK_MENU : toggle menu mode
			if (msg==WM_SYSKEYUP)
			{
				ToggleTrackButtonMode();
			}
			return TRUE;

		}
		else if ((msg==WM_SYSKEYDOWN || msg==WM_KEYDOWN))
		{
			if (m_iTrackingState == TRACK_BUTTON)
			{
				// I am tracking: handle left/right/up/down/space/Esc
				switch (vkey)
				{
				case VK_LEFT:
				case VK_RIGHT:
					// left or right-arrow: change hot button if tracking buttons
					GetToolBarCtrl().SetHotItem(GetNextOrPrevButton(GetToolBarCtrl().GetHotItem(), vkey==VK_LEFT));
					return TRUE;

				case VK_SPACE:  // (personally, I like SPACE to enter menu too)
				case VK_UP:
				case VK_DOWN:
					// up or down-arrow: move into current menu, if any
					TrackPopup(GetToolBarCtrl().GetHotItem());
					return TRUE;

				case VK_ESCAPE:
					// escape key: exit tracking mode
					SetTrackingState(TRACK_NONE);
					return TRUE;
				}
			}
			// Handle alphanumeric key: invoke menu. Note that Alt-X
			// chars come through as WM_SYSKEYDOWN, plain X as WM_KEYDOWN.
			if ((bAlt || m_iTrackingState == TRACK_BUTTON) && isalnum(vkey))
			{
				// Alt-X, or else X while in tracking mode
				UINT nID;
				if (MapAccelerator(vkey, nID))
				{
					TrackPopup(nID);	 // found menu mnemonic: track it
					return TRUE;		 // handled
				}
				else if (m_iTrackingState==TRACK_BUTTON && !bAlt)
				{
					return TRUE;
				}
			}
			// Default for any key not handled so far: return to no-menu state
			if (m_iTrackingState > 0)
			{
				SetTrackingState(TRACK_NONE);
			}
		}
	}
	}catch(...){}

	return FALSE; // not handled, pass along
}

#ifdef _DEBUG
void CMenuBar::AssertValid() const
{
}

void CMenuBar::Dump(CDumpContext& dc) const
{
}
#endif

BOOL CMenuBar::MapAccelerator(TCHAR ch, UINT & nID)
{
	return SendMessage(TB_MAPACCELERATOR, (WPARAM)ch, (LPARAM)&nID); 
}

UINT CMenuBar::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_iTrackingState == TRACK_POPUP)
	{
		ScreenToClient(&point);
		int iButton = HitTest(point);
		if (IsValidButton(iButton) && iButton != m_iPopupTracking)
		{
			// user moved mouse over a different button: track its popup
			CancelMenuAndTrackNewOne(iButton);
			m_iPopupTracking = iButton;
		}
		m_ptMouse = point;
	}

	return CToolBar::OnNcHitTest(point);
}
