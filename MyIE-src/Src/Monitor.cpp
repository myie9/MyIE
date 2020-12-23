// Monitor.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "Monitor.h"
#include "MainFrm.h"
#include "CollectorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonitor dialog


CMonitor::CMonitor(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMonitor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonitor)
	DDX_Control(pDX, IDC_PERCENT, m_percent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMonitor, CDialog)
	//{{AFX_MSG_MAP(CMonitor)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_DROPFILES()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonitor message handlers

BOOL CMonitor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	ModifyStyleEx(0,WS_EX_TOPMOST);
	CollectorTrans( pmf->m_nMonitorTrans);
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMonitor::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonDown (nFlags, point );
	//PostMessage (WM_NCLBUTTONDOWN , HTCAPTION , MAKELPARAM (point.x, point. y) );
	SendMessage (WM_NCLBUTTONDOWN , HTCAPTION , MAKELPARAM (point.x, point. y) );
	pmf->SetForegroundWindow();
}

void CMonitor::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	pmf->SendMessage(WSM_TRAY,0,WM_RBUTTONDOWN);
}

void CMonitor::OnDropFiles(HDROP hDropInfo) 
{
	pmf->DropFiles(hDropInfo);
}

void CMonitor::CollectorSetTrans(int mode)
{
	if (this->GetSafeHwnd())
	{
		CollectorTrans(255);
		CollectorTrans(mode);
	}
}

void CMonitor::CollectorTrans(int mode)
{
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	HINSTANCE hInst = LoadLibrary("User32.DLL"); 
	if(hInst) 
	{ 
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL;
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if(fun)
		{
			fun(this->GetSafeHwnd(),0, mode, 2); 
		}
		FreeLibrary(hInst); 
	}
}

void CMonitor::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	pmf->SendMessage(WM_HOTKEY,NULL,NULL);
	CDialog::OnLButtonDblClk(nFlags, point);
}
