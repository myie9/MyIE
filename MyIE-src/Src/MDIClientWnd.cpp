// MDIClientWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "MDIClientWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDIClientWnd

CMDIClientWnd::CMDIClientWnd()
{
}

CMDIClientWnd::~CMDIClientWnd()
{
}

      WNDPROC* CMDIClientWnd::GetSuperWndProcAddr() {

          static WNDPROC NEAR pfnSuper = NULL;
          return &pfnSuper;
      }

BEGIN_MESSAGE_MAP(CMDIClientWnd, CWnd)
	//{{AFX_MSG_MAP(CMDIClientWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMDIClientWnd message handlers



BOOL CMDIClientWnd::OnEraseBkgnd(CDC* pDC)
{
 
  // Save old brush
  HGDIOBJ pOldBrush = pDC->SelectObject(GetSysColorBrush(COLOR_WINDOW)); //GetStockObject(SYSTEM_FONT));

  CRect rect;
  //gf pDC->GetClipBox(&rect);     // Erase the area needed
  GetClientRect(&rect);

  pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(),
      PATCOPY);
  pDC->SelectObject(pOldBrush);
  return TRUE;
}
		