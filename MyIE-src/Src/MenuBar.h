////////////////////////////////////////////////////////////////
// Copyright 1998 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
#ifndef __MENUBAR_H
#define __MENUBAR_H

#include "ToolMenu.h"	// Added by ClassView


//////////////////
// CMenuBar implements an Office 97-style menu bar. Use it the way you would
// a CToolBar, only you need not call LoadToolbar. All you have to do is
//
// * Create the CMenuBar from your OnCreate or OnCreateBands handler.
//
// * Call LoadMenu to load a menu. This will set your frame's menu to NULL.
//
// * Implemenent your frame's PreTranslateMessage function, to call
//   CMenuBar::TranslateFrameMessage. 
//
class CMenuBar : public CToolBar {
public:
	void  OnMenuSelect(HMENU hmenu, UINT nItemID);
	HMENU					m_hmenu;				 // the menu
	void ChangeMenuOwnerDraw(CToolMenu *pMenu);
	void ChangeAllMenuOwnerDraw();
	int m_nMenuItems;
	CToolMenu submenu;
	CImageList m_ImgList;
	CImageList m_ImgMenu;
	CMenu m_Menu;
	BOOL MapAccelerator(TCHAR ch, UINT& nID);

	CMenuBar();
	~CMenuBar();

	// You must call this from your frame's PreTranslateMessage fn
	virtual BOOL TranslateFrameMessage(MSG* pMsg);

	HMENU LoadMenu(HMENU hmenu);				// load menu
	HMENU LoadMenu(LPCSTR lpszMenuName);	// ...from resource file
	HMENU LoadMenu(UINT nID) {
		return LoadMenu(MAKEINTRESOURCE(nID));
	}
	CMenu* GetMenu() 
	{
		if(m_Menu.m_hMenu==NULL)
			m_Menu.Attach(m_hmenu); 
		return &m_Menu; 
	}

	enum TRACKINGSTATE { // menubar has three states:
		TRACK_NONE = 0,   // * normal, not tracking anything
		TRACK_BUTTON,     // * tracking buttons (F10/Alt mode)
		TRACK_POPUP       // * tracking popups
	};

	TRACKINGSTATE GetTrackingState(int& iPopup) {
		iPopup = m_iPopupTracking; return m_iTrackingState;
	}
	static BOOL bTRACE;						 // set TRUE to see TRACE msgs

protected:
	CStringArray		m_arStrings;		 // array of menu item names

	int		m_iPopupTracking;				 // which popup I'm tracking if any
	int		m_iNewPopup;						 // next menu to track
	BOOL	m_bProcessRightArrow;			 // process l/r arrow keys?
	BOOL	m_bProcessLeftArrow;			 // ...
	BOOL	m_bEscapeWasPressed;			 // user pressed escape to exit menu
	CPoint	m_ptMouse;							 // mouse location when tracking popup

	TRACKINGSTATE m_iTrackingState;		 // current tracking state

	// helpers
	void	UpdateFont();
	int		GetNextOrPrevButton(int iButton, BOOL bPrev);
	void	SetTrackingState(TRACKINGSTATE iState, int iButton=-1);
	void	TrackPopup(int iButton);
	void	ToggleTrackButtonMode();
	void	CancelMenuAndTrackNewOne(int iButton);
	CPoint ComputeMenuTrackPoint(const CRect& rcButn, TPMPARAMS& tpm);

	BOOL	IsValidButton(int iButton) const
			{ return 0 <= iButton && iButton <m_nMenuItems ; }//GetToolBarCtrl().GetButtonCount()
	virtual BOOL OnMenuInput(MSG& m);	 // handle popup menu input

	// overrides
	int HitTest(CPoint p) const;

	// command/message handlers
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnUpdateMenuButton(CCmdUI* pCmdUI);
	afx_msg UINT OnNcHitTest(CPoint point);

	static LRESULT CALLBACK MenuInputFilter(int code, WPARAM wp, LPARAM lp);
	
	DECLARE_DYNAMIC(CMenuBar)
	DECLARE_MESSAGE_MAP()

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#endif
