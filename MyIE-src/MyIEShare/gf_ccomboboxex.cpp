// gf_ccomboboxex.cpp : implementation file
//

#include "stdafx.h"
#include "gf_ccomboboxex.h"
#include "macro.h"
#include "MainFrm.h"
#include "Resource.h"
#include "MyIE9.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGf_CComboBoxEx

CGf_CComboBoxEx::CGf_CComboBoxEx()
{
}

CGf_CComboBoxEx::~CGf_CComboBoxEx()
{
}


BEGIN_MESSAGE_MAP(CGf_CComboBoxEx, CComboBoxEx)
	//{{AFX_MSG_MAP(CGf_CComboBoxEx)
	ON_COMMAND(ID_REDIT_PASTE_ENTER, OnReditPasteEnter)
	ON_COMMAND(ID_REDIT_DELETE, OnReditDelete)
	ON_COMMAND(ID_REDIT_COPY, OnReditCopy)
	ON_COMMAND(ID_REDIT_UNDO, OnReditUndo)
	ON_UPDATE_COMMAND_UI(ID_REDIT_UNDO, OnUpdateReditUndo)
	ON_COMMAND(ID_REDIT_PASTE, OnReditPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGf_CComboBoxEx message handlers

void CGf_CComboBoxEx::OnReditPasteEnter() 
{
	this->GetEditCtrl()->SetWindowText("");

	this->GetEditCtrl()->Paste();

	Enter();
}

void CGf_CComboBoxEx::OnReditDelete() 
{
	this->SelAllWhenEmpty();
	this->GetEditCtrl()->ReplaceSel("", TRUE);
}

void CGf_CComboBoxEx::OnReditCopy() 
{
	this->SelAllWhenEmpty();
	this->GetEditCtrl()->Copy();
}

void CGf_CComboBoxEx::OnReditUndo() 
{
	this->GetEditCtrl()->Undo();
}

void CGf_CComboBoxEx::OnUpdateReditUndo(CCmdUI* pCmdUI) 
{
	
	if (this->GetEditCtrl()->CanUndo())
	{
		pCmdUI->Enable();
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
	
	
}

BOOL CGf_CComboBoxEx::PreTranslateMessage(MSG* pMsg) 
{
	if (WM_RBUTTONUP == pMsg->message)
	{
		this->GetEditCtrl()->SetFocus();

		CMenu menu;
		CMenu* pPopup;
		
		menu.LoadMenu(IDR_RMENU_EDIT);
		pPopup = menu.GetSubMenu(0);
		LOADMENU(pPopup,"REdit");
		pPopup->TrackPopupMenu(
			TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, 
			pMsg->pt.x, pMsg->pt.y, this
				);

		return TRUE;
	}
	return CComboBoxEx::PreTranslateMessage(pMsg);
}

void CGf_CComboBoxEx::Enter()
{

	this->GetEditCtrl()->SetFocus();
	keybd_event(VK_RETURN, MapVirtualKey(VK_RETURN, 0), 0, 0);
}

void CGf_CComboBoxEx::SelAllWhenEmpty()
{
	if (IsEmpty())
	{
		this->GetEditCtrl()->SetSel(0,-1);
		this->GetEditCtrl()->SetFocus();
	}


}

BOOL CGf_CComboBoxEx::IsEmpty()
{
	int nb,ne;
	this->GetEditCtrl()->GetSel(nb, ne);
	if (nb==ne)
	{
		return TRUE;
	}

	return FALSE;
}

void CGf_CComboBoxEx::OnReditPaste() 
{
	this->GetEditCtrl()->Paste();
	this->GetEditCtrl()->SetFocus();
}
