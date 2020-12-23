#if !defined(AFX_MyIE9OPTIONS_H__9DC8E053_7252_11D4_9BF5_0000E85300AE__INCLUDED_)
#define AFX_MyIE9OPTIONS_H__9DC8E053_7252_11D4_9BF5_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SAPrefsDialog.h"
#include "AdvTabCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CWindowProp dialog

class CWindowProp : public CSAPrefsSubDlg
{
	DECLARE_DYNCREATE(CWindowProp)

// Construction
public:
	CWindowProp();
	~CWindowProp();

// Dialog Data
	//{{AFX_DATA(CWindowProp)
	enum { IDD = IDD_PROPPAGE_WINDOW };
	CSpinButtonCtrl	m_spin4;
	CSpinButtonCtrl	m_spin3;
	CSpinButtonCtrl	m_spin2;
	CSpinButtonCtrl	m_spin1;
	BOOL	m_bActiveNew;
	int		m_nInterval;
	int		m_nMaxWins;
	BOOL	m_bOpenFolderOut;
	int		m_nCollectorTrans;
	int		m_nMonitorTrans;
	int		m_nOpenNew;
	int		m_nNewSequence;
	int		m_nCloseSequence;
	BOOL	m_nNewAddress;
	BOOL	m_bNewFav;
	BOOL	m_bNewHistory;
	BOOL	m_bNewLinks;
	BOOL	m_bNewLocal;
	BOOL	m_bActiveFileNew;
	BOOL	m_bAutoRefreshActive;
	BOOL	m_bKeepOnePage;
	BOOL	m_bBlankMonitorClip;
	BOOL	m_bActiveNewAddress;
	BOOL	m_bNewHome;
	BOOL	m_bUseTargetFrameName;
	BOOL	m_bReloadUnShownImage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWindowProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWindowProp)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabProp dialog

class CTabProp : public CSAPrefsSubDlg
{
	DECLARE_DYNCREATE(CTabProp)

// Construction
public:
	int		m_nLDbClickTabBlank;
	int		m_nRDbClickTabBlank;
	CTabProp();
	~CTabProp();

// Dialog Data
	//{{AFX_DATA(CTabProp)
	enum { IDD = IDD_PROPPAGE_TAB };
	CSpinButtonCtrl	m_spin3;
	CSpinButtonCtrl	m_spin2;
	CSpinButtonCtrl	m_spin1;
	CComboBox	m_RDbClickBlank;
	CComboBox	m_LDbClickBlank;
	BOOL	m_bAutoFitTab;
	UINT	m_nMaxChars;
	int		m_nMaxWidth;
	int		m_nMinWidth;
	BOOL	m_bMultiTab;
	int		m_nLDBClick;
	int		m_nMCLICK;
	BOOL	m_bShowIndex;
	int		m_nRClick;
	int		m_bClickClose;
	BOOL	m_bWheelOnTab;
	BOOL	m_bWheelTab;
	BOOL	m_bShowDot;
	BOOL m_bShowX;
	BOOL	m_bShowFavIcon;
	BOOL	m_bShowRead;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabProp)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabProp)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CGeneralProp dialog

class CGeneralProp : public CSAPrefsSubDlg
{
	DECLARE_DYNCREATE(CGeneralProp)

// Construction
public:
	void SetFavPathState(BOOL state);
	void SetPageSizeState(BOOL state);
	void SetLinkPathState(BOOL state);
	CGeneralProp();
	~CGeneralProp();

// Dialog Data
	//{{AFX_DATA(CGeneralProp)
	enum { IDD = IDD_PROPPAGE_GENERAL };
	BOOL	m_bAniIcon;
	BOOL	m_bAutoHideFavSideBar;

	BOOL _bEnableIE8Mode;
	BOOL _bAlwaysIEMenuRightClickImg;
	BOOL _bDirectOpenForOneWord;
	BOOL	m_bTopCloseExitApp;

	CString	m_strSkinPath;
	CString	m_strGroupPath;

	CString	m_strFavPath;
	CString m_strUserAppTitle;
	BOOL	m_bUseDefaultFavPath;
	BOOL	m_bForbidMainTitle;

	CString	m_strLinkPath;
	BOOL	m_bUseDefaultLinkPath;



