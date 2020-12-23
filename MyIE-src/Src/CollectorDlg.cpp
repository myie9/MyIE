// CollectorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "CollectorDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCollectorDlg dialog

//#pragma optimize( "s", on)

#define MAX_COLLECTOR	1024*1024

CCollectorDlg::CCollectorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCollectorDlg::IDD, pParent)
{
	_bInited = FALSE;
	//{{AFX_DATA_INIT(CCollectorDlg)
	//}}AFX_DATA_INIT
}


void CCollectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCollectorDlg)
	DDX_Control(pDX, IDC_TXT, m_txt);
	DDX_Control(pDX, IDC_IMAGE, m_output);
	DDX_Control(pDX, IDC_FILTER, m_script);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCollectorDlg, CDialog)
	//{{AFX_MSG_MAP(CCollectorDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_COLLECTOR_TXT, OnCollectorTxt)
	ON_COMMAND(ID_COLLECTOR_OUTPUT, OnCollectorOutput)
	ON_COMMAND(ID_COLLECTOR_SCRIPT, OnCollectorScript)
	ON_COMMAND(ID_COLLECTOR_CLEAR, OnCollectorClean)
	ON_COMMAND(ID_COLLECTOR_SAVE, OnCollectorSave)
	ON_COMMAND(ID_COLLECTOR_TRANS, OnCollectorMode)
	ON_COMMAND(ID_RUN_SCRIPT, OnRunScript)
	ON_COMMAND(ID_OPEN_SELECTED_TEXT, OnOpenSelectedText)
	ON_COMMAND(ID_OPEN_CURSOR_LINE, OnOpenCursorLine)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCollectorDlg message handlers

void CCollectorDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	SaveCollector();
	CDialog::OnClose();
}

void CCollectorDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
	if (pmf->m_bOutputAutoClean)
		CleanOutputCollector();
	SaveCollector();
}


