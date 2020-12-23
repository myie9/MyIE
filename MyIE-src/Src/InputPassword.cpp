// InputPassword.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "InputPassword.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CInputPassword dialog


CInputPassword::CInputPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CInputPassword::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputPassword)
	m_strPassword = _T("");
	m_bRemPsw = FALSE;
	//}}AFX_DATA_INIT
}


void CInputPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputPassword)
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	DDX_Check(pDX, IDC_REM_PSW, m_bRemPsw);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputPassword, CDialog)
	//{{AFX_MSG_MAP(CInputPassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputPassword message handlers

BOOL CInputPassword::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogPassword");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogPassword");
	CDialog::OnInitDialog();

	if (!m_bRemPsw)
		GetDlgItem(IDC_REM_PSW)->ShowWindow(FALSE);
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//#pragma optimize( "s", off)