	//gfsize
	BOOL	_bKeepPageSize;
	BOOL	_bForbidAutoUpdate;
	int _iPageSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGeneralProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGeneralProp)
	afx_msg void OnGetSkinpath();
	afx_msg void OnGetGrouppath();
	afx_msg void OnShowTray();
	virtual BOOL OnInitDialog();
	afx_msg void OnGetFavPath();

	afx_msg void OnSetToIEFavPath(); //gffave path
	afx_msg void OnOpenIEFavPath(); //gffave path
	afx_msg void OnKeepPageSize();

	afx_msg void OnDefaultFavPath();
	afx_msg void OnGetLinkPath();
	afx_msg void OnDefaultLinkPath();

	afx_msg void OnEnaleIE8Mode();//gf ie8
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// COtherProp dialog

class COtherProp : public CSAPrefsSubDlg
{
	DECLARE_DYNCREATE(COtherProp)

// Construction
public:
	WORD m_OldModifiers, m_OldKey;
	WORD Mod2HotKey(WORD key);
	WORD HotKey2Mod(WORD key);
	COtherProp();
	~COtherProp();

// Dialog Data
	//{{AFX_DATA(COtherProp)
	enum { IDD = IDD_PROPPAGE_OTHERS };
	CHotKeyCtrl	m_HotKey;
	CString	m_strCE1;
	CString	m_strCE2;
	CString	m_strCSE1;
	CString	m_strCSE2;
	CString	m_strSE1;
	CString	m_strSE2;
	int		m_nMenuStyle;
	BOOL	m_bMinTray;
	BOOL	m_bCloseTray;
	BOOL	m_bShowTray;
	BOOL	m_bCloseAllWhenCloseToTray;
	BOOL	_bOpenWordOutOfIE;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COtherProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COtherProp)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowTray();
	afx_msg void OnUpdateHotkey();
	afx_msg void OnSetDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CSaveProp dialog

class CSaveProp : public CSAPrefsSubDlg
{
	DECLARE_DYNCREATE(CSaveProp)

// Construction
public:
	CSaveProp();
	~CSaveProp();

// Dialog Data
	//{{AFX_DATA(CSaveProp)
	enum { IDD = IDD_PROPPAGE_SAVE };
	CString	m_strSavePath;
	CString	m_strImagePath;
	int		m_nAutoSave;
	BOOL	m_bCateWithSite;
	BOOL	m_bImgCateWithSite;
	CString	m_strSavePathSort;
	BOOL	m_bNameByTitle;
	BOOL	m_bSavePageByDate;
	BOOL	m_bShowSortSaveDlg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSaveProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSaveProp)
	afx_msg void OnGetSavepath();
	afx_msg void OnGetImagepath();
	afx_msg void OnGetSavepathSort();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CMouseProp dialog

class CMouseProp : public CSAPrefsSubDlg
{
	DECLARE_DYNCREATE(CMouseProp)

// Construction
public:
	CMouseProp();
	~CMouseProp();

// Dialog Data
	//{{AFX_DATA(CMouseProp)
	enum { IDD = IDD_PROPPAGE_MOUSE };
	int		m_nImgDown;
	int		m_nImgLeft;
	int		m_nImgRight;
	int		m_nImgUp;
	int		m_nLinkDown;
	int		m_nLinkLeft;
	int		m_nLinkRight;
	int		m_nLinkUp;
	int		m_nTextDown;
	int		m_nTextLeft;
	int		m_nTextRight;
	int		m_nTextUp;
	BOOL	m_bSimuDragLink;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMouseProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMouseProp)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_MyIE9OPTIONS_H__9DC8E053_7252_11D4_9BF5_0000E85300AE__INCLUDED_)
/////////////////////////////////////////////////////////////////////////////
// CGestureProp dialog

class CGestureProp : public CSAPrefsSubDlg
{
	DECLARE_DYNCREATE(CGestureProp)
public:
	int		m_nLastIdx;
	int		m_nMButtonDown;
	int		m_nLButtonDown;
	int		m_nRButtonDown;
	int		m_nLDbClick;
	int		m_nRDbClick;
	int		m_nLeftRight;
	int		m_nRightLeft;
	//
	void GetMenuItemString(CString &strName, int nID);
	void InputOneGesture(int id);
	CGestureProp();
	~CGestureProp();

// Dialog Data
	//{{AFX_DATA(CGestureProp)
	enum { IDD = IDD_PROPPAGE_GESTURE };
	CEdit	m_gesture;
	CComboBox	m_command;
	CListCtrl	m_list;
	BOOL	m_bShowGesture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGestureProp)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGestureProp)
	afx_msg void OnNew();
	afx_msg void OnUpdate();
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg void OnLeft();
	afx_msg void OnRight();
	afx_msg void OnDelete();
	afx_msg void OnBack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CToolbarProp dialog