void CCollectorDlg::SaveCollector()
{
	CString txt;
	CFile f;
	m_txt.GetWindowText(txt);
	if(f.Open(theApp.m_strUser+"CollectorText.txt", CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
	{
		f.Write((void*)(LPCSTR)txt, txt.GetLength());
		f.Close();
	}
	//
	m_output.GetWindowText(txt);
	if(f.Open(theApp.m_strUser+"CollectorOutput.txt", CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
	{
		f.Write((void*)(LPCSTR)txt, txt.GetLength());
		f.Close();
	}
	//
	m_script.GetWindowText(txt);
	if(f.Open(theApp.m_strUser+"CollectorScript.txt", CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
	{
		f.Write((void*)(LPCSTR)txt, txt.GetLength());
		f.Close();
	}
}


void CCollectorDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (!_bInited)
	{
		return;
	}

	// TODO: Add your message handler code here
	m_txt.SetWindowPos(&CWnd::wndTop, 0,23, cx,cy-23, 0);
	m_output.SetWindowPos(&CWnd::wndTop, 0,23, cx,cy-23, 0);
	m_script.SetWindowPos(&CWnd::wndTop, 0,23, cx,cy-23, 0);
	m_ToolBar.SetWindowPos(&CWnd::wndTop, 0,0, cx,22, 0);
}


/*************************************************************
CTxtDropTarget
**************************************************************/

DROPEFFECT CTxtDropTarget::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point)
{
	return DROPEFFECT_SCROLL | DROPEFFECT_COPY | DROPEFFECT_LINK;
}

DROPEFFECT CTxtDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, 
                                      DWORD dwKeyState, CPoint point)
{
    DROPEFFECT dwEffect;
	dwEffect=DROPEFFECT_COPY;
    return dwEffect;
}

void CTxtDropTarget::OnDragLeave(CWnd* pWnd)
{
}

DROPEFFECT CTxtDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, 
                                     DWORD dwKeyState, CPoint point)
{
    DROPEFFECT dwEffect;
	dwEffect=DROPEFFECT_COPY;
    return dwEffect;
}

//type:
//0: text collector
//11: filter
//12: page
//13: img
void CTxtDropTarget::SaveText(CString str, int type)
{
	CString strTime;
	if (pmf->m_bOutputSaveTime)
	{
		SYSTEMTIME time;
		::GetLocalTime(&time);
		strTime.Format("[%02d/%02d-%02d:%02d:%02d] ",time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);
	}
	if (type==0)
	{
		if (strTime.GetLength())
			str = strTime + "\r\n" + str;
	}
	else
	{
		str = strTime + str;
		pmf->SetMessageText(str);
	}
	//
	str += "\r\n";
	CEdit *edit;
	CString strfile;
	//
	if (type==0)
	{
		edit = &(pmf->m_pCollectDlg->m_txt);
		strfile = theApp.m_strUser+"CollectorText.txt";
		if(pmf->m_pCollectDlg)
			pmf->m_pCollectDlg->OnCollectorTxt();
	}
	else
	{
		if (type==11 && !pmf->m_bOutputSaveFilter)
			return;
		else if (type==12 && !pmf->m_bOutputSavePage)
			return;
		else if (type==13 && !pmf->m_bOutputSaveImg)
			return;
		else if (type==14 && !pmf->m_bOutputSaveText)
			return;
		else if (type==15 && !pmf->m_bOutputSaveUrlFilter)
			return;
		edit = &(pmf->m_pCollectDlg->m_output);
		strfile = theApp.m_strUser+"CollectorOutput.txt";
		//gfmo default show txt tab  if(pmf->m_pCollectDlg)
			//gfmo default show txt tab  pmf->m_pCollectDlg->OnCollectorOutput();
	}
	//
	if(pmf->m_pCollectDlg)
	{
		int len = edit->GetWindowTextLength( );
		if (len>=MAX_COLLECTOR)
		{
			if (type==0)
				MSGBOX(IDS_MAX_COLLECTOR_TXT);
			else if ((type==1) || ((type>=11)&&(type<=15)))
			{
				edit->SetWindowText("");
				//MSGBOX(IDS_MAX_COLLECTOR_OUTPUT);
			}
			else
				MSGBOX(IDS_MAX_COLLECTOR_SCRIPT);
			return;
		}
		else
		{
			if (pmf->m_bCollectorAddtoBottom)
				edit->SetSel(len, len);
			else
				edit->SetSel(0,0);
			edit->ReplaceSel(str);
		}
	}
	else
	{
		CFile f;
		if(f.Open(strfile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyNone))
		{
			f.SeekToBegin();
			f.Write((void*)(LPCSTR)str, str.GetLength());
			f.Close();
		}
	}
}

BOOL CTxtDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
                           DROPEFFECT dropEffect, CPoint point)
{
	try{
	CString strMsg;
	//
    HGLOBAL hData=pDataObject->GetGlobalData(CF_TEXT);
    if (hData)
	{
		LPCSTR lpcszData=(LPCSTR)GlobalLock(hData);
		GlobalUnlock(hData);

		//if it is a link
		HGLOBAL hLinkData=pDataObject->GetGlobalData(RegisterClipboardFormat("FileGroupDescriptor"));
		LPCSTR lpcszLink;
		if(hLinkData)
		{
			lpcszLink=((LPCSTR)GlobalLock(hLinkData)) + 76;
			strMsg = lpcszLink;
			strMsg = strMsg.Left(strMsg.GetLength()-4);//cut .url of abc.url
			strMsg += " => ";
			strMsg += lpcszData;
			GlobalUnlock(hLinkData);
		}
		else
			strMsg += lpcszData;
		strMsg += "\r\n";
		SaveText(strMsg, 0);
	}
	else
	{
		hData=pDataObject->GetGlobalData(CF_HDROP);
		if (!hData) 
		{
			TRACE("Fail in getting data\n");
			MSGBOX(IDS_NOT_DROP_IMG);
			return FALSE;
		}

		HDROP hdrop =(HDROP)GlobalLock(hData);
		DragQueryFile(hdrop, 0, g_szFile, 1024);
		GlobalUnlock(hData);

		CString filename, rfile;
		filename = g_szFile;
		int l = filename.ReverseFind('\\');
		filename = filename.Mid(l+1);
		//fix filename, remove [x]
		l = filename.ReverseFind('[');
		int i = filename.ReverseFind(']');
		CString ext = filename.Mid(i+1);
		filename = filename.Left(l);
		//?
		CString imgPath = ((CMyIE9App*)AfxGetApp())->m_strImagePath;
		if(pmf->m_bImgCateWithSite)
		{
			CString site;
			CChildFrame* tcf = (CChildFrame*)pmf->MDIGetActive();
			site = tcf->m_pView->m_lpszUrl;
			pmf->GetSiteName(site);
			imgPath = imgPath + site + "\\";
		}
		//create dir
		_CreateFullDir(imgPath);
		filename = imgPath + filename;
		//compare file size
		DWORD sz1=0, sz2=0;
		HANDLE hfile = INVALID_HANDLE_VALUE;
		hfile = CreateFile(g_szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hfile != INVALID_HANDLE_VALUE)
			sz1 = GetFileSize(hfile, NULL);
		CloseHandle(hfile);
		hfile = INVALID_HANDLE_VALUE;
		hfile = CreateFile(filename+ext, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hfile != INVALID_HANDLE_VALUE)
			sz2 = GetFileSize(hfile, NULL);
		CloseHandle(hfile);

		rfile = filename + ext;
		if (sz1==0)//img not show
		{
			MSGBOX(IDS_SAVEIMAGEFAIL);
			return TRUE;
		}
		else if (sz1==sz2)//exist the same file
		{
			LOADSTR(strMsg ,IDS_IMG_EXISTED);
			strMsg += rfile;
			pmf->SetMessageText(strMsg);//SaveText(strMsg,13);
			return TRUE;
		}
		else if(sz1!=sz2 && sz2!=0)
		{
			char tmp[4];
			l = 0;
			HFILE hf; OFSTRUCT of;
			hf = OpenFile(rfile, &of, OF_EXIST);
			while(hf!=HFILE_ERROR)
			{
				l++;
				rfile = filename;
				rfile += '[';
				itoa(l, tmp, 10);
				rfile += tmp;
				rfile += ']';
				rfile += ext;
				hf = OpenFile(rfile, &of, OF_EXIST);
			}
		}
		
		//copy files
		if(!CopyFile(g_szFile, rfile, FALSE))
		{
			DWORD dwError = GetLastError();
			if(dwError == ERROR_PATH_NOT_FOUND)
				MSGBOX(IDS_SAVEIMAGEFAIL);
		}
		else
		{
			LOADSTR(strMsg ,IDS_IMG_SAVE_TO);
			strMsg += rfile;
			//
			CChildFrame* tcf = (CChildFrame*)pmf->MDIGetActive();
			strMsg += " <= ";
			strMsg += tcf->m_pView->m_lpszUrl;
			//
			SaveText(strMsg,13);
		}
	}

	}catch(...){}

    return TRUE;
}

//#pragma optimize( "s", off)

int CCollectorDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SAVE)),FALSE);
	return 0;
}

