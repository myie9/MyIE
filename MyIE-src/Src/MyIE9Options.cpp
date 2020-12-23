// MyIE9Options.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE9.h"
#include "MyIE9Options.h"
#include "MainFrm.h"
#include "InputDlg.h"

#include "Gf_AppStatic.h"
#include "PathHelper.h"
#include "RegHelper.h"
#include "GfAppString.h"
#include "Gf_Win.h"
#include "Gf_Config.h"
#include "Consts_gf.h"
#include "Gf_StartPageChoice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_MOUSE_ACTION		7 //gf 6

extern BOOL	g_bRegisterHotKeyOk;

//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CWindowProp property page

IMPLEMENT_DYNCREATE(CWindowProp, CSAPrefsSubDlg)

CWindowProp::CWindowProp() : CSAPrefsSubDlg(CWindowProp::IDD)
{
	//{{AFX_DATA_INIT(CWindowProp)
	m_bActiveNew = FALSE;
	m_nInterval = 0;
	m_nMaxWins = 0;
	m_bOpenFolderOut = FALSE;
	m_nCollectorTrans = 0;
	m_nMonitorTrans = 0;
	m_nOpenNew = -1;
	m_nNewSequence = -1;
	m_nCloseSequence = -1;
	m_nNewAddress = FALSE;
	m_bNewFav = FALSE;
	m_bNewHistory = FALSE;
	m_bNewLinks = FALSE;
	m_bNewLocal = FALSE;
	m_bActiveFileNew = FALSE;
	m_bAutoRefreshActive = FALSE;
	m_bKeepOnePage = FALSE;
	m_bBlankMonitorClip = FALSE;
	m_bActiveNewAddress = FALSE;
	m_bNewHome = FALSE;
	m_bUseTargetFrameName = FALSE;
	m_bReloadUnShownImage = FALSE;
	//}}AFX_DATA_INIT
}

CWindowProp::~CWindowProp()
{
}

void CWindowProp::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindowProp)
	DDX_Control(pDX, IDC_SPIN4, m_spin4);
	DDX_Control(pDX, IDC_SPIN3, m_spin3);
	DDX_Control(pDX, IDC_SPIN2, m_spin2);
	DDX_Control(pDX, IDC_SPIN1, m_spin1);
	DDX_Check(pDX, IDC_ACTIVATE_NEWWIN, m_bActiveNew);
	DDX_Text(pDX, IDC_INTERVAL, m_nInterval);
	DDX_Text(pDX, IDC_MAXWIN, m_nMaxWins);
	DDX_Check(pDX, IDC_OPEN_FOLDER_OUT, m_bOpenFolderOut);
	DDX_Text(pDX, IDC_TRANS, m_nCollectorTrans);
	DDV_MinMaxInt(pDX, m_nCollectorTrans, 5, 255);
	DDX_Text(pDX, IDC_MONITOR_TRANS, m_nMonitorTrans);
	DDV_MinMaxInt(pDX, m_nMonitorTrans, 5, 255);
	DDX_CBIndex(pDX, IDC_BLANKWIN, m_nOpenNew);
	DDX_CBIndex(pDX, IDC_NEW_SEQUENCE, m_nNewSequence);
	DDX_CBIndex(pDX, IDC_CLOSE_SEQUENCE, m_nCloseSequence);
	DDX_Check(pDX, ID_NEW_ADDRESS, m_nNewAddress);
	DDX_Check(pDX, ID_NEW_FAV, m_bNewFav);
	DDX_Check(pDX, ID_NEW_HISTORY, m_bNewHistory);
	DDX_Check(pDX, ID_NEW_LINKS, m_bNewLinks);
	DDX_Check(pDX, ID_NEW_LOCAL, m_bNewLocal);
	DDX_Check(pDX, ID_ACTIVE_FILE_NEW, m_bActiveFileNew);
	DDX_Check(pDX, IDC_AUTO_REFRESH_ACTIVE, m_bAutoRefreshActive);
	DDX_Check(pDX, IDC_KEEP_ONEPAGE, m_bKeepOnePage);
	DDX_Check(pDX, IDC_MONITOR_CLIP, m_bBlankMonitorClip);
	DDX_Check(pDX, IDC_ACTIVATE_NEW_ADDRESS, m_bActiveNewAddress);
	DDX_Check(pDX, ID_NEW_HOME, m_bNewHome);
	DDX_Check(pDX, IDC_USE_TARGET_FRAME_NAME, m_bUseTargetFrameName);
	DDX_Check(pDX, IDC_RELOAD_UNSHOWN_IMAGE, m_bReloadUnShownImage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWindowProp, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CWindowProp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindowProp message handlers
/////////////////////////////////////////////////////////////////////////////
// CTabProp property page

IMPLEMENT_DYNCREATE(CTabProp, CSAPrefsSubDlg)

CTabProp::CTabProp() : CSAPrefsSubDlg(CTabProp::IDD)
{
	//{{AFX_DATA_INIT(CTabProp)
	m_bAutoFitTab = FALSE;
	m_nMaxChars = 0;
	m_nMaxWidth = 0;
	m_nMinWidth = 0;
	m_bMultiTab = FALSE;
	m_nLDBClick = -1;
	m_nMCLICK = -1;
	m_bShowIndex = FALSE;
	m_nRClick = -1;
	m_bClickClose = -1;
	m_bWheelOnTab = FALSE;
	m_bWheelTab = FALSE;
	m_bShowDot = FALSE;
	m_bShowX=FALSE;
	m_bShowFavIcon = FALSE;
	m_bShowRead = FALSE;
	//}}AFX_DATA_INIT
}

CTabProp::~CTabProp()
{
}

void CTabProp::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabProp)
	DDX_Control(pDX, IDC_SPIN3, m_spin3);
	DDX_Control(pDX, IDC_SPIN2, m_spin2);
	DDX_Control(pDX, IDC_SPIN1, m_spin1);
	DDX_Control(pDX, IDC_RDBCLICK_BLANK, m_RDbClickBlank);
	DDX_Control(pDX, IDC_LDBCLICK_BLANK, m_LDbClickBlank);
	DDX_Check(pDX, IDC_AUTOFIT_TAB, m_bAutoFitTab);
	DDX_Text(pDX, IDC_MAXCHARS, m_nMaxChars);
	DDX_Text(pDX, IDC_MAXWIDTH, m_nMaxWidth);
	DDX_Text(pDX, IDC_MINWIDTH, m_nMinWidth);
	DDX_Check(pDX, IDC_MULTILINE_TAB, m_bMultiTab);
	DDX_CBIndex(pDX, IDC_LDBCLICK, m_nLDBClick);
	DDX_CBIndex(pDX, IDC_MCLIK, m_nMCLICK);
	DDX_Check(pDX, IDC_SHOW_INDEX, m_bShowIndex);
	DDX_CBIndex(pDX, IDC_RCLIK, m_nRClick);
	DDX_Radio(pDX, IDC_CC_DISABLE, m_bClickClose);
	DDX_Check(pDX, IDC_WHEEL_ON_TAB, m_bWheelOnTab);
	DDX_Check(pDX, IDC_WHEEL_TAB, m_bWheelTab);
	DDX_Check(pDX, IDC_SHOW_DOT, m_bShowDot);
	DDX_Check(pDX, IDC_SHOW_X, m_bShowX);
	DDX_Check(pDX, IDC_SHOW_FAVICON, m_bShowFavIcon);
	DDX_Check(pDX, IDC_SHOW_READ, m_bShowRead);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabProp, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CTabProp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabProp message handlers
/////////////////////////////////////////////////////////////////////////////
// CGeneralProp property page

IMPLEMENT_DYNCREATE(CGeneralProp, CSAPrefsSubDlg)

CGeneralProp::CGeneralProp() : CSAPrefsSubDlg(CGeneralProp::IDD)
{
	//{{AFX_DATA_INIT(CGeneralProp)
	m_bAniIcon = FALSE;
	m_bAutoHideFavSideBar = FALSE;

	_bEnableIE8Mode = FALSE;
	_bAlwaysIEMenuRightClickImg=TRUE;
	_bDirectOpenForOneWord = TRUE;
	m_bTopCloseExitApp = FALSE;

	m_strSkinPath = _T("");
	m_strGroupPath = _T("");
	
	m_strUserAppTitle=_T("");

	m_strFavPath = _T("");
	m_bUseDefaultFavPath = TRUE;
	m_bForbidMainTitle = FALSE;

	m_strLinkPath = _T("");
	m_bUseDefaultLinkPath = FALSE;

	_bForbidAutoUpdate = FALSE;
	_bKeepPageSize = FALSE;
	_iPageSize = 100;
	//}}AFX_DATA_INIT
}

CGeneralProp::~CGeneralProp()
{
}