class CToolbarProp : public CSAPrefsSubDlg
{
	DECLARE_DYNCREATE(CToolbarProp)
public:
	DWORD m_dwFsShow;
	DWORD m_dwAhShow;
	CToolbarProp();
	~CToolbarProp();

// Dialog Data
	//{{AFX_DATA(CToolbarProp)
	enum { IDD = IDD_PROPPAGE_TOOLBAR };
	BOOL	m_bShowToolbarLabel;
	BOOL	m_bComboShowIcon;
	BOOL	m_bGoProxyMenu;
	BOOL	m_bShowSearchBarButton;
	BOOL	m_bShowAddressBarButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolbarProp)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolbarProp)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CMenuProp dialog

class CMenuProp : public CSAPrefsSubDlg
{
	DECLARE_DYNCREATE(CMenuProp)

// Construction
public:
	CMenuProp();
	~CMenuProp();

// Dialog Data
	//{{AFX_DATA(CMenuProp)
	enum { IDD = IDD_PROPPAGE_MENU };
	CSpinButtonCtrl	m_spin1;
	BOOL	m_bSingleMenu;
	BOOL	m_bAddFav;
	BOOL	m_bDisplayOpenAll;
	BOOL	m_bCompactFavBar;
	BOOL	m_bFavShowUrl;
	BOOL	m_bShowHidden;
	int		m_nMenuSize;
	int		m_nMenuSizeOther;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMenuProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMenuProp)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CStartProp dialog

class CStartProp : public CSAPrefsSubDlg
{
	DECLARE_DYNCREATE(CStartProp)

// Construction
public:
	virtual void OnOK();
	CStartProp();
	~CStartProp();

// Dialog Data
	//{{AFX_DATA(CStartProp)
	enum { IDD = IDD_PROPPAGE_START };
	BOOL	m_bCloseCleanCache;
	BOOL	m_bCloseCleanCookies;
	BOOL	m_bCloseCleanFormData;
	BOOL	m_bCloseCleanHistory;
	BOOL	m_bCloseCleanTypedAddress;
	BOOL	m_bCloseCleanTypedSearch;
	BOOL	m_bCloseCleanUndoClose;
	BOOL	m_bMenuLoadLastClose;
	int		m_nStartMode;
	BOOL	m_bOneInstance;	
	BOOL	m_bAutoRun;
	BOOL	m_bLockHomepage;
	CString	m_strStartHomeURL;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStartProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStartProp)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonHomeCur();
	afx_msg void OnButtonHomeNone();
	afx_msg void OnButtonHomeBlank();
	afx_msg void OnButtonHomeGetIe();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CDeleteProp dialog

class CDeleteProp : public CSAPrefsSubDlg
{
	DECLARE_DYNCREATE(CDeleteProp)

// Construction
public:
	CDeleteProp();
	~CDeleteProp();

// Dialog Data
	//{{AFX_DATA(CDeleteProp)
	enum { IDD = IDD_PROPPAGE_DELETE };
	CListCtrl	m_list2;
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDeleteProp)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDeleteProp)
	afx_msg void OnDelete();
	afx_msg void OnDeleteall();
	afx_msg void OnDeleteSearch();
	afx_msg void OnDeleteallSearch();
	virtual BOOL OnInitDialog();
	afx_msg void OnEdit();
	afx_msg void OnEditSearch();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteCurrent();
	afx_msg void OnDeleteCurrentSearch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CStatusBarProp dialog

class CStatusBarProp : public CSAPrefsSubDlg
{
	DECLARE_DYNCREATE(CStatusBarProp)

// Construction
public:
	CStatusBarProp();
	~CStatusBarProp();

// Dialog Data
	//{{AFX_DATA(CStatusBarProp)
	enum { IDD = IDD_PROPPAGE_STATUSBAR };
	CString	m_strStatusStringFormat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatusBarProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStatusBarProp)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CConfirmProp dialog

