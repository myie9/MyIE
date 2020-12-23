// AddFlyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "AddFlyDlg.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddFlyDlg dialog


CAddFlyDlg::CAddFlyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddFlyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddFlyDlg)
	m_strUrl = _T("");
	m_bFilterCurrent = TRUE;
	//}}AFX_DATA_INIT
}


void CAddFlyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddFlyDlg)
	DDX_Text(pDX, IDC_URL, m_strUrl);
	DDX_Check(pDX, IDC_FILTER_FLY_PAGE, m_bFilterCurrent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddFlyDlg, CDialog)
	//{{AFX_MSG_MAP(CAddFlyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddFlyDlg message handlers

BOOL CAddFlyDlg::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogAddToFly");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogAddToFly");
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
