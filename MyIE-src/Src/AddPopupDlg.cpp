// AddPopupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "AddPopupDlg.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddPopupDlg dialog


CAddPopupDlg::CAddPopupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddPopupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddPopupDlg)
	m_strUrl = _T("");
	m_bClosePage = TRUE;
	//}}AFX_DATA_INIT
}


void CAddPopupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddPopupDlg)
	DDX_Text(pDX, IDC_URL, m_strUrl);
	DDX_Check(pDX, IDC_CLOSE_PAGE, m_bClosePage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddPopupDlg, CDialog)
	//{{AFX_MSG_MAP(CAddPopupDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddPopupDlg message handlers

BOOL CAddPopupDlg::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogAddToPopup");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogAddToPopup");
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
