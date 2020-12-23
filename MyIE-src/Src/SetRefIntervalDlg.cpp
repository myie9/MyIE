// SetRefIntervalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "SetRefIntervalDlg.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CSetRefIntervalDlg dialog


CSetRefIntervalDlg::CSetRefIntervalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetRefIntervalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetRefIntervalDlg)
	m_nRefInt = 0;
	//}}AFX_DATA_INIT
}


void CSetRefIntervalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetRefIntervalDlg)
	DDX_Text(pDX, IDC_REFINTERVAL, m_nRefInt);
	DDV_MinMaxUInt(pDX, m_nRefInt, 1, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetRefIntervalDlg, CDialog)
	//{{AFX_MSG_MAP(CSetRefIntervalDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetRefIntervalDlg message handlers

//#pragma optimize( "s", off )


BOOL CSetRefIntervalDlg::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogRefresh");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogRefresh");
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