void CGeneralProp::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralProp)
	DDX_Check(pDX, IDC_DISP_ANIICON, m_bAniIcon);
	DDX_Check(pDX, IDC_AUTO_HIDE_FAVORITES, m_bAutoHideFavSideBar);

	DDX_Check(pDX, IDC_ENABLE_IE8MODE, _bEnableIE8Mode);
	DDX_Check(pDX, IDC_AlwaysIEMenuRightClickImg, _bAlwaysIEMenuRightClickImg);	
	DDX_Check(pDX, IDC_TOP_CLOSE, m_bTopCloseExitApp);		
	DDX_Check(pDX, IDC_DirectOpenForOneWord, _bDirectOpenForOneWord);	

	DDX_Text(pDX, IDC_SKINPATH, m_strSkinPath);
	DDV_MaxChars(pDX, m_strSkinPath, 255);
	DDX_Text(pDX, IDC_GROUP_PATH, m_strGroupPath);
	DDV_MaxChars(pDX, m_strGroupPath, 255);

	DDX_Text(pDX, IDC_FAV_PATH, m_strFavPath);	
	DDX_Text(pDX, IDC_EDIT_USER_APP_TITLE, m_strUserAppTitle);
	DDV_MaxChars(pDX, m_strUserAppTitle, 80);

	DDX_Text(pDX, IDC_PAGE_SIZE, _iPageSize); //gfsize
	DDX_Check(pDX, IDC_FORBID_UPDATE, _bForbidAutoUpdate);
	DDX_Check(pDX, IDC_KEEP_PAGE_SIZE, _bKeepPageSize);

	DDV_MaxChars(pDX, m_strFavPath, 255);
	DDX_Check(pDX, IDC_DEFAULT_FAVPATH, m_bUseDefaultFavPath);
	DDX_Check(pDX, IDC_FORBID_MAIN_TITLE, m_bForbidMainTitle);

	DDX_Text(pDX, IDC_LINK_PATH, m_strLinkPath);
	DDV_MaxChars(pDX, m_strLinkPath, 255);
	DDX_Check(pDX, IDC_DEFAULT_LINKPATH, m_bUseDefaultLinkPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeneralProp, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CGeneralProp)
	ON_BN_CLICKED(IDC_GET_SKINPATH, OnGetSkinpath)
	ON_BN_CLICKED(IDC_GET_GROUPPATH, OnGetGrouppath)
	ON_BN_CLICKED(IDC_GET_FAVPATH, OnGetFavPath)
	ON_BN_CLICKED(IDC_OPEN_IE_FAV, OnOpenIEFavPath)
	ON_BN_CLICKED(IDC_SET_TO_IE_FAV, OnSetToIEFavPath)
	ON_BN_CLICKED(IDC_DEFAULT_FAVPATH, OnDefaultFavPath)
	ON_BN_CLICKED(IDC_KEEP_PAGE_SIZE, OnKeepPageSize)
	ON_BN_CLICKED(IDC_GET_LINKPATH, OnGetLinkPath)
	ON_BN_CLICKED(IDC_DEFAULT_LINKPATH, OnDefaultLinkPath)
	ON_BN_CLICKED(IDC_ENABLE_IE8MODE, OnEnaleIE8Mode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COtherProp property page

IMPLEMENT_DYNCREATE(COtherProp, CSAPrefsSubDlg)

COtherProp::COtherProp() : CSAPrefsSubDlg(COtherProp::IDD)
{
	//{{AFX_DATA_INIT(COtherProp)
	m_strCE1 = _T("");
	m_strCE2 = _T("");
	m_strCSE1 = _T("");
	m_strCSE2 = _T("");
	m_strSE1 = _T("");
	m_strSE2 = _T("");
	m_nMenuStyle = 1;
	m_bMinTray = FALSE;
	m_bCloseTray = FALSE;
	m_bShowTray = FALSE;
	m_bCloseAllWhenCloseToTray = FALSE;
	_bOpenWordOutOfIE = FALSE;
	//}}AFX_DATA_INIT
}

COtherProp::~COtherProp()
{
}

void COtherProp::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COtherProp)
	DDX_Control(pDX, IDC_HOTKEY, m_HotKey);
	DDX_Text(pDX, IDC_CE1, m_strCE1);
	DDX_Text(pDX, IDC_CE2, m_strCE2);
	DDX_Text(pDX, IDC_CSE1, m_strCSE1);
	DDX_Text(pDX, IDC_CSE2, m_strCSE2);
	DDX_Text(pDX, IDC_SE1, m_strSE1);
	DDX_Text(pDX, IDC_SE2, m_strSE2);
	DDX_Radio(pDX, IDC_MENU_WIN9X, m_nMenuStyle);
	DDX_Check(pDX, IDC_MIN_TRAY, m_bMinTray);
	DDX_Check(pDX, IDC_CLOSE_TRAY, m_bCloseTray);
	DDX_Check(pDX, IDC_SHOW_TRAY, m_bShowTray);
	DDX_Check(pDX, IDC_CLOSE_TRAY_CLOSEALL, m_bCloseAllWhenCloseToTray);
	DDX_Check(pDX, IDC_OPEN_WORD_OUT_IE, _bOpenWordOutOfIE);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COtherProp, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(COtherProp)
	ON_BN_CLICKED(IDC_SHOW_TRAY, OnShowTray)
	ON_BN_CLICKED(IDC_UPDATE_HOTKEY, OnUpdateHotkey)
	ON_BN_CLICKED(IDC_SET_DEFAULT, OnSetDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COtherProp message handlers

/////////////////////////////////////////////////////////////////////////////
// CSaveProp property page

IMPLEMENT_DYNCREATE(CSaveProp, CSAPrefsSubDlg)

CSaveProp::CSaveProp() : CSAPrefsSubDlg(CSaveProp::IDD)
{
	//{{AFX_DATA_INIT(CSaveProp)
	m_strSavePath = _T("");
	m_strImagePath = _T("");
	m_nAutoSave = -1;
	m_bCateWithSite = FALSE;
	m_bImgCateWithSite = FALSE;
	m_strSavePathSort = _T("");
	m_bNameByTitle = FALSE;
	m_bSavePageByDate = FALSE;
	m_bShowSortSaveDlg = FALSE;
	//}}AFX_DATA_INIT
}

CSaveProp::~CSaveProp()
{
}

void CSaveProp::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveProp)
	DDX_Text(pDX, IDC_SAVEPATH, m_strSavePath);
	DDX_Text(pDX, IDC_IMAGEPATH, m_strImagePath);
	DDX_CBIndex(pDX, IDC_AUTOSAVE, m_nAutoSave);
	DDX_Check(pDX, IDC_CATEWITHSITE, m_bCateWithSite);
	DDX_Check(pDX, IDC_IMG_CATEWITHSITE, m_bImgCateWithSite);
	DDX_Text(pDX, IDC_SAVEPATH_SORT, m_strSavePathSort);
	DDX_Check(pDX, IDC_NAME_BY_TITLE, m_bNameByTitle);
	DDX_Check(pDX, IDC_SAVE_BY_DATE, m_bSavePageByDate);
	DDX_Check(pDX, IDC_SHOW_SORT_DLG, m_bShowSortSaveDlg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveProp, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CSaveProp)
	ON_BN_CLICKED(IDC_GET_SAVEPATH, OnGetSavepath)
	ON_BN_CLICKED(IDC_GET_IMAGEPATH, OnGetImagepath)
	ON_BN_CLICKED(IDC_GET_SAVEPATH_SORT, OnGetSavepathSort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveProp message handlers


//#pragma optimize( "s", off)

BOOL COtherProp::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogOtherProp");
#endif
	LOADDLG("DialogOtherProp");
	CSAPrefsSubDlg::OnInitDialog();

	try{
	// TODO: Add extra initialization here
	UINT Modifiers = AfxGetApp()->GetProfileInt("Settings", "ShowHideModifiers", MOD_CONTROL);
	UINT Key = AfxGetApp()->GetProfileInt("Settings", "ShowHideKey", 192);
	m_HotKey.SetHotKey(Key, Mod2HotKey(Modifiers));
	m_OldModifiers = Modifiers;
	m_OldKey = Key;
	}catch(...){}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSaveProp::OnGetSavepath() 
{
	// TODO: Add your control notification handler code here
	if (_BrowserPath(this,m_strSavePath) == TRUE)
		GetDlgItem(IDC_SAVEPATH)->SetWindowText(m_strSavePath);//UpdateData(FALSE);
}

void CSaveProp::OnGetImagepath() 
{
	// TODO: Add your control notification handler code here
	if (_BrowserPath(this,m_strImagePath) == TRUE)
		GetDlgItem(IDC_IMAGEPATH)->SetWindowText(m_strImagePath);//UpdateData(FALSE);
}

void CGeneralProp::OnGetSkinpath() 
{
	// TODO: Add your control notification handler code here
	if (_BrowserPath(this,m_strSkinPath) == TRUE) 
		GetDlgItem(IDC_SKINPATH)->SetWindowText(m_strSkinPath);//UpdateData(FALSE);
}

void CGeneralProp::OnGetGrouppath() 
{
	// TODO: Add your control notification handler code here
	if (_BrowserPath(this,m_strGroupPath) == TRUE)
		GetDlgItem(IDC_GROUP_PATH)->SetWindowText(m_strGroupPath);//UpdateData(FALSE);
}
/////////////////////////////////////////////////////////////////////////////
// CMouseProp property page

IMPLEMENT_DYNCREATE(CMouseProp, CSAPrefsSubDlg)

CMouseProp::CMouseProp() : CSAPrefsSubDlg(CMouseProp::IDD)
{
	//{{AFX_DATA_INIT(CMouseProp)
	m_nImgDown = -1;
	m_nImgLeft = -1;
	m_nImgRight = -1;
	m_nImgUp = -1;
	m_nLinkDown = -1;
	m_nLinkLeft = -1;
	m_nLinkRight = -1;
	m_nLinkUp = -1;
	m_nTextDown = -1;
	m_nTextLeft = -1;
	m_nTextRight = -1;
	m_nTextUp = -1;
	m_bSimuDragLink = FALSE;
	//}}AFX_DATA_INIT
}

CMouseProp::~CMouseProp()
{
}

void CMouseProp::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMouseProp)
	DDX_CBIndex(pDX, IDC_IMG_DOWN, m_nImgDown);
	DDX_CBIndex(pDX, IDC_IMG_LEFT, m_nImgLeft);
	DDX_CBIndex(pDX, IDC_IMG_RIGHT, m_nImgRight);
	DDX_CBIndex(pDX, IDC_IMG_UP, m_nImgUp);
	DDX_CBIndex(pDX, IDC_LINK_DOWN, m_nLinkDown);
	DDX_CBIndex(pDX, IDC_LINK_LEFT, m_nLinkLeft);
	DDX_CBIndex(pDX, IDC_LINK_RIGHT, m_nLinkRight);
	DDX_CBIndex(pDX, IDC_LINK_UP, m_nLinkUp);
	DDX_CBIndex(pDX, IDC_TXT_DOWN, m_nTextDown);
	DDX_CBIndex(pDX, IDC_TXT_LEFT, m_nTextLeft);
	DDX_CBIndex(pDX, IDC_TXT_RIGHT, m_nTextRight);
	DDX_CBIndex(pDX, IDC_TXT_UP, m_nTextUp);
	DDX_Check(pDX, IDC_SIMU_DRAG_LINK, m_bSimuDragLink);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMouseProp, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CMouseProp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMouseProp message handlers