BOOL CCollectorDlg::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogCollector");
	this->OnCancel();
	return TRUE;
#endif
	LOADDLG("DialogCollector");
	CDialog::OnInitDialog();

	m_txt.SetLimitText(MAX_COLLECTOR);
	m_output.SetLimitText(MAX_COLLECTOR);
	m_script.SetLimitText(MAX_COLLECTOR);
	if (!m_ToolBar.CreateEx(this))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	// set up toolbar properties
	CImageList img;
	img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 8, 1);
	HBITMAP hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_COLLECTOR_TOOL));
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);
	m_ToolBar.GetToolBarCtrl().SetImageList(&img);
	img.Detach();
	//
	m_ToolBar.SetButtons(NULL, 12);
	m_ToolBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT| TBSTYLE_TOOLTIPS );
	m_ToolBar.ModifyStyleEx(0, WS_EX_TOOLWINDOW);
	m_ToolBar.SetBarStyle(m_ToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);
	// set up each toolbar button
	CString str;

	LOADSTR(str ,ID_COLLECTOR_TXT);
	m_ToolBar.SetButtonInfo(0, ID_COLLECTOR_TXT, TBSTYLE_BUTTON, 0);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_COLLECTOR_TXT);

	LOADSTR(str ,ID_COLLECTOR_OUTPUT);
	m_ToolBar.SetButtonInfo(1, ID_COLLECTOR_OUTPUT, TBSTYLE_BUTTON, 1);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_COLLECTOR_OUTPUT);

	LOADSTR(str ,ID_COLLECTOR_SCRIPT);
	m_ToolBar.SetButtonInfo(2, ID_COLLECTOR_SCRIPT, TBSTYLE_BUTTON, 2);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_COLLECTOR_SCRIPT);

	m_ToolBar.SetButtonInfo(3, 0, TBSTYLE_SEP, 0);

	LOADSTR(str ,ID_COLLECTOR_SAVE);
	m_ToolBar.SetButtonInfo(4, ID_COLLECTOR_SAVE, TBSTYLE_BUTTON, 4);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_COLLECTOR_SAVE);

	LOADSTR(str ,ID_COLLECTOR_CLEAR);
	m_ToolBar.SetButtonInfo(5, ID_COLLECTOR_CLEAR, TBSTYLE_BUTTON, 3);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_COLLECTOR_CLEAR);

	m_ToolBar.SetButtonInfo(6, 0, TBSTYLE_SEP, 0);

	LOADSTR(str ,ID_COLLECTOR_TRANS);
	m_ToolBar.SetButtonInfo(7, ID_COLLECTOR_TRANS, TBSTYLE_BUTTON, 5);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_COLLECTOR_TRANS);

	m_ToolBar.SetButtonInfo(8, 0, TBSTYLE_SEP, 0);

	LOADSTR(str ,ID_RUN_SCRIPT);
	m_ToolBar.SetButtonInfo(9, ID_RUN_SCRIPT, TBSTYLE_BUTTON, 6);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_RUN_SCRIPT);

	LOADSTR(str ,ID_OPEN_SELECTED_TEXT);
	m_ToolBar.SetButtonInfo(10, ID_OPEN_SELECTED_TEXT, TBSTYLE_BUTTON, 7);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_OPEN_SELECTED_TEXT);

	LOADSTR(str ,ID_OPEN_CURSOR_LINE);
	m_ToolBar.SetButtonInfo(11, ID_OPEN_CURSOR_LINE, TBSTYLE_BUTTON, 8);
	m_ToolBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(str, &m_ToolBar, ID_OPEN_CURSOR_LINE);

	m_nOldMode = 255;

	if (pmf->m_nCollectorTrans<255)
		CollectorTrans(pmf->m_nCollectorTrans);

	m_edit = 0;
	OnCollectorTxt();

	_bInited = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCollectorDlg::OnCollectorMode()
{
	if (pmf->m_nCollectorTrans>=255)
		return;
	if (m_nOldMode < 255)
		CollectorTrans(255);
	else
		CollectorTrans(pmf->m_nCollectorTrans);
}