class CConfirmProp : public CSAPrefsSubDlg
{
	DECLARE_DYNCREATE(CConfirmProp)

// Construction
public:
	CConfirmProp();
	~CConfirmProp();

// Dialog Data
	//{{AFX_DATA(CConfirmProp)
	enum { IDD = IDD_PROPPAGE_CONFIRM };
	BOOL	m_bConfirmExit;
	BOOL	m_bConfirmCloseAll;
	BOOL	m_bConfirmCloseLock;
	BOOL	m_bConfirmExitDowning;
	BOOL	m_bConfirmCloseAllKeepCur;
	BOOL	m_bConfirmUnsafePage;
	BOOL	_bEnableMixContentAlert;
	BOOL	m_bKeepSilent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConfirmProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConfirmProp)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CCollectorProp dialog

class CCollectorProp : public CSAPrefsSubDlg
{
	DECLARE_DYNCREATE(CCollectorProp)

// Construction
public:
	CCollectorProp();
	~CCollectorProp();

// Dialog Data
	//{{AFX_DATA(CCollectorProp)
	enum { IDD = IDD_PROPPAGE_COLLECTOR };
	BOOL	m_bOutputSaveTime;
	BOOL	m_bOutputSaveFilter;
	BOOL	m_bOutputSaveImg;
	BOOL	m_bOutputSavePage;
	BOOL	m_bOutputAutoClean;
	int		m_bCollectorAddtoBottom;
	BOOL	m_bOutputSaveText;
	BOOL	m_bOutputSaveUrlFilter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCollectorProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCollectorProp)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CDownProp dialog

class CDownProp : public CSAPrefsSubDlg
{
	DECLARE_DYNCREATE(CDownProp)

// Construction
public:
	CDownProp();
	~CDownProp();

// Dialog Data
	//{{AFX_DATA(CDownProp)
	enum { IDD = IDD_PROPPAGE_DOWNLOAD };
	CComboBox	m_DownManager;
	BOOL	m_bDLActivex;
	BOOL	m_bDLApplet;
	BOOL	m_bDLImage;
	BOOL	m_bDLScript;
	BOOL	m_bDLSound;
	BOOL	m_bDLVideo;
	int		m_nDownAll;
	BOOL	m_bDLAnimation;
	BOOL	m_bDLFlash;
	int		m_nManagerIndex;
	CString	m_strManagerType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDownProp)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDownProp)
	virtual BOOL OnInitDialog();
	afx_msg void OnDownEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabStyleProp dialog

class CTabStyleProp : public CSAPrefsSubDlg
{
	DECLARE_DYNCREATE(CTabStyleProp)

// Construction
public:
	COLORREF m_crUnSelColor;
	COLORREF m_crSelColor; //gf
	COLORREF m_crUnSelFontColor;
	COLORREF m_crSelFontColor;
	COLORREF m_crSepColor;

	//void UpdateTabStyle();
	CTabStyleProp();
	~CTabStyleProp();

// Dialog Data
	//{{AFX_DATA(CTabStyleProp)
	enum { IDD = IDD_PROPPAGE_TABSTYLE };
	CAdvTabCtrl	m_tab;
	int		m_nTabStyle;
	BOOL	m_bTabDefaultColor;
	BOOL	m_bTopTab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabStyleProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabStyleProp)
	afx_msg void OnUnselColor();
	afx_msg void OnSelColor();
	afx_msg void OnUnselFontColor();
	afx_msg void OnSelFontColor();
	afx_msg void OnDefaultColor();
	virtual BOOL OnInitDialog();
	afx_msg void OnTabTop();
	afx_msg void OnStyleNt();
	afx_msg void OnSepColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CFtpProp dialog

class CFtpProp : public CSAPrefsSubDlg
{
	DECLARE_DYNCREATE(CFtpProp)

// Construction
public:
	void SetOpenTypeState();	 
	CFtpProp();
	~CFtpProp();

// Dialog Data
	//{{AFX_DATA(CFtpProp)
	enum { IDD = IDD_PROPPAGE_FTP };
	int		m_nFtpOpenType;
	CString	m_strFtpExePath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFtpProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFtpProp)
	afx_msg void OnBrowse();
	virtual BOOL OnInitDialog();
	afx_msg void OnFtpOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