void CGeneralProp::SetPageSizeState(BOOL state)
{
	GetDlgItem(IDC_PAGE_SIZE)->EnableWindow(state);
}


void CGeneralProp::SetFavPathState(BOOL state)
{
	GetDlgItem(IDC_STATIC_FAVPATH)->EnableWindow(state);
	GetDlgItem(IDC_FAV_PATH)->EnableWindow(state);
	GetDlgItem(IDC_GET_FAVPATH)->EnableWindow(state);

	GetDlgItem(IDC_SET_TO_IE_FAV)->EnableWindow(state); //gffav
}

void CGeneralProp::SetLinkPathState(BOOL state)
{
	GetDlgItem(IDC_STATIC_LINKPATH)->EnableWindow(state);
	GetDlgItem(IDC_LINK_PATH)->EnableWindow(state);
	GetDlgItem(IDC_GET_LINKPATH)->EnableWindow(state);
}

BOOL CGeneralProp::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogGeneralProp");
#endif
	LOADDLG("DialogGeneralProp");
	CSAPrefsSubDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
//	if (m_bUseDefaultFavPath)
//		SetFavPathState(FALSE);

	if (m_bUseDefaultLinkPath)
		SetLinkPathState(FALSE);

	SetPageSizeState(_bKeepPageSize); //gfsize

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGeneralProp::OnGetFavPath() 
{
	// TODO: Add your control notification handler code here
	if (_BrowserPath(this,m_strFavPath ) == TRUE) 
		GetDlgItem(IDC_FAV_PATH)->SetWindowText(m_strFavPath);//UpdateData(FALSE);
}

void CGeneralProp::OnSetToIEFavPath() 
{
	if (!CPathHelper::IsExist(m_strFavPath) )
	{
		AfxMessageBox(CGfAppString::GetString(IDS_MSG_PATH_NOT_EXIST) );
		return;
	}

	if (CRegHelper::SetFavoriteFolder(m_strFavPath) )
	{
		AfxMessageBox( CGfAppString::GetString(IDS_MSG_SET_IE_PATH_OK) );
	}
	else
	{
		AfxMessageBox( CGfAppString::GetString(IDS_MSG_SET_IE_PATH_FAILED), MB_ICONERROR|MB_OK);
	}

}

void CGeneralProp::OnOpenIEFavPath() 
{
	CString strPath = "";
	BOOL bRet = CRegHelper::GetFavoriteFolder(strPath);

	if (bRet)
	{
		CGf_Win::OpenNormal(strPath);
	}
	else
	{
		AfxMessageBox(CGfAppString::GetString(IDS_MSG_GET_IE_PATH_FAILED) );
	}
}

void CGeneralProp::OnKeepPageSize() 
{
	CButton *p = (CButton*)GetDlgItem(IDC_KEEP_PAGE_SIZE);
	BOOL state = p->GetCheck();
	SetPageSizeState(state);
}

void CGeneralProp::OnDefaultFavPath() 
{

//	CButton *p = (CButton*)GetDlgItem(IDC_DEFAULT_FAVPATH);
//	BOOL state = p->GetCheck();
//	SetFavPathState(!state);
}

void CGeneralProp::OnEnaleIE8Mode() 
{
	CButton *p = (CButton*)GetDlgItem(IDC_ENABLE_IE8MODE);
	BOOL state = p->GetCheck();
	
	if (state)
	{
		int iRetAsk =AfxMessageBox(CGfAppString::GetString(IDS_COMFIRM_IE8MODE), MB_YESNOCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2);

		if (IDYES == iRetAsk)
		{
			_bEnableIE8Mode = TRUE;
		}
		else
		{
			p->SetCheck(_bEnableIE8Mode);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CMouseProp property page

IMPLEMENT_DYNCREATE(CGestureProp, CSAPrefsSubDlg)

CGestureProp::CGestureProp() : CSAPrefsSubDlg(CGestureProp::IDD)
{
	//{{AFX_DATA_INIT(CGestureProp)
	m_bShowGesture = FALSE;
	//}}AFX_DATA_INIT
	m_nLastIdx = MAX_MOUSE_ACTION;
}

CGestureProp::~CGestureProp()
{
}
void CGestureProp::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGestureProp)
	DDX_Control(pDX, IDC_GESTURE, m_gesture);
	DDX_Control(pDX, IDC_COMMAND, m_command);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Check(pDX, IDC_SHOW_GESTURE, m_bShowGesture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGestureProp, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CGestureProp)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_NEW, OnNew)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemchangedList)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_BN_CLICKED(IDC_LEFT, OnLeft)
	ON_BN_CLICKED(IDC_RIGHT, OnRight)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGestureProp message handlers
void CGestureProp::OnNew()
{
	m_gesture.SetWindowText("");
	m_command.SetCurSel(-1);
	m_nLastIdx = MAX_MOUSE_ACTION;
}

void CGestureProp::OnUpdate() 
{
	// TODO: Add your control notification handler code here
	CString strGesture;
	m_gesture.GetWindowText(strGesture);
	if (strGesture.GetLength()<1)
		return;
	int nCurSel = m_command.GetCurSel();
	if (nCurSel<0)
		return;
	CString strItem;
	int count = m_list.GetItemCount();
	for (int i=0; i<count; i++)
	{
		strItem = m_list.GetItemText(i,0);
		if (strItem == strGesture)
			break;
	}
	if (i==count)
		m_list.InsertItem(count,strGesture);
	m_command.GetLBText(nCurSel, strGesture);
	m_list.SetItemText(i,1,strGesture);
	m_list.SetItemData(i,m_command.GetItemData(nCurSel));
	LIST_FOCUS_POS (m_list, i);
}

void CGestureProp::OnDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int current = m_list.GetNextSelectedItem(pos);
	if (current<MAX_MOUSE_ACTION)
	{
		CString str;
		LOADSTR(str,IDS_NONE);
		m_list.SetItemText(current,1,str);
		m_list.SetItemData(current,0);
		return;
	}
	m_list.DeleteItem(current);
	LIST_FOCUS_POS(m_list,current);
}

void CGestureProp::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int i=-1, nIndex = pNMListView->iItem;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if(pos!=NULL)
		i = m_list.GetNextSelectedItem(pos);
	if(nIndex==i)
	{
		m_gesture.SetWindowText(m_list.GetItemText(nIndex,0));
		m_command.SelectString(0,m_list.GetItemText(nIndex,1));
	}
	else
	{
		m_gesture.SetWindowText("");
		m_command.SetCurSel(-1);
	}
	m_nLastIdx = i;
	*pResult = 0;
}

void CGestureProp::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int nItem = -1, newItem;
	if(pos!=NULL)
	{
		nItem = m_list.GetNextSelectedItem(pos);
		if (nItem<MAX_MOUSE_ACTION)
			goto _myexit;
		if(pNMUpDown->iDelta<0)
			newItem = nItem -1;
		else
			newItem = nItem + 1;
		if (newItem<MAX_MOUSE_ACTION)
			goto _myexit;
		int n = m_list.GetItemCount();
		if(newItem>=0 && newItem<n)
		{
			CString str = m_list.GetItemText(nItem,0);
			CString str2 = m_list.GetItemText(nItem, 1);
			DWORD   bState = m_list.GetItemData(nItem);
			m_list.DeleteItem(nItem);

			m_list.InsertItem(newItem, str);
			m_list.SetItemText(newItem, 1, str2);
			m_list.SetItemState(newItem, LVIS_SELECTED, LVIS_SELECTED);
			m_list.SetItemData(newItem, bState);
			m_list.EnsureVisible(newItem, FALSE);
		}
	}

_myexit:
	*pResult = 0;
}

void CGestureProp::GetMenuItemString(CString &strName, int nID)
{
	int count = m_command.GetCount();
	for (int i=0;i<count;i++)
	{
		if (m_command.GetItemData(i) == (DWORD)nID)
			m_command.GetLBText(i,strName);
	}
}

