#if !defined(AFX_TOOLMENU_H__E8777520_DE4C_11D3_84C7_0080450EA020__INCLUDED_)
#define AFX_TOOLMENU_H__E8777520_DE4C_11D3_84C7_0080450EA020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
/*
	This is our custom owner drawn menu that shows the hidden tool bar buttons
	with optional text
*/
BOOL DestroyMenuData(HMENU hMenu=NULL,int nStart=0);
BOOL DestroyMenuItemData(HMENU hMenu,int nPos, BOOL bPos);
BOOL DestroySubMenu(HMENU hMenu,HMENU hSubMenu);

class CToolMenu : public CMenu
{
public:
	CToolMenu();

	// The toolbar from which the bitmaps/icons have to be extracted
	CImageList	* m_pImg;

	// Overridables
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

public:
	BOOL DestroyMenu();
	virtual ~CToolMenu();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_TOOLMENU_H__E8777520_DE4C_11D3_84C7_0080450EA020__INCLUDED_)
