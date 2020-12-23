// GroupUrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "GroupUrlDlg.h"

#include <Mshtmdid.h>
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma optimize( "s", on )


/////////////////////////////////////////////////////////////////////////////
// CGroupUrlDlg dialog
typedef HRESULT (CALLBACK* LPFNDLLFUNC1)(HWND ,DWORD);


CGroupUrlDlg::CGroupUrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGroupUrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroupUrlDlg)
	m_strName = _T("");
	m_strUrl = _T("");
	m_bState = TRUE;
	//}}AFX_DATA_INIT
	m_dwProperty = DEFAULT_PROPERTY;
}


void CGroupUrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroupUrlDlg)
	DDX_Control(pDX, IDC_USE_SPECIAL, m_UseSpecial);
	DDX_Control(pDX, IDC_NAME, m_edName);
	DDX_Control(pDX, IDC_URL, m_edURL);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_URL, m_strUrl);
	DDX_Check(pDX, IDC_STATE, m_bState);
	DDX_Control(pDX, IDC_DLACTIVEX, m_bDLActivex);
	DDX_Control(pDX, IDC_DLAPPLET, m_bDLApplet);
	DDX_Control(pDX, IDC_DLIMG, m_bDLImage);
	DDX_Control(pDX, IDC_DLJS, m_bDLScript);
	DDX_Control(pDX, IDC_DLSOUND, m_bDLSound);
	DDX_Control(pDX, IDC_DLVIDEO, m_bDLVideo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroupUrlDlg, CDialog)
	//{{AFX_MSG_MAP(CGroupUrlDlg)
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	ON_EN_CHANGE(IDC_URL, OnChangeUrl)
	ON_BN_CLICKED(IDC_USE_SPECIAL, OnUseSpecial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupUrlDlg message handlers

void CGroupUrlDlg::OnChangeName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(!m_bNotEmpty)
		return;

	CString name;
	m_edName.GetWindowText(name);
	name.TrimLeft();
	if(name.IsEmpty())
		m_btnOK.EnableWindow(0);
	else
	{
		if(!m_btnOK.IsWindowEnabled())
			m_btnOK.EnableWindow();
	}
}

void CGroupUrlDlg::OnChangeUrl() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString url;
	m_edURL.GetWindowText(url);
	url.TrimLeft();
	if(url.IsEmpty())
		m_btnOK.EnableWindow(0);
	else
	{
		if(!m_btnOK.IsWindowEnabled())
			m_btnOK.EnableWindow();
	}	
}

BOOL CGroupUrlDlg::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogGroupUrl");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogGroupUrl");
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	try{
	if(m_strUrl.IsEmpty())
		m_btnOK.EnableWindow(0);
	//
	BOOL bSpecial;
	bSpecial = m_dwProperty==DEFAULT_PROPERTY ? false : true;
	m_UseSpecial.SetCheck(bSpecial);
	OnUseSpecial();
	if (!bSpecial)
		m_dwProperty = pmf->m_dwDefaultProperty;
	//
	m_bDLImage.SetCheck(m_dwProperty & DLCTL_DLIMAGES);
	m_bDLSound.SetCheck(m_dwProperty & DLCTL_BGSOUNDS);
	m_bDLVideo.SetCheck(m_dwProperty & DLCTL_VIDEOS);
	m_bDLScript.SetCheck(!(m_dwProperty & DLCTL_NO_SCRIPTS));
	m_bDLActivex.SetCheck(!(m_dwProperty & DLCTL_NO_RUNACTIVEXCTLS));
	m_bDLApplet.SetCheck(!(m_dwProperty & DLCTL_NO_JAVA));
	}catch(...){}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//#pragma optimize( "s", off)

void CGroupUrlDlg::OnUseSpecial() 
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

void CGroupUrlDlg::OnOK() 
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
	CDialog::OnOK();
}