BOOL CGestureProp::OnInitDialog()
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogMouseProp");
#endif
	LOADDLG("DialogMouseProp");
	CSAPrefsSubDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_list.SetExtendedStyle(m_list.GetExtendedStyle()| LVS_EX_FULLROWSELECT);
	// TODO: Add extra initialization here
	CRect rect;
	CString str;
	LOADSTR(str ,IDS_NAME);
	m_list.InsertColumn(0,str);
	m_list.GetWindowRect(rect);
	m_list.SetColumnWidth(0, 120);
	LOADSTR(str ,IDS_COMMAND);
	m_list.InsertColumn(1,str);
	m_list.SetColumnWidth(1, rect.Width()-150);
	//
	_LoadMenuString2ComboBox(pmf->GetMenu(),&m_command);
	//load mouse item
	LOADSTR(str ,IDS_LEFT_CLICK);m_list.InsertItem(0,str);m_list.SetItemData(0,m_nLButtonDown);
	GetMenuItemString(str,m_nLButtonDown);m_list.SetItemText(0,1,str);
	//
	LOADSTR(str ,IDS_LEFT_DBCLICK);m_list.InsertItem(1,str);m_list.SetItemData(1,m_nLDbClick);
	GetMenuItemString(str,m_nLDbClick);m_list.SetItemText(1,1,str);
	//
	LOADSTR(str ,IDS_MIDDLE_CLICK);m_list.InsertItem(2,str);m_list.SetItemData(2,m_nMButtonDown);
	GetMenuItemString(str,m_nMButtonDown);m_list.SetItemText(2,1,str);
	//
	LOADSTR(str ,IDS_RIGHT_CLICK);m_list.InsertItem(3,str);m_list.SetItemData(3,m_nRButtonDown);
	GetMenuItemString(str,m_nRButtonDown);m_list.SetItemText(3,1,str);
	//
	LOADSTR(str ,IDS_LEFT_RIGHT);m_list.InsertItem(4,str);m_list.SetItemData(4,m_nLeftRight);
	GetMenuItemString(str,m_nLeftRight);m_list.SetItemText(4,1,str);
	//
	LOADSTR(str ,IDS_RIGHT_LEFT);m_list.InsertItem(5,str);m_list.SetItemData(5,m_nRightLeft);
	GetMenuItemString(str,m_nRightLeft);m_list.SetItemText(5,1,str);

	LOADSTR(str ,IDS_RIGHT_DBCLICK);m_list.InsertItem(6,str);m_list.SetItemData(6,m_nRDbClick);
	GetMenuItemString(str,m_nRDbClick);m_list.SetItemText(6,1,str);
	//
	CGesture *item;
	CString strUp,strDown,strLeft,strRight;
	if (theApp.m_bUseLngFile)
	{
		LOADSTR(strUp,IDS_UP);
		LOADSTR(strDown,IDS_DOWN);
		LOADSTR(strLeft,IDS_LEFT);
		LOADSTR(strRight,IDS_RIGHT);
	}
	CString strItem;
	for(int i=0;i<=pmf->m_GestureList.m_Gestures.GetUpperBound();i++)
	{
		item = pmf->m_GestureList.m_Gestures.GetAt(i);
		strItem = item->gesture;
		if (theApp.m_bUseLngFile)
		{
			strItem.Replace("U",strUp);
			strItem.Replace("D",strDown);
			strItem.Replace("L",strLeft);
			strItem.Replace("R",strRight);
		}
		m_list.InsertItem(i+MAX_MOUSE_ACTION, strItem);
		//
		GetMenuItemString(strItem,item->command);
		m_list.SetItemText(i+MAX_MOUSE_ACTION, 1, strItem);
		m_list.SetItemData(i+MAX_MOUSE_ACTION,item->command);
	}
	//
	if (theApp.m_bUseLngFile)
	{
		GetDlgItem(IDC_UP)->SetWindowText(strUp);
		GetDlgItem(IDC_DOWN)->SetWindowText(strDown);
		GetDlgItem(IDC_LEFT)->SetWindowText(strLeft);
		GetDlgItem(IDC_RIGHT)->SetWindowText(strRight);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGestureProp::OnOK() 
{
	OnUpdate(); //gf
	
	CString strUp,strDown,strLeft,strRight;
	if (theApp.m_bUseLngFile)
	{
		LOADSTR(strUp,IDS_UP);
		LOADSTR(strDown,IDS_DOWN);
		LOADSTR(strLeft,IDS_LEFT);
		LOADSTR(strRight,IDS_RIGHT);
	}
	//
	CGesture *ps;
	pmf->m_GestureList.Clean();
	int n = m_list.GetItemCount();
	for( int i=MAX_MOUSE_ACTION; i<n; i++)
	{
		ps = new CGesture;
		ps->gesture = m_list.GetItemText(i,0);
		if (theApp.m_bUseLngFile)
		{
			ps->gesture.Replace(strUp,"U");
			ps->gesture.Replace(strDown,"D");
			ps->gesture.Replace(strLeft,"L");
			ps->gesture.Replace(strRight,"R");
		}
		//
		ps->command = m_list.GetItemData(i);
		pmf->m_GestureList.m_Gestures.Add(ps);
	}
	pmf->m_GestureList.SaveGestureList();
	//
	m_nLButtonDown = m_list.GetItemData(0);
	m_nLDbClick = m_list.GetItemData(1);
	m_nMButtonDown = m_list.GetItemData(2);
	m_nRButtonDown = m_list.GetItemData(3);
	m_nLeftRight = m_list.GetItemData(4);
	m_nRightLeft = m_list.GetItemData(5);
	m_nRDbClick = m_list.GetItemData(6);
	//
	CSAPrefsSubDlg::OnOK();
}

void CGestureProp::InputOneGesture(int id) 
{
	if (m_nLastIdx>=0 && m_nLastIdx < MAX_MOUSE_ACTION )
		return;

	CString str;
	m_gesture.GetWindowText(str);

	CString strArrow;
	LOADSTR(strArrow,id);
	
	int nArrowLen = strArrow.GetLength();
	if (str.GetLength()>=8*nArrowLen)
		return;
	if (str.GetLength())
	{
		if (strArrow == str.Right(nArrowLen))
			return;
	}
	str += strArrow;
	m_gesture.SetWindowText(str);
}

void CGestureProp::OnUp() 
{
	// TODO: Add your control notification handler code here
	InputOneGesture(IDS_UP);
}

void CGestureProp::OnDown() 
{
	// TODO: Add your control notification handler code here
	InputOneGesture(IDS_DOWN);
}

void CGestureProp::OnLeft() 
{
	// TODO: Add your control notification handler code here
	InputOneGesture(IDS_LEFT);
}

void CGestureProp::OnRight() 
{
	// TODO: Add your control notification handler code here
	InputOneGesture(IDS_RIGHT);
}

void CGestureProp::OnBack() 
{
	// TODO: Add your control notification handler code here
	if (m_nLastIdx>=0 && m_nLastIdx < MAX_MOUSE_ACTION )
		return;
	CString str;
	CString strArrow;
	LOADSTR(strArrow,IDS_UP);
	m_gesture.GetWindowText(str);
	if (str.GetLength())
	{
		int len = strArrow.GetLength();
		str = str.Left(str.GetLength()-len);
	}
	m_gesture.SetWindowText(str);
}

void COtherProp::OnShowTray() 
{
	// TODO: Add your control notification handler code here
	CButton *p = (CButton*)GetDlgItem(IDC_SHOW_TRAY);
	BOOL state = p->GetCheck();
	p = (CButton*)GetDlgItem(IDC_MIN_TRAY);
	p->SetCheck(state);
	p = (CButton*)GetDlgItem(IDC_CLOSE_TRAY);
	p->SetCheck(state);
}

BOOL CMouseProp::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogMouseDragProp");
#endif
	CSAPrefsSubDlg::OnInitDialog();
#ifdef _WRITE_LNG_FILE_
	_WriteDlgComboBox(this,"DialogMouseDragProp");
#endif
	LOADDLG("DialogMouseDragProp");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

WORD COtherProp::Mod2HotKey(WORD key)
{
	WORD temp=0;
	if (key&MOD_ALT)
		temp |= HOTKEYF_ALT;
	if (key&MOD_CONTROL)
		temp |= HOTKEYF_CONTROL;
	if (key&MOD_SHIFT)
		temp |= HOTKEYF_SHIFT;
	return temp;
}

WORD COtherProp::HotKey2Mod(WORD key)
{
	WORD temp=0;
	if (key&HOTKEYF_ALT)
		temp |= MOD_ALT;
	if (key&HOTKEYF_CONTROL)
		temp |= MOD_CONTROL;
	if (key&HOTKEYF_SHIFT)
		temp |= MOD_SHIFT;
	return temp;
}

void COtherProp::OnUpdateHotkey() 
{
	// TODO: Add your control notification handler code here
	WORD Modifiers, Key;
	WORD temp=0;
	m_HotKey.GetHotKey(Key, Modifiers);
	Modifiers = HotKey2Mod(Modifiers);//HOTKEYF_ALT != MOD_ALT !!!
	if (Key<=0)
	{
		if(MSGBOX(IDS_REG_HOTKEY_NULL, MB_YESNO|MB_ICONQUESTION)==IDYES)
		{
			AfxGetApp()->WriteProfileInt("Settings", "ShowHideModifiers", 0);
			AfxGetApp()->WriteProfileInt("Settings", "ShowHideKey", 0);
			UnregisterHotKey (pmf->m_hWnd, 0);
			return;
		}
		m_HotKey.SetHotKey(m_OldKey, Mod2HotKey(m_OldModifiers));
		return;
	}
	UnregisterHotKey (pmf->m_hWnd, 0);
	g_bRegisterHotKeyOk = RegisterHotKey ( pmf->m_hWnd, 0, Modifiers , Key);
	if (!g_bRegisterHotKeyOk)
	{
		MSGBOX(IDS_REG_HOTKEY_FAIL);
		//back to old key
		g_bRegisterHotKeyOk = RegisterHotKey ( pmf->m_hWnd, 0, m_OldModifiers , m_OldKey);
	}
	else
	{
		AfxGetApp()->WriteProfileInt("Settings", "ShowHideModifiers", Modifiers);
		AfxGetApp()->WriteProfileInt("Settings", "ShowHideKey", Key);
		m_OldModifiers = Modifiers;
		m_OldKey = Key;
		MSGBOX(IDS_REG_HOTKEY_OK);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CToolbarProp dialog
IMPLEMENT_DYNCREATE(CToolbarProp, CSAPrefsSubDlg)

CToolbarProp::CToolbarProp() : CSAPrefsSubDlg(CToolbarProp::IDD)
{
	//{{AFX_DATA_INIT(CToolbarProp)
	m_bShowToolbarLabel = TRUE;
	m_bComboShowIcon = FALSE;
	m_bGoProxyMenu = FALSE;
	m_bShowSearchBarButton = FALSE;
	m_bShowAddressBarButton = FALSE;
	//}}AFX_DATA_INIT
}

CToolbarProp::~CToolbarProp()
{
}

void CToolbarProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolbarProp)
	DDX_Check(pDX, ID_TB_LABEL, m_bShowToolbarLabel);
	DDX_Check(pDX, ID_TB_COMBO_ICON, m_bComboShowIcon);
	DDX_Check(pDX, IDC_GO_PROXY_MENU, m_bGoProxyMenu);
	DDX_Check(pDX, ID_SEARCH_BUTTON, m_bShowSearchBarButton);
	DDX_Check(pDX, ID_ADDRESS_BUTTON, m_bShowAddressBarButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolbarProp, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CToolbarProp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolbarProp message handlers

BOOL CToolbarProp::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogToolbarProp");
#endif
	LOADDLG("DialogToolbarProp");
	CSAPrefsSubDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	int id,index;
	CButton *pWnd; 
	index = 1;
	for (id=IDC_FS_TOOL;id<=IDC_FS_STATUS;id++)
	{
		pWnd = (CButton*)GetDlgItem(id);
		pWnd->SetCheck(m_dwFsShow & index);
		index *= 2;
	}
	index = 1;
	for (id=IDC_FS_TOOL2;id<=IDC_FS_STATUS2;id++)
	{
		pWnd = (CButton*)GetDlgItem(id);
		pWnd->SetCheck(m_dwAhShow & index);
		index *= 2;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CToolbarProp::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	int id,index;
	CButton *pWnd;

	m_dwFsShow = 0;
	index = 1;
	for (id=IDC_FS_TOOL;id<=IDC_FS_STATUS;id++)
	{
		pWnd = (CButton*)GetDlgItem(id);
		if (pWnd->GetCheck())
			m_dwFsShow |= index;
		index *= 2;
	}
	//
	m_dwAhShow = 0;
	index = 1;
	for (id=IDC_FS_TOOL2;id<=IDC_FS_STATUS2;id++)
	{
		pWnd = (CButton*)GetDlgItem(id);
		if (pWnd->GetCheck())
			m_dwAhShow |= index;
		index *= 2;
	}
	CSAPrefsSubDlg::OnOK();
}
/////////////////////////////////////////////////////////////////////////////
// CMenuProp property page

IMPLEMENT_DYNCREATE(CMenuProp, CSAPrefsSubDlg)

CMenuProp::CMenuProp() : CSAPrefsSubDlg(CMenuProp::IDD)
{
	//{{AFX_DATA_INIT(CMenuProp)
		// NOTE: the ClassWizard will add member initialization here
	m_bSingleMenu = FALSE;
	m_bCompactFavBar = FALSE;
	m_bAddFav = FALSE;
	m_bDisplayOpenAll = FALSE;
	m_bFavShowUrl = FALSE;
	m_bShowHidden = FALSE;
	m_nMenuSize = 0;
	m_nMenuSizeOther = 0;
	//}}AFX_DATA_INIT
}

CMenuProp::~CMenuProp()
{
}

void CMenuProp::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMenuProp)
	DDX_Control(pDX, IDC_SPIN1, m_spin1);
	DDX_Check(pDX, IDC_SINGLEMENU, m_bSingleMenu);
	DDX_Check(pDX, IDC_ADDFAV, m_bAddFav);
	DDX_Check(pDX, IDC_DISPLAY_OPENALL, m_bDisplayOpenAll);
	DDX_Check(pDX, IDC_COMPACTFAVBAR, m_bCompactFavBar);
	DDX_Check(pDX, IDC_FAV_SHOW_URL, m_bFavShowUrl);
	DDX_Check(pDX, IDC_SHOW_HIDDEN, m_bShowHidden);
	DDX_Text(pDX, IDC_MAXMENU, m_nMenuSize);
	DDX_Text(pDX, IDC_MAXMENU_OTHER, m_nMenuSizeOther);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMenuProp, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CMenuProp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStartProp property page

IMPLEMENT_DYNCREATE(CStartProp, CSAPrefsSubDlg)

CStartProp::CStartProp() : CSAPrefsSubDlg(CStartProp::IDD)
{
	//{{AFX_DATA_INIT(CStartProp)
	m_bCloseCleanCache = FALSE;
	m_bCloseCleanCookies = FALSE;
	m_bCloseCleanFormData = FALSE;
	m_bCloseCleanHistory = FALSE;
	m_bCloseCleanTypedAddress = FALSE;
	m_bCloseCleanTypedSearch = FALSE;
	m_bMenuLoadLastClose = FALSE;
	m_nStartMode = -1;
	m_bOneInstance = FALSE;
	m_bAutoRun = FALSE;
	m_bLockHomepage = FALSE;
	m_strStartHomeURL = _T("");
	//}}AFX_DATA_INIT
}

CStartProp::~CStartProp()
{
}

void CStartProp::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStartProp)
	DDX_Check(pDX, IDC_CLOSE_CLEAN_CACHE, m_bCloseCleanCache);
	DDX_Check(pDX, IDC_CLOSE_CLEAN_COOKIES, m_bCloseCleanCookies);
	DDX_Check(pDX, IDC_CLOSE_CLEAN_FORM_DATA, m_bCloseCleanFormData);
	DDX_Check(pDX, IDC_CLOSE_CLEAN_HISTORY, m_bCloseCleanHistory);
	DDX_Check(pDX, IDC_CLOSE_CLEAN_TYPED_ADDRESS, m_bCloseCleanTypedAddress);
	DDX_Check(pDX, IDC_CLOSE_CLEAN_TYPED_SEARCH, m_bCloseCleanTypedSearch);
	DDX_Check(pDX, IDC_CLOSE_CLEAN_UNDO_CLOSE, m_bCloseCleanUndoClose);
	DDX_Check(pDX, IDC_LOAD_LAST_CLOSE, m_bMenuLoadLastClose);
	DDX_Radio(pDX, IDC_START_MODE, m_nStartMode);
	DDX_Check(pDX, IDC_ONLYONE, m_bOneInstance);
	DDX_Check(pDX, IDC_AUTO_RUN, m_bAutoRun);
	DDX_Check(pDX, IDC_LOCKHOME, m_bLockHomepage);
	DDX_Text(pDX, IDC_EDIT_HOME_URL, m_strStartHomeURL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStartProp, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CStartProp)
	ON_BN_CLICKED(IDC_BUTTON_HOME_CUR, OnButtonHomeCur)
	ON_BN_CLICKED(IDC_BUTTON_HOME_NONE, OnButtonHomeNone)
	ON_BN_CLICKED(IDC_BUTTON_HOME_BLANK, OnButtonHomeBlank)
	ON_BN_CLICKED(IDC_BUTTON_HOME_GET_IE, OnButtonHomeGetIe)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStartProp message handlers

/////////////////////////////////////////////////////////////////////////////
// CDeleteProp property page

IMPLEMENT_DYNCREATE(CDeleteProp, CSAPrefsSubDlg)

CDeleteProp::CDeleteProp() : CSAPrefsSubDlg(CDeleteProp::IDD)
{
	//{{AFX_DATA_INIT(CDeleteProp)
	//}}AFX_DATA_INIT
}

CDeleteProp::~CDeleteProp()
{
}

void CDeleteProp::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeleteProp)
	DDX_Control(pDX, IDC_LIST2, m_list2);
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeleteProp, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CDeleteProp)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_DELETEALL, OnDeleteall)
	ON_BN_CLICKED(IDC_DELETE_SEARCH, OnDeleteSearch)
	ON_BN_CLICKED(IDC_DELETEALL_SEARCH, OnDeleteallSearch)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_EDIT_SEARCH, OnEditSearch)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnDblclkList2)
	ON_BN_CLICKED(IDC_DELETE_CURRENT, OnDeleteCurrent)
	ON_BN_CLICKED(IDC_DELETE_CURRENT_SEARCH, OnDeleteCurrentSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeleteProp message handlers

void CDeleteProp::OnDelete() 
{
	// TODO: Add your control notification handler code here
	int i,count = m_list.GetItemCount();
	if (count<=0)
		return;
	for (i=count-1;i>=0;i--)
	{
		if (m_list.GetCheck(i))
			m_list.DeleteItem(i);
	}
}

void CDeleteProp::OnDeleteall() 
{
	// TODO: Add your control notification handler code here
	m_list.DeleteAllItems();
}

void CDeleteProp::OnDeleteSearch() 
{
	// TODO: Add your control notification handler code here
	int i,count = m_list2.GetItemCount();
	if (count<=0)
		return;
	for (i=count-1;i>=0;i--)
	{
		if (m_list2.GetCheck(i))
			m_list2.DeleteItem(i);
	}
}

void CDeleteProp::OnDeleteallSearch() 
{
	// TODO: Add your control notification handler code here
	m_list2.DeleteAllItems();
}

BOOL CDeleteProp::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogDeleteProp");
#endif
	LOADDLG("DialogDeleteProp");
	CSAPrefsSubDlg::OnInitDialog();

	try{
	// TODO: Add extra initialization here
	TCHAR           sz[MAX_PATH];
	COMBOBOXEXITEM item;
	item.mask = CBEIF_TEXT;
	item.pszText = (LPTSTR)sz;
	item.cchTextMax = MAX_PATH;
	
	CRect rect;
	m_list.InsertColumn(0,"");
	m_list.GetWindowRect(rect);
	m_list.SetColumnWidth(0, rect.Width()-25);
	m_list.SetExtendedStyle(m_list.GetExtendedStyle()| LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);
	//
	m_list2.InsertColumn(0,"");
	m_list2.GetWindowRect(rect);
	m_list2.SetColumnWidth(0, rect.Width()-25);
	m_list2.SetExtendedStyle(m_list2.GetExtendedStyle()| LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);
	//
	int nc,i;
	//
	nc = pmf->m_wndAddress->SendMessage(CB_GETCOUNT, 0, 0);
	for(i=0;i<nc;i++)
	{
		item.iItem = i;
		pmf->m_wndAddress->GetItem(&item);
		m_list.InsertItem(i,sz);
	}
	//
	nc = pmf->m_wndSearch->SendMessage(CB_GETCOUNT, 0, 0);
	for(i=0;i<nc;i++)
	{
		item.iItem = i;
		pmf->m_wndSearch->GetItem(&item);
		m_list2.InsertItem(i,sz);
	}

	}catch(...){}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeleteProp::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	TCHAR           sz[MAX_PATH];

	COMBOBOXEXITEM item;
	item.mask = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE ;
	item.iImage = 1;
	item.iSelectedImage = 1;
	item.pszText = (LPTSTR)sz;
	item.cchTextMax = MAX_PATH;

	int nc,i;
	nc = m_list.GetItemCount();
	if (!GF_ADD_ADRRESS_TITLE)
	{
		pmf->m_wndAddress->ResetContent();
	}
	
	for(i=0;i<nc;i++)
	{
		item.iItem = i;
		m_list.GetItemText(i,0,sz,MAX_PATH);

		if (!GF_ADD_ADRRESS_TITLE)
		{
			pmf->m_wndAddress->InsertItem(&item);
		}
		
	}	
	pmf->SaveTypedURL();
	//
	nc = m_list2.GetItemCount();
	pmf->m_wndSearch->ResetContent();
	pmf->m_astrTypedSearch.RemoveAll();
	for(i=0;i<nc;i++)
	{
		item.iItem = i;
		m_list2.GetItemText(i,0,sz,MAX_PATH);
		pmf->m_wndSearch->InsertItem(&item);
		//
		pmf->m_astrTypedSearch.AddTail(sz);
	}	
	pmf->SaveSearchKey();

	CSAPrefsSubDlg::OnOK();
}
/////////////////////////////////////////////////////////////////////////////
// CStatusBarProp property page

