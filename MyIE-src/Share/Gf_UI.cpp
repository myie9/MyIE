// Gf_UI.cpp: implementation of the CGf_UI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyIE9.h"
#include "Gf_UI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_UI::CGf_UI()
{

}

CGf_UI::~CGf_UI()
{

}

void CGf_UI::SetIconForEmptyCombo(CComboBoxEx * pCombo, int nIcon)
{
	CString strMsg;
	pCombo->GetEditCtrl()->GetWindowText(strMsg);

	if (!strMsg.IsEmpty())
	{
		return;
	}

	COMBOBOXEXITEM comboitem;
	comboitem.mask = CBEIF_IMAGE|CBEIF_SELECTEDIMAGE ; //CBEIF_TEXT|
	comboitem.iImage = nIcon;
	comboitem.iSelectedImage = nIcon;
	comboitem.iItem = -1;
	//comboitem.pszText = (LPTSTR)(LPCTSTR)keyw;
	pCombo->SetItem(&comboitem);
	
}
