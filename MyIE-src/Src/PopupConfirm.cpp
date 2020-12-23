// PopupConfirm.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "PopupConfirm.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPopupConfirm dialog


CPopupConfirm::CPopupConfirm(CWnd* pParent /*=NULL*/)
	: CDialog(CPopupConfirm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPopupConfirm)
	m_strURL = _T("");
	m_bNotConfirmPopupFilter = FALSE;
	m_bAddToPopList = FALSE;
	//}}AFX_DATA_INIT
}


void CPopupConfirm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPopupConfirm)
	DDX_Control(pDX, IDC_URL, m_url);
	DDX_Text(pDX, IDC_URL, m_strURL);
	DDX_Check(pDX, IDC_NOT_POP_CONFIRM, m_bNotConfirmPopupFilter);
	DDX_Check(pDX, IDC_ADDTO_POP_LIST, m_bAddToPopList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPopupConfirm, CDialog)
	//{{AFX_MSG_MAP(CPopupConfirm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopupConfirm message handlers

BOOL CPopupConfirm::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogPopupConfirm");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogPopupConfirm");
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
