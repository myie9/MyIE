// InputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "InputDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputDlg dialog


CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputDlg)
	m_strMsg = _T("");
	m_strOutput = _T("");
	//}}AFX_DATA_INIT
	m_nOKType = 0;
	m_bSave = FALSE;
}


void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputDlg)
	DDX_Control(pDX, IDC_OUTPUT, m_output);
	DDX_Text(pDX, IDC_MSG, m_strMsg);
	DDX_Text(pDX, IDC_OUTPUT, m_strOutput);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
	//{{AFX_MSG_MAP(CInputDlg)
	ON_BN_CLICKED(IDC_UP_URL, OnUpUrl)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputDlg message handlers

BOOL CInputDlg::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogInput");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogInput");
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CEdit *p = (CEdit*)GetDlgItem(IDC_OUTPUT);
	//
	CString str;
	if (!IS_URL(m_strOutput))
	{
		//GetDlgItem(IDC_UP_URL)->ShowWindow(FALSE);
		LOADSTR(str ,IDS_SAVE);
		GetDlgItem(IDC_UP_URL)->SetWindowText(str);
		m_bSave = TRUE;
	}
	if (m_nOKType)
	{
		LOADSTR(str ,ID_LL_OPEN_SELECT);
		GetDlgItem(IDOK)->SetWindowText(str);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputDlg::OnUpUrl() 
{
	// TODO: Add your control notification handler code here
	CString strUrl,strUrlUp;
	m_output.GetWindowText(strUrl);
	if (m_bSave)
	{
		if (strUrl.GetLength()<1)
			return;
		CString str;
		LOADSTR(str ,IDS_TYPE_COLLECTOR);
		CString strFileName;
		GET_FILE_NAME(strFileName,strUrl)
 		CFileDialog dlg(FALSE, "txt", strFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str);
		if (dlg.DoModal()==IDOK)
		{
			CFile f;
			if(f.Open(dlg.GetPathName(), CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
			{
				f.Write((void*)(LPCSTR)strUrl, strUrl.GetLength());
				f.Close();
			}
			OnCancel();
		}
	}
	else
	{
		//up url
		strUrlUp = _StringGetUpUrl(strUrl);
		if (strUrl != strUrlUp)
		{
			m_output.SetWindowText(strUrlUp);
			m_output.SetFocus();
			m_output.SetSel(0,-1);
		}
	}
}

void CInputDlg::OnCopy() 
{
	// TODO: Add your control notification handler code here
	int nStart, nEnd;
	CString strText, strCode;

	m_output.GetWindowText(strText);
	m_output.GetSel(nStart,nEnd);
	if (nStart<nEnd && nStart>=0 && nEnd>=0)
		strText = strText.Mid(nStart,nEnd-nStart);
	//
	_SetClipString(strText);
	OnCancel();
}