IMPLEMENT_DYNCREATE(CStatusBarProp, CSAPrefsSubDlg)

CStatusBarProp::CStatusBarProp() : CSAPrefsSubDlg(CStatusBarProp::IDD)
{
	//{{AFX_DATA_INIT(CStatusBarProp)
	m_strStatusStringFormat = _T("");
	//}}AFX_DATA_INIT
}

CStatusBarProp::~CStatusBarProp()
{
}

void CStatusBarProp::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatusBarProp)
	DDX_Text(pDX, IDC_STATUS_STRING_FORMAT, m_strStatusStringFormat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatusBarProp, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CStatusBarProp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatusBarProp message handlers
/////////////////////////////////////////////////////////////////////////////
// CConfirmProp property page

IMPLEMENT_DYNCREATE(CConfirmProp, CSAPrefsSubDlg)

CConfirmProp::CConfirmProp() : CSAPrefsSubDlg(CConfirmProp::IDD)
{
	//{{AFX_DATA_INIT(CConfirmProp)
	m_bKeepSilent = FALSE;
	m_bConfirmExit = FALSE;
	m_bConfirmCloseAll = FALSE;
	m_bConfirmCloseLock = FALSE;
	m_bConfirmExitDowning = FALSE;
	m_bConfirmCloseAllKeepCur = FALSE;
	m_bConfirmUnsafePage = FALSE;
	_bEnableMixContentAlert = FALSE;
	//}}AFX_DATA_INIT
}

CConfirmProp::~CConfirmProp()
{
}

void CConfirmProp::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfirmProp)
	DDX_Check(pDX, IDC_KEEPSILENT, m_bKeepSilent);
	DDX_Check(pDX, IDC_COMFIRMEXIT, m_bConfirmExit);
	DDX_Check(pDX, IDC_CONFIRMCLOSEALL, m_bConfirmCloseAll);
	DDX_Check(pDX, IDC_CONFIRMCLOSELOCK, m_bConfirmCloseLock);
	DDX_Check(pDX, IDC_COMFIRMEXIT_DOWNING, m_bConfirmExitDowning);
	DDX_Check(pDX, IDC_CONFIRMCLOSEALL_KEEPCUR, m_bConfirmCloseAllKeepCur);
	DDX_Check(pDX, IDC_CONFIRM_UNSAFE_PAGE, m_bConfirmUnsafePage);	
	DDX_Check(pDX, IDC_CONFIRM_MIX_CONTENT_ALERT, _bEnableMixContentAlert); //gf
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfirmProp, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CConfirmProp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfirmProp message handlers

