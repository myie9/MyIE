// CleanAll.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "CleanAll.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCleanAll dialog


CCleanAll::CCleanAll(CWnd* pParent /*=NULL*/)
	: CDialog(CCleanAll::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCleanAll)
	m_bCache = FALSE;
	m_bCookies = FALSE;
	m_bFormData = FALSE;
	m_bHistory = FALSE;
	m_bTypedAddress = FALSE;
	m_bTypedSearch = FALSE;
	m_bUndoClose = FALSE;
	//}}AFX_DATA_INIT
}


void CCleanAll::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCleanAll)
	DDX_Check(pDX, IDC_CLOSE_CLEAN_CACHE, m_bCache);
	DDX_Check(pDX, IDC_CLOSE_CLEAN_COOKIES, m_bCookies);
	DDX_Check(pDX, IDC_CLOSE_CLEAN_FORM_DATA, m_bFormData);
	DDX_Check(pDX, IDC_CLOSE_CLEAN_HISTORY, m_bHistory);
	DDX_Check(pDX, IDC_CLOSE_CLEAN_TYPED_ADDRESS, m_bTypedAddress);
	DDX_Check(pDX, IDC_CLOSE_CLEAN_TYPED_SEARCH, m_bTypedSearch);
	DDX_Check(pDX, IDC_CLOSE_CLEAN_UNDO_CLOSE, m_bUndoClose);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCleanAll, CDialog)
	//{{AFX_MSG_MAP(CCleanAll)
	ON_BN_CLICKED(IDC_SELECT_ALL, OnSelectAll)
	ON_BN_CLICKED(IDC_SELECT_NONE, OnSelectNone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCleanAll message handlers

void CCleanAll::OnSelectAll() 
{
	// TODO: Add your control notification handler code here
	m_bCache = TRUE;
	m_bCookies = TRUE;
	m_bFormData = TRUE;
	m_bHistory = TRUE;
	m_bTypedAddress = TRUE;
	m_bTypedSearch = TRUE;
	m_bUndoClose = TRUE;
	UpdateData(FALSE);
}

void CCleanAll::OnSelectNone() 
{
	// TODO: Add your control notification handler code here
	m_bCache = FALSE;
	m_bCookies = FALSE;
	m_bFormData = FALSE;
	m_bHistory = FALSE;
	m_bTypedAddress = FALSE;
	m_bTypedSearch = FALSE;
	m_bUndoClose = FALSE;
	UpdateData(FALSE);
}

#define IS_SELECTED(x)	(m_nCleanAllMode&x)/x
BOOL CCleanAll::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogCleanAll");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogCleanAll");
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bCookies = IS_SELECTED(1);
	m_bHistory = IS_SELECTED(2);
	m_bCache = IS_SELECTED(4);
	m_bFormData = IS_SELECTED(8);
	m_bTypedAddress = IS_SELECTED(16);
	m_bTypedSearch = IS_SELECTED(32);
	m_bUndoClose = IS_SELECTED(64);
	//
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCleanAll::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	m_nCleanAllMode = 0;
	if (m_bCookies)		m_nCleanAllMode |= 1;
	if (m_bHistory)		m_nCleanAllMode |= 2;
	if (m_bCache)		m_nCleanAllMode |= 4;
	if (m_bFormData)	m_nCleanAllMode |= 8;
	if (m_bTypedAddress) m_nCleanAllMode |= 16;
	if (m_bTypedSearch)	m_nCleanAllMode |= 32;
	if (m_bUndoClose)	m_nCleanAllMode |= 64;

	CDialog::OnOK();
}
