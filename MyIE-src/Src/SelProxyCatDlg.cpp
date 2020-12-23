// SelProxyCatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "SelProxyCatDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CSelProxyCatDlg dialog


CSelProxyCatDlg::CSelProxyCatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelProxyCatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelProxyCatDlg)
	m_bClear = FALSE;
	m_strCatName = _T("");
	m_SelCates = -1;
	//}}AFX_DATA_INIT
}


void CSelProxyCatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelProxyCatDlg)
	DDX_Control(pDX, IDC_CLEAR, m_ClearProxy);
	DDX_Control(pDX, IDC_PROXYCATS, m_Proxies);
	DDX_Check(pDX, IDC_CLEAR, m_bClear);
	DDX_CBString(pDX, IDC_PROXYCATS, m_strCatName);
	DDX_CBIndex(pDX, IDC_PROXYCATS, m_SelCates);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelProxyCatDlg, CDialog)
	//{{AFX_MSG_MAP(CSelProxyCatDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelProxyCatDlg message handlers

BOOL CSelProxyCatDlg::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogSelectProxy");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogSelectProxy");
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CProxyList * pl;
	for(int l = 0;l <= pmf->m_aProxyCategories.m_ProxyCategory.GetUpperBound();l++)
	{
		pl = pmf->m_aProxyCategories.m_ProxyCategory.GetAt(l);
		if(!pl->m_bIsWebProxy)
			m_Proxies.AddString(pl->m_strCategoryName);
	}

	m_Proxies.SetCurSel(0);

	if(m_bDisableClear)
		m_ClearProxy.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//#pragma optimize( "s", off )