void CDeleteProp::OnEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int current = m_list.GetNextSelectedItem(pos);
	if (current<0)
		return;
	CInputDlg dlg;
	CString strMsg;
	LOADSTR(strMsg ,IDS_INPUT_URL);
	dlg.m_strMsg = strMsg;
	CString strData = m_list.GetItemText(current,0);
	dlg.m_strOutput = strData;
	
	if(dlg.DoModal() == IDOK)
		m_list.SetItemText(current,0,dlg.m_strOutput);
}

void CDeleteProp::OnEditSearch() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_list2.GetFirstSelectedItemPosition();
	int current = m_list2.GetNextSelectedItem(pos);
	if (current<0)
		return;
	CInputDlg dlg;
	CString strMsg;
	LOADSTR(strMsg ,IDS_INPUT_KEYWORD);
	dlg.m_strMsg = strMsg;
	CString strData = m_list2.GetItemText(current,0);
	dlg.m_strOutput = strData;
	
	if(dlg.DoModal() == IDOK)
		m_list2.SetItemText(current,0,dlg.m_strOutput);
}

void CDeleteProp::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEdit();
	*pResult = 0;
}

void CDeleteProp::OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEditSearch();
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CCollectorProp property page

IMPLEMENT_DYNCREATE(CCollectorProp, CSAPrefsSubDlg)

CCollectorProp::CCollectorProp() : CSAPrefsSubDlg(CCollectorProp::IDD)
{
	//{{AFX_DATA_INIT(CCollectorProp)
	m_bOutputSaveTime = FALSE;
	m_bOutputSaveFilter = FALSE;
	m_bOutputSaveImg = FALSE;
	m_bOutputSavePage = FALSE;
	m_bOutputAutoClean = FALSE;
	m_bCollectorAddtoBottom = -1;
	m_bOutputSaveText = FALSE;
	m_bOutputSaveUrlFilter = FALSE;
	//}}AFX_DATA_INIT
}

CCollectorProp::~CCollectorProp()
{
}

