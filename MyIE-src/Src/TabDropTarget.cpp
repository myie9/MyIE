// TabDropTarget.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "TabDropTarget.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on)

/////////////////////////////////////////////////////////////////////////////
// CTabDropTarget

/*************************************************************
CTabDropTarget
**************************************************************/
BOOL CTabDropTarget::Register(CTabCtrl* pCtrl)
{
    ms_pCtl=pCtrl;
    return COleDropTarget::Register(pCtrl);
}

DROPEFFECT CTabDropTarget::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point)
{
	return DROPEFFECT_COPY | DROPEFFECT_LINK;
}

DROPEFFECT CTabDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, 
                                      DWORD dwKeyState, CPoint point)
{
	DROPEFFECT dwEffect;
	dwEffect=DROPEFFECT_COPY;

    return dwEffect;
}

void CTabDropTarget::OnDragLeave(CWnd* pWnd)
{
}

DROPEFFECT CTabDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, 
                                     DWORD dwKeyState, CPoint point)
{
	DROPEFFECT dwEffect;    
	dwEffect=DROPEFFECT_COPY;

	return dwEffect;
}

BOOL CTabDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
                           DROPEFFECT dropEffect, CPoint point)
{
    HGLOBAL hData=pDataObject->GetGlobalData(CF_TEXT);
    if (!hData) {
        TRACE("Fail in getting data\n");
        return FALSE;
    }
    LPCSTR lpcszData=(LPCSTR)GlobalLock(hData);
	CString str2(lpcszData), url;
    GlobalUnlock(hData);

	//if the protocol is javascript then ignore
	if(IS_SCRIPT_MAILTO(str2))
		return FALSE;

	//check alias
	if(pmf->m_bUseAlias)
		if(pmf->m_mapAlias.Lookup(str2, url))
			str2 = url;

	TCHITTESTINFO hti;
	hti.pt = point;
	hti.flags = TCHT_ONITEM | TCHT_ONITEMICON | TCHT_ONITEMLABEL ;
	int nTabID = ms_pCtl->HitTest(&hti);

	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	TabCtrlItem.lParam = NULL;

	ms_pCtl->GetItem(nTabID, &TabCtrlItem);
	if(TabCtrlItem.lParam != NULL)
	{
		CChildFrame* tcf = ((CChildFrame*)TabCtrlItem.lParam);
		tcf->m_pView->ToNavigate(str2,0,NULL);
	}
	else
	{
		CMyIE9View* pvw = CreateNewView();
		if(pvw!=NULL)
		{
			pvw->ToNavigate(str2,0,NULL);
		}
	}

    return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CTabDropTarget commands

//#pragma optimize( "s", off)
