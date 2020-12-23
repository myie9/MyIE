// ProxyInfo.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "ProxyInfo.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProxyInfo dialog


CProxyInfo::CProxyInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CProxyInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProxyInfo)
	m_ProxyName = _T("");
	m_Proxy = _T("");
	m_Speed = 3;
	//}}AFX_DATA_INIT
}


void CProxyInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProxyInfo)
	DDX_Text(pDX, IDC_NAME, m_ProxyName);
	DDX_Text(pDX, IDC_PROXY, m_Proxy);
	DDX_CBIndex(pDX, IDC_SPEED, m_Speed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProxyInfo, CDialog)
	//{{AFX_MSG_MAP(CProxyInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProxyInfo message handlers

void CProxyInfo::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if (m_ProxyName.GetLength()==0)
	{
		MSGBOX(IDS_NAME_NULL);
		return;
	}
	else if (m_Proxy.GetLength()==0)
	{
		MSGBOX(IDS_URL_NULL);
		return;
	}
	CDialog::OnOK();
}

BOOL CProxyInfo::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogProxyInfo");
#endif
	CDialog::OnInitDialog();
#ifdef _WRITE_LNG_FILE_
	_WriteDlgComboBox(this,"DialogProxyInfo");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogProxyInfo");

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