void CCollectorProp::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCollectorProp)
	DDX_Check(pDX, IDC_OUTPUT_SAVE_TIME, m_bOutputSaveTime);
	DDX_Check(pDX, IDC_OUTPUT_SAVE_FILTER, m_bOutputSaveFilter);
	DDX_Check(pDX, IDC_OUTPUT_SAVE_IMG, m_bOutputSaveImg);
	DDX_Check(pDX, IDC_OUTPUT_SAVE_PAGE, m_bOutputSavePage);
	DDX_Check(pDX, IDC_OUTPUT_AUTO_CLEAN, m_bOutputAutoClean);
	DDX_Radio(pDX, IDC_ADD_NEW_BOTTOM, m_bCollectorAddtoBottom);
	DDX_Check(pDX, IDC_OUTPUT_SAVE_TEXT, m_bOutputSaveText);
	DDX_Check(pDX, IDC_OUTPUT_SAVE_URL_FILTER, m_bOutputSaveUrlFilter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCollectorProp, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CCollectorProp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCollectorProp message handlers

void CGeneralProp::OnGetLinkPath() 
{
	// TODO: Add your control notification handler code here
	if (_BrowserPath(this,m_strLinkPath ) == TRUE) 
		GetDlgItem(IDC_LINK_PATH)->SetWindowText(m_strLinkPath);//UpdateData(FALSE);
}

void CGeneralProp::OnDefaultLinkPath() 
{
	// TODO: Add your control notification handler code here
	CButton *p = (CButton*)GetDlgItem(IDC_DEFAULT_LINKPATH);
	BOOL state = p->GetCheck();
	SetLinkPathState(!state);
}
/////////////////////////////////////////////////////////////////////////////
// CDownProp property page

IMPLEMENT_DYNCREATE(CDownProp, CSAPrefsSubDlg)

CDownProp::CDownProp() : CSAPrefsSubDlg(CDownProp::IDD)
{
	//{{AFX_DATA_INIT(CDownProp)
	m_bDLActivex = FALSE;
	m_bDLApplet = FALSE;
	m_bDLImage = FALSE;
	m_bDLScript = FALSE;
	m_bDLSound = FALSE;
	m_bDLVideo = FALSE;
	m_nDownAll = -1;
	m_bDLAnimation = FALSE;
	m_bDLFlash = FALSE;
	m_nManagerIndex = -1;
	m_strManagerType = _T("");
	//}}AFX_DATA_INIT
}

CDownProp::~CDownProp()
{
}

void CDownProp::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownProp)
	DDX_Control(pDX, IDC_DOWN_MANAGER, m_DownManager);
	DDX_Check(pDX, IDC_DLACTIVEX, m_bDLActivex);
	DDX_Check(pDX, IDC_DLAPPLET, m_bDLApplet);
	DDX_Check(pDX, IDC_DLIMG, m_bDLImage);
	DDX_Check(pDX, IDC_DLJS, m_bDLScript);
	DDX_Check(pDX, IDC_DLSOUND, m_bDLSound);
	DDX_Check(pDX, IDC_DLVIDEO, m_bDLVideo);
	DDX_CBIndex(pDX, IDC_DOWN_ALL, m_nDownAll);
	DDX_Check(pDX, IDC_PLAY_ANIMATION, m_bDLAnimation);
	DDX_Check(pDX, IDC_PLAY_FLASH, m_bDLFlash);
	DDX_CBIndex(pDX, IDC_DOWN_MANAGER, m_nManagerIndex);
	DDX_Text(pDX, IDC_DOWN_TYPE, m_strManagerType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDownProp, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CDownProp)
	ON_BN_CLICKED(IDC_DOWN_EDIT, OnDownEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownProp message handlers

BOOL CDownProp::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogDownProp");
#endif
	CSAPrefsSubDlg::OnInitDialog();
#ifdef _WRITE_LNG_FILE_
	_WriteDlgComboBox(this,"DialogDownProp");
#endif
	LOADDLG("DialogDownProp");

	// TODO: Add extra initialization here
	CString filename = theApp.m_strUser+"DownManager.ini";

	char key[10] = "N";
	TCHAR buf[1024];
	CString str;
	LOADSTR(str ,IDS_NONE);
	m_DownManager.AddString(str);
	int i=1;
	BOOL r = TRUE;
	while(r)
	{
		itoa(i, key+1, 10);
		r = ::GetPrivateProfileString("DownManager", key, _T(""), buf, 1024, filename);
		if (r)
			m_DownManager.AddString(buf);
		i++;
	}
	m_nManagerIndex = pmf->m_nManagerIndex;
	m_DownManager.SetCurSel(m_nManagerIndex);
	m_strManagerType = pmf->m_strManagerType;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDownProp::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CString filename = theApp.m_strUser+"DownManager.ini";

	if (m_nManagerIndex != pmf->m_nManagerIndex)
	{
		char key[10] = "S";
		if (m_nManagerIndex>0)
		{
			CString str;
			TCHAR buf[1024];
			m_DownManager.GetWindowText(str);
			itoa(m_nManagerIndex, key+1, 10);
			::GetPrivateProfileString("DownManager", key, _T(""), buf, 1024, filename);
			str = buf;
			if (str.GetLength()<1)
				m_nManagerIndex = 0;
			else
			{
				str.Replace("\\n","\r\n");
				pmf->m_strManagerScript = str;
			}
		}
		if (m_nManagerIndex==0)
			pmf->m_strManagerScript.Empty();
		//
		pmf->m_nManagerIndex = m_nManagerIndex;
		itoa(m_nManagerIndex, key, 10);
		::WritePrivateProfileString("DownManager","ManagerIndex",key,filename);
	}
	m_strManagerType.MakeUpper();
	if (m_strManagerType != pmf->m_strManagerType)
	{
		::WritePrivateProfileString("DownManager","MonitorFileType",m_strManagerType,filename);
		pmf->m_strManagerType = m_strManagerType ;
	}
	CSAPrefsSubDlg::OnOK();
}

void CDownProp::OnDownEdit() 
{
	// TODO: Add your control notification handler code here
	CString filename = theApp.m_strUser+"DownManager.ini";
	ShellExecute(NULL, "open", filename , "", NULL, SW_SHOW );
}

void COtherProp::OnSetDefault() 
{
	// TODO: Add your control notification handler code here
	pmf->SetAsDefaultBrowser();
}
/////////////////////////////////////////////////////////////////////////////
// CTabStyleProp property page

IMPLEMENT_DYNCREATE(CTabStyleProp, CSAPrefsSubDlg)

CTabStyleProp::CTabStyleProp() : CSAPrefsSubDlg(CTabStyleProp::IDD)
{
	//{{AFX_DATA_INIT(CTabStyleProp)
	m_nTabStyle = -1;
	m_bTabDefaultColor = FALSE;
	m_bTopTab = FALSE;
	//}}AFX_DATA_INIT
}

CTabStyleProp::~CTabStyleProp()
{
}

void CTabStyleProp::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabStyleProp)
	DDX_Control(pDX, IDC_TAB, m_tab);
	DDX_Radio(pDX, IDC_STYLE_NT, m_nTabStyle);
	DDX_Check(pDX, IDC_DEFAULT_COLOR, m_bTabDefaultColor);
	DDX_Check(pDX, IDC_TAB_TOP, m_bTopTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabStyleProp, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CTabStyleProp)
	ON_BN_CLICKED(IDC_UNSEL_COLOR, OnUnselColor)
	ON_BN_CLICKED(IDC_SEL_COLOR, OnSelColor)
	ON_BN_CLICKED(IDC_UNSEL_FONT_COLOR, OnUnselFontColor)
	ON_BN_CLICKED(IDC_SEL_FONT_COLOR, OnSelFontColor)
	ON_BN_CLICKED(IDC_DEFAULT_COLOR, OnDefaultColor)
	ON_BN_CLICKED(IDC_TAB_TOP, OnTabTop)
	ON_BN_CLICKED(IDC_STYLE_NT, OnStyleNt)
	ON_BN_CLICKED(IDC_STYLE_FT, OnStyleNt)
	ON_BN_CLICKED(IDC_STYLE_FB, OnStyleNt)
	ON_BN_CLICKED(IDC_STYLE_SKIN_TAB, OnStyleNt)
	ON_BN_CLICKED(IDC_STYLE_OK_TAB, OnStyleNt)

	ON_BN_CLICKED(IDC_SEP_COLOR, OnSepColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabStyleProp message handlers
BOOL CTabStyleProp::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogTabStyleProp");
#endif
	LOADDLG("DialogTabStyleProp");
	CSAPrefsSubDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	CImageList img;
	img.Create(pmf->m_wndTab.GetImageList());
	m_tab.SetImageList(&img);
	img.Detach();

	// add some dummy pages to the tabctrl
	char tmp[10];
	for (int i=0;i<TI_COUNT;i++)
	{
		sprintf(tmp, "Tab %d", i);
		m_tab.InsertItem(i, tmp, i);
	}

	DWORD dwExStyle= m_tab.GetExtendedStyle();
	m_tab.SetExtendedStyle(dwExStyle  | TCS_EX_FLATSEPARATORS);

	m_tab.SetTabPos(m_bTopTab);
	m_tab.SetTabStyle(m_nTabStyle);
	m_tab.SetFonts();

	m_tab.SetUnSelColor(m_crUnSelColor);
	m_tab.SetUnSelColor(m_crSelColor);//gf
	m_tab.SetUnSelFontColor(m_crUnSelFontColor);
	m_tab.SetSelFontColor(m_crSelFontColor);
	m_tab.SetSepColor(m_crSepColor);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabStyleProp::OnUnselColor() 
{
	// TODO: Add your control notification handler code here
	try{
	CColorDialog colordlg;
	colordlg.m_cc.Flags |= CC_RGBINIT;
	colordlg.m_cc.rgbResult = m_crUnSelColor;
	if(colordlg.DoModal() == IDOK)
	{
		m_crUnSelColor = colordlg.GetColor();
		m_tab.SetUnSelColor(m_crUnSelColor);
		m_tab.Invalidate();
	}
	}catch(...){}
}

void CTabStyleProp::OnSelColor() 
{
	// TODO: Add your control notification handler code here
	try{
	CColorDialog colordlg;
	colordlg.m_cc.Flags |= CC_RGBINIT;
	colordlg.m_cc.rgbResult = m_crSelColor;
	if(colordlg.DoModal() == IDOK)
	{
		m_crSelColor = colordlg.GetColor();
		m_tab.SetSelColor(m_crSelColor);
		m_tab.Invalidate();
	}
	}catch(...){}
}

void CTabStyleProp::OnUnselFontColor() 
{
	// TODO: Add your control notification handler code here
	try{
	CColorDialog colordlg;
	colordlg.m_cc.Flags |= CC_RGBINIT;
	colordlg.m_cc.rgbResult = m_crUnSelFontColor;
	if(colordlg.DoModal() == IDOK)
	{
		m_crUnSelFontColor = colordlg.GetColor();
		m_tab.SetUnSelFontColor(m_crUnSelFontColor);
		m_tab.Invalidate();
	}
	}catch(...){}
}

void CTabStyleProp::OnSelFontColor() 
{
	// TODO: Add your control notification handler code here
	try{
	CColorDialog colordlg;
	colordlg.m_cc.Flags |= CC_RGBINIT;
	colordlg.m_cc.rgbResult = m_crSelFontColor;
	if(colordlg.DoModal() == IDOK)
	{
		m_crSelFontColor = colordlg.GetColor();
		m_tab.SetSelFontColor(m_crSelFontColor);
		m_tab.Invalidate();
	}
	}catch(...){}
}

void CTabStyleProp::OnDefaultColor() 
{
	// TODO: Add your control notification handler code here
	m_crUnSelColor		= -1;
	m_crSelColor		= -1;
	m_crUnSelFontColor	= -1;
	m_crSelFontColor	= -1;
	m_crSepColor		= -1;
	//
	m_tab.SetUnSelColor(m_crUnSelColor);
	m_tab.SetSelColor(m_crSelColor);
	m_tab.SetUnSelFontColor(m_crUnSelFontColor);
	m_tab.SetSelFontColor(m_crSelFontColor);
	m_tab.SetSepColor(m_crSepColor);
	//
	m_tab.Invalidate();
}
void CTabStyleProp::OnStyleNt() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	m_tab.SetTabStyle(m_nTabStyle);
	//m_tab.SetFonts();
	m_tab.Invalidate();
}
void CTabStyleProp::OnTabTop() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_tab.SetTabPos(m_bTopTab);
	m_tab.Invalidate();
}