void CCollectorDlg::CollectorSetTrans(int mode)
{
	if (!this->GetSafeHwnd())
		return;
	if (!CollectorTrans(mode))
		CollectorTrans(mode);//if not success, try again
}

int CCollectorDlg::CollectorTrans(int mode)
{
	BOOL bRet = FALSE;
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	HINSTANCE hInst = LoadLibrary("User32.DLL"); 
	if(hInst) 
	{ 
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL;
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if(fun)
		{
			bRet = fun(this->GetSafeHwnd(),0, mode, 2); 
			m_nOldMode = mode;
		}
		FreeLibrary(hInst); 
	}
	return bRet;
}

void CCollectorDlg::OnCollectorClean()
{
	if(MSGBOX(IDS_CLEAN_COLLECTOR, MB_YESNO|MB_ICONQUESTION)==IDYES)
	{
		m_edit->SetWindowText("");
		if (m_edit==&m_output)
		{
			pmf->m_nPopFiltered = 0;
			//pmf->UpdatePopFiltered();
		}
	}
}

void CCollectorDlg::CleanOutputCollector()
{
	m_output.SetWindowText("");
}

void CCollectorDlg::OnCollectorSave()
{
	CString txt;
	m_edit->GetWindowText(txt);
	if(txt.IsEmpty())
		return;
	//CFileDialog dlg(0);
	CString str;
	LOADSTR(str ,IDS_TYPE_COLLECTOR);
 	CFileDialog dlg(FALSE, "txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str);
	if (dlg.DoModal()==IDOK)
	{
		CFile f;
		if(f.Open(dlg.GetPathName(), CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		{
			f.Write((void*)(LPCSTR)txt, txt.GetLength());
			f.Close();
		}
	}
}

void CCollectorDlg::OnCollectorTxt()
{
	if (m_edit!=&m_txt)
	{
		m_output.ShowWindow(SW_HIDE);
		m_script.ShowWindow(SW_HIDE);
		m_txt.ShowWindow(SW_SHOW);
		m_ToolBar.GetToolBarCtrl().CheckButton(ID_COLLECTOR_TXT, 1);
		m_ToolBar.GetToolBarCtrl().CheckButton(ID_COLLECTOR_OUTPUT, 0);
		m_ToolBar.GetToolBarCtrl().CheckButton(ID_COLLECTOR_SCRIPT, 0);
		m_edit = &m_txt;
		m_edit->SetFocus();
	}
}

void CCollectorDlg::OnCollectorOutput()
{
	if (m_edit!=&m_output)
	{
		m_txt.ShowWindow(SW_HIDE);
		m_script.ShowWindow(SW_HIDE);
		m_output.ShowWindow(SW_SHOW);
		m_ToolBar.GetToolBarCtrl().CheckButton(ID_COLLECTOR_TXT, 0);
		m_ToolBar.GetToolBarCtrl().CheckButton(ID_COLLECTOR_OUTPUT, 1);
		m_ToolBar.GetToolBarCtrl().CheckButton(ID_COLLECTOR_SCRIPT, 0);
		m_edit = &m_output;
		if (IsWindowVisible())//if not visible, not rob the focus from page
			m_edit->SetFocus();
	}
}

void CCollectorDlg::OnCollectorScript()
{
	if (m_edit!=&m_script)
	{
		m_output.ShowWindow(SW_HIDE);
		m_txt.ShowWindow(SW_HIDE);
		m_script.ShowWindow(SW_SHOW);
		m_ToolBar.GetToolBarCtrl().CheckButton(ID_COLLECTOR_TXT, 0);
		m_ToolBar.GetToolBarCtrl().CheckButton(ID_COLLECTOR_OUTPUT, 0);
		m_ToolBar.GetToolBarCtrl().CheckButton(ID_COLLECTOR_SCRIPT, 1);
		m_edit = &m_script;
		m_edit->SetFocus();
	}
}

void CCollectorDlg::OnRunScript()
{
	// TODO: Add your message handler code here and/or call default
	int nStart, nEnd;

	m_script.GetSel(nStart,nEnd);
	if (nStart>=nEnd)
	{
		MSGBOX(IDS_NOT_SELECT_SCRIPT);
		return;
	}
	if (nStart>=0 && nEnd>=0)
	{
		CString strText, strCode;
		m_script.GetWindowText(strText);
		strCode = strText.Mid(nStart,nEnd-nStart);
		if (strCode.GetLength())
			pmf->RunScript(strCode);
	}
}

void CCollectorDlg::OnOpenSelectedText()
{
	// TODO: Add your message handler code here and/or call default
	int nStart, nEnd;

	m_edit->GetSel(nStart,nEnd);
	if (nStart>=nEnd)
	{
		MSGBOX(IDS_NOT_SELECT_TEXT);
		return;
	}
	if (nStart>=0 && nEnd>=0)
	{
		CString strText, strCode;
		m_edit->GetWindowText(strText);
		strCode = strText.Mid(nStart,nEnd-nStart);
		if (strCode.GetLength())
		{
			strCode.TrimLeft();strCode.TrimRight();
			if (!IS_URL(strCode))
				MSGBOX(IDS_NOT_VALID_URL);
			else
				pmf->NewChildWindow(1,2,strCode);
		}
	}
}

#define MAX_LINE_LENGTH	1030
void CCollectorDlg::OnOpenCursorLine()
{
	// TODO: Add your message handler code here and/or call default
	try{
	int nStart, nEnd, nCaret;
	m_edit->GetSel(nStart,nEnd);

	int row = m_edit->LineFromChar(nStart);
	nStart = m_edit->LineIndex(row);
	nCaret = nEnd - nStart;
	//
	char	strLine[MAX_LINE_LENGTH+1];
	int		nLineLength;
	nLineLength = m_edit->GetLine(row,strLine,MAX_LINE_LENGTH);
	strLine[nLineLength] = 0;
	if (strLine[nLineLength-1] == '\r')
		strLine[nLineLength-1] = 0;
	else if (strLine[nLineLength-2] == '\r')
		strLine[nLineLength-2] = 0;
	//
	CString strCode;
	strCode.Empty();
	char *p1 = strstr(strLine," => ");
	char *p2 = strstr(strLine," <= ");
	if (!p1)
		strCode = strLine;
	else if (!p2)
		strCode = p1 + 4;//move after " => "
	else
	{
		if (strLine + nCaret > p2)
			strCode = p2 + 4;//move after " <= "
		else
		{
			*p2 = 0;
			strCode = p1 + 4;//move after " => "
		}
	}
	//
	strCode.TrimLeft();strCode.TrimRight();
	if (!IS_URL(strCode))
		MSGBOX(IDS_NOT_VALID_URL);
	else
		pmf->NewChildWindow(1,2,strCode);

	}catch(...){}
}

BOOL CCollectorDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN )
	{ 
		if (PRESS_CTRL)
		{
			if (pMsg->wParam == 'A' )
			{
				m_edit->SetSel(0,-1);
				return TRUE;
			}
			if (pMsg->wParam == 'S' )
			{
				OnCollectorSave();
				return TRUE;
			}
		}
	}
	if ( pMsg->message == WM_LBUTTONDBLCLK )
	{
		if (pMsg->hwnd == m_output.m_hWnd)
		{
			OnOpenCursorLine();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CCollectorDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDblClk(nFlags, point);
}
