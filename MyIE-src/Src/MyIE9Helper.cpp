// MyIE9Helper.cpp: implementation of the CMyIE9Helper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyIE9.h"
#include "MyIE9Helper.h"
#include "MyIE9View.h"
#include "ChildFrm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyIE9View * CreateNewView()
{
    CWinApp* pApp = AfxGetApp();
 
	CChildFrame *pWnd;
	pWnd = ((CMainFrame*)pApp->m_pMainWnd)->NewChildWindow(0);

	return pWnd->m_pView;
}

CMyIE9View* GetCurrentView(int nFlag, BOOL bInNewWindow)
{
	CChildFrame* tcf = (CChildFrame*)((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	if(tcf!=NULL && !bInNewWindow && !PRESS_CTRL)
	{
		return (CMyIE9View*)tcf->m_pView;
	}
	else
	{
		if(nFlag == 1 || bInNewWindow || PRESS_CTRL) //create new 
			return CreateNewView();
		return NULL;
	}
}