void CTabStyleProp::OnSepColor() 
{
	// TODO: Add your control notification handler code here
	try{
	CColorDialog colordlg;
	colordlg.m_cc.Flags |= CC_RGBINIT;
	colordlg.m_cc.rgbResult = m_crSepColor;
	if(colordlg.DoModal() == IDOK)
	{
		m_crSepColor = colordlg.GetColor();
		m_tab.SetSepColor(m_crSepColor);
		m_tab.Invalidate();
	}
	}catch(...){}
}

void CDeleteProp::OnDeleteCurrent() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int current = m_list.GetNextSelectedItem(pos);
	if (current<0)
		return;
	m_list.DeleteItem(current);
	LIST_FOCUS_POS(m_list, current);
}

void CDeleteProp::OnDeleteCurrentSearch() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_list2.GetFirstSelectedItemPosition();
	int current = m_list2.GetNextSelectedItem(pos);
	if (current<0)
		return;
	m_list2.DeleteItem(current);
	LIST_FOCUS_POS(m_list2, current);
}
/////////////////////////////////////////////////////////////////////////////
// CFtpProp property page

IMPLEMENT_DYNCREATE(CFtpProp, CSAPrefsSubDlg)

CFtpProp::CFtpProp() : CSAPrefsSubDlg(CFtpProp::IDD)
{
	//{{AFX_DATA_INIT(CFtpProp)
	m_nFtpOpenType = -1;
	m_strFtpExePath = _T("");
	//}}AFX_DATA_INIT
}

CFtpProp::~CFtpProp()
{
}

void CFtpProp::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFtpProp)
	DDX_Radio(pDX, IDC_FTP_OPEN, m_nFtpOpenType);
	DDX_Text(pDX, IDC_EXE_PATH, m_strFtpExePath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFtpProp, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CFtpProp)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_FTP_OPEN, OnFtpOpen)
	ON_BN_CLICKED(IDC_RADIO2, OnFtpOpen)
	ON_BN_CLICKED(IDC_RADIO3, OnFtpOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFtpProp message handlers

void CFtpProp::OnBrowse() 
{
	// TODO: Add your control notification handler code here
	CString str;
	LOADSTR(str ,IDS_TYPE_EXE);
 	CFileDialog dlg(TRUE, "exe", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str);
	if (dlg.DoModal()==IDOK)
	{
		m_strFtpExePath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

BOOL CFtpProp::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogFtpProp");
#endif
	LOADDLG("DialogFtpProp");
	CSAPrefsSubDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetOpenTypeState();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFtpProp::OnFtpOpen() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	SetOpenTypeState();
}

void CFtpProp::SetOpenTypeState()
{
	GetDlgItem(IDC_EXE_PATH)->EnableWindow(m_nFtpOpenType==2);
	GetDlgItem(IDC_BROWSE)->EnableWindow(m_nFtpOpenType==2);
}

void CSaveProp::OnGetSavepathSort() 
{
	// TODO: Add your control notification handler code here
	if (_BrowserPath(this,m_strSavePathSort) == TRUE)
		GetDlgItem(IDC_SAVEPATH_SORT)->SetWindowText(m_strSavePathSort);//UpdateData(FALSE);
}

BOOL CCollectorProp::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogCollectorProp");
#endif
	LOADDLG("DialogCollectorProp");
	CSAPrefsSubDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CConfirmProp::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogConfirmProp");
#endif
	LOADDLG("DialogConfirmProp");
	CSAPrefsSubDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMenuProp::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogMenuProp");
#endif
	LOADDLG("DialogMenuProp");
	CSAPrefsSubDlg::OnInitDialog();

	// TODO: Add extra initialization here
	m_spin1.SetRange(0,200);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSaveProp::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogSaveProp");
#endif
	CSAPrefsSubDlg::OnInitDialog();
#ifdef _WRITE_LNG_FILE_
	_WriteDlgComboBox(this,"DialogSaveProp");
#endif
	LOADDLG("DialogSaveProp");

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CStartProp::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogStartProp");
#endif
	LOADDLG("DialogStartProp");
	CSAPrefsSubDlg::OnInitDialog();
	
	GF_TRY
	{

	
	CGf_StartPageChoice::GetIns().LoadNames();
	((CComboBox*)GetDlgItem(IDC_COMBO_STARTPAGE))->ResetContent();

	for (int i=0;i< CGf_StartPageChoice::GetIns().GetNamesSize(); i++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_STARTPAGE))->InsertString(i, CGf_StartPageChoice::GetIns().GetName(i));
	}

	((CComboBox*)GetDlgItem(IDC_COMBO_STARTPAGE))->SetCurSel( CGf_StartPageChoice::GetIns().GetOldSel());
	
	}
	GF_CATCH

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CStatusBarProp::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogStatusBarProp");
#endif
	LOADDLG("DialogStatusBarProp");
	CSAPrefsSubDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTabProp::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogTabProp");
#endif
	CSAPrefsSubDlg::OnInitDialog();
#ifdef _WRITE_LNG_FILE_
	_WriteDlgComboBox(this,"DialogTabProp");
#endif
	LOADDLG("DialogTabProp");

	// TODO: Add extra initialization here
	_LoadMenuString2ComboBox(pmf->GetMenu(),&m_LDbClickBlank);
	//
	int count = m_LDbClickBlank.GetCount();
	for (int i=0; i<count; i++)
	{
		if (m_LDbClickBlank.GetItemData(i) == (DWORD)m_nLDbClickTabBlank)
		{
			m_LDbClickBlank.SetCurSel(i);
			break;
		}
	}
	//
	_LoadMenuString2ComboBox(pmf->GetMenu(),&m_RDbClickBlank);
	//
	count = m_RDbClickBlank.GetCount();
	for (i=0; i<count; i++)
	{
		if (m_RDbClickBlank.GetItemData(i) == (DWORD)m_nRDbClickTabBlank)
		{
			m_RDbClickBlank.SetCurSel(i);
			break;
		}
	}
	
	m_spin1.SetRange(0,200);
	m_spin2.SetRange(0,200);
	m_spin3.SetRange(0,200);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabProp::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_nLDbClickTabBlank = m_LDbClickBlank.GetItemData(m_LDbClickBlank.GetCurSel());
	m_nRDbClickTabBlank = m_RDbClickBlank.GetItemData(m_RDbClickBlank.GetCurSel());

	CSAPrefsSubDlg::OnOK();
}

BOOL CWindowProp::OnInitDialog() 
{
#ifdef _WRITE_LNG_FILE_
	_WriteDlgString(this,"DialogWindowProp");
#endif
	CSAPrefsSubDlg::OnInitDialog();
#ifdef _WRITE_LNG_FILE_
	_WriteDlgComboBox(this,"DialogWindowProp");
#endif
	LOADDLG("DialogWindowProp");
	
	// TODO: Add extra initialization here
	m_spin1.SetRange(0,200);
	m_spin2.SetRange(0,200);
	m_spin3.SetRange(5,255);
	m_spin4.SetRange(5,255);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStartProp::OnButtonHomeCur() 
{
	//GetDlgItem(IDC_EDIT_HOME_URL)->SetWindowText(pmf->GetHomePage() );	
	if (!m_strStartHomeURL.IsEmpty())
	{
		m_strStartHomeURL+="\r\n";
	}

	m_strStartHomeURL+=pmf->GetCurrentUrl();
	UpdateData(FALSE);
}

void CStartProp::OnButtonHomeNone() 
{
	m_strStartHomeURL="";
	UpdateData(FALSE);	
}

void CStartProp::OnButtonHomeBlank() 
{
	m_strStartHomeURL="about:blank";
	UpdateData(FALSE);		
}

void CStartProp::OnButtonHomeGetIe() 
{
	m_strStartHomeURL=pmf->GetHomePage();
	UpdateData(FALSE);		
}

void CStartProp::OnOK()
{

	GF_TRY{
	int npos=((CComboBox*)GetDlgItem(IDC_COMBO_STARTPAGE))->GetCurSel();
	CGf_StartPageChoice::GetIns().SetSel(npos);

	}GF_CATCH
}
