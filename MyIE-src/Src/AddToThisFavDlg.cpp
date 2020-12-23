// AddToThisFavDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "AddToThisFavDlg.h"

#include <Mshtmdid.h>
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddToThisFavDlg dialog


CAddToThisFavDlg::CAddToThisFavDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddToThisFavDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddToThisFavDlg)
	m_strFolder = _T("");
	m_strName = _T("");
	m_strURL = _T("");
	//}}AFX_DATA_INIT
}


void CAddToThisFavDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddToThisFavDlg)
	DDX_Control(pDX, IDC_COMMAND, m_command);
	DDX_Text(pDX, IDC_FOLDER, m_strFolder);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_URL, m_strURL);
	DDX_Control(pDX, IDC_USE_SPECIAL, m_UseSpecial);
	DDX_Control(pDX, IDC_DLACTIVEX, m_bDLActivex);
	DDX_Control(pDX, IDC_DLAPPLET, m_bDLApplet);
	DDX_Control(pDX, IDC_DLIMG, m_bDLImage);
	DDX_Control(pDX, IDC_DLJS, m_bDLScript);
	DDX_Control(pDX, IDC_DLSOUND, m_bDLSound);
	DDX_Control(pDX, IDC_DLVIDEO, m_bDLVideo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddToThisFavDlg, CDialog)
	//{{AFX_MSG_MAP(CAddToThisFavDlg)
	ON_BN_CLICKED(IDC_USE_SPECIAL, OnUseSpecial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddToThisFavDlg message handlers

BOOL CAddToThisFavDlg::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogAddToFav");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogAddToFav");
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_UseSpecial.SetCheck(FALSE);
	OnUseSpecial();
	//
	m_bDLImage.SetCheck(m_dwProperty & DLCTL_DLIMAGES);
	m_bDLSound.SetCheck(m_dwProperty & DLCTL_BGSOUNDS);
	m_bDLVideo.SetCheck(m_dwProperty & DLCTL_VIDEOS);
	m_bDLScript.SetCheck(!(m_dwProperty & DLCTL_NO_SCRIPTS));
	m_bDLActivex.SetCheck(!(m_dwProperty & DLCTL_NO_RUNACTIVEXCTLS));
	m_bDLApplet.SetCheck(!(m_dwProperty & DLCTL_NO_JAVA));
	//
	_LoadMenuString2ComboBox(pmf->GetMenu(),&m_command);
	//
	m_nAutoRunCmd = 0;
	m_command.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddToThisFavDlg::OnUseSpecial() 
{
	// TODO: Add your control notification handler code here
	BOOL bSpecial = m_UseSpecial.GetCheck();
	m_bDLImage.EnableWindow(bSpecial);
	m_bDLSound.EnableWindow(bSpecial);
	m_bDLVideo.EnableWindow(bSpecial);
	m_bDLScript.EnableWindow(bSpecial);
	m_bDLActivex.EnableWindow(bSpecial);
	m_bDLApplet.EnableWindow(bSpecial);
}

void CAddToThisFavDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (m_UseSpecial.GetCheck())
	{
		m_dwProperty = 0;
		if (m_bDLImage.GetCheck())
			m_dwProperty |= DLCTL_DLIMAGES; 
		if (m_bDLSound.GetCheck())
			m_dwProperty |=DLCTL_BGSOUNDS;
		if (m_bDLVideo.GetCheck())
			m_dwProperty |=DLCTL_VIDEOS;
		if (!m_bDLScript.GetCheck())
			m_dwProperty |= DLCTL_NO_SCRIPTS;
		if (!m_bDLActivex.GetCheck())
			m_dwProperty |= DLCTL_NO_RUNACTIVEXCTLS;
		if (!m_bDLApplet.GetCheck())
			m_dwProperty |= DLCTL_NO_JAVA;
	}
	else
		m_dwProperty = DEFAULT_PROPERTY;
	//
	int nCurSel = m_command.GetCurSel();
	if (nCurSel>0)
		m_nAutoRunCmd = m_command.GetItemData(nCurSel);
	else
		m_nAutoRunCmd = 0;

	CDialog::OnOK();
}
