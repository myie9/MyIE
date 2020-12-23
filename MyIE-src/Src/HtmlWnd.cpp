// HtmlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "HtmlWnd.h"
#include <ExDispID.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHtmlWnd

CHtmlWnd::CHtmlWnd()
{
}

CHtmlWnd::~CHtmlWnd()
{

}


BEGIN_MESSAGE_MAP(CHtmlWnd, CWebBrowser2)
	//{{AFX_MSG_MAP(CHtmlWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Manually added event sink map
BEGIN_EVENTSINK_MAP(CHtmlWnd, CWebBrowser2)
	ON_EVENT(CHtmlWnd, IDS_TITLE_SEARCH, DISPID_NEWWINDOW2, OnNewWindow2, VTS_PDISPATCH VTS_PBOOL)
	ON_EVENT(CHtmlWnd, IDS_TITLE_RESOURCE, DISPID_NEWWINDOW2, OnNewWindow2, VTS_PDISPATCH VTS_PBOOL)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHtmlWnd message handlers
void CHtmlWnd::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* bCancel)
{
	try{
	CMyIE9View* pv = CreateNewView();
	if(pv!=NULL)
		*ppDisp = pv->GetApplication();
	}catch(...){}
}

