#include "stdafx.h"
#include "MyIE9.h"
#include <INITGUID.H> 
#include <Mshtmhst.h>
#include <wininet.h>
#include <ExDisp.h>// For ShellUIHelper
//#include <comdef.h>// For Compiler COM Support Classes
#include <UrlHist.h>
#include "popupdlg.h"
#include "MainFrm.h"
#include "setproxy.h"
#include "externalUtils.h"
#include "HistoryTree.h"
#include "FavTree.h"
#include "ShellTree.h"
#include "ToolMenu.h"
#include "MenuData.h"
//#include <afxsock.h>
#include <Winsock.h>
#include "setscdlg.h"
#include "setaliasdlg.h"
#include "customizetoolbar.h"
#include "MyIE9Options.h"
#include "WebFileOpenDlg.h"
#include "InputPassword.h"
#include "Resourcebar.h"
#include "MenuOrder.h"
#include "OrgGroupDlg.h"
#include "GroupUrlDlg.h"
#include "MAPI.h"
#include "SimpleSAH.h"
#include "SelProxyCatDlg.h"
#include "AnimateIcon.h"
#include "QuickSearchDlg.h"
#include "Crypt.h"
#include "CollectorDlg.h"
#include "LinkListDlg.h"
#include "Monitor.h"
#include "AddToThisFavDlg.h"
#include "inputalias.h"
#include "urlfilterprop.h"
#include "rasspdm.h"//ras speed 
#include "GroupSelectDlg.h"
#include "SelectURL.h"
#include "PopupConfirm.h"
#include <mmsystem.h>//for sndplaysound
#include <Mshtmdid.h>
#include "AddPopupDlg.h"
#include "InputDlg.h"
#include "SetFormDataDlg.h"
#include "SAPrefsDialog.h"
#include "CleanAll.h"
#include <direct.h>//mkdir
#include "SelectLang.h"

#include "RegHelper.h" //gff
#include "PathHelper.h"
#include "GfPathManager.h"
#include "FilterSites.h"
#include "GfAppString.h"

#include "Gf_UI.h"
#include "Consts_gf.h"
#include "ConfigLoaded.h"

#ifdef _WRITE_LNG_FILE_
#include "SortSaveDlg.h"
#include "ProxyInfo.h"
#include "SetRefIntervalDlg.h"
#endif

#include "ExpMenu.h"
#include <afxinet.h>//for CInternetSession

#include "HelperException.h"

#include "HelperStr.h"
#include "GFBusiness.h"

#include "Gf_Update.h"
#include "Gf_TranslateUrl.h"
#include "Gf_ShareConfig.h"
#include "Gf_Config.h"
#include "Gf_IE.h"
#include "Gf_AppStatic.h"
#include "AddFlyDlg.h"
#include "FlyDlg.h"
#include "Gf_html.h"
#include "Gf_Win.h"
#include "Gf_AppStatic.h"
#include "Gf_IniFile.h"
#include "Gf_StartupManager.h"

#include "Gf_SaveForm.h"
#include "Gf_FillForm.h"
#include "Gf_StartPageMgr.h"
#include "Gf_UpdateFilterIniControl.h"
//##############################################################
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//##############################################################
//Define the command group GUID for the WebBrowser control 
//It is undocumented, and supose to be changed in the feature
DEFINE_GUID(CGID_IWebBrowser,0xED016940L,0xBD5B,0x11cf,0xBA,
			0x4E,0x00,0xC0,0x4F,0xD7,0x08,0x16); 
static const IID IID_ITravelLogStg	= { 0x7EBFDD80, 0xAD18, 0x11d3, {0xA4,0xC5,0x00,0xC0,0x4F,0x72,0xD6,0xB8}};
//track pop menu
#define TPM_RECURSE         0x0001L

//call back
typedef HRESULT (CALLBACK* LPFNDLLFUNC1)(HWND ,DWORD);
typedef BOOL (CALLBACK* LPFNDLLFUNC2)(HWND ,LPTSTR, int, BOOL);
typedef ULONG (CALLBACK* LPMAPISendDocuments)(ULONG, LPTSTR, LPTSTR, LPTSTR, ULONG);
typedef LONG (CALLBACK* GETRES)(int);

//tab
#define TABPADDINGX 6
#define TABPADDINGY 4
#define FBPADDINGX 3
#define FBPADDINGY 2
//some else put here
#define PROXYID		2
#define TAB_HEIGHT	19
#define MAX_GO_UP	10
//fav delete start pos
#define FAV_START	4
//search type
#define ST_SEARCHBAR	1
#define ST_ADDRBAR		2
#define ST_DRAGMENU		3
//drag drop menu type
#define DT_IMAGE		1
#define DT_LINK			2
#define DT_TEXT			3

//##############################################################
typedef struct tagBandInfo
{
	UINT nIndex;
	UINT nID;
	UINT fStyle;
	UINT cx;
}BandInfo;

#define RBBS_USECHEVRON     0x00000200  // display drop-down button for this band if it's sized smaller than ideal width
BandInfo g_BandInfo[] = {
	{ 0, ID_MENU_BAR,		RBBS_FIXEDBMP | RBBS_GRIPPERALWAYS | RBBS_USECHEVRON,			100 },
	{ 1, ID_SYSMENU_BAR,	RBBS_FIXEDBMP | RBBS_BREAK | RBBS_GRIPPERALWAYS, 100 },
	{ 2, ID_VIEW_TOOLBAR,	RBBS_FIXEDBMP | RBBS_BREAK | RBBS_GRIPPERALWAYS | RBBS_USECHEVRON, 100 },
	{ 3, ID_VIEW_ADDRESSBAR,RBBS_FIXEDBMP | RBBS_BREAK | RBBS_GRIPPERALWAYS, 100 },
	{ 4, ID_VIEW_LINKS,		RBBS_FIXEDBMP | RBBS_BREAK | RBBS_GRIPPERALWAYS | RBBS_USECHEVRON, 100 },
	{ 5, ID_VIEW_FAVS,		RBBS_FIXEDBMP | RBBS_BREAK | RBBS_GRIPPERALWAYS | RBBS_USECHEVRON, 100 },
	{ 6, ID_VIEW_EX_TOOL,	RBBS_FIXEDBMP | RBBS_BREAK | RBBS_GRIPPERALWAYS | RBBS_USECHEVRON, 100 },
	{ 7, ID_VIEW_SEARCH,	RBBS_FIXEDBMP | RBBS_BREAK | RBBS_GRIPPERALWAYS, 100 },
	{ 8, ID_VIEW_TASKBAR,	RBBS_FIXEDBMP | RBBS_BREAK | RBBS_GRIPPERALWAYS | RBBS_USECHEVRON, 100 }
};

//##############################################################
#define MAX_BAR	10
UINT g_BarInfo[MAX_BAR] = {
	ID_VIEW_TOOLBAR,
		ID_VIEW_EX_TOOL,
		ID_VIEW_ADDRESSBAR,
		ID_VIEW_SEARCH,
		ID_VIEW_FAVS,
		ID_VIEW_LINKS,
		ID_VIEW_TASKBAR,
		ID_SYSMENU_BAR,
		ID_MENU_BAR,
		ID_VIEW_STATUS_BAR
};

//##############################################################
#define IS_ENV_URL(str)		(str.GetAt(0)=='%')
#define DO_ENV_URL(str);	\
	if ( IS_ENV_URL(str) )	\
{	\
	TCHAR           szPath[MAX_PATH];	\
	ExpandEnvironmentStrings(str, szPath, MAX_PATH);	\
	str = szPath;	\
	}

#define DO_LOCAL_URL(str);	\
	if (m_bOpenFolderOut)	\
{	\
	if (IS_LOCAL_FOLDER(str))	\
{	\
	ShellExecute(NULL, "open", str , "", NULL, SW_SHOW );	\
	return;	\
	}	\
	}

//www.netcaptor.com
//bbs.netcaptor.com
//www.abc.com
//bbs.abc.com
//abc.cn

//abc.com
//abc.com.cn
//abc.net
//abc.org
#define DO_QUICK_URL(str);	\
	if(str.Find(':') < 0)	\
{	\
	int nDot = str.Find('.');	\
	if( nDot > 0)	\
{	\
	CString strLower = str;	\
	strLower.MakeLower();	\
	if (strLower.Left(4)!="www.")	\
{	\
				if (strLower.Mid(nDot,4)==".com" || strLower.Mid(nDot,4)==".net" ||	strLower.Mid(nDot,4)==".edu" ||	strLower.Mid(nDot,4)==".org" )	\
{	\
	TCHAR ch = strLower.GetAt(nDot+4);	\
	if (ch == 0 || ch == '.')	\
	str = "www." + str;	\
				}	\
	}	\
	}	\
	}

//##############################################################
#define SEARCH_STR(str)	\
	if (str.GetLength()) \
{\
	m_wndSearch->GetEditCtrl()->SetWindowText(str);	\
	m_nSearchBarAction = (PRESS_SHIFT ? 4 : 1);	\
	SendMessage(WM_COMMAND, ID_SEARCH_OK); \
	}

//##############################################################
#define DO_CMD_STR(strUrl);	\
	if(strUrl.Left(1) == '\"')	\
	strUrl = strUrl.Mid(1);	\
	if(strUrl.Right(1) == '\"')	\
	strUrl = strUrl.Left(strUrl.GetLength()-1);	\
	DO_ENV_URL(strUrl);	\
DO_RELATIVE_URL(strUrl);

#define DO_SEARCH_STR(url,keyw)	\
	url.Replace("%c", pmf->GetDomain() );  \
	if (url.Find("%u") > -1)	\
{	\
	_Ascii2Utf8(keyw);	\
	url.Replace("%u", keyw);	\
	}	\
	else	\
{	\
	_AsciiFormat(keyw);	\
	url.Replace("%s", keyw);	\
	}

#define GET_ACTIVE	\
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();	\
if (!tcf || !tcf->m_pView) return;

#define IS_FILTER_ENABLE (g_bKillPop|m_bUseUrlFilter|m_bUseDialogFilter|m_bUseFlyingFilter)

//######################变量名后加MyIE99，以免冲掉网页中本身的变量########################################
#define STR_JAVASCRIPT	"JavaScript"

#define STR_RELOAD_UNSHOWN_IMAGE	\
" function cancelErrorMyIE99() {return true;} onerror=cancelErrorMyIE99; \
 { \
 var iMyIE99,lenMyIE99,srcMyIE99,imgMyIE99; \
 lenMyIE99=document.images.length; \
 for(iMyIE99=0;iMyIE99<lenMyIE99;iMyIE99++) \
 { \
  imgMyIE99=document.images[iMyIE99]; \
  if(imgMyIE99.fileSize<0) \
  { srcMyIE99=imgMyIE99.src; imgMyIE99.src=''; imgMyIE99.src=srcMyIE99; } \
 } }"

	//if(!imgMyIE99.complete||imgMyIE99.fileSize<0) \

#define STR_KILL_BLANK	\
	"	function cancelError() {return true;} onerror=cancelError; \
	function killTag(tagName) \
	{ \
	var url; \
	var objs=document.getElementsByTagName(tagName); \
	for(i=0;i<objs.length;i++) \
	{ \
	url = tagName=='object'?objs[i].movie:objs[i].src; \
	if (url.indexOf('://')<0) \
	{ \
	url = document.URL.substring(0,document.URL.lastIndexOf('/')+1); \
	url += tagName=='object'?objs[i].movie:objs[i].src; \
	} \
	if(external.gbIsAdContent(url)) \
	{ \
				objs[i].outerHTML='$AD'; \
				i--; \
				} \
				} \
				} \
				killTag('iframe');killTag('img');killTag('embed');killTag('object');"
				
#define STR_KILL_FLYING	\
"	function cancelErrorMyIE99() {return true;} \
onerror=cancelErrorMyIE99; \
function KillFlyMyIE99() \
{ \
{ \
var objsMyIE99=document.all; \
var lMyIE99=objsMyIE99.length; \
for (iMyIE99=0;iMyIE99<lMyIE99;iMyIE99++) \
{ \
if (objsMyIE99[iMyIE99].style.position=='absolute' && objsMyIE99[iMyIE99].style.display != 'none') \
objsMyIE99[iMyIE99].style.visibility='hidden'; \
} \
} \
if( window.frames != null ) \
{ \
for(ifrmMyIE99 = 0; ifrmMyIE99 < window.frames.length; ifrmMyIE99++) \
{ \
var objsMyIE99=window.frames(ifrmMyIE99).document.all; \
var lMyIE99=objsMyIE99.length; \
for (iMyIE99=0;iMyIE99<lMyIE99;iMyIE99++) \
{ \
if (objsMyIE99[iMyIE99].style.position=='absolute'  && objsMyIE99[iMyIE99].style.display != 'none') \
objsMyIE99[iMyIE99].style.visibility='hidden'; \
} \
} \
} \
} \
window.setTimeout(KillFlyMyIE99,500);"
				
				//##############################################################
#define MARKED(pView)		(pView->m_nOldIcon==TI_MARK)


				
				//##############################################################
				CString GF_TITILE_UNCHANGE="MyIE9";

				CMainFrame* pmf = NULL;
				//fav
				CStringArray g_astrLinks;
				CStringArray g_astrFavs;
				//filter
				CMapStringToPtr g_mapUrlFilter;
				CStringArray	g_aUrlPattern;
				CMapStringToPtr g_mapUrlUnFilter;
				CStringArray	g_aUrlUnPattern;
				
				float g_fSeed = 1;
				GETRES g_pGetRes = NULL;
				static char g_szAddr[256];
				char g_szFile[1024];
				BSTR g_bstrYes;
				char*	g_strLngBuf=NULL;
				
				int g_nPercent = -1;
				int g_bMax;  //1. max, 0, unmax, 2, tile/cascade, 3, restore, 4 minimize
				int g_nMaxWins = 100;
				int g_nNextTab = -1;
				int g_nDefFontSize = 3;
				int g_nTrayIconID;
				int	g_nOldNewSequence = 1;//back the value when open fav all
				int	g_nSearchType=0;//0:search bar, 1:address bar, 2: all search of text drag menu
				int	g_nDragType=0;
				int	g_nBarVisible=0;
				int	g_nStartPage=0;
				
				BOOL g_bConfirmExit;
				BOOL g_bConfirmExitDowning;
				BOOL g_bExitAtOnce=FALSE;
				BOOL g_bSingleMenu = FALSE;
				BOOL g_bSilent = TRUE;
				BOOL g_bDisScrollText = FALSE;
				BOOL g_bFilteBlank = FALSE;
				BOOL g_bKillListWin;
				BOOL g_bKillPop,g_bKillAutoWin,g_bKillDupWin;
				BOOL g_bLinkInNewWindow, g_bFileInNewWindow, g_bFavInNewWindow, g_bHisInNewWindow;
				BOOL g_bAddressInNewWindow, g_bHomeInNewWindow;
				BOOL g_bCompactFavBar = FALSE;
				BOOL g_bConfirmCloseAll = TRUE;
				BOOL g_bConfirmCloseAllKeepCur = TRUE;
				BOOL g_bConfirmCloseLock;
				BOOL g_bForbidMult;
				BOOL g_bTopCloseExitApp;
				BOOL g_bRTab=0;
				BOOL g_bVerbose;
				BOOL g_bAutoSetBG, g_bLockHomepage;
				BOOL g_bProxyDirty = FALSE;
				
				//for tab
				BOOL g_bShowIndex; //show 1,2,3... at tab head
				BOOL g_bClickClose;//click icon close page
				BOOL g_bShowDot;
				BOOL g_bShowRead;
				BOOL g_bShowFavIcon;
				BOOL g_bRegisterHotKeyOk;
				UINT g_nGroupSize;
				
				CString g_strIeProxy;
				CString g_strLastUrl;//temp url before pop menu when drag
				CString g_strLastText;//temp url before pop menu when drag
				CString g_strMenuUrl;//sometime can not get fav item's url (fav bar's chev menu), but onmenuselect can get
				//
				CString g_strGoUpUrl[MAX_GO_UP];
				CString g_strKillFlyingCode;
				CString g_strKillFlyingLang;
				CString g_strKillBlankCode;
				
				/////////////////////////////////////////////////////////////////////////////
				// CMainFrame
				
				IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)
					
					const UINT MSWHELL_ROLLMSG = RegisterWindowMessage( "MSWHEEL_ROLLMSG" );
				BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
					ON_WM_CREATE()
					ON_COMMAND(ID_GO_BACK_AND_CLOSE, OnGoBackAndClose)
					ON_COMMAND(ID_FILE_NEW_BLANK, OnFileNewBlank)
					ON_COMMAND(ID_FILE_NEW_CURRENTPAGE, OnFileNewCurrentpage)
					ON_COMMAND(ID_VIEW_ADDRESSBAR, OnViewAddressbar)
					ON_COMMAND(ID_VIEW_TASKBAR, OnViewTaskbar)
					ON_COMMAND(ID_FILE_CLOSEALL, OnFileCloseall)
					ON_COMMAND(ID_FILE_WORKOFFLINE, OnFileWorkoffline)
					ON_WM_CLOSE()
					ON_COMMAND(ID_VIEW_SOURCE, OnViewSource)
					ON_COMMAND(ID_FILE_CLOSE_1, OnFileClose)
					ON_COMMAND(ID_APP_EXIT, OnExitApp)
					ON_COMMAND(ID_TOP_CLOSE, OnExitApp)
					ON_COMMAND(ID_FILE_CLOSE_2, OnFileClose2)
					ON_COMMAND(ID_TOOLS_INTERNETOPTIONS, OnToolsInternetoptions)
					ON_COMMAND(ID_OPTIONS_SET_DEFAULT, OnSetAsDefaultBrowser)
					ON_COMMAND(ID_FAVORITES_ADDTOFAVORITES, OnFavoritesAddtofavorites)
					ON_COMMAND(ID_FAVORITES_EXPORTFAVORITES, OnFavoritesExportfavorites)
					ON_COMMAND(ID_FAVORITES_IMPORTFAVORITES, OnFavoritesImportfavorites)
					ON_COMMAND(ID_FAVORITES_ORGNIZEFAVORITES, OnFavoritesOrgnizefavorites)
					ON_COMMAND(ID_TOOLS_ADDTOPOPUPFILTER, OnToolsAddtopopupfilter)
					ON_COMMAND(ID_RTAB_ADDTOPOPUPFILTER, OnRtabAddtopopupfilter)
					ON_COMMAND(ID_RTAB_ADDTOFLYFILTER, OnRtabAddtoFlyfilter)
					
					ON_COMMAND(ID_TOOLS_EDITPOPUPFILTERLIST, OnToolsEditpopupfilterlist)
					ON_UPDATE_COMMAND_UI(ID_FILE_WORKOFFLINE, OnUpdateFileWorkoffline)
					ON_UPDATE_COMMAND_UI(ID_TOOLS_INTERNETOPTIONS, OnUpdateMenu)
					ON_COMMAND(ID_GO_UP_PAGE, OnGoUpPage)
					ON_COMMAND(ID_TOOLS_EMAIL, OnToolsEmail)
					ON_COMMAND(ID_TOOLS_ADDR, OnToolsAddr)
					ON_UPDATE_COMMAND_UI(ID_OPTIONS_USE_PROXY, OnUpdateOptionsUseProxy)
					ON_COMMAND(ID_OPTIONS_USE_PROXY, OnOptionsUseProxy)
					ON_COMMAND(ID_TOOLS_COOKIE, OnToolsCleanCookie)
					ON_COMMAND(ID_TOOLS_CACHE, OnToolsCache)
					ON_COMMAND(ID_TOOLS_ALL, OnToolsAll)
					ON_COMMAND(ID_TOOLS_HISTORY, OnToolsHistory)
					ON_COMMAND(ID_OPTIONS_SET_PROXY, OnOptionsSetProxy)
					ON_COMMAND(ID_TOOLS_EXTERNALUTILITIES_UTILITYMANAGER, OnToolsExternalUtilitymanager)
					ON_COMMAND(ID_VIEW_HIS, OnViewHis)
					ON_UPDATE_COMMAND_UI(ID_VIEW_HIS, OnUpdateViewExp)
					ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullscreen)
					ON_UPDATE_COMMAND_UI(ID_VIEW_FULLSCREEN, OnUpdateViewFullscreen)
					ON_COMMAND(ID_ADDR_DROP, OnAddrDrop)
					ON_COMMAND(ID_ADDR_FOCUS, OnAddrFocus)
					ON_COMMAND(ID_FILE_NEW_S, OnFileNew)
					ON_COMMAND(ID_HELP_HELP, OnHelpHelp)
					ON_COMMAND(ID_OPTIONS_ACTIVENEWWINDOW, OnOptionsActivenewwindow)
					ON_UPDATE_COMMAND_UI(ID_OPTIONS_ACTIVENEWWINDOW, OnUpdateOptionsActivenewwindow)
					ON_COMMAND(ID_RTAB_ADDFAV, OnRtabAddfav)
					ON_COMMAND(ID_VIEW_LINKS, OnViewLinks)
					ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
					ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateViewToolbar)
					ON_WM_SIZE()
					ON_COMMAND(ID_HELP_FORUM, OnHelpForum)
					ON_COMMAND(ID_HELP_MAIL_TO, OnHelpMail)
					ON_COMMAND(ID_HELP_CUR_EVIL, OnCurIsEvilWeb)
					ON_COMMAND(ID_FILE_CLOSE_KEEPCUR, OnFileCloseKeepcur)
					ON_COMMAND(ID_VIEW_REFRESHALL, OnViewRefreshall)
					ON_COMMAND(ID_VIEW_STOP_ALL, OnViewStopAll)
					ON_COMMAND(ID_HELP_HOMEPAGE, OnHelpHomepage)
					ON_COMMAND(ID_FILE_NEW_CLIP, OnFileNewClip)
					ON_COMMAND(ID_WINDOW_CASCADE, OnWindowCascade)
					ON_COMMAND(ID_WINDOW_TILE_HORZ, OnWindowTileHorz)
					ON_COMMAND(ID_WINDOW_TILE_VERT, OnWindowTileVert)
					ON_WM_SETTINGCHANGE()
					ON_WM_MEASUREITEM()
					ON_COMMAND(ID_WINDOW_RESTOR, OnWindowRestore)
					ON_WM_DRAWITEM()
					ON_COMMAND(ID_WINDOW_MIN, OnWindowMin)
					ON_COMMAND(ID_MENU_BAR, OnViewMenubar)
					ON_UPDATE_COMMAND_UI(ID_TOOLS_SKIN_DEFAULT, OnUpdateToolsSetSkin)
					ON_COMMAND(ID_FILE_NEWMyIE9, OnFileNewMyIE9)
					ON_COMMAND(ID_FILE_OPENF, OnFileOpen)
					ON_COMMAND(ID_VIEW_TOOLBARTEXTLABEL, OnViewToolbartextlabel)
					ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBARTEXTLABEL, OnUpdateViewToolbartextlabel)
					ON_COMMAND(ID_SHORTCUT_USE_SC, OnShortcutUseSc)
					ON_UPDATE_COMMAND_UI(ID_SHORTCUT_USE_SC, OnUpdateShortcutUseSc)
					ON_COMMAND(ID_SHORTCUT_SC, OnShortcutSc)
					ON_COMMAND(ID_SHORTCUT_ALIAS, OnShortcutAlias)
					ON_UPDATE_COMMAND_UI(ID_SHORTCUT_ALIAS, OnUpdateShortcutAlias)
					ON_COMMAND(ID_SHORTCUT_MANALIAS, OnShortcutManalias)
					ON_COMMAND(ID_VIEW_TOOLBARS_CUSTOMIZE, OnViewToolbarsCustomize)
					ON_COMMAND(ID_OPTIONS_MyIE9OPTIONS, OnOptionsMyIE9options)
					ON_WM_INITMENUPOPUP()
					ON_UPDATE_COMMAND_UI(ID_FILE_QUICKSAVE, OnUpdateMenu)
					ON_UPDATE_COMMAND_UI(ID_VIEW_SOURCE, OnUpdateMenu)
					ON_UPDATE_COMMAND_UI(ID_FAVORITES_DROPDOWN, OnUpdateViewExp)
					ON_COMMAND(ID_FAVORITES_BEST, DoNothing)
					ON_WM_SYSCOMMAND()
					ON_COMMAND(ID_RTAB_STICKNAME, OnRtabStickname)
					ON_COMMAND(ID_RTAB_STOP, OnRtabStop)
					ON_COMMAND(ID_RTAB_REFRESH, OnRtabRefresh)
					ON_COMMAND(ID_TAB_PRE, OnTabPre)
					ON_COMMAND(ID_TAB_NEXT, OnTabNext)
					ON_COMMAND(ID_OPTIONS_NEWTAB, OnNewTab)
					ON_UPDATE_COMMAND_UI(ID_OPTIONS_NEWTAB, OnUpdateNewTab)
					//{{AFX_MSG_MAP(CMainFrame)
					ON_COMMAND(ID_OPTIONS_URLINSTATUS, OnOptionsUrlinstatus)
					ON_UPDATE_COMMAND_UI(ID_OPTIONS_URLINSTATUS, OnUpdateOptionsUrlinstatus)
					ON_COMMAND(ID_VIEW_FAVS, OnViewFavs)
					ON_COMMAND(ID_VIEW_PROTECT, OnViewProtect)
					ON_COMMAND(ID_VIEW_EX_TOOL, OnViewExTool)
					ON_COMMAND(ID_VIEW_SEARCH, OnViewSearch)
					ON_WM_DROPFILES()
					ON_WM_TIMER()
					ON_COMMAND(ID_FILE_STOP_OPENALL, OnFileStopOpenall)
					ON_UPDATE_COMMAND_UI(ID_FILE_STOP_OPENALL, OnUpdateFileStopOpenall)
					ON_COMMAND(ID_FILE_HIDE, OnFileHide)
					ON_COMMAND(ID_VIEW_FOLDER, OnViewFolder)
					ON_COMMAND(ID_OPTIONS_OPENLINKINNEWWINDOW, OnOptionsOpenlinkinnewwindow)
					ON_UPDATE_COMMAND_UI(ID_OPTIONS_OPENLINKINNEWWINDOW, OnUpdateOptionsOpenlinkinnewwindow)
					ON_WM_MOUSEWHEEL()
					ON_COMMAND(ID_EDIT_UNDOCLOSE, OnEditUndoclose)
					ON_UPDATE_COMMAND_UI(ID_EDIT_UNDOCLOSE, OnUpdateEditUndoclose)
					ON_COMMAND(ID_EDIT_EDIT, OnEditEdit)
					ON_WM_ACTIVATE()
					ON_COMMAND(ID_ORGGROUP, OnOrggroup)
					ON_WM_SETTINGCHANGE()
					ON_COMMAND(ID_NEWWINDOW, OnNewwindow)
					ON_COMMAND(ID_FILE_SENDPAGEBYEMAIL, OnFileSendpagebyemail)
					ON_COMMAND(ID_FILE_QUICKSAVE, OnFileQuicksave)
					ON_COMMAND(ID_FILE_SENDLINKBYEMAIL, OnFileSendlinkbyemail)
					ON_COMMAND(ID_TOOLS_SELBG, OnToolsSelbg)
					ON_COMMAND(ID_TOOLS_SETBG, OnToolsSetbg)
					ON_COMMAND(ID_RTAB_AUTOREF, OnRtabAutoref)
					ON_UPDATE_COMMAND_UI(ID_RTAB_AUTOREF, OnUpdateRtabAutoref)
					ON_COMMAND(ID_RTAB_REFFRE, OnRtabReffre)
					ON_COMMAND(ID_TOOLS_ALL_BK, OnToolsAllBk)
					ON_COMMAND(ID_TOOLS_QUICKSEARCH, OnToolsQuicksearch)
					ON_COMMAND(ID_TOOLS_SYNC, OnToolsSync)
					ON_COMMAND(ID_SC_FILLFORM, OnScFillform)
					ON_COMMAND(ID_SC_SAVEFORM, OnScSaveform)
					ON_COMMAND(ID_SC_SAVEGENFORM, OnScSavegenform)
					ON_COMMAND(ID_TOOLS_COLLECT, OnToolsCollect)
					ON_COMMAND(ID_TOOLS_CLEANADDR, OnToolsCleanaddr)
					ON_COMMAND(ID_ALL_PAGE_LINKS, OnAllPageLinks)
					ON_COMMAND(ID_TOOLS_AUTOSETBG, OnToolsAutosetbg)
					ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOSETBG, OnUpdateToolsAutosetbg)
					ON_COMMAND(ID_VIEW_ENCODING_AUTO, OnViewEncodingAuto)
					ON_UPDATE_COMMAND_UI(ID_VIEW_ENCODING_AUTO, OnUpdateViewEncodingAuto)
					ON_COMMAND(ID_SAVEAS_GROUP, OnSaveasGroup)
					ON_COMMAND(ID_TOOL_CLEANMyIE9FORM, OnToolCleanMyIE9form)
					ON_COMMAND(ID_RTAB_ADDALIAS, OnRtabAddalias)
					ON_COMMAND(ID_RTAB_ADDSC, OnRtabAddsc)
					ON_COMMAND(ID_RTAB_AUTOSAVE, OnRtabAutosave)
					ON_COMMAND(ID_TOOLS_MOUSE_SCR, OnToolsMouseScr)
					ON_UPDATE_COMMAND_UI(ID_TOOLS_MOUSE_SCR, OnUpdateToolsMouseScr)
					ON_COMMAND(ID_TOOLS_URL_FILTER, OnToolsUrlFilter)
					ON_UPDATE_COMMAND_UI(ID_TOOLS_URL_FILTER, OnUpdateToolsUrlFilter)
					ON_COMMAND(ID_VIEW_LOCKTOOLBAR, OnViewLocktoolbar)
					ON_UPDATE_COMMAND_UI(ID_VIEW_LOCKTOOLBAR, OnUpdateViewLocktoolbar)
					ON_COMMAND(ID_TOOLS_AUTOSCROLL_ALL, OnToolsAutoscrollAll)
					ON_UPDATE_COMMAND_UI(ID_TOOLS_AUTOSCROLL_ALL, OnUpdateToolsAutoscrollAll)
					ON_COMMAND(ID_FILE_NEW_SELECT, OnFileNewSelect)
					ON_COMMAND(ID_OPTIONS_USE_IE_PROXY, OnOptionsUseIeProxy)
					ON_UPDATE_COMMAND_UI(ID_OPTIONS_USE_IE_PROXY, OnUpdateOptionsUseIeProxy)
					ON_COMMAND(ID_CLEAN_TYPED_SEARCH, OnCleanTypedSearch)
					ON_COMMAND(ID_VIEW_AUTOHIDE, OnViewAutoHide)
					ON_UPDATE_COMMAND_UI(ID_VIEW_AUTOHIDE, OnUpdateViewAutohide)
					ON_COMMAND(ID_SYSMENU_BAR, OnViewSysMenuBar)
					ON_COMMAND(ID_TOOLS_MONITOR, OnToolsMonitor)
					ON_UPDATE_COMMAND_UI(ID_TOOLS_MONITOR, OnUpdateToolsMonitor)
					ON_COMMAND(ID_GROUP, DoGroup)
					ON_COMMAND(ID_VIEW_LOCK, OnViewLock)
					ON_WM_ENDSESSION()
					ON_COMMAND(ID_TOOLS_SEARCH_TEXT, OnToolsSearchText)
					ON_COMMAND(ID_LAST_VISITED, OnLastVisited)
					ON_COMMAND(ID_WINDOW_MANAGER, OnWindowManager)
					ON_UPDATE_COMMAND_UI(ID_WINDOW_MANAGER, OnUpdateWindowManager)
					ON_COMMAND(ID_FILE_NEW_SELECTED_LINK, OnFileNewSelectedLink)
					ON_COMMAND(ID_FILE_AUTOSAVE_ALL, OnFileAutoSaveAll)
					ON_UPDATE_COMMAND_UI(ID_FILE_AUTOSAVE_ALL, OnUpdateFileAutoSaveAll)
					ON_COMMAND(ID_FILE_NEW_CURRENT_HOME_PAGE, OnFileNewCurrentHomePage)
					ON_COMMAND(ID_VIEW_EXPLOREBAR_RESOURCE, OnViewExplorebarResource)
					ON_COMMAND(ID_FILE_NEW_CURRENT_UP_PAGE, OnFileNewCurrentUpPage)
					ON_COMMAND(ID_GO_START_PAGE, OnGoStartPage)
					ON_COMMAND(ID_SC_SET_FORM_DATA, OnScSetFormData)
					ON_COMMAND(ID_VIEW_FONTS_INCREASE, OnViewFontsIncrease)
					ON_COMMAND(ID_VIEW_FONTS_DECREASE, OnViewFontsDecrease)
					ON_COMMAND(ID_VIEW_MARK_KIND, OnViewMarkKind)
					ON_COMMAND(ID_VIEW_MARK, OnViewMark)
					ON_COMMAND(ID_TOOLS_NO_BACKGROUND, OnToolsNoBackground)
					ON_UPDATE_COMMAND_UI(ID_TOOLS_NO_BACKGROUND, OnUpdateToolsNoBackground)
					ON_COMMAND(ID_VIEW_SET_UNDO_CLOSE, OnViewSetUndoClose)
					ON_COMMAND(ID_TOOLS_CLEAN_UNDO_CLOSE, OnToolsCleanUndoClose)
					ON_COMMAND(ID_ADDTO_IE, OnAddtoIe)
					ON_COMMAND(ID_FILE_CLOSE_ALIKE_TITLE, OnFileCloseAlikeTitle)
					ON_COMMAND(ID_FILE_CLOSE_ALL_LEFT, OnFileCloseAllLeft)
					ON_COMMAND(ID_FILE_CLOSE_ALL_RIGHT, OnFileCloseAllRight)
					ON_COMMAND(ID_FILE_CLOSE_ALIKE_URL, OnFileCloseAlikeUrl)
					ON_COMMAND(ID_FILE_CLOSE_MARKED_URL, OnFileCloseMarkedUrl)
					ON_COMMAND(ID_OPTIONS_ALLOW_ANIMATE, OnOptionsAllowAnimate)
					ON_UPDATE_COMMAND_UI(ID_OPTIONS_ALLOW_ANIMATE, OnUpdateOptionsAllowAnimate)
					ON_COMMAND(ID_OPTIONS_DISABLE_FLASH, OnOptionsDisableFlash)
					ON_UPDATE_COMMAND_UI(ID_OPTIONS_DISABLE_FLASH, OnUpdateOptionsDisableFlash)
					ON_COMMAND(ID_TOOLS_SEARCH_CASE, OnToolsSearchCase)
					ON_UPDATE_COMMAND_UI(ID_TOOLS_SEARCH_CASE, OnUpdateToolsSearchCase)
					ON_COMMAND(ID_TOOLS_SEARCH_WHOLE, OnToolsSearchWhole)
					ON_UPDATE_COMMAND_UI(ID_TOOLS_SEARCH_WHOLE, OnUpdateToolsSearchWhole)
					ON_UPDATE_COMMAND_UI(ID_TOOLS_QUICKSEARCH, OnUpdateToolsQuickSearch)
					ON_COMMAND(ID_TOOLS_EXTERNAL_AUTORUN, OnToolsExternalAutorun)
					ON_UPDATE_COMMAND_UI(ID_TOOLS_EXTERNAL_AUTORUN, OnUpdateToolsExternalAutorun)
					ON_COMMAND(ID_VIEW_COMPLETELY_REFRESH, OnViewCompletelyRefresh)
					ON_COMMAND(ID_TEXT_AUTOSAVE, OnTextAutoSave)
					ON_COMMAND(ID_TEXT_COPY, OnTextCopy)
					ON_COMMAND(ID_TEXT_EDIT, OnTextEdit)
					ON_COMMAND(ID_TEXT_SAVE, OnTextSave)
					ON_COMMAND(ID_TEXT_SEARCH, OnTextSearch)
					ON_COMMAND(ID_URL_COPY, OnUrlCopy)
					ON_COMMAND(ID_URL_EDIT, OnUrlEdit)
					ON_COMMAND(ID_URL_FILTER, OnUrlFilter)
					ON_COMMAND(ID_URL_OPEN, OnUrlOpen)
					ON_COMMAND(ID_IMAGE_SAVEAS, OnImageSaveAs)
					ON_COMMAND(ID_URL_OPEN_ACTIVE, OnUrlOpenActive)
					ON_COMMAND(ID_URL_OPEN_IE, OnUrlOpenIe)
					ON_COMMAND(ID_URL_OPEN_JUMP, OnUrlOpenJump)
					ON_COMMAND(ID_URL_SHOW, OnUrlShow)
					ON_COMMAND(ID_TEXT_HIGH_LIGHT, OnTextHighLight)
					ON_COMMAND(ID_TEXT_FIND, OnTextFind)
					ON_COMMAND(ID_TEXT_OPEN_LINKS, OnTextOpenLinks)
					ON_COMMAND(ID_URL_ADDTO_FAV, OnUrlAddtoFav)
					ON_COMMAND(ID_URL_COLLECTOR, OnUrlCollector)
					ON_COMMAND(ID_TEXT_COLLECTOR, OnTextCollector)
					ON_COMMAND(ID_TOOLS_SEARCH_DELETE, OnToolsSearchDelete)
					ON_COMMAND(ID_PROXY_IMPORT, OnProxyImport)
					ON_COMMAND(ID_PROXY_EXPORT, OnProxyExport)
					ON_COMMAND(ID_URL_OPEN_INACTIVE, OnUrlOpenInactive)
					ON_COMMAND(ID_TOOLS_EXTERNAL_RUN, OnToolsExternalRun)
					ON_COMMAND(ID_RTAB_SORTSAVE, OnRtabSortSave)
					ON_COMMAND(ID_VIEW_SELECT_LANG, OnViewSelectLang)
					ON_COMMAND(ID_RTAB_NEW_WINDOW, OnRtabNewWindow)
					ON_COMMAND(ID_GO_PAGE_UP, OnGoPageUp)
					ON_COMMAND(ID_GO_PAGE_DOWN, OnGoPageDown)
					ON_COMMAND(ID_GO_PAGE_BOTTOM, OnGoPageBottom)
					ON_COMMAND(ID_GO_PAGE_TOP, OnGoPageTop)
					ON_COMMAND(ID_HIGHLIGHT_SELECTED_TEXT, OnHighlightSelectedText)
					ON_COMMAND(ID_FIND_NEXT_SELECTED_TEXT, OnFindNextSelectedText)
					ON_COMMAND(ID_OPEN_LINK, OnOpenLink)
					ON_COMMAND(ID_OPEN_LINK_ACTIVE, OnOpenLinkActive)
					ON_COMMAND(ID_OPEN_LINK_DEACTIVE, OnOpenLinkDeactive)
					ON_COMMAND(ID_FIND_PREV_SELECTED_TEXT, OnFindPrevSelectedText)
					ON_COMMAND(ID_NOT_SAVE_KEY, OnNotSaveKey)
					ON_UPDATE_COMMAND_UI(ID_NOT_SAVE_KEY, OnUpdateNotSaveKey)
					ON_COMMAND(ID_URL_OPEN_OTHER_SIDE, OnUrlOpenOtherSide)
					ON_COMMAND(ID_URL_OPEN_FTP, OnUrlOpenFtp)
					ON_COMMAND(ID_VIEW_FORBID_AUTO_NAV, OnViewForbidAutoNav)
					ON_COMMAND(ID_RTAB_PROTECT, OnRtabProtect)
					ON_COMMAND(ID_RTAB_FORBID_AUTO_NAV, OnRtabForbidAutoNav)
					ON_COMMAND(ID_RTAB_LOCK, OnRtabLock)
					ON_COMMAND(ID_RTAB_MARK, OnRtabMark)
					ON_COMMAND(ID_GO_PAGE_RIGHT, OnGoPageRight)
					ON_COMMAND(ID_GO_PAGE_LEFT, OnGoPageLeft)
					ON_COMMAND(ID_URL_SHOW_IMG, OnUrlShowImg)
					ON_COMMAND(ID_RTAB_CLOSE_KEEPCUR, OnRtabCloseKeepcur)
					ON_COMMAND(ID_TOOLS_FLY_FILTER, OnToolsFlyFilter)
					ON_UPDATE_COMMAND_UI(ID_TOOLS_FLY_FILTER, OnUpdateToolsFlyFilter)
					ON_COMMAND(ID_TOOLS_DISDOWN_ACTIVEX, OnToolsDisDownActivex)
					ON_UPDATE_COMMAND_UI(ID_TOOLS_DISDOWN_ACTIVEX, OnUpdateToolsDisDownActivex)
					ON_COMMAND(ID_TOOLS_DIALOG_FILTER, OnToolsDialogFilter)
					ON_UPDATE_COMMAND_UI(ID_TOOLS_DIALOG_FILTER, OnUpdateToolsDialogFilter)
					ON_COMMAND(ID_EDIT_IMG_URL, OnEditImgUrl)
					ON_WM_PAINT()
					ON_UPDATE_COMMAND_UI(ID_VIEW_EXPLOREBAR_RESOURCE, OnUpdateViewExp)
					ON_COMMAND(ID_FAVORITES_RESET_ORDER, OnFavoritesResetOrder)
					//}}AFX_MSG_MAP
					ON_COMMAND_RANGE(ID_VIEW_WIN_PRE, ID_VIEW_WIN_CLOSE, OnViewWinCust)
					ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_WIN_PRE, ID_VIEW_WIN_CLOSE, OnUpdateViewWinCust)
					ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnDropDown)
					ON_NOTIFY(TBN_DROPDOWN, ID_TOOLBAR_STATUS, OnDropDown)
					ON_COMMAND(IDOK, OnNewAddressEnter)
					ON_COMMAND(ID_SEARCH_OK, OnNewSearchEnter)
					ON_COMMAND(ID_FONT_DROPDOWN, DoFont)
					ON_COMMAND(ID_ENCODE_DROPDOWN, OnEncode)
					ON_COMMAND(ID_FAVORITES_DROPDOWN, OnViewFavorite)
					ON_CBN_SELENDOK(ID_TOOLBAR_ADDRESS,OnNewAddress)
					ON_COMMAND_RANGE(B_FAV, E_FAV,	   OpenMenuItem)
					ON_COMMAND_RANGE(B_GROUP, E_GROUP, OpenMenuItem)
					ON_COMMAND_RANGE(B_GO_UP, E_GO_UP, OpenGoUpMenuItem)
					ON_COMMAND_RANGE(B_SORT_SAVE, E_SORT_SAVE, OnSortSaveItem)
					ON_UPDATE_COMMAND_UI(ID_INDICATOR_PROCESS, OnUpdateProgress)
					ON_UPDATE_COMMAND_UI(ID_INDICATOR_MSG, OnUpdateMsg)
					ON_UPDATE_COMMAND_UI(ID_INDICATOR_DISP, OnUpdateDisp)
					ON_NOTIFY(TCN_SELCHANGE, ID_TOOLBAR_TAB, OnSelChange)
					ON_NOTIFY(NM_RCLICK, ID_TOOLBAR_TAB, OnRClick)
					ON_NOTIFY(NM_RCLICK, AFX_IDW_TOOLBAR, OnRClickElse)
					ON_NOTIFY(NM_RCLICK, ID_TOOLBAR_STATUS, OnRClickStatusToolbar)
					ON_COMMAND_RANGE(0x8044, 0x8047, OnEncoding)
					ON_COMMAND_RANGE(ID_VIEW_ENCODING_JANESEEUC, ID_VIEW_ENCODING_JANESEAUTO, OnEncoding)
					ON_COMMAND_RANGE(ID_VIEW_ENCODING_KOREAN_AUTO, ID_VIEW_ENCODING_KOREAN_ISO, OnEncoding)
					ON_COMMAND_RANGE(ID_MENUITEM33065, ID_MENUITEM33091, OnEncoding)
					ON_COMMAND_RANGE(ID_VIEW_FONTS_LARGEST , ID_VIEW_FONTS_SMALLEST, OnFonts)
					ON_COMMAND_RANGE(B_EX_TOOL, E_EX_TOOL, OnUtils)
					ON_COMMAND_RANGE(B_SEARCH, E_SEARCH, OnSearch)
					ON_COMMAND_RANGE(B_SEARCH_DRAG, E_SEARCH_DRAG, OnSearch)
					ON_MESSAGE(WM_USER_SHELL_OPEN, OnShellOpen) 
					ON_COMMAND(ID_LINK_POPUP, OnLinkPopup)
					ON_COMMAND(ID_FAV_POPUP, OnFavPopup)
					ON_MESSAGE(WM_UPDATE_FAV, OnUpdateFav) 
					ON_MESSAGE(WM_UPDATE_TAB, OnUpdateTab) 
					ON_COMMAND_RANGE(B_OPEN_FAV, E_OPEN_FAV, OnAllFav)
					ON_MESSAGE(WM_UPDATE_TAB_TIP, OnUpdateTabTips) 
					ON_MESSAGE(WM_ACTIVATE_WINDOW, OnActivateWindow) 
					ON_MESSAGE(WM_SEL_TAB, OnSelTab) 
					ON_NOTIFY(TTN_NEEDTEXT, 0, OnTabNeedText)
					ON_COMMAND_RANGE(ID_TOOLS_SKIN_DEFAULT, ID_TOOLS_SKIN_DEFAULT, OnToolsSetSkin)
					ON_COMMAND_RANGE(B_SKIN, E_SKIN, OnToolsSetSkin)
					ON_MESSAGE(WM_UPDATE_TOOLBAR, OnUpdateToolbar) 
					ON_COMMAND_RANGE(B_PROXY, E_PROXY, OnChangeProxy)
					ON_COMMAND_RANGE(B_WEB_PROXY, E_WEB_PROXY, OnChangeWebProxy)
					ON_COMMAND_RANGE(B_TRANS, E_TRANS, OnChangeWebProxy)
					ON_COMMAND_RANGE(B_ADDTO_FAV, E_ADDTO_FAV, OnAddThisFav)
					ON_MESSAGE(WSM_TRAY, OnNotifyTray)
					ON_COMMAND_RANGE(ID_F_2, ID_F_12, OnF2)
					ON_WM_MENUSELECT()
					ON_COMMAND_RANGE(ID_C_1, ID_C_0, OnSwitchWins)
					ON_MESSAGE(WM_HOTKEY, OnHotKey) 
					ON_UPDATE_COMMAND_UI(ID_VIEW_FOLDER, OnUpdateViewExp)
					ON_REGISTERED_MESSAGE(MSWHELL_ROLLMSG, OnScRll) 
					ON_COMMAND_RANGE(ID_TOOLS_AUTOSCROLL_SLOW, ID_TOOLS_AUTOSCROLL_FAST, OnToolsAutoscrollSpeed)
					ON_COMMAND_RANGE(ID_TOOLS_AUTOSCROLL_PAGE0, ID_TOOLS_AUTOSCROLL_PAGE1, OnToolsAutoscrollPage)
					ON_COMMAND(ID_VIEW_STOP, OnViewStop)
					ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
					ON_COMMAND(ID_FILE_PRINTPREVIEW, OnFilePrintpreview)
					ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
					ON_COMMAND_RANGE(B_OPEN_GROUP, E_OPEN_GROUP, OnOpenGroup)
					ON_COMMAND_RANGE(B_ADDTO_GROUP, E_ADDTO_GROUP, OnAddtoGroup)
					ON_COMMAND_RANGE(B_WINDOW, E_WINDOW, OnWindowList)
					ON_COMMAND_RANGE(B_UNDO, B_UNDO+MAX_UNDO, OnUndoList)
					ON_MESSAGE(WM_DELAYLOAD_CONFIG, OnDelayLoadConfig) 
					//add
					ON_COMMAND(ID_RMOUSE_DRAG, OnRMouseDrag)
					ON_UPDATE_COMMAND_UI(ID_RMOUSE_DRAG, OnUpdateRMouseDrag)
					ON_COMMAND(ID_RMOUSE_GESTURE, OnRMouseGesture)
					ON_UPDATE_COMMAND_UI(ID_RMOUSE_GESTURE, OnUpdateRMouseGesture)
					
					ON_COMMAND(ID_TOOLS_EABLEPOPUPFILTER, OnToolsEnablePopFilter)
					ON_UPDATE_COMMAND_UI(ID_TOOLS_EABLEPOPUPFILTER, OnUpdateToolsEnablePopFilter)
					
					ON_COMMAND(ID_ENABLE_FILTER, OnEnableFilter)
					ON_UPDATE_COMMAND_UI(ID_ENABLE_FILTER, OnUpdateEnableFilter)
					//	
					ON_COMMAND(ID_FIND_SEARCH_BAR, OnFindSearchBar)
					ON_COMMAND(ID_HIGH_LIGHT_SEARCH_BAR, OnHighLightSearchBar)
					
					ON_MESSAGE(WM_APPCOMMAND, OnAppCommand)
					//
					ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
					ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
					
					ON_MESSAGE(WM_MENURBUTTONUP, OnMenuRButtonUp)
					
					ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString) //gf
					
					//gfxtip
					ON_WM_NCMOUSEMOVE()
					ON_WM_NCRBUTTONUP()

					ON_COMMAND(ID_VIEW_TOP_MOST, OnTopMost)
					ON_UPDATE_COMMAND_UI(ID_VIEW_TOP_MOST, OnUpdateTopMost)

					//TODO ON_COMMAND(ID_NEW_CHILD_WINDOW, OnNewChildWindow)
					END_MESSAGE_MAP()
					
					static UINT g_nIndicators[] =
					{
						ID_INDICATOR_MSG,
							ID_INDICATOR_PROCESS,
							ID_INDICATOR_TOOLS,
							//
							ID_INDICATOR_DISP,
					};
					
					//button tips. also is button ID, don't change it
					UINT g_nToolBarID[]=
					{
						ID_NEWWINDOW,
							0,
							ID_GO_BACK,
							ID_GO_FORWARD,
							ID_VIEW_STOP,
							ID_VIEW_REFRESH,
							ID_GO_START_PAGE,
							0,
							ID_ALL_PAGE_LINKS,
							ID_FAVORITES_DROPDOWN,
							ID_VIEW_HIS,//10
							ID_VIEW_FOLDER,
							ID_VIEW_EXPLOREBAR_RESOURCE,
							0,
							ID_TOOLS_EXTERNAL_AUTORUN,
							ID_FILE_PRINT,
							ID_TOOLS_EMAIL,
							ID_FONT_DROPDOWN,
							ID_ENCODE_DROPDOWN,
							ID_OPTIONS_LOADIMAGE,
							ID_VIEW_FULLSCREEN,//20
							ID_GO_UP_PAGE,
							ID_VIEW_AUTOHIDE,
							ID_OPTIONS_USE_WEBP,
							ID_OPTIONS_USE_PROXY,
							ID_FILE_WORKOFFLINE,
							0,
							ID_FILE_OPENF,
							ID_EDIT_EDIT,
							ID_FILE_STOP_OPENALL,
							ID_TOOLS_AUTOSCROLL_USEAUTOSCROLLING,//30
							ID_FILE_SAVEAS,
							ID_EDIT_UNDOCLOSE,
							ID_EDIT_COPY,
							ID_OPTIONS_MyIE9OPTIONS,
							ID_VIEW_STOP_ALL,
							ID_VIEW_REFRESHALL,
							ID_TOOLS_SETBG,
							ID_GROUP,
							ID_EDIT_FIND,
							ID_TOP_CLOSE, //gfgb ID_OPTIONS_NEWTAB,//40
							ID_SC_FILLFORM,//ID_FILE_PRINTPREVIEW,
							ID_TOOLS_ALL,
							ID_EDIT_PASTE,
					};
					
					//button text
					UINT g_nToolBarText[]=
					{
						IDS_NEW_WINDOW,
							0,
							IDS_GO_BACK,
							ID_GO_FORWARD,
							ID_VIEW_STOP,
							ID_VIEW_REFRESH,
							ID_GO_START_PAGE,
							0,
							ID_ALL_PAGE_LINKS,
							ID_FAVORITES_DROPDOWN,
							ID_VIEW_HIS,
							ID_VIEW_FOLDER,
							ID_VIEW_EXPLOREBAR_RESOURCE,
							0,
							ID_TOOLS_EXTERNAL_AUTORUN,
							ID_FILE_PRINT,
							ID_TOOLS_EMAIL,
							ID_FONT_DROPDOWN,
							ID_ENCODE_DROPDOWN,
							ID_OPTIONS_LOADIMAGE,
							ID_VIEW_FULLSCREEN,
							ID_GO_UP_PAGE,
							IDS_VIEW_AUTOHIDE,
							ID_OPTIONS_USE_WEBP,
							ID_OPTIONS_USE_PROXY,
							ID_FILE_WORKOFFLINE,
							0,
							ID_FILE_OPENF,
							ID_EDIT_EDIT,
							ID_FILE_STOP_OPENALL,
							ID_TOOLS_AUTOSCROLL_USEAUTOSCROLLING,
							ID_FILE_SAVEAS,
							IDS_EDIT_UNDOCLOSE,
							ID_EDIT_COPY,
							IDS_OPTIONS_MyIE9OPTIONS,
							ID_VIEW_STOP_ALL,
							ID_VIEW_REFRESHALL,
							ID_TOOLS_SETBG,
							ID_GROUP,
							ID_EDIT_FIND,
							IDS_TOP_CLOSE,//gfgb ID_OPTIONS_NEWTAB,
							IDS_SC_FILLFORM,
							IDS_TOOLS_ALL,
							ID_EDIT_PASTE,
					};
					
					static UINT g_nToolBarStyle[] = 
					{
						TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
							TBBS_SEPARATOR,
							TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
							TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
							TBSTYLE_BUTTON,
							TBSTYLE_BUTTON,
							TBSTYLE_BUTTON,
							TBBS_SEPARATOR,
							TBSTYLE_BUTTON,
							TBSTYLE_BUTTON,
							TBSTYLE_BUTTON,
							TBSTYLE_BUTTON,
							TBSTYLE_BUTTON,
							TBBS_SEPARATOR,
							TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
							TBSTYLE_BUTTON,
							TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
							TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
							TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
							TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
							TBSTYLE_BUTTON,
							TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
							TBSTYLE_BUTTON,
							TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
							TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
							TBSTYLE_BUTTON,
							TBBS_SEPARATOR,
							TBSTYLE_BUTTON,
							TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
							TBSTYLE_BUTTON,
							TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
							TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
							TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
							TBSTYLE_BUTTON,
							TBSTYLE_BUTTON,
							TBSTYLE_BUTTON,
							TBSTYLE_BUTTON,
							TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
							TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
							TBSTYLE_BUTTON,
							TBSTYLE_BUTTON,
							TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
							TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,
							TBSTYLE_BUTTON,
					};
					
					UINT g_nToolBarImg[] = 
					{
						9,0,0,1,2,3,4,0,5,6,12,23,24,0,25,7,13,8,14,10,11,15,16,17,18,19,0,21,20,22,26,28,27,29,30,31,32,33,34,35,36,37,38,39,
					};
					
					//有选择的显示按钮文字
					UINT g_ToolBarLabel[]=
					{
						//0,0,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
						//gf hideUp 0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
						//0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,
						0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
					};
					
					/////////////////////////////////////////////////////////////////////////////
					// CMainFrame construction/destruction
					
					CMainFrame::CMainFrame()
					:m_bMinimizeToTray(0)
					,m_bCloseToTray(0)
					,m_bGoProxyMenu(TRUE)
					{
						m_dwLastTimeWheel = 0;
						_bTopMost= FALSE;
								
						{
							
							hbTabBkTop = NULL;
					
							hbTabBkTopActive = NULL;
					
							hbTabBkBottom=NULL;
						
							hbTabBkBottomActive=NULL;
						}

						m_nAnimateIconTimer= FALSE;
						_bCustemRClickTab=FALSE;
						m_pwndExpObj = NULL;
						m_bFullScreenMode = FALSE;//full screen mode on
						m_bFullScreened = FALSE;//when mouse move, some bars need auto show
						m_bAutoHideMode = FALSE;
						m_bAutoHided = FALSE;
						m_bFavDirty=FALSE;
						m_bMultiLineTab = FALSE;
						m_bmBGPic = NULL;
						m_bNoModemSpeed = TRUE;
						m_bRemPsw = TRUE;
						m_nProtectNum = 0;
						m_nTimerID = 0;
						m_nUpdateTimer = 0;
						m_bRemPsw = TRUE;
						m_hResInst = NULL;
						m_nNeedIB = 0;
						m_bIsSecure = m_bLastIsSecure = FALSE;
						m_bAllScroll = FALSE;
						
						m_bSaveConfig = FALSE;
						m_nWin=0;
						m_nSwitch = 0;
						CString str="yes";
						g_bstrYes = str.AllocSysString();
						m_nScrollWidth = GetSystemMetrics(SM_CXVSCROLL);
						m_nScrollArrowH = GetSystemMetrics(SM_CYVSCROLL);
						
						m_pCollectDlg = NULL;
						m_pLinkListDlg = NULL;
						m_pMonitor = NULL;
						
						m_bIsOffline = FALSE;//get from register
						m_nPopFiltered = 0;
						//gfpop del						m_bAutoPopWin = FALSE;
						LOADSTR(g_strIeProxy ,IDS_IE_PROXY);
						m_nPreLines = 0;//must do this, or tab will OnUpdateTab when start
						m_bNotShowRightMenu = 0;
						//
						m_cTrafficClass[0].SetTrafficType(MFNetTraffic::IncomingTraffic);
						m_cTrafficClass[1].SetTrafficType(MFNetTraffic::OutGoingTraffic);
						m_cTrafficClass[2].SetTrafficType(MFNetTraffic::AllTraffic);
						//
						m_nSearchBarAction = 0;
						m_bShowScriptResult = true;
						
						//for ndebug add
						m_bAutoRunExternal = 0;
						m_bActiveNewWindow = 1;
						m_bRMouseDrag = 0;
						m_bRMouseGesture = 1;
						
						//must init to 0, or it will be random 
						m_bIsAutoSave = 0;
						m_pCurView = NULL;
						m_strLastNavUrl.Empty();
						m_nDialogFilterCount = 0;
						
						m_bAddressTyped = FALSE; //gfl
					}
					
					CMainFrame::~CMainFrame()
					{
						try{
							m_astrFavoriteFolders.RemoveAll();
							m_ExternalUtilList.RemoveAll();
							m_astrSkins.RemoveAll();
							m_aProxyCategories.RemoveAll();
							
							if(m_pwndExpObj)
								delete m_pwndExpObj;
							if(m_bmBGPic)
								DeleteObject(m_bmBGPic);
							if(m_nTimerID)
								KillTimer(m_nTimerID);
							if(m_nAnimateIconTimer)
								KillTimer(m_nAnimateIconTimer);

							DeleteTabBkImgs();

							SysFreeString(g_bstrYes);
							pmf = NULL;
							DestroyMenuData(m_TabMenu.m_hMenu,0);
							DestroyMenuData(m_TrayMenu.m_hMenu,0);
							FREENULL(g_strLngBuf);
							
	}catch(...){ASSERT(false);}
					}
					
					int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
					{
						try
						{
							
							pmf = this;
							AfxOleInit();
							//
							CImageList img;
							CString str;
							_app = (CMyIE9App*)AfxGetApp();
							::SetProp(m_hWnd, _app->m_pszAppName, (HANDLE)1);
							if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
								return -1;
							
							//gf for brush my background
							if (!m_wndMDIClient.SubclassWindow (m_hWndMDIClient)) { // Add
								TRACE ("Failed to subclass MDI client window\n");   // Add
								ASSERT(false);
								//return (-1);                                        // Add
							}                                                       // Add
							

							//create rebar
							if (!m_wndReBar.Create(this,
								RBS_BANDBORDERS | RBS_DBLCLKTOGGLE | RBS_REGISTERDROP | RBS_VARHEIGHT,
								CCS_NODIVIDER | CCS_NOPARENTALIGN |
								WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | CBRS_TOP)
								)
							{
								return -1;      // fail to create
							}
							
							m_bLockToolbar = _app->GetProfileInt("Settings", "LockToolbar", FALSE);
							m_strSkinName = _app->GetProfileString("Settings", "SkinName", "default");
							m_dwFsShow = _app->GetProfileInt("Settings", "FullScreenShow", 588);
							m_dwAhShow = _app->GetProfileInt("Settings", "AutoHideShow", 588);
							CString filename = _app->m_strSkinPath + m_strSkinName + "\\";
							if ( !_FileIsExist(filename) )
								m_strSkinName = "default";

							//menu
							g_bCompactFavBar = _app->GetProfileInt("Settings", "CompactFavBar", FALSE);
							m_nMenuWidth = _app->GetProfileInt("Settings", "MenuSize", 26);
							CGf_AppStatic::_instance._nMenuWidthOther = _app->GetProfileInt("Settings", "MenuSizeOther", CGf_AppStatic::_instance._nMenuWidthOther);
							m_bDisplayOpenAll = _app->GetProfileInt("Settings", "DisplayOpenAll", TRUE);
							m_bDisplayAddFav = _app->GetProfileInt("Settings", "DisplayAddFav", TRUE);
							m_bShowHiddenFav = _app->GetProfileInt("Settings", "ShowHiddenFav", FALSE);
							g_bSingleMenu = _app->GetProfileInt("Settings", "SingleMenu", FALSE);
							m_bUseDefaultFavPath = _app->GetProfileInt("Settings", "UseDefaultFavPath", TRUE);
							m_strFavPath = _app->GetProfileString("Settings", "FavPath", "");
							m_bUseDefaultLinkPath = _app->GetProfileInt("Settings", "UseDefaultLinkPath", TRUE);
							m_strLinkPath = _app->GetProfileString("Settings", "LinkPath", "");
							m_bFavShowUrl = _app->GetProfileInt("Settings", "FavShowUrl", TRUE);

							m_nMenuStyle = _app->GetProfileInt("Settings", "MenuStyle", 1);

							//create main menu
							CMenuBar& mb = m_wndMenuBar;
							VERIFY(mb.CreateEx(this));
							mb.ModifyStyle(0, TBSTYLE_TRANSPARENT|TBSTYLE_FLAT| TBSTYLE_LIST);
							mb.LoadMenu(IDR_MAINFRAME);
							InitMenuBarImage();
							//menu
							CMenu* pMenu = mb.GetMenu();
							_LoadMenuStringOwnerDraw(pMenu, "MenuMain",&(mb.m_ImgMenu));
							pMenu->Detach();
							//tab menu
							m_bTopTab = _app->GetProfileInt("TabStyle", "TabPos", 1);
							ReloadTabMenu();
							//
							m_TrayMenu.LoadMenu(IDR_TRAYMENU);
							_LoadMenuStringOwnerDraw(m_TrayMenu.GetSubMenu(0),"MenuTray", &(mb.m_ImgMenu));
							
							//sysmenu
							if (!m_SysMenuBar.CreateEx(this))
							{
								TRACE0("Failed to create toolbar\n");
								return -1;      // fail to create
							}


							
// 							//gfxtip
// 							EnableToolTips(TRUE);
// 							_ttSys.Create(this);
// 
// 							_ttSys.AddTool(this, GetSysXTip() );
// 							
// 							_ttSys.SetDelayTime(2000);
// 							_ttSys.SetDelayTime(TTDT_INITIAL, 400);
// 
// 
// 							//load win buttons
// 							char key[5]="W";
// 							for(int i=0; i<6; i++)
// 							{
// 								itoa(i, key+1, 10);
// 								m_abWinButtons[i] = _app->GetProfileInt("WinButtons", key, 1);
// 							}
// 							InitSystemBarImage();
// 							BuildWinButtons();
// 							
// 							// link and fav menu
// 							g_bCompactFavBar = _app->GetProfileInt("Settings", "CompactFavBar", FALSE);
// 							m_nMenuWidth = _app->GetProfileInt("Settings", "MenuSize", 12);
// 							m_bDisplayOpenAll = _app->GetProfileInt("Settings", "DisplayOpenAll", TRUE);
// 							m_bDisplayAddFav = _app->GetProfileInt("Settings", "DisplayAddFav", TRUE);
// 							m_bShowHiddenFav = _app->GetProfileInt("Settings", "ShowHiddenFav", FALSE);
// 							g_bSingleMenu = _app->GetProfileInt("Settings", "SingleMenu", FALSE);
// 							m_bUseDefaultFavPath = _app->GetProfileInt("Settings", "UseDefaultFavPath", TRUE);
// 							m_strFavPath = _app->GetProfileString("Settings", "FavPath", "");
// 							m_bUseDefaultLinkPath = _app->GetProfileInt("Settings", "UseDefaultLinkPath", TRUE);
// 							m_strLinkPath = _app->GetProfileString("Settings", "LinkPath", "");
// 							m_bFavShowUrl = _app->GetProfileInt("Settings", "FavShowUrl", TRUE);
// 							
// 							CGf_AppStatic::_instance._bForbidAutoUpdate = _app->GetProfileInt("Settings", "ForbidAutoUpdate", FALSE); //gfsize
// 							CGf_AppStatic::_instance._bKeepPageSize = _app->GetProfileInt("Settings", "KeepPageSize", FALSE);
// 							CGf_AppStatic::_instance._iPageSize = _app->GetProfileInt("Settings", "PageSize", 100);
// 							
// 							if (m_bUseDefaultFavPath) //gff 
// 							{
// 								CString strDefaultFavPath;
// 								LOADSTR(strDefaultFavPath, IDS_DEFAULT_FAV_PATH);
// 								
// 								CString strFavFromReg="";
// 								CString csFavoritePath="";
// 								
// 								if (CGf_AppStatic::_instance._bStartFirstTime && CPathHelper::IsNeedChangeFavDir(strDefaultFavPath, strFavFromReg, csFavoritePath))
// 								{
// 									CString strMsgFav,strMsg;
// 									LOADSTR(strMsgFav ,IDS_FAV_ASK);
// 									
// 									strMsg.Format(strMsgFav, csFavoritePath, strFavFromReg);
// 									
// 									if (MSGBOX(strMsg, MB_YESNO|MB_ICONQUESTION)==IDYES)
// 									{
// 										
// 										CPathHelper::CreateFullDir(csFavoritePath);
// 										
// 										CPathHelper::CopyDirectory(strFavFromReg, csFavoritePath);
// 										
// 										CRegHelper::SetFavoriteFolder(csFavoritePath);
// 									}
// 									
// 								}
// 								
// 							}
							
							//link bar
							if (!m_LinkBar.CreateEx(this))
							{
								TRACE0("Failed to create toolbar\n");
								return -1;      // fail to create
							}
							InitLinkBarImage();	
							m_LinkBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_LIST|TBSTYLE_TOOLTIPS );
							m_LinkBar.SetBarStyle(m_LinkBar.GetBarStyle() |
								CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
							//Favorite bar
							if (!m_FavBar.CreateEx(this))
							{
								TRACE0("Failed to create toolbar\n");
								return -1;      // fail to create
							}
							InitFavBarImage();
							m_FavBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_LIST|TBSTYLE_TOOLTIPS );
							m_FavBar.SetBarStyle(m_FavBar.GetBarStyle() |
								CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
							//
// 							InitFavorites();

							
							//Tool Bar
							if (!m_wndToolBar.CreateEx(this))
							{
								TRACE0("Failed to create toolbar\n");
								return -1;      // fail to create
							}
							m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT |TBSTYLE_TOOLTIPS );
							//m_wndToolBar.ModifyStyleEx(0, WS_EX_TOOLWINDOW);
							m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
							// Create and set the hot toolbar image list.
							m_bSmallToolBar = _app->GetProfileInt("Settings", "SmallToolbar",0);
							m_nTextLabel = _app->GetProfileInt("Settings", "TextLabel", 0);
							m_bColorIcon = _app->GetProfileInt("Settings", "ColorIcon", TRUE);
							InitToolBar();
							
							//AddressBar
							m_bShowToolbarLabel = _app->GetProfileInt("Settings", "ShowToolbarLabel", TRUE);
							m_bShowAddressBarButton = _app->GetProfileInt("Settings", "ShowAddressBarButton", TRUE);
							m_bShowSearchBarButton = _app->GetProfileInt("Settings", "ShowSearchBarButton", TRUE);
							m_bComboShowIcon = _app->GetProfileInt("Settings", "ComboShowIcon", TRUE);
							if (!m_AddressBar.CreateEx(this))
							{
								TRACE0("Failed to create toolbar\n");
								return -1;      // fail to create
							}
							InitGoImage();
							m_AddressBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT |TBSTYLE_TOOLTIPS );
							m_AddressBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
							m_AddressBar.SetButtons(NULL, 2);
							m_AddressBar.SetButtonInfo(0, 0, TBBS_SEPARATOR, 0);
							m_AddressBar.SetButtonInfo(1, IDOK, TBSTYLE_BUTTON| TBSTYLE_DROPDOWN,0 );//| TBSTYLE_DROPDOWN, 0);
							CString strTip;
							LOADSTR(strTip ,IDS_GO);
							m_AddressBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(strTip, &m_AddressBar, IDOK);
							m_AddressBar.Init();
							//get the reference to the comboboxex.
							m_wndAddress = m_AddressBar.GetAddressBox();
							
							//external tool bar
							if (!m_ExternalTool.CreateEx(this))
							{
								TRACE0("Failed to create toolbar\n");
								return -1;      // fail to create
							}
							m_ExternalTool.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT |TBSTYLE_TOOLTIPS );
							m_ExternalTool.SetBarStyle(m_ExternalTool.GetBarStyle() |
								CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
							//
							SHFILEINFO shFinfo;
							HIMAGELIST hImgList = NULL;
							hImgList = (HIMAGELIST)SHGetFileInfo( "C:\\",
								0,
								&shFinfo,
								sizeof( shFinfo ),
								SHGFI_SYSICONINDEX | 
								SHGFI_SMALLICON );
// 							gf if ( !hImgList )
// 								return FALSE;
							if ( hImgList )
								m_ExternalTool.SendMessage(TB_SETIMAGELIST, 0, (LPARAM)hImgList);

// 							//better place here, not flash when delay load config
// 							CString wp;
// 							CExternalUtilItem* eui;
// 							wp = _app->GetProfileString("ExUtils", "name1", NULL);
// 							if(wp == "" || wp.IsEmpty())
// 							{
// 							}
// 							else
// 							{
// 								//new format
// 								char Keyname1[10] = "name", Keyname2[10] = "cmds", Keyname3[10] = "sta", Keyname4[10] = "cls", Keyname5[10] = "run";
// 								char Keyname11[10] = "para", Keyname12[10] = "path";
// 								CString cmd,para,path;
// 								BOOL bstart, bClose,bRun;
// 								int i=1;
// 								cmd = _app->GetProfileString("ExUtils", "cmds1", NULL);
// 								para = _app->GetProfileString("ExUtils", "para1", NULL);
// 								path = _app->GetProfileString("ExUtils", "path1", NULL);
// 								bstart = _app->GetProfileInt("ExUtils", "sta1", FALSE);
// 								bClose = _app->GetProfileInt("ExUtils", "cls1", bstart);
// 								bRun = _app->GetProfileInt("ExUtils", "run1", FALSE);
// 								while(wp != "" && !wp.IsEmpty())
// 								{
// 									eui = new CExternalUtilItem;
// 									eui->m_strUtilName = wp;
// 									eui->m_strUtilCmd = "\""+cmd+"\"";
// 									eui->m_strUtilPara = para;
// 									eui->m_strUtilPath = path;
// 									eui->m_bUtilStart = bstart;
// 									eui->m_bUtilClose = bClose;
// 									eui->m_bUtilRun = bRun;
// 									m_ExternalUtilList.m_UtilList.Add(eui);
// 									i++;
// 									itoa(i, Keyname1+4, 10);
// 									itoa(i, Keyname2+4, 10);
// 									itoa(i, Keyname11+4, 10);
// 									itoa(i, Keyname12+4, 10);
// 									//
// 									itoa(i, Keyname3+3, 10);
// 									itoa(i, Keyname4+3, 10);
// 									itoa(i, Keyname5+3, 10);
// 									wp = _app->GetProfileString("ExUtils", Keyname1, NULL);
// 									cmd = _app->GetProfileString("ExUtils", Keyname2, NULL);
// 									para = _app->GetProfileString("ExUtils", Keyname11, NULL);
// 									path = _app->GetProfileString("ExUtils", Keyname12, NULL);
// 									bstart = _app->GetProfileInt("ExUtils", Keyname3, FALSE);
// 									bClose = _app->GetProfileInt("ExUtils", Keyname4, bstart);
// 									bRun = _app->GetProfileInt("ExUtils", Keyname5, FALSE);
// 								}
// 							}
// 							BuildUtilMenu();
							
							//search bar
							if (!m_SearchBar.CreateEx(this))
							{
								TRACE0("Failed to create toolbar\n");
								return -1;      // fail to create
							}
							m_SearchList.LoadSearchList();//must before init
							m_SearchBar.Init();
							m_wndSearch = &m_SearchBar.m_wndComboBox;
							//LoadTypedSearch();
							m_SearchBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT |TBSTYLE_TOOLTIPS );
							m_SearchBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
							m_SearchBar.InitButton();
							//
							//status bar
							if (!m_wndStatusBar.Create(this) ||
								!m_wndStatusBar.SetIndicators(g_nIndicators,
								sizeof(g_nIndicators)/sizeof(UINT)))
							{
								TRACE0("Failed to create status bar\n");
								return -1;      // fail to create
							}
							m_wndStatusBar.SetProgressBarWidth(80);
							m_wndStatusBar.SetRange(0,100);
							//
							m_wndStatusBar.SetPaneStyle(0, SBPS_STRETCH|SBPS_NOBORDERS);
							for (int i=1 ; i<=3; i++)
								m_wndStatusBar.SetPaneStyle(i, SBPS_NORMAL|SBPS_NOBORDERS);
							
							UINT sbstyle, sbID;
							m_wndStatusBar.GetPaneInfo(3, sbID, sbstyle, m_nBaseWidth);
							m_wndStatusBar.SetPaneText(3, "");
							m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_TOOLS , SBPS_NORMAL|SBPS_NOBORDERS, m_wndStatusBar.m_nToolbarWidth-4);
							
							m_hNormalIcon = (HICON)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_NORMAL),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
							m_hSecuIcon   = (HICON)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_SECU),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
							m_wndStatusBar.GetStatusBarCtrl().SetIcon(0, m_hNormalIcon);
							
							if(_app->GetProfileInt("Bands", "Status", 1)==0)
								m_wndStatusBar.ShowWindow(SW_HIDE);
							
							//create bottom bar
							if (!m_wndReBar2.Create(this,RBS_BANDBORDERS,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS| WS_CLIPCHILDREN | CBRS_BOTTOM))
							{
								TRACE0("Failed to create rebar\n");
								return -1;      // fail to create
							}
							
							m_bMultiLineTab = _app->GetProfileInt("Settings", "MultiLineTab", 1);
							m_bAutoTab = _app->GetProfileInt("Settings", "AutoTab", 1);
							m_nTabStyle = _app->GetProfileInt("TabStyle", "TabStyle2", 3);
							
							CRect rect(0,0,300, TAB_HEIGHT);
							if (m_nTabStyle == 0)
								rect.bottom += 5;
							else if (m_nTabStyle == 1)
								rect.bottom += 4;
							DWORD dwStyle = TCS_BUTTONS|TCS_FLATBUTTONS|TCS_RAGGEDRIGHT;
							if(m_nTabStyle!=2)
							{
								if(!m_bTopTab)
									dwStyle = TCS_BOTTOM|WS_CHILD|TCS_OWNERDRAWFIXED;
								else
									dwStyle = WS_CHILD|TCS_OWNERDRAWFIXED;
							}
							if(m_bMultiLineTab)
							{
								if(!m_bAutoTab)
									m_wndTab.Create(WS_VISIBLE|dwStyle|TCS_FOCUSNEVER|TCS_FORCELABELLEFT|TCS_TOOLTIPS|TCS_MULTILINE, rect,this, ID_TOOLBAR_TAB);
								else
									m_wndTab.Create(WS_VISIBLE|dwStyle|TCS_FOCUSNEVER|TCS_FORCELABELLEFT|TCS_TOOLTIPS|TCS_MULTILINE|TCS_FIXEDWIDTH, rect,this, ID_TOOLBAR_TAB);
								GetMenu()->CheckMenuItem(ID_OPTION_MULTI_TAB, MF_CHECKED);
							}
							else if(!m_bAutoTab)
								m_wndTab.Create(WS_VISIBLE|dwStyle|TCS_FOCUSNEVER|TCS_FORCELABELLEFT|TCS_TOOLTIPS, rect,this, ID_TOOLBAR_TAB);
							else
								m_wndTab.Create(WS_VISIBLE|dwStyle|TCS_FOCUSNEVER|TCS_FORCELABELLEFT|TCS_TOOLTIPS|TCS_FIXEDWIDTH , rect,this, ID_TOOLBAR_TAB);
							DWORD dwExStyle= m_wndTab.GetExtendedStyle();
							m_wndTab.SetExtendedStyle(dwExStyle  | TCS_EX_FLATSEPARATORS);
							
							InitTaskBarImage();
							
							CSize size;
							if(m_nTabStyle==2)
							{
								size.cx = FBPADDINGX;
								size.cy = FBPADDINGY;
								m_wndTab.SetPadding( size );
							}
							GetMenu()->CheckMenuItem(ID_VIEW_TASKBAR, MF_CHECKED);
							m_wndTab.SetFonts();
							
							//add tab to top or bottom
							REBARBANDINFO rbbi;
							if(m_bTopTab)
								m_wndTab.ModifyStyle(0, CCS_NORESIZE);
							else
							{
								rbbi.cbSize = sizeof(rbbi);
								rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE  |RBBIM_ID;
								rbbi.wID = ID_VIEW_TASKBAR;
								rbbi.cxMinChild = 0;
								CRect rectAddress;
								m_wndTab.GetWindowRect(&rectAddress);
								rbbi.cyMinChild = rectAddress.Height();
								rbbi.cxIdeal = 200;
								//
								m_wndReBar2.AddBar(&m_wndTab);
								if(m_nTabStyle!=2)
								{
									rbbi.fMask |=  RBBIM_STYLE;
									rbbi.fStyle = RBBS_FIXEDBMP | RBBS_NOGRIPPER; 
								}
								m_wndReBar2.GetReBarCtrl().SetBandInfo(0, &rbbi);
							}
							//create rebar
							GetProfileReBarBandsState();
							if(m_bLockToolbar)
								FixToolbar(TRUE);
							//
							m_bDisableBackground = _app->GetProfileInt("Settings", "DisableBackground", FALSE);
							if (m_bDisableBackground==FALSE)
								ShowBackground(m_bDisableBackground);
							//bottom bar
							if(_app->GetProfileInt("Bands", "bandb0", 1)==0)
							{
								m_wndReBar2.GetReBarCtrl().ShowBand(0, FALSE);
								GetMenu()->CheckMenuItem(ID_VIEW_TASKBAR, MF_UNCHECKED);
							}
							//auto complete
							HINSTANCE hIns = LoadLibrary("shlwapi.dll");
							if(hIns != NULL)
							{
								LPFNDLLFUNC1 lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hIns, "SHAutoComplete");
								if(lpfnDllFunc1!=NULL)
									lpfnDllFunc1(m_wndAddress->GetEditCtrl()->m_hWnd, 0xe);
								
								FreeLibrary(hIns);
							}
							
							m_hResInst = LoadLibrary("RSRC32.dll");
							g_pGetRes = (GETRES) GetProcAddress(m_hResInst, "_MyGetFreeSystemResources32@4"); 
							
							m_wndTab.ModifyStyle(CCS_NORESIZE, 0);
							m_wndTab.SetTabStyle(m_nTabStyle);
							
							m_crUnSelColor = _app->GetProfileInt("TabStyle", "UnSelColor", -1);
							m_crSelColor = _app->GetProfileInt("TabStyle", "SelColor", -1);
							m_crUnSelFontColor = _app->GetProfileInt("TabStyle", "UnSelFontColor", -1);
							m_crSelFontColor   = _app->GetProfileInt("TabStyle", "SelFontColor", -1);
							m_crSepColor   = _app->GetProfileInt("TabStyle", "SepColor", -1);
							m_wndTab.SetUnSelColor(m_crUnSelColor);
							m_wndTab.SetSelColor(m_crSelColor);
							m_wndTab.SetUnSelFontColor(m_crUnSelFontColor);
							m_wndTab.SetSelFontColor(m_crSelFontColor);
							m_wndTab.SetSepColor(m_crSepColor);
							
							//gft
							CGf_AppStatic::_instance._strUserAppTitle=_app->GetProfileString("Settings", "UserAppTitle", "MyIE9");
							SetWindowText(CGf_AppStatic::_instance._strUserAppTitle);
							
							//
							UINT Modifiers = _app->GetProfileInt("Settings", "ShowHideModifiers", MOD_CONTROL);
							UINT Key = _app->GetProfileInt("Settings", "ShowHideKey", 192);
							g_bRegisterHotKeyOk = FALSE;
							if (Key)
								g_bRegisterHotKeyOk = RegisterHotKey (m_hWnd, 0, Modifiers , Key);
// 							//
// 							m_bMenuLoadLastClose = _app->GetProfileInt("Settings", "MenuLoadLastClose", 1);
// 							if (m_bMenuLoadLastClose)
// 								OpenLastClose();

							CGf_UI::SetIconForEmptyCombo(m_wndSearch, 1);
							CGf_UI::SetIconForEmptyCombo(m_wndAddress, 1);


							//m_wndAddress->GetEditCtrl()->SetFocus(); //gf
							
							
							return 0;
							
							
						}
						catch (...) {
							CHelperException::ErrorOcurred();
							return -1;
						}
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::SetAddress(LPCTSTR lpszUrl)
{
	// This is called when the browser has completely loaded the new location,
	// so make sure the text in the address bar is up to date and stop the
	try{
		SetAddressText(lpszUrl);
		COMBOBOXEXITEM item;
		item.mask = CBEIF_IMAGE|CBEIF_SELECTEDIMAGE ;
		item.iItem = -1;
		item.iImage = 1;
		item.iSelectedImage = 1;
		m_wndAddress->SetItem(&item);
	}catch(...){}
}

void CMainFrame::OnNewAddress()
{
	// gets called when an item in the Address combo box is selected
	// just navigate to the newly selected location.
	CString str;
	
	try{
		int i = m_AddressBar.m_wndAddress.GetCurSel();
		if(i<0)
			return;
		
		m_AddressBar.GetSelText(i, str);
		SetAddressText(str); //gfl
		
		//
		if (DoSpecialUrl(str))
			return;
		//better to place here, for that set address new text
		if (IS_RES_URL(str))
		{
			DO_RES_URL(str)
				SetAddressText(str);
		}
		
		CChildFrame* tcf = (CChildFrame*)MDIGetActive();
		short ks=GetKeyState(VK_CONTROL), ss=GetKeyState(VK_SHIFT);
		//create new if not exist
		if(tcf==NULL || g_bAddressInNewWindow)
		{
			CString strProxy;
			strProxy.Empty();
			if (VALID_TCF(tcf) && tcf->m_pView->m_bEnWebProxy)
			{
				strProxy = tcf->m_pView->m_strWebProxy;
			}
			tcf = NewChildWindow(0);
			if (VALID_TCF(tcf) && strProxy.GetLength()>0)
			{
				tcf->m_pView->m_bEnWebProxy = 1;
				tcf->m_pView->m_strWebProxy = strProxy;
			}
			if (m_bActiveNewAddress)
				tcf->m_bForceActive = TRUE;
		}
		//web proxy
		if (VALID_TCF(tcf))
		{
			tcf->m_pView->ToNavigate(str, 0, NULL);
			//
			tcf = (CChildFrame*)MDIGetActive();
			tcf->ViewSetFocus();//must use post
		}
	}catch(...){}
}

void CMainFrame::OnNewAddressEnter()
{
	// gets called when an item is entered manually into the edit box portion
	// of the Address combo box.
	// navigate to the newly selected location and also add this address to the
	// list of addresses in the combo box.
	CString oldstr, url;
	CString str, str2;
	COMBOBOXEXITEM item;
	
	try{
		if ( (CWnd*)m_wndSearch->GetEditCtrl() == GetFocus() )
		{
			OnNewSearchEnter();
			return;
		}
		//
		m_wndAddress->GetEditCtrl()->GetWindowText(str2);
		
		str2.TrimLeft();str2.TrimRight();
		
		if (str2.GetLength()<=0)
			return;
		//focus
		m_wndAddress->SetFocus();
		//
		if (DoSpecialUrl(str2))
			return;
		if (IS_RES_URL(str2))
		{
			DO_RES_URL(str2)
				SetAddressText(str2);
		}
		
		CChildFrame* tcf = (CChildFrame*)MDIGetActive();
		//GetKeyState return >0 means press the key
		short ks=GetKeyState(VK_CONTROL), ss=GetKeyState(VK_SHIFT), as=GetKeyState(VK_MENU);
		if(ks>=0 && ss>=0) // only Enter need to expand alias and other shortcuts
		{
			//alisa
			if(m_bUseAlias && m_mapAlias.Lookup(str2, url))
			{
				str2 = url;
				SetAddressText(str2);
			}
			//use quick search
			else if( GF_B_SEARCH_BY_ENGINE_NAME && (str2.Find(' ')>0 ) )
			{
				int i = str2.Find(' ');
				CString qs = str2.Left(i);
				CString keyw = str2.Mid(i+1);
				keyw.TrimLeft();
				qs.TrimLeft();qs.TrimRight();
				if(m_SearchList.Lookup(qs, url)!=-1)
				{
					DO_SEARCH_STR(url,keyw)
						str2 = url;
					SetAddressText(str2);
				}
			}
			else if ( CGf_AppStatic::_instance._bDirectOpenForOneWord && (!CHelperStr::Contains(str2, ".") ) && (!CHelperStr::Contains(str2, ":") ) && (!CHelperStr::Contains(str2, "/") ) && (!CHelperStr::Contains(str2, " ") && (!CHelperStr::Contains(str2, "localhost") ) ) && CHelperStr::IsEnglish(str2) ) //gf open web directly
			{
				CString strKeyw = str2;
				strKeyw.TrimLeft();
				strKeyw.TrimRight();
				
				url = GF_DIRECT_OPEN_WEB;
				DO_SEARCH_STR(url,strKeyw)
					str2 = url;
				SetAddressText(str2);
			}
			
		}
		oldstr = str2;
		item.mask = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE ;
		item.iItem = 0;
		str2.TrimLeft();
		str2.TrimRight();
		
		//javascript:void(document.bgColor='#FFFFFF')
		//books://www.
		if (str2.Find(':')>0)
			str2.Replace('\\','/');
		else
		{
			if(ks<0 && ss>=0 && as>=0)
				str2 = m_strCE1+str2+m_strCE2;
			else if(ks>=0 && ss<0 && as>=0)
				str2 = m_strSE1+str2+m_strSE2;
			else if(ks<0 && ss<0 && as>=0)
				str2 = m_strCSE1 + str2 + m_strCSE2;
			else if (str2=="localhost")
				str2 = "http://"+str2;
			else if (str2.Left(4)=="ftp.")
				str2 = "ftp://"+str2;
			else if (NOT_SEARCH_STR(str2))
			{
				if (str2.Left(2)!="\\\\") // \\server1
					str2 = "http://"+str2;
			}
			else//search
			{
				SEARCH_STR(str2)
					return;
			}
			oldstr = str2;
			SetAddressText(str2);
		}
		str =str2;
		
		m_strAddressTyped = str2;
		
		//create new if not exist and always new
		if(tcf==NULL || g_bAddressInNewWindow || as<0)
		{
			CString strProxy;
			strProxy.Empty();
			if (VALID_TCF(tcf) && tcf->m_pView->m_bEnWebProxy)
			{
				strProxy = tcf->m_pView->m_strWebProxy;
			}
			tcf = NewChildWindow(0);
			if (VALID_TCF(tcf) && strProxy.GetLength()>0)
			{
				tcf->m_pView->m_bEnWebProxy = 1;
				tcf->m_pView->m_strWebProxy = strProxy;
			}
			if (m_bActiveNewAddress)
				tcf->m_bForceActive = TRUE;
		}
		//web proxy
		if (VALID_TCF(tcf))
		{
			tcf->m_pView->ToNavigate(oldstr, 0, NULL);
			//
			tcf = (CChildFrame*)MDIGetActive();
			tcf->ViewSetFocus();
		}
		
		//gfl
		m_bAddressTyped = TRUE;
		
		//		if(m_AddressBar.FindStringExact(-1,str) == CB_ERR)
		//		{
		//			if (str.Right(1)=='/')
		//				str = str.Left(str.GetLength()-1);
		//			else
		//				str += '/';
		//			if(m_AddressBar.FindStringExact(-1, str) == CB_ERR)
		//			{
		//				//the URL is new
		//				item.pszText = (LPTSTR)(LPCTSTR)str2;
		//				item.iImage = 1;
		//				item.iSelectedImage = 1;
		//				m_wndAddress->InsertItem(&item);
		//			}
		//		}
	}catch(...){}
}

void CMainFrame::DoNothing()
{
	// this is here only so that the toolbar buttons for the dropdown menus
	// will have a callback, and thus will not be disabled.
}

int CMainFrame::BuildFavoritesMenu(LPCTSTR pszPath, int nStartPos, CMenu* pMenu,int nFixMenu, int FirstLevel, int& nFavs)
{
	CString         strPath; 
	CString         strPath2;
	CString         str, str2;
	WIN32_FIND_DATA wfd;
	HANDLE          h;
	int             nPos;
	int             nEndPos;
	int             nLastDir;
	CStringArray    astrFavorites;
	CArray<int,int>	anFavID;
	CStringArray    astrDirs;
	BOOL			isLink = FALSE;
	static CString	strOpenAll, strAddFav;
	int				nSubFavs = 0;
	
	try{
		strPath = pszPath;
		nFavs = 0;
		LOADSTR(strOpenAll ,IDS_OPEN_ALL_FAV);
		LOADSTR(strAddFav ,IDS_ADDFAV);
		// make sure there's a trailing backslash
		if(strPath[strPath.GetLength() - 1] != _T('\\'))
			strPath += _T('\\');
		//for relative path
		CString strLinkPath = m_strLinkPath;
		DO_RELATIVE_URL(strLinkPath);
		if(strPath==strLinkPath)
			isLink = TRUE;
		strPath2 = strPath;
		strPath += "*.*";
		nLastDir = 0;
		
		// now scan the directory, first for .URL files and then for subdirectories
		// that may also contain .URL files
		h = FindFirstFile(strPath, &wfd);
		DWORD fileattr;
		if(m_bShowHiddenFav == 1)
			fileattr = FILE_ATTRIBUTE_DIRECTORY;
		else
			fileattr = FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_HIDDEN;
		if(h != INVALID_HANDLE_VALUE)
		{
			nEndPos = nStartPos;
			do
			{
				if((wfd.dwFileAttributes & fileattr)==0)
				{
					str = wfd.cFileName; str2= str;
					if (IS_FAVURL(str))
					{
						nPos = nEndPos;
						astrFavorites.Add( str2);
						anFavID.Add(nPos);
						++nEndPos;
						nFavs ++;
					}
					else if(str.Right(4) == _T(".lnk"))
					{
						nPos = nEndPos;
						astrFavorites.Add(str2);
						anFavID.Add(nPos);
						++nEndPos;
						nFavs ++;
					}
				}
				else if((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (m_bShowHiddenFav==1 || (wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)==0))
				{
					// ignore the current and parent directory entries
					if(lstrcmp(wfd.cFileName, _T(".")) == 0 || lstrcmp(wfd.cFileName, _T("..")) == 0 )//|| lstrcmp(wfd.cFileName, strcha) == 0 || lstrcmp(wfd.cFileName, strcha2) == 0
						continue;
					astrDirs.Add(wfd.cFileName); 
					nLastDir++;	
				}
			}while(FindNextFile(h, &wfd));
			FindClose(h);
		}
		
		// Now add these items to the menu
		CMenuOrder mo;
		BOOL found;
		int j;
		WORD type;
		if(GetFavoriteFolder(strPath))
		{
			strPath = strPath2.Mid(strPath.GetLength()+1);
			if(mo.LoadMenuOrder(strPath))
			{
				for(int i = 0; i<mo.len; i++)
				{
					str = mo.mois[mo.index[i]].longname;
					type = mo.mois[mo.index[i]].filetype;
					//find in url first
					found = FALSE;
					for(j = 0; j<astrFavorites.GetSize() && !found && (type==0x20 || type==0); j++)
					{
						if(astrFavorites[j].CompareNoCase(str)==0)
						{
							found = TRUE;
							FavMenuAddURL(astrFavorites[j], strPath2, FirstLevel, isLink, pMenu, anFavID[j]);
							astrFavorites[j].Empty();
						}
					}
					for(j = 0; j<astrDirs.GetSize() && !found && (type==0x10 || type==0x14 || type==0); j++)
					{
						if(astrDirs[j].CompareNoCase(str)==0)
						{
							FavMenuAddFolder(astrDirs[j], strPath2, FirstLevel, isLink, pMenu, nEndPos);
							astrDirs[j].Empty();
						}
					}				
				}
			}
		}
		
		//sort dirs and favs
		CString tmp;
		int n = astrDirs.GetSize();
		int i, ch, m;
		ch = n;
		while(ch>0)
		{
			m = ch-1; ch = 0;
			for(i=1; i<=m; i++)
			{
				if(lstrcmp(astrDirs[i-1], astrDirs[i])>0)
				{
					tmp = astrDirs[i-1];
					astrDirs[i-1] = astrDirs[i];
					astrDirs[i] = tmp;
					ch = i;
				}
			}
		}
		n = astrFavorites.GetSize();
		ch = n;
		int ntmp;
		while(ch>0)
		{
			m = ch-1; ch = 0;
			for(i=1; i<=m; i++)
			{
				if(lstrcmpiA(astrFavorites[i-1], astrFavorites[i])>0)
				{
					tmp = astrFavorites[i-1];
					astrFavorites[i-1] = astrFavorites[i];
					astrFavorites[i] = tmp;
					ntmp = anFavID[i-1];
					anFavID[i-1] = anFavID[i];
					anFavID[i] = ntmp;
					ch = i;
				}
			}
		}	
		for(i = 0; i<astrDirs.GetSize(); i++)
		{
			if(astrDirs[i].IsEmpty())
				continue;
			FavMenuAddFolder(astrDirs[i], strPath2, FirstLevel, isLink, pMenu, nEndPos);
		}		
		for(i = 0 ; i < astrFavorites.GetSize() ; i++)
		{
			if(astrFavorites[i].IsEmpty())
				continue;
			FavMenuAddURL(astrFavorites[i], strPath2, FirstLevel, isLink, pMenu, anFavID[i]);
		}
		//add a break if needed
		if(!g_bSingleMenu)
			AddMenuBreak(pMenu,2);
		if(isLink)
		{
			CRect rectToolBar;
			REBARBANDINFO rbbi;
			m_LinkBar.GetItemRect(0, &rectToolBar);
			
			rbbi.cbSize = sizeof(rbbi);
			rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE /*| RBBIM_SIZE*/ |RBBIM_ID;// |RBBIM_STYLE;
			rbbi.wID = ID_VIEW_LINKS;
			rbbi.cxMinChild = 0;
			if(rectToolBar.Height()<22)
				rbbi.cyMinChild = 22;
			else
				rbbi.cyMinChild = rectToolBar.Height();
			int bc=m_LinkBar.GetToolBarCtrl().GetButtonCount();
			if(bc>0)
			{
				m_LinkBar.GetToolBarCtrl().GetItemRect(bc-1,&rectToolBar);
				/*rbbi.cx =*/ rbbi.cxIdeal = rectToolBar.right;
			}
			else
				/*rbbi.cx =*/ rbbi.cxIdeal = 2;
				int bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_LINKS);
			m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
		}
		if(FirstLevel)
		{
			CRect rectToolBar;
			REBARBANDINFO rbbi;
			m_FavBar.GetItemRect(0, &rectToolBar);
			rbbi.cbSize = sizeof(rbbi);
			rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE /*| RBBIM_SIZE*/ |RBBIM_ID ;
			rbbi.wID = ID_VIEW_FAVS;
			rbbi.cxMinChild = 0;//rectToolBar.Width()+2;
			if(rectToolBar.Height()<22)
				rbbi.cyMinChild = 22;
			else
				rbbi.cyMinChild = rectToolBar.Height();
			
			int bc=m_FavBar.GetToolBarCtrl().GetButtonCount();
			if(bc>0)
			{
				m_FavBar.GetToolBarCtrl().GetItemRect(bc-1,&rectToolBar);
				/*rbbi.cx =*/ rbbi.cxIdeal = rectToolBar.right;
			}
			else
				/*rbbi.cx =*/ rbbi.cxIdeal = 2;
				int bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_FAVS);
			m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
		}
	}catch(...){}
	
	return nEndPos;
}

void CMainFrame::OpenGoUpMenuItem(UINT nID)
{
	try
	{
		
		GET_ACTIVE
			//do local url is enough
			DO_LOCAL_URL(g_strGoUpUrl[nID-B_GO_UP]);
		tcf->m_pView->ToNavigate(g_strGoUpUrl[nID-B_GO_UP],0, NULL);
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnGoUpPage()
{
	try
	{
		
		GET_ACTIVE
			//no g_strGoUpUrl, get up url direct
			CString strURL;
		strURL = _StringGetUpUrl(tcf->m_pView->m_lpszUrl);
		if (strURL != tcf->m_pView->m_lpszUrl)
		{
			DO_LOCAL_URL(strURL);
			if (g_bLinkInNewWindow)
				NewChildWindow(1,2,strURL);
			else
				tcf->m_pView->ToNavigate(strURL, 0, NULL);
		}
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OpenMenuItem(UINT nID)
{
	try{
		TCHAR buf[INTERNET_MAX_PATH_LENGTH];
		
		CString strUrl;
		MENUITEMINFO mii;
		mii.fMask = MIIM_DATA|MIIM_TYPE;    // get the state of the menu item
		mii.dwTypeData = NULL;
		mii.cch = 0;
		mii.dwItemData = NULL;
		mii.cbSize = sizeof(mii);
		::GetMenuItemInfo(GetMenu()->m_hMenu, nID, FALSE, &mii); 
		CMenuData* pmd = (CMenuData*)(mii.dwItemData);
		if (!pmd || !pmd->m_szURL || strlen(pmd->m_szURL)<1)
			strUrl = g_strMenuUrl;
		else
			strUrl = pmd->m_szURL;
		if (strUrl.GetLength()<1)
		{
			m_bFavDirty=TRUE;//gf
			return;
		}
		int nAutoRunCmd = 0;
		if(IS_FAVURL(strUrl))
		{
			pmd->m_dwProperty = ::GetPrivateProfileInt(_T("InternetShortcut"), _T("Property"), DEFAULT_PROPERTY, strUrl);
			nAutoRunCmd = ::GetPrivateProfileInt(_T("InternetShortcut"), _T("AutoRunCmd"), 0, strUrl);
			::GetPrivateProfileString(_T("InternetShortcut"), _T("URL"), _T(""), buf, INTERNET_MAX_PATH_LENGTH, strUrl);
			strUrl = buf;
		}
		OpenMenuUrl(nID, strUrl, pmd ? pmd->m_dwProperty : DEFAULT_PROPERTY, nAutoRunCmd);
		
		CGf_StartPageMgr::_ins.AddFav(pmd->m_szMenuText, strUrl);

	}catch(...){ASSERT(false);}
}

void CMainFrame::OpenMenuUrl(UINT nID, CString strUrl, DWORD dwProperty, int nAutoRunCmd)
{
	try{
		if(strUrl.Right(4) == _T(".lnk"))
		{
			TCHAR buf[INTERNET_MAX_PATH_LENGTH];
			if(ResolveLink(strUrl, buf))
			{
				if (strlen(buf) == 0)//if length=0, maybe means net pc
					strUrl = "\\\\" + GET_TITLE(strUrl);
				else
					strUrl = buf;
			}
			else
				strUrl = "about:blank";
		}
		//
		if (DoSpecialUrl(strUrl))
			return;
		CChildFrame* tcf = (CChildFrame*)MDIGetActive();
		if (VALID_TCF(tcf) && !g_bFavInNewWindow && !PRESS_CTRL)
		{
			tcf->m_pView->ToNavigate(strUrl, 0, NULL);
			//not do this
			//SetAddressText(strUrl);
		}
		else
		{
			tcf = NewChildWindow(0);
			if(VALID_TCF(tcf))
			{
				if (dwProperty!=DEFAULT_PROPERTY )//&& nID>=B_GROUP && nID<=E_GROUP)
					tcf->m_pView->SetSpecialProperty(dwProperty);
				tcf->m_pView->m_nAutoRunCmd = nAutoRunCmd;
				tcf->m_pView->ToNavigate(strUrl, 0, NULL);
				if (m_bActiveNewWindow || m_nWin==1)
					SetAddressText(strUrl);
			}
		}
	}catch(...){}
}

void CMainFrame::OnFileNewBlank() 
{
	try
	{
		
		NewChildWindow(2,0, NULL, NULL, TRUE);
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnFileNewCurrentpage() 
{
	// TODO: Add your command handler code here
	try{
		CString strURL;
		CChildFrame* tcf = (CChildFrame*)MDIGetActive();
		if (VALID_TCF(tcf))
			strURL = tcf->m_pView->m_lpszUrl;
		else
			strURL = "about:blank";
		if (m_bActiveFileNew)
			NewChildWindow(1,2,strURL, NULL, TRUE);
		else
			NewChildWindow(1,2,strURL);
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnUpdateProgress(CCmdUI* pCmdUI) 
{
	try{

		//CString strTitle=CGf_AppStatic::_instance._strUserAppTitle; //CGfAppString::GetString(AFX_IDS_APP_TITLE)

		if(g_nPercent<0)
		{
			//gft title
			SetWindowText(CGf_AppStatic::_instance._strUserAppTitle);

			if(m_wndStatusBar.IsProgressBarVisible())
			{
				m_wndStatusBar.ShowProgressBar(false);
				
			}
			if (m_pMonitor)
			{
				//
				if (::IsWindow(m_pMonitor->m_percent.GetSafeHwnd()) && m_pMonitor->m_percent.IsWindowVisible())
					m_pMonitor->m_percent.ShowWindow(SW_HIDE);
			}
		}
		else
		{
			if(!m_wndStatusBar.IsProgressBarVisible())
				m_wndStatusBar.ShowProgressBar(true);
			m_wndStatusBar.SetPos(g_nPercent);
			static TCHAR str[20];
			sprintf(str, "%d%%", g_nPercent);
			m_wndStatusBar.SetWindowText(str);
			//
			if (m_pMonitor)
			{
				if (!m_pMonitor->m_percent.IsWindowVisible())
					m_pMonitor->m_percent.ShowWindow(SW_SHOW);
				m_pMonitor->m_percent.SetWindowText(str);
			}
			//
			static CString s_title="";
			s_title.Format("%d%%-%s", g_nPercent, CGf_AppStatic::_instance._strUserAppTitle);
			//sprintf(str, "%d%%-" +CGf_AppStatic::_instance._strUserAppTitle, g_nPercent);
			SetWindowText(s_title);
		}
	}catch(...){ASSERT(false);}
}

int CMainFrame::AddNewTab(CChildFrame* tcf, int CurID, LPTSTR label, BOOL bAddLast)
{
	if(tcf==NULL)
		return 0;
	try{
		int presel = m_wndTab.GetCurSel();
		if(m_nNewSequence==0)
		{
			if (tcf->m_nActive & INW_OTHER_SIDE)
				CurID = presel;
			else
				CurID = m_nWin;
		}
		else if(m_nNewSequence==1)
		{
			if (tcf->m_nActive & INW_OTHER_SIDE)
				CurID = presel;
			else
				CurID = presel+1;
		}
		else if(m_nNewSequence==2)
		{
			if (tcf->m_nActive & INW_OTHER_SIDE)
				CurID = presel+1;
			else
				CurID = presel;
		}
		if (CurID<0)
			CurID = 0;	
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_TEXT|TCIF_PARAM;
		if(label==NULL)
			TabCtrlItem.pszText = "about:blank";
		else
			TabCtrlItem.pszText = label;
		TabCtrlItem.lParam = (LPARAM)tcf;
		m_wndTab.InsertItem( CurID, &TabCtrlItem );
		//select this tab
		if(tcf->m_pView!=NULL)
		{
			if(tcf->m_pView->m_bIsActive)//m_bActiveNewWindow
				m_wndTab.SetCurSel(CurID);
			else if(m_nNewSequence==2)
			{
				if (tcf->m_nActive & INW_OTHER_SIDE)
					m_wndTab.SetCurSel(presel);
				else
					m_wndTab.SetCurSel(CurID+1);
			}
			else
			{
				if (tcf->m_nActive & INW_OTHER_SIDE)
					m_wndTab.SetCurSel(CurID+1);
				else
					m_wndTab.SetCurSel(presel);
			}
		}
		
		PostMessage(WM_UPDATE_TAB, TRUE);
		m_nWin = m_wndTab.GetItemCount();
		if(m_nWin<=2)
			m_SysMenuBar.PostMessage(WM_IDLEUPDATECMDUI, TRUE, 0);
		//??
		if(m_nTabStyle==1 && m_nWin==1)
			m_wndTab.Invalidate();
		//??
		static BOOL	g_bAdjustTabWidth = 0;
		if (!g_bAdjustTabWidth)
		{
			CRect rect;
			m_wndTab.GetWindowRect(&rect);
			m_wndTab.SetWindowPos(&wndTop, 0,0, rect.Width()+4, rect.Height(), SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOSENDCHANGING|SWP_NOACTIVATE);
			g_bAdjustTabWidth = 1;
		}
		g_bVerbose = TRUE;
		if(m_nNewSequence)
			UpdateTabTitleIndex(CurID);
		
	}catch(...){}
	
	return CurID;
}

void CMainFrame::OnSelChange(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	OnSelTab(0,0);
	*pResult = 0;
}

void CMainFrame::OnSelTab(WPARAM wParam, LPARAM lParam)
{
	try{
		m_wndTab.m_bDragging = FALSE;
		TCITEM TabCtrlItem;
		int n = m_wndTab.GetCurSel();
		if(n>=0)
		{
			TabCtrlItem.mask = TCIF_PARAM;
			m_wndTab.GetItem(n, &TabCtrlItem);
			if(TabCtrlItem.lParam != NULL)
			{
				((CChildFrame*)TabCtrlItem.lParam)->m_bSkipSel = TRUE;
				((CChildFrame*)TabCtrlItem.lParam)->m_nCurTabID = n;
				if(((CChildFrame*)TabCtrlItem.lParam)->m_bProtect)
					PostMessage(WM_ACTIVATE_WINDOW, 0, TabCtrlItem.lParam);
				else
					MDIActivate((CChildFrame*)TabCtrlItem.lParam);
			}		
		}
	}catch(...){}
}

void CMainFrame::OnRClickElse(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	try
	{
		
		CPoint point;
		GetCursorPos(&point);
		CWnd * pw = WindowFromPoint(point);
		if(pw!=NULL && (pw->m_hWnd ==  m_wndTab.GetSafeHwnd() 
			|| ::IsChild(m_wndAddress->GetSafeHwnd(), pw->m_hWnd)))
			return;
		if (pw->GetSafeHwnd()==m_LinkBar.GetSafeHwnd() || pw->GetSafeHwnd()==m_FavBar.GetSafeHwnd())
		{
			CToolBar	*pBar;
			CPoint		pt = point;
			pBar = (pw->GetSafeHwnd()==m_LinkBar.GetSafeHwnd() ? &m_LinkBar : &m_FavBar);
			pBar->ScreenToClient(&pt);
			int iButton = pBar->GetToolBarCtrl().HitTest(&pt);
			if (iButton >= 0 && iButton<pBar->GetToolBarCtrl().GetButtonCount())
			{
				TBBUTTON tbb;
				if( pBar->GetToolBarCtrl().GetButton( iButton,  &tbb ))
				{
					CString strUrl;
					if (tbb.idCommand == ID_LINK_POPUP || tbb.idCommand == ID_FAV_POPUP)
					{
						CString strDir ;
						if (tbb.idCommand == ID_FAV_POPUP)
						{
							GetFavoriteFolder(strUrl);
							CHECK_TAIL(strUrl);
							strDir = g_astrFavs[iButton];
						}
						else
						{
							strUrl = m_strLinkPath;
							DO_RELATIVE_URL(strUrl);
							strDir = g_astrLinks[iButton];
						}
						strUrl += strDir;
					}
					else
					{
						strUrl = GetMenuItemUrl(tbb.idCommand,GetMenu()->m_hMenu, FALSE);
					}
					//exp menu
					if (strUrl.GetLength())
					{
						UINT nCmd = DoExplorerMenu(pw->m_hWnd,strUrl,pt,TRUE);
						if (nCmd==18 && !_FileIsExist(strUrl))//delete
						{
							DeleteFav((HMENU)tbb.dwData, tbb.idCommand, iButton, pBar==&m_LinkBar ? 1 : 2);
						}
						return;
					}
				}
			}
		}
		//else pop menu
		CMenu *pmenu;
		pmenu = SMP_TOOLSBAR; 
		if(pmenu!=NULL)
		{
			pmenu->TrackPopupMenu(
				TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, 
				point.x, point.y, AfxGetMainWnd()
				);
		}
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnRClickStatusToolbar(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	try
	{
		
		CPoint point;
		GetCursorPos(&point);
		CWnd * pw = WindowFromPoint(point);
		if(pw!=NULL && (pw->m_hWnd ==  m_wndTab.GetSafeHwnd() 
			|| ::IsChild(m_wndAddress->GetSafeHwnd(), pw->m_hWnd)))
			return;
		CMenu* pPopup = m_TrayMenu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,AfxGetMainWnd());
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnRClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	try
	{
		if (_bCustemRClickTab)
		{
			return;
		}
		
		CPoint point;
		
		// translate the current toolbar item rectangle into screen coordinates
		// so that we'll know where to pop up the menu
		
		//gf if(PRESS_CTRL)
		//	return;

		GetCursorPos(&point);
		BOOL found = false;
		int i=0;
		CRect rect;
		
		try{
			m_wndTab.ScreenToClient(&point);
			TCHITTESTINFO hti;
			hti.pt = point;
			hti.flags = TCHT_ONITEM | TCHT_ONITEMICON | TCHT_ONITEMLABEL ;
			m_nRTabID = m_wndTab.HitTest(&hti);
			m_wndTab.ClientToScreen(&point);
			CMenu menu;
			CMenu* pPopup, *pGroups = NULL;
			if(m_nRTabID>=0)
			{
				//menu.LoadMenu(IDR_RTAB);
				pPopup = m_TabMenu.GetSubMenu(0);
				//if is locked
				TCITEM TabCtrlItem;
				TabCtrlItem.mask = TCIF_PARAM;
				m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
				CChildFrame *tcf = (CChildFrame*)TabCtrlItem.lParam;
				if(tcf && pPopup!=NULL)
				{
					if (tcf && tcf->m_bProtect && tcf != (CChildFrame*)MDIGetActive())
					{
						MSGBOX(IDS_TIP_PROTECT);
						return ;
					}
					pPopup->CheckMenuItem(ID_RTAB_LOCK, tcf->m_bLock ? MF_CHECKED : MF_UNCHECKED);
					pPopup->CheckMenuItem(ID_RTAB_PROTECT, tcf->m_bProtect ? MF_CHECKED : MF_UNCHECKED);
					pPopup->CheckMenuItem(ID_RTAB_MARK, MARKED(tcf->m_pView) ? MF_CHECKED : MF_UNCHECKED);
					pPopup->CheckMenuItem(ID_RTAB_FORBID_AUTO_NAV, tcf->m_pView->m_bForbidAutoNavigate ? MF_CHECKED : MF_UNCHECKED);
				}
				
				//gf no group
				//				//load group list
				//				if(m_bTopTab)
				//					pGroups = pPopup->GetSubMenu(6);
				//				else
				//					pGroups = pPopup->GetSubMenu(2);
				// 				MenuLoadGroupList(pGroups, 1);
				
				g_bRTab = TRUE;
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
			}
			
		}
		catch (...) {
			CHelperException::ErrorOcurred();
		}
		
		*pResult = 0;
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

//#pragma optimize( "s", on)

void CMainFrame::SaveTypedURL()
{
	TCHAR           sz[MAX_PATH];
	HKEY            hKey;
	CString			idUrl = "url";
	CString			idTitle = "title";
	//gf int				l=0 ;
	
	try{
		
		COMBOBOXEXITEM item;
		item.mask = CBEIF_TEXT;
		item.pszText = (LPTSTR)sz;
		item.cchTextMax = MAX_PATH;
		
		int nc =  m_wndAddress->SendMessage(CB_GETCOUNT, 0, 0);//gfl m_slAddressTypedUrls.GetCount();

		if (nc> CConsts::_iAdressListMaxItems)
			nc= CConsts::_iAdressListMaxItems;

		if (nc==0) //verify
		{
			return;
		}

		RegDeleteKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\TypedUrls"));
		if(RegCreateKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\TypedUrls"), &hKey) != ERROR_SUCCESS)
				return ;

		for(int i=0;i<nc;i++)
		{
			item.iItem = i;
			sz[0]=0;
			m_wndAddress->GetItem(&item);
			//gf			itoa(l+1, id+3, 10);
			idUrl.Format("%s%d", "url", i+1);
			
			if(GF_ADD_ADRRESS_TITLE)
			{
				
				idTitle.Format("%s%d", "title", i+1);
				
				CString strUrl = CHelperStr::GetStrFromList(m_slAddressTypedUrls, i);
				CString strTitle = "";
				if(m_AddressTypedUrlTitles.Lookup(strUrl, strTitle) )
				{
					RegSetValueEx(hKey, (idTitle), NULL, REG_SZ, (LPBYTE)(LPTSTR)(LPCTSTR)(strTitle), strlen(strTitle));
				}
				
				RegSetValueEx(hKey, (idUrl), NULL, REG_SZ, (LPBYTE)(LPTSTR)(LPCTSTR)(strUrl), strlen(strUrl));
				
				if ( CGf_StartPageMgr::_ins._bWrite)
				{
					CGf_StartPageMgr::_ins.AddTyped(strUrl);
				}
				
			}
			
			
			if ( (!GF_ADD_ADRRESS_TITLE) &&(item.pszText!=NULL && strlen(item.pszText)>0) )
			{
				
				RegSetValueEx(hKey, (idUrl), NULL, REG_SZ, (LPBYTE)(item.pszText), strlen(item.pszText));
			
				
				if ( CGf_StartPageMgr::_ins._bWrite)
				{
					CGf_StartPageMgr::_ins.AddTyped(item.pszText);
				}
				
				

				//gf l++;
			}
		}
		RegCloseKey(hKey);
	}catch(...){ASSERT(false);}
}

////#pragma optimize( "s", on )

void CMainFrame::OnDropDown(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	// this function handles the dropdown menus from the toolbar
	NMTOOLBAR* pNMToolBar = (NMTOOLBAR*)pNotifyStruct;
	CRect rect;
	
	try{
		switch (pNMToolBar->iItem)
		{
		case IDOK:
			m_AddressBar.GetToolBarCtrl().GetRect(pNMToolBar->iItem, &rect);
			break;
			
		case ID_SEARCH_OK:
			m_SearchBar.GetToolBarCtrl().GetRect(pNMToolBar->iItem, &rect);
			break;
			
		case ID_FILE_CLOSEALL:
			m_SysMenuBar.GetToolBarCtrl().GetRect(pNMToolBar->iItem, &rect);
			break;
			
		case ID_ENABLE_FILTER:
			m_wndStatusBar.m_ToolBar.GetToolBarCtrl().GetRect(pNMToolBar->iItem, &rect);
			break;
			
		default:
			m_wndToolBar.GetToolBarCtrl().GetRect(pNMToolBar->iItem, &rect);
		}
		rect.top = rect.bottom;
		::ClientToScreen(pNMToolBar->hdr.hwndFrom, &rect.TopLeft());
		//
		g_bRTab=0;
		CMenu menu;
		CMenu* pPopup;
		if(pNMToolBar->iItem == ID_OPTIONS_USE_PROXY)
		{
			menu.CreatePopupMenu();
			BuildProxyMenu(&menu);
			menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
			DestroyMenuData(menu.m_hMenu,0);
			menu.DestroyMenu();
		}
		else if(pNMToolBar->iItem == ID_OPTIONS_USE_WEBP || (pNMToolBar->iItem == IDOK && m_bGoProxyMenu))
		{
			m_dwWebProxyFrom = pNMToolBar->iItem;
			menu.CreatePopupMenu();
			BuildWebProxyMenu(&menu, 2);
			menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
			DestroyMenuData(menu.m_hMenu,0);
			menu.DestroyMenu();
		}
		else if(pNMToolBar->iItem == ID_GO_BACK)
		{
			try{
				CChildFrame* tcf = (CChildFrame*)MDIGetActive();
				if (VALID_TCF(tcf))
				{
					// the font popup is stored in a resource
					menu.CreatePopupMenu();
					SessionHistory(&menu, ID_GO_BACK, tcf);
					int cmd = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, rect.left, rect.top + 1, this);
					menu.DestroyMenu();
					if(cmd>=B_HISTORY)
						tcf->m_pView->GoBack(cmd-B_HISTORY+1);
				}
		}catch(...){}
		}
		else if(pNMToolBar->iItem == ID_GO_FORWARD)
		{
			try{
				CChildFrame* tcf = (CChildFrame*)MDIGetActive();
				if (VALID_TCF(tcf))
				{
					// the font popup is stored in a resource
					menu.CreatePopupMenu();
					SessionHistory(&menu, ID_GO_FORWARD, tcf);
					int cmd = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, rect.left, rect.top + 1, AfxGetMainWnd());
					menu.DestroyMenu();
					if(cmd>=B_HISTORY)
						tcf->m_pView->GoForward(cmd-B_HISTORY+1);
				}
		}catch(...){}
		}
		else if(pNMToolBar->iItem == ID_TOOLS_EMAIL)
		{
			try{
				menu.CreatePopupMenu();
				//get all email clients from registry
				static TCHAR           sz[MAX_PATH+1];
				HKEY            hKey;
				DWORD           dwSize = MAX_PATH+1;
				int i = 0;
				// find out from the registry where the mail clients are located.
				if(RegOpenKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Clients\\Mail"), &hKey) == ERROR_SUCCESS)
				{
					while(RegEnumKey(hKey, i, sz, dwSize) == ERROR_SUCCESS)
					{
						//menu.AppendMenu(MF_STRING, i+1, sz);
						MenuAppendOwnerItem(&menu, MF_STRING, i+1, sz);
						i++;
					}
					int cmd = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, rect.left, rect.top + 1, AfxGetMainWnd());
					menu.DestroyMenu();
					
					if(cmd>0 && RegEnumKey(hKey, cmd-1, sz, dwSize) == ERROR_SUCCESS)
					{
						LaunchEmail(sz);
					}
					RegCloseKey(hKey);
				}
		}catch(...)	{}
		}
		else if(pNMToolBar->iItem == ID_EDIT_EDIT)
		{
			menu.CreatePopupMenu();
			//get all email clients from registry
			static TCHAR    sz[MAX_PATH+1];
			HKEY            hKey, hKey2;
			DWORD           dwSize = MAX_PATH+1;
			int i = 0, l = 1;
			CString key,key2;
			CStringArray editors;
			LONG lRet;
			// find out from the registry where the editor clients are located.
			if(RegOpenKey(HKEY_CLASSES_ROOT, _T(".htm\\OpenWithList"), &hKey) == ERROR_SUCCESS)
			{
				while(RegEnumKey(hKey, i, sz, dwSize) == ERROR_SUCCESS)
				{
					key = "Applications\\";
					key += sz;
					key += "\\shell";
					lRet = RegOpenKey(HKEY_CLASSES_ROOT, key, &hKey2);
					if (lRet != ERROR_SUCCESS)
					{
						key = ".htm\\OpenWithList\\";
						key += sz;
						key += "\\shell";
						lRet = RegOpenKey(HKEY_CLASSES_ROOT, key, &hKey2);
					}
					if (lRet == ERROR_SUCCESS)
					{
						dwSize = sizeof(sz);
						RegQueryValueEx(hKey2, _T("FriendlyCache"), NULL, NULL, (LPBYTE)sz, &dwSize);
						editors.Add(key);
						//menu.AppendMenu(MF_STRING, l, sz);
						MenuAppendOwnerItem(&menu, MF_STRING, l, sz);
						l++;
						RegCloseKey(hKey2);
					}
					dwSize = MAX_PATH+1;
					i++;
				}
				int cmd = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, rect.left, rect.top + 1, AfxGetMainWnd());
				menu.DestroyMenu();
				RegCloseKey(hKey);
				if(cmd>0)
				{
					key = editors.GetAt(cmd-1);
					key += "\\edit\\command";
					if(RegOpenKey(HKEY_CLASSES_ROOT, key, &hKey2) == ERROR_SUCCESS)
					{
						dwSize = sizeof(sz);
						RegQueryValueEx(hKey2, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
						RegCloseKey(hKey2);
						CChildFrame* tcf = (CChildFrame*)MDIGetActive();
						if (VALID_TCF(tcf))
						{
							LaunchEditor(sz, tcf->m_pView->m_lpszUrl);
						}
					}
				}
			}
		}
		else if(pNMToolBar->iItem == ID_SEARCH_OK || (pNMToolBar->iItem == IDOK && !m_bGoProxyMenu))
		{
			if (pNMToolBar->iItem == IDOK)
				g_nSearchType = ST_ADDRBAR;
			else
				g_nSearchType = ST_SEARCHBAR;
			pPopup = SMP_SEARCH;
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
		}
		else if(pNMToolBar->iItem == ID_GO_UP_PAGE)
		{
			menu.CreatePopupMenu();
			BuildGoUpMenu(&menu);
			menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
			DestroyMenuData(menu.m_hMenu,0);
			menu.DestroyMenu();
		}
		else if(pNMToolBar->iItem == ID_ENABLE_FILTER)
		{
			pPopup = SMP_FILTER; //gfm GetMenu()->GetSubMenu(4)->GetSubMenu(7)
			//因为在底部,弹出方式特殊点
			pPopup->TrackPopupMenu(TPM_BOTTOMALIGN | TPM_LEFTBUTTON, rect.left, rect.top - 19, AfxGetMainWnd());
		}
		else //some simple button
		{
			switch (pNMToolBar->iItem)
			{
			case ID_FILE_SAVEAS:		pPopup = SMP_SAVE;break;
			case ID_FONT_DROPDOWN:		pPopup = SMP_FONTS;break;
			case ID_TOOLS_SETBG:		pPopup = SMP_BGCOLOR;break;
			case ID_OPTIONS_LOADIMAGE:	pPopup = SMP_DOWNLOAD;break;
			case ID_EDIT_UNDOCLOSE:		pPopup = SMP_UNDO;break;
			case ID_TOOLS_ALL:			pPopup = SMP_CLEAN;break;
			case ID_SC_FILLFORM:		pPopup = SMP_AUTOFILL;break;
			case ID_ENCODE_DROPDOWN:	pPopup = SMP_ENCODE;break;
			case ID_GROUP:				pPopup = SMP_GROUP;break;
			case ID_NEWWINDOW:			pPopup = SMP_NEW;break;
			case ID_FILE_CLOSEALL:		pPopup = SMP_CLOSE;break;
			case ID_TOOLS_EXTERNAL_AUTORUN:				pPopup = SMP_EXTOOLS;break;
			case ID_TOOLS_AUTOSCROLL_USEAUTOSCROLLING:	pPopup = SMP_AUTOSCROLL;break;
			default:
				*pResult = TBDDRET_DEFAULT;
				return;
			}
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
		}
	}catch(...){}
	
	*pResult = TBDDRET_DEFAULT;
}

void CMainFrame::OnViewAddressbar() 
{
	try
	{
		
		// TODO: Add your command handler code here
		REBARBANDINFO rbbi;
		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
		int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_VIEW_ADDRESSBAR);
		m_wndReBar.GetReBarCtrl().GetBandInfo(nb, &rbbi);
		
		if( rbbi.fStyle & RBBS_HIDDEN  || !m_wndAddress->IsWindowVisible())
		{
			m_AddressBar.ShowWindow(SW_SHOW);
			m_wndReBar.GetReBarCtrl( ).ShowBand(nb, TRUE);
			GetMenu()->CheckMenuItem(ID_VIEW_ADDRESSBAR, MF_CHECKED);
			
		}
		else
		{
			m_wndReBar.GetReBarCtrl( ).ShowBand(nb,FALSE);
			m_AddressBar.ShowWindow(SW_HIDE);
			GetMenu()->CheckMenuItem(ID_VIEW_ADDRESSBAR, MF_UNCHECKED);
		}
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnViewTaskbar() 
{
	try
	{
		
		// TODO: Add your command handler code here
		REBARBANDINFO rbbi;
		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
		int nb;
		if(m_bTopTab)
		{
			nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_VIEW_TASKBAR);
			m_wndReBar.GetReBarCtrl().GetBandInfo(nb, &rbbi);
		}
		else
		{
			nb=0;
			m_wndReBar2.GetReBarCtrl().GetBandInfo(nb, &rbbi);
		}
		
		if(rbbi.fStyle & RBBS_HIDDEN)
		{
			if(m_bTopTab)
			{
				m_wndReBar.GetReBarCtrl( ).ShowBand(nb,TRUE);
			}
			else
			{
				m_wndReBar2.GetReBarCtrl( ).ShowBand(0,TRUE);
			}
			m_wndTab.ShowWindow(SW_SHOW);
			GetMenu()->CheckMenuItem(ID_VIEW_TASKBAR, MF_CHECKED);
		}
		else
		{
			if(m_bTopTab)
			{
				m_wndReBar.GetReBarCtrl( ).ShowBand(nb,FALSE);
			}
			else
			{
				m_wndReBar2.GetReBarCtrl( ).ShowBand(0,FALSE);
			}
			m_wndTab.ShowWindow(SW_HIDE);
			GetMenu()->CheckMenuItem(ID_VIEW_TASKBAR, MF_UNCHECKED);
		}
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnFileWorkoffline() 
{
	// TODO: Add your command handler code here
	DWORD dwRet = _RegGetDword(HKEY_CURRENT_USER,
		"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings",
		"GlobalUserOffline"
		);
	dwRet = (dwRet!=0 ? 0 : 1);
	_RegSetDword(HKEY_CURRENT_USER,
		"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings",
		"GlobalUserOffline",
		dwRet
		);
	//infor the system that setting is changed.
	InternetSetOption(
		NULL,
		INTERNET_OPTION_SETTINGS_CHANGED,
		NULL,
		0
		);
	m_bIsOffline = dwRet;
}

void CMainFrame::LoadTypedURL()
{
	
	TCHAR           sz[GF_REGEDIT_BUFF_LENGTH];
	TCHAR           szTitle[GF_REGEDIT_BUFF_LENGTH];
	HKEY            hKey;
	DWORD           dwSize;
	TCHAR			id[9] = "url";
	int				i = 1;
	
	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\TypedUrls"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Typed URLs not found\n");
		return ;
	}
	itoa(i, id+3, 10);
	COMBOBOXEXITEM item;
	item.mask = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE ;
	item.iImage = 1;
	item.iSelectedImage = 1;
	
	dwSize = GF_REGEDIT_BUFF_LENGTH -1;
	int iInserted = 0;
	while(RegQueryValueEx(hKey, _T(id), NULL, NULL, (LPBYTE)sz, &dwSize) == ERROR_SUCCESS && (iInserted < CConsts::_iAdressListMaxItems ) )
	{
		if(strlen(sz)>0 && CGFBusiness::IsValidAdressList(CString(sz) ) )
		{
			CString strItem;
			strItem.Format("%s", sz);
			
			m_slAddressTypedUrls.AddTail(strItem);
			
			dwSize = GF_REGEDIT_BUFF_LENGTH -1;
			
			
			if (GF_ADD_ADRRESS_TITLE )
			{
				CString strTitleKey = "";
				strTitleKey.Format("title%d", i);
				
				memset(szTitle, 0, GF_REGEDIT_BUFF_LENGTH );
				
				if (RegQueryValueEx(hKey, strTitleKey, NULL, NULL, (LPBYTE)szTitle, &dwSize) == ERROR_SUCCESS) 
				{				
					m_AddressTypedUrlTitles.SetAt(strItem, CString(szTitle) );
					strItem = CGFBusiness::GetAddressShowStr(strItem, CString(szTitle));
				}	
				
			}
			
			item.iItem = iInserted;
			item.pszText = (LPTSTR)(LPCTSTR)strItem;
			
			m_wndAddress->InsertItem(&item);
			
			iInserted++;
		}
		
		i++;
		itoa(i, id+3, 10);
		dwSize = GF_REGEDIT_BUFF_LENGTH - 1;
	}
	RegCloseKey(hKey);
}

void CMainFrame::LockTab(CChildFrame *tcf)
{
	try{
		if (!tcf || !tcf->m_pView)
			return;
		CMyIE9View *pView = tcf->m_pView;
		if(!tcf->m_bProtect)
		{
			tcf->m_bLock =!tcf->m_bLock;
			if(pView->m_nProgress<0)
			{
				int nID = FindTab(tcf);
				if(tcf->m_bLock)
				{
					pView->m_nOlderIcon = pView->m_nOldIcon;
					SetTabIcon( TI_LOCK, nID, pView );
				}
				else
					SetTabIcon( pView->m_nOlderIcon, nID, pView );
				//update tab bar yab0
				if(m_nTabStyle==2)
					PostMessage(WM_UPDATE_TAB, FALSE);
			}
		}
	}catch(...){}
}

void CMainFrame::MarkTab(CChildFrame *tcf)
{
	try{
		if (!VALID_TCF(tcf))
			return;
		CMyIE9View *pView = tcf->m_pView;
		int nID = FindTab(tcf);
		if (MARKED(pView))
		{
			SetTabIcon( pView->m_nOlderIcon, nID, pView );
		}
		else
		{
			pView->m_nOlderIcon = pView->m_nOldIcon;
			SetTabIcon( TI_MARK, nID, pView );
		}
	}catch(...){}
}

void CMainFrame::OnRtabLock() 
{
	// TODO: Add your command handler code here
	try{
		CChildFrame* tcf ;
		if (g_bRTab)
		{
			TCITEM TabCtrlItem;
			TabCtrlItem.mask = TCIF_PARAM;
			m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
			tcf = (CChildFrame*)TabCtrlItem.lParam;
		}
		else
		{
			tcf = (CChildFrame*)MDIGetActive();
		}
		if (tcf)
			LockTab(tcf);
	}catch(...){}
}

void CMainFrame::OnViewLock() 
{
	// TODO: Add your command handler code here
	g_bRTab = 0;
	OnRtabLock();
}

void CMainFrame::OnToolsAddtopopupfilter() 
{
	// TODO: Add your command handler code here
	GET_ACTIVE
		AddToPopup(tcf);
}

void CMainFrame::OnRtabAddtopopupfilter() 
{
	try
	{
		
		// TODO: Add your command handler code here
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
		if(TabCtrlItem.lParam != NULL)
		{
			AddToPopup(((CChildFrame*)TabCtrlItem.lParam));
		}
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnRtabAddtoFlyfilter() 
{
	try
	{
		
		// TODO: Add your command handler code here
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
		if(TabCtrlItem.lParam != NULL)
		{
			AddToFly(((CChildFrame*)TabCtrlItem.lParam));
		}
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::AddToPopup(CChildFrame *pChFrm)
{
	try{
		CMyIE9View* pView = (CMyIE9View*)pChFrm->m_pView;
		if(pView!=NULL)
		{
			
			CString strURL;
			pView->GetLocationURL(strURL);
			if(strURL.IsEmpty())
				strURL = pView->m_lpszUrl;
			if (strURL == "about:blank")
				return;
			CAddPopupDlg dlg;
			dlg.m_strUrl = strURL;
			if (dlg.DoModal()==IDOK)
			{
				if(m_astrPopup.Find(dlg.m_strUrl)==NULL)
				{
					m_astrPopup.AddTail(dlg.m_strUrl);
					m_bSaveConfig = TRUE;
				}
				
				if (dlg.m_bClosePage)
				{
					//CLOSE_PAGE(pChFrm); 不能用这个,否则无法提醒用户是否关闭
					pChFrm->m_pView->m_bToClose = TRUE;
					pChFrm->SendMessage(WM_CLOSE);
				}
				
				SaveFilterList2File(0);
			}
		}
		
	}catch(...){ASSERT(false);}
}


void CMainFrame::AddToFly(CChildFrame *pChFrm)
{
	try{
		CMyIE9View* pView = (CMyIE9View*)pChFrm->m_pView;
		if(pView!=NULL)
		{
			CString strURL;
			pView->GetLocationURL(strURL);
			if(strURL.IsEmpty())
				strURL = pView->m_lpszUrl;
			if (strURL == "about:blank")
				return;
			CAddFlyDlg dlg;
			dlg.m_strUrl = strURL;
			if (dlg.DoModal()==IDOK)
			{
				if(m_astrFly.Find(dlg.m_strUrl)==NULL)
				{
					m_astrFly.AddTail(dlg.m_strUrl);
					m_bSaveConfig = TRUE;
				}
				
				if (dlg.m_bFilterCurrent)
				{
					pmf->RunKillFlying(pChFrm);
				}
				
				m_bUseFlyingFilter = TRUE;
				
				SaveFilterFlyList2File();
			}
		}
	}catch(...){}
}


UINT _cdecl SavePopFilterList(LPVOID con)
{
	int	i = 1;
	CString filename = theApp.m_strUser+"Filter.ini";
	char Keyname[12];
	POSITION pos ;
	
	//pop filter
	strcpy(Keyname, "pop");
	pos = pmf->m_astrPopup.GetHeadPosition();
	WritePrivateProfileSection("PopFilter", NULL, filename);
	while(pos!=NULL)
	{
		itoa(i, Keyname+3, 10);
		::WritePrivateProfileString("PopFilter", Keyname, pmf->m_astrPopup.GetAt(pos), filename);
		pmf->m_astrPopup.GetNext(pos);
		i++;
	}
	//pop unfilter
	i=1;
	strcpy(Keyname, "pop");
	pos = pmf->m_astrUnPopup.GetHeadPosition();
	WritePrivateProfileSection("PopUnFilter", NULL, filename);
	while(pos!=NULL)
	{
		itoa(i, Keyname+3, 10);
		::WritePrivateProfileString("PopUnFilter", Keyname, pmf->m_astrUnPopup.GetAt(pos), filename);
		pmf->m_astrUnPopup.GetNext(pos);
		i++;
	}
	return 0;
}

UINT _cdecl SaveUrlFilterList(LPVOID con)
{
	int	i = 1;
	CString filename = theApp.m_strUser+"Filter.ini";
	char Keyname[12];
	
	//url filter
	strcpy(Keyname, "url");
	WritePrivateProfileSection("UrlFilter", NULL, filename);
	for(i=0; i < pmf->m_astrUrlFilter.GetSize(); i++)
	{
		itoa(i+1, Keyname+3, 10);
		::WritePrivateProfileString("UrlFilter", Keyname, pmf->m_astrUrlFilter.GetAt(i), filename);
	}
	//url unfilter
	i = 1;
	strcpy(Keyname, "url");
	WritePrivateProfileSection("UrlUnFilter", NULL, filename);
	for(i=0; i< pmf->m_astrUrlUnFilter.GetSize(); i++)
	{
		itoa(i+1, Keyname+3, 10);
		::WritePrivateProfileString("UrlUnFilter", Keyname, pmf->m_astrUrlUnFilter.GetAt(i), filename);
	}
	return 0;
}

void CMainFrame::SaveFilterList2File(BOOL bUrlFilter)
{
	AfxBeginThread(bUrlFilter ? SaveUrlFilterList : SavePopFilterList, NULL);
}

UINT _cdecl SaveFlyFilterListThreadApi(LPVOID con)
{
	int	i = 1;
	CString filename = theApp.m_strUser+"FilterFly.ini";
	char Keyname[12];
	POSITION pos ;
	
	//pop filter
	strcpy(Keyname, "url");
	pos = pmf->m_astrFly.GetHeadPosition();
	WritePrivateProfileSection("FlyFilter", NULL, filename);
	while(pos!=NULL)
	{
		itoa(i, Keyname+3, 10);
		::WritePrivateProfileString("FlyFilter", Keyname, pmf->m_astrFly.GetAt(pos), filename);
		pmf->m_astrFly.GetNext(pos);
		i++;
	}
	//pop unfilter
	i=1;
	strcpy(Keyname, "url");
	pos = pmf->m_astrUnFly.GetHeadPosition();
	WritePrivateProfileSection("FlyUnFilter", NULL, filename);
	while(pos!=NULL)
	{
		itoa(i, Keyname+3, 10);
		::WritePrivateProfileString("FlyUnFilter", Keyname, pmf->m_astrUnFly.GetAt(pos), filename);
		pmf->m_astrUnFly.GetNext(pos);
		i++;
	}
	return 0;
}


void CMainFrame::SaveFilterFlyList2File()
{
	AfxBeginThread( SaveFlyFilterListThreadApi, NULL);
}

void CMainFrame::OnToolsEditpopupfilterlist() 
{
	// TODO: Add your command handler code here
	if (g_nStartPage!=16 && g_nStartPage!=17)
		g_nStartPage = 16;
	OnOptionsMyIE9options();
}

void CMainFrame::OnToolsEmail() 
{
	try
	{
		
		// TODO: Add your command handler code here
		TCHAR           sz[MAX_PATH];
		HKEY            hKey;
		DWORD           dwSize = MAX_PATH;
		
		// find out from the registry where the favorites are located.
		if(RegOpenKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Clients\\Mail"), &hKey) != ERROR_SUCCESS)
		{
			TRACE0("mail client not found\n");
			return ;
		}
		dwSize = sizeof(sz);
		RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
		if(dwSize>0)
		{
			LaunchEmail(sz);
		}
		RegCloseKey(hKey);
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnToolsAddr() 
{
	try
	{
		
		// TODO: Add your command handler code here
		TCHAR           sz[MAX_PATH];
		TCHAR           szPath[MAX_PATH];
		HKEY            hKey;
		DWORD           dwSize = MAX_PATH;
		
		// find out from the registry where the favorites are located.
		if(RegOpenKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Clients\\Contacts"), &hKey) != ERROR_SUCCESS)
		{
			TRACE0("address book client not found\n");
			return ;
		}
		dwSize = sizeof(sz);
		RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
		if(dwSize>0)
		{
			RegCloseKey(hKey);
			CString key = "SOFTWARE\\Clients\\Contacts\\";
			key += sz;
			key += "\\Shell\\open\\command";
			if(RegOpenKey(HKEY_LOCAL_MACHINE, key, &hKey) == ERROR_SUCCESS)
			{
				//name found, to get the path
				dwSize = sizeof(sz);
				RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
				if(strlen(sz)==0)
				{
					return;
				}
				ExpandEnvironmentStrings(sz, szPath, MAX_PATH);
				
				//run
				char * ch, *ch1 = szPath;
				ch = strchr(szPath, '\"');
				if(ch!=NULL)
				{
					ch1 = ch+1;
					ch = strchr(ch1, '\"');
					if(ch!=NULL)
						*ch ='\0';
				}
				else
				{
					ch = strchr(szPath, ' ');
					if(ch!=NULL)
						*ch = '\0';
				}
				if(ch!=NULL)
					ch++;
				ShellExecute(NULL, "open", ch1 , ch, ".", SW_SHOW );
			}
		}
		RegCloseKey(hKey);
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnToolsCleanCookie() 
{
	try
	{
		
		
		DelTempFiles(COOKIE_CACHE_ENTRY);
		
		//clear password
		if (CRegHelper::IsIE8() )
		{
			CGf_Win::OpenHide( _T("rundll32.exe"), _T("InetCpl.cpl,ClearMyTracksByProcess 32") );
		}
		else
		{
			CGf_IE::ClearPasswords();
		}
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

BOOL CMainFrame::DelTempFiles(DWORD CacheType, BOOL op)
{
    BOOL bResult = FALSE;
    BOOL bDone = FALSE;
    LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;  
    DWORD  dwTrySize, dwEntrySize = 4096; // start buffer size    
    HANDLE hCacheDir = NULL;    
    DWORD  dwError = ERROR_INSUFFICIENT_BUFFER;
    
    do
	{                               
        switch (dwError)
        {
            // need a bigger buffer
		case ERROR_INSUFFICIENT_BUFFER: 
			delete [] lpCacheEntry;            
			lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
			lpCacheEntry->dwStructSize = dwEntrySize;
			dwTrySize = dwEntrySize;
			BOOL bSuccess;
			if (hCacheDir == NULL)
			{                  
				bSuccess = (hCacheDir 
					= FindFirstUrlCacheEntry(NULL, lpCacheEntry,
					&dwTrySize)) != NULL;
			}
			else
				bSuccess = FindNextUrlCacheEntry(hCacheDir, lpCacheEntry, &dwTrySize);
			if (bSuccess)
				dwError = ERROR_SUCCESS;    
			else
			{
				dwError = GetLastError();
				dwEntrySize = dwTrySize; // use new size returned
			}
			break;
			
		case ERROR_NO_MORE_ITEMS:// we are done
			bDone = TRUE;
			bResult = TRUE;          
			break;
			
		case ERROR_SUCCESS:// we have got an entry
			// if CacheType=0 then delete all, else only delete CacheType
			if(op)
			{
				if (CacheType==0 || (lpCacheEntry->CacheEntryType & CacheType))                
					DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
			}
			else
			{
				if (CacheType==0 || !(lpCacheEntry->CacheEntryType & CacheType))                
					DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
			}
			// get ready for next entry
			dwTrySize = dwEntrySize;
			if (FindNextUrlCacheEntry(hCacheDir, lpCacheEntry, &dwTrySize))
				dwError = ERROR_SUCCESS;          
			else
			{
				dwError = GetLastError();
				dwEntrySize = dwTrySize; // use new size returned
			}                    
			break;
			
		default:// unknown error
			bDone = TRUE;                
			break;
        }
        if (bDone)
        {   
            delete [] lpCacheEntry; 
            if (hCacheDir)
                FindCloseUrlCache(hCacheDir);         
        }
    }while (!bDone);
	return bResult;
}


void CMainFrame::OnToolsCache() 
{
	try
	{
		
		DelTempFiles(COOKIE_CACHE_ENTRY, FALSE);
		//CGf_IE::DelTempFilesInThread();
		//AfxBeginThread(CGf_IE::DelTempFilesThreadApi, NULL); 
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnToolsHistory() 
{
	try
	{
		
		IUrlHistoryStg2* pUrlHistoryStg2 = NULL;
		HRESULT hr = CoCreateInstance(CLSID_CUrlHistory,
			NULL, CLSCTX_INPROC, IID_IUrlHistoryStg2,
			(void**)&pUrlHistoryStg2);
		if (SUCCEEDED(hr))
		{     
			hr = pUrlHistoryStg2->ClearHistory(); 
			pUrlHistoryStg2->Release();
			//clean history list
			if(HasExpBar(IDS_TITLE_HIS))
				m_pwndExpObj->Initialize();
		}
		//more history under HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Internet Settings\5.0\Cache\Extensible Cache\MSHist******
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnToolsCleanaddr()
{
	m_wndAddress->ResetContent();
	
	m_slAddressTypedUrls.RemoveAll();
	m_AddressTypedUrlTitles.RemoveAll();
	
	//
	HKEY            hKey;
	TCHAR			id[9] = "url";
	int				i = 1;
	
	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\TypedUrls"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Typed URLs not found\n");
		return ;
	}
	itoa(i, id+3, 10);
	while(RegDeleteValue(hKey, _T(id)) == ERROR_SUCCESS)
	{
		i++;
		itoa(i, id+3, 10);
	}
	RegCloseKey(hKey);
}

void CMainFrame::OnCleanTypedSearch()
{
	m_astrTypedSearch.RemoveAll();
	m_wndSearch->ResetContent();
	m_wndSearch->GetEditCtrl()->SetWindowText("");
	SaveSearchKey();
}

void CMainFrame::OnToolsCleanUndoClose()
{
	
	m_strUndoName.RemoveAll();
	m_strUndoURL.RemoveAll();
	
	UpdateUndoMenu();
	SaveLastClose();
}

void CMainFrame::OnToolsAll() 
{
	try
	{
		
		// TODO: Add your command handler code here
		CCleanAll dlg;
		dlg.m_nCleanAllMode = m_nCleanAllMode;
		if (dlg.DoModal()==IDOK)
		{
			
			//			if (dlg.m_bCache)
			//				OnToolsCache();
			//			if (dlg.m_bCookies)
			// 				OnToolsCleanCookie();			
			//			if (dlg.m_bFormData)
			//				OnToolCleanMyIE9form();
			//			if (dlg.m_bHistory)
			//				OnToolsHistory();
			//			if (dlg.m_bTypedAddress)
			//				OnToolsCleanaddr();
			//			if (dlg.m_bTypedSearch)
			//				OnCleanTypedSearch();
			//			if (dlg.m_bUndoClose)
			// 				OnToolsCleanUndoClose();
			
			
			if (dlg.m_bCache)
				OnToolsCache();
			
			if (dlg.m_bHistory)
				OnToolsHistory();
			if (dlg.m_bTypedAddress)
				OnToolsCleanaddr();
			if (dlg.m_bTypedSearch)
				OnCleanTypedSearch();
			if (dlg.m_bUndoClose)
				OnToolsCleanUndoClose();
			
			if (dlg.m_bCookies)
				OnToolsCleanCookie();
			
			
			if (dlg.m_bFormData)
				OnToolCleanMyIE9form();
			
			SAVEINT(dlg.m_nCleanAllMode,m_nCleanAllMode,"Settings","CleanAllMode");
			
		}
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}


//#pragma optimize( "s", off)

void CMainFrame::OnFonts(UINT nID)
{
	GET_ACTIVE
		
		g_nDefFontSize = tcf->m_pView->m_nFontSize = nID-ID_VIEW_FONTS_LARGEST;
	long font = 4- g_nDefFontSize;
	COleVariant vaZoomFactor(font);
	
	tcf->m_pView->ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER, &vaZoomFactor, NULL);
}

//#pragma optimize( "s", on)

void CMainFrame::SetAsDefaultBrowser() 
{
	// TODO: Add your command handler code here
	TCHAR           sz[MAX_PATH];
	HKEY            hKey;
	DWORD           dwSize = MAX_PATH;
	
	BOOL setdef=FALSE;
	int re = MSGBOX(IDS_SET_DEFAULT, MB_YESNOCANCEL|MB_ICONQUESTION);
	if(re==IDYES)
	{
		setdef=TRUE;

		CRegHelper::SetDefaultBrowserForWin7(CPathHelper::GetExePath());

		CRegHelper::SetUserAgent();

		//用myie9会使myie9跳出，但不打开网页。更糟糕
		CString strNo="C:\\Program Files\\Internet Explorer\\iexplore9.exe";
		int reFake = MSGBOX(IDS_FAKE_AS_IE, MB_YESNOCANCEL|MB_ICONQUESTION);
		if (reFake == IDYES)
		{
			CRegHelper::SetDefaultBrowser(strNo); //gfdb 
		}
		else if (reFake == IDNO)
		{
			CRegHelper::SetDefaultBrowser(CGf_IE::GetIEFilePath()); //gfdb 
		}
		
	}
	else if(re == IDCANCEL)
		return;
	else //gfdb
	{
		CString strIePath = CGf_IE::GetIEFilePath();
		CRegHelper::SetDefaultBrowser(strIePath); //gfdb 
		
		CRegHelper::SetDefaultBrowserForWin7(strIePath);

		CRegHelper::DelUserAgent();
	}


	//htm
	if(RegOpenKey(HKEY_CLASSES_ROOT, _T(".htm"), &hKey) == ERROR_SUCCESS)
	{
		//set back the default value.
		RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)("htmlfile"), 9);
		RegCloseKey(hKey);
	}
	//html
	if(RegOpenKey(HKEY_CLASSES_ROOT, _T(".html"), &hKey) == ERROR_SUCCESS)
	{
		//set back the default value.
		RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)("htmlfile"), 9);
		RegCloseKey(hKey);
	}
	//set the key value of default browser, open
	SetDefault(_T("htmlfile\\shell"), setdef);
	//url
	if(RegOpenKey(HKEY_CLASSES_ROOT, _T(".url"), &hKey) == ERROR_SUCCESS)
	{
		dwSize = sizeof(sz);
		RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
		if(strcmp(sz, "InternetShortcut")!=0)//set back the default value.
			RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)("InternetShortcut"), 16);
		RegCloseKey(hKey);
	}
	SetDefault(_T("InternetShortcut\\shell"), setdef);
	//mht, mhtml
	if(RegOpenKey(HKEY_CLASSES_ROOT, _T(".mht"), &hKey) == ERROR_SUCCESS)
	{
		dwSize = sizeof(sz);
		RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
		if(strcmp(sz, "mhtmlfile")!=0)//set back the default value.
			RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)("mhtmlfile"), 10);
		RegCloseKey(hKey);
	}
	if(RegOpenKey(HKEY_CLASSES_ROOT, _T(".mhtml"), &hKey) == ERROR_SUCCESS)
	{
		*sz='\0';
		dwSize = sizeof(sz);
		RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
		if(strcmp(sz, "mhtmlfile")!=0)//set back the default value.
			RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)("mhtmlfile"), 10);
		RegCloseKey(hKey);
	}
	//set the key value of default browser, open
	SetDefault(_T("mhtmlfile\\shell"), setdef);
	//http
	SetDefault( _T("http\\shell"), setdef);

	//vista
	SetDefault( _T("IE.HTTP\\shell"), setdef);

	//https
	SetDefault( _T("https\\shell"), setdef);
	//file
	SetDefault( _T("file\\shell"), setdef);
	//ftp
	SetDefault( _T("ftp\\shell"), setdef);
	//shtm, shtml
	if(setdef)
	{
		if(RegOpenKey(HKEY_CLASSES_ROOT, _T(".shtm"), &hKey) == ERROR_SUCCESS)
		{
			dwSize = sizeof(sz);
			RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
			if(strcmp(sz, "htmlfile")!=0)
				RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)("htmlfile"), 9);
			RegSetValueEx(hKey, _T("Content Type"), NULL, REG_SZ, (LPBYTE)("text/html"), 10);
			RegCloseKey(hKey);
		}
		if(RegOpenKey(HKEY_CLASSES_ROOT, _T(".shtml"), &hKey) == ERROR_SUCCESS)
		{
			*sz='\0';
			dwSize = sizeof(sz);
			RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
			if(strcmp(sz, "htmlfile")!=0)//set back the default value.
				RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)("htmlfile"), 9);
			RegSetValueEx(hKey, _T("Content Type"), NULL, REG_SZ, (LPBYTE)("text/html"), 10);
			RegCloseKey(hKey);
		}
	}
}

void CMainFrame::OnAddrDrop() 
{
	// TODO: Add your command handler code here
	m_wndAddress->SetFocus();
	m_wndAddress->ShowDropDown(!m_wndAddress->GetDroppedState());
}

void CMainFrame::OnAddrFocus() 
{
	// TODO: Add your command handler code here
	m_wndAddress->SetFocus();
}

void CMainFrame::OnHelpHelp() 
{
// 	// TODO: Add your command handler code here
//     HINSTANCE hInstance = AfxGetResourceHandle();
//     ASSERT(hInstance != NULL);
// 	TCHAR lpszModule[_MAX_PATH];
// 	
//     if (GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
//     {
// 		CString strHelp(lpszModule);
// 		int i = strHelp.ReverseFind('\\');
// 		strHelp = strHelp.Left(i+1);
// 		strHelp += "Help.htm";
// 		NewChildWindow(1,2,strHelp);
//     }

	CString strHelp="http://myie9.net/help/";
	NewChildWindow(1,2,strHelp);

}

void CMainFrame::OnOptionsActivenewwindow() 
{
	// TODO: Add your command handler code here
	if(m_bActiveNewWindow )
		m_bActiveNewWindow = FALSE;
	else
		m_bActiveNewWindow = TRUE;

	CString str;
	str.Format("%d",m_bActiveNewWindow);
	WritePrivateProfileString("Settings", "ActiveNew", str, CGfPathManager::GetUserConfig());
}

BOOL CMainFrame::SetDefault(LPCSTR lpszKey, BOOL setdef)
{
	HKEY            hKey, hSubKey, hSubKey2;
	DWORD           dwSize = MAX_PATH;

	if(RegOpenKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) != ERROR_SUCCESS)
	  if(RegCreateKey(HKEY_CLASSES_ROOT, _T(lpszKey), &hKey) != ERROR_SUCCESS)
		return FALSE;

	if(setdef)
	{
		DWORD dwRes;
		char lpszModule[_MAX_PATH];
		
		HINSTANCE hInstance = AfxGetInstanceHandle();
		if (GetModuleFileName(hInstance, lpszModule+1, _MAX_PATH))
		{
			*lpszModule = '\"';
			//gf strcat(lpszModule, "\" \"%1\"");

			strcat(lpszModule, "\"");

			strcat(lpszModule, " \"%1\"");

			//default
			RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)(LPCSTR)(GF_TITILE_UNCHANGE),strlen(GF_TITILE_UNCHANGE));
			//open
			RegCreateKeyEx(hKey, "open", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey, &dwRes);
			RegCreateKeyEx(hSubKey, "command", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey2, &dwRes);
			RegSetValueEx(hSubKey2, _T(""), NULL, REG_SZ, (LPBYTE)(lpszModule), strlen(lpszModule));
			
			
			RegCloseKey(hSubKey2);

			//del ddeexec, othervise, would open 2 in vista
			_RegDeletePath(hSubKey, "ddeexec");

			RegCloseKey(hSubKey);

			//
//			//gf RegCloseKey(hSubKey);
//			RegCloseKey(hSubKey2);
//
//			//gf
//			RegCreateKeyEx(hSubKey, "ddeexec", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey2, &dwRes);
//			RegCreateKeyEx(hSubKey2, "Application", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey3, &dwRes);
//			RegSetValueEx(hSubKey3, _T(""), NULL, REG_SZ, (LPBYTE)(LPCSTR)GF_TITILE_UNCHANGE, GF_TITILE_UNCHANGE.GetLength());
//
//			RegCloseKey(hSubKey);
//			RegCloseKey(hSubKey2);
//			RegCloseKey(hSubKey3);
//  			//gf end
			
			//MyIE9
			RegCreateKeyEx(hKey, GF_TITILE_UNCHANGE, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey, &dwRes);
			RegCreateKeyEx(hSubKey, "command", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey2, &dwRes);
			RegSetValueEx(hSubKey2, _T(""), NULL, REG_SZ, (LPBYTE)(lpszModule), strlen(lpszModule));
			RegCloseKey(hSubKey);
			RegCloseKey(hSubKey2);
		}
	}
	else
	{
		//get ie path
		CString strIePath = CGf_IE::GetIEFilePath();

		//CRegHelper::SetDefaultBrowser(strIePath);

		//
		char	lpszKeyIcon[MAX_PATH];
		TCHAR   sz[MAX_PATH];
		char	*p;
		HKEY	hKeyIcon;
		//del 
		strcpy(lpszKeyIcon, lpszKey);
		strcat(lpszKeyIcon, "\\" + GF_TITILE_UNCHANGE + "\\command");
		RegDeleteKey(HKEY_CLASSES_ROOT, lpszKeyIcon);
		strcpy(lpszKeyIcon, lpszKey);
		strcat(lpszKeyIcon, "\\" + GF_TITILE_UNCHANGE );
		RegDeleteKey(HKEY_CLASSES_ROOT, lpszKeyIcon);
		//
		strcpy(lpszKeyIcon, lpszKey);
		p = strstr(lpszKeyIcon,"\\");
		*p = 0;
		strcat(lpszKeyIcon,"\\DefaultIcon");
		//icon
		if(RegOpenKey(HKEY_CLASSES_ROOT, _T(lpszKeyIcon), &hKeyIcon) == ERROR_SUCCESS)
		{
			dwSize = sizeof(sz);
			RegQueryValueEx(hKeyIcon, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
			if(strstr(sz, GF_TITILE_UNCHANGE))
			{
				strcpy(sz, strIePath);
				strcat(sz, ",1");
				RegSetValueEx(hKeyIcon, _T(""), NULL, REG_SZ, (LPBYTE)(sz), strlen(sz));
			}
			RegCloseKey(hKeyIcon);
		}
		//open
		if(RegOpenKey(hKey, _T("opennew"), &hSubKey) == ERROR_SUCCESS)
		{
			RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)("opennew"), 7);
			RegCloseKey(hSubKey);
		}
		if(RegOpenKey(hKey, _T("open"), &hSubKey) == ERROR_SUCCESS)
		{
			RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)("open"), 5);
			RegCloseKey(hSubKey);
			//
			if(RegOpenKey(hKey, _T("open\\command"), &hSubKey) == ERROR_SUCCESS)
			{
				dwSize = sizeof(sz);
				RegQueryValueEx(hSubKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
				if(strstr(sz, GF_TITILE_UNCHANGE))
				{
					//strcpy(sz,"\"%ProgramFiles%\\Internet Explorer\\iexplore.exe\",%1");
					sprintf(sz,"\"%s\" %%1",strIePath);
					RegSetValueEx(hKeyIcon, _T(""), NULL, REG_SZ, (LPBYTE)(sz), strlen(sz));
				}
			}
			RegCloseKey(hSubKey);
		}
	}
	//close hkey
	RegCloseKey(hKey);
	return TRUE;
}

void CMainFrame::OnViewLinks() 
{
	// TODO: Add your command handler code here
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
	int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_VIEW_LINKS);
	m_wndReBar.GetReBarCtrl().GetBandInfo(nb, &rbbi);
	
	if( rbbi.fStyle & RBBS_HIDDEN  || !m_LinkBar.IsWindowVisible())
	{
		m_LinkBar.ShowWindow(SW_SHOW);
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb, TRUE);
		GetMenu()->CheckMenuItem(ID_VIEW_LINKS, MF_CHECKED);
		
	}
	else
	{
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb,FALSE);
		m_LinkBar.ShowWindow(SW_HIDE);
		GetMenu()->CheckMenuItem(ID_VIEW_LINKS, MF_UNCHECKED);
	}
	
}

void CMainFrame::OnViewToolbar() 
{
	// TODO: Add your command handler code here
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
	int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_VIEW_TOOLBAR);
	m_wndReBar.GetReBarCtrl().GetBandInfo(nb, &rbbi);
	
	if( rbbi.fStyle & RBBS_HIDDEN)
	{
		m_wndToolBar.ShowWindow(SW_SHOW);
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb, TRUE);
		GetMenu()->CheckMenuItem(ID_VIEW_TOOLBAR, MF_CHECKED);
		
	}
	else
	{
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb,FALSE);
		m_wndToolBar.ShowWindow(SW_HIDE);
		GetMenu()->CheckMenuItem(ID_VIEW_TOOLBAR, MF_UNCHECKED);
	}
}

void CMainFrame::OnCurIsEvilWeb() 
{
	
	GET_ACTIVE

	if (IDOK != MSGBOX( CGfAppString::GetString(IDS_ASK_CONFIRM), MB_OKCANCEL))
	{
		return;
	}


	CFilterSites::AddUserBadSite(tcf->m_pView->m_lpszUrl);

	AfxMessageBox(CGfAppString::GetString(IDS_ADDED_BADSITE));
}

void CMainFrame::OnHelpMail() 
{
	CString filename = CGfPathManager::GetUpdateExeFilePath();
	ShellExecute(NULL, "open", filename , "", NULL, SW_SHOW );
	
	//gf
	//	// TODO: Add your command handler code here
	//	CString strName = AfxGetApp()->GetProfileString("Register", "YourName", NULL);
	//	if (strName.GetLength()==0)
	//		MSGBOX(IDS_HELP_MAIL);
	//	DefaultMail("mailto:MyIE9B@gmail.com");
}

void CMainFrame::OnHelpForum() 
{
	CString str = "http://myie9.net/bbs/";
//	LOADSTR(str, IDS_SOFTWARE_FORUM);
	
	NewChildWindow(1,2,str);
}

void CMainFrame::OnFileNewClip() 
{
	// TODO: Add your command handler code here
	CString strUrl = _GetClipString();
	if ( strlen(strUrl) < 1 )
		return;
	if (IS_SCRIPT_MAILTO(strUrl))
		MSGBOX(IDS_NOT_DRAG_SCRIPT);
	else
	{
		g_nSearchType = ST_SEARCHBAR;
		DragDropUrl(strUrl,m_bActiveFileNew);
	}
}

void CMainFrame::OnViewMenubar() 
{
	// TODO: Add your command handler code here
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
	int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_MENU_BAR);
	m_wndReBar.GetReBarCtrl().GetBandInfo(nb, &rbbi);
	
	if( rbbi.fStyle & RBBS_HIDDEN )
	{
		m_wndMenuBar.ShowWindow(SW_SHOW);
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb, TRUE);
		GetMenu()->CheckMenuItem(ID_MENU_BAR, MF_CHECKED);
	}
	else
	{
		m_wndMenuBar.ShowWindow(SW_HIDE);
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb,FALSE);
		GetMenu()->CheckMenuItem(ID_MENU_BAR, MF_UNCHECKED);
	}
	
}

void CMainFrame::ShowSkin()
{
	try
	{
		//tool bar
		InitToolBarImage(TRUE);
		//m_wndToolBar.Invalidate();
		//address bar
		InitGoImage();m_AddressBar.InitImage();
		//m_AddressBar.Invalidate();
		//tab bar
		InitTaskBarImage();
		m_wndTab.Invalidate();		
		//link bar
		InitLinkBarImage();
		//m_LinkBar.Invalidate();
		//fav bar
		InitFavBarImage();
		//m_FavBar.Invalidate();
		//search bar
		m_SearchBar.InitImage();
		//m_SearchBar.Invalidate();
		//system bar
		InitSystemBarImage();
		//m_SysMenuBar.Invalidate();
		
		m_wndReBar.Invalidate();
		
		//status bar
		m_wndStatusBar.InitImage();
		m_wndStatusBar.Invalidate();
		//main anim icon
		InitMainAnimIcon();
		m_animIcon.ShowFirstImage();
		//menu bar
		InitMenuBarImage();
		//back ground
		ShowBackground(m_bDisableBackground);
	}catch(...){}
}

void CMainFrame::ShowBackground(BOOL bDisable)
{
	BOOL bShow = !bDisable;
	
	CString filename = theApp.m_strSkinPath + m_strSkinName + "\\BackGround.bmp";
	if (bShow)
	{
		if ( !_FileIsExist(filename) )
			bShow = FALSE;
		else
			bShow = TRUE;
	}
	try{
		if (bShow)
		{
			// show bitmap: load if needed
			if (m_bmBGPic)
			{
				DeleteObject(m_bmBGPic);
				m_bmBGPic= NULL;
			}
			// load bitmap
			m_bmBGPic = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, 
				LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
		}
		else
		{
			// hide bitmap: delete if needed
			if (m_bmBGPic)
			{
				DeleteObject(m_bmBGPic); // delete bitmap
				m_bmBGPic = NULL;
			}
		}
		
		REBARBANDINFO rbbi;
		rbbi.cbSize = sizeof(rbbi);
		int n = m_wndReBar.GetReBarCtrl().GetBandCount();
		for (int i=0; i< n; i++) 
		{
			rbbi.fMask  = RBBIM_STYLE;
			m_wndReBar.GetReBarCtrl().GetBandInfo(i, &rbbi);
			rbbi.fMask  |= RBBIM_BACKGROUND;
			rbbi.fStyle |= RBBS_FIXEDBMP;
			rbbi.hbmBack = m_bmBGPic;
			m_wndReBar.GetReBarCtrl().SetBandInfo(i, &rbbi);
			
			rbbi.fMask = RBBIM_CHILD | RBBIM_ID;
			m_wndReBar.GetReBarCtrl().GetBandInfo ( i, &rbbi );
			CWnd * pWnd = CWnd::FromHandle(rbbi.hwndChild);
			if(pWnd!=NULL )
				pWnd->Invalidate();
		}
		m_wndReBar.Invalidate();
		//must do this
		m_wndAddress->GetEditCtrl()->Invalidate();
		m_wndSearch->GetEditCtrl()->Invalidate();
	}catch(...){}
}

void CMainFrame::OnToolsSetSkin(UINT nID) 
{
	try
	{
		
		// TODO: Add your command handler code here
		int n = m_astrSkins.GetCount();
		if(nID == ID_TOOLS_SKIN_DEFAULT)
		{
			m_strSkinName = "default";
		}
		else
		{
			int m = nID - B_SKIN;
			if(m>=0 && m<n)
			{
				POSITION pos = m_astrSkins.FindIndex(m);
				if(pos !=NULL)
					m_strSkinName = m_astrSkins.GetAt(pos);
			}
		}
		ShowSkin();
		//
		CMenu* pMenu = GetMenu();
		if(pMenu!=NULL)
		{
			pMenu->CheckMenuItem(ID_TOOLS_SKIN_DEFAULT, MF_UNCHECKED);
			for(int i=0; i<n; i++)
				pMenu->CheckMenuItem(B_SKIN+i, MF_UNCHECKED);
			pMenu->CheckMenuItem(nID, MF_CHECKED);
		}
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnShortcutSc() 
{
	// TODO: Add your command handler code here
	CSetSCDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnShortcutManalias() 
{
	// TODO: Add your command handler code here
	CSetAliasDlg dlg;
	if (dlg.DoModal()==IDOK)
		SaveAliasConfig();
}

void CMainFrame::OnViewToolbarsCustomize() 
{
	// TODO: Add your command handler code here
	try{
		CCustomizeToolbar dlg;
		dlg.m_nTextOpt = m_nTextLabel;
		dlg.m_bColorIcon = m_bColorIcon;
		dlg.m_bSmallToolBar = m_bSmallToolBar;
		if(dlg.DoModal()==IDOK)
		{
			m_nTextLabel = dlg.m_nTextOpt;
			if(m_bColorIcon != dlg.m_bColorIcon || dlg.m_bSmallToolBar != m_bSmallToolBar)
			{
				m_bSmallToolBar = dlg.m_bSmallToolBar;
				m_bColorIcon = dlg.m_bColorIcon;
				InitToolBar(TRUE);
			}
			else
			{
				m_bSmallToolBar = dlg.m_bSmallToolBar;
				m_bColorIcon = dlg.m_bColorIcon;
				InitToolBar();
			}
		}
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnOptionsMyIE9options() 
{
	GF_TRY{
		
		// TODO: Add your command handler code here
		CMyIE9App *app = (CMyIE9App*)AfxGetApp();
		
		CGeneralProp	genprop;
		CConfirmProp	confirmprop;
		CStartProp		startprop;
		CDeleteProp		deleteprop;
		
		CWindowProp			winprop;
		CMenuProp		menuprop;
		CToolbarProp	toolbarprop;
		CStatusBarProp	statusbarprop;
		CCollectorProp	collectorprop;
		CTabProp	tabprop;
		CTabStyleProp	tabsprop;
		CSaveProp	saveprop;
		CDownProp	downprop;
		CFtpProp	ftpprop;
		CGestureProp gestureprop;
		CMouseProp	mouseprop;
		CPopupDlg popProp;
		CFlyDlg flyProp; //gffly
		CUrlFilterProp urlProp;
		COtherProp	otherprop;
		
		CSAPrefsDialog sheet;
		
		CString strTitle;
		LOADSTR(strTitle ,IDS_GENERAL);
		sheet.AddPage(genprop,strTitle);
		LOADSTR(strTitle ,IDS_START_CLOSE);
		sheet.AddPage(startprop,strTitle,&genprop);

		
		LOADSTR(strTitle ,IDS_ELSE);
		sheet.AddPage(otherprop,strTitle);

		LOADSTR(strTitle ,IDS_TAB);
		sheet.AddPage(tabprop,strTitle);
		LOADSTR(strTitle ,IDS_TAB_STYLE);
		sheet.AddPage(tabsprop,strTitle,&tabprop);

		LOADSTR(strTitle ,IDS_MOUSE);
		sheet.AddPage(gestureprop,strTitle);
		LOADSTR(strTitle ,IDS_DRAG);
		sheet.AddPage(mouseprop,strTitle,&gestureprop);
		LOADSTR(strTitle ,IDS_POPUP_FILTER);
		sheet.AddPage(popProp,strTitle);
		
		LOADSTR(strTitle ,IDS_FLY_FILTER); //gffly
		sheet.AddPage(flyProp,strTitle,&popProp);
		
		LOADSTR(strTitle ,IDS_CONTENT_FILTER);
		sheet.AddPage(urlProp,strTitle,&popProp);

		LOADSTR(strTitle ,IDS_WINDOW);
		sheet.AddPage(winprop,strTitle);
		LOADSTR(strTitle ,IDS_MENU);
		sheet.AddPage(menuprop,strTitle,&winprop);
		LOADSTR(strTitle ,IDS_TOOLBAR);
		sheet.AddPage(toolbarprop,strTitle,&winprop);
		LOADSTR(strTitle ,IDS_STATUSBAR);
		sheet.AddPage(statusbarprop,strTitle,&winprop);
		LOADSTR(strTitle ,IDS_COLLECTOR);
		sheet.AddPage(collectorprop,strTitle,&winprop);
		
		LOADSTR(strTitle ,IDS_CONFIRM);
		sheet.AddPage(confirmprop,strTitle,&winprop);
		LOADSTR(strTitle ,IDS_DELETE);
		sheet.AddPage(deleteprop,strTitle,&winprop);

		LOADSTR(strTitle ,IDS_SAVE);
		sheet.AddPage(saveprop,strTitle);
		LOADSTR(strTitle ,IDS_DOWNLOAD);
		sheet.AddPage(downprop,strTitle);
		LOADSTR(strTitle ,IDS_FTP);
		sheet.AddPage(ftpprop,strTitle,&downprop);


		
		//down prop
		downprop.m_bDLImage		= (m_dwDefaultProperty & DLCTL_DLIMAGES ? TRUE : FALSE);
		downprop.m_bDLVideo		= (m_dwDefaultProperty & DLCTL_VIDEOS	? TRUE : FALSE);
		downprop.m_bDLSound		= (m_dwDefaultProperty & DLCTL_BGSOUNDS ? TRUE : FALSE);
		//
		downprop.m_bDLApplet	= (m_dwDefaultProperty & DLCTL_NO_JAVA				? FALSE : TRUE);
		downprop.m_bDLActivex	= (m_dwDefaultProperty & DLCTL_NO_RUNACTIVEXCTLS	? FALSE : TRUE);
		downprop.m_bDLScript	= (m_dwDefaultProperty & DLCTL_NO_SCRIPTS			? FALSE : TRUE);
		
		//animation
		CString strRet;
		strRet = _RegGetString(HKEY_CURRENT_USER,
			"Software\\Microsoft\\Internet Explorer\\Main",
			"Play_Animations");
		downprop.m_bDLAnimation = (strRet=="yes" ? 1 : 0);
		BOOL bDLAnimationOld = downprop.m_bDLAnimation;
		//flash
		DWORD dwRet = _RegGetDword(HKEY_LOCAL_MACHINE,
			"Software\\Microsoft\\Internet Explorer\\ActiveX Compatibility\\{D27CDB6E-AE6D-11cf-96B8-444553540000}",
			"Compatibility Flags"
			);
		downprop.m_bDLFlash = (dwRet==0x400 ? 0 : 1);
		BOOL bDLFlashOld = downprop.m_bDLFlash;
		//down all
		downprop.m_nDownAll = m_nDownAll;
		
		//ftp prop
		ftpprop.m_nFtpOpenType = m_nFtpOpenType;
		ftpprop.m_strFtpExePath= m_strFtpExePath;
		
		//genaral
		genprop.m_bAniIcon = m_bAnimatIcon;
		genprop.m_bAutoHideFavSideBar = CGf_AppStatic._instance._bAutoHideFavSideBar;

		genprop.m_bTopCloseExitApp = g_bTopCloseExitApp;

		//for relative path reason
		//genprop.m_strSkinPath = _app->m_strSkinPath;
		//genprop.m_strGroupPath = theApp.m_strGroupPath;
		genprop.m_strSkinPath = _app->GetProfileString("Settings", "SkinPath", _app->m_strSkinPath);
		genprop.m_strGroupPath = _app->GetProfileString("Settings", "GroupPath", theApp.m_strGroupPath);
		
		//
		genprop.m_bUseDefaultFavPath = m_bUseDefaultFavPath;
		genprop.m_strFavPath = m_strFavPath;
		genprop.m_strUserAppTitle=CGf_AppStatic::_instance._strUserAppTitle;
		genprop.m_bUseDefaultLinkPath = m_bUseDefaultLinkPath;
		genprop.m_strLinkPath = m_strLinkPath;
		
		genprop._bForbidAutoUpdate=	CGf_AppStatic::_instance._bForbidAutoUpdate; //gfsize
		genprop._bKeepPageSize=	CGf_AppStatic::_instance._bKeepPageSize;
		genprop._iPageSize=	CGf_AppStatic::_instance._iPageSize;
		
		genprop._bEnableIE8Mode = CGf_AppStatic::_instance._bEnableIE8Mode;
		genprop._bAlwaysIEMenuRightClickImg = CGf_AppStatic::_instance._bAlwaysIEMenuRightClickImg;
		genprop._bDirectOpenForOneWord = CGf_AppStatic::_instance._bDirectOpenForOneWord;

		genprop.m_bForbidMainTitle = m_bForbidMainTitle;
		
		//
		confirmprop.m_bKeepSilent = g_bSilent;
		confirmprop.m_bConfirmExit = g_bConfirmExit;
		confirmprop.m_bConfirmCloseAll = g_bConfirmCloseAll;
		confirmprop.m_bConfirmCloseAllKeepCur = g_bConfirmCloseAllKeepCur;
		confirmprop.m_bConfirmCloseLock = g_bConfirmCloseLock;
		confirmprop.m_bConfirmExitDowning = g_bConfirmExitDowning;
		confirmprop.m_bConfirmUnsafePage = m_bConfirmUnsafePage;
		confirmprop._bEnableMixContentAlert = CGf_AppStatic::_instance._bEnableMixContentAlert;
		
		//menu prop
		menuprop.m_bSingleMenu = g_bSingleMenu;
		menuprop.m_bCompactFavBar = g_bCompactFavBar;
		menuprop.m_nMenuSize = m_nMenuWidth;		
		menuprop.m_nMenuSizeOther = CGf_AppStatic::_instance._nMenuWidthOther;
		menuprop.m_bDisplayOpenAll = m_bDisplayOpenAll;
		menuprop.m_bAddFav = m_bDisplayAddFav;
		menuprop.m_bShowHidden = m_bShowHiddenFav;
		menuprop.m_bFavShowUrl = m_bFavShowUrl;
		
		//tab
		tabprop.m_nMaxChars = m_nTitleLen;
		tabprop.m_nMaxWidth = m_nMaxTabWidth;
		tabprop.m_nMinWidth = m_nMinTabWidth;
		tabprop.m_nLDBClick	= m_nAllowDBC;
		tabprop.m_nMCLICK = m_nAllowMC;
		tabprop.m_nRClick = m_nAllowRC;
		tabprop.m_bAutoFitTab = m_bAutoTab;
		tabprop.m_bMultiTab = m_bMultiLineTab;
		tabprop.m_bClickClose = g_bClickClose;
		tabprop.m_bShowIndex = g_bShowIndex;
		tabprop.m_bWheelTab = m_bWheelTab;
		tabprop.m_bWheelOnTab = m_bWheelOnTab;
		tabprop.m_bShowDot = g_bShowDot;
		tabprop.m_bShowX= CGf_AppStatic::_instance._bShowXIconInTab;
		tabprop.m_bShowRead = g_bShowRead;
		tabprop.m_bShowFavIcon = g_bShowFavIcon;
		tabprop.m_nLDbClickTabBlank = m_nLDbClickTabBlank;
		tabprop.m_nRDbClickTabBlank = m_nRDbClickTabBlank;
		//tab style
		tabsprop.m_nTabStyle = m_nTabStyle;
		tabsprop.m_bTabDefaultColor = m_bTabDefaultColor;
		tabsprop.m_crUnSelColor = m_crUnSelColor;
		tabsprop.m_crSelColor = m_crSelColor;
		tabsprop.m_crUnSelFontColor = m_crUnSelFontColor;
		tabsprop.m_crSelFontColor = m_crSelFontColor;
		tabsprop.m_crSepColor = m_crSepColor;
		tabsprop.m_bTopTab = m_bTopTab;
		//save
		//因为m_strDefaultDir记录的是绝对地址,所以要从ini里面读
		saveprop.m_strSavePath = _app->GetProfileString("Settings", "DefaultDir",app->m_strDefaultDir);
		saveprop.m_strSavePathSort = _app->GetProfileString("Settings", "DefaultDirSort",app->m_strDefaultDirSort);
		saveprop.m_strImagePath = _app->GetProfileString("Settings", "ImagePath",app->m_strImagePath);
		//
		saveprop.m_nAutoSave = m_nAutoSave;
		saveprop.m_bCateWithSite = m_bCateWithSite;
		saveprop.m_bImgCateWithSite = m_bImgCateWithSite;
		saveprop.m_bImgCateWithSite = m_bImgCateWithSite;
		saveprop.m_bNameByTitle = m_bNameByTitle;
		saveprop.m_bSavePageByDate = m_bSavePageByDate;
		saveprop.m_bShowSortSaveDlg = m_bShowSortSaveDlg;
		//other
		otherprop.m_strCE1 = m_strCE1;
		otherprop.m_strCE2 = m_strCE2;
		otherprop.m_strSE1 = m_strSE1;
		otherprop.m_strSE2 = m_strSE2;
		otherprop.m_strCSE1 = m_strCSE1;
		otherprop.m_strCSE2 = m_strCSE2;
		//otherprop.m_nCharSet = m_nRegularCharSet;
		otherprop.m_nMenuStyle = m_nMenuStyle;
		//
		otherprop.m_bShowTray = m_bShowTrayIcon;
		otherprop.m_bMinTray = m_bMinimizeToTray;
		otherprop.m_bCloseTray = m_bCloseToTray;
		otherprop.m_bCloseAllWhenCloseToTray = m_bCloseAllWhenCloseToTray;
		otherprop._bOpenWordOutOfIE = CGf_AppStatic::_instance._bOpenWordOutOfIE;
		//win
		winprop.m_nMaxWins = g_nMaxWins;
		winprop.m_nInterval = m_nInterval;
		winprop.m_nOpenNew = m_nNewWindow;
		winprop.m_bActiveNew = m_bActiveNewWindow;
		winprop.m_nNewSequence = m_nNewSequence;
		winprop.m_nCloseSequence = m_nCloseSequence;
		winprop.m_bOpenFolderOut = m_bOpenFolderOut;
		winprop.m_nCollectorTrans = m_nCollectorTrans;
		winprop.m_nMonitorTrans = m_nMonitorTrans;
		winprop.m_nNewAddress=g_bAddressInNewWindow;
		winprop.m_bNewFav=g_bFavInNewWindow;
		winprop.m_bNewHistory=g_bHisInNewWindow;
		winprop.m_bNewLinks=g_bLinkInNewWindow;
		winprop.m_bNewLocal=g_bFileInNewWindow;
		winprop.m_bNewHome=g_bHomeInNewWindow;
		winprop.m_bReloadUnShownImage=m_bReloadUnShownImage;
		//
		winprop.m_bActiveFileNew=m_bActiveFileNew;
		winprop.m_bAutoRefreshActive=m_bAutoRefreshActive;
		winprop.m_bKeepOnePage = m_bKeepOnePage;
		winprop.m_bBlankMonitorClip = m_bBlankMonitorClip;
		winprop.m_bActiveNewAddress = m_bActiveNewAddress;
		winprop.m_bUseTargetFrameName = m_bUseTargetFrameName;
		//
		
		startprop.m_nStartMode = CGf_StartupManager::GetStartMode(CGfPathManager::GetUserConfig() );
		startprop.m_strStartHomeURL = CGf_StartupManager::GetStartupUrlStr(CGfPathManager::GetUserConfig() );
		startprop.m_bCloseCleanCache = m_bCloseCleanCache;
		startprop.m_bCloseCleanCookies = m_bCloseCleanCookies;
		startprop.m_bCloseCleanFormData = m_bCloseCleanFormData;
		startprop.m_bCloseCleanHistory = m_bCloseCleanHistory;
		startprop.m_bCloseCleanTypedAddress = m_bCloseCleanTypedAddress;
		startprop.m_bCloseCleanTypedSearch = m_bCloseCleanTypedSearch;
		startprop.m_bCloseCleanUndoClose = m_bCloseCleanUndoClose;
		startprop.m_bMenuLoadLastClose = m_bMenuLoadLastClose;
		startprop.m_bOneInstance = g_bForbidMult;
		startprop.m_bLockHomepage = g_bLockHomepage;
		
		//auto run
		BOOL bAutoRun;
		strRet = _RegGetString(HKEY_LOCAL_MACHINE,
			"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
			GF_TITILE_UNCHANGE );
		
		startprop.m_bAutoRun = bAutoRun = (strRet.GetLength()>8 ? TRUE : FALSE);
		//
		
		//toolbar
		toolbarprop.m_bShowToolbarLabel = m_bShowToolbarLabel;
		toolbarprop.m_bShowAddressBarButton = m_bShowAddressBarButton;
		toolbarprop.m_bShowSearchBarButton = m_bShowSearchBarButton;
		toolbarprop.m_bGoProxyMenu = m_bGoProxyMenu;
		toolbarprop.m_bComboShowIcon = m_bComboShowIcon;
		toolbarprop.m_dwFsShow = m_dwFsShow;
		toolbarprop.m_dwAhShow = m_dwAhShow;
		//
		statusbarprop.m_strStatusStringFormat = m_strStatusStringFormat;
		//
		collectorprop.m_bOutputSaveTime = m_bOutputSaveTime;
		collectorprop.m_bOutputSaveFilter = m_bOutputSaveFilter;
		collectorprop.m_bOutputSaveUrlFilter = m_bOutputSaveUrlFilter;
		collectorprop.m_bOutputSaveImg = m_bOutputSaveImg;
		collectorprop.m_bOutputSavePage = m_bOutputSavePage;
		collectorprop.m_bOutputSaveText = m_bOutputSaveText;
		collectorprop.m_bOutputAutoClean = m_bOutputAutoClean;
		collectorprop.m_bCollectorAddtoBottom = m_bCollectorAddtoBottom;
		//mouse
		gestureprop.m_nLDbClick = m_nLDbClick;
		gestureprop.m_nRDbClick = m_nRDbClick;
		gestureprop.m_nLButtonDown = m_nLButtonDown;
		gestureprop.m_nMButtonDown = m_nMButtonDown;
		gestureprop.m_nRButtonDown = m_nRButtonDown;
		gestureprop.m_nLeftRight = m_nLeftRight;
		gestureprop.m_nRightLeft = m_nRightLeft;
		gestureprop.m_bShowGesture = m_bShowGesture;
		//
		mouseprop.m_nImgUp		= m_nImgUp;
		mouseprop.m_nImgDown	= m_nImgDown;
		mouseprop.m_nImgLeft	= m_nImgLeft;
		mouseprop.m_nImgRight	= m_nImgRight;
		
		mouseprop.m_nTextUp		= m_nTextUp;
		mouseprop.m_nTextDown	= m_nTextDown;
		mouseprop.m_nTextLeft	= m_nTextLeft;
		mouseprop.m_nTextRight	= m_nTextRight;
		
		mouseprop.m_nLinkUp		= m_nLinkUp;
		mouseprop.m_nLinkDown	= m_nLinkDown;
		mouseprop.m_nLinkLeft	= m_nLinkLeft;
		mouseprop.m_nLinkRight	= m_nLinkRight;
		mouseprop.m_bSimuDragLink	= m_bSimuDragLink;
		
		//
		flyProp.m_bKillFly = m_bUseFlyingFilter; //gffly
		flyProp.m_bKillFlyWhenFresh = m_bKillFlyWhenFresh;
		
		//
		popProp.m_bKillPop = g_bKillPop;
		popProp.m_bKillAutoWin = g_bKillAutoWin;
		popProp.m_bKillListWin = g_bKillListWin;
		popProp.m_bKillDupWin = g_bKillDupWin;
		//
		popProp.m_bNotConfirmPopupFilter = m_bNotConfirmPopupFilter;
		popProp.m_bPlayPopupFilterSound = m_bPlayPopupFilterSound;
		popProp.m_nPopDelay = (int)(m_nPopDelay*100);
		popProp.m_bFilterRefresh = m_bFilterRefresh;
		popProp.m_bNotFilterClickDialog = m_bNotFilterClickDialog;
		
		urlProp.m_bUseUrlFilter = m_bUseUrlFilter;
		urlProp.m_bKillBlank = m_bKillBlank;
		urlProp.m_strBlankContent = m_strBlankContent;
		
		sheet.SetStartPage(g_nStartPage);
		if(sheet.DoModal() == IDOK)
		{
			DWORD dwNewProp = 0;
			if (downprop.m_bDLImage)	dwNewProp |= DLCTL_DLIMAGES; 
			if (downprop.m_bDLSound)	dwNewProp |= DLCTL_BGSOUNDS;
			if (downprop.m_bDLVideo)	dwNewProp |= DLCTL_VIDEOS;
			if (!downprop.m_bDLScript)	dwNewProp |= DLCTL_NO_SCRIPTS;
			if (!downprop.m_bDLActivex)	dwNewProp |= DLCTL_NO_RUNACTIVEXCTLS;
			if (!downprop.m_bDLApplet)	dwNewProp |= DLCTL_NO_JAVA;
			SAVEINT(dwNewProp, m_dwDefaultProperty, "Settings", "DefaultProperty")
				//animations
				if (bDLAnimationOld != downprop.m_bDLAnimation)
				{
					_RegSetString(HKEY_CURRENT_USER,
						"Software\\Microsoft\\Internet Explorer\\Main",
						"Play_Animations",
						downprop.m_bDLAnimation ? "yes" : "no" );
				}
				//flash
				if (bDLFlashOld != downprop.m_bDLFlash)
				{
					dwRet = (downprop.m_bDLFlash==1 ? 0 : 0x400);
					_RegSetDword(HKEY_LOCAL_MACHINE,
						"Software\\Microsoft\\Internet Explorer\\ActiveX Compatibility\\{D27CDB6E-AE6D-11cf-96B8-444553540000}",
						"Compatibility Flags",
						dwRet );
				}
				//down all
				SAVEINT(downprop.m_nDownAll, m_nDownAll, "Settings", "DownAll")
					//
					//ftp prop
					SAVEINT(ftpprop.m_nFtpOpenType, m_nFtpOpenType,"FtpOpen","FtpOpenType")
					SAVESTR(ftpprop.m_strFtpExePath, m_strFtpExePath,"FtpOpen","FtpExePath")
					
					//genaral
					if(m_bAnimatIcon && m_bAnimatIcon != genprop.m_bAniIcon)
						m_animIcon.ShowFirstImage();
					m_bAnimatIcon = genprop.m_bAniIcon;
					
					if (m_bAnimatIcon && !m_nAnimateIconTimer)
					{
						m_nAnimateIconTimer = SetTimer(99, 250, NULL);
					}
					else if (!m_bAnimatIcon && m_nAnimateIconTimer)
					{
						KillTimer(m_nAnimateIconTimer);
						m_nAnimateIconTimer=FALSE;
					}
					
					SAVEINT(genprop.m_bTopCloseExitApp,g_bTopCloseExitApp,"Settings", "TopCloseExitApp")

					_ttSys.UpdateTipText( GetSysXTip(), this );
						
						//gffave
						SAVEINT(genprop.m_bAutoHideFavSideBar, CGf_AppStatic::_instance._bAutoHideFavSideBar,"Settings", "AutoHideFavSideBar")
						
						SAVEINT(genprop._bForbidAutoUpdate, CGf_AppStatic::_instance._bForbidAutoUpdate,"Settings", "ForbidAutoUpdate")
						SAVEINT(genprop._bKeepPageSize, CGf_AppStatic::_instance._bKeepPageSize,"Settings", "KeepPageSize")
						SAVEINT(genprop._iPageSize, CGf_AppStatic::_instance._iPageSize,"Settings", "PageSize")
						
						if (CGf_AppStatic::_instance._bKeepPageSize)
						{
							try
							{							
								CChildFrame* tcf = (CChildFrame*)MDIGetActive();
								tcf->m_pView->SetPageSize(genprop._iPageSize);
							}
							catch (...) {
								ASSERT(false);
							}
						}
						
						
							SAVEINT(genprop._bDirectOpenForOneWord, CGf_AppStatic::_instance._bDirectOpenForOneWord,"Settings","DirectOpenForOneWord");

							SAVEINT(genprop._bAlwaysIEMenuRightClickImg, CGf_AppStatic::_instance._bAlwaysIEMenuRightClickImg,"Settings","AlwaysIEMenuRightClickImg");

							SAVEINT(genprop._bEnableIE8Mode, CGf_AppStatic::_instance._bEnableIE8Mode,"Settings","EnableIE8Mode");
							if (CGf_AppStatic::_instance._bEnableIE8Mode)
							{
								CRegHelper::EnableIE8Mode();
							}
							else
							{
								CRegHelper::DisableIE8Mode();
							}
							
							//confirm
							SAVEINT(confirmprop.m_bConfirmExit,g_bConfirmExit,"Settings", "ConfirmExit")
								SAVEINT(confirmprop.m_bConfirmCloseAll,g_bConfirmCloseAll,"Settings", "ConfirmCloseAll")
								SAVEINT(confirmprop.m_bConfirmCloseAllKeepCur,g_bConfirmCloseAllKeepCur,"Settings", "ConfirmCloseAllKeepCur")
								SAVEINT(confirmprop.m_bConfirmCloseLock,g_bConfirmCloseLock,"Settings", "ConfirmCloseLock")
								
								SAVEINT(confirmprop.m_bConfirmExitDowning,g_bConfirmExitDowning,"Settings","ConfirmExitDowning");
							SAVEINT(confirmprop.m_bConfirmUnsafePage,m_bConfirmUnsafePage,"Settings","ConfirmEvilPage");
							SAVEINT(confirmprop._bEnableMixContentAlert, CGf_AppStatic::_instance._bEnableMixContentAlert,"Settings","EnableMixContentAlert");
							
							if (CGf_AppStatic::_instance._bEnableMixContentAlert)
							{
								CRegHelper::EnableMixContentAlert();
							}
							else
							{
								CRegHelper::DisableMixContentAlert();
							}
							
							//
							if(g_bSilent != confirmprop.m_bKeepSilent)
							{
								g_bSilent = confirmprop.m_bKeepSilent;
								_RegSetString(HKEY_CURRENT_USER,
									"Software\\Microsoft\\Internet Explorer\\Main",
									"Disable Script Debugger",
									g_bSilent ? "yes" : "no" );
								theApp.WriteProfileInt("Settings", "Silent", g_bSilent);
							}
							
							///////
							
							if (genprop.m_bForbidMainTitle != m_bForbidMainTitle)
							{
								m_bForbidMainTitle = genprop.m_bForbidMainTitle ;
								app->WriteProfileInt("Settings", "ForbidMainTitle", m_bForbidMainTitle);
								CChildFrame* tcf = (CChildFrame*)MDIGetActive();
								if (tcf)
								{
									if (m_bForbidMainTitle)
										tcf->SetWindowText("");
									else
										tcf->SetWindowText(tcf->m_strTitle);
								}
							}
							
							
							
							//menu prop
							if(m_bDisplayOpenAll != menuprop.m_bDisplayOpenAll)
							{
								m_bFavDirty = TRUE;
								m_bDisplayOpenAll = menuprop.m_bDisplayOpenAll;
								app->WriteProfileInt("Settings", "DisplayOpenAll", m_bDisplayOpenAll);
							}
							if(m_bDisplayAddFav != menuprop.m_bAddFav)
							{
								m_bFavDirty = TRUE;
								m_bDisplayAddFav = menuprop.m_bAddFav;
								app->WriteProfileInt("Settings", "DisplayAddFav", m_bDisplayAddFav);
							}
							if(g_bSingleMenu != menuprop.m_bSingleMenu)
							{
								m_bFavDirty = TRUE;
								g_bSingleMenu = menuprop.m_bSingleMenu;
								app->WriteProfileInt("Settings","SingleMenu", g_bSingleMenu);
								InitGroups();
							}
							if(m_bShowHiddenFav != menuprop.m_bShowHidden)
							{
								m_bFavDirty = TRUE;
								m_bShowHiddenFav = menuprop.m_bShowHidden;
								app->WriteProfileInt("Settings", "ShowHiddenFav", m_bShowHiddenFav);
							}
							if (menuprop.m_bFavShowUrl != m_bFavShowUrl)
							{
								m_bFavDirty = TRUE;
								m_bFavShowUrl = menuprop.m_bFavShowUrl ;
								app->WriteProfileInt("Settings", "FavShowUrl", m_bFavShowUrl);
							}
							if(g_bCompactFavBar != menuprop.m_bCompactFavBar)
							{
								m_bFavDirty = TRUE;
								g_bCompactFavBar = menuprop.m_bCompactFavBar;
								InitFavBarImage();
								InitLinkBarImage();
								app->WriteProfileInt("Settings", "CompactFavBar", g_bCompactFavBar);
							}
							SAVEINT(menuprop.m_nMenuSize, m_nMenuWidth,"Settings","MenuSize")
							SAVEINT(menuprop.m_nMenuSizeOther, CGf_AppStatic::_instance._nMenuWidthOther,"Settings","MenuSizeOther")
								
								//tab
							SAVEINT(tabprop.m_nLDBClick,m_nAllowDBC,"Settings","AllowDBC");							
							SAVEINT(tabprop.m_nMCLICK,m_nAllowMC,"Settings","AllowMC");
							SAVEINT(tabprop.m_nRClick,m_nAllowRC,"Settings","AllowRC2");
							SAVEINT(tabprop.m_nLDbClickTabBlank,m_nLDbClickTabBlank,"Settings","LDbClickTabBlank");
							SAVEINT(tabprop.m_nRDbClickTabBlank,m_nRDbClickTabBlank,"Settings","RDbClickTabBlank");
							
							bool bUpdateTab = false;//need update title
							bool bUpdateTabTitle = false;//need redraw tab
							if (m_nTitleLen != tabprop.m_nMaxChars)
							{
								m_nTitleLen = tabprop.m_nMaxChars;
								bUpdateTabTitle = true;
							}
							if (m_nMaxTabWidth != tabprop.m_nMaxWidth && (tabprop.m_nMaxWidth >= 50)) //gf 50是图片前后的2个25
							{
								m_nMaxTabWidth = tabprop.m_nMaxWidth;
								bUpdateTab = true;
								SAVEINT(tabprop.m_nMaxWidth, m_nMaxTabWidth,"Settings", "MaxTabWidth");
							}
							if(m_nMinTabWidth != tabprop.m_nMinWidth && (tabprop.m_nMinWidth >= 50) && tabprop.m_nMaxWidth>=tabprop.m_nMinWidth)
							{
								m_nMinTabWidth = tabprop.m_nMinWidth;
								bUpdateTab = true;

								SAVEINT(tabprop.m_nMinWidth, m_nMinTabWidth,"Settings", "MinTabWidth");
							}
							//
							SAVEINT(tabprop.m_bClickClose,g_bClickClose,"Settings","ClickClose");
							if (g_bShowIndex != tabprop.m_bShowIndex)
							{
								SAVEINT(tabprop.m_bShowIndex,g_bShowIndex,"Settings","ShowIndex");
								bUpdateTabTitle = true;
								bUpdateTab = true;
							}
							if (g_bShowDot != tabprop.m_bShowDot)
							{
								SAVEINT(tabprop.m_bShowDot,g_bShowDot,"Settings","ShowDot");
								bUpdateTabTitle = true;
								bUpdateTab = true;
							}

							if (CGf_AppStatic::_instance._bShowXIconInTab != tabprop.m_bShowX) //gfx
							{
								SAVEINT(tabprop.m_bShowX,CGf_AppStatic::_instance._bShowXIconInTab,"Settings","ShowX");
								bUpdateTabTitle = true;
								bUpdateTab = true;
							}
							if (g_bShowRead != tabprop.m_bShowRead)
							{
								SAVEINT(tabprop.m_bShowRead,g_bShowRead,"Settings","ShowRead");
								bUpdateTabTitle = true;
								bUpdateTab = true;
							}
							if (g_bShowFavIcon != tabprop.m_bShowFavIcon)
							{
								SAVEINT(tabprop.m_bShowFavIcon,g_bShowFavIcon,"Settings","ShowFavIcon");
								bUpdateTabTitle = true;
								bUpdateTab = true;
							}
							if (bUpdateTabTitle)
								UpdateTabTitleIndex(0);
							if (bUpdateTab)
								PostMessage(WM_UPDATE_TAB, TRUE);
							//update tab style...
							if(m_bMultiLineTab != tabprop.m_bMultiTab)
							{
								OnOptionMultiTab();
								m_bMultiLineTab = tabprop.m_bMultiTab;
							}
							if(m_bAutoTab != tabprop.m_bAutoFitTab)
							{
								OnOptionAutotab();
								m_bAutoTab = tabprop.m_bAutoFitTab;
							}
							SAVEINT(tabprop.m_bWheelTab,m_bWheelTab,"Mouse", "WheelTab")
								SAVEINT(tabprop.m_bWheelOnTab,m_bWheelOnTab,"Mouse", "WheelOnTab")
								//tab style
								bUpdateTab=FALSE;
							if (tabsprop.m_nTabStyle != m_nTabStyle)
							{
								m_nTabStyle = tabsprop.m_nTabStyle;
								app->WriteProfileInt("TabStyle","TabStyle2",m_nTabStyle);
								OnOptionTabStyle();//!!
								bUpdateTab = TRUE;
							}
							//SAVEINT(tabsprop.m_bTabDefaultColor,m_bTabDefaultColor,"TabStyle","TabDefaultColor");
							if (tabsprop.m_crUnSelColor != m_crUnSelColor)
							{
								m_crUnSelColor = tabsprop.m_crUnSelColor;
								app->WriteProfileInt("TabStyle","UnSelColor",m_crUnSelColor);
								m_wndTab.SetUnSelColor(m_crUnSelColor);
								bUpdateTab = TRUE;
							}
							
							if (tabsprop.m_crSelColor != m_crSelColor) //gf
							{
								m_crSelColor = tabsprop.m_crSelColor;
								app->WriteProfileInt("TabStyle","SelColor",m_crSelColor);
								m_wndTab.SetSelColor(m_crSelColor);
								bUpdateTab = TRUE;
							}
							
							
							//
							if (tabsprop.m_crUnSelFontColor != m_crUnSelFontColor)
							{
								m_crUnSelFontColor = tabsprop.m_crUnSelFontColor;
								app->WriteProfileInt("TabStyle","UnSelFontColor",m_crUnSelFontColor);
								m_wndTab.SetUnSelFontColor(m_crUnSelFontColor);
								bUpdateTab = TRUE;
							}
							if (tabsprop.m_crSelFontColor != m_crSelFontColor)
							{
								m_crSelFontColor = tabsprop.m_crSelFontColor;
								app->WriteProfileInt("TabStyle","SelFontColor",m_crSelFontColor);
								m_wndTab.SetSelFontColor(m_crSelFontColor);
								bUpdateTab = TRUE;
							}
							if (tabsprop.m_crSepColor != m_crSepColor)
							{
								m_crSepColor = tabsprop.m_crSepColor;
								app->WriteProfileInt("TabStyle","SepColor",m_crSepColor);
								m_wndTab.SetSepColor(m_crSepColor);
								bUpdateTab = TRUE;
							}
							if(m_bTopTab != tabsprop.m_bTopTab)
							{
								ChangTabPos(tabsprop.m_bTopTab);
								m_bTopTab = tabsprop.m_bTopTab;
								app->WriteProfileInt("TabStyle", "TabPos", m_bTopTab);
								bUpdateTab = FALSE;//no use paint tab again
								//
								ReloadTabMenu();
							}
							if (bUpdateTab)
								m_wndTab.Invalidate();
							
							//skin path
							CHECK_PATH(genprop.m_strSkinPath);
							CString strAbsDir = genprop.m_strSkinPath;
							DO_RELATIVE_URL(strAbsDir);
							if(app->m_strSkinPath != strAbsDir)
							{
								app->WriteProfileString("Settings", "SkinPath", genprop.m_strSkinPath);
								app->m_strSkinPath = strAbsDir;
								_CreateFullDir(strAbsDir);
							}
							//group path
							CHECK_PATH(genprop.m_strGroupPath);
							strAbsDir = genprop.m_strGroupPath;
							DO_RELATIVE_URL(strAbsDir);
							if (theApp.m_strGroupPath != strAbsDir)
							{
								app->WriteProfileString("Settings", "GroupPath", genprop.m_strGroupPath);
								theApp.m_strGroupPath = strAbsDir;
								_CreateFullDir(strAbsDir);
								InitGroups();
							}

							
							SAVESTR(genprop.m_strUserAppTitle, CGf_AppStatic::_instance._strUserAppTitle,"Settings", "UserAppTitle");
							SetWindowText(CGf_AppStatic::_instance._strUserAppTitle);

							//fav path
							CHECK_PATH(genprop.m_strFavPath);
							if (!genprop.m_bUseDefaultFavPath && genprop.m_strFavPath.GetLength()<1)
								;// do nothing
							else if (m_bUseDefaultFavPath != genprop.m_bUseDefaultFavPath)
							{
								m_bUseDefaultFavPath = genprop.m_bUseDefaultFavPath;
								app->WriteProfileInt("Settings", "UseDefaultFavPath", m_bUseDefaultFavPath);
								if (!genprop.m_bUseDefaultFavPath && m_strFavPath != genprop.m_strFavPath)
								{
									m_strFavPath = genprop.m_strFavPath;
									app->WriteProfileString("Settings", "FavPath", m_strFavPath);
								}
								m_bFavDirty = TRUE;
							}
							else if (!genprop.m_bUseDefaultFavPath && m_strFavPath != genprop.m_strFavPath)
							{
								m_strFavPath = genprop.m_strFavPath;
								app->WriteProfileString("Settings", "FavPath", m_strFavPath);
								m_bFavDirty = TRUE;
							}
							//link path
							CHECK_PATH(genprop.m_strLinkPath);
							if (!genprop.m_bUseDefaultLinkPath && genprop.m_strLinkPath.GetLength()<1 )
								;// do nothing
							else if (m_bUseDefaultLinkPath != genprop.m_bUseDefaultLinkPath)
							{
								m_bUseDefaultLinkPath = genprop.m_bUseDefaultLinkPath;
								app->WriteProfileInt("Settings", "UseDefaultLinkPath", m_bUseDefaultLinkPath);
								if (!genprop.m_bUseDefaultLinkPath && m_strLinkPath != genprop.m_strLinkPath)
								{
									m_strLinkPath = genprop.m_strLinkPath;
									app->WriteProfileString("Settings", "LinkPath", m_strLinkPath);
								}
								m_bFavDirty = TRUE;
							}
							else if (!genprop.m_bUseDefaultLinkPath && m_strLinkPath != genprop.m_strLinkPath)
							{
								m_strLinkPath = genprop.m_strLinkPath;
								app->WriteProfileString("Settings", "LinkPath", m_strLinkPath);
								m_bFavDirty = TRUE;
							}

							
							//page path
							CHECK_PATH(saveprop.m_strSavePath);
							strAbsDir = saveprop.m_strSavePath;
							DO_RELATIVE_URL(strAbsDir);
							if (app->m_strDefaultDir != strAbsDir)
							{
								app->WriteProfileString("Settings", "DefaultDir", saveprop.m_strSavePath);
								app->m_strDefaultDir = strAbsDir ;
								//_CreateFullDir(strAbsDir);
							}
							//page path sort
							CHECK_PATH(saveprop.m_strSavePathSort);
							strAbsDir = saveprop.m_strSavePathSort;
							DO_RELATIVE_URL(strAbsDir);
							if (app->m_strDefaultDirSort != strAbsDir && strAbsDir.GetLength())
							{
								app->WriteProfileString("Settings", "DefaultDirSort", saveprop.m_strSavePathSort);
								app->m_strDefaultDirSort = strAbsDir;
								//_CreateSortSavePath();
							}
							//img path
							CHECK_PATH(saveprop.m_strImagePath);
							strAbsDir = saveprop.m_strImagePath ;
							DO_RELATIVE_URL(strAbsDir);
							if (app->m_strImagePath != strAbsDir)
							{
								app->WriteProfileString("Settings", "ImagePath", saveprop.m_strImagePath);
								app->m_strImagePath = strAbsDir;
								//_CreateFullDir(strAbsDir);
							}
							

							SAVEINT(saveprop.m_nAutoSave,m_nAutoSave,"Settings", "AutoSave");
							SAVEINT(saveprop.m_bCateWithSite,m_bCateWithSite,"Settings", "CateWithSite");
							SAVEINT(saveprop.m_bImgCateWithSite,m_bImgCateWithSite,"Settings", "ImgCateWithSite");
							SAVEINT(saveprop.m_bNameByTitle, m_bNameByTitle, "Settings", "NameByTitle");
							SAVEINT(saveprop.m_bSavePageByDate, m_bSavePageByDate, "Settings", "SavePageByDate");
							m_bShowSortSaveDlg = saveprop.m_bShowSortSaveDlg;
							
							SAVESTR(otherprop.m_strCE1,m_strCE1,"Domains", "CE1")
								SAVESTR(otherprop.m_strCE2,m_strCE2,"Domains", "CE2")
								SAVESTR(otherprop.m_strSE1,m_strSE1,"Domains", "SE1")
								SAVESTR(otherprop.m_strSE2,m_strSE2,"Domains", "SE2")
								SAVESTR(otherprop.m_strCSE1,m_strCSE1,"Domains", "CSE1")
								SAVESTR(otherprop.m_strCSE2,m_strCSE2,"Domains", "CSE2")
								//
								SAVEINT(otherprop.m_nMenuStyle,m_nMenuStyle,"Settings", "MenuStyle");
							//
							if (m_bShowTrayIcon != otherprop.m_bShowTray)
							{
								m_bShowTrayIcon = otherprop.m_bShowTray;
								if (m_bShowTrayIcon)
									AddTrayIcon();
								else
									DelTrayIcon();
								app->WriteProfileInt("Settings", "ShowTrayIcon", m_bShowTrayIcon);
							}
							
							SAVEINT(otherprop.m_bMinTray,m_bMinimizeToTray,"Settings", "MinimizeToTray")
								SAVEINT(otherprop.m_bCloseTray,m_bCloseToTray,"Settings", "CloseToTray")
								SAVEINT(otherprop.m_bCloseAllWhenCloseToTray,m_bCloseAllWhenCloseToTray,"Settings", "CloseAllWhenCloseToTray")
								
								
								if (otherprop._bOpenWordOutOfIE != CGf_AppStatic::_instance._bOpenWordOutOfIE)
								{
									CRegHelper::SetOpenWordOutIE(otherprop._bOpenWordOutOfIE);
								}
								
								SAVEINT(otherprop._bOpenWordOutOfIE, CGf_AppStatic::_instance._bOpenWordOutOfIE,"Settings", "OpenWordOutOfIE")
									
									//window
									if (winprop.m_nOpenNew != m_nNewWindow)
									{
										CMenu* pNew = SMP_NEW; //gfm menu
										pNew->SetDefaultItem(m_nNewWindow, TRUE);
										SAVEINT(winprop.m_nOpenNew, m_nNewWindow, "Settings", "NewWindow")
									}
									SAVEINT(winprop.m_nInterval, m_nInterval, "Settings", "Interval")
										SAVEINT(winprop.m_nMaxWins, g_nMaxWins, "Settings","MaxWins")		
										SAVEINT(winprop.m_nCloseSequence, m_nCloseSequence, "Settings", "CloseSequence")
										SAVEINT(winprop.m_bOpenFolderOut, m_bOpenFolderOut, "Settings", "OpenFolderOut")
										
										m_bActiveNewWindow = winprop.m_bActiveNew;
									m_nNewSequence = winprop.m_nNewSequence;
									
									if (m_nCollectorTrans != winprop.m_nCollectorTrans && winprop.m_nCollectorTrans >= 5)
									{
										m_nCollectorTrans = winprop.m_nCollectorTrans;
										if (m_pCollectDlg)
											m_pCollectDlg->CollectorSetTrans(m_nCollectorTrans);
										app->WriteProfileInt("Settings", "CollectorTrans", m_nCollectorTrans);
									}
									if (m_nMonitorTrans != winprop.m_nMonitorTrans && winprop.m_nMonitorTrans >= 5)
									{
										m_nMonitorTrans = winprop.m_nMonitorTrans;
										if (m_pMonitor)
											m_pMonitor->CollectorSetTrans(m_nMonitorTrans);
										app->WriteProfileInt("Settings", "MonitorTrans", m_nMonitorTrans);
									}
									SAVEINT(winprop.m_nNewAddress,g_bAddressInNewWindow,"Settings","AddressInNewWindow");
									SAVEINT(winprop.m_bNewFav,g_bFavInNewWindow,"Settings","FavInNewWindow");
									SAVEINT(winprop.m_bNewHistory,g_bHisInNewWindow,"Settings","HisInNewWindow");
									SAVEINT(winprop.m_bNewLinks,g_bLinkInNewWindow,"Settings","LinkInNewWindow");
									SAVEINT(winprop.m_bNewLocal,g_bFileInNewWindow,"Settings","FileInNewWindow");
									SAVEINT(winprop.m_bNewHome,g_bHomeInNewWindow,"Settings","HomeInNewWindow");
									//
									SAVEINT(winprop.m_bActiveFileNew,m_bActiveFileNew,"Settings","ActiveFileNew");
									//
									m_bAutoRefreshActive = winprop.m_bAutoRefreshActive;
									SAVEINT(winprop.m_bKeepOnePage,m_bKeepOnePage,"Settings","KeepOnePage");
									SAVEINT(winprop.m_bBlankMonitorClip,m_bBlankMonitorClip,"Settings","BlankMonitorClip2");
									SAVEINT(winprop.m_bActiveNewAddress,m_bActiveNewAddress,"Settings","ActiveNewAddress");
									SAVEINT(winprop.m_bUseTargetFrameName,m_bUseTargetFrameName,"Settings","UseTargetFrameName");
									SAVEINT(winprop.m_bReloadUnShownImage,m_bReloadUnShownImage,"Settings","ReloadUnShownImage3");
									
									//start prop
									CGf_StartupManager::SaveStartMode(startprop.m_nStartMode, CGfPathManager::GetUserConfig() );
									CGf_StartupManager::SaveStartURL(startprop.m_strStartHomeURL, CGfPathManager::GetUserConfig() );

									SAVEINT(startprop.m_bMenuLoadLastClose, m_bMenuLoadLastClose, "Settings", "MenuLoadLastClose");
									
									
									SAVEINT(startprop.m_bOneInstance,g_bForbidMult,"Settings", "ForbidMult")
										
										if(startprop.m_bLockHomepage != g_bLockHomepage && startprop.m_bLockHomepage)
											m_strHomePage = GetHomePage();
										
										g_bLockHomepage = startprop.m_bLockHomepage;
										
										//auto run
										if (bAutoRun != startprop.m_bAutoRun)
										{
											HKEY	hKey;
											TCHAR	sz[MAX_PATH];
											bAutoRun = startprop.m_bAutoRun;
											if(RegOpenKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), &hKey) == ERROR_SUCCESS)
											{
												if(bAutoRun)
												{
													HINSTANCE hInstance = AfxGetResourceHandle();
													if (GetModuleFileName(hInstance, sz, MAX_PATH))
													{
														CString str=sz;
														str.Replace("\\MyIE9Data\\MyIE9.exe", "\\MyIE9.exe");
														RegSetValueEx(hKey, GF_TITILE_UNCHANGE, NULL, REG_SZ, (LPBYTE)(LPCTSTR)str, strlen(str));
													}
												}
												else
													RegDeleteValue(hKey, GF_TITILE_UNCHANGE );
												RegCloseKey(hKey);
											}
										}
										//end 
									//close clean
									SAVEINT(startprop.m_bCloseCleanCache, m_bCloseCleanCache, "CloseMode", "CloseCleanCache");
									SAVEINT(startprop.m_bCloseCleanCookies, m_bCloseCleanCookies, "CloseMode", "CloseCleanCookies");
									SAVEINT(startprop.m_bCloseCleanFormData, m_bCloseCleanFormData, "CloseMode", "CloseCleanFormData");
									SAVEINT(startprop.m_bCloseCleanHistory, m_bCloseCleanHistory, "CloseMode", "CloseCleanHistory");
									SAVEINT(startprop.m_bCloseCleanTypedAddress, m_bCloseCleanTypedAddress, "CloseMode", "CloseCleanTypedAddress");
									SAVEINT(startprop.m_bCloseCleanTypedSearch, m_bCloseCleanTypedSearch, "CloseMode", "CloseCleanTypedSearch");
									SAVEINT(startprop.m_bCloseCleanUndoClose, m_bCloseCleanUndoClose, "CloseMode", "CloseCleanUndoClose");
									
									m_bCloseCleanTypedAddress = startprop.m_bCloseCleanTypedAddress ;
									m_bCloseCleanTypedSearch = startprop.m_bCloseCleanTypedSearch ;
									m_bCloseCleanUndoClose = startprop.m_bCloseCleanUndoClose ;
									m_bMenuLoadLastClose = startprop.m_bMenuLoadLastClose ;		
									
									//toolbar
									if (m_bShowToolbarLabel != toolbarprop.m_bShowToolbarLabel)
									{
										m_bShowToolbarLabel = toolbarprop.m_bShowToolbarLabel;
										app->WriteProfileInt("Settings", "ShowToolbarLabel", m_bShowToolbarLabel);
										ShowBarLabel();
									}
									
									if (m_bShowAddressBarButton != toolbarprop.m_bShowAddressBarButton)
									{
										m_bShowAddressBarButton = toolbarprop.m_bShowAddressBarButton;
										app->WriteProfileInt("Settings", "ShowAddressBarButton", m_bShowAddressBarButton);
										CRect rect;
										m_AddressBar.GetWindowRect(&rect);
										m_AddressBar.PostMessage(WM_SIZE, 0, ((rect.Height())<<16)+rect.Width());
									}
									if (m_bShowSearchBarButton != toolbarprop.m_bShowSearchBarButton)
									{
										m_bShowSearchBarButton = toolbarprop.m_bShowSearchBarButton;
										app->WriteProfileInt("Settings", "ShowSearchBarButton", m_bShowSearchBarButton);
										CRect rect;
										m_SearchBar.GetWindowRect(&rect);
										m_SearchBar.PostMessage(WM_SIZE, 0, ((rect.Height())<<16)+rect.Width());
									}
									//
									if (m_strStatusStringFormat != statusbarprop.m_strStatusStringFormat)
									{
										m_strStatusStringFormat = statusbarprop.m_strStatusStringFormat;
										app->WriteProfileString("Settings", "StatusDispFormat", m_strStatusStringFormat);
										//SetDispWidth();
									}
									SAVEINT(toolbarprop.m_bGoProxyMenu, m_bGoProxyMenu, "Settings", "GoProxyMenu2")
										
										if (m_bComboShowIcon != toolbarprop.m_bComboShowIcon)
										{
											m_bComboShowIcon = toolbarprop.m_bComboShowIcon;
											m_AddressBar.InitImage();
											m_SearchBar.InitImage();
											app->WriteProfileInt("Settings", "ComboShowIcon", m_bComboShowIcon);
										}
										if (m_dwFsShow != toolbarprop.m_dwFsShow)
										{
											if (m_bFullScreenMode)
											{
												OnViewFullscreen();
												m_dwFsShow = toolbarprop.m_dwFsShow;
												OnViewFullscreen();
											}
											else
												m_dwFsShow = toolbarprop.m_dwFsShow;
											app->WriteProfileInt("Settings", "FullScreenShow", m_dwFsShow);
										}
										if (m_dwAhShow != toolbarprop.m_dwAhShow)
										{
											if (m_bAutoHideMode)
											{
												OnViewAutoHide();
												m_dwAhShow = toolbarprop.m_dwAhShow;
												OnViewAutoHide();
											}
											else
												m_dwAhShow = toolbarprop.m_dwAhShow;
											app->WriteProfileInt("Settings", "AutoHideShow", m_dwAhShow);
										}
										//
										SAVEINT(collectorprop.m_bOutputSaveTime, m_bOutputSaveTime, "Collector", "OutputSaveTime");
										SAVEINT(collectorprop.m_bOutputSaveFilter, m_bOutputSaveFilter, "Collector", "OutputSaveFilter");
										SAVEINT(collectorprop.m_bOutputSaveUrlFilter, m_bOutputSaveUrlFilter, "Collector", "OutputSaveUrlFilter");
										SAVEINT(collectorprop.m_bOutputSavePage, m_bOutputSavePage, "Collector", "OutputSavePage");
										SAVEINT(collectorprop.m_bOutputSaveImg, m_bOutputSaveImg, "Collector", "OutputSaveImg");
										SAVEINT(collectorprop.m_bOutputSaveText, m_bOutputSaveText, "Collector", "OutputSaveText");
										SAVEINT(collectorprop.m_bOutputAutoClean, m_bOutputAutoClean, "Collector", "OutputAutoClean");
										SAVEINT(collectorprop.m_bCollectorAddtoBottom, m_bCollectorAddtoBottom, "Collector", "CollectorAddtoBottom");
										//mouse
										SAVEINT(gestureprop.m_nLDbClick, m_nLDbClick,"Mouse","LDbClick");
										SAVEINT(gestureprop.m_nRDbClick, m_nRDbClick,"Mouse","RDbClick");
										SAVEINT(gestureprop.m_nLButtonDown, m_nLButtonDown, "Mouse", "LButtonDown");
										SAVEINT(gestureprop.m_nMButtonDown, m_nMButtonDown, "Mouse", "MButtonDown");
										SAVEINT(gestureprop.m_nRButtonDown, m_nRButtonDown, "Mouse", "RButtonDown");
										SAVEINT(gestureprop.m_nLeftRight, m_nLeftRight, "Mouse", "LeftRight");
										SAVEINT(gestureprop.m_nRightLeft, m_nRightLeft, "Mouse", "RightLeft");
										SAVEINT(gestureprop.m_bShowGesture, m_bShowGesture, "Mouse", "ShowGesture");
										//
										SAVEINT(mouseprop.m_nImgUp,m_nImgUp,"Mouse","ImgUp");
										SAVEINT(mouseprop.m_nImgDown,m_nImgDown,"Mouse","ImgDown");
										SAVEINT(mouseprop.m_nImgLeft,m_nImgLeft,"Mouse","ImgLeft");
										SAVEINT(mouseprop.m_nImgRight,m_nImgRight,"Mouse","ImgRight");
										
										SAVEINT(mouseprop.m_nTextUp,m_nTextUp,"Mouse","TextUp");
										SAVEINT(mouseprop.m_nTextDown,m_nTextDown,"Mouse","TextDown");
										SAVEINT(mouseprop.m_nTextLeft,m_nTextLeft,"Mouse","TextLeft");
										SAVEINT(mouseprop.m_nTextRight,m_nTextRight,"Mouse","TextRight");
										
										SAVEINT(mouseprop.m_nLinkUp,m_nLinkUp,"Mouse","LinkUp");
										SAVEINT(mouseprop.m_nLinkDown,m_nLinkDown,"Mouse","LinkDown");
										SAVEINT(mouseprop.m_nLinkLeft,m_nLinkLeft,"Mouse","LinkLeft");
										SAVEINT(mouseprop.m_nLinkRight,m_nLinkRight,"Mouse","LinkRight");
										
										SAVEINT(mouseprop.m_bSimuDragLink,m_bSimuDragLink,"Mouse","SimuDragLink");
										//url filter
										if(m_bUseUrlFilter != urlProp.m_bUseUrlFilter)
										{
											m_bUseUrlFilter = urlProp.m_bUseUrlFilter;
											InitialNameSpace();
										}
										if (m_bKillBlank != urlProp.m_bKillBlank || m_strBlankContent != urlProp.m_strBlankContent)
										{
											SAVEINT(urlProp.m_bKillBlank,m_bKillBlank,"Filter","KillBlank");
											SAVESTR(urlProp.m_strBlankContent,m_strBlankContent,"Filter","BlankContent");
											RefreshKillBlankCode();
										}
										
										//gf fly
										m_bUseFlyingFilter = flyProp.m_bKillFly;
										SAVEINT(flyProp.m_bKillFlyWhenFresh, m_bKillFlyWhenFresh,"Filter","KillFlyWhenRefresh");
										
										//pop filter
										g_bKillPop = popProp.m_bKillPop;
										g_bKillAutoWin = popProp.m_bKillAutoWin;
										g_bKillListWin = popProp.m_bKillListWin;
										g_bKillDupWin = popProp.m_bKillDupWin;
										
										if (!GF_ALLOW_DUPLICATE_FILTER)
										{
											g_bKillDupWin = FALSE;
										}
										
										//
										SAVEINT(popProp.m_bNotConfirmPopupFilter,m_bNotConfirmPopupFilter,"Settings", "NotConfirmPopupFilter");
										SAVEINT(popProp.m_bPlayPopupFilterSound,m_bPlayPopupFilterSound,"Settings", "PlayPopupFilterSound");
										SAVEINT(popProp.m_bFilterRefresh,m_bFilterRefresh,"Settings", "FilterRefresh");
										SAVEINT(popProp.m_bNotFilterClickDialog,m_bNotFilterClickDialog,"Settings", "NotFilterClickDialog");
										//
										if (popProp.m_nPopDelay != m_nPopDelay*100)
										{
											theApp.WriteProfileInt("Settings", "PopupBound", popProp.m_nPopDelay);
											m_nPopDelay = (double)popProp.m_nPopDelay/100;
										}
										//
										m_bSaveConfig = TRUE;
	}
	g_nStartPage = sheet.GetCurrentPage();
	
	
	}GF_CATCH
}

void CMainFrame::OnRtabStickname() 
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	
	if(tcf!=NULL)
	{
		CInputDlg dlg;
		CString strMsg;
		LOADSTR(strMsg ,IDS_INPUT_TITLE);
		dlg.m_strMsg = strMsg;
		dlg.m_strOutput = tcf->m_strTitle;
		
		if(dlg.DoModal() == IDOK)
		{
			if (!PRESS_SHIFT)
			{
				tcf->m_strStickName = dlg.m_strOutput;
				CString strName;
				GET_TAB_TITLE(tcf,strName);
				//
				SetTabTitle((LPCSTR)strName, FindTab(tcf));
				tcf->SetWindowText(strName);
			}
			else//fix all tab
			{
				int n = m_wndTab.GetItemCount();
				int l =n-1;
				
				for(int i=n-1;i>=0;i--)
				{
					m_wndTab.GetItem(l, &TabCtrlItem);
					tcf = ((CChildFrame*)TabCtrlItem.lParam);
					if(tcf != NULL)
					{
						tcf->m_strStickName = dlg.m_strOutput;
						CString strName;
						GET_TAB_TITLE(tcf,strName);
						//
						SetTabTitle((LPCSTR)strName, FindTab(tcf));
						tcf->SetWindowText(strName);
					}
					l--;
				}
			}
		}
	}
}

void CMainFrame::OnViewFavs() 
{
	try
	{
		
		// TODO: Add your command handler code here
		REBARBANDINFO rbbi;
		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
		int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_VIEW_FAVS);
		m_wndReBar.GetReBarCtrl().GetBandInfo(nb, &rbbi);
		
		if( rbbi.fStyle & RBBS_HIDDEN  || !m_FavBar.IsWindowVisible())
		{
			m_FavBar.ShowWindow(SW_SHOW);
			m_wndReBar.GetReBarCtrl( ).ShowBand(nb, TRUE);
			GetMenu()->CheckMenuItem(ID_VIEW_FAVS, MF_CHECKED);
			
		}
		else
		{
			m_wndReBar.GetReBarCtrl( ).ShowBand(nb,FALSE);
			m_FavBar.ShowWindow(SW_HIDE);
			GetMenu()->CheckMenuItem(ID_VIEW_FAVS, MF_UNCHECKED);
		}		
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnViewSearch() 
{
	// TODO: Add your command handler code here
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
	int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_VIEW_SEARCH);
	m_wndReBar.GetReBarCtrl().GetBandInfo(nb, &rbbi);
	
	if( rbbi.fStyle & RBBS_HIDDEN  || !m_SearchBar.IsWindowVisible())
	{
		m_SearchBar.ShowWindow(SW_SHOW);
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb, TRUE);
		GetMenu()->CheckMenuItem(ID_VIEW_SEARCH, MF_CHECKED);
		
	}
	else
	{
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb,FALSE);
		m_SearchBar.ShowWindow(SW_HIDE);
		GetMenu()->CheckMenuItem(ID_VIEW_SEARCH, MF_UNCHECKED);
	}
}

void CMainFrame::OnAddtoGroup(UINT nID)
{
	CChildFrame * tcf = NULL;
	if (g_bRTab==2)
	{
		AddtoGroup(g_strLastText, g_strLastUrl, nID); 
		return;
	}
	if(g_bRTab)
	{
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		
		m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
		tcf = (CChildFrame*)TabCtrlItem.lParam;
	}
	else
		tcf = (CChildFrame*)MDIGetActive();
	
	if(VALID_TCF(tcf))
	{
		CString strName, strURL;
		GET_TAB_TITLE(tcf,strName);
		tcf->m_pView->GetLocationURL(strURL);
		AddtoGroup(strName, strURL, nID); 
	}
}

void CMainFrame::AddtoGroup(LPCSTR pszName, LPCSTR pszURL, UINT nID)
{
	if (!_FileIsExist(theApp.m_strGroupPath))
	{
		MSGBOX(IDS_GROUP_NOT_EXIST);
		return;
	}
	CGroupUrlDlg dlg;
	dlg.m_strName = pszName;
	dlg.m_strUrl = pszURL;
	dlg.m_bNotEmpty = TRUE;
	dlg.m_bState = TRUE;
	dlg.m_dwProperty = DEFAULT_PROPERTY;
	if(dlg.DoModal()==IDOK)
	{
		CString filename;
		GetGroupNameByIndex(nID-B_ADDTO_GROUP, filename);
		filename = theApp.m_strGroupPath + filename;
		
		BOOL bAdded = _GroupAddItem(dlg.m_strName, dlg.m_strUrl, dlg.m_bState, dlg.m_dwProperty, filename);
		if (bAdded)
			InitGroups();
	}
}

void CMainFrame::OnToolsSelbg() 
{
	
	// TODO: Add your command handler code here
	try{
		CColorDialog colordlg;
		colordlg.m_cc.Flags |= CC_RGBINIT;
		colordlg.m_cc.rgbResult = m_crPageBackColor;
		if(colordlg.DoModal() == IDOK)
		{
			m_crPageBackColor = colordlg.GetColor();
			BYTE b;
			char tmp[4];
			m_strBGColor = "#";
			b = GetRValue(m_crPageBackColor);
			sprintf(tmp,"%02x",b);
			m_strBGColor += tmp;
			b = GetGValue(m_crPageBackColor);
			sprintf(tmp,"%02x",b);
			m_strBGColor += tmp;
			b = GetBValue(m_crPageBackColor);
			sprintf(tmp,"%02x",b);
			m_strBGColor += tmp;
			
			OnToolsSetbg();
		}
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnToolsSetbg() 
{
	try
	{
		
		// TODO: Add your command handler code here
		GET_ACTIVE
			
			VARIANT color;
		color.vt = VT_BSTR;
		color.bstrVal = m_strBGColor.AllocSysString();
		SetBackground(tcf->m_pView, color);
		SysFreeString(color.bstrVal);
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::SetBackground(CMyIE9View *pview, VARIANT& color)
{
	IHTMLDocument2 * pDoc = NULL;
	IHTMLElement *pElem = NULL;
	IHTMLBodyElement * pBody = NULL;
	BSTR bstrSrc = NULL;
	
	try{
		pDoc = (IHTMLDocument2*)(pview->GetHtmlDocument());
		if(pDoc!=NULL)
		{
			pElem = NULL;
			pDoc->get_body(&pElem);
			if(pElem!=NULL)
			{
				pElem->QueryInterface(&pBody);
				if(pBody!=NULL)
				{
					pBody->put_background(bstrSrc);
					pBody->put_bgColor(color);
				}
			}
		}
	}catch(...){}
	
	try{
		if(pBody != NULL)
			pBody->Release();
		if(pDoc != NULL)
			pDoc->Release();
		if(pElem != NULL)
			pElem->Release();
	}catch(...){}
}

//#pragma optimize( "s", on)
UINT _cdecl SaveSearchEngine(LPVOID con)
{
	// TODO: Add extra validation here
	int n = pmf->m_SearchList.m_Searchs.GetUpperBound();
	CString filename = theApp.m_strUser+"SearchEngine.ini";
	char key[9];
	char num[10];
	WritePrivateProfileSection("SearchEngine", NULL, filename);
	//save quick search
	CSearch *item;
	for( int i=0; i<=n; i++)
	{
		item = pmf->m_SearchList.m_Searchs.GetAt(i);
		if (item)
		{
			if (item->state)
			{
				key[0]='C';
				itoa(i, key+1, 10);
				itoa(item->state, num, 10);
				::WritePrivateProfileString("SearchEngine", key, num, filename);
			}
			key[0]='N';
			itoa(i, key+1, 10);
			::WritePrivateProfileString("SearchEngine", key, item->name, filename);
			key[0]='S';
			itoa(i, key+1, 10);
			::WritePrivateProfileString("SearchEngine", key, item->url, filename);
			if (item->icon.GetLength())
			{
				key[0]='I';
				itoa(i, key+1, 10);
				::WritePrivateProfileString("SearchEngine", key, item->icon, filename);
			}
		}
	}
	return 0;
}

void CMainFrame::OnToolsQuicksearch() 
{
	try
	{
		
		// TODO: Add your command handler code here
		CQuickSearchDlg dlg;
		if (dlg.DoModal()==IDOK)
		{
			m_SearchBar.InitImage();
			m_SearchBar.InitButton();
			CRect rect;
			m_SearchBar.GetWindowRect(&rect);
			m_SearchBar.PostMessage(WM_SIZE, 0, ((rect.Height())<<16)+rect.Width());
			//
			int bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_SEARCH);
			REBARBANDINFO rbBand;
			rbBand.cbSize = sizeof(REBARBANDINFO);
			rbBand.fMask = RBBIM_CHILDSIZE | RBBIM_ID ;
			rbBand.wID = ID_VIEW_SEARCH;
			m_wndReBar.GetReBarCtrl().GetBandInfo(bandid, &rbBand);
			if (m_SearchList.m_nIconCount)
			{
				CRect rect1;
				m_SearchBar.GetItemRect(3,&rect1);
				rbBand.cxMinChild = 150 + m_SearchList.m_nIconCount * rect1.Width();
			}
			else
				rbBand.cxMinChild = 150;
			m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbBand);
			
			AfxBeginThread(SaveSearchEngine, NULL);
		}
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnToolsSync() 
{
	// TODO: Add your command handler code here
	ShellExecute(NULL, "open", "mobsync.exe" , "", NULL, SW_SHOW);
}

//#pragma optimize( "s", off)

void CMainFrame::OnToolsCollector(BOOL bShow) 
{
	try
	{
		
		// TODO: Add your command handler code here
		if(m_pCollectDlg==NULL)
		{
			m_pCollectDlg = new CCollectorDlg();
			if(m_pCollectDlg!=NULL)
			{
				m_pCollectDlg->Create(IDD_COLLECTOR);
				CMyIE9App* app = (CMyIE9App*)AfxGetApp();
				RECT rect;
				int top = _app->GetProfileInt("Collector", "top", 300);
				int left = _app->GetProfileInt("Collector", "left", 360);
				int width = _app->GetProfileInt("Collector", "width", 400);
				int height = _app->GetProfileInt("Collector", "height", 220);
				if (top>10000) top = 100;
				if (left>10000) left = 100;
				rect.top = top;
				rect.left = left;
				rect.bottom = top+height;
				rect.right = left+width;
				m_pCollectDlg->SendMessage(WM_SIZE, 0, ((height)<<16)+width);
				m_pCollectDlg->MoveWindow(&rect);
				
				//register drag drop
				if (!m_pCollectDlg->ms_dropTarget.Register(m_pCollectDlg)) {
					TRACE("Fail in registing drop target\n");
				}
				
				//load saved contents
				char *tmp;
				tmp = _FileGetBuffer(theApp.m_strUser+"CollectorText.txt");
				m_pCollectDlg->m_txt.SetWindowText(tmp);
				FREENULL(tmp);
				tmp = _FileGetBuffer(theApp.m_strUser+"CollectorOutput.txt");
				m_pCollectDlg->m_output.SetWindowText(tmp);
				FREENULL(tmp);
				tmp = _FileGetBuffer(theApp.m_strUser+"CollectorScript.txt");
				m_pCollectDlg->m_script.SetWindowText(tmp);
				FREENULL(tmp);
				if (bShow)
					m_pCollectDlg->ShowWindow(SW_SHOW);
			}
		}
		else if(!m_pCollectDlg->IsWindowVisible())
			m_pCollectDlg->ShowWindow(SW_SHOW);
		else
			m_pCollectDlg->SendMessage(WM_CLOSE);
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnToolsCollect()
{
	OnToolsCollector(TRUE);
}

//#pragma optimize( "", on )

void CMainFrame::OnAllPageLinks() 
{
	// TODO: Add your command handler code here
	GET_ACTIVE
		
		try{
		GetAllLinks(tcf->m_pView);
		if(m_pLinkListDlg==NULL)
		{
			m_pLinkListDlg = new CLinkListDlg();
			if(m_pLinkListDlg!=NULL)
			{
				m_pLinkListDlg->Create(IDD_LINKLIST);
				CWinApp* app = AfxGetApp();
				RECT rect;
				int top = rect.top = _app->GetProfileInt("LinkList", "top", 130);
				int left = rect.left = _app->GetProfileInt("LinkList", "left", 90);
				int width = _app->GetProfileInt("LinkList", "width", 628);
				int height = _app->GetProfileInt("LinkList", "height", 380);
				if (width==-1)
					m_pLinkListDlg->ShowWindow(SW_MAXIMIZE);
				else
				{
					rect.bottom = top+height;
					rect.right = left+width;
					m_pLinkListDlg->SendMessage(WM_SIZE, 0, ((height)<<16)+width);
					m_pLinkListDlg->MoveWindow(&rect);
				}
			}
		}
		if(m_pLinkListDlg)
		{
			if (m_pLinkListDlg->IsIconic())
				m_pLinkListDlg->ShowWindow(SW_RESTORE);
			else
			{
				//m_pLinkListDlg->CenterWindow();
				m_pLinkListDlg->ShowWindow(SW_SHOW);
			}
			m_pLinkListDlg->FillLinkList();
		}
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

BOOL CMainFrame::GetAllLinks(CMyIE9View *pView)
{
	IHTMLDocument2 * pDoc = NULL;
	IHTMLElementCollection  * pAllElem = NULL;
	IHTMLElement *pElem = NULL;
	IHTMLAnchorElement * pAnchor = NULL;
	IHTMLElement* ptext;
	long p;
	BSTR bstrSrc;
	VARIANT name;
	name.vt = VT_I4;
	CLink *item;
	
	try{
		pDoc = (IHTMLDocument2*)(pView->GetHtmlDocument());
		if(pDoc!=NULL)
		{
			m_LinkList.Clean();
			pDoc->get_links(&pAllElem);
			if(pAllElem!=NULL)
			{
				pAllElem->get_length(&p);
				if(p>0)
				{
					//save all links
					for(int i=0; i<p; i++)
					{
						name.lVal = i;
						if(pAllElem->item(name, name, (LPDISPATCH*)&pElem)==S_OK)
						{
							if(pElem != NULL)
							{
								pElem->QueryInterface(&pAnchor);
								if(pAnchor!=NULL)
								{
									pAnchor->get_href(&bstrSrc);
									CString s1(bstrSrc);
									SysFreeString(bstrSrc);
									
									pAnchor->QueryInterface(&ptext);
									ptext->get_innerText(&bstrSrc);
									CString s(bstrSrc);
									SysFreeString(bstrSrc);
									ptext->Release();
									s.TrimLeft();s.TrimRight();
									s.Replace("\n", "\\n");
									s.Replace("\r", "\\r");
									s.Replace("\"", "\\\"");
									s.Replace("\'", "\\\'");
									//
									item = new CLink;
									item->name = s;
									item->url = s1;
									m_LinkList.m_Links.Add(item);
									//
									pAnchor->Release();
									pAnchor = NULL;
								}
								pElem->Release();
								pElem = NULL;
							}
						}
					}
				}
			}
		}
	}catch(...){}
	
	try{
		if(pDoc != NULL)
			pDoc->Release();
		if(pAllElem != NULL)
			pAllElem->Release();
	}catch(...){}
	
	return TRUE;
}

void CMainFrame::OnToolsAllBk() 
{
	// TODO: Add your command handler code here
	VARIANT color;
	color.vt = VT_BSTR;
	color.bstrVal = m_strBGColor.AllocSysString();
	
	CChildFrame* tcf;
	UINT n = m_wndTab.GetItemCount();
	m_nWin = n;
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	
	for(UINT i=0;i<n;i++)
	{
		tcf = NULL;
		TabCtrlItem.lParam = NULL;
		m_wndTab.GetItem(i, &TabCtrlItem);
		tcf = ((CChildFrame*)TabCtrlItem.lParam);
		if(VALID_TCF(tcf))
		{
			SetBackground(tcf->m_pView, color);
		}
	}
	SysFreeString(color.bstrVal);
}

void CMainFrame::OnFileCloseall() 
{
	// TODO: Add your command handler code here
	try	{
		int n = m_wndTab.GetItemCount();
		if (n<1)
			return;
		if(g_bConfirmCloseAll)
		{
			if(MSGBOX(IDS_CLOSE_ALL_CONF, MB_YESNO|MB_ICONQUESTION)==IDNO)
				return;
		}
		//
		CChildFrame* tcf;
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		int l =n-1;
		for(int i=n-1;i>=0;i--)
		{
			m_wndTab.GetItem(l, &TabCtrlItem);
			tcf = ((CChildFrame*)TabCtrlItem.lParam);
			CLOSE_PAGE(tcf);
			l--;
		}
	}catch(...){}
}

void CMainFrame::SetTabTitle(LPCSTR lpszTitle, int nItem)
{
	//set the new title of tab
	TCITEM TabCtrlItem;
	if(lpszTitle==NULL)
		return;
	
	try
	{
		static char temp[256];
		try{
			TabCtrlItem.mask = TCIF_TEXT;
			if(strlen(lpszTitle)>m_nTitleLen)
			{
				STRNCPY(temp, lpszTitle, m_nTitleLen);
				TabCtrlItem.pszText = temp;
			}
			else
			{
				strcpy(temp, lpszTitle);
				TabCtrlItem.pszText = temp;
			}
			
			m_wndTab.SetItem( nItem, &TabCtrlItem );
			m_wndTab.GetToolTips()->UpdateTipText(lpszTitle, &m_wndTab, nItem);
		}catch(...){}
	}catch(...){}
}

int CMainFrame::FindTab(CChildFrame *lpFrame, BOOL bCleanBlank)
{
	int n;
	BOOL found;
	register int i=0;
	TCITEM TabCtrlItem;
	
	try{
		i=0; 
		n = m_wndTab.GetItemCount();
		found = false;
		TabCtrlItem.mask = TCIF_PARAM;
		while (i<n && !found)
		{
			TabCtrlItem.lParam = 0;
			m_wndTab.GetItem(i, &TabCtrlItem);
			if(TabCtrlItem.lParam == (LPARAM)lpFrame)
			{	
				found = true;				
			}
			else
				i++;
		}
	}catch(...){}
	
	if(!found)
		i=-2;
	
	return i;
}

BOOL CMainFrame::IsDownloading()
{
	// TODO: Add your command handler code here
	CChildFrame* tcf;
	int n = m_wndTab.GetItemCount();
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	int l =n-1;
	
	try{
		for(int i=n-1;i>=0;i--)
		{
			m_wndTab.GetItem(l, &TabCtrlItem);
			tcf = ((CChildFrame*)TabCtrlItem.lParam);
			if(VALID_TCF(tcf))
			{
				if (tcf->m_pView->GetBusy())//if (tcf->m_pView->m_nProgress>0)
					return TRUE;
			}
			l--;
		}
	}catch(...)	{}
	
	return FALSE;
}

BOOL CALLBACK TerminateAppEnum( HWND hwnd, LPARAM lParam )
{
	DWORD dwID ;
	
	// If we can't open the process with PROCESS_TERMINATE rights,
	// then we give up immediately.
	GetWindowThreadProcessId(hwnd, &dwID) ;
	
	if(dwID == (DWORD)lParam)
		PostMessage(hwnd, WM_CLOSE, 0, 0) ;
	
	return TRUE ;
}

DWORD WINAPI TerminateApp( DWORD dwPID, DWORD dwTimeout )
{
	DWORD   dwRet = 0;	
	HANDLE hProc = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, FALSE,
		dwPID);
	if(hProc == NULL)
	{
		return 1 ;
	}
	// TerminateAppEnum() posts WM_CLOSE to all windows whose PID
	// matches your process's.
	EnumWindows((WNDENUMPROC)TerminateAppEnum, (LPARAM) dwPID) ;
	
	// Wait on the handle. If it signals, great. If it times out,
	// then you kill it.
	/*if(WaitForSingleObject(hProc, dwTimeout)!=WAIT_OBJECT_0)
	dwRet=(TerminateProcess(hProc,0)?TA_SUCCESS_KILL:TA_FAILED);
	else
	dwRet = TA_SUCCESS_CLEAN ;
	*/
	CloseHandle(hProc) ;
	
	return dwRet ;
}

void CMainFrame::OnExitApp()
{
	try
	{
		
		
		DoClose();
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnClose()
{
	try
	{
		
		
		if((!g_bTopCloseExitApp) && (m_wndTab.GetItemCount() > 0) )
		{
			OnFileClose();
			return;
		}
		else
		{
			DoClose();
		}
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::DoClose() 
{
	// TODO: Add your message handler code here and/or call default
	try
	{
		if(g_bConfirmExit && !g_bExitAtOnce)
		{
			if(MSGBOX(IDS_CONFIRM_EXIT, MB_YESNO|MB_ICONQUESTION)==IDNO)
				return;
		}
		
		//some page are downloading
		if (g_bConfirmExitDowning)
		{
			//confirm ie download dlg
			HWND hWnd;
			DWORD dwProcessId = 0;
			DWORD dwThreadId = 0;
			DWORD dwCurrProcessId = ::GetCurrentProcessId();
			DWORD dwCurrThreadId  = ::GetCurrentThreadId();
			for (	hWnd = ::FindWindowEx(NULL, NULL, "#32770",NULL);
			hWnd != NULL;
			hWnd = ::FindWindowEx(NULL, hWnd, "#32770",NULL)
				)
			{	
				dwThreadId = ::GetWindowThreadProcessId(hWnd, &dwProcessId);
				if ( dwCurrProcessId == dwProcessId && dwCurrThreadId != dwThreadId )
				{
					if(MSGBOX(IDS_CONFIRM_IE_DOWN, MB_YESNO|MB_ICONQUESTION)==IDNO)
						return;
				}
			}

			if (IsDownloading())
			{
				if(MSGBOX(IDS_CONFIRM_DOWNLOADING, MB_YESNO|MB_ICONQUESTION)==IDNO)
					return;
			}
		}
		
		DestroyIcon(m_hNormalIcon);
		DestroyIcon(m_hSecuIcon);
		if(ms_TabDropTarget!=NULL)
			delete ms_TabDropTarget;
		
		CWinApp* app = AfxGetApp();
		CRect wrect;
		if(m_pCollectDlg!=NULL)
		{
			m_pCollectDlg->GetWindowRect(&wrect);
			app->WriteProfileInt("Collector", "top", wrect.top);
			app->WriteProfileInt("Collector", "left", wrect.left);
			app->WriteProfileInt("Collector", "width", wrect.Width());
			app->WriteProfileInt("Collector", "height", wrect.Height());
			
			m_pCollectDlg->DestroyWindow();
			delete m_pCollectDlg;
		}
		if(m_pLinkListDlg!=NULL)
		{
			if (m_pLinkListDlg->IsZoomed())
				app->WriteProfileInt("LinkList", "width", -1);
			else
			{
				m_pLinkListDlg->GetWindowRect(&wrect);
				app->WriteProfileInt("LinkList", "top", wrect.top);
				app->WriteProfileInt("LinkList", "left", wrect.left);
				app->WriteProfileInt("LinkList", "width", wrect.Width());
				app->WriteProfileInt("LinkList", "height", wrect.Height());
			}
			m_pLinkListDlg->DestroyWindow();
			delete m_pLinkListDlg;
		}
		if(m_pMonitor)
		{
			app->WriteProfileInt("Monitor", "ShowMonitor", IsMonitorVisible());
			m_pMonitor->GetWindowRect(&wrect);
			app->WriteProfileInt("Monitor", "top", wrect.top);
			app->WriteProfileInt("Monitor", "left", wrect.left);
			m_pMonitor->DestroyWindow();
			delete m_pMonitor;
		}
		
		//kill update timer
		if(m_nUpdateTimer != 0)
			KillTimer(m_nUpdateTimer);
		m_nUpdateTimer = 0;
		if(m_nAnimateIconTimer)
			KillTimer(m_nAnimateIconTimer);
		m_nAnimateIconTimer=0;
		if(m_hResInst != NULL)
			FreeLibrary(m_hResInst);
		m_hResInst = NULL;
		
		if(app != NULL)
		{
			app->WriteProfileInt("Settings", "DefaultProperty", m_dwDefaultProperty);

			app->WriteProfileInt("Settings", "AutoHide", m_bAutoHideMode);
			if(m_bAutoHideMode)
				OnViewAutoHide();
			//save full screen
			app->WriteProfileInt("Settings", "FullScreen", m_bFullScreenMode);
			if(m_bFullScreenMode)
				OnViewFullscreen();
			
			//window position
			CRect wrect;
			if(IsZoomed())
				app->WriteProfileInt("Settings", "Zoomed", TRUE);
			else if(!IsIconic())
			{
				app->WriteProfileInt("Settings", "Zoomed", FALSE);
				
				GetWindowRect(&wrect);
				app->WriteProfileInt("Settings", "top", wrect.top);
				app->WriteProfileInt("Settings", "left", wrect.left);
				app->WriteProfileInt("Settings", "width", wrect.Width());
				app->WriteProfileInt("Settings", "Height", wrect.Height());
			}
			//erase running flag
			app->WriteProfileInt("Settings", "Running", 0);
			app->WriteProfileInt("Settings", "EnableProxy", m_bEnProxy);
			WriteProfileReBarBandsState();
			//bottom bar
			UINT i;
			REBARBANDINFO rbbi;
			char temp2[8]="bandb";
			rbbi.cbSize = sizeof(rbbi);
			rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
			for(i=0; i< m_wndReBar2.GetReBarCtrl().GetBandCount(); i++)
			{
				itoa(i, temp2+5, 10);
				m_wndReBar2.GetReBarCtrl().GetBandInfo(i, &rbbi);
				//show hide bands
				if(rbbi.fStyle & RBBS_HIDDEN )
					app->WriteProfileInt("Bands", temp2, 0);
				else
					app->WriteProfileInt("Bands", temp2,1);
			}
			//status bar
			if(IsWindowVisible())//if (!bHidden)
				app->WriteProfileInt("Bands", "Status", m_wndStatusBar.IsWindowVisible());
			//
			//ShowWindow(SW_MINIMIZE);
			ShowWindow(SW_HIDE);
			UpdateWindow();
			//
			UnregisterHotKey (m_hWnd, 0);
			DelTrayIcon();
		}
	}catch(...){}

	
	GF_TRY{		
		DoCloseSave();
	}GF_CATCH

	GF_TRY{
		CloseExternalUtil();
		UninitialNameSpace();
		ClearSpeedMonitor();
	}GF_CATCH
	
		
	try
	{
		CMDIFrameWnd::OnClose();
		pmf = NULL;
	}catch(...){ASSERT(false);}
}

void CMainFrame::OnViewSource() 
{
	// TODO: Add your command handler code here
	GET_ACTIVE
		ExecCmdTarget(HTMLID_VIEWSOURCE, tcf->m_pView);
}

void CMainFrame::OnFileClose() 
{
	// TODO: Add your command handler code here
	GET_ACTIVE
		tcf->m_pView->m_bToClose = TRUE;
	tcf->PostMessage(WM_CLOSE);
}

void CMainFrame::OnFileClose2() 
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;	
	try
	{
		m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
		if(TabCtrlItem.lParam != NULL)
		{
			CChildFrame* tcf = ((CChildFrame*)TabCtrlItem.lParam);
			//CLOSE_PAGE(tcf)
			tcf->m_pView->m_bToClose = TRUE;
			tcf->PostMessage(WM_CLOSE);
		}
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

//Used to Execute Find, View Source and Internet Option commands.
HRESULT CMainFrame::ExecCmdTarget(DWORD nCmdID, CFixedHtmlView* pHVW)
{
	LPDISPATCH lpDispatch = NULL;
    LPOLECOMMANDTARGET lpOleCommandTarget = NULL;
	
	if(pHVW == NULL)
		return S_OK;
	try
	{
		lpDispatch = pHVW->GetHtmlDocument();
		if(lpDispatch == NULL)
			return S_OK;
		ASSERT(lpDispatch);
		
		// Get an IDispatch pointer for the IOleCommandTarget interface.
		lpDispatch->QueryInterface(IID_IOleCommandTarget, (void**)&lpOleCommandTarget);
		ASSERT(lpOleCommandTarget);
		
		lpDispatch->Release();
		lpDispatch = NULL;
		
		// Invoke the given command id for the WebBrowser control
		if(lpOleCommandTarget != NULL)
		{
			HRESULT hr = lpOleCommandTarget->Exec(&CGID_IWebBrowser, nCmdID, 0,
				NULL, NULL);
			lpOleCommandTarget->Release();
			lpOleCommandTarget = NULL;
			return hr;
		}
		else
			return S_OK;
	}catch(...){}
	
	return S_OK;
}

void CMainFrame::OnSetAsDefaultBrowser()
{
	SetAsDefaultBrowser();
}

void CMainFrame::OnToolsInternetoptions() 
{
	// TODO: Add your command handler code here
	GET_ACTIVE
		ExecCmdTarget(HTMLID_OPTIONS, tcf->m_pView);
	
	m_strHomePage = GetHomePage();
	GetIeProxy();
	//
	m_wndAddress->GetEditCtrl()->Invalidate();
	m_wndSearch->GetEditCtrl()->Invalidate();
}

void CMainFrame::OnFavoritesAddtofavorites() 
{
	try
	{
		
		// TODO: Add your command handler code here
		GET_ACTIVE
			AddFavorite(tcf);
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}


//import/export
void CMainFrame::OnFavoritesExportfavorites() 
{
	if (m_wndTab.GetItemCount()<=0)
	{
		OnFileNewBlank();
	}

	GET_ACTIVE
	tcf->m_pView->OnFileImportExport();

	m_bFavDirty = TRUE;
// 
// 	// TODO: Add your command handler code here
// 	IShellUIHelper* pShellUIHelper=NULL;
// 	HRESULT hr = CoCreateInstance(CLSID_ShellUIHelper, NULL, 
// 		CLSCTX_INPROC_SERVER, IID_IShellUIHelper, 
// 		(LPVOID*)&pShellUIHelper);
// 	if (SUCCEEDED(hr) && pShellUIHelper!=NULL)
// 	{
// 		try
// 		{  
// 			CString strPath = "";
// 			GetFavoriteFolder(strPath);
// 			pShellUIHelper->ImportExportFavorites( VARIANT_FALSE, strPath.AllocSysString());
// 	   }catch(...){}
// 	   pShellUIHelper->Release();
// 	}	
}

//open dir
void CMainFrame::OnFavoritesImportfavorites() 
{

	CString strPath = "";
	GetFavoriteFolder(strPath);
	CGf_Win::OpenNormal(strPath);

	//xp2 下被微软废弃了
// 	// TODO: Add your command handler code here
// 	IShellUIHelper* pShellUIHelper = NULL;
// 	HRESULT hr = CoCreateInstance(CLSID_ShellUIHelper, NULL, 
// 		CLSCTX_INPROC_SERVER, IID_IShellUIHelper, 
// 		(LPVOID*)&pShellUIHelper);
// 	if (SUCCEEDED(hr) && pShellUIHelper!=NULL)
// 	{
// 		try	
// 		{
// 			CString strPath = "";
// 			GetFavoriteFolder(strPath);
// 			pShellUIHelper->ImportExportFavorites( VARIANT_TRUE, strPath.AllocSysString());
// 	   } catch(...) {ASSERT(false); }
// 	   pShellUIHelper->Release();
// 	   //rebuild favorite menu
// 	   m_bFavDirty = TRUE;
// 	}
}

typedef UINT (CALLBACK* LPFNORGFAV)(HWND, LPTSTR);
void CMainFrame::OnFavoritesOrgnizefavorites() 
{
	try
	{
		HMODULE hMod = NULL;

		if (CRegHelper::IsIE8()  )
		{
			hMod = (HMODULE)LoadLibrary("ieframe.dll");
		}
		else
		{
			hMod = (HMODULE)LoadLibrary("shdocvw.dll");
		}
		
		
		if (hMod)
		{
			LPFNORGFAV lpfnDoOrganizeFavDlg = (LPFNORGFAV)GetProcAddress(hMod, 
				"DoOrganizeFavDlg");
			if (lpfnDoOrganizeFavDlg)
			{
				CString           szPath;
				
				GetFavoriteFolder(szPath);
				lpfnDoOrganizeFavDlg(m_hWnd, (LPSTR)(LPCTSTR)szPath);
				
				//rebuild favorites menu
				m_bFavDirty = TRUE;
			}
			FreeLibrary(hMod);
		}
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::InitFavorites(BOOL bOnlyLink)
{
	// set up Favorites menu
	static TCHAR	sz[MAX_PATH];
	CString         szPath, link;
	CMenu*          pMenu = NULL;
	
	// first get rid of bogus submenu items.
	try{
		pMenu = SMP_FAV;
		if(pMenu == NULL)
			return;
		DestroyMenuData(pMenu->m_hMenu,FAV_START);
		while(pMenu->DeleteMenu(FAV_START, MF_BYPOSITION));	
		// find out from the registry where the favorites are located.
		if(!GetFavoriteFolder(szPath))
			return ;
		
		//link bar
		{
			REBARBANDINFO rbbi;
			int bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_LINKS);
			rbbi.cbSize = sizeof(rbbi);
			rbbi.fMask = RBBIM_SIZE;//|RBBIM_STYLE;
			m_wndReBar.GetReBarCtrl().GetBandInfo(bandid, &rbbi);
			int b = m_LinkBar.GetToolBarCtrl().GetButtonCount();
			for(int i = 0; i<b;i++)
				m_LinkBar.GetToolBarCtrl().DeleteButton(0);
			rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE |RBBIM_ID;// |RBBIM_STYLE ;
			rbbi.wID = ID_VIEW_LINKS;
			rbbi.cxMinChild = 0;//2;
			rbbi.cyMinChild = 22;
			/*rbbi.cx =*/ rbbi.cxIdeal = 2;
			m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
		}
		
		//fav bar
		{
			REBARBANDINFO rbbi;
			int bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_FAVS);
			
			rbbi.cbSize = sizeof(rbbi);
			rbbi.fMask = RBBIM_SIZE;
			m_wndReBar.GetReBarCtrl().GetBandInfo(bandid, &rbbi);
			//remove all Fav buttons
			int b = m_FavBar.GetToolBarCtrl().GetButtonCount();
			for(int i = 0; i<b;i++)
				m_FavBar.GetToolBarCtrl().DeleteButton(0);
			rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE |RBBIM_ID ;
			rbbi.wID = ID_VIEW_FAVS;
			rbbi.cxMinChild = 0;//2;
			rbbi.cyMinChild = 22;
			/*rbbi.cx =*/ rbbi.cxIdeal = 2;
			m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
		}
		
		if (m_bUseDefaultLinkPath)
		{
			
			link = CGfAppString::GetString(IDS_QUICK_LINKS_FOLDER_NAME); //gflb
			
			BOOL foundlink = FALSE;
			if(!link.IsEmpty())
			{
				strcpy(sz, (LPCSTR)szPath);
				if(szPath.Right(1) != "\\")
					strcat(sz, "\\");
				strcat(sz, (LPCSTR)link);
				strcat(sz, "\\");
				if(CPathHelper::IsExist(sz)) //gf SetCurrentDirectory
				{
					foundlink = TRUE;
					m_strLinkPath = sz;
				}
			}
			
			if ( (!CGf_AppStatic::_instance._bStartFirstTime)&&(!foundlink) )
			{
				
				strcpy(sz, (LPCSTR)szPath);
				if(szPath.Right(1) != "\\")
					strcat(sz, "\\");
				
				CString strOldLink = CGfAppString::GetString(IDS_OLD_QUICK_LINKS_FOLDER_NAME); //gflb
				
				strcat(sz, (LPCSTR)strOldLink);
				strcat(sz, "\\");
				if(CPathHelper::IsExist(sz)) //gf SetCurrentDirectory
				{
					foundlink = TRUE;
					m_strLinkPath = sz;
				}
				
			}
			
			//gflb
			if ( CGf_AppStatic::_instance._bStartFirstTime&&(!foundlink) )
			{
				CPathHelper::CreateFullDir(sz);
				
				if (CPathHelper::IsExist(sz) )
				{
					foundlink = TRUE;
					CPathHelper::CopyDirectory(CGfPathManager::GetTempleLinksFolder(), sz);
				}
				
				m_strLinkPath = sz;
			}
			
			if(FALSE && (!foundlink) ) //gf
			{
				//if last isn't \\, must add it
				if(szPath.Right(1) != "\\")
					szPath += "\\";
				strcpy(sz, (LPCSTR)szPath);
				strcat(sz, "链接\\");
				if(!CPathHelper::IsExist(sz))
				{
					strcpy(sz, (LPCSTR)szPath);
					strcat(sz, "Links\\");
					if(!CPathHelper::IsExist(sz))
					{
						strcpy(sz, (LPCSTR)szPath);
						strcat(sz, "硈挡\\");
						if(!CPathHelper::IsExist(sz))
						{
							strcpy(sz, (LPCSTR)szPath);
							strcat(sz, "links\\");
							if(!CPathHelper::IsExist(sz))
							{
								strcpy(sz, (LPCSTR)szPath);
								strcat(sz, "連結\\");
								if(!CPathHelper::IsExist(sz))
								{
									strcpy(sz,"");
								}
							}
						}
					}
				}
			}
			m_strLinkPath = sz;
		}	


		m_astrFavoriteFolders.RemoveAll(); 
		m_nFavSize=0;
		g_astrLinks.RemoveAll();
		g_astrFavs.RemoveAll(); 
		
		{
			int nFavs, fs;
			//gf m_nFavSize = BuildFavoritesMenu(szPath,0 , pMenu, FAV_START, TRUE, nFavs);
			m_nFavSize = BuildFavoritesMenu(bOnlyLink?m_strLinkPath:szPath,0 , pMenu, FAV_START, TRUE, nFavs);
			
			if(m_bDisplayOpenAll || m_bDisplayAddFav)
			{
				if(nFavs || m_bDisplayAddFav)
					MenuInsertSeparator(pMenu,FAV_START);//pMenu->InsertMenu(nStart,MF_BYPOSITION | MF_SEPARATOR);
				fs=m_astrFavoriteFolders.GetCount();
				m_astrFavoriteFolders.AddTail(szPath+"\\");
			}
			if(m_bDisplayOpenAll)
			{
				//add open all
				CString strOpenAll;//, strOpenMost;
				LOADSTR(strOpenAll ,IDS_OPEN_ALL_FAV);
				if(nFavs)
					MenuInserOwnerItem(pMenu,FAV_START,MF_BYPOSITION| MF_STRING ,B_OPEN_FAV+fs, strOpenAll,52);
			}
			if(m_bDisplayAddFav)
			{
				//add fav
				CString strAddFav;
				LOADSTR(strAddFav ,IDS_ADDFAV);
				MenuInserOwnerItem(pMenu,FAV_START,MF_BYPOSITION| MF_STRING ,B_ADDTO_FAV+fs, strAddFav,51);
			}
		}
	}catch(...){}
}

//Get pointer to text SK
const char *lpstrText = "Texthgp";//SK: we use const to prevent misuse
void CMainFrame::AddMenuBreak(CMenu *pMenu, BOOL bElse) //BOOL bFavMenu)
{
    CFont m_fontMenu;
    LOGFONT m_lf;
    ZeroMemory ((PVOID) &m_lf,sizeof (LOGFONT));
    NONCLIENTMETRICS nm;
    nm.cbSize = sizeof (NONCLIENTMETRICS);
    VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		nm.cbSize,&nm,0)); 
    m_lf =  nm.lfMenuFont;
    m_fontMenu.CreateFontIndirect (&m_lf);
	
    // Obtain the width of the text:
    CDC *pDC = GetDC();              // Get device context
    CFont* pFont=NULL;    // Select menu font in...
    pFont = pDC->SelectObject (&m_fontMenu);// Select menu font in...
	
    SIZE ssize;
	::GetTextExtentPoint32(pDC->m_hDC,lpstrText,
		strlen(lpstrText),&ssize);
    CSize t = CSize(ssize);
    pDC->SelectObject (pFont);  // Select old font in
    ReleaseDC(pDC);  // Release the DC
	
    // Set width and height:
	int temp = t.cy+4;
    temp = temp>16+4 ? temp : 16+4;
    m_fontMenu.DeleteObject();
	int ms = t.cy+6; //GetSystemMetrics(SM_CYMENU);
	int mc = pMenu->GetMenuItemCount(); 
	ms = temp;
	mc += bElse;//3;//add the open all menu
	
	int size = ms*mc;
	int scr = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYCAPTION)*4;
	int start = 1;
	CString strMenu;
	while(size > scr)
	{
		//need a break bar
		int i = scr/ms + start;
		//if(bFavMenu)
		if (start==1)//only first col must -bElse
			i -= bElse;//3; //conside the open all menu
		MENUITEMINFO info;
		info.cch =0;
		info.dwTypeData = 0;
		info.cbSize = sizeof (MENUITEMINFO); // must fill up this field
		info.fMask = MIIM_DATA|MIIM_TYPE|MIIM_ID|MIIM_SUBMENU;             // get the state of the menu item
		pMenu->GetMenuItemInfo(i, &info, TRUE);
		pMenu->GetMenuString(i, strMenu, MF_BYPOSITION);
		if(info.fType & MFT_OWNERDRAW)
			if(IsMenu(info.hSubMenu))
				pMenu->ModifyMenu(i, MF_BYPOSITION | MF_MENUBARBREAK| MF_POPUP|MF_OWNERDRAW, (UINT)info.hSubMenu, (char*)info.dwItemData);
			else
				pMenu->ModifyMenu(i, MF_BYPOSITION | MF_MENUBARBREAK|MF_OWNERDRAW , info.wID, (char*)info.dwItemData);
			else
				if(IsMenu(info.hSubMenu))
					pMenu->ModifyMenu(i, MF_BYPOSITION | MF_MENUBARBREAK| MF_POPUP|MF_STRING, (UINT)info.hSubMenu, strMenu);
				else
					pMenu->ModifyMenu(i, MF_BYPOSITION | MF_MENUBARBREAK|MF_STRING , info.wID, strMenu);
				
				size = (mc-i)*ms;
				start = i+1;
	} 
}

void CMainFrame::OnUpdateFileWorkoffline(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DWORD dwRet = _RegGetDword(HKEY_CURRENT_USER,
		"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings",
		"GlobalUserOffline"
		);
	if(pCmdUI->m_pOther && pCmdUI->m_pOther==&m_wndToolBar)
		ToolBarSetCheck(pCmdUI, dwRet!=0);
	else
		pCmdUI->SetCheck(dwRet!=0);
	
	m_bIsOffline = dwRet ;
}

void CMainFrame::OnUpdateMenu(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_nWin==0 ? FALSE : TRUE);	
}

void CMainFrame::OnUpdateOptionsUseProxy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ToolBarSetCheck(pCmdUI, m_bEnProxy);
}

void CMainFrame::OnOptionsUseProxy() 
{
	// TODO: Add your command handler code here
	m_bEnProxy = !m_bEnProxy;
	UpdateProxy();
}

void CMainFrame::OnOptionsSetProxy() 
{
	// TODO: Add your command handler code here
	CSetProxy dlg;
	
	try{
		dlg.m_bUseProxy = m_bEnProxy;
		dlg.m_nSelProxyList = PROXYID;
		if (m_strCurProxyName != g_strIeProxy)
		{
			dlg.m_SelProxy = m_strCurProxy;
		}	
		CChildFrame* tcf = (CChildFrame*)MDIGetActive();
		CMyIE9View* pvw;
		if(tcf!=NULL)
		{
			pvw = tcf->m_pView;
			dlg.m_bUseWebProxy = pvw->m_bEnWebProxy;
			dlg.m_SelWebProxy = pvw->m_strWebProxy;
		}
		if (dlg.m_SelWebProxy.IsEmpty())
		{
			dlg.m_SelWebProxy = m_strDefaultWebProxy;
		}	
		if(dlg.DoModal()==IDOK)
		{
			m_bSaveConfig = TRUE;
			g_bProxyDirty = TRUE;		
			//web proxy
			dlg.m_SelWebProxy.TrimLeft();
			dlg.m_SelWebProxy.TrimRight();
			if(tcf!=NULL)
			{
				pvw->m_bEnWebProxy = dlg.m_bUseWebProxy;
				pvw->m_strWebProxy = dlg.m_SelWebProxy;
			}
			if(dlg.m_SelWebProxy.GetLength())
			{
				m_strDefaultWebProxy = dlg.m_SelWebProxy;
			}
			else if(tcf)
			{
				pvw->m_bEnWebProxy = FALSE;
			}
			//proxy
			if (m_strCurProxyName == g_strIeProxy)
			{
				if (m_bEnProxy != dlg.m_bUseProxy)
				{
					m_bEnProxy = dlg.m_bUseProxy;
					UpdateProxy();
				}
				return;
			}
			//else
			m_bEnProxy = dlg.m_bUseProxy;
			//
			dlg.m_SelProxy.MakeLower();
			dlg.m_SelProxy.TrimLeft();
			dlg.m_SelProxy.TrimRight();
			m_strCurProxy = dlg.m_SelProxy;
			m_strCurProxyName = GetProxyName(m_strCurProxy);
			m_strProxyByPass = m_aProxyCategories.m_ProxyCategory.GetAt(PROXYID)->m_strProxyByPass;
			if(m_strProxyByPass.Find("<local>")<0)
			{
				m_strProxyByPass += " <local>";
			}
			UpdateProxy();
		}
	}catch(...){}
}

void CMainFrame::OnEncoding(UINT nID)
{
	GET_ACTIVE
		
		try{
		LPDISPATCH pDisp = NULL;
		pDisp = tcf->m_pView->GetHtmlDocument();
		IHTMLDocument2 *pHTMLDoc;
		if( pDisp )
		{
			if (SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
			{
				BSTR bstr=NULL;
				switch (nID)
				{
				case ID_VIEW_ENCODING_WESTERNEUROPEAN:
					bstr = SysAllocString(L"iso-8859-1");break;
				case ID_MENUITEM33080:
					bstr = SysAllocString(L"utf-8");break;
				case ID_VIEW_ENCODING_CHINESESIMPLIFIED:
					bstr = SysAllocString(L"gb2312");break;
				case ID_MENUITEM33091:
					bstr = SysAllocString(L"HZ-GB-2312");break;
				case ID_VIEW_ENCODING_CHINESETRADITIONAL:
					bstr = SysAllocString(L"big5");break;
				case ID_VIEW_ENCODING_JANESE:
					bstr = SysAllocString(L"iso-2022-jp");break;
				case ID_VIEW_ENCODING_JANESEEUC:
					bstr = SysAllocString(L"euc-jp");break;
				case ID_VIEW_ENCODING_JANESEAUTO:
					bstr = SysAllocString(L"_autodetect");break;
				case ID_VIEW_ENCODING_KOREAN_AUTO:
					bstr = SysAllocString(L"_autodetect_kr");break;
				case ID_VIEW_ENCODING_KOREAN_EUC:
					bstr = SysAllocString(L"euc-kr");break;
				case ID_VIEW_ENCODING_KOREAN_ISO:
					bstr = SysAllocString(L"iso-2022-kr");break;
				case ID_MENUITEM33065:
					bstr = SysAllocString(L"ibm852");break;
				case ID_MENUITEM33066:
					bstr = SysAllocString(L"ibm866");break;
				case ID_MENUITEM33067:
					bstr = SysAllocString(L"iso-8859-1");break;
				case ID_MENUITEM33068:
					bstr = SysAllocString(L"iso-8859-1");break;
				case ID_MENUITEM33069:
					bstr = SysAllocString(L"iso-8859-2");break;
				case ID_MENUITEM33070:
					bstr = SysAllocString(L"iso-8859-3");break;
				case ID_MENUITEM33071:
					bstr = SysAllocString(L"iso-8859-4");break;
				case ID_MENUITEM33072:
					bstr = SysAllocString(L"iso-8859-5");break;
				case ID_MENUITEM33073:
					bstr = SysAllocString(L"iso-8859-6");break;
				case ID_MENUITEM33074:
					bstr = SysAllocString(L"iso-8859-7");break;
				case ID_MENUITEM33075:
					bstr = SysAllocString(L"iso-8859-8");break;
				case ID_MENUITEM33076:
					bstr = SysAllocString(L"koi8-r");break;
				case ID_MENUITEM33077:
					bstr = SysAllocString(L"unicode");break;
				case ID_MENUITEM33078:
					bstr = SysAllocString(L"unicodeFEFF");break;
				case ID_MENUITEM33079:
					bstr = SysAllocString(L"utf-7");break;
				case ID_MENUITEM33081:
					bstr = SysAllocString(L"windows-1250");break;
				case ID_MENUITEM33082:
					bstr = SysAllocString(L"windows-1251");break;
				case ID_MENUITEM33083:
					bstr = SysAllocString(L"windows-1253");break;
				case ID_MENUITEM33084:
					bstr = SysAllocString(L"windows-1254");break;
				case ID_MENUITEM33085:
					bstr = SysAllocString(L"windows-1255");break;
				case ID_MENUITEM33086:
					bstr = SysAllocString(L"windows-1256");break;
				case ID_MENUITEM33087:
					bstr = SysAllocString(L"windows-1257");break;
				case ID_MENUITEM33088:
					bstr = SysAllocString(L"windows-1258");break;
				case ID_MENUITEM33089:
					bstr = SysAllocString(L"windows-874");break;
				case ID_MENUITEM33090:
					bstr = SysAllocString(L"x-user-defined");break;
				}
				if (bstr)
				{
					pHTMLDoc->put_charset( bstr );
					SysFreeString(bstr);
					tcf->m_pView->Refresh2(1);
				}
				pHTMLDoc->Release();
				//
				GetMenu()->CheckMenuItem(m_nDefaultCharSet, MF_UNCHECKED);
				m_nDefaultCharSet = nID;
				GetMenu()->CheckMenuItem(m_nDefaultCharSet, MF_CHECKED);
			}
			pDisp->Release();
		}
	}catch(...){}
}

void CMainFrame::BuildExpBar(LPCSTR lpszTitle)
{
	try{
		m_wndInstantBar.SetVertWidth(m_nExpWidth);
		if (!m_wndInstantBar.Create(_T(lpszTitle), this, 127))
		{
			TRACE0("Failed to create instant bar\n");
			return ;		// fail to create
		}
		m_wndInstantBar.SetSCBStyle(m_wndInstantBar.GetSCBStyle() |
			SCBS_SIZECHILD);
		m_wndInstantBar.SetBarStyle(m_wndInstantBar.GetBarStyle() |
			CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		m_wndInstantBar.EnableDocking(CBRS_ALIGN_ANY);
		EnableDocking(CBRS_ALIGN_ANY);
		DockControlBar(&m_wndInstantBar, AFX_IDW_DOCKBAR_LEFT);
		
#ifdef _SCB_REPLACE_MINIFRAME
		//m_pFloatingFrameClass = RUNTIME_CLASS(CSCBMiniDockFrameWnd);
#endif //_SCB_REPLACE_MINIFRAME
		
		ShowControlBar(&m_wndInstantBar, FALSE, FALSE);
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnViewHis() 
{
	// TODO: Add your command handler code here
	CString str,str1;
	
	try{
		if(m_wndInstantBar.GetSafeHwnd()==NULL)
			BuildExpBar("");
		BOOL bNeedInI=false;
		if(m_pwndExpObj == NULL)
			bNeedInI = true;
		else
			LOADSTR(str ,IDS_TITLE_HIS);
		m_wndInstantBar.GetWindowText(str1);
		if(bNeedInI || str1!=str)
		{
			RemoveExp();
			try
			{
				m_pwndExpObj = new CHistoryTree();
			}
			catch(...)
			{
				m_pwndExpObj = NULL;
				return;
			}
			m_pwndExpObj->Create(&m_wndInstantBar);
			str = m_pwndExpObj->GetTitle();
			CRect rect;
			m_wndInstantBar.GetClientRect(&rect);
			m_wndInstantBar.SendMessage(WM_SIZE, 0, ((rect.Height())<<16)+rect.Width());
			
			m_wndInstantBar.SetWindowText(str);
			//if(!m_wndInstantBar.IsVisible())
			ShowControlBar(&m_wndInstantBar, TRUE, FALSE);
			m_pwndExpObj->Initialize();
		}
		else
		{
			if(!m_wndInstantBar.IsVisible())
				m_pwndExpObj->Update(0);
			ShowControlBar(&m_wndInstantBar, !m_wndInstantBar.IsVisible(), FALSE);
		}
	}catch(...){}
}

void CMainFrame::OnUpdateViewExp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
		if(m_wndInstantBar.GetSafeHwnd()==NULL)
			return;
		BOOL bCheck = FALSE;
		if(m_wndInstantBar.IsVisible())
		{
			CString strId;
			CString strText;
			m_wndInstantBar.GetWindowText(strText);
			//
			if (pCmdUI->m_nID == ID_VIEW_HIS )
			{
				LOADSTR(strId ,IDS_TITLE_HIS);
				if(strId==strText)
					bCheck = TRUE;
			}
			else if (pCmdUI->m_nID == ID_FAVORITES_DROPDOWN)
			{
				LOADSTR(strId ,IDS_TITLE_FAVORITE);
				if(strId==strText)
					bCheck = TRUE;
			}
			else if (pCmdUI->m_nID == ID_VIEW_FOLDER )
			{
				LOADSTR(strId ,IDS_TITLE_SHELL);
				if(strId==strText)
					bCheck = TRUE;
			}
			else if (pCmdUI->m_nID == ID_VIEW_EXPLOREBAR_RESOURCE )
			{
				LOADSTR(strId ,IDS_TITLE_RESOURCE);
				if(strId==strText)
					bCheck = TRUE;
			}
		}
		ToolBarSetCheck(pCmdUI, bCheck);
	}catch(...){}
}

void CMainFrame::RemoveExp()
{
	try{
		if(m_pwndExpObj != NULL)
		{
			m_pwndExpObj->Destroy();
			delete m_pwndExpObj;
			m_pwndExpObj = NULL;
		}
	}catch(...){}
}

#define POS		(1<<i)
void CMainFrame::HideSelectBars(DWORD dwShow)
{
	g_nBarVisible = 0;
	for (int i=0; i<MAX_BAR; i++)
	{
		if (!(dwShow & POS))
		{
			CWnd *pWnd = GetBandWnd(g_BarInfo[i]);
			g_nBarVisible |= pWnd->IsWindowVisible() * POS;
			pWnd->ShowWindow(SW_HIDE);
			GetMenu()->CheckMenuItem(g_BarInfo[i], MF_UNCHECKED);
		}
	}
}

void CMainFrame::ShowSelectBars(DWORD dwShow)
{
	for (int i=0; i<MAX_BAR; i++)
	{
		if ( g_nBarVisible & POS && !(dwShow & POS))
		{
			GetBandWnd(g_BarInfo[i])->ShowWindow(SW_SHOW);
			GetMenu()->CheckMenuItem(g_BarInfo[i], MF_CHECKED);
		}
	}
}

void CMainFrame::FullScreenModeOn(BOOL bOnlyBar)
{
	if (!bOnlyBar)
	{
		// now save the old positions of the main and child windows
		GetWindowRect(&m_rMainRect);
		// remove the caption of the mainWnd:
		LONG style=::GetWindowLong(m_hWnd,GWL_STYLE);
		style&=~WS_CAPTION;
		::SetWindowLong(m_hWnd,GWL_STYLE,style);
		int screenx=GetSystemMetrics(SM_CXSCREEN);
		int screeny=GetSystemMetrics(SM_CYSCREEN);
		// resize:
		SetWindowPos(NULL,-4,-3,screenx+8,screeny+6,SWP_NOZORDER);
	}
	HideSelectBars(m_dwFsShow);
	RecalcLayout();
	m_bFullScreened = TRUE;
}

void CMainFrame::FullScreenModeOff(BOOL bOnlyBar)
{
	if (!bOnlyBar)
	{
		LONG style=::GetWindowLong(m_hWnd,GWL_STYLE);
		style|=WS_CAPTION;
		::SetWindowLong(m_hWnd,GWL_STYLE,style);
		MoveWindow(&m_rMainRect);
	}
	ShowSelectBars(m_dwFsShow);
	RecalcLayout();
	m_bFullScreened=FALSE;
}

void CMainFrame::OnViewFullscreen() 
{
	// TODO: Add your command handler code here
	if(!IsWindowVisible() || IsIconic())//if(bHidden)
		OnHotKey(0,0);
	if (m_bAutoHideMode)
		OnViewAutoHide();
	//
	m_bFullScreenMode = !m_bFullScreenMode;
	if(m_bFullScreenMode)
		FullScreenModeOn();
	else
		FullScreenModeOff();
}

//##############################################################
void CMainFrame::AutoHideModeOn()
{
	try
	{
		
		HideSelectBars(m_dwAhShow);
		RecalcLayout();
		m_bAutoHided=TRUE;
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::AutoHideModeOff()
{
	try
	{
		
		ShowSelectBars(m_dwAhShow);
		RecalcLayout();
		m_bAutoHided=FALSE;
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnViewAutoHide() 
{
	try
	{
		
		// TODO: Add your command handler code here
		if(!IsWindowVisible() || IsIconic())//if(bHidden)
			OnHotKey(0,0);
		//cancel
		if (m_bFullScreenMode)
			OnViewFullscreen();
		//
		m_bAutoHideMode = !m_bAutoHideMode;
		if (m_bAutoHideMode)
			AutoHideModeOn();
		else
			AutoHideModeOff();
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnUpdateViewFullscreen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
		ToolBarSetCheck(pCmdUI, m_bFullScreenMode);
	}catch(...){}
}

//message from another instance
void CMainFrame::OnShellOpen(WPARAM wParam,LPARAM)
{
	try{
		/*beign :user share memory*/
		LPSTR lpData;
		HANDLE hMapping = CreateFileMapping(
			(HANDLE)0xFFFFFFFF,NULL,
			PAGE_READWRITE,0,2560,"MY9IESHARE"
			);
		if (hMapping==NULL){
			MSGBOX("Fail to create share memory!");
			return;
		}
		lpData = (LPSTR) MapViewOfFile(hMapping,FILE_MAP_ALL_ACCESS,0,0,0);
		if (lpData==NULL){
			MSGBOX("MapViewOfFile Fail");
			CloseHandle(hMapping);
			return;
		}
		CString	strURL = lpData;
		//release memory
		UnmapViewOfFile(lpData);
		CloseHandle(hMapping);
		/*end :user share memory*/
		
		//remove the tray icon
		if(!IsWindowVisible())
			OnHotKey(1,0);
		else
			SetForegroundWindow();
		
		TCHAR	buf[INTERNET_MAX_PATH_LENGTH];
		if (IS_FAVURL(strURL))
		{
			// an .URL file is formatted just like an .INI file, so we can
			// use GetPrivateProfileString() to get the information we want
			::GetPrivateProfileString(_T("InternetShortcut"), _T("URL"),
				_T(""), buf, INTERNET_MAX_PATH_LENGTH,
				strURL);
			strURL =  buf;
		}
		NewChildWindow(1,2,strURL, NULL, TRUE);
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

BOOL CMainFrame::ResolveLink(LPCSTR lpszLinkFile, LPSTR lpszPath)
{
    static char szGotPath[MAX_PATH]; 
    static char szDescription[MAX_PATH]; 
    static WCHAR wsz[MAX_PATH]; 
	
	if(lpszLinkFile == NULL)
		return FALSE;
    HRESULT hres; 
    IShellLink* psl = NULL; 
    IPersistFile* ppf = NULL; 
    WIN32_FIND_DATA wfd; 
	
	try
	{
		*lpszPath = 0; // assume failure 
		// Get a pointer to the IShellLink interface. 
		hres = CoCreateInstance(CLSID_ShellLink, NULL, 
			CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *) &psl); 
		if (SUCCEEDED(hres))
		{ 
			// Get a pointer to the IPersistFile interface. 
			hres = psl->QueryInterface(IID_IPersistFile, 
				(void**)&ppf); 
			if (SUCCEEDED(hres))
			{
				// Ensure that the string is Unicode. 
				MultiByteToWideChar(CP_ACP, 0, lpszLinkFile, -1, wsz, 
					MAX_PATH); 
				// Load the shortcut. 
				hres = ppf->Load(wsz, STGM_READ); 
				if (SUCCEEDED(hres))
				{ 
					// Resolve the link. 
					hres = psl->Resolve(this->m_hWnd, SLR_ANY_MATCH); 
					if (SUCCEEDED(hres))
					{
						// Get the path to the link target. 
						hres = psl->GetPath(szGotPath, 
							MAX_PATH, (WIN32_FIND_DATA *)&wfd, 
							SLGP_SHORTPATH ); 
						// Get the description of the target. 
						hres = psl->GetDescription(szDescription, MAX_PATH); 
						lstrcpy(lpszPath, szGotPath); 
					} 
				} 
				// Release the pointer to the IPersistFile interface. 
				ppf->Release(); 
				ppf = NULL;
			} 
			// Release the pointer to the IShellLink interface. 
			psl->Release(); 
			psl = NULL;
		}
	}catch(...){}
	
    return SUCCEEDED(hres);
}

void CMainFrame::OnFileNew() 
{
	if (m_bActiveFileNew)
		NewChildWindow(1,1,NULL, NULL, TRUE);
	else
		NewChildWindow(1,1,NULL);
}

CChildFrame* CMainFrame::NewChildWindow(int nIniLevel, int nOpenType, LPCSTR strUrl, CMyIE9View * pParentView, BOOL bForceActive)
{
	//remove no use 0d0a
	CString strUrlNew = strUrl;
	//not remove mid blank, ex: H:\My Documents\web
	strUrlNew.TrimLeft();strUrlNew.TrimRight();

	if ( strUrlNew.FindOneOf("\r\n") >= 0 )
	{
		strUrlNew.Remove('\r');
		strUrlNew.Remove('\n');
	}
	if (strUrlNew.GetLength()>0)
	{
		if ( IS_SCRIPT_MAILTO_STR(strUrl) )
			return NULL;
		if (DoSpecialUrl(strUrlNew))
			return NULL;
		//for drag morequick.com, need add www.
		DO_QUICK_URL(strUrlNew);
	}
	//find if there any unsed window then reuse it.
	int n = m_nWin;
	BOOL found = FALSE;
	int i=0;
	TCITEM TabCtrlItem;
	CString str;
	CChildFrame *pWnd = NULL;
	CString url;
	
	try{
		TabCtrlItem.mask = TCIF_PARAM;
		str = "";
		if(nIniLevel <2)
		{
			//check the blank window first;
			if(pParentView != NULL && g_bFilteBlank)
			{
				try{
					CChildFrame* tcf = ((CChildFrame*)pParentView->GetParentFrame())->m_pBlankChild;
					if(tcf!=NULL)
					{
						tcf->m_pView->GetLocationURL(url);
						double secs = 1;
						if(url == "")
						{
							LARGE_INTEGER liFinish;
							QueryPerformanceCounter(&liFinish);
							LARGE_INTEGER liFreq;
							QueryPerformanceFrequency(&liFreq);
							secs = (double)(liFinish.LowPart - ((CChildFrame*)pParentView->GetParentFrame())->m_tBlankChild.LowPart)/(double)liFreq.LowPart;
						}
						if(url=="about:blank" || secs<0.25) //if the time is too short we assume it as an blank window.
						{
							pWnd = tcf;
							found = TRUE;
							pWnd->m_pView->m_nProgress = 0;
						}
					}
				}catch(...)
				{
					found = FALSE;
				}
			}
			
			i=0;
			while (i<n && !found)
			{
				TabCtrlItem.mask = TCIF_PARAM;
				TabCtrlItem.lParam = 0;
				m_wndTab.GetItem(i, &TabCtrlItem);
				if(TabCtrlItem.lParam != NULL)
				{	
					if(((CChildFrame *)TabCtrlItem.lParam)->m_pView->m_nProgress<0)
					{
						str = ((CChildFrame *)TabCtrlItem.lParam)->m_pView->m_lpszUrl;
						if(str.IsEmpty())
							((CChildFrame *)TabCtrlItem.lParam)->GetWindowText(str);
						if(str == "about:blank")
						{
							pWnd = ((CChildFrame *)TabCtrlItem.lParam);
							found = TRUE;
							pWnd->m_pView->m_nProgress = -1;
							pWnd->m_pView->m_bReUsed = TRUE;
						}
						else
							i++;
					}
					else
						i++;
				}
				else
					i++;
			}
		}
		
		if(!found)
		{
			try
			{
				pWnd = new CChildFrame;
			}
			catch(...)
			{
				return NULL;
			}
			if(pWnd == NULL)
				return NULL;
			pWnd->m_bForceActive = bForceActive;
			if (!pWnd->Create(_T("ChildFrame"), "",
				WS_CHILD  | WS_OVERLAPPEDWINDOW, rectDefault, this))
			{
				delete pWnd;
				return NULL;
			}
			if(bForceActive)
			{
				pWnd->ShowWindow(SW_SHOW);
				if (strUrlNew.GetLength()>0)
					SetAddressText(strUrlNew);
			}
		}
		
		if(pWnd!=NULL && pWnd->m_pView !=NULL)
		{
			pWnd->m_pView->m_bFirst = TRUE;
			pWnd->m_pView->m_nProgress = -1;
			//inherite parent's settings
			if(pParentView!=NULL)
			{
				pWnd->m_pView->m_strWebProxy = pParentView->m_strWebProxy;
				pWnd->m_pView->m_bEnWebProxy = pParentView->m_bEnWebProxy;
				pWnd->m_pView->m_dwProperty = pParentView->m_dwProperty;
				pWnd->m_pView->m_nSrSpeed = pParentView->m_nSrSpeed; //1-slow, 2-medium, 3-fast
				pWnd->m_pView->m_bScrollAfterDL = pParentView->m_bScrollAfterDL;
				pWnd->m_pView->m_bUseAutoScroll = pParentView->m_bUseAutoScroll;
			}
			else
			{
				pWnd->m_pView->m_nSrSpeed = m_nDefSrSpeed; //1-slow, 2-medium, 3-fast
				pWnd->m_pView->m_bScrollAfterDL = m_bDefScrollAfterDL;
				pWnd->m_pView->m_bUseAutoScroll = FALSE;
			}
			if(nIniLevel && nOpenType == 2)
				pWnd->m_pView->m_lpszUrl = strUrlNew;
			if(found)
			{
				if( m_nWin==0 || bForceActive)
				{
					pWnd->m_pView->m_bIsActive=TRUE;
					MDIActivate(pWnd);
					m_wndTab.SetCurSel(i);
				}
			}
			if(nIniLevel)
				pWnd->m_pView->OnInitialUpdate(nOpenType, strUrlNew);
		}
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}	
	return pWnd;
}

void CMainFrame::OnUpdateOptionsActivenewwindow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(!m_bActiveNewWindow ); //gf 界面显示的意思反过来了 setcheck为true时界面是凹下去的
}

void CMainFrame::OnViewFavorite()
{
	CString str,str1;
	
	try{
		if(m_wndInstantBar.GetSafeHwnd()==NULL)
			BuildExpBar("");
		BOOL bNeedInI=false;
		if(m_pwndExpObj == NULL)
			bNeedInI = true;
		else
			LOADSTR(str ,IDS_TITLE_FAVORITE);
		m_wndInstantBar.GetWindowText(str1);
		if(bNeedInI || str1!=str)
		{
			RemoveExp();
			try{
				m_pwndExpObj = new CFavTree;
			}
			catch(...)
			{
				m_pwndExpObj = NULL;
				return;
			}
			m_pwndExpObj->Create(&m_wndInstantBar);
			m_pwndExpObj->Initialize();
			str = m_pwndExpObj->GetTitle();
			
			CRect rect;
			m_wndInstantBar.GetClientRect(&rect);
			m_wndInstantBar.SendMessage(WM_SIZE, 0, ((rect.Height())<<16)+rect.Width());
			
			m_wndInstantBar.SetWindowText(str);
			//if(!m_wndInstantBar.IsVisible())
			ShowControlBar(&m_wndInstantBar, TRUE, FALSE);
		}
		else
		{
			ShowControlBar(&m_wndInstantBar, !m_wndInstantBar.IsVisible(), FALSE);
		}
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

BOOL CMainFrame::HasExpBar(int nStr)
{
	try{
		if(m_wndInstantBar.GetSafeHwnd()==NULL)
			return FALSE;
		CString strText;
		CString strId;
		if(m_pwndExpObj == NULL)
			return FALSE;
		else
			LOADSTR(strId ,nStr);
		m_wndInstantBar.GetWindowText(strText);
		if(strId!=strText)
			return FALSE;
		else
			return TRUE;
	}catch(...)
	{
		return FALSE;
	}
}

void CMainFrame::OnRtabAddfav() 
{
	try
	{
		
		// TODO: Add your command handler code here
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		
		m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
		CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
		AddFavorite(tcf);
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

HRESULT ItemFromPath (LPCTSTR lpszPath, LPITEMIDLIST& pidl)
{
	ASSERT (lpszPath != NULL);
	LPSHELLFOLDER pDesktopFolder;
	HRESULT hr = SHGetDesktopFolder (&pDesktopFolder);
	if (FAILED (hr))
	{
		return hr;
	}
	
	OLECHAR olePath [MAX_PATH];
	//------------------------------------------------------------ 
	// IShellFolder::ParseDisplayName requires the file name be in
	// Unicode.
	//------------------------------------------------------------ 
#ifdef _UNICODE
	lstrcpy (olePath, lpszPath);
#else
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpszPath, -1,
		olePath, MAX_PATH);
#endif
	
	//-----------------------------------
	// Convert the path to an ITEMIDLIST.
	//-----------------------------------
	ULONG chEaten;
	ULONG dwAttributes;
	hr = pDesktopFolder->ParseDisplayName (
		NULL,
		NULL,
		olePath,
		&chEaten,
		&pidl,
		&dwAttributes);
	
	pDesktopFolder->Release ();
	return hr;
}

typedef BOOL (CALLBACK* LPFNADDFAV)(HWND, TCHAR*, UINT, TCHAR*, UINT, LPITEMIDLIST);

void CMainFrame::AddFavorite(CChildFrame* tcf)
{
	try{	
		if (!tcf)
			return;
		CMyIE9View* pView = tcf->m_pView;
		CString strTitle,strURL;
		GET_TAB_TITLE(tcf,strTitle);
		//REPLACE_ERR_PATH_CHAR(strTitle);//no use do this, Do AddToFavDlg will do the work
		pView->GetLocationURL(strURL);
		if (strTitle.GetLength()<1 || strURL.GetLength()<1)
			return;

		CGf_StartPageMgr::_ins.AddFav(strTitle, strURL);

		if (m_bUseDefaultFavPath)
		{
			VARIANT vtTitle;
			vtTitle.vt=VT_BSTR;
			vtTitle.bstrVal = strTitle.AllocSysString();
			BSTR URL = strURL.AllocSysString();
			AddtoFavorite(URL, &vtTitle);
			//
			SysFreeString(URL);
			SysFreeString(vtTitle.bstrVal);
		}
		else
		{
			HMODULE hMod = (HMODULE)LoadLibrary("shdocvw.dll");
			if (hMod)
			{
				LPFNADDFAV lpfn = (LPFNADDFAV)GetProcAddress(hMod, "DoAddToFavDlg");
				if (lpfn)
				{
					CString favPath;
					GetFavoriteFolder(favPath);
					
					LPITEMIDLIST pidl;
					ItemFromPath(favPath.GetBuffer(favPath.GetLength()), pidl);
					//
					BOOL bRet = lpfn(m_hWnd, 
						(LPSTR)(LPCTSTR)favPath.GetBuffer(MAX_PATH),MAX_PATH,
						(LPSTR)(LPCTSTR)strTitle.GetBuffer(MAX_PATH),MAX_PATH,
						pidl
						);
					favPath.ReleaseBuffer();
					strTitle.ReleaseBuffer();
					//
					::WritePrivateProfileString(_T("InternetShortcut"), _T("URL"), strURL, favPath);
					m_bFavDirty = TRUE;
				}
				FreeLibrary(hMod);
			}
		}
	}catch(...){}
}

int CMainFrame::OpenAllFavs(LPCSTR lpszPath)
{
	WIN32_FIND_DATA wfd;
	HANDLE          h;
	CString         strPath2(lpszPath);
	static TCHAR	buf[INTERNET_MAX_PATH_LENGTH];
	int				nWins = 0;
	CString strPath(lpszPath);
	strPath += "*.*";
	if(strPath2=="" || strPath2.IsEmpty())
		return 0;
	
	g_nOldNewSequence = m_nNewSequence;
	m_nNewSequence = 0;
	// now scan the directory, first for .URL files and then for subdirectories
	// that may also contain .URL files
	h = FindFirstFile(strPath, &wfd);
	if(h != INVALID_HANDLE_VALUE)
	{
		try{
			do
			{
				if((wfd.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_HIDDEN))==0)
				{
					m_strMenuUrl = wfd.cFileName;
					if(IS_FAVURL(m_strMenuUrl))
					{
						// an .URL file is formatted just like an .INI file, so we can
						// use GetPrivateProfileString() to get the information we want
						::GetPrivateProfileString(_T("InternetShortcut"), _T("URL"),
							_T(""), buf, INTERNET_MAX_PATH_LENGTH,
							strPath2 + m_strMenuUrl);
						if(nWins !=0)
							m_astrOpenAll.AddTail(buf);
						else
							NewChildWindow(1,2, buf);
						nWins ++;
					}
					else  
					{
						if(ResolveLink(strPath2 + m_strMenuUrl, buf))
						{
							if(nWins !=0)
								m_astrOpenAll.AddTail(buf);
							else
								NewChildWindow(1,2, buf);
							nWins ++;
						}
					}
				}
			}while(FindNextFile(h, &wfd) && (nWins<g_nMaxWins || g_nMaxWins==0));
			FindClose(h);
		}catch(...){}
		
		if(m_nInterval && nWins>1)
			m_nTimerID = SetTimer(1000, m_nInterval*1000, NULL);
		else if(nWins>1)
			m_nTimerID = SetTimer(1000, 20, NULL);
		//enable toolbar button
		m_wndToolBar.GetToolBarCtrl().EnableButton(ID_FILE_STOP_OPENALL);
	} 
	return 1;
}

BOOL CMainFrame::GetFavoriteFolder(CString & strFav)
{
	//static TCHAR		szPath[MAX_PATH];
	
	if (!m_bUseDefaultFavPath)
	{
		strFav = m_strFavPath;
		DO_RELATIVE_URL(strFav);
		return TRUE;
	}
	
	return CRegHelper::GetFavoriteFolder(strFav);
	
	//gff 错。和set的真正起作用的不是实时同步的。
	//	BOOL r = TRUE;
	//	
	//	try
	//	{
	//		LPITEMIDLIST pidlFavorites = NULL;
	//		// Get a pointer to the IMalloc interface
	//		IMalloc * m_pMalloc = NULL;
	//		HRESULT hr = SHGetMalloc(&m_pMalloc);
	//		if (SUCCEEDED(hr))
	//		{
	//			HRESULT hr = SHGetSpecialFolderLocation(NULL, CSIDL_FAVORITES, &pidlFavorites);
	//			if (NOERROR != hr)
	//				r = FALSE;
	//			else
	//				r = SHGetPathFromIDList( pidlFavorites,szPath);
	//			m_pMalloc->Free(pidlFavorites);
	//			m_pMalloc->Release();
	//			m_pMalloc = NULL;
	//		}		
	//		if(r)
	//		{
	//			strFav = szPath;
	//		}
	//		else
	//		{
	//			MSGBOX("Can't get path of favorite.");
	//			::GetWindowsDirectory (szPath, MAX_PATH); 
	//			strFav = szPath;
	//			strFav += "\\favorites";
	//		}
	//		m_strFavPath = strFav;
	//	}catch(...){}
	
	//	return TRUE;	
}

void CMainFrame::OnLinkPopup() 
{
	CPoint pt,pt2;
	GetCursorPos(&pt);
	m_LinkBar.ScreenToClient(&pt);
	int iButton = m_LinkBar.GetToolBarCtrl().HitTest(&pt);
	if (iButton >= 0 && iButton<m_LinkBar.GetToolBarCtrl().GetButtonCount()) // if mouse is over a button:
	{
		TBBUTTON tbb;
		CRect rect;
		if( m_LinkBar.GetToolBarCtrl().GetButton( iButton,  &tbb ))
		{
			m_LinkBar.GetToolBarCtrl().GetItemRect(iButton, &rect);
			m_LinkBar.ClientToScreen(&rect);
			
			HMENU hmenu = (HMENU)tbb.dwData;
			CToolMenu * menu = NULL;
			try
			{
				menu = new CToolMenu;
				menu->Attach(hmenu);
				menu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left,rect.bottom+1, this);
				menu->Detach();
			}catch(...){}
			
			try{
				if(menu)
					delete menu;
			}catch(...){}
		}
	}
}

void CMainFrame::OnFavPopup() 
{
	CPoint pt,pt2;
	GetCursorPos(&pt);
	m_FavBar.ScreenToClient(&pt);
	int iButton = m_FavBar.GetToolBarCtrl().HitTest(&pt);
	if (iButton >= 0 && iButton<m_FavBar.GetToolBarCtrl().GetButtonCount()) // if mouse is over a button:
	{
		TBBUTTON tbb;
		CRect rect;
		if( m_FavBar.GetToolBarCtrl().GetButton( iButton,  &tbb ))
		{
			m_FavBar.GetToolBarCtrl().GetItemRect(iButton, &rect);
			m_FavBar.ClientToScreen(&rect);
			
			HMENU hmenu = (HMENU)tbb.dwData;
			CToolMenu menu;
			menu.Attach(hmenu);
			menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left,rect.bottom+1, this);
			menu.Detach();
		}
	}
}

void CMainFrame::OnUpdateViewToolbar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
		REBARBANDINFO rbbi;
		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
		int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_VIEW_TOOLBAR);
		m_wndReBar.GetReBarCtrl().GetBandInfo(nb, &rbbi);
		if( rbbi.fStyle & RBBS_HIDDEN )
			pCmdUI->SetCheck(0);
		else
			pCmdUI->SetCheck();
	}catch(...){}
}

void CMainFrame::OnUpdateFav(WPARAM wParam,LPARAM)
{
	try{
		InitFavorites();
		//wParam==2 means change from fav tree
		if (wParam != 2)
		{
			if(HasExpBar(IDS_TITLE_FAVORITE))
				m_pwndExpObj->Update(1);
		}
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
}}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	try{
		CMDIFrameWnd::OnSize(nType, cx, cy);	
		PostMessage(WM_UPDATE_TAB, TRUE);
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnOptionMultiTab() 
{
	// TODO: Add your command handler code here
	if(m_bMultiLineTab)
	{
		m_wndTab.ModifyStyle(TCS_MULTILINE, 0);
		m_bMultiLineTab = FALSE;
	}
	else
	{
		m_wndTab.ModifyStyle(0, TCS_MULTILINE);
		m_bMultiLineTab = TRUE;
	}
	PostMessage(WM_UPDATE_TAB);
}

void CMainFrame::OnUpdateTab(WPARAM wParam,LPARAM)
{
	try{
		int y = AdjustTabWidth(!wParam);
		int r = m_wndTab.GetRowCount();
		if(r==0)
			r=1;
		if(m_nPreLines==r && wParam!=2)
			return;
		m_nPreLines = r;
		
		//adjust tab height
		if(r==1 || m_nTabStyle!=2)
			y+=0; //1
		else
			y+=3;
		
		REBARBANDINFO rbbi;
		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE |RBBIM_ID;
		rbbi.wID = ID_VIEW_TASKBAR;
		rbbi.cxMinChild = 0;
		if(m_nTabStyle==2)
			rbbi.cyMinChild = y*r;// + 10
//gfb	else if(m_nTabStyle==1)
//			rbbi.cyMinChild = y*r+4;// + 10
		else if(m_nTabStyle==0)
			rbbi.cyMinChild = y*r+5;// + 10
		else
		{
			rbbi.cyMinChild = y*r+4;// + 10
		}

		rbbi.cxIdeal = 200;
		
		//add tab to top or bottom
		if(m_bTopTab)
		{
			int nb=m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_TASKBAR);
			m_wndReBar.GetReBarCtrl().SetBandInfo(nb, &rbbi);
		}
		else
		{
			m_wndReBar2.GetReBarCtrl().SetBandInfo(0, &rbbi);
		}
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnAllFav(UINT nID)
{
	int i = nID-B_OPEN_FAV;
	POSITION pos = m_astrFavoriteFolders.FindIndex(i);
	if(pos!=NULL)
		OpenAllFavs(m_astrFavoriteFolders.GetAt(pos)+"\\");
}

void CMainFrame::OnRtabCloseKeepcur() 
{
	try
	{
		
		// TODO: Add your command handler code here
		int n = m_wndTab.GetItemCount();
		if (n<1)
			return;
		if(g_bConfirmCloseAllKeepCur)
		{
			if(MSGBOX(IDS_CLOSE_ALL_CONF, MB_YESNO|MB_ICONQUESTION)==IDNO)
				return;
		}
		
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		//
		CChildFrame* tcf, *cur;
		if (g_bRTab)
		{	
			m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
			cur = (CChildFrame*)TabCtrlItem.lParam;
		}
		else
			cur = (CChildFrame*)MDIGetActive();
		
		if (!cur || !cur->m_pView)
			return ;
		
		int l =n-1;
		for(int i=n-1;i>=0;i--)
		{
			m_wndTab.GetItem(l, &TabCtrlItem);
			tcf = ((CChildFrame*)TabCtrlItem.lParam);
			if(tcf != NULL)
			{
				if(tcf!=cur)
				{
					CLOSE_PAGE(tcf);
				}
			}
			l--;
		}
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnFileCloseKeepcur() 
{
	// TODO: Add your command handler code here
	//for that ctrl+k not make rtab as 0
	g_bRTab = 0;
	OnRtabCloseKeepcur();
}

void CMainFrame::OnViewRefreshall() 
{
	// TODO: Add your command handler code here
	CChildFrame* tcf;
	int n = m_wndTab.GetItemCount();
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	int l =n-1;
	
	try
	{
		for(int i=n-1;i>=0;i--)
		{
			m_wndTab.GetItem(l, &TabCtrlItem);
			tcf = ((CChildFrame*)TabCtrlItem.lParam);
			if(VALID_TCF(tcf))
			{
				tcf->m_pView->PowerRefresh();//Refresh2(REFRESH_COMPLETELY);
			}
			l--;
		}
	}catch(...){}
}

void CMainFrame::OnViewStopAll() 
{
	// TODO: Add your command handler code here
	int i;
	int count = m_wndTab.GetItemCount();
	for(i=0; i<count; i++)
		StopView(i);
}

void CMainFrame::OnHelpHomepage() 
{
	CString str = "http://myie9.net/";
	//LOADSTR(str, IDS_HOMEPAGE);
	
	NewChildWindow(1,2, str);
}

int CMainFrame::GetTabIcon(int nItem)
{
	try
	{
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_IMAGE;
		m_wndTab.GetItem( nItem, &TabCtrlItem);
		return TabCtrlItem.iImage;
	}catch(...){}
	
	return -1;
}

void CMainFrame::SetTabIcon(int nIcon, int nItem, CMyIE9View* pView)
{
	try{
		//better place at first
		if (g_bShowRead && nIcon==TI_NO)
			nIcon = TI_READ;
		if (nIcon == pView->m_nOldIcon)
			return;
		if (g_bShowFavIcon)
		{
			if (nIcon==TI_100 || nIcon==TI_NO || nIcon==TI_READ)
			{
				if (IS_FAVICON(pView->m_nOldIcon))
					return;
				int nFavIcon = GetFavIcon(pView->m_lpszUrl);
				if (IS_FAVICON(nFavIcon))
					nIcon = nFavIcon;
			}
		}
		//set icon
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_IMAGE;
		TabCtrlItem.iImage = nIcon;
		m_wndTab.SetItem( nItem, &TabCtrlItem );
		//
		pView->m_nOldIcon = nIcon;
	}
	catch(...){}
}

void CMainFrame::OnUpdateTabTips(WPARAM wParam,LPARAM)
{
	CChildFrame* tcf;
	int n = m_nWin;//m_wndTab.GetItemCount();
	int l =n-1;
	TCITEM TabCtrlItem;
	
	try{
		TabCtrlItem.mask = TCIF_PARAM;
		CToolTipCtrl* pTT = m_wndTab.GetToolTips();
		if(pTT !=NULL)
		{
			for(int i=n-1;i>=1;i--)
			{
				TabCtrlItem.lParam = 0;
				m_wndTab.GetItem(l, &TabCtrlItem);
				tcf = ((CChildFrame*)TabCtrlItem.lParam);
				if(VALID_TCF(tcf) && !tcf->m_pView->m_bToClose)
				{
					pTT->UpdateTipText(tcf->m_strTitle, &m_wndTab, i);
				}
				l--;
			}
		}
	}catch(...){}
}

void CMainFrame::OnWindowCascade() 
{
	// TODO: Add your command handler code here
	if(m_nProtectNum>0)
	{
		MSGBOX(IDS_CANT_DO);
		return;
	}
	g_bMax = 2;
	CMDIFrameWnd::OnMDIWindowCmd(ID_WINDOW_CASCADE);
}

void CMainFrame::OnWindowTileHorz() 
{
	// TODO: Add your command handler code here
	if(m_nProtectNum>0)
	{
		MSGBOX(IDS_CANT_DO);
		return;
	}
	g_bMax = 2;
	CMDIFrameWnd::OnMDIWindowCmd(ID_WINDOW_TILE_HORZ);
}

void CMainFrame::OnWindowTileVert() 
{
	// TODO: Add your command handler code here
	if(m_nProtectNum>0)
	{
		MSGBOX(IDS_CANT_DO);
		return;
	}
	g_bMax = 2;
	CMDIFrameWnd::OnMDIWindowCmd(ID_WINDOW_TILE_VERT);
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CMDIFrameWnd::OnSettingChange(uFlags, lpszSection);
	
	// TODO: Add your message handler code here
	try{
		m_nScrollWidth = GetSystemMetrics(SM_CXVSCROLL);
		m_nScrollArrowH = GetSystemMetrics(SM_CYVSCROLL);
	}catch(...){}
}

void CMainFrame::OnTabNeedText(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	try{
		TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNotifyStruct;
		UINT nID =pNotifyStruct->idFrom;
		if (pTTT!=NULL  &&  ! (pTTT->uFlags & TTF_IDISHWND))
		{
			// idFrom is actually the HWND of the tool
			if(nID==0)
			{
				CChildFrame* tcf;
				TCITEM TabCtrlItem;
				TabCtrlItem.mask = TCIF_PARAM;
				
				m_wndTab.GetItem(0, &TabCtrlItem);
				tcf = ((CChildFrame*)TabCtrlItem.lParam);
				if(VALID_TCF(tcf))
				{
					CString strTip;
					tcf->GetWindowText(strTip);
					STRNCPY(pTTT->szText, strTip, 79);
				}
			}
		}
    }catch(...){}
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	try
	{
		// TODO: Add your specialized code here and/or call the base class
		if (DoKeyMsg(pMsg))
			return TRUE;
		
		m_wndMenuBar.TranslateFrameMessage(pMsg);
		if (DoMouseMsg(pMsg))
			return TRUE;
		return CMDIFrameWnd::PreTranslateMessage(pMsg);
	}
	catch(...)
	{
		return TRUE;
	}
}

BOOL CMainFrame::DoKeyMsg(MSG* pMsg)
{
	try{
		
		if (pMsg->message!=WM_KEYDOWN || PRESS_CTRL)
			return FALSE;
		//
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			if (m_wndAddress->GetDroppedState()==TRUE)
			{
				m_wndAddress->ShowDropDown(FALSE);
				return TRUE;
			}
			if (m_wndSearch->GetDroppedState()==TRUE)
			{
				m_wndSearch->ShowDropDown(FALSE);
				return TRUE;
			}
			//
			if (GetFocus() == (CWnd*)m_wndAddress->GetEditCtrl())
			{
				CChildFrame* tcf;
				tcf = (CChildFrame*)MDIGetActive();
				if (VALID_TCF(tcf) && tcf->m_pView->m_lpszUrl)
				{
					CString strTemp;
					m_wndAddress->GetEditCtrl()->GetWindowText(strTemp);
					if (strTemp != tcf->m_pView->m_lpszUrl)
						SetAddress(tcf->m_pView->m_lpszUrl);
					else
						tcf->ViewSetFocus();
					return TRUE;
				}
			}
			if (GetFocus() == (CWnd*)m_wndSearch->GetEditCtrl())
			{
				ActiveViewSetFocus();
				return TRUE;
			}
			break;
			
		case VK_DELETE:
			if (m_wndSearch->GetDroppedState()==TRUE)
			{
				int i = m_wndSearch->GetCurSel();
				if(i>=0)
				{
					m_wndSearch->DeleteItem(i);
					POSITION pos = m_astrTypedSearch.FindIndex(i);
					if(pos !=NULL)
						m_astrTypedSearch.RemoveAt(pos);
					if (i>0 && i>=m_wndSearch->GetCount())
						i--;
					m_wndSearch->SetCurSel(i);
					return TRUE;
				}
			}
			else if (m_wndAddress->GetDroppedState()==TRUE)
			{
				int i = m_AddressBar.m_wndAddress.GetCurSel();
				if(i>=0)
				{
					m_AddressBar.m_wndAddress.DeleteItem(i);
					if (i>0 && i>=m_AddressBar.m_wndAddress.GetCount())
						i--;
					m_AddressBar.m_wndAddress.SetCurSel(i);
					return TRUE;
				}
			}
			break;
			
		case VK_TAB:
			if ((CWnd*)m_wndSearch->GetEditCtrl() == GetFocus() || (CWnd*)m_wndSearch->GetComboBoxCtrl() == GetFocus() )
			{
				ActiveViewSetFocus();
				return TRUE;
			}
			else if (m_wndAddress->GetDroppedState()==TRUE)
			{
				OnNewAddressEnter();
				return TRUE;
			}
			else if (GetFocus() == (CWnd*)m_wndAddress->GetEditCtrl())
			{
				CChildFrame* tcf;
				tcf = (CChildFrame*)MDIGetActive();
				if (VALID_TCF(tcf) && tcf->m_pView->m_lpszUrl)
				{
					CString strTemp;
					m_wndAddress->GetEditCtrl()->GetWindowText(strTemp);
					if (strTemp == tcf->m_pView->m_lpszUrl)
					{
						tcf->ViewSetFocus();
						return TRUE;
					}
				}
			}
			break;
		}
		
	}catch(...){ASSERT(false);}
	
	//
	return FALSE;
}

BOOL CMainFrame::DoMouseMsg(MSG* pMsg)
{
	try{
		//not scroll address bar
		switch (pMsg->message)
		{
		case WM_MOUSEWHEEL:
			if (m_bWheelOnTab)
			{
				CPoint point;
				GetCursorPos(&point);
				CRect rect;
				
				::GetWindowRect(m_wndTab.GetSafeHwnd(), &rect);
				if (rect.PtInRect(point))
				{
					short zDelta = HIWORD(pMsg->wParam);
					if ( zDelta > 0)
						PostMessage(WM_COMMAND, ID_TAB_PRE, 0);
					else
						PostMessage(WM_COMMAND, ID_TAB_NEXT, 0);
					return TRUE;
				}
			}
			//
			if(pMsg->hwnd == m_wndAddress->GetEditCtrl()->GetSafeHwnd())
			{
				PostMessage(WM_MOUSEWHEEL, pMsg->wParam, pMsg->lParam);
				return TRUE;
			}
			else if(pMsg->hwnd == m_wndSearch->GetEditCtrl()->GetSafeHwnd())
			{	
				CPoint point;
				GetCursorPos(&point);
				CRect rect;
				
				::GetWindowRect(m_wndSearch->GetEditCtrl()->GetSafeHwnd(), &rect);
				if (!rect.PtInRect(point))
				{
					PostMessage(WM_MOUSEWHEEL, pMsg->wParam, pMsg->lParam);
					return TRUE;
				}
			}
			break;
			
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
			{
				RECT rect;
				::GetWindowRect(m_wndTab,&rect);
				if (::PtInRect(&rect,pMsg->pt))
				{
					CPoint point = pMsg->pt;
					m_wndTab.ScreenToClient(&point);
					//
					if (m_wndTab.GetTabIDFromPoint(point)<0)
					{
						PostMessage(WM_COMMAND, 
							pMsg->message==WM_LBUTTONDBLCLK ? m_nLDbClickTabBlank : m_nRDbClickTabBlank,
							0);
						return TRUE;
					}
				}
			}
			break;
		}
		//#################################################################
		//auto hide
		if (m_bAutoHideMode)
		{
			if (pMsg->message == WM_NCMOUSEMOVE)
			{
				if (m_bAutoHided)
				{
					POINT pt = pMsg->pt;
					ScreenToClient(&pt);
					//gf if (pt.y<0)
					if ( (pt.y<0)&&(pMsg->pt.x < (GetSystemMetrics(SM_CXSCREEN) * GF_AUTO_HIDE_TOP_RADIO ) ) )
						AutoHideModeOff();
				}
			}
			else if (pMsg->message == WM_MOUSEMOVE)
			{
				if (!m_bAutoHided)
				{
					RECT rect;
					::GetWindowRect(m_wndReBar,&rect);
					if (::PtInRect(&rect,pMsg->pt)==FALSE)
						AutoHideModeOn();
				}
			}
		}
		//full screen
		if (m_bFullScreenMode)
		{
			if (pMsg->message == WM_MOUSEMOVE)
			{
				static BOOL bShellTop=FALSE;
				if (!bShellTop)
				{
					if (pMsg->pt.y > GetSystemMetrics(SM_CYSCREEN)-4)
					{
						::BringWindowToTop(::FindWindow("Shell_TrayWnd",NULL));
						bShellTop=TRUE;
					}
				}
				else
				{
					::BringWindowToTop(GetSafeHwnd());
					bShellTop = FALSE;
				}
				//
				if (m_bFullScreened)
				{
					if (pMsg->pt.y<4)
						FullScreenModeOff(TRUE);
				}
				else
				{
					RECT rect;
					::GetWindowRect(m_wndReBar,&rect);
					if (::PtInRect(&rect,pMsg->pt)==FALSE)
						FullScreenModeOn(TRUE);
				}
			}
		}
		//
		
	}catch(...){ASSERT(false);}
	
	
	return FALSE;
}

void CMainFrame::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	try
	{
		
		// TODO: Add your message handler code here and/or call default
		if(lpMeasureItemStruct->CtlType==ODT_MENU)
		{
			CToolMenu menu;
			menu.MeasureItem(lpMeasureItemStruct);
		}
		CMDIFrameWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
		
	}
	catch (...) {
	}
}

BOOL CMainFrame::DecodeEscap(CString &strUrl)
{
	CString strUrl2 = strUrl;
	strUrl2.MakeLower();
	int n = strUrl2.GetLength();
	if(n==0 || strUrl.Find('%')<0)
		return FALSE;
	char c = strUrl.GetAt(0), ch1,ch2;
	int i=0;
	CString strTemp;
	while(c!='#' && c!='?' && i<n)
	{
		if(c!='%' || i+2>=n)
			strTemp+=c;
		else
		{
			ch1 = strUrl2.GetAt(i+1);
			ch2 = strUrl2.GetAt(i+2);
			if(((ch1>='0' && ch1<='9')||(ch1>='a' && ch1<='f')) && ((ch2>='0' && ch2<='9')||(ch2>='a' && ch2<='f')))
			{
				if(ch1>='0' && ch1<='9')
					c= (ch1-'0')*16;
				else
					c= (ch1-'a'+10)*16;
				if(ch2>='0' && ch2<='9')
					c+= ch2-'0';
				else
					c+= ch2-'a'+10;
				i+=2;
				strTemp+=c;
			}
			else
				strTemp+=c;
		}
		i++;
		c = strUrl.GetAt(i);
	}
	
	for(int l=i; l<n; l++)
		strTemp+= strUrl.GetAt(l);
	strUrl = strTemp;
	
	return TRUE;
}

CMenu* CMainFrame::GetMenu()
{
	return m_wndMenuBar.GetMenu();
}

void CMainFrame::OnWindowRestore() 
{
	try{
		
		// TODO: Add your command handler code here
		if(m_nProtectNum>0)
		{
			MSGBOX(IDS_CANT_DO);
			return;
		}
		g_bMax = 3;
		CChildFrame* tcf = (CChildFrame*)MDIGetActive();
		
		if (tcf!=NULL)
		{
			MDIRestore(tcf);
		}
		
	}catch(...){ASSERT(false);}
}

void CMainFrame::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	
	// TODO: Add your message handler code here and/or call default
	try{
		if(lpDrawItemStruct->CtlType==ODT_MENU)
		{
			CToolMenu menu;
			menu.DrawItem(lpDrawItemStruct);
		}
	}catch(...){}
	try{
		
		CMDIFrameWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
	}catch(...){ASSERT(false);}
}

void CMainFrame::OnWindowMin() 
{
	try{
		
		// TODO: Add your command handler code here
		if(m_nProtectNum>0)
		{
			MSGBOX(IDS_CANT_DO);
			return;
		}
		g_bMax = 3;
		CChildFrame* tcf = (CChildFrame*)MDIGetActive();
		
		if (tcf!=NULL)
		{
			tcf->ShowWindow(SW_MINIMIZE);
		}
		
	}catch(...){ASSERT(false);}
}


void CMainFrame::OnUpdateToolsSetSkin(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
		char tmp[5];
		
		m_astrSkins.RemoveAll();
		m_strSkinName.MakeLower();
		pCmdUI->SetCheck( m_strSkinName=="default" );
		
		CString strTemp;
		CString strPath = theApp.m_strSkinPath;
		strPath += "*.*";
		
		CMenu* pmenu = SMP_SKIN;
		DestroyMenuData(pmenu->m_hMenu,3);
		while(pmenu->DeleteMenu(3, MF_BYPOSITION));
		
		int i=0;
		WIN32_FIND_DATA wfd;
		HANDLE h = FindFirstFile(strPath, &wfd);
		if(h != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				{
					if(lstrcmp(wfd.cFileName, _T(".")) == 0 || lstrcmp(wfd.cFileName, _T("..")) == 0 || lstrcmpi(wfd.cFileName, _T("default")) == 0)
						continue;
					itoa(i+1, tmp, 10);
					strTemp = tmp;
					strTemp += ". ";
					strTemp += wfd.cFileName;
					MenuAppendOwnerItem( pmenu, MF_ENABLED, B_SKIN + i , strTemp);
					m_astrSkins.AddTail(wfd.cFileName);
					strTemp = wfd.cFileName; strTemp.MakeLower();
					if(strTemp == m_strSkinName)
						pmenu->CheckMenuItem(B_SKIN + i, MF_CHECKED);
					i++;
				}
			}
			while(FindNextFile(h, &wfd) && i<511);
		}
		FindClose(h);
		AddMenuBreak(pmenu, 0);
		
	}catch(...){}
}

void CMainFrame::InitToolBarImage(BOOL bReplaceIcon)
{
	// Create and set the normal toolbar image list.
	CImageList img, *oldimg;
	HBITMAP hbmp;
	oldimg = m_wndToolBar.GetToolBarCtrl().GetHotImageList();
	if(oldimg!=NULL && bReplaceIcon)
		oldimg->DeleteImageList();
	
	if(oldimg == NULL || bReplaceIcon)
	{
		//disable
		if(m_bSmallToolBar)
		{
			hbmp = GetBitmap("MainToolGray16.bmp");
			img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 40, 1);
		}
		else
		{
			hbmp = GetBitmap("MainToolGray24.bmp");
			img.Create(24, 24, ILC_COLORDDB|ILC_MASK, 40, 1);
		}
		ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
		DeleteObject(hbmp);
		m_wndToolBar.SendMessage(TB_SETDISABLEDIMAGELIST, 0, (LPARAM)img.m_hImageList);
		img.Detach();
		//normal
		if(m_bSmallToolBar)
		{
			hbmp = GetBitmap("MainTool16.bmp");
			img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 40, 1);
		}
		else
		{
			hbmp = GetBitmap("MainTool24.bmp");
			img.Create(24, 24, ILC_COLORDDB|ILC_MASK, 40, 1);
		}
		ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
		DeleteObject(hbmp);
		m_wndToolBar.SendMessage(TB_SETHOTIMAGELIST, 0, (LPARAM)img.m_hImageList);
		img.Detach();
	}
	
	oldimg = m_wndToolBar.GetToolBarCtrl().GetImageList();
	if(oldimg!=NULL && bReplaceIcon)
		oldimg->DeleteImageList();
	
	if(oldimg == NULL || bReplaceIcon)
	{
		if(m_bSmallToolBar)
		{
			if(m_bColorIcon)
				hbmp = GetBitmap("MainTool16.bmp");
			else
				hbmp = GetBitmap("MainToolGray16.bmp");
			img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 40, 1);
		}
		else
		{
			if(m_bColorIcon)
				hbmp = GetBitmap("MainTool24.bmp");
			else
				hbmp = GetBitmap("MainToolGray24.bmp");
			img.Create(24, 24, ILC_COLORDDB|ILC_MASK, 40, 1);
			
		}
		ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
		DeleteObject(hbmp);
		m_wndToolBar.SendMessage(
			TB_SETIMAGELIST, //TB_SETDISABLEDIMAGELIST,//TB_SETIMAGELIST, 
			0, (LPARAM)img.m_hImageList);
		img.Detach();
	}
}

void CMainFrame::InitToolBar(BOOL bReplaceIcon)
{
	InitToolBarImage(bReplaceIcon);
	//remove all buttons
	int b = m_wndToolBar.GetToolBarCtrl().GetButtonCount();
	int bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_TOOLBAR);
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_SIZE|RBBIM_STYLE;
	m_wndReBar.GetReBarCtrl().GetBandInfo(bandid, &rbbi);
	rbbi.fStyle &= ~RBBS_NOGRIPPER;
	
	for(int i = 0; i<b;i++)
		m_wndToolBar.GetToolBarCtrl().DeleteButton(0);
	CRect rectToolBar;
	
	if(m_nTextLabel<2)
		m_wndToolBar.ModifyStyle(TBSTYLE_LIST , TBSTYLE_FLAT | TBSTYLE_TRANSPARENT );
	else
		m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT| TBSTYLE_LIST );
	
	// set up each toolbar button
	int nMaxBtn = sizeof(g_nToolBarID)/sizeof(UINT);
	int ids[50], idx=0, id;
	static char tmp[6]; tmp[0]='B'; 
	
	id=0;idx=0;
	while(idx<nMaxBtn && id!=-1)
	{
		itoa(idx,tmp+1,10);
		id = (int)theApp.GetProfileInt("ToolBar", tmp, -5);
		if(id==-5)
			ids[idx] = idx;
		else if(id>=nMaxBtn)
			ids[idx] = 1;//add a seprate
		else
			ids[idx]=id;
		idx++;
	}
	int n=0;
	CString str;
	str.Empty();
	m_wndToolBar.SetButtons(NULL, idx-1);
	for(i=0; i<idx; i++)
	{
		itoa(i,tmp+1,10);
		id = ids[i];
		if(id>=0 && id<nMaxBtn)
		{
			if(g_nToolBarStyle[id]!=TBBS_SEPARATOR && m_nTextLabel!=0)//if(m_nTextLabel == 2 && g_nToolBarStyle[id]!=TBBS_SEPARATOR)
				m_wndToolBar.SetButtonInfo(n,g_nToolBarID[id],g_nToolBarStyle[id]| TBSTYLE_AUTOSIZE,g_nToolBarImg[id]);
			else
				m_wndToolBar.SetButtonInfo(n,g_nToolBarID[id],g_nToolBarStyle[id],g_nToolBarImg[id]);
			if(m_nTextLabel==1 || (m_nTextLabel==2 && g_ToolBarLabel[id]==1))
			{
				LOADSTR(str ,g_nToolBarText[id]);
				m_wndToolBar.SetButtonText(n, str.Right(str.GetLength()-str.Find('\n')-1));
			}
			n++;
			str.Empty();
		}
		else if(id>=nMaxBtn)
			continue;
		else
			break;
	}
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_FLYBY | CBRS_SIZE_DYNAMIC);//CBRS_TOOLTIPS |
	//button size
	if(m_nTextLabel!=1)
	{
		if(m_bSmallToolBar)
			m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(20,18));
		else
			m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(24,22));
	}
	else 
	{
		if(m_bSmallToolBar)
			m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(58,36));
		else
			m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(58,44));
	}
	
	m_wndToolBar.GetItemRect(0, &rectToolBar);
	
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE |RBBIM_ID |RBBIM_STYLE;
	rbbi.wID = ID_VIEW_TOOLBAR;
	rbbi.cxMinChild = rectToolBar.Width()+2;
	if(rectToolBar.Height()<18)
		rbbi.cyMinChild = 18;
	else
		rbbi.cyMinChild = rectToolBar.Height();
	
	int bc=m_wndToolBar.GetToolBarCtrl().GetButtonCount();
	if(bc>0)
	{
		m_wndToolBar.GetToolBarCtrl().GetItemRect(bc-1,&rectToolBar);
		/*rbbi.cx =*/ rbbi.cxIdeal = rectToolBar.right;
	}
	else
		rbbi.cx = rbbi.cxIdeal = 2;
	if(m_bLockToolbar)
	{
		rbbi.fStyle &= ~RBBS_GRIPPERALWAYS;
		rbbi.fStyle |= RBBS_NOGRIPPER;
	}
	m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
	m_wndToolBar.Invalidate();
}

void CMainFrame::OnUpdateToolbar(WPARAM wParam,LPARAM)
{
	try{
		InitToolBar();
	}catch(...){}
}

void CMainFrame::OnFileNewMyIE9() 
{
	// TODO: Add your command handler code here
    HINSTANCE hInstance = AfxGetResourceHandle();
    ASSERT(hInstance != NULL);
    LPTSTR lpszModule = new TCHAR[_MAX_PATH];
    if (GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
    {
		if((UINT)ShellExecute(NULL, "open", lpszModule , "", ".", SW_SHOW )<=32)
			MSGBOX(IDS_UTIL_FAIL);
    }
    delete [] lpszModule;
}

void CMainFrame::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CWebFileOpenDlg fileDlg;
	fileDlg.m_bDirectOpenFile = m_bDirectOpenFile;
	fileDlg.m_bOpenInNew = m_bOpenInNew;
	
	if(fileDlg.DoModal() == IDOK )
	{
		m_bDirectOpenFile = fileDlg.m_bDirectOpenFile;
		m_bOpenInNew = fileDlg.m_bOpenInNew;
		//
		if (fileDlg.m_strAddress.GetLength()<1)
			return;
		if (DoSpecialUrl(fileDlg.m_strAddress))
			return;
		CChildFrame* tcf = (CChildFrame*)MDIGetActive();
		//create new if not exist
		if(tcf==NULL || fileDlg.m_bOpenInNew)
			tcf = NewChildWindow(0);
		
		if (VALID_TCF(tcf))
		{
			tcf->m_pView->m_lpszUrl=fileDlg.m_strAddress;
			tcf->m_pView->Navigate2(fileDlg.m_strAddress, 0, NULL);
		}
	}
}

void CMainFrame::OnViewToolbartextlabel() 
{
	// TODO: Add your command handler code here
	if(m_nTextLabel==1)
		m_nTextLabel = 0;
	else
		m_nTextLabel = 1;
	InitToolBar();
}

void CMainFrame::OnUpdateViewToolbartextlabel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_nTextLabel==1);
}

void CMainFrame::OnChangeProxy(UINT nID)
{
	int i = nID- B_PROXY;
	CString old = m_strCurProxy;
	CProxyList* pl;
	int index;
	CProxyItem* pi = m_aProxyCategories.FindIndex(i, &pl, &index);
	if(pi->index == i)
	{
		if (m_bEnProxy && m_strCurProxy==pi->m_strProxy)// && m_strCurProxyName==pi->m_strProxyName)
			m_bEnProxy = FALSE;
		else
		{
			m_bEnProxy = TRUE;
			m_strCurProxy = pi->m_strProxy;
			m_strCurProxyName = pi->m_strProxyName;
			m_strProxyByPass = pl->m_strProxyByPass;
		}
		UpdateProxy();
	}
}

void CMainFrame::OnChangeWebProxy(UINT nID)
{
	GET_ACTIVE
		
		int i; 
	CProxyList* pl;
	if(nID >= B_WEB_PROXY)
	{
		pl = m_aProxyCategories.GetWebProxyList();
		i = nID - B_WEB_PROXY;
	}
	else
	{
		pl = m_aProxyCategories.GetWebProxyList(TRUE);
		i= nID - B_TRANS;
	}
	if (pl==NULL)
		return;
	//
	if (tcf->m_pView->m_bEnWebProxy && tcf->m_pView->m_strWebProxy == pl->m_Proxies.GetAt(i)->m_strProxy)
		tcf->m_pView->m_bEnWebProxy = FALSE;
	else
	{
		tcf->m_pView->m_bEnWebProxy = TRUE;
		tcf->m_pView->m_strWebProxy = pl->m_Proxies.GetAt(i)->m_strProxy;
		m_strDefaultWebProxy = pl->m_Proxies.GetAt(i)->m_strProxy;
		if(m_dwWebProxyFrom == ID_OPTIONS_USE_WEBP)
		{
			//auto refresh
			if(!tcf->m_pView->m_lpszUrl.IsEmpty() && tcf->m_pView->m_lpszUrl!="about:blank")
			{
				tcf->m_pView->ToNavigate(tcf->m_pView->m_lpszUrl, 0, NULL, NULL, NULL, 0, TRUE);
			}
		}
		else if(m_dwWebProxyFrom == IDOK)
		{
			//start new address
			OnNewAddressEnter();				
			tcf->m_pView->m_bEnWebProxy = FALSE;
		}
	}
}

void CMainFrame::OnOptionAutotab() 
{
	// TODO: Add your command handler code here
	if(m_bAutoTab)
	{
		if(m_bMultiLineTab)
			m_wndTab.ModifyStyle(TCS_FIXEDWIDTH, TCS_MULTILINE);
		else
			m_wndTab.ModifyStyle(TCS_FIXEDWIDTH|TCS_MULTILINE, 0);
		m_bAutoTab = FALSE;
	}
	else
	{
		m_wndTab.ModifyStyle(/*TCS_MULTILINE*/ 0, TCS_FIXEDWIDTH);
		m_bAutoTab = TRUE;
	}
	PostMessage(WM_UPDATE_TAB,TRUE);
}

int CMainFrame::AdjustTabWidth(BOOL bNoReCalSize)
{
	CRect rect;
	int x, y, oldx;
	
	try{
		m_wndTab.GetItemRect(0, &rect);
		y = rect.Height();
		oldx=rect.Width();	
		if(y==0)
			y=TAB_HEIGHT;
		if(m_bAutoTab)
		{
			m_wndTab.GetWindowRect(&rect);
			if(!bNoReCalSize)
			{
				int c = m_wndTab.GetItemCount();
				if(c==0)
					x = m_nMaxTabWidth;
				else if(m_nTabStyle==2)
					x = rect.Width()/c-11;
				else
					x = (rect.Width()-5)/c;
				//
				if(x>m_nMaxTabWidth)
					x=m_nMaxTabWidth;
				else if(x<m_nMinTabWidth)
					x=m_nMinTabWidth;
				if(x != oldx)
				{
					CSize size(x,y);
					m_wndTab.SetItemSize(size);
				}
			}
			if(m_nTabStyle==2)
			{
				m_wndTab.SetWindowPos(&wndTop, 0,0, rect.Width()+1, rect.Height(), SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOSENDCHANGING|SWP_NOACTIVATE);
				m_wndTab.SetWindowPos(&wndTop, 0,0, rect.Width(),   rect.Height(), SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOSENDCHANGING|SWP_NOACTIVATE);
			}		
		}
	}catch(...){}
	
	return y;
}

void CMainFrame::OnAddThisFav(UINT nID)
{
	try{
		GET_ACTIVE
			
			int i = nID-B_ADDTO_FAV;
		POSITION pos = m_astrFavoriteFolders.FindIndex(i);
		if(pos!=NULL)
		{
			CString folder = m_astrFavoriteFolders.GetAt(pos);
			if(folder.Right(1)!="\\")
				folder += "\\";
			CString title, url, root;
			tcf->m_pView->GetLocationURL(url);
			CAddToThisFavDlg dlg;
			dlg.m_strFolder = folder;
			GetFavoriteFolder(root);
			dlg.m_strFolder = dlg.m_strFolder.Mid(root.GetLength());
			GET_TAB_TITLE(tcf,title);
			GET_URL_NAME(title)
				REPLACE_ERR_PATH_CHAR(title)
				dlg.m_strName = title;
			dlg.m_strURL = url;
			dlg.m_dwProperty = tcf->m_pView->m_dwProperty;
			if(dlg.DoModal() == IDOK)
			{
				title = dlg.m_strName; url = dlg.m_strURL;
				REPLACE_ERR_PATH_CHAR(title)
					if (title.GetLength())
					{
						CString strFileName = folder+title+".url";
						
						::WritePrivateProfileSection("InternetShortcut", NULL, strFileName);
						::WritePrivateProfileString(_T("InternetShortcut"), _T("URL"), url, strFileName);
						if (dlg.m_dwProperty != DEFAULT_PROPERTY)
						{
							char num[15];
							ultoa( dlg.m_dwProperty, num, 10);
							::WritePrivateProfileString(_T("InternetShortcut"), _T("Property"), num, strFileName);
						}
						if (dlg.m_nAutoRunCmd)
						{
							char num[15];
							ultoa( dlg.m_nAutoRunCmd, num, 10);
							::WritePrivateProfileString(_T("InternetShortcut"), _T("AutoRunCmd"), num, strFileName);
						}
						m_bFavDirty = TRUE;
					}
			}
		}
	}catch(...){}
}

void CMainFrame::OnF2(UINT nID) 
{
	// TODO: Add your command handler code here
	try{
		if(m_bUseSC)
		{
			CString url = m_strShortCuts[nID-ID_F_2];
			if(url=="" || url.IsEmpty())
				goto _MyAction;
			if (url=="javascript:")
				return;
			else if (IS_SCRIPT(url))
			{
				CChildFrame* tcf = (CChildFrame*)MDIGetActive();
				if (VALID_TCF(tcf))
					tcf->m_pView->ToNavigate(url, 0, NULL);
			}
			else if ( url.Find('|') == -1 )
				NewChildWindow(1,2,url);
			else
			{
				CString url2;
				int pos = 0;
				int pos2 = 0;
				int urlLength = url.GetLength(); 
				//
				BOOL bTemp = m_nNewSequence;
				m_nNewSequence = 0;
				while(pos<urlLength)
				{
					pos2 = url.Find('|',pos);
					if (pos2==-1)
						pos2 = urlLength ;
					url2 = url.Mid(pos, pos2-pos);
					url2.TrimLeft();
					url2.TrimRight();
					if (url2.GetLength()>0)
						NewChildWindow(1,2,url2);
					pos = pos2+1;
				}
				m_nNewSequence  = bTemp;
			}
			return;
		}
		
_MyAction:
		switch(nID)
		{
		case ID_F_2: OnTabPre(); break;
		case ID_F_3: OnTabNext();break;
		case ID_F_4: 
			if (m_bUseIeKey)
				OnAddrDrop();
			else
				PostMessage(WM_COMMAND, ID_FILE_CLOSE_1, 0);
			break;
		case ID_F_5: SendMessage(WM_COMMAND, ID_VIEW_REFRESH);break;
			//
		case ID_F_6: 
		case ID_F_7:
			{
				CChildFrame* tcf = (CChildFrame*)MDIGetActive();
				if (VALID_TCF(tcf))
				{
					if (nID==ID_F_6)
						tcf->m_pView->m_bFindPrev=TRUE;
					OnFindSearchBar();
					//
					if (GetFocus() == (CWnd*)m_wndSearch->GetEditCtrl())
						tcf->ViewSetFocus();
				}
				break;
			}
		case ID_F_8: m_wndAddress->SetFocus();break;
		case ID_F_9: OnAddrDrop();break;
		case ID_F_10: m_wndSearch->SetFocus();break;
		case ID_F_11: OnViewFullscreen();break;
		case ID_F_12: OnViewAutoHide();break;
		}
	}catch(...){}
}

void CMainFrame::OnShortcutUseSc() 
{
	// TODO: Add your command handler code here
	m_bUseSC = !m_bUseSC;
}

void CMainFrame::OnUpdateShortcutUseSc(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bUseSC);
}

void CMainFrame::OnShortcutAlias() 
{
	// TODO: Add your command handler code here
	m_bUseAlias = !m_bUseAlias;
}

void CMainFrame::OnUpdateShortcutAlias(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bUseAlias);
}

void CMainFrame::OnUpdateMsg(CCmdUI* pCmdUI) 
{
	if(m_bLastIsSecure != m_bIsSecure)
	{
		if(m_bIsSecure)
			m_wndStatusBar.GetStatusBarCtrl().SetIcon(0, m_hSecuIcon);
		else
			m_wndStatusBar.GetStatusBarCtrl().SetIcon(0, m_hNormalIcon);
		
		m_bLastIsSecure = m_bIsSecure;
	}
}

void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CMDIFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	// TODO: Add your message handler code here
	
	if(pPopupMenu == NULL)
		return;
	static CString strOpenAll, strAddFav;
	
	try
	{
		MENUITEMINFO mii;
		mii.fMask = MIIM_DATA|MIIM_TYPE;    // get the state of the menu item
		mii.dwTypeData = NULL;
		mii.cch = 0;
		mii.dwItemData = NULL;
		mii.cbSize = sizeof(mii);
		pPopupMenu->GetMenuItemInfo(0, &mii, TRUE); 
		CMenuData* pmd = (CMenuData*)(mii.dwItemData);
		if(mii.fType & MFT_OWNERDRAW && pmd!=NULL)
		{
			//get the ini flag
			if(!pmd->m_bIni)
			{
				//get path
				if(pmd->m_szMenuText!=NULL)
				{
					int nFavs, fs;
					//no use do this, for that InitFav have destroydata,
					//DestroyMenuData(pPopupMenu->m_hMenu,0);
					pPopupMenu->DeleteMenu(0, TRUE);
					m_nFavSize = BuildFavoritesMenu(pmd->m_szMenuText, m_nFavSize, pPopupMenu, 0, FALSE, nFavs);
					
					//add open all
					if(m_bDisplayOpenAll || m_bDisplayAddFav)
					{
						if(nFavs || m_bDisplayAddFav)
							MenuInsertSeparator(pPopupMenu,0);//pPopupMenu->InsertMenu(0,MF_BYPOSITION | MF_SEPARATOR);
						fs=m_astrFavoriteFolders.GetCount();
						m_astrFavoriteFolders.AddTail(pmd->m_szMenuText);
					}	
					if(m_bDisplayOpenAll)
					{
						if(nFavs)
						{
							LOADSTR(strOpenAll ,IDS_OPEN_ALL_FAV);
							MenuInserOwnerItem(pPopupMenu,0,MF_BYPOSITION| MF_STRING ,B_OPEN_FAV+fs, strOpenAll,52);
						}
					}
					if(m_bDisplayAddFav)
					{
						LOADSTR(strAddFav ,IDS_ADDFAV);
						MenuInserOwnerItem(pPopupMenu,0,MF_BYPOSITION| MF_STRING ,B_ADDTO_FAV+fs, strAddFav,51);
					}

					//gf 不应该有这个吧
// 					delete pmd;
// 					pmd=NULL;
				}
			}
		}
	}catch(...){ASSERT(false);}
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	try{
		if (g_bRegisterHotKeyOk || m_bShowTrayIcon)
		{
			if ( (nID==SC_MINIMIZE && m_bMinimizeToTray) || (nID==SC_CLOSE && m_bCloseToTray) )
			{
				if (nID==SC_CLOSE && m_bCloseAllWhenCloseToTray)
					OnFileCloseall(); 
				//not hide at once, for that update tab will redraw win
				PostMessage(WM_COMMAND, ID_FILE_HIDE);	//OnFileHide();
				return;
			}
		}
		//
		BOOL bMonitorVisible = IsMonitorVisible();
		if(nID == SC_RESTORE)
		{
			if(IsIconic())
			{
				BOOL bMax;
				CChildFrame* tcf = (CChildFrame*)MDIGetActive(&bMax);
				if(tcf!=NULL && tcf->m_bProtect)
				{
					CInputPassword dlg;
					BOOL validate = FALSE;
					if(dlg.DoModal()==IDOK)
					{
						if(dlg.m_strPassword == tcf->m_strPsw)
							validate = TRUE;
						else
							MSGBOX(IDS_PASSWORD_ERROR);
					}
					if(!validate)
					{
						return;
					}
				}
			}
			CMDIFrameWnd::OnSysCommand(nID, lParam);
		}
		else
			CMDIFrameWnd::OnSysCommand(nID, lParam);
		
		if(nID == SC_MINIMIZE)//! must reshow monitor
		{
			if (m_pMonitor && bMonitorVisible)
			{
				m_pMonitor->ShowWindow(SW_SHOW);
				SetForegroundWindow();
			}
		}
	}catch(...){}
}

void CMainFrame::OnNotifyTray(WPARAM wParam, LPARAM lParam)
{
	try{
		
		switch(lParam)
		{
		case WM_LBUTTONDOWN:
			OnHotKey(0,0);
			break;
		case WM_RBUTTONDOWN:
			{
				CPoint pos;
				GetCursorPos(&pos);
				
				SetForegroundWindow();
				CMenu* pPopup = m_TrayMenu.GetSubMenu(0);
				pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pos.x,pos.y,AfxGetMainWnd());
			}
			break;
		}
		
	}catch(...){ASSERT(false);}
}

void CMainFrame::OnTabPre() 
{
	// TODO: Add your command handler code here
	try{
		int nTabID = m_wndTab.GetCurSel();
		nTabID --;
		if(nTabID<0)
			nTabID = m_nWin - 1;
		if(nTabID>=0)
		{
			TCITEM TabCtrlItem;
			TabCtrlItem.mask = TCIF_PARAM;
			
			m_wndTab.GetItem(nTabID, &TabCtrlItem);
			CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
			if(tcf!=NULL)
			{
				MDIActivate(tcf);
				m_wndTab.SetCurSel(nTabID);
			}
		}
	}catch(...){}
}

void CMainFrame::OnTabNext() 
{
	// TODO: Add your command handler code here
	try{
		int nTabID = m_wndTab.GetCurSel();
		nTabID ++;
		if(nTabID>=0)
		{
			TCITEM TabCtrlItem;
			TabCtrlItem.mask = TCIF_PARAM;
			TabCtrlItem.lParam = NULL;
			
			m_wndTab.GetItem(nTabID, &TabCtrlItem);
			CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
			if(tcf == NULL)
			{
				nTabID = 0;
				TabCtrlItem.mask = TCIF_PARAM;
				TabCtrlItem.lParam = NULL;
				
				m_wndTab.GetItem(nTabID, &TabCtrlItem);
				tcf = (CChildFrame*)TabCtrlItem.lParam;
			}
			
			if(tcf!=NULL)
			{
				MDIActivate(tcf);
				m_wndTab.SetCurSel(nTabID);
			}
		}
	}catch(...){}
}

void CMainFrame::OnOptionsUrlinstatus() 
{
	// TODO: Add your command handler code here
	g_bDisScrollText = !g_bDisScrollText;
}

void CMainFrame::OnUpdateOptionsUrlinstatus(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(g_bDisScrollText);
}

CString CMainFrame::GetMenuItemUrl(UINT nItemID, HMENU hSysMenu,BOOL bRealUrl)
{
	CString strUrl;
	MENUITEMINFO mii; 
	
	mii.fMask = MIIM_DATA|MIIM_TYPE;    // get the state of the menu item
	mii.dwTypeData = NULL;
	mii.cch = 0;
	mii.dwItemData = NULL;
	mii.cbSize = sizeof(mii);
	::GetMenuItemInfo(hSysMenu, nItemID, FALSE, &mii); 
	CMenuData* pmd = (CMenuData*)(mii.dwItemData);
	if(mii.fType & MFT_OWNERDRAW && pmd!=NULL)
		strUrl = pmd->m_szURL;
	else
		strUrl.Empty();
	//
	if(strUrl.IsEmpty())
	{
		GetMenu()->GetMenuItemInfo(nItemID, &mii, FALSE); 
		pmd = (CMenuData*)(mii.dwItemData);
		if(mii.fType & MFT_OWNERDRAW && pmd!=NULL)
			strUrl = pmd->m_szURL;
	}
	//get url
	if(bRealUrl && IS_FAVURL(strUrl))
	{
		TCHAR	buf[INTERNET_MAX_PATH_LENGTH];
		::GetPrivateProfileString(_T("InternetShortcut"), _T("URL"), _T(""), buf, INTERNET_MAX_PATH_LENGTH, strUrl);
		strUrl= buf;
	}
	return strUrl;
}

void CMainFrame::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu) 
{
	try
	{
		
		CMDIFrameWnd::OnMenuSelect(nItemID, nFlags, hSysMenu);
		//add for menubar arrow key
		m_wndMenuBar.OnMenuSelect(hSysMenu,nItemID);
		
		CString strUrl;
		if(nItemID>=B_FAV && nItemID <= E_FAV)
		{
			//some chev bar drop down menu item can not get item url, save url at here
			if(nItemID-B_FAV<m_nFavSize && hSysMenu!=NULL)
				strUrl = g_strMenuUrl = GetMenuItemUrl(nItemID,hSysMenu);
		}
		else if(nItemID>=B_GROUP && nItemID <= E_GROUP)
		{
			if(nItemID-B_GROUP<g_nGroupSize && hSysMenu!=NULL)
				strUrl = GetMenuItemUrl(nItemID,hSysMenu);
		}
		else if(nItemID>=B_UNDO && nItemID <= B_UNDO+MAX_UNDO)
		{
			strUrl = m_strUndoURL.GetAt(nItemID - B_UNDO);
		}
		else if(nItemID>=B_HISTORY && nItemID <= E_HISTORY)
			strUrl = GetMenuItemUrl(nItemID,hSysMenu);
		else
			return;
		
		SetMessageText(strUrl);
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnSwitchWins(UINT nID)
{
	try{
		int n = nID-ID_C_1;
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		m_wndTab.GetItem(n, &TabCtrlItem);
		CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
		if(tcf!=NULL)
		{
			MDIActivate(tcf);
			m_wndTab.SetCurSel(n);
		}
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnRtabStop() 
{
	// TODO: Add your command handler code here
	StopView(m_nRTabID);
}

void CMainFrame::OnRtabRefresh() 
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	
	if(VALID_TCF(tcf))
	{
		tcf->m_pView->PowerRefresh();//Refresh2(REFRESH_COMPLETELY);
	}
}

void CMainFrame::OnViewProtect() 
{
	// TODO: Add your command handler code here
	g_bRTab = 0;
	OnRtabProtect();
}

void CMainFrame::OnRtabProtect() 
{
	// TODO: Add your command handler code here
	CChildFrame *tcf;
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	if (g_bRTab)
	{
		m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
		tcf = (CChildFrame*)TabCtrlItem.lParam;
	}
	else
	{
		tcf = (CChildFrame*)MDIGetActive();
		m_nRTabID = FindTab(tcf);
	}
	
	if(tcf != NULL)
	{
		CMyIE9View* pView = tcf->m_pView;
		CInputPassword dlg;
		if(!tcf->m_bProtect)
		{
			dlg.m_strPassword = m_strProtectPwd;
			dlg.m_bRemPsw = m_bRemPsw;
			
			if(dlg.DoModal()==IDOK && dlg.m_strPassword!="")
			{
				tcf->m_strPsw = dlg.m_strPassword;
				tcf->m_bProtect =TRUE;
				m_bRemPsw = dlg.m_bRemPsw;
				m_nProtectNum++;
				if(dlg.m_bRemPsw)
					m_strProtectPwd = dlg.m_strPassword;
				
				if(pView->m_nProgress<0)
				{
					pView->m_nOlderIcon = pView->m_nOldIcon;
					SetTabIcon( TI_PROTECT, m_nRTabID, pView );
					//update tab bar
					if(m_nTabStyle==2)
						PostMessage(WM_UPDATE_TAB);
				}
			}
		}
		else
		{
			if(dlg.DoModal()==IDOK)
			{
				if(tcf->m_strPsw == dlg.m_strPassword)
				{
					tcf->m_bProtect =FALSE;
					m_nProtectNum--;
					m_bRemPsw = dlg.m_bRemPsw;
					if(dlg.m_bRemPsw)
						m_strProtectPwd = dlg.m_strPassword;
					
					if(pView->m_nProgress<0)
					{
						SetTabIcon( pView->m_nOlderIcon, m_nRTabID, pView );
						//update tab bar
						if(m_nTabStyle==2)
							PostMessage(WM_UPDATE_TAB);
					}
				}
				else
					MSGBOX(IDS_PASSWORD_ERROR);
			}
		}
	}	
}

void CMainFrame::OnActivateWindow(WPARAM wParam, LPARAM lParam)
{
	try{
		
		MDIActivate((CChildFrame*)lParam);
	}catch(...){ASSERT(false);}
}

void CMainFrame::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	try{
		WORD wNumFilesDropped = DragQueryFile(hDropInfo, -1, NULL, 0);
		CString firstFile="", tmp;
		char buf[2048];
		//
		for (WORD x = 0 ; x < wNumFilesDropped; x++) 
		{
			WORD wPathnameSize = DragQueryFile(hDropInfo, x, NULL, 0);
			char * npszFile = (char *) LocalAlloc(LPTR, wPathnameSize += 1);
			if (npszFile == NULL)
				continue;
			DragQueryFile(hDropInfo, x, npszFile, wPathnameSize);
			firstFile=npszFile;
			tmp = firstFile.Right(4);
			tmp.MakeLower();
			if(tmp == _T(".lnk"))
			{
				if(ResolveLink(firstFile, buf))
					firstFile = buf;
			}
			else if(IS_FAVURL(tmp))
			{
				::GetPrivateProfileString(_T("InternetShortcut"), _T("URL"),
					_T(""), buf, 2048, firstFile);
				firstFile = buf;
			}
			LocalFree(npszFile);
			if(PRESS_SHIFT)
			{
				CChildFrame* tcf = (CChildFrame*)MDIGetActive();
				if(tcf != NULL && tcf->m_pView != NULL)
				{
					tcf->m_pView->Navigate2(firstFile);
					tcf->m_pView->m_nProgress = 0;
				}
				else
					NewChildWindow(1,2, firstFile);
			}
			else
				NewChildWindow(1,2, firstFile,NULL,TRUE);
		}
		//
		DragFinish(hDropInfo);
	}catch(...){}
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	try{
		if(nIDEvent == m_nUpdateTimer)
		{
			// send WM_IDLEUPDATECMDUI to the main window
			if (m_hWnd != NULL && IsWindowVisible() && !IsIconic())
			{
				if(m_wndInstantBar.GetSafeHwnd()!=NULL)
				{
					if(m_wndInstantBar.IsVisible())
					{
						AfxCallWndProc(this, m_hWnd,
							WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);
						m_wndInstantBar.SendMessage(WM_IDLEUPDATECMDUI, TRUE, 0);
					}
				}
				if(m_nSwitch%2==0 && m_nState != WA_INACTIVE)
				{
					try{
						//update all
						m_wndToolBar.PostMessage(WM_IDLEUPDATECMDUI, TRUE, 0);
						m_wndStatusBar.PostMessage(WM_IDLEUPDATECMDUI, TRUE, 0);
						m_wndStatusBar.IdleUpdateUI();
						if(g_bVerbose && m_nSwitch==40)
						{
							SaveLastVisit(1);
							g_bVerbose = FALSE;
						}
				}catch(...){}
				}
				else
				{
					//only status bar
					m_wndStatusBar.PostMessage(WM_IDLEUPDATECMDUI, TRUE, 0);
				}
				m_nSwitch++;
				if(m_nSwitch > 40)
					m_nSwitch = 0;
			}
		}	
		else if(nIDEvent == m_nAnimateIconTimer )
		{
			if(m_bAnimatIcon)
			{
				//animate icon
				if(m_nNeedIB==6)
				{
					BOOL bBrowsing = IsBrowsing();
					m_nNeedIB = 0;
					if(!bBrowsing && m_bLastIB)
						m_animIcon.ShowFirstImage();
					m_bLastIB = bBrowsing;
				}
				else
					m_nNeedIB ++;
				if(m_bLastIB)
					m_animIcon.ShowNextImage();
			}
		}
		else if(nIDEvent == m_nTimerID)
		{
			if(m_astrOpenAll.IsEmpty())
			{
				KillTimer(m_nTimerID);
				m_nTimerID = 0;
				m_nNewSequence = g_nOldNewSequence;
			}
			else
			{
				CString strTemp;
				int max = 6;
				if(m_nInterval)
					max = 1;
				try
				{
					while(!m_astrOpenAll.IsEmpty() && max>0)
					{
						strTemp= m_astrOpenAll.GetHead();
						m_astrOpenAll.RemoveHead();
						//is exe?
						if(strTemp.Right(4)==".EXE" || strTemp.Right(4)==".exe")
							ShellExecute(NULL, "open", strTemp , "", NULL, SW_SHOW );
						else
							NewChildWindow(1,2,strTemp);
						max --;
					}
			}catch(...){}
			
			if(m_astrOpenAll.IsEmpty())
			{
				KillTimer(m_nTimerID);
				m_nTimerID = 0;
				m_nNewSequence = g_nOldNewSequence;
			}
			}
		}
		
		CMDIFrameWnd::OnTimer(nIDEvent);
		
	}catch(...){}
}

void CMainFrame::OnFileStopOpenall() 
{
	// TODO: Add your command handler code here
	m_astrOpenAll.RemoveAll();
}

void CMainFrame::OnUpdateFileStopOpenall(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_nTimerID ? TRUE : FALSE);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	try{
		
		// TODO: Add your specialized code here and/or call the base class
		cs.x = 0;
		cs.y = 0;
		cs.cx = GetSystemMetrics(SM_CXSCREEN);//800;
		cs.cy = GetSystemMetrics(SM_CYSCREEN); //600;
		
		return CMDIFrameWnd::PreCreateWindow(cs);
		
	}catch(...){ASSERT(false); return FALSE;}
	
}

void CMainFrame::OnFileHide() 
{
	// TODO: Add your command handler code here
	OnHotKey(0,0);
}

//wParam=1 means shellopen, no use input password
void CMainFrame::OnHotKey(WPARAM wParam,LPARAM)
{
	try{
		if(!IsWindowVisible() || IsIconic())
		{
			ShowWindow(SW_SHOW);
			SetForegroundWindow();
			BOOL bMax;
			CChildFrame* tcf = (CChildFrame*)MDIGetActive(&bMax);
			if(tcf!=NULL && tcf->m_bProtect && wParam==0)
			{
				CInputPassword dlg;
				BOOL validate = FALSE;
				if(dlg.DoModal()==IDOK)
				{
					if(dlg.m_strPassword == tcf->m_strPsw)
						validate = TRUE;
					else
						MSGBOX(IDS_PASSWORD_ERROR);
				}
				if(!validate)
				{
					//can not hide direct or onhokey, 
					goto _Hide;//ShowWindow(SW_HIDE);//OnHotKey(0,0);
				}
			}
			ShowWindow(SW_RESTORE);
		}
		else
		{
_Hide:
		if (g_bRegisterHotKeyOk || m_bShowTrayIcon || m_pMonitor->IsWindowVisible())
		{
			BOOL bMonitorVisible = IsMonitorVisible();
			ShowWindow(SW_MINIMIZE);
			ShowWindow(SW_HIDE);
			if (m_pMonitor && bMonitorVisible)
				m_pMonitor->ShowWindow(SW_SHOW);
			SetForegroundWindow();
		}
		}
	}catch(...){}
}

void CMainFrame::OnViewFolder() 
{
	// TODO: Add your command handler code here
	try{
		CString str,str1;
		if(m_wndInstantBar.GetSafeHwnd()==NULL)
			BuildExpBar("");
		BOOL bNeedInI=false;
		if(m_pwndExpObj == NULL)
			bNeedInI = true;
		else
			LOADSTR(str ,IDS_TITLE_SHELL);
		m_wndInstantBar.GetWindowText(str1);
		if(bNeedInI || str1!=str)
		{
			RemoveExp();
			try
			{
				m_pwndExpObj = new CShellTree;
			}
			catch(...)
			{
				m_pwndExpObj = NULL;
				return;
			}
			m_pwndExpObj->Create(&m_wndInstantBar);
			m_pwndExpObj->Initialize();
			str = m_pwndExpObj->GetTitle();
			
			CRect rect;
			m_wndInstantBar.GetClientRect(&rect);
			m_wndInstantBar.SendMessage(WM_SIZE, 0, ((rect.Height())<<16)+rect.Width());
			
			m_wndInstantBar.SetWindowText(str);
			//if(!m_wndInstantBar.IsVisible())
			ShowControlBar(&m_wndInstantBar, TRUE, FALSE);
		}
		else
		{
			ShowControlBar(&m_wndInstantBar, !m_wndInstantBar.IsVisible(), FALSE);
		}
	}catch(...){}
}

void CMainFrame::OnOptionsOpenlinkinnewwindow() 
{
	// TODO: Add your command handler code here
	g_bLinkInNewWindow = !g_bLinkInNewWindow;
}

void CMainFrame::OnUpdateOptionsOpenlinkinnewwindow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(g_bLinkInNewWindow);
}

void CMainFrame::OnScRll(WPARAM wParam,LPARAM lParam)
{
	GET_ACTIVE
		tcf->m_pView->m_wndBrowser.SendMessage(MSWHELL_ROLLMSG, wParam, lParam);
}

BOOL CMainFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	try
	{
		
		// TODO: Add your message handler code here and/or call default
		CChildFrame* tcf = (CChildFrame*)MDIGetActive();
		if (!tcf || !tcf->m_pView)
			return CMDIFrameWnd::OnMouseWheel(nFlags, zDelta, pt);
		
		LPDISPATCH pDisp = NULL;
		IHTMLDocument2 *pHTMLDoc = NULL;
		IHTMLWindow2 *pHTMLWnd = NULL;
		try
		{
			pDisp = tcf->m_pView->GetHtmlDocument();
			if( pDisp )
			{
				if (SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
				{
					if(SUCCEEDED(pHTMLDoc->get_parentWindow( &pHTMLWnd )))
					{
						if(zDelta>0)
						{
							pHTMLWnd->scrollBy(0, -20);
							pHTMLWnd->scrollBy(0, -30);
						}
						else
						{
							pHTMLWnd->scrollBy(0, 20);
							pHTMLWnd->scrollBy(0, 30);
						}
					}
				}
			}
	}catch(...){}
	
	try{
		if(pHTMLWnd != NULL)
			pHTMLWnd->Release();
		if(pHTMLDoc != NULL)
			pHTMLDoc->Release();
		if(pDisp != NULL)
			pDisp->Release();
		
		return CMDIFrameWnd::OnMouseWheel(nFlags, zDelta, pt);
	}
	catch(...)
	{
		return TRUE;
	}
	
	}
	
	catch(...)
	{
		return TRUE;
	}
}

void CMainFrame::OnEditUndoclose() 
{
	try
	{
		
		//		// TODO: Add your command handler code here
		//		int c =	m_nUndoEnd-1;
		//		if(c<0)
		// 			c += MAX_UNDO;
		
		if(!m_strUndoURL.IsEmpty() && m_strUndoURL.GetHead() != "")
		{
			NewChildWindow(1,2,m_strUndoURL.RemoveHead() );

			m_strUndoName.RemoveHead();
			
			UpdateUndoMenu();
		}
		
	}
	catch(...)
	{
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnUpdateEditUndoclose(CCmdUI* pCmdUI) 
{
	try
	{
		
		// TODO: Add your command update UI handler code here
		pCmdUI->Enable(m_strUndoName.IsEmpty() ? FALSE : TRUE);
	}
	catch(...)
	{
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnEditEdit() 
{
	try
	{
		
		// TODO: Add your command handler code here
		GET_ACTIVE
			
			static TCHAR    sz[MAX_PATH];
		HKEY            hKey;
		DWORD           dwSize = MAX_PATH;
		
		// find out from the registry where the editor are located.
		if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Default HTML Editor\\shell\\edit\\command"), &hKey) != ERROR_SUCCESS)
		{
			if(RegOpenKey(HKEY_CLASSES_ROOT, _T("InternetShortcut\\shell\\edit\\command"), &hKey) != ERROR_SUCCESS)
			{
				//use view source instead
				ExecCmdTarget(HTMLID_VIEWSOURCE, tcf->m_pView);
				return ;
			}
		}
		dwSize = sizeof(sz);
		RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
		RegCloseKey(hKey);
		if(dwSize>0)
			LaunchEditor(sz, tcf->m_pView->m_lpszUrl);
		else//use view source instead
			ExecCmdTarget(HTMLID_VIEWSOURCE, tcf->m_pView);
		
		
	}
	catch(...)
	{
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::FavMenuAddFolder(LPCSTR path, LPCSTR root, BOOL FirstLevel, BOOL IsLink, CMenu* pMenu, int& nEndPos)
{
	int iString;
	TBBUTTON tbb;
	static CString	strOpenAll, strAddFav;
	
	if(strlen(path)==0)
		return;
	
	BOOL rec = FALSE;
	m_strMenuUrl = root;
	m_strMenuUrl+= path;
	m_strMenuUrl+= "\\";
	CString strLinkPath = m_strLinkPath;
	DO_RELATIVE_URL(strLinkPath);
	//only in or is link folder we will recurse
	if(m_strMenuUrl.Find(strLinkPath)==0)
		rec = TRUE;
	//
	int	nSubFavs = 0, fs;
	CMenu * pSubMenu = NULL;
	try
	{
		pSubMenu = new CMenu;
	}
	catch(...)
	{
		return;
	}
	
	pSubMenu->CreatePopupMenu();
	if(rec)
	{
		// call this function recursively.
		m_strMenuUrl = root;
		m_strMenuUrl += path;
		nEndPos = BuildFavoritesMenu(m_strMenuUrl, nEndPos, pSubMenu, 0, FALSE, nSubFavs);
	}
	else
	{
		try
		{
			CMenuData * md= new CMenuData;
			md->m_pImgList = &m_wndMenuBar.m_ImgList;
			md->m_nBitmap = -1;
			SetMenuText(m_strMenuUrl,md);
			md->m_bIni = FALSE;
			
			pSubMenu->AppendMenu(MF_OWNERDRAW|MF_DISABLED|MF_GRAYED , 0, (char*)md );
		}catch(...){}
	}
	
	try{
		// only intert a submenu if there are in fact .URL files in the subdirectory
		m_strMenuText = path;
		AdjustMenuWidth(m_strMenuText);
		
		m_strMenuUrl = root;
		m_strMenuUrl += path;
		m_strMenuUrl += "\\";
		
		CMenuData * md= new CMenuData;
		md->m_pImgList = &m_wndMenuBar.m_ImgList;
		md->m_nBitmap = 0;
		
		SetMenuText(m_strMenuText, md);
		//pMenu->InsertMenu(nPos+nFixMenu, MF_BYPOSITION | MF_POPUP | MF_OWNERDRAW, (UINT)pSubMenu->m_hMenu , (char*)md );
		pMenu->AppendMenu(MF_BYPOSITION | MF_POPUP | MF_OWNERDRAW, (UINT)pSubMenu->m_hMenu , (char*)md );
		
		if(IsLink)
		{
			memset(&tbb, 0, sizeof(tbb));
			iString = m_LinkBar.GetToolBarCtrl().AddStrings(m_strMenuText);
			tbb.iString = iString;
			tbb.fsState = TBSTATE_ENABLED;
			tbb.fsStyle = TBSTYLE_AUTOSIZE;
			tbb.iBitmap = 0;
			if(g_bCompactFavBar)
				tbb.iBitmap = -1;
			tbb.idCommand = ID_LINK_POPUP;
			tbb.dwData = (UINT)pSubMenu->m_hMenu;
			VERIFY(m_LinkBar.GetToolBarCtrl().AddButtons(1, &tbb));
			g_astrLinks.Add(path);
		}
		//add favorites bar
		if(FirstLevel)
		{
			memset(&tbb, 0, sizeof(tbb));
			iString = m_FavBar.GetToolBarCtrl().AddStrings(m_strMenuText);
			tbb.iString = iString;
			tbb.fsState = TBSTATE_ENABLED;
			tbb.fsStyle = TBSTYLE_AUTOSIZE;
			tbb.iBitmap = 0;
			if(g_bCompactFavBar)
				tbb.iBitmap = -1;
			tbb.idCommand = ID_FAV_POPUP;
			tbb.dwData = (UINT)pSubMenu->m_hMenu;
			VERIFY(m_FavBar.GetToolBarCtrl().AddButtons(1, &tbb));
			g_astrFavs.Add(path);
		}
		//
		if(rec)
		{
			LOADSTR(strOpenAll ,IDS_OPEN_ALL_FAV);
			LOADSTR(strAddFav ,IDS_ADDFAV);
			//add open all favorites menu
			if(m_bDisplayOpenAll || m_bDisplayAddFav)
			{
				if(nSubFavs || m_bDisplayAddFav)
					MenuInsertSeparator(pSubMenu,0);//pSubMenu->InsertMenu(0,MF_BYPOSITION | MF_SEPARATOR);
				fs=m_astrFavoriteFolders.GetCount();
				m_strMenuUrl = root;
				m_astrFavoriteFolders.AddTail(m_strMenuUrl + path);
			}
			if(m_bDisplayOpenAll)
			{
				if(nSubFavs)
					MenuInserOwnerItem(pSubMenu,0,MF_BYPOSITION| MF_STRING ,B_OPEN_FAV+fs, strOpenAll,52);
			}
			if(m_bDisplayAddFav)
			{
				//add fav
				//pSubMenu->InsertMenu(0,MF_BYPOSITION| MF_STRING ,B_ADDTO_FAV+fs, strAddFav);
				MenuInserOwnerItem(pSubMenu,0,MF_BYPOSITION| MF_STRING ,B_ADDTO_FAV+fs, strAddFav,51);
			}
		}
		pSubMenu->Detach();
		delete pSubMenu;
		
	}catch(...){}
}

void CMainFrame::FavMenuAddURL(LPCSTR path, LPCSTR root, BOOL FirstLevel, BOOL IsLink, CMenu *pMenu, int nPos)
{
	TBBUTTON tbb;
	static TCHAR	buf[INTERNET_MAX_URL_LENGTH];
	buf[0]='\0';
	CMenuData		*md = NULL;
	md= new CMenuData;
	if (md==NULL)
		return;
	
	try{
		md->m_pImgList = &m_wndMenuBar.m_ImgList;
		md->m_nBitmap  = 1;
		
		//menu url
		m_strMenuText = root;
		m_strMenuText += path;
		SetURL(m_strMenuText,md);
		
		{//gf
			if (IS_FAVURL(m_strMenuText))
				::GetPrivateProfileString(_T("InternetShortcut"), _T("URL"), _T(""), buf, INTERNET_MAX_PATH_LENGTH, m_strMenuText);
			
			
			CString strUrl = buf;
			if ( IS_HTTP(strUrl))
			{
				CString strRoot = _StringGetTopUrl(strUrl);
				
				//get favicon from cache
				int nIcon = CGf_IE::FindCacheFavIcon(strRoot, md->m_pImgList );
				if (FI_NOT_EXIST != nIcon )
				{
					md->m_nBitmap = nIcon;
				}
				
			}
			
		}
		
		//menu text
		m_strMenuUrl = path;
		m_strMenuUrl = m_strMenuUrl.Left(m_strMenuUrl.GetLength()-4);


		//gffs AdjustMenuWidth(m_strMenuUrl);
		CString strTitleShow=m_strMenuUrl;
		if ( (FirstLevel && m_bFavShowUrl)||IsLink)
		{
			AdjustMenuWidth(strTitleShow, m_nMenuWidth);
		}
		else
		{

			AdjustMenuWidth(strTitleShow);
		}
		

		//SetMenuText(m_strMenuUrl, md);
		SetMenuText(strTitleShow, md);
		pMenu->AppendMenu ( MF_ENABLED|MF_OWNERDRAW, B_FAV + nPos , (char*)md );
		
		if(IsLink)
		{
			memset(&tbb, 0, sizeof(tbb));
			int iString = m_LinkBar.GetToolBarCtrl().AddStrings(strTitleShow); //gffs m_strMenuUrl);
			tbb.iString = iString;
			tbb.fsState = TBSTATE_ENABLED;
			tbb.fsStyle = TBSTYLE_AUTOSIZE;
			tbb.iBitmap = 1; //gf md->m_nBitmap;
			tbb.idCommand = B_FAV + nPos;
			tbb.dwData = 0;
			
			if(g_bCompactFavBar)
				tbb.iBitmap = -1;
			VERIFY(m_LinkBar.GetToolBarCtrl().AddButtons(1, &tbb));
			
			g_astrLinks.Add(m_strMenuUrl); //m_strMenuUrl);
			//
			
			CString strTip="";
			if (m_strMenuUrl.GetLength()>m_nMenuWidth)
			{
				strTip=m_strMenuUrl+"\r\n";
			}
			else
			{
			  //wu
			}

			if (IS_FAVURL(m_strMenuText))
			{
				//gf ::GetPrivateProfileString(_T("InternetShortcut"), _T("URL"), _T(""), buf, INTERNET_MAX_URL_LENGTH, m_strMenuText);
				strTip+=buf;
			}
			else
			{
				strTip+=m_strMenuText;
			}

			m_LinkBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(strTip, &m_LinkBar, B_FAV+nPos);
		}
		
		if(FirstLevel && m_bFavShowUrl)
		{
			memset(&tbb, 0, sizeof(tbb));
			int iString = m_FavBar.GetToolBarCtrl().AddStrings(strTitleShow);
			tbb.iString = iString;
			tbb.fsState = TBSTATE_ENABLED;
			tbb.fsStyle = TBSTYLE_AUTOSIZE;
			tbb.iBitmap = 1; //gf md->m_nBitmap;
			tbb.idCommand = B_FAV + nPos;
			tbb.dwData = 0;
			if(g_bCompactFavBar)
				tbb.iBitmap = -1;
			
			
			VERIFY(m_FavBar.GetToolBarCtrl().AddButtons(1, &tbb));
			
			g_astrFavs.Add(m_strMenuUrl); //m_strMenuUrl);
			
			CString strTip="";
			if (m_strMenuUrl.GetLength()>m_nMenuWidth)
			{
				strTip=m_strMenuUrl+"\r\n";
			}
			else
			{
				//wu
			}
			
			if (IS_FAVURL(m_strMenuText))
			{
				strTip+=buf;
			}
			else
			{
				strTip+=m_strMenuText;
			}
			
			
			m_FavBar.GetToolBarCtrl().GetToolTips()->UpdateTipText(strTip, &m_FavBar, B_FAV+nPos);
		}
		
	}catch(...){ASSERT(false);}
}

void CMainFrame::OnToolsAutoscrollSpeed(UINT nID) 
{
	
	try
	{
		
		// TODO: Add your command handler code here
		GET_ACTIVE
			
			m_nDefSrSpeed = tcf->m_pView->m_nSrSpeed = nID - ID_TOOLS_AUTOSCROLL_SLOW+1;
		tcf->m_pView->ChangeSpeed(m_nDefSrSpeed);
		
		
	}
	catch(...)
	{
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnViewRefresh()
{
	GET_ACTIVE
		
		try{
		tcf->m_pView->PowerRefresh();
	}catch(...){}
}

void CMainFrame::OnViewStop()
{
	GET_ACTIVE
		int nTabID = FindTab(tcf);
	StopView(nTabID);
}

void CMainFrame::OnFilePrintpreview() 
{
	try
	{
		
		// TODO: Add your command handler code here
		GET_ACTIVE
			tcf->m_pView->ExecCommand(OLECMDID_PRINTPREVIEW);
		
	}catch(...){ASSERT(false);}
}

void CMainFrame::OnFilePrint() 
{
	try{
		
		// TODO: Add your command handler code here
		GET_ACTIVE
			tcf->m_pView->ExecCommand(OLECMDID_PRINT);
		
	}catch(...){ASSERT(false);}
}

void CMainFrame::ToolBarSetCheck(CCmdUI *pCmdUI, BOOL bCheck)
{
	try
	{
		if(m_strSkinName.IsEmpty() || pCmdUI->m_pMenu !=NULL)
			pCmdUI->SetCheck(bCheck);
		else
		{
			pCmdUI->SetCheck(0);
			CToolBar    *pToolBar = (CToolBar*)pCmdUI->m_pOther;
			if (pToolBar && pToolBar->GetToolBarCtrl().IsButtonPressed(pCmdUI->m_nID) != bCheck)
				pToolBar->GetToolBarCtrl().PressButton(pCmdUI->m_nID, bCheck);
		}
	}catch(...){}
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	m_nState = nState;
	//kill dialog
	try{
		if (nState || !m_bUseDialogFilter || m_nDialogFilterCount<1)
			return;
		m_nDialogFilterCount--;
		HWND hWin = pWndOther->GetSafeHwnd();
		if (hWin && ::GetParent(hWin)==this->GetSafeHwnd())
		{
			CHAR str[260];
			::GetClassName(hWin,str,256);
			if (strcmp(str,"Internet Explorer_TridentDlgFrame")==0)
			{
				::GetWindowText(hWin,str,256);
				if (strstr(str, " -- "))//cancel find dlg
				{
					pWndOther->ShowWindow(SW_HIDE);
					pWndOther->SendMessage(WM_CLOSE);
					GET_ACTIVE
						tcf->ViewSetFocus();
				}
			}
		}
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::LoadGroup()
{
	m_astrGroup.RemoveAll();
	
	char szItem[MAX_PATH];
	char *buf = _FileGetBuffer(theApp.m_strGroupPath+"order.txt");
	if (buf)
	{
		char *pEnd = buf + strlen(buf);
		char *p1=buf;
		char *p2;
		do
		{
			p2 = strstr(p1,"\r\n");
			if (!p2 || p1==p2)
				break;
			STRNCPY(szItem, p1, (p2-p1));
			m_astrGroup.Add(szItem);
			p1 = p2 + 2;
		}while(p1<pEnd);
	}
	
	int i=0;
	WIN32_FIND_DATA wfd;
	HANDLE          h;
	CString			psz = theApp.m_strGroupPath+"*.cgp";
	h = FindFirstFile(psz, &wfd);
	if(h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (buf)
			{
				sprintf(szItem, "%s\r\n", wfd.cFileName);
				if (!strstr(buf,szItem))
					m_astrGroup.Add(wfd.cFileName);
			}
			else
				m_astrGroup.Add(wfd.cFileName);
			i++;
		}
		while(FindNextFile(h, &wfd) && i<255);
	}
	FindClose(h);
	
	//
	FREENULL(buf);
}

void CMainFrame::GetGroupNameByIndex(int index, CString &strGroupName)
{
	strGroupName.Empty();
	
	int i=0;
	for (i=0; i<=m_astrGroup.GetUpperBound(); i++)
	{
		if (index == i)
		{
			strGroupName = m_astrGroup.GetAt(i);
			break;
		}
	}
}

void CMainFrame::InitGroups()
{
	LoadGroup();
	
	CMenu* pmenu = SMP_GROUP;
	if(pmenu == NULL)
		return;
	DestroyMenuData(pmenu->m_hMenu,4);
	while(pmenu->DeleteMenu(4, MF_BYPOSITION));
	
	int i=0;
	g_nGroupSize = 0;
	char tmp[5]; 
	CString strMenu;
	//
	for (i=0; i<=m_astrGroup.GetUpperBound(); i++)
	{
		sprintf(tmp,"%02d",i+1);//itoa(i+1, tmp, 10);
		strMenu = tmp;
		strMenu += ". "; 
		strMenu += m_astrGroup.GetAt(i);
		strMenu = strMenu.Left(strMenu.GetLength()-4);
		AdjustMenuWidth(strMenu);
		//
		CMenuData * md;
		md= new CMenuData;
		md->m_pImgList = &m_wndMenuBar.m_ImgMenu;
		md->m_nBitmap = 34;
		SetMenuText(strMenu, md);
		//
		if (m_bGroupMenuShowMember)
		{
			CMenu * pSubMenu = NULL;
			pSubMenu = new CMenu;
			pSubMenu->CreatePopupMenu();
			pmenu->AppendMenu( MF_POPUP | MF_OWNERDRAW, (UINT)pSubMenu->m_hMenu , (char*)md );
			MenuLoadGroupURL(pSubMenu,m_astrGroup.GetAt(i),i);
		}
		else
		{
			pmenu->AppendMenu ( MF_ENABLED|MF_OWNERDRAW, B_OPEN_GROUP + i , (char*)md );
		}
	}
	
	//
	AddMenuBreak(pmenu, 0);
	
	//update add to group menus
	pmenu = SMP_GROUP->GetSubMenu(2);
	MenuLoadGroupList(pmenu, 1);
}

void CMainFrame::OnOpenGroup(UINT nID)
{
	if (!_FileIsExist(theApp.m_strGroupPath))
	{
		MSGBOX(IDS_GROUP_NOT_EXIST);
		return;
	}
	if (nID<B_OPEN_GROUP)
		return;
	CString strGroupName;
	GetGroupNameByIndex(nID - B_OPEN_GROUP, strGroupName);
	if (strGroupName.GetLength())
	{
		if (m_bCloseAllBeforeNewGroup)
			CloseAllTabDirect();
		OpenGroup(strGroupName);
	}
}

void CMainFrame::OnOrggroup() 
{
	// TODO: Add your command handler code here
	COrgGroupDlg dlg;
	if(dlg.DoModal()==IDOK)
		InitGroups();
}

//type==1 means self draw
void CMainFrame::MenuLoadGroupList(CMenu *pMenu, int type)
{
	if(pMenu == NULL)
		return;
	
	DestroyMenuData(pMenu->m_hMenu,0);
	while(pMenu->DeleteMenu(0, MF_BYPOSITION));
	
	int i=0;
	char tmp[5]; 
	CString strMenu;
	for (i=0; i<=m_astrGroup.GetUpperBound(); i++)
	{
		sprintf(tmp,"%02d",i+1);
		strMenu = tmp;
		strMenu += ". "; 
		strMenu += m_astrGroup.GetAt(i);
		strMenu = strMenu.Left(strMenu.GetLength()-4);
		AdjustMenuWidth(strMenu);
		if (type==0)
			pMenu->AppendMenu( MF_ENABLED, B_ADDTO_GROUP + i , strMenu);
		else
			MenuAppendOwnerItem( pMenu, MF_ENABLED, B_ADDTO_GROUP + i , strMenu );
	}
	
	if(m_astrGroup.GetUpperBound()==0)
	{
		LOADSTR(strMenu ,IDS_EMPTY);
		if (type==0)
			pMenu->AppendMenu(MF_GRAYED, 0, strMenu);
		else
			MenuAppendOwnerItem(pMenu, MF_GRAYED, 0, strMenu);
	}
	else
		AddMenuBreak(pMenu, 0);//FALSE);
}

void CMainFrame::MenuLoadGroupURL(CMenu *pMenu, CString strItem, int index)
{
	//load group names
	if(pMenu == NULL)
		return;
	
	DestroyMenuData(pMenu->m_hMenu,0);
	while(pMenu->DeleteMenu(0, MF_BYPOSITION));
	
	CString str;
	CMenuData * md = NULL;
	
	LOADSTR(str ,IDS_ADD_GROUP);
	md= new CMenuData;
	md->m_pImgList = &(m_wndMenuBar.m_ImgMenu);
	md->m_nBitmap = 69;
	SetMenuText(str,md);
	pMenu->AppendMenu ( MF_ENABLED|MF_OWNERDRAW, B_ADDTO_GROUP + index , (char*)md );
	//
	LOADSTR(str ,IDS_OPEN_GROUP);
	md= new CMenuData;
	md->m_pImgList = &(m_wndMenuBar.m_ImgMenu);
	md->m_nBitmap = 70;
	SetMenuText(str,md);
	pMenu->AppendMenu ( MF_ENABLED|MF_OWNERDRAW, B_OPEN_GROUP + index , (char*)md );
	//
	MenuAppendSeparator(pMenu);
	//
	CString filename=theApp.m_strGroupPath + strItem;
	
	char state[10]="state",name[9]="name",url[8]="url",download[13]="download"; // x+5
	int i=0,nState;
	char tmp[INTERNET_MAX_PATH_LENGTH];
	BOOL r = TRUE;
	CString strName,strURL;
	while(r)
	{
		itoa(i, state+5, 10);
		itoa(i, name+4, 10);
		itoa(i, url+3, 10);
		itoa(i, download+8, 10);
		nState = ::GetPrivateProfileInt("Group", state, 1, filename);
		r = ::GetPrivateProfileString("Group", name, NULL, tmp, INTERNET_MAX_PATH_LENGTH, filename);
		if (r)
		{
			strName = tmp;
			r = ::GetPrivateProfileString("Group", url, NULL, tmp, INTERNET_MAX_PATH_LENGTH, filename);
			if (r)
			{
				AdjustMenuWidth(strName);
				strURL = tmp;
				//				
				CMenuData * md = NULL;
				md= new CMenuData;
				md->m_pImgList = &(m_wndMenuBar.m_ImgMenu);
				if (nState)
					md->m_nBitmap = 71;
				else
					md->m_nBitmap = 72;
				SetMenuText(strName,md);
				SetURL(strURL,md);
				md->m_dwProperty = ::GetPrivateProfileInt("Group", download, DEFAULT_PROPERTY, filename);
				pMenu->AppendMenu ( MF_ENABLED|MF_OWNERDRAW, B_GROUP+g_nGroupSize , (char*)md );
				
				g_nGroupSize++;
			}
		}
		i++;
	}
	if(!g_bSingleMenu)
		AddMenuBreak(pMenu, 0);
}

void CMainFrame::OpenGroup(LPCSTR gname)
{
	if(gname == NULL || strlen(gname)==0)
		return;
	
	try{
		CString filename = theApp.m_strGroupPath + gname;
		char state[10]="state",url[8]="url",download[13]="download"; // x+5
		int i=0,nState;
		CChildFrame* tcf ;
		
		//save the mewseq
		g_nOldNewSequence = m_nNewSequence;
		m_nNewSequence = 0;
		
		char tmp[INTERNET_MAX_PATH_LENGTH];
		BOOL r = TRUE;
		while(r)
		{
			itoa(i, state+5, 10);
			itoa(i, url+3, 10);
			itoa(i, download+8, 10);
			i++;//add before continue
			
			nState = ::GetPrivateProfileInt("Group", state, 1, filename);
			if (nState)
			{
				r = ::GetPrivateProfileString("Group", url, NULL, tmp, INTERNET_MAX_PATH_LENGTH, filename);
				if (r)
				{
					CString strURL = tmp;
					if (DoSpecialUrl(strURL))
						continue;
					//gf tcf = NewChildWindow(0);

					if (1==i)
					{
						tcf = NewChildWindow(0);
					}
					else
					{
						BOOL btmp=m_bActiveNewWindow;
						m_bActiveNewWindow=0;
						tcf = NewChildWindowInBack(1);
						m_bActiveNewWindow=btmp;
					}

					if(VALID_TCF(tcf))
					{
						DWORD dwProperty = ::GetPrivateProfileInt("Group", download, DEFAULT_PROPERTY, filename);
						if (dwProperty!=DEFAULT_PROPERTY)
							tcf->m_pView->SetSpecialProperty(dwProperty);
						tcf->m_pView->ToNavigate(strURL, 0, NULL);
					}
				}
			}
		}
		//
		m_nNewSequence = g_nOldNewSequence;
		
	}catch(...){}
}

void CMainFrame::AddtoFavorite(BSTR bstrURL, VARIANT *pTitle)
{
	IShellUIHelper* pShellUIHelper = NULL;
	try
	{
		HRESULT hr = CoCreateInstance(CLSID_ShellUIHelper, NULL, 
			CLSCTX_INPROC_SERVER, IID_IShellUIHelper, 
			(LPVOID*)&pShellUIHelper);
		if (SUCCEEDED(hr))
		{
			pShellUIHelper->AddFavorite(bstrURL, pTitle);
			m_bFavDirty = TRUE;
			//update favorites list
			if(m_pwndExpObj!=NULL && HasExpBar(IDS_TITLE_FAVORITE))
				m_pwndExpObj->Update(0);
		}
	}catch(...){ASSERT(false);}
	
	try{
		if(pShellUIHelper)
			pShellUIHelper->Release();
	}catch(...){}
}

void CMainFrame::OnWindowList(UINT nID)
{
	try{
		int i = nID-B_WINDOW;
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		m_wndTab.GetItem(i, &TabCtrlItem);
		CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
		if(tcf!=NULL)
		{
			MDIActivate(tcf);
			m_wndTab.SetCurSel(i);
		}
	}catch(...){}
}

void CMainFrame::SaveLastVisit(BOOL bVisiting)
{
	//save last visited urls
	try{
		CChildFrame* tcf;
		int n = m_wndTab.GetItemCount();
		m_nWin = n;
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		
		char state[10]="state",name[9]="name",url[8]="url"; // x+5
		char num[10];
		int bRunning;
		bRunning = theApp.GetProfileInt("Settings", "Running", 0);
		CString filename;
		if (bVisiting)
			filename = theApp.m_strUser+"Visiting.ini";
		else
			filename = theApp.m_strUser+"LastVisit.ini";
		WritePrivateProfileSection("LastVisit", NULL, filename);

		for( int i=0; i<n; i++)
		{
			m_wndTab.GetItem(i, &TabCtrlItem);
			tcf = ((CChildFrame*)TabCtrlItem.lParam);
			if(VALID_TCF(tcf))
			{
				if(!tcf->m_pView->m_lpszUrl.IsEmpty() && !tcf->m_pView->m_bToClose)
				{
					if (tcf->m_pView->m_lpszUrl != "about:blank")
					{
						itoa(i, state+5, 10);
						itoa(i, name+4, 10);
						itoa(i, url+3, 10);
						itoa(tcf->m_pView->m_nOldIcon, num, 10);
						::WritePrivateProfileString("LastVisit", state, num, filename);
						CString strName;
						GET_TAB_TITLE(tcf,strName);
						::WritePrivateProfileString("LastVisit", name, strName, filename);
						::WritePrivateProfileString("LastVisit", url, tcf->m_pView->m_lpszUrl, filename);

						if (!bVisiting && CGf_StartPageMgr::_ins._bWrite)
						{
							CGf_StartPageMgr::_ins.AddLastVisit(strName, tcf->m_pView->m_lpszUrl);
						}
						
					}
				}
			}
		}
	}catch(...){}
}

void CMainFrame::SaveLastClose()
{
	try{
		char name[9]="name",url[8]="url"; // x+5
		CString filename;
		filename = theApp.m_strUser+"LastClose.ini";
		WritePrivateProfileSection("LastClose", NULL, filename);
		
		
		for (int i =0; i< m_strUndoName.GetCount(); i++)
		{
			
			itoa(i, name+4, 10);
			itoa(i, url+3, 10);
			::WritePrivateProfileString("LastClose", name, m_strUndoName.GetAt(i), filename);
			::WritePrivateProfileString("LastClose", url, m_strUndoURL.GetAt(i), filename);
			
		}
		
	}catch(...){}
}

void CMainFrame::OpenLastClose()
{
	//load last visit
	char name[9]="name",url[8]="url"; // x+5
	CString filename = theApp.m_strUser+"LastClose.ini";
	int i=0;
	char tmp[INTERNET_MAX_PATH_LENGTH];
	BOOL r = TRUE;
	while(r)
	{
		itoa(i, name+4, 10);
		itoa(i, url+3, 10);
		r = GetPrivateProfileString("LastClose", name, NULL, tmp, INTERNET_MAX_PATH_LENGTH, filename);
		if (r)
		{
			m_strUndoName.AddTail(tmp);
			r = ::GetPrivateProfileString("LastClose", url, NULL, tmp, INTERNET_MAX_PATH_LENGTH, filename);
			if (r)
				m_strUndoURL.AddTail(tmp);
			else
			{
				m_strUndoURL.AddTail("about:blank");
			}
			
		}
		i++;
	}
	
	UpdateUndoMenu();
}

void CMainFrame::OnNewwindow() 
{
	try
	{
		
		// TODO: Add your command handler code here
		switch(m_nNewWindow)
		{
		case 0:
			OnFileNewBlank();
			break;
		case 2://home
			OnFileNew();
			break;
		case 3://clip
			OnFileNewClip();
			break;
		case 5://select string
			OnFileNewSelect();
			break;
		case 6://select link
			OnFileNewSelectedLink();
			break;
		case 8:
			OnFileNewCurrentpage();
			break;
		case 9://up
			OnFileNewCurrentUpPage();
			break;
		case 10://root
			OnFileNewCurrentHomePage();
			break;
		}
		
	}catch(...){ASSERT(false);}
}

void CMainFrame::OnFileSendpagebyemail() 
{
	// TODO: Add your command handler code here
	GET_ACTIVE
		
		try{
		CMyIE9View * pvw = NULL;
		pvw = tcf->m_pView;
		
		//add more
		IHTMLDocument2 *pHTMLDoc = NULL;
		IPersistFile *pPersistFile = NULL;
		LPDISPATCH pDisp = NULL;
		
		//save the file to temp
		CString filename;
		CString filename2;
		CString base;
		TCHAR tmp[256];
		BOOL IsLocalFile = FALSE;
		int i;
		
		try{
			pvw->GetLocationURL(base);
			if(base.Left(5)=="file:")
				IsLocalFile = TRUE;
			if(!IsLocalFile)
				pvw->GetLocationName(filename);
			else
			{
				filename = base;
				filename = filename.Mid(8);
				filename.Replace('/', '\\');
			}
			if(filename.Left(5) =="file:" || filename.Left(5)=="http:")
			{
				if(filename.Right(1) == "/")
					filename = filename.Left(filename.GetLength()-1);
				
				i = filename.ReverseFind('/');
				filename = filename.Mid(i+1);
				filename.Replace(':', '_');
			}
			
			filename2 = filename;
			if(!IsLocalFile)
			{
				filename += ".htm0";
				filename2 +=".html";
				GetTempPath(255, tmp);
				filename = tmp + filename;
				filename2 = tmp + filename2;
			}
			i = base.ReverseFind('/');
			base = base.Left(i+1);
	}catch(...){}
	
	if(!IsLocalFile)
	{
		BSTR bfilename = filename.AllocSysString();
		try
		{
			pDisp = pvw->GetHtmlDocument();
			if( pDisp )
			{
				if (SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
				{
					if (SUCCEEDED(pDisp->QueryInterface(IID_IPersistFile, (void**)&pPersistFile)))
					{
						pPersistFile->Save(bfilename, FALSE);
					}
				}
			}
		}catch(...){}
		
		try
		{
			SysFreeString(bfilename);
			if(pPersistFile!=NULL)
				pPersistFile->Release();
			if(pHTMLDoc!=NULL)
				pHTMLDoc->Release();
			if(pDisp!=NULL)
				pDisp->Release();
		}catch(...){}
	}
	//add base url
	if(!IsLocalFile)
	{
		CStdioFile inFile(filename, CFile::modeRead), outFile(filename2, CFile::modeCreate | CFile::modeWrite);
		
		CString buf;
		outFile.WriteString("<BASE HREF=\"");
		outFile.WriteString(base);
		outFile.WriteString("\">\n");
		while(inFile.ReadString(buf))
		{
			outFile.WriteString(buf);
			outFile.WriteString("\n");
		}
		
		outFile.Close();
		inFile.Close();
		DeleteFile(filename);
	}
	
	//send email
	try
	{
		sDocuments *pMailDocs = new sDocuments;
		strncpy(pMailDocs->filename, (LPCSTR)filename2, 255);
		pMailDocs->IsLocalFile = IsLocalFile;
		AfxBeginThread(SendDocuments, (LPVOID)pMailDocs);
	}catch(...){}
	
	}catch(...){}
}

void CMainFrame::OnFileQuicksave() 
{
	// TODO: Add your command handler code here
	try{
		CString filename, root, htmlname, ext;
		CMyIE9View * pvw = NULL;
		if(m_bIsAutoSave)
			pvw = m_pCurView;
		else
		{
			GET_ACTIVE
				pvw = tcf->m_pView;
		}
		if(pvw == NULL)
			return;
		pvw->GetLocationURL(filename);
		if(filename.Left(5)=="file:")
		{
			pvw->PostMessage(WM_COMMAND, ID_FILE_SAVEAS, 0);
			return;
		}
		
		IHTMLDocument2 * pDoc = NULL;
		IHTMLElementCollection  * pAllElem = NULL;
		IHTMLElement *pElem = NULL;
		IHTMLImgElement * pImg = NULL;
		IHTMLInputImage * pInputImg = NULL;
		IHTMLBodyElement * pBody = NULL;
		IPersistFile *pPersistFile = NULL;
		VARIANT name;
		long p;
		BSTR bstrSrc;
		int l;
		sSaveFiles *SaveFiles = new sSaveFiles;
		CString rt;
		name.vt = VT_I4;
		//save dialog
		filename.Replace('/', '\\');
		if(filename.Right(1) == "\\")
			filename = filename.Left(filename.GetLength()-1);
		
		l = filename.ReverseFind('\\');
		filename = filename.Mid(l+1);
		filename.Replace(':', '_');
		l = filename.Find('?');
		if(l>0)
			filename = filename.Left(l);
		
		//auto save
		if(m_bIsAutoSave)
		{
			int l = 0;
			l = filename.ReverseFind('.');
			if(l>0)
			{
				ext = filename.Right(filename.GetLength()-l);
				ext.MakeLower();
				filename = filename.Left(l);
			}
			if(ext != ".htm" && ext != ".html")
				ext = ".htm";
			
			HFILE hf; OFSTRUCT of; char tmp[4];
			htmlname = filename + ext;
			hf = OpenFile(m_strAutoSavePath + htmlname, &of, OF_EXIST);
			l=0;
			while(hf!=HFILE_ERROR)
			{
				l++;
				htmlname = filename;
				htmlname += '[';
				itoa(l, tmp, 10);
				htmlname += tmp;
				htmlname += ']';
				htmlname += ext;
				hf = OpenFile(m_strAutoSavePath + htmlname, &of, OF_EXIST);
			}
			htmlname = m_strAutoSavePath + htmlname;
			root = htmlname.Left(htmlname.GetLength() - ext.GetLength()); 
		}	
		else
		{
			CString str;
			LOADSTR(str ,IDS_TYPE_QUICKSAVE);
			CFileDialog dlg(FALSE, ".htm", filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str);
			if(m_bIsAutoSave)
				dlg.m_ofn.lpstrInitialDir = m_strAutoSavePath;
			if(dlg.DoModal() != IDOK)
				return;
			htmlname = dlg.GetPathName();
			root = dlg.GetPathName();
			ext = dlg.GetFileExt();
			ext.MakeLower();
			if(ext == "htm" || ext == "html")
				root = root.Left(root.GetLength() - ext.GetLength()-1);
			else
				htmlname += ".htm";
		}
		
		root += "_files\\";
		SaveFiles->root = root;
		SaveFiles->filename = htmlname;
		
		CString base;
		int i;
		pvw->GetLocationURL(base);
		i = base.ReverseFind('/');
		base = base.Left(i+1);
		SaveFiles->base = base;
		
		pDoc = (IHTMLDocument2*)(pvw->GetHtmlDocument());
		if(pDoc!=NULL)
		{
			pDoc->get_images(&pAllElem);
			if(pAllElem!=NULL)
			{
				pAllElem->get_length(&p);
				if(p>0)
				{
					//save all images
					for(int i=0; i<p; i++)
					{
						name.lVal = i;
						if(pAllElem->item(name, name, (LPDISPATCH*)&pElem)==S_OK)
						{
							if(pElem != NULL)
							{
								pElem->QueryInterface(&pImg);
								if(pImg!=NULL)
								{
									pImg->get_href(&bstrSrc);
									CString s(bstrSrc);
									SysFreeString(bstrSrc);
									
									if(!SaveFiles->mapDLFiles.Lookup(s, rt))
									{
										SaveFiles->mapDLFiles.SetAt(s, "");
									}
									pImg->Release();
									pImg = NULL;
								}
								
								pElem->Release();
								pElem = NULL;
							}
						}
					}
				}
			}
			
			//get background
			pElem = NULL;
			pDoc->get_body(&pElem);
			if(pElem!=NULL)
			{
				pElem->QueryInterface(&pBody);
				if(pBody!=NULL)
				{
					pBody->get_background(&bstrSrc);
					CString s(bstrSrc);
					SysFreeString(bstrSrc);
					
					if(!s.IsEmpty())
					{
						if(s.Find(':')<0)
						{
							s = base + s;
						}
						if(!SaveFiles->mapDLFiles.Lookup(s, rt))
						{
							SaveFiles->mapDLFiles.SetAt(s, "");
						}
					}
					pBody->Release();
				}
				pElem->Release();
			}
		}
		
		// Get the HTML.
		BSTR bfilename = htmlname.AllocSysString();
		if (SUCCEEDED(pDoc->QueryInterface(IID_IPersistFile, (void**)&pPersistFile)))
			pPersistFile->Save(bfilename, FALSE);
		SysFreeString(bfilename);
		
		try{
			RELEASE(pPersistFile);
			RELEASE(pDoc);
			RELEASE(pAllElem);
	}catch(...){}
	
	try{
		AfxBeginThread(QuickSaveDocuments, (LPVOID)SaveFiles);
	}catch(...){}
	
	}catch(...){}
}

UINT _cdecl SendDocuments(LPVOID file)
{
	if(file == NULL)
		return 0;
	sDocuments *pMailDocs = (sDocuments*)file;
	
	try{
		HINSTANCE hIns = LoadLibrary("mapi32.dll");
		if(hIns != NULL)
		{
			LPMAPISendDocuments lpMAPISendDocuments = (LPMAPISendDocuments)GetProcAddress(hIns, "MAPISendDocuments");
			if(lpMAPISendDocuments!=NULL)
				lpMAPISendDocuments(0, ";", pMailDocs->filename, "", 0);
			else
				MSGBOX("This function need MAPI");
			FreeLibrary(hIns);
		}
		else
			MSGBOX("This function need MAPI");
		if(!pMailDocs->IsLocalFile)
			DeleteFile(pMailDocs->filename);
	}catch(...){}
	
	delete pMailDocs;
	
	return 0;
}

void CMainFrame::OnFileSendlinkbyemail() 
{
	// TODO: Add your command handler code here
	GET_ACTIVE
		
		try{
		CMyIE9View * pvw = NULL;
		pvw = tcf->m_pView;
		
		CString mail("mailto:?subject=");
		CString filename;
		CString base;
		int i;
		BOOL IsLocalFile = FALSE;
		try{
			pvw->GetLocationURL(base);
			if(base.Left(5)=="file:")
				IsLocalFile = TRUE;
			if(!IsLocalFile)
				pvw->GetLocationName(filename);
			else
			{
				filename = base;
				filename = filename.Mid(8);
				filename.Replace('/', '\\');
			}
			if(filename.Left(5) =="file:" || filename.Left(5)=="http:")
			{
				if(filename.Right(1) == "/")
					filename = filename.Left(filename.GetLength()-1);
				
				i = filename.ReverseFind('/');
				filename = filename.Mid(i+1);
				filename.Replace(':', '_');
			}
			if(!IsLocalFile)
			{
				filename += ".htm";
			}
	}catch(...){}
	
	mail += filename;
	mail += "&body=";
	mail += base;
	DefaultMail(mail);
	
	}catch(...){}
}

void CMainFrame::DefaultMail(LPCSTR mailto)
{
	TCHAR           sz[MAX_PATH];
	TCHAR           szPath[MAX_PATH];
	HKEY            hKey;
	DWORD           dwSize = MAX_PATH;
	
	// find out from the registry where the favorites are located.
	if(RegOpenKey(HKEY_CLASSES_ROOT, _T("mailto\\shell\\"), &hKey) != ERROR_SUCCESS)
	{
		MSGBOX("mail client not found\n");
		return ;
	}
	sz[0]=0;
	dwSize = sizeof(sz);
	RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
	if(strlen(sz)==0 || dwSize==0)
		strcpy(sz, "open");
	RegCloseKey(hKey);
	CString key = "mailto\\shell\\";
	key += sz;
	key += "\\command";
	if(RegOpenKey(HKEY_CLASSES_ROOT, key, &hKey) == ERROR_SUCCESS)
	{
		//name found, to get the path
		dwSize = sizeof(sz);
		RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
		if(strlen(sz)==0)
		{
			MSGBOX(IDS_FAIL_EMAIL);
			return;
		}
		ExpandEnvironmentStrings(sz, szPath, MAX_PATH);
		
		//run
		//MSGBOX(szPath);
		char * ch, *ch1 = szPath;
		ch = strchr(szPath, '\"');
		if(ch!=NULL)
		{
			ch1 = ch+1;
			ch = strchr(ch1, '\"');
			if(ch!=NULL)
				*ch ='\0';
		}
		else
		{
			ch = strchr(szPath, ' ');
			if(ch!=NULL)
				*ch = '\0';
		}
		if(ch!=NULL)
			ch++;
		
		CString para(ch),tmp;
		int i = para.Find("%1");
		tmp = para.Right(para.GetLength()-i-2);
		para = para.Left(i);
		para += mailto;
		para += tmp;
		ShellExecute(NULL, "open", ch1 , para, ".", SW_SHOW );
	}
	RegCloseKey(hKey);
}

CString CMainFrame::GetProxyName(CString& proxy)
{
	if(proxy=="")
		return "";
	if (m_strIeProxy == proxy)
		return g_strIeProxy;
	
	CProxyList* pl;
	int l;
	for (int i=0;i <= m_aProxyCategories.m_ProxyCategory.GetUpperBound();i++)
	{
		pl = m_aProxyCategories.m_ProxyCategory.GetAt(i);
		if((l=pl->Find(proxy))>=0)
		{
			return pl->m_Proxies.GetAt(l)->m_strProxyName;
		}
	}
	
	return proxy;
}

void ResolveRelativePath(LPCSTR base, LPCSTR site, CString& path)
{
	if(path.Find(':')<0 && path.Left(2)!="\\\\")
	{
		if(path.Left(1) == "/")
			path = site + path;
		else if(path.Left(3) == "../")
		{
			CString Base(base);
			Base = Base.Left(Base.GetLength()-1);
			int i = Base.ReverseFind('/');
			Base = Base.Left(i+1);
			path = path.Right(path.GetLength()-3);
			path = Base + path;
		}
		else if(path.Left(2) == "./")
		{
			path = path.Right(path.GetLength()-2);
			path = base + path;
		}
		else
			path = base + path;
	}
}

UINT _cdecl QuickSaveDocuments(LPVOID savefiles)
{
	CString filename, root, url, ext, dir, site;
	int l, i;
	sSaveFiles* pSaveFiles = (sSaveFiles*)savefiles;
    DWORD dwEntrySize=0;
    LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry;
	
	try{
		if(pSaveFiles==NULL )
			return 0;
		if(pSaveFiles->mapDLFiles.IsEmpty())
		{
			delete pSaveFiles;
			return 0;
		}
		root = pSaveFiles->root;
		CreateDirectory(root, NULL);
		
		dir = root.Left(root.GetLength()-1);
		l = dir.ReverseFind('\\');
		dir = dir.Mid(l+1);
		dir += '/';
		
		site = pSaveFiles->base;
		l = -1;
		for(i=0; i<3; i++)
			l = site.Find('/', l+1);
		site = site.Left(l);
		
		CString downloadfrom("<!--Download from ");
		downloadfrom += pSaveFiles->base;
		downloadfrom += " -->\n";
		
		//get input images and scripts
		CSimpleSAH sah;
		CString attri, val;
		CStringList tags;
		tags.AddTail("input");
		tags.AddTail("script");
		sah.LoadHTML(pSaveFiles->filename, FALSE);
		while(sah.GetNextTags(tags))
		{
			attri = sah.GetAttribute("src");
			if(!attri.IsEmpty())
			{
				ResolveRelativePath(pSaveFiles->base, site, attri);
				if(!pSaveFiles->mapDLFiles.Lookup(attri, val))
					pSaveFiles->mapDLFiles.SetAt(attri, "");
			}
		}
		sah.Close();
		
		//get table background
		tags.RemoveAll();
		tags.AddTail("table");
		tags.AddTail("tr");
		tags.AddTail("td");
		sah.LoadHTML(pSaveFiles->filename, FALSE);
		while(sah.GetNextTags(tags))
		{
			attri = sah.GetAttribute("background");
			if(!attri.IsEmpty())
			{
				ResolveRelativePath(pSaveFiles->base, site, attri);
				if(!pSaveFiles->mapDLFiles.Lookup(attri, val))
					pSaveFiles->mapDLFiles.SetAt(attri, "");
			}
		}
		sah.Close();
		
		//get external CSS
		tags.RemoveAll();
		tags.AddTail("link");
		sah.LoadHTML(pSaveFiles->filename, FALSE);
		while(sah.GetNextTags(tags))
		{
			attri = sah.GetAttribute("href");
			if(!attri.IsEmpty())
			{
				ext = attri.Right(4);
				ext.MakeLower();
				if( ext == ".css")
				{
					ResolveRelativePath(pSaveFiles->base, site, attri);
					if(!pSaveFiles->mapDLFiles.Lookup(attri, val))
						pSaveFiles->mapDLFiles.SetAt(attri, "");
				}
			}
		}
		sah.Close();
		
		POSITION pos = pSaveFiles->mapDLFiles.GetStartPosition();
		//save all component files
		while(pos!=NULL)
		{
			pSaveFiles->mapDLFiles.GetNextAssoc(pos, url, ext);
			
			if (!GetUrlCacheEntryInfo(url,NULL,&dwEntrySize))
			{
				if (GetLastError()==ERROR_INSUFFICIENT_BUFFER)
				{            
					lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
					if (GetUrlCacheEntryInfo(url,lpCacheEntry,&dwEntrySize))
					{
						filename = url;
						filename.Replace('/', '\\');
						if(!filename.IsEmpty())
						{
							l = filename.ReverseFind('\\');
							filename = filename.Mid(l+1);
							CopyFile(lpCacheEntry->lpszLocalFileName, root+filename, FALSE);
							pSaveFiles->mapDLFiles.SetAt(url, filename);
						}
					}
					delete lpCacheEntry;
					lpCacheEntry = NULL;
					dwEntrySize = 0;
				}
			}
		}
		//save html file
		sah.LoadHTML(pSaveFiles->filename);
		//add download from
		sah.AddText(downloadfrom);
		tags.RemoveAll();
		tags.AddTail("img");
		tags.AddTail("input");
		tags.AddTail("script");
		tags.AddTail("image");
		while(sah.GetNextTags(tags))
		{
			attri = sah.GetAttribute("src");
			if(!attri.IsEmpty())
			{
				ResolveRelativePath(pSaveFiles->base, site, attri);
				if(pSaveFiles->mapDLFiles.Lookup(attri, val))
					sah.SetAttribute("src", dir+val);
			}
		}
		sah.Close();
		//background
		sah.LoadHTML(pSaveFiles->filename);
		tags.RemoveAll();
		tags.AddTail("body");
		tags.AddTail("table");
		tags.AddTail("tr");
		tags.AddTail("td");
		while(sah.GetNextTags(tags))
		{
			attri = sah.GetAttribute("background");
			ResolveRelativePath(pSaveFiles->base, site, attri);
			if(pSaveFiles->mapDLFiles.Lookup(attri, val))
				sah.SetAttribute("background", dir+val);
		}
		sah.Close();
		
		//CSS
		sah.LoadHTML(pSaveFiles->filename);
		tags.RemoveAll();
		tags.AddTail("link");
		while(sah.GetNextTags(tags))
		{
			attri = sah.GetAttribute("href");
			ResolveRelativePath(pSaveFiles->base, site, attri);
			if(pSaveFiles->mapDLFiles.Lookup(attri, val))
				sah.SetAttribute("href", dir+val);
		}
		sah.Close();
		
	}catch(...){}
	
	delete pSaveFiles;
	return 0;
}

void CMainFrame::LaunchEmail(LPCSTR mailclient)
{
	HKEY hKey;
	DWORD dwSize;
	TCHAR           sz[MAX_PATH];
	TCHAR           szPath[MAX_PATH];
	
	CString key = "SOFTWARE\\Clients\\Mail\\";
	key += mailclient;
	key += "\\Shell\\open\\command";
	if(RegOpenKey(HKEY_LOCAL_MACHINE, key, &hKey) == ERROR_SUCCESS)
	{
		//name found, to get the path
		dwSize = sizeof(sz);
		RegQueryValueEx(hKey, _T(""), NULL, NULL, (LPBYTE)sz, &dwSize);
		if(strlen(sz)==0)
		{
			MSGBOX(IDS_FAIL_EMAIL);
			return;
		}
		ExpandEnvironmentStrings(sz, szPath, MAX_PATH);
		
		//run
		char * ch, *ch1 = szPath;
		ch = strchr(szPath, '\"');
		if(ch==szPath)
		{
			ch1 = ch+1;
			ch = strchr(ch1, '\"');
			if(ch!=NULL)
				*ch ='\0';
		}
		else
		{
			ch = strchr(szPath, ' ');
			if(ch!=NULL)
				*ch = '\0';
		}
		if(ch!=NULL)
			ch++;
		ShellExecute(NULL, "open", ch1 , ch, ".", SW_SHOW );
		
		RegCloseKey(hKey);
	}
}

void CMainFrame::LaunchEditor(LPCSTR editor, LPCSTR file)
{
	static TCHAR           szPath[MAX_PATH];
	CString url(file);
	
	int i = url.ReverseFind('#');
	if(i>0)
		url = url.Left(i);
	
	ExpandEnvironmentStrings(editor, szPath, MAX_PATH);
	
	//run
	char * ch, *ch1 = szPath;
	ch = strchr(szPath, '\"');
	if(ch==szPath)
	{
		ch1 = ch+1;
		ch = strchr(ch1, '\"');
		if(ch!=NULL)
			*ch ='\0';
	}
	else
	{
		ch = strchr(szPath, ' ');
		if(ch!=NULL)
			*ch = '\0';
	}
	if(ch!=NULL)
		ch++;
	
	if(url.Left(5) == "file:")
	{
		url = url.Mid(8);
		url.Replace('/', '\\');
		url.Replace("%20", " ");
	}
	else
	{
		_strupr(ch1);
		if(strstr(ch1, "WORD.EXE")==NULL && strstr(ch1, "DREAMWEAVER.EXE")==NULL && strstr(ch1, "FRONTPAGE")==NULL)
		{
			//get cache file path
			DWORD dwEntrySize=0;
			LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry;
			if (!GetUrlCacheEntryInfo(url,NULL,&dwEntrySize))
			{
				if (GetLastError()==ERROR_INSUFFICIENT_BUFFER)
				{
					lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
					
					if (GetUrlCacheEntryInfo(url,lpCacheEntry,&dwEntrySize))
					{
						url = lpCacheEntry->lpszLocalFileName;						
					}
					delete lpCacheEntry;
					lpCacheEntry = NULL;
				}
			}
		}
	}	
	url = "\""+url;
	url += "\"";	
	ShellExecute(NULL, "open", ch1 , url, NULL, SW_SHOW );
}

void CMainFrame::UpdateUndoMenu()
{
	try
	{
		//update menu items
		CMenu * pmenu = SMP_UNDO;
		DestroyMenuData(pmenu->m_hMenu,3);
		while(pmenu->DeleteMenu(3, MF_BYPOSITION));
		
		for (int i =0; i< m_strUndoName.GetCount(); i++)
		{
			
			CString str = m_strUndoName.GetAt(i);
			AdjustMenuWidth(str);
			
			MenuAppendOwnerItem(pmenu, MF_ENABLED, B_UNDO+i, str );
		}
		
		
	}catch(...){}
}

void CMainFrame::OnUndoList(UINT nID)
{
	int i = nID - B_UNDO;
	NewChildWindow(1,2,m_strUndoURL.GetAt(i) );
	
	m_strUndoName.Remove(i);
	m_strUndoURL.Remove(i);
	
	UpdateUndoMenu();
}

void CMainFrame::OnRtabAutoref() 
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	
	m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	
	if(VALID_TCF(tcf))
	{
		tcf->m_pView->SendMessage(WM_COMMAND, ID_TOOLS_AUTOREF, 0);
	}	
}

void CMainFrame::OnUpdateRtabAutoref(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	
	m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	
	if(VALID_TCF(tcf))
	{
		pCmdUI->SetCheck(tcf->m_pView->m_bUseAutoRef);
	}
}

void CMainFrame::OnRtabReffre() 
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	
	m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	
	if(VALID_TCF(tcf))
	{
		tcf->m_pView->SendMessage(WM_COMMAND, ID_TOOLS_REFFRE, 0);
	}	
}

int CMainFrame::FindDupURL(LPCSTR lpszURL, CChildFrame* pSelf)
{
	int n = m_nWin; //m_wndTab.GetItemCount();
	BOOL found = FALSE;
	int i=0;
	TCITEM TabCtrlItem;
	int l=-1;
	
	try
	{
		if(!g_bKillDupWin)
			return l;		
		CString strUrlNew = GetStandUrl(lpszURL);
		while (i<n && !found)
		{
			TabCtrlItem.mask = TCIF_PARAM;
			TabCtrlItem.lParam = 0;
			m_wndTab.GetItem(i, &TabCtrlItem);
			if(TabCtrlItem.lParam != NULL && (CChildFrame *)TabCtrlItem.lParam != pSelf)
			{	
				//((CChildFrame *)TabCtrlItem.lParam)->GetWindowText(str);
				if(((CChildFrame *)TabCtrlItem.lParam)->m_pView->m_lpszUrl == strUrlNew)
				{
					found = TRUE;
					l=i;
				}
				else
					i++;
			}
			else
				i++;
		}
	}catch(...){}
	
	return l;
}

BOOL CMainFrame::IsBrowsing()
{
	BOOL ib = FALSE;
	try{
		CChildFrame* tcf;
		UINT n = m_wndTab.GetItemCount();
		m_nWin = n;
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		
		for(UINT i=0;i<n;i++)
		{
			
			tcf = NULL;
			TabCtrlItem.lParam = NULL;
			m_wndTab.GetItem(i, &TabCtrlItem);
			tcf = ((CChildFrame*)TabCtrlItem.lParam);
			if(VALID_TCF(tcf))
			{
				if(tcf->m_pView->m_nProgress>0 && tcf->m_pView->m_nProgress<100)
				{
					ib=TRUE;
					break;
				}
			}
		}
	}catch(...){}
	
	return ib;
}

void CMainFrame::OnScFillform() 
{
	// TODO: Add your command handler code here
	GET_ACTIVE
		
		int inputs = 0;
	GF_TRY
	{
		CString filename = theApp.m_strFormDataPath;
		CMyIE9View* pvw = tcf->m_pView;
		
		char buf[2048];
		
		CString base,env;
		pvw->GetLocationURL(base);
		if(base.IsEmpty())
			return;

		//get password
		BOOL r = ::GetPrivateProfileString(base, "FormFillPwd", _T(""), buf, 2048, filename);
		if (r)
		{
			CInputPassword dlg;
			dlg.m_bRemPsw = false;
			if (dlg.DoModal()==IDOK)
			{
				CString strEncode,strTruePwd;
				strTruePwd = buf;
				Encoding2(dlg.m_strPassword, strEncode);
				if (strTruePwd != strEncode)
				{
					MSGBOX(IDS_PASSWORD_ERROR);
					return;
				}
			}
			else
				return;
		}
		
		CGf_FillForm gf_FillForm;
		CComPtr<IDispatch> spDispTmp;
		spDispTmp.Attach(pvw->GetHtmlDocument());
		CComQIPtr<IHTMLDocument2> pqDoc=spDispTmp;
		int inputs=gf_FillForm.Execute(m_bEncryptStringItem, pqDoc, base, filename );
		
		if(inputs>0)
				{
					CString strMsg;
					LOADSTR(strMsg ,IDS_FILL_FORM_COUNT);
					char strCount[20];
					itoa(inputs,strCount,10);
					strMsg += strCount;
					SetMessageText(strMsg);//MSGBOX(strMsg);
				}

	}
	GF_CATCH 
}

void CMainFrame::OnScSaveform() 
{
	// TODO: Add your command handler code here
	SaveForm(FALSE);

	SaveForm(TRUE);//gf
}

void CMainFrame::OnScSavegenform() 
{
	// TODO: Add your command handler code here
	SaveForm(TRUE);
}

void CMainFrame::SaveForm(BOOL bIsGeneral)
{
	GET_ACTIVE
		
	try	{
		int inputs = 0;
		CString filename = theApp.m_strFormDataPath;
		CMyIE9View* pvw = tcf->m_pView;
		
		CString base,env;
		if(bIsGeneral)
			base = "General";
		else
		{
			pvw->GetLocationURL(base);
			if(base.IsEmpty())
				return;

			//gf
			if (!bIsGeneral)
			{
				if (CGf_IniFile::IsExistSection(base, filename))
				{
					if (MSGBOX(CGfAppString::GetString(IDS_OVERWRITE_FORM), MB_YESNO|MB_ICONQUESTION) !=IDYES)
					{
						return;
					}
				}
					
			}

			WritePrivateProfileSection(base, NULL, filename);

			if (!bIsGeneral)
			{				
				WritePrivateProfileString(base, MYIE9_FORM_SECTION_TITLE, tcf->m_strTitle, filename);
			}
			
		}

		CGf_SaveForm gf_SaveForm;
		CComPtr<IDispatch> spDispTmp;
		spDispTmp.Attach(pvw->GetHtmlDocument()); //这样才能避免内存泄露
		CComQIPtr<IHTMLDocument2> pqDoc=spDispTmp;
		inputs=gf_SaveForm.Execute(pqDoc, base, filename);
		
		if(inputs>0)
		{
			CString strMsg;
			LOADSTR(strMsg ,IDS_SAVEDFORM);
			char strCount[20];
			itoa(inputs,strCount,10);
			strMsg += strCount;
			SetMessageText(strMsg);
			
			if (!bIsGeneral)
			{
				MSGBOX(strMsg);
						
				CGf_StartPageMgr::_ins.AddFav(tcf->m_strTitle, base);
			}
			
		}
		else
		{
			if (!bIsGeneral)
				MSGBOX(CGfAppString::GetString(IDS_NO_DATA_NEED_TO_SAVE));
		}

	}catch(...)
	{
	
		ASSERT(false);
		
		if (!bIsGeneral)
		{
			MSGBOX(CGfAppString::GetString(IDS_FAILED_TO_SAVE_FORM));
	
		}
	}
}

void CMainFrame::OnToolsAutosetbg() 
{
	// TODO: Add your command handler code here
	g_bAutoSetBG = !g_bAutoSetBG;
}

void CMainFrame::OnUpdateToolsAutosetbg(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(g_bAutoSetBG);
}

void CMainFrame::ChangTabPos(int npos)
{
	REBARBANDINFO rbbi;	
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID;//RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE |
	rbbi.wID = ID_VIEW_TASKBAR;
	
	if(m_bTopTab ==1)
	{
		//will move to bottom
		m_wndTab.ModifyStyle(0, TCS_BOTTOM);
		//deleete the band in top bar
		int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_VIEW_TASKBAR);
		if(nb<0)
			return;
		m_wndReBar.GetReBarCtrl( ).DeleteBand(nb);
		m_wndReBar2.AddBar(&m_wndTab);
		if(m_nTabStyle!=2)
		{
			rbbi.fMask |=  RBBIM_STYLE;
			rbbi.fStyle = RBBS_FIXEDBMP | RBBS_NOGRIPPER; 
		}
		m_wndReBar2.GetReBarCtrl().SetBandInfo(0, &rbbi);
	}
	else if(m_bTopTab==0)
	{
		//will move to top
		m_wndTab.ModifyStyle(TCS_BOTTOM,0);
		//hide the bottom bar
		m_wndReBar2.GetReBarCtrl( ).DeleteBand(0);
		m_wndReBar.AddBar(&m_wndTab,NULL, NULL,  RBBS_FIXEDBMP | RBBS_BREAK |RBBS_GRIPPERALWAYS);
		if(m_bLockToolbar)
		{
			rbbi.fMask |=  RBBIM_STYLE;
			rbbi.fStyle = RBBS_FIXEDBMP | RBBS_BREAK |RBBS_NOGRIPPER; 
			rbbi.fStyle &= ~RBBS_GRIPPERALWAYS;
		}
		int nb = countof(g_BandInfo)-1;//add tab to the last row
		m_wndReBar.GetReBarCtrl().SetBandInfo(nb, &rbbi);
	}
	{
		CRect rect;
		m_wndTab.GetWindowRect(&rect);
		m_wndTab.SetWindowPos(&wndTop, 0,0, rect.Width()+4, rect.Height(), SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOSENDCHANGING|SWP_NOACTIVATE);
	}
}

void CMainFrame::OnViewEncodingAuto() 
{
	// TODO: Add your command handler code here
	DWORD           sz;
	HKEY            hKey;
	DWORD           dwSize = sizeof(DWORD);
	
	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\International"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Global settings not found\n");
		return ;
	}
	dwSize = sizeof(sz);
	if(RegQueryValueEx(hKey, _T("AutoDetect"), NULL, NULL, (LPBYTE)&sz, &dwSize) == ERROR_SUCCESS)
	{
		//set new offline value
		if(sz != 0)
			sz = 0;
		else
			sz = 1;
		RegSetValueEx(hKey, _T("AutoDetect"), NULL, REG_DWORD, (LPBYTE)(&sz), dwSize);
	}
	RegCloseKey(hKey);
	
	//refresh current
	GET_ACTIVE
		tcf->m_pView->Refresh2(1);
}

void CMainFrame::OnUpdateViewEncodingAuto(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DWORD           sz;
	HKEY            hKey;
	DWORD           dwSize = sizeof(DWORD);
	
	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\International"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Global settings not found\n");
		return ;
	}
	dwSize = sizeof(sz);
	if(RegQueryValueEx(hKey, _T("AutoDetect"), NULL, NULL, (LPBYTE)&sz, &dwSize) == ERROR_SUCCESS)
	{
		//set new offline value
		pCmdUI->SetCheck(sz!=0);
	}
	RegCloseKey(hKey);	
}

void CMainFrame::OnSaveasGroup() 
{
	// TODO: Add your command handler code here
	m_nWin=m_wndTab.GetItemCount();
	if(m_nWin==0)
		return;
	
	CGroupSelectDlg dlg;
	if (dlg.DoModal()==IDOK)
		InitGroups();
}

CString CMainFrame::GetHomePage()
{
	//gf
// 	if (strHomePage.GetLength()==0)
// 	{
// 		strHomePage = "about:blank";
// 		SetHomePage(strHomePage);
// 	}
	return CGf_IE::GetIEHomePage();
}

void CMainFrame::SetHomePage(CString strUrl)
{
	_RegSetString(HKEY_CURRENT_USER,
		"Software\\Microsoft\\Internet Explorer\\Main",
		"Start Page",
		strUrl);
}

void CMainFrame::OnToolCleanMyIE9form() 
{
	
	//gf	DeleteFile(theApp.m_strFormDataPath);
	
	if (CRegHelper::IsIE8() )
	{
		CGf_Win::OpenHide( _T("rundll32.exe"), _T("InetCpl.cpl,ClearMyTracksByProcess 16") );
		return;
	}
	
	CGf_IE::DeleteAutoForms();
	
}

UINT _cdecl ThreadDelayLoad(LPVOID con)
{
	pmf->DelayLoad();
	return 0;
}

void CMainFrame::OnDelayLoadConfig(WPARAM wParam,LPARAM)
{
	GF_TRY
	{
		DelayMustInUI();

		AfxBeginThread(ThreadDelayLoad, NULL);

		
		m_bShowTrayIcon = _app->GetProfileInt("Settings", "ShowTrayIcon", 1);
		if (m_bShowTrayIcon)
			AddTrayIcon();

		//!!!!!!!!!!!!!!! add prev !!!!!!!!!!!!!!!!!!!!!!!!!!
		//!!! better at end to set main window foreground
		int bShowMonitor = _app->GetProfileInt("Monitor", "ShowMonitor", 1);
		if (bShowMonitor)
			OnToolsMonitor();

		SetForegroundWindow();

		
		m_wndAddress->GetEditCtrl()->SetFocus(); //gf

	}GF_CATCH
}

void CMainFrame::DelayLoad()
{
	CConfigLoaded configLoaded;
	try
	{
		OnCreateMovedToDelay();

		
		//gftip		SetMessageText( CGfAppString::GetString(IDS_STARTUP_STATUS) ); //gfG 鼠标手势	
		
		CString filename;
		CMyIE9App* app = (CMyIE9App*)AfxGetApp();
		//
		filename = theApp.m_strUser+"DownManager.ini";
		if (!CPathHelper::IsExist(filename))
		{
			CopyFile(app->m_strRoot+"Resource\\DownManager.ini",filename, FALSE);
		}
			
		//
		filename = _app->m_strUser+"CollectorScript.txt";
		if (!CPathHelper::IsExist(filename))
			CopyFile(app->m_strRoot+"Resource\\CollectorScript.txt",filename, FALSE);

		//
		filename = _app->m_strUser+"Proxy.ini";
		if (!CPathHelper::IsExist(filename))
			CopyFile(app->m_strRoot+"Resource\\Proxy.ini",filename, FALSE);
		
		//load proxy lists
		m_aProxyCategories.LoadProxies();
		
		//load filter
		TCHAR buf[1024];
		DWORD rlens;
		
		//gffly //load fly filter list
		{
			filename = theApp.m_strUser+"FilterFly.ini";
			
			char Keyname[12] ;
			int i ;
			BOOL r ;
			//
			strcpy(	Keyname, "url");
			i = 1;
			r = TRUE;
			while(r)
			{
				itoa(i, Keyname+3, 10);
				rlens = ::GetPrivateProfileString("FlyFilter", Keyname, _T(""), buf, 1024, filename);
				if(rlens==0)
					r=FALSE;
				else
					m_astrFly.AddTail(buf);
				i++;
			}
			//
			strcpy(	Keyname, "url");
			i = 1;
			r = TRUE;
			while(r)
			{
				itoa(i, Keyname+3, 10);
				rlens = ::GetPrivateProfileString("FlyUnFilter", Keyname, _T(""), buf, 1024, filename);
				if(rlens==0)
					r=FALSE;
				else
					m_astrUnFly.AddTail(buf);
				i++;
			}
		}
		
		filename = theApp.m_strUser+"Filter.ini";
		//load popup filter list
		char Keyname[12] ;
		int i ;
		BOOL r ;
		//
		strcpy(	Keyname, "pop");
		i = 1;
		r = TRUE;
		while(r)
		{
			itoa(i, Keyname+3, 10);
			rlens = ::GetPrivateProfileString("PopFilter", Keyname, _T(""), buf, 1024, filename);
			if(rlens==0)
				r=FALSE;
			else
				m_astrPopup.AddTail(buf);
			i++;
		}
		//
		strcpy(	Keyname, "pop");
		i = 1;
		r = TRUE;
		while(r)
		{
			itoa(i, Keyname+3, 10);
			rlens = ::GetPrivateProfileString("PopUnFilter", Keyname, _T(""), buf, 1024, filename);
			if(rlens==0)
				r=FALSE;
			else
				m_astrUnPopup.AddTail(buf);
			i++;
		}
		
		//load url filter
		strcpy(Keyname,"url");
		i = 1;
		r = TRUE;	
		while(r)
		{
			itoa(i, Keyname+3, 10);
			rlens = ::GetPrivateProfileString("UrlFilter", Keyname, _T(""), buf, 1024, filename);
			if(rlens==0)
				r=FALSE;
			else
				m_astrUrlFilter.Add(buf);
			i++;
		}
		//
		strcpy(Keyname,"url");
		i = 1;
		r = TRUE;	
		while(r)
		{
			itoa(i, Keyname+3, 10);
			rlens = ::GetPrivateProfileString("UrlUnFilter", Keyname, _T(""), buf, 1024, filename);
			if(rlens==0)
				r=FALSE;
			else
				m_astrUrlUnFilter.Add(buf);
			i++;
		}
		m_bUrlFilterDirty = TRUE;
		InitialUrlFilter();
		
		
				TCHAR key[10];
				

// 		//load alias
// 		TCHAR key[10];
// 		CString url, alias;
// 		i=0; //int j;
// 		r = TRUE;
// 		while(r)
// 		{
// 			key[0] = 'A';
// 			itoa(i, key+1, 10);
// 			alias = _app->GetProfileString("Alias", key);
// 			if(alias=="" || alias.IsEmpty())
// 				r=FALSE;
// 			else
// 			{
// 				alias.TrimLeft();
// 				alias.TrimRight();
// 				key[0] = 'U';
// 				itoa(i, key+1, 10);
// 				url = _app->GetProfileString("Alias", key);
// 				if(url=="" || url.IsEmpty())
// 					r=FALSE;
// 				else
// 					m_mapAlias.SetAt(alias, url);
// 			}
// 			i++;
// 		}
// 		
// 		//register drag
// 		ms_TabDropTarget = new CTabDropTarget;
// 		ms_TabDropTarget->Register(&m_wndTab);
// 		
// 		//namespace handle
// 		m_pSession = NULL;
// 		m_pFactory = NULL;
// 		InitialNameSpace();
		
		m_tLastIPUpdate = m_tLastRes = /*m_tLastWinsUpdate = m_tLastProxyUpdate*/ CTime::GetCurrentTime();
		QueryPerformanceCounter(&m_tPreBytes);
		m_nUpdateTimer = SetTimer(1530, 750, NULL);
		//
		m_nDefaultCharSet = _app->GetProfileInt("Settings", "DefaultCharSet", 0);
		if (m_nDefaultCharSet)
			GetMenu()->CheckMenuItem(m_nDefaultCharSet, MF_CHECKED);
		m_animIcon.ShowFirstImage();
		//
		m_tStartTime = CTime::GetCurrentTime();
		//
		m_bDirectOpenFile = _app->GetProfileInt("Settings", "DirectOpenFile", 1);
//		m_bOpenInNew = _app->GetProfileInt("Settings", "OpenInNew", 1);
		
		//################################################
// 		LoadTypedURL();
// 		
// 		m_bGroupMenuShowMember = _app->GetProfileInt("Settings", "GroupMenuShowMember", TRUE);
// 		m_bCloseAllBeforeNewGroup = _app->GetProfileInt("Settings", "CloseAllBeforeNewGroup", FALSE);
// 		InitGroups();
// 		
		InitMainAnimIcon();
		m_animIcon.ShowFirstImage();


		if (m_bAnimatIcon)
		{
			m_nAnimateIconTimer = SetTimer(99, 250, NULL);
		}
		
		
		//get host name
		g_szAddr[0]='\0';
		gethostname(g_szAddr,255);
		
		//get host name and domain
		GetNetWorkInfo();
		
		//get homepage
		m_strHomePage = GetHomePage();
		
		m_bMinimizeToTray = _app->GetProfileInt("Settings", "MinimizeToTray", 0);
		m_bCloseToTray = _app->GetProfileInt("Settings", "CloseToTray", 0);
		m_bCloseAllWhenCloseToTray = _app->GetProfileInt("Settings", "CloseAllWhenCloseToTray", 0);
		CGf_AppStatic::_instance._bOpenWordOutOfIE = _app->GetProfileInt("Settings", "OpenWordOutOfIE", FALSE);
// 		
// 		m_bShowTrayIcon = _app->GetProfileInt("Settings", "ShowTrayIcon", 1);
// 		if (m_bShowTrayIcon)
// 			AddTrayIcon();
// 		
// 		m_strStatusStringFormat = _app->GetProfileString("Settings", "StatusDispFormat", "L_J_D:E:F");
// 		//
// 		m_bCloseCleanCache = _app->GetProfileInt("CloseMode", "CloseCleanCache", 0);
// 		m_bCloseCleanCookies = _app->GetProfileInt("CloseMode", "CloseCleanCookies", 0);
// 		m_bCloseCleanFormData = _app->GetProfileInt("CloseMode", "CloseCleanFormData", 0);
// 		m_bCloseCleanHistory = _app->GetProfileInt("CloseMode", "CloseCleanHistory", 0);
// 		m_bCloseCleanTypedAddress = _app->GetProfileInt("CloseMode", "CloseCleanTypedAddress", 0);
// 		m_bCloseCleanTypedSearch = _app->GetProfileInt("CloseMode", "CloseCleanTypedSearch", 0);
// 		m_bCloseCleanUndoClose = _app->GetProfileInt("CloseMode", "CloseCleanUndoClose", 0);
		
		m_bAutoRefreshActive = _app->GetProfileInt("Settings", "AutoRefreshActive", 1);
		
		//add next !!!!!!!!!!!!!!!!!!!!!!!!!!
		m_bEncryptStringItem = _app->GetProfileInt("Settings", "EncryptStringItem", 0);
		g_bConfirmExitDowning = _app->GetProfileInt("Settings", "ConfirmExitDowning", 0);
		m_bForbidMainTitle = _app->GetProfileInt("Settings", "ForbidMainTitle", 0);
		//
		m_bOutputSaveTime = _app->GetProfileInt("Collector", "OutputSaveTime", TRUE);
		m_bOutputSaveFilter = _app->GetProfileInt("Collector", "OutputSaveFilter", TRUE);
		m_bOutputSaveUrlFilter = _app->GetProfileInt("Collector", "OutputSaveUrlFilter", FALSE);
		m_bOutputSaveImg = _app->GetProfileInt("Collector", "OutputSaveImg", TRUE);
		m_bOutputSavePage = _app->GetProfileInt("Collector", "OutputSavePage", TRUE);
		m_bOutputSaveText = _app->GetProfileInt("Collector", "OutputSaveText", TRUE);
		m_bOutputAutoClean = _app->GetProfileInt("Collector", "OutputAutoClean", FALSE);
		m_bCollectorAddtoBottom = _app->GetProfileInt("Collector", "CollectorAddtoBottom", FALSE);
		
		m_bSearchMatchCase = _app->GetProfileInt("Search", "SearchMatchCase", FALSE);
		m_bSearchMatchWhole = _app->GetProfileInt("Search", "SearchMatchWhole", FALSE);
		m_bNotSaveKeyword = _app->GetProfileInt("Search", "NotSaveKeyword", FALSE);
		//
		filename = theApp.m_strUser+"DownManager.ini";
		m_nManagerIndex = ::GetPrivateProfileInt("DownManager", "ManagerIndex", 0, filename);
		m_strManagerScript.Empty();
		if (m_nManagerIndex>0)
		{
			CString str;
			key[0] = 'S';
			itoa(m_nManagerIndex, key+1, 10);
			::GetPrivateProfileString("DownManager", key, _T(""), buf, 1024, filename);
			str = buf;
			if (str.GetLength()<1)
				m_nManagerIndex = 0;
			else
			{
				str.Replace("\\n","\r\n");
				m_strManagerScript = str;
			}
		}
		::GetPrivateProfileString("DownManager", "MonitorFileType", ".ZIP;.RAR;.EXE",buf, 1024, filename);
		m_strManagerType = buf;
		//
		m_strFormMainPwd = _app->GetProfileString("Settings", "FormMainPwd", "");
		m_bActiveNewAddress = _app->GetProfileInt("Settings", "ActiveNewAddress", TRUE);
		m_nFtpOpenType = _app->GetProfileInt("FtpOpen", "FtpOpenType", 1);
		m_strFtpExePath = _app->GetProfileString("FtpOpen", "FtpExePath", NULL);
		
// 		//always new window
// 		g_bAddressInNewWindow = _app->GetProfileInt("Settings", "AddressInNewWindow", 1);
// 		g_bLinkInNewWindow = _app->GetProfileInt("Settings", "LinkInNewWindow", 0);
// 		g_bFavInNewWindow = _app->GetProfileInt("Settings", "FavInNewWindow", TRUE);
// 		g_bHisInNewWindow = _app->GetProfileInt("Settings", "HisInNewWindow", TRUE);
// 		g_bFileInNewWindow = _app->GetProfileInt("Settings", "FileInNewWindow", TRUE);
// 		g_bHomeInNewWindow = _app->GetProfileInt("Settings", "HomeInNewWindow", TRUE);
		//
// 		m_bActiveFileNew = _app->GetProfileInt("Settings", "ActiveFileNew", TRUE);
// 		m_bGoProxyMenu = _app->GetProfileInt("Settings", "GoProxyMenu2", TRUE);
		
		//get my document folder
		TCHAR           szPath[MAX_PATH];
		r = TRUE;
		try
		{
			LPITEMIDLIST pidlMyDoc = NULL;
			// Get a pointer to the IMalloc interface
			IMalloc * m_pMalloc = NULL;
			HRESULT hr = SHGetMalloc(&m_pMalloc);
			if (SUCCEEDED(hr))
			{
				HRESULT hr = SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &pidlMyDoc);
				if (NOERROR != hr)
					r = FALSE;
				else
					r = SHGetPathFromIDList( pidlMyDoc,szPath);
				m_pMalloc->Free(pidlMyDoc);
				m_pMalloc->Release();
				m_pMalloc = NULL;
			}
	}catch(...){}
	
	//default path
	app->m_strDefaultDir = _app->GetProfileString("Settings", "DefaultDir");
	DO_RELATIVE_URL(app->m_strDefaultDir);
	if(app->m_strDefaultDir.IsEmpty())
	{
		app->m_strDefaultDir = (r ? szPath : _app->m_strRoot);
		app->m_strDefaultDir += "\\My Html\\";
	}
	else
	{
		SetCurrentDirectory(app->m_strDefaultDir);
		_RegSetString(HKEY_CURRENT_USER,
			"Software\\Microsoft\\Internet Explorer\\Main",
			"Save Directory",
			app->m_strDefaultDir);
	}
	if(app->m_strDefaultDir.Right(1) != "\\")
		app->m_strDefaultDir += "\\";
	
	//default path sort
	app->m_strDefaultDirSort = _app->GetProfileString("Settings", "DefaultDirSort");
	DO_RELATIVE_URL(app->m_strDefaultDirSort);
	if(app->m_strDefaultDirSort.IsEmpty())
	{
		app->m_strDefaultDirSort = (r ? szPath : _app->m_strRoot);
		app->m_strDefaultDirSort += "\\My Html Sort\\";
	}
	if(app->m_strDefaultDirSort.Right(1) != "\\")
		app->m_strDefaultDirSort += "\\";
	
	//auto save type
	m_nAutoSave = _app->GetProfileInt("Settings", "AutoSave", 2);
	m_bCateWithSite = _app->GetProfileInt("Settings", "CateWithSite", 1);
	m_bImgCateWithSite = _app->GetProfileInt("Settings", "ImgCateWithSite", 1);
	
	//image path
	app->m_strImagePath = _app->GetProfileString("Settings", "ImagePath");
	DO_RELATIVE_URL(app->m_strImagePath);
	if(app->m_strImagePath.IsEmpty())
	{
		app->m_strImagePath = (r ? szPath : _app->m_strRoot);
		app->m_strImagePath += "\\My Pictures\\";
	}
	if(app->m_strImagePath.Right(1) != "\\")
		app->m_strImagePath += "\\";
	
// 	//
// 	g_bClickClose = _app->GetProfileInt("Settings","ClickClose", 0);
// 	//confirm when exit
// 	g_bConfirmExit = _app->GetProfileInt("Settings", "ConfirmExit", FALSE);
// 	//use shortcuts
// 	m_bUseSC = _app->GetProfileInt("Settings", "UseSC", FALSE);
// 	//use alias
// 	m_bUseAlias = _app->GetProfileInt("Settings", "UseAlias", TRUE);	
// 	//sequence
// 	m_nCloseSequence = _app->GetProfileInt("Settings", "CloseSequence", 1); //gf 2
// 	//confirm
// 	g_bConfirmCloseAll  = _app->GetProfileInt("Settings", "ConfirmCloseAll", FALSE);
// 	g_bConfirmCloseAllKeepCur  = _app->GetProfileInt("Settings", "ConfirmCloseAllKeepCur", FALSE);
// 	g_bConfirmCloseLock = _app->GetProfileInt("Settings", "ConfirmCloseLock", TRUE); //gf
// 	//new window
// 	m_nNewWindow = _app->GetProfileInt("Settings", "NewWindow", 0);
// 	SMP_NEW->SetDefaultItem(m_nNewWindow, TRUE); //gfm
// 	//allow double click to close window
// 	m_nAllowDBC = _app->GetProfileInt("Settings", "AllowDBC", 1);
// 	m_nAllowMC = _app->GetProfileInt("Settings", "AllowMC", 5); //gf 4
// 	m_nAllowRC = _app->GetProfileInt("Settings", "AllowRC2", 0); //gf 5
// 	m_nLDbClickTabBlank = _app->GetProfileInt("Settings", "LDbClickTabBlank", ID_FILE_NEW_BLANK);
// 	m_nRDbClickTabBlank = _app->GetProfileInt("Settings", "RDbClickTabBlank", ID_FILE_NEW_CURRENTPAGE);
// 
// 	//mouse
// 	m_nLDbClick = _app->GetProfileInt("Mouse", "LDbClick", 0);
// 	m_nRDbClick = _app->GetProfileInt("Mouse", "RDbClick", 0); // //gf
// 	m_nLButtonDown = _app->GetProfileInt("Mouse", "LButtonDown", 0);
// 	m_nMButtonDown = _app->GetProfileInt("Mouse", "MButtonDown", ID_OPEN_LINK_DEACTIVE);
// 	m_nRButtonDown = _app->GetProfileInt("Mouse", "RButtonDown", 0);
// 	m_nLeftRight = _app->GetProfileInt("Mouse", "LeftRight", 0); //gf 0
// 	m_nRightLeft = _app->GetProfileInt("Mouse", "RightLeft", ID_FILE_CLOSE_1); //0
// 	m_bShowGesture = _app->GetProfileInt("Mouse", "ShowGesture", TRUE);
// 	//
// 	m_bWheelTab = _app->GetProfileInt("Mouse", "WheelTab", 1);
// 	m_bWheelOnTab = _app->GetProfileInt("Mouse", "WheelOnTab", 0);
// 	//
// 	m_nImgUp = _app->GetProfileInt("Mouse", "ImgUp", 6);
// 	m_nImgDown = _app->GetProfileInt("Mouse", "ImgDown", 1);
// 	m_nImgLeft = _app->GetProfileInt("Mouse", "ImgLeft", 3);
// 	m_nImgRight = _app->GetProfileInt("Mouse", "ImgRight", 1);
// 	//
// 	m_nTextUp = _app->GetProfileInt("Mouse", "TextUp", 7);
// 	m_nTextDown = _app->GetProfileInt("Mouse", "TextDown", 9);
// 	m_nTextLeft = _app->GetProfileInt("Mouse", "TextLeft", 2);
// 	m_nTextRight = _app->GetProfileInt("Mouse", "TextRight", 3);
// 	//
// 	m_nLinkUp = _app->GetProfileInt("Mouse", "LinkUp", 9);
// 	m_nLinkDown = _app->GetProfileInt("Mouse", "LinkDown", 3);
// 	m_nLinkLeft = _app->GetProfileInt("Mouse", "LinkLeft", 2);
// 	m_nLinkRight = _app->GetProfileInt("Mouse", "LinkRight", 3);
// 	//
// 	m_bSimuDragLink = _app->GetProfileInt("Mouse", "SimuDragLink", TRUE);
// 	//
// 	m_bRMouseDrag = _app->GetProfileInt("Mouse", "RMouseDrag", 0);
// 	m_bRMouseGesture = _app->GetProfileInt("Mouse", "RMouseGesture", 1);
// 	//
// 	m_nMenuStyle = _app->GetProfileInt("Settings", "MenuStyle", 1);
// 	//
// 	m_bBlankMonitorClip = _app->GetProfileInt("Settings", "BlankMonitorClip2", TRUE);
	
	key[0] = 'F';
	for(i=0; i<11; i++)
	{
		itoa(i+2, key+1, 10);
		m_strShortCuts[i] = _app->GetProfileString("Shortcuts", key);
	}
// 	//Domains
// 	m_strCE1 = _app->GetProfileString("Domains", "CE1", "http://www.");
// 	m_strCE2 = _app->GetProfileString("Domains", "CE2", ".com");
// 	m_strSE1 = _app->GetProfileString("Domains", "SE1", "http://www.");
// 	m_strSE2 = _app->GetProfileString("Domains", "SE2", ".net");
// 	m_strCSE1 = _app->GetProfileString("Domains", "CSE1", "http://www.");
// 	m_strCSE2 = _app->GetProfileString("Domains", "CSE2", ".com.cn");
	//
	m_nCleanAllMode = _app->GetProfileInt("Settings", "CleanAllMode", 127);
	m_bNameByTitle = _app->GetProfileInt("Settings", "NameByTitle", TRUE);
	m_strSortSaveFolder = _app->GetProfileString("Settings", "SortSaveFolder", "");
	m_bSavePageByDate = _app->GetProfileInt("Settings", "SavePageByDate", FALSE);
	m_bShowSortSaveDlg = _app->GetProfileInt("Settings", "ShowSortSaveDlg", TRUE);
	//
// 	m_GestureList.LoadGestureList();
	//
	m_bUseIeKey = _app->GetProfileInt("Shortcuts", "UseIeKey", FALSE);
	
// 	//!!!!!!!!!!!!!!! add prev !!!!!!!!!!!!!!!!!!!!!!!!!!
// 	//!!! better at end to set main window foreground
// 	int bShowMonitor = _app->GetProfileInt("Monitor", "ShowMonitor", 1);
// 	if (bShowMonitor)
// 		OnToolsMonitor();

	//gf OnToolsCollector(FALSE);//must create collector first!

//	SetForegroundWindow();
	//
#ifdef _WRITE_LNG_FILE_
	_WriteStringTable();
	_WriteAllMenuString();
	//
	{CAddPopupDlg dlg;		dlg.DoModal();}
	{CAddToThisFavDlg dlg;	dlg.DoModal();}
	{CCleanAll dlg;	dlg.DoModal();}
	{CCollectorDlg dlg;	dlg.DoModal();}
	{CCustomizeToolbar dlg;	dlg.DoModal();}
	{CExternalUtils dlg;	dlg.DoModal();}
	{CAboutDlg dlg;	dlg.DoModal();}
	{CGroupSelectDlg dlg;	dlg.DoModal();}
	{CGroupUrlDlg dlg;	dlg.DoModal();}
	{CInputAlias dlg;	dlg.DoModal();}
	{CInputDlg dlg;	dlg.DoModal();}
	{CInputPassword dlg;	dlg.DoModal();}
	{CLinkListDlg dlg;	dlg.DoModal();}
	{COrgGroupDlg dlg;	dlg.DoModal();}
	{CPopupConfirm dlg;	dlg.DoModal();}
	{CProxyInfo dlg;	dlg.DoModal();}
	{CQuickSearchDlg dlg;	dlg.DoModal();}
	{CSelectLang dlg;	dlg.DoModal();}
	{CSelectURL dlg;	dlg.DoModal();}
	{CSelProxyCatDlg dlg;	dlg.DoModal();}
	{CSetAliasDlg dlg;	dlg.DoModal();}
	{CSetFormDataDlg dlg;	dlg.DoModal();}
	{CSetProxy dlg;	dlg.DoModal();}
	{CSetRefIntervalDlg dlg;	dlg.DoModal();}
	{CSetSCDlg dlg;	dlg.DoModal();}
	{CSortSaveDlg dlg;	dlg.DoModal();}
	{CWebFileOpenDlg dlg;	dlg.DoModal();}
	//
	OnOptionsMyIE9options();
#endif
	
	Gf_delayLoad();

	CGf_AppStatic::_instance._bDelayLoaded=TRUE;

	InitFavorites();//very slow when many favorites
	
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
	
}

UINT _cdecl TSaveConfig(LPVOID con)
{
	theApp.SaveConfg();
	return 0;
}

void CMainFrame::BuildWebProxyMenu(CMenu *pMenu, int nType)
{
	DestroyMenuData(pMenu->m_hMenu,0);
	while(pMenu->DeleteMenu(0, MF_BYPOSITION));
	
	CString strName;
	LOADSTR(strName ,ID_OPTIONS_SET_PROXY);
	MenuAppendOwnerItem(pMenu, MF_STRING, ID_OPTIONS_SET_PROXY, strName);
	MenuAppendSeparator(pMenu);
	//
	CString strCurWebProxy;
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (VALID_TCF(tcf))
	{
		if(tcf->m_pView->m_bEnWebProxy)
		{
			strCurWebProxy = tcf->m_pView->m_strWebProxy;
		}
	}
	
	CProxyList* pl;
	int i=0;
	CString str, str1;
	
	if(nType == 0 || nType == 2)
	{
		//pure web proxy
		pl = m_aProxyCategories.GetWebProxyList();
		if(pl!=NULL)
		{
			for(i=0; i<=pl->m_Proxies.GetUpperBound(); i++)
			{
				str = pl->m_Proxies.ElementAt(i)->m_strProxyName;
				if(str.Left(2)=="--")
					MenuAppendSeparator(pMenu);//pMenu->AppendMenu(MF_SEPARATOR);
				else
				{
					AdjustMenuWidth(str);
					LOADSTR(str1 ,IDS_SP_INVALID + pl->m_Proxies.ElementAt(i)->m_nProxySpeed);
					str1 = "(" + str1 + ")";
					MenuAppendOwnerItem(pMenu, MF_STRING, B_WEB_PROXY + i, str+str1 );
					if(strCurWebProxy == pl->m_Proxies.ElementAt(i)->m_strProxy)
						pMenu->CheckMenuItem(B_WEB_PROXY + i, MF_CHECKED | MF_BYCOMMAND );
				}
			}
		}
	}
	
	if(nType == 2 )
		MenuAppendSeparator(pMenu);//pMenu->AppendMenu(MF_SEPARATOR);
	
	if(nType == 1 || nType == 2)
	{
		//translate
		i = 0;
		pl = m_aProxyCategories.GetWebProxyList(TRUE);
		if(pl!=NULL)
		{
			for(i=0; i<=pl->m_Proxies.GetUpperBound(); i++)
			{
				str = pl->m_Proxies.ElementAt(i)->m_strProxyName;
				if(str.Left(2)=="--")
					MenuAppendSeparator(pMenu);//pMenu->AppendMenu(MF_SEPARATOR);
				else
				{
					AdjustMenuWidth(str);
					LOADSTR(str1 ,IDS_SP_INVALID + pl->m_Proxies.ElementAt(i)->m_nProxySpeed);
					str1 = "(" + str1 + ")";
					MenuAppendOwnerItem( pMenu, MF_STRING, B_TRANS + i, str+str1);
					if(strCurWebProxy == pl->m_Proxies.ElementAt(i)->m_strProxy)
						pMenu->CheckMenuItem(B_TRANS + i, MF_CHECKED | MF_BYCOMMAND );
				}
			}
		}
	}
}

void CMainFrame::OnRtabAddalias() 
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	
	m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	if(tcf!=NULL)
	{
		CString url, alias;
		CInputAlias dlg;
		dlg.m_url = tcf->m_pView->m_lpszUrl;;
		dlg.m_alias = tcf->m_strTitle;
		if(dlg.DoModal()==IDOK)
		{
			alias = dlg.m_alias; url = dlg.m_url;
			alias.TrimLeft();
			url.TrimLeft();
			if(alias !="" && !alias.IsEmpty() && url!="" && !url.IsEmpty())
			{
				m_mapAlias.SetAt(alias, url);
				SaveAliasConfig();
			}
		}
	}
}

void CMainFrame::OnRtabAddsc() 
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	
	m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	if(tcf!=NULL)
	{
		CSetSCDlg dlg;
		dlg.m_strEdit = tcf->m_pView->m_lpszUrl;
		dlg.DoModal();
	}	
}

void CMainFrame::OnRtabAutosave() 
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	
	m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	if(tcf!=NULL)
	{
		tcf->m_pView->SendMessage(WM_COMMAND, ID_FILE_AUTOSAVE);
	}
}

void CMainFrame::OnViewWinCust(UINT nID) 
{
	// TODO: Add your command handler code here
	m_abWinButtons[nID - ID_VIEW_WIN_PRE] = !m_abWinButtons[nID - ID_VIEW_WIN_PRE];
	BuildWinButtons();
}

void CMainFrame::OnUpdateViewWinCust(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_abWinButtons[pCmdUI->m_nID - ID_VIEW_WIN_PRE]);
	
}

void CMainFrame::BuildWinButtons()
{
	//remove all buttons
	int b = m_SysMenuBar.GetToolBarCtrl().GetButtonCount();
	for(int i = 0; i<b;i++)
		m_SysMenuBar.GetToolBarCtrl().DeleteButton(0);
	//better place here
	m_SysMenuBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT |TBSTYLE_TOOLTIPS );
	m_SysMenuBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
	//not use tool tip, will have dup tip bug
	m_SysMenuBar.SetBarStyle(m_SysMenuBar.GetBarStyle() |
		/*CBRS_TOOLTIPS |*/ CBRS_FLYBY | CBRS_SIZE_FIXED);
		//set count
		b= 0;
	for(i=0; i <6; i++)
	{
		if(m_abWinButtons[i])
			b++;
	}
	if(m_abWinButtons[0] || m_abWinButtons[1])
		b++;
	if(m_abWinButtons[2] && b>1)
		b++;
	m_SysMenuBar.SetButtons(NULL, b);
	//add button
	i=0;
	if(m_abWinButtons[0])
		m_SysMenuBar.SetButtonInfo(i++, ID_TAB_PRE, TBSTYLE_BUTTON, 5);
	if(m_abWinButtons[1])
		m_SysMenuBar.SetButtonInfo(i++, ID_TAB_NEXT, TBSTYLE_BUTTON, 4);
	if(m_abWinButtons[0] || m_abWinButtons[1])
		m_SysMenuBar.SetButtonInfo(i++,0,TBBS_SEPARATOR,0);
	if(m_abWinButtons[2])
	{	
		m_SysMenuBar.SetButtonInfo(i++, ID_FILE_CLOSEALL, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, 3);
		if (b>1)
			m_SysMenuBar.SetButtonInfo(i++,0,TBBS_SEPARATOR,0);
	}
	if(m_abWinButtons[3])
		m_SysMenuBar.SetButtonInfo(i++, ID_WINDOW_MIN, TBSTYLE_BUTTON, 0);
	if(m_abWinButtons[4])
		m_SysMenuBar.SetButtonInfo(i++, ID_WINDOW_RESTOR, TBSTYLE_BUTTON, 1);
	if(m_abWinButtons[5])
		m_SysMenuBar.SetButtonInfo(i++, ID_FILE_CLOSE_1, TBSTYLE_BUTTON, 2);
	
	//rebar
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	int bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_SYSMENU_BAR);
	rbbi.fMask = RBBIM_STYLE;
	m_wndReBar.GetReBarCtrl().GetBandInfo(bandid, &rbbi);
	rbbi.fStyle &= ~RBBS_NOGRIPPER;	
	
	CRect rectToolBar;
	m_SysMenuBar.GetItemRect(0, &rectToolBar);
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE|RBBIM_ID|RBBIM_STYLE;
	rbbi.wID = ID_SYSMENU_BAR;
	rbbi.cyMinChild = rectToolBar.Height();
	m_SysMenuBar.GetToolBarCtrl().GetItemRect(m_SysMenuBar.GetToolBarCtrl().GetButtonCount()-1,&rectToolBar);
	rbbi.cxMinChild = /*rbbi.cx = */rbbi.cxIdeal = rectToolBar.right+2;
	m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
	
	if(m_bLockToolbar)
	{
		rbbi.fMask = RBBIM_STYLE;
		rbbi.fStyle &= ~RBBS_GRIPPERALWAYS;
        rbbi.fStyle |= RBBS_NOGRIPPER;
		m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
	}
	//save config
	char key[3];
	key[0]='W';
	for(i=0; i <6; i++)
	{
		itoa(i, key+1, 10);
		theApp.WriteProfileInt("WinButtons", key, m_abWinButtons[i]);
	}
}

void CMainFrame::OnNewTab() 
{
	// TODO: Add your command handler code here
	m_nNewSequence = (m_nNewSequence==1 ? 2 : 1);
}

void CMainFrame::OnUpdateNewTab(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ToolBarSetCheck(pCmdUI, m_nNewSequence==1);
}
void CMainFrame::OnToolsMouseScr() 
{
	// TODO: Add your command handler code here
	m_bMouseCtrlScrSpeed = !m_bMouseCtrlScrSpeed;
	
	if(!m_bMouseCtrlScrSpeed)
	{
		GET_ACTIVE
			tcf->m_pView->ChangeSpeed(tcf->m_pView->m_nSrSpeed);
	}
}

void CMainFrame::OnUpdateToolsMouseScr(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bMouseCtrlScrSpeed);
}

void CMainFrame::OnToolsUrlFilter() 
{
	// TODO: Add your command handler code here
	m_bUseUrlFilter = !m_bUseUrlFilter;
	InitialNameSpace();
}

void CMainFrame::OnUpdateToolsUrlFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bUseUrlFilter);
}

void CMainFrame::InitialUrlFilter()
{
	if(!m_bUrlFilterDirty)
		return;
	
	try{
		g_mapUrlFilter.RemoveAll();
		g_aUrlPattern.RemoveAll();
		g_aUrlPattern.FreeExtra();
		int i;
		CString tmp;
		for(i = 0; i<=m_astrUrlFilter.GetUpperBound(); i++)
		{
			tmp = m_astrUrlFilter.GetAt(i);
			tmp.MakeLower();tmp.TrimLeft(); tmp.TrimRight();
			if(tmp.Find('*')>=0)
				g_aUrlPattern.Add(tmp);
			else
				g_mapUrlFilter.SetAt(tmp, NULL);
		}
		//
		g_mapUrlUnFilter.RemoveAll();
		g_aUrlUnPattern.RemoveAll();
		g_aUrlUnPattern.FreeExtra();
		for(i = 0; i<=m_astrUrlUnFilter.GetUpperBound(); i++)
		{
			tmp = m_astrUrlUnFilter.GetAt(i);
			tmp.MakeLower();tmp.TrimLeft(); tmp.TrimRight();
			if(tmp.Find('*')>=0)
				g_aUrlUnPattern.Add(tmp);
			else
				g_mapUrlUnFilter.SetAt(tmp, NULL);
		}
		//
		m_bUrlFilterDirty = FALSE;
	}catch(...){}
}

UINT _cdecl TInitialMonitor(LPVOID con)
{
	InitSpeedMonitor();
	GetRASSpeed((DWORD*)&pmf->m_lPreBytes);
	QueryPerformanceCounter(&pmf->m_tPreBytes);
	glo_fInitializing = FALSE;
	return 0;
}

void CMainFrame::InitialNameSpace()
{
	if (m_bUseUrlFilter || m_bUseDialogFilter)
	{
		if(m_pSession==NULL)
		{
			HRESULT hr = CoGetClassObject(CLSID_MyIE9NSHandle, CLSCTX_SERVER, NULL, IID_IClassFactory, (void**)&m_pFactory);
			if(hr==S_OK)
			{
				if(CoInternetGetSession(0, &m_pSession, 0)==S_OK)
				{
					m_pSession->RegisterNameSpace(m_pFactory, CLSID_MyIE9NSHandle, L"http", 0, NULL, 0);
					m_pSession->RegisterNameSpace(m_pFactory, CLSID_MyIE9NSHandle, L"https", 0, NULL, 0);
				}
			}
		}
	}
	else
	{
		UninitialNameSpace();
	}
}

void CMainFrame::UninitialNameSpace()
{
	if(m_pSession!=NULL)
	{
		m_pSession->UnregisterNameSpace(m_pFactory, L"http");
		m_pSession->UnregisterNameSpace(m_pFactory, L"https");
		m_pSession->Release();
		m_pSession = NULL;
	}
	if(m_pFactory!=NULL)
	{
		m_pFactory->Release();
		m_pFactory = NULL;
	}
}

void CMainFrame::OnViewLocktoolbar() 
{
	// TODO: Add your command handler code here
	m_bLockToolbar = !m_bLockToolbar;
	FixToolbar(m_bLockToolbar);
}

void CMainFrame::OnUpdateViewLocktoolbar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bLockToolbar);
}

void CMainFrame::FixToolbar(BOOL bFixed)
{
	try{
		REBARBANDINFO rbbi;
		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask  = RBBIM_STYLE;
		
		int n = m_wndReBar.GetReBarCtrl().GetBandCount();
		for(int i=0; i<n; i++)
		{
			m_wndReBar.GetReBarCtrl().GetBandInfo(i, &rbbi);
			if(m_bLockToolbar)
			{
				rbbi.fStyle &= ~RBBS_GRIPPERALWAYS;
				rbbi.fStyle |= RBBS_NOGRIPPER;
			}
			else
			{
				rbbi.fStyle |= RBBS_GRIPPERALWAYS;
				rbbi.fStyle &= ~RBBS_NOGRIPPER;
			}	
			m_wndReBar.GetReBarCtrl().SetBandInfo(i, &rbbi);
		}
		
	}catch(...){}
}

void CMainFrame::OnOptionTabStyle()
{
	m_wndTab.SetTabStyle(m_nTabStyle);
	if (m_nTabStyle!=2)
	{
		if(m_bTopTab)
			m_wndTab.ModifyStyle(TCS_BOTTOM,0);
		else
			m_wndTab.ModifyStyle(0, TCS_BOTTOM);
	}
	//
	PostMessage(WM_UPDATE_TAB, 2);
}

void CMainFrame::SessionHistory(CMenu *pMenu, int ID, CChildFrame* pcf)
{
	ITravelLogStg * pITL = NULL;
	pITL = pcf->m_pView->m_pITravelLog;
	int i=0, l=0;
	if(pITL!=NULL)
	{
		IEnumTravelLogEntry * pEnum;
		ITravelLogEntry* pEntry;
		ULONG celt;
		LPOLESTR postr,posUrl;
		CString str,url;
		DWORD flag;
		if(ID == ID_GO_BACK)
			flag = TLEF_RELATIVE_BACK;
		else
			flag = TLEF_RELATIVE_FORE;
		if(pITL->EnumEntries(flag, &pEnum)==S_OK)
		{
			while(pEnum->Next(1, &pEntry, &celt)==S_OK )//&& l<9)
			{
				pEntry->GetTitle(&postr);
				pEntry->GetURL(&posUrl);
				str = postr;
				url = posUrl;
				MenuAppendOwnerUrl(pMenu, MF_STRING, l+B_HISTORY, str, url);
				l++;
				CoTaskMemFree(postr);
				CoTaskMemFree(posUrl);
				pEntry->Release();
			}
			pEnum->Release();
		}
	}
	else
	{
		if(ID == ID_GO_BACK)
		{
			for(i=pcf->m_pView->m_nCurHistory-1; i>=0 && l<MAX_HISTORY-1; i--)
			{
				if(pcf->m_pView->m_astrHistory.GetAt(i).IsEmpty())
					MenuAppendOwnerItem(pMenu, MF_STRING, l+B_HISTORY, pcf->m_pView->m_astrHistoryURL.GetAt(i) );
				else
					MenuAppendOwnerItem(pMenu, MF_STRING, l+B_HISTORY, pcf->m_pView->m_astrHistory.GetAt(i));
				l++;
			}
		}
		else
		{
			for(i=pcf->m_pView->m_nCurHistory+1; i<pcf->m_pView->m_nHistoryLength && l<MAX_HISTORY-1; i++)
			{
				if(pcf->m_pView->m_astrHistory.GetAt(i).IsEmpty())
					MenuAppendOwnerItem(pMenu, MF_STRING, l+B_HISTORY, pcf->m_pView->m_astrHistoryURL.GetAt(i));
				else
					MenuAppendOwnerItem(pMenu, MF_STRING, l+B_HISTORY, pcf->m_pView->m_astrHistory.GetAt(i));
				l++;
			}
		}
	}
}

void CMainFrame::OnToolsAutoscrollAll() 
{
	// TODO: Add your command handler code here
	m_bAllScroll = !m_bAllScroll;
	
	GET_ACTIVE
		
		if(m_bAllScroll)
		{
			tcf->m_pView->ChangeSpeed(STOP_TIMER);
		}
		else if(!tcf->m_pView->m_bUseAutoScroll)
		{
			if(tcf->m_pView->m_nScrollTimer)
				KillTimer(tcf->m_pView->m_nScrollTimer);
			tcf->m_pView->m_nScrollTimer = 0;
		}
}

void CMainFrame::OnUpdateToolsAutoscrollAll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bAllScroll);
}

void CMainFrame::UpdateTabTitleIndex(int nItem)
{
	int i;
	int cn = m_wndTab.GetItemCount();
	CChildFrame* tcf;
	CString strTitle,strWindow;
	TCITEM TabCtrlItem;
	
	TabCtrlItem.mask = TCIF_PARAM;
	for (i=nItem; i<cn; i++)
	{
		m_wndTab.GetItem(i, &TabCtrlItem);
		tcf = ((CChildFrame*)TabCtrlItem.lParam);
		if(tcf->m_strStickName=="")
		{
			//tcf->GetWindowText(strWindow);
			if (tcf->m_strTitle.GetLength()<1)
				tcf->m_strTitle = "about:blank";
			if (g_bShowIndex)
			{
				strTitle.Format("%d-%s",i+1,tcf->m_strTitle);
				SetTabTitle( strTitle, i );
			}
			else
				SetTabTitle( tcf->m_strTitle, i );
		}
	}
}

//0-means my computer file
//1-means local net work
//2-means other web file
//3-means "about:blank"
int CMainFrame::GetUrlType(LPCTSTR URL)
{
	//C:/My%20Documents/11.txt //file:///C:/My%20Documents/11.txt
	CString strUrl;
	if (IS_SCRIPT_MAILTO_STR(URL))
	{
		//javascript:pop("/html/2.html",710,550)..http://ww/html/2.html
		LPCTSTR p = URL;
		while(*p)
		{
			if (*p==0x01)
			{
				p+=2;
				break;
			}
			p++;
		}
		strUrl = p;
	}
	else
		strUrl = URL;
	//###############
	if (strUrl.Left(11)=="about:blank")
		return 3;
	if (strUrl.Left(6)=="res://")
		return 3;//some internal url may like "res://C:\WINNT\system32\shdoclc.dll/navcancl.htm"
	if (strUrl.Left(5)=="file:" || strUrl.Find(':')==1)//file://c:/qwe.htm,c:/qwe.htm
		return 0;
	
	//###############
	int nDot = strUrl.Find('.');
	int nSep = strUrl.Find('/',MAX_CL);//not use 7
	
	//http://www/
	if (nDot==-1)
		return 1;
	//http://www/qwe.htm
	if ( nDot > nSep )//not use 7
		return 1;
	int len = m_strDomain.GetLength();
	if (len && strUrl.Left(len) == m_strDomain)//http://10.170.
		return 1;
	
	if (strUrl.Left(9) =="127.0.0.1" || 
		strUrl.Left(9) =="localhost" ||
		strUrl.Left(16)=="http://127.0.0.1" || 
		strUrl.Left(16)=="http://localhost"
		)
		return 1;
	
	return 2;
}

void CMainFrame::GetSiteName(CString& site)
{
	int l;
	site.MakeLower();
	site.Replace('\\', '/');
	l = site.Find(':');
	l++;
	if(site.GetAt(l)=='/')
		while(site.GetAt(l)=='/')
			l++;
		site = site.Mid(l);
		l =	site.Find('/');
		if(l>0)
			site = site.Left(l);
		if(site.Right(1) == ":")
			site = site.Left(site.GetLength()-1);
		static TCHAR alph[] = "abcdefghijkmnopqistuvwxyz";
		l = site.FindOneOf(alph);
		if(l>=0)
		{
			l = site.Find('.');
			if(l>0)
				site = site.Mid(l+1);
			l = site.Find('.');
			if(l>0)
				site = site.Left(l);
		}
		REPLACE_ERR_PATH_CHAR(site)
}

void CMainFrame::MenuInserOwnerItem(CMenu *pMenu,UINT nPosition, UINT nFlags, UINT nIDNewItem,LPCTSTR lpszNewItem,int icon)
{
	CMenuData * md = NULL;
	
	md= new CMenuData;
	SetMenuText(lpszNewItem,md);
	md->m_pImgList = &(m_wndMenuBar.m_ImgMenu);	
	md->m_nBitmap = icon;
	pMenu->InsertMenu(nPosition,MF_BYPOSITION| MF_STRING | MF_OWNERDRAW  ,nIDNewItem, (LPCTSTR)md);
}

void CMainFrame::MenuAppendOwnerItem(CMenu *pMenu, UINT nFlags, UINT nIDNewItem, CString str)
{
	CMenuData * md = NULL;
	md= new CMenuData;
	md->m_pImgList = &(m_wndMenuBar.m_ImgMenu);
	md->m_nBitmap = -1;
	SetMenuText(str,md);
	pMenu->AppendMenu ( nFlags|MF_OWNERDRAW, nIDNewItem, (char*)md );
}

void CMainFrame::MenuAppendOwnerUrl(CMenu *pMenu, UINT nFlags, UINT nIDNewItem, CString str, CString strUrl)
{
	CMenuData * md = NULL;
	md= new CMenuData;
	md->m_pImgList = &(m_wndMenuBar.m_ImgMenu);
	md->m_nBitmap = -1;
	SetMenuText(str,md);
	SetURL(strUrl,md);
	pMenu->AppendMenu ( nFlags|MF_OWNERDRAW, nIDNewItem, (char*)md );
}

void CMainFrame::MenuAppendSeparator(CMenu *pMenu)
{
	CMenuData * md = NULL;
	md= new CMenuData;
	md->m_pImgList = NULL;
	md->m_nBitmap = -1;
	pMenu->AppendMenu ( MF_SEPARATOR|MF_OWNERDRAW, 0, (char*)md );
}

void CMainFrame::MenuInsertSeparator(CMenu *pMenu, UINT nPosition)
{
	CMenuData * md = NULL;
	md= new CMenuData;
	md->m_pImgList = NULL;
	md->m_nBitmap = -1;
	pMenu->InsertMenu ( nPosition, MF_BYPOSITION|MF_SEPARATOR|MF_OWNERDRAW, 0, (char*)md );
}

void CMainFrame::OnEncode()
{
	if (m_nDefaultCharSet==0)
		return;
	OnEncoding(m_nDefaultCharSet);
}

void CMainFrame::GetHtmlSelectedText(CString &str)
{
	GET_ACTIVE
		
		str.Empty();
	IHTMLDocument2 * pDoc = NULL;
	
	try{
		pDoc = (IHTMLDocument2*)(tcf->m_pView->GetHtmlDocument());
		if(pDoc!=NULL)
		{
			IHTMLSelectionObject *pSelection;
			IDispatch *pRange;
			IHTMLTxtRange *pText;
			//
			pDoc->get_selection(&pSelection);
			if (pSelection)
			{
				pSelection->createRange(&pRange);
				if (pRange)
				{
					pRange->QueryInterface(&pText);
					if (pText)
					{
						BSTR bstrSrc;
						pText->get_text(&bstrSrc);
						str = bstrSrc;
						str.TrimLeft();
						str.TrimRight();
						//
						SysFreeString(bstrSrc);
						pText->Release();
					}
					pRange->Release();
				}
				pSelection->Release();
			}
		}
	}catch(...){}
	
	try{
		if (pDoc)
			pDoc->Release();
	}catch(...){}
}

void CMainFrame::OnFileNewSelect() 
{
	// TODO: Add your command handler code here
	CString str;
	GetHtmlSelectedText(str);
	if (str.GetLength())
	{
		if (m_bActiveFileNew)
			NewChildWindow(1,2,str,NULL,TRUE);
		else
			NewChildWindow(1,2,str);
	}
}

void CMainFrame::GetNetWorkInfo()
{
	try
	{
		
		char name[256];
		gethostname(name,255);
		m_strHostName = name;
		//get domain
		if (m_strHostName.GetLength())
		{
			struct hostent * pHostent=NULL;
			pHostent=gethostbyname(m_strHostName);
			CString x;
			if (pHostent != NULL)
			{
				m_strDomain.Format("http://%u.%u.",
					(unsigned int)((unsigned char*)pHostent->h_addr_list[0])[0],
					(unsigned int)((unsigned char*)pHostent->h_addr_list[0])[1]
					);
			}
		}
		
	}catch(...){ASSERT(false);}
}

void CMainFrame::OnUpdateRMouseDrag(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bRMouseDrag);
}

void CMainFrame::OnUpdateRMouseGesture(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bRMouseGesture);
}

void CMainFrame::OnRMouseDrag()
{
	m_bRMouseDrag = !m_bRMouseDrag;
	if (m_bRMouseDrag)
		m_bRMouseGesture = 0;
}

void CMainFrame::OnRMouseGesture()
{
	m_bRMouseGesture = !m_bRMouseGesture;
	if (m_bRMouseGesture)
		m_bRMouseDrag = 0;
}

CString CMainFrame::GetStandUrl(CString url)
{
	if (url.Find(':')!=1)
		return url;
	CString newUrl = "file:///";
	url.Replace('\\', '/');	
	newUrl += url;
	return newUrl;
}

void CMainFrame::OnToolsEnablePopFilter() 
{
	// TODO: Add your command handler code here
	g_bKillPop = !g_bKillPop;
}

void CMainFrame::OnUpdateToolsEnablePopFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(g_bKillPop);
}

void CMainFrame::OnToolsAutoscrollPage(UINT nID)
{
	switch(nID)
	{
	case ID_TOOLS_AUTOSCROLL_PAGE0:
		m_nAutoScrollPage = 0;
		break;
	case ID_TOOLS_AUTOSCROLL_PAGE1:
		m_nAutoScrollPage = 1;
		break;
	case ID_TOOLS_AUTOSCROLL_PAGE2:
		m_nAutoScrollPage = 2;
		break;
	case ID_TOOLS_AUTOSCROLL_PAGE4:
		m_nAutoScrollPage = 4;
		break;
	}
}

void CMainFrame::BuildGoUpMenu(CMenu *pPopup)
{
	GET_ACTIVE
		
		if (tcf->m_pView->m_lpszUrl == "about:blank")
			return;
		CString strUrl,strUrlUp;
		strUrl = tcf->m_pView->m_lpszUrl;
		CMenuData * md = NULL;
		int nIndex = 0;
		while(nIndex < MAX_GO_UP)
		{
			strUrlUp = _StringGetUpUrl(strUrl);
			if (strUrlUp == strUrl)
				break;
			strUrl = strUrlUp;
			//
			md= new CMenuData;
			md->m_pImgList = NULL;
			SetMenuText(strUrl,md);
			//SetURL(strUrl,md);
			pPopup->AppendMenu ( MF_ENABLED|MF_OWNERDRAW, B_GO_UP + nIndex, (char*)md );
			g_strGoUpUrl[nIndex] = strUrl;
			//
			nIndex++;
		}
}

void CMainFrame::BuildProxyMenu(CMenu *pPopup)
{
	CString strName;
	LOADSTR(strName ,ID_OPTIONS_SET_PROXY);
	MenuAppendOwnerItem(pPopup, MF_STRING, ID_OPTIONS_SET_PROXY, strName);
	MenuAppendSeparator(pPopup);
	//
	GetIeProxy();
	CString strMenu = g_strIeProxy;
	strMenu += "("+m_strIeProxy+")";
	MenuAppendOwnerItem(pPopup, MF_STRING, ID_OPTIONS_USE_IE_PROXY, strMenu);
	MenuAppendSeparator(pPopup);
	//
	int i=0;
	CString str, str1;
	CProxyList* pl;
	CMenu submenu;
	int pindex = 0;
	int pcates = 0;
	CProxyItem* pi;
	for(int l = 0;l <= m_aProxyCategories.m_ProxyCategory.GetUpperBound();l++)
	{	
		pl = m_aProxyCategories.m_ProxyCategory.GetAt(l);
		if(pl!=NULL && !pl->m_bIsWebProxy)
			pcates++;
	}
	for(l = 0;l <= m_aProxyCategories.m_ProxyCategory.GetUpperBound();l++)
	{
		pl = m_aProxyCategories.m_ProxyCategory.GetAt(l);
		if(pl!=NULL && !pl->m_bIsWebProxy)
		{
			if(pcates>1)
				submenu.CreatePopupMenu();
			for(i=0; i<=pl->m_Proxies.GetUpperBound(); i++)
			{
				str = pl->m_Proxies.ElementAt(i)->m_strProxyName;
				if(str.Left(2)=="--")
				{
					if(pcates>1)
						MenuAppendSeparator(&submenu);
					else if(pcates==1)
						MenuAppendSeparator(pPopup);
				}
				else
				{
					AdjustMenuWidth(str);
					LOADSTR(str1 ,IDS_SP_INVALID + pl->m_Proxies.ElementAt(i)->m_nProxySpeed);
					str1 = "(" + str1 + ")";
					if(pcates>1)
						MenuAppendOwnerItem(&submenu, MF_STRING, B_PROXY + pindex, str+str1);
					else if(pcates==1)
						MenuAppendOwnerItem(pPopup, MF_STRING, B_PROXY + pindex, str+str1);
					//
					pi = pl->m_Proxies.ElementAt(i);
					if (m_bEnProxy && m_strCurProxy==pi->m_strProxy)// && m_strCurProxyName==pi->m_strProxyName)
					{
						if(pcates>1)
							submenu.CheckMenuItem(B_PROXY + pindex, MF_CHECKED | MF_BYCOMMAND );
						else if(pcates==1)
							pPopup->CheckMenuItem(B_PROXY + pindex, MF_CHECKED | MF_BYCOMMAND );
					}
					pl->m_Proxies.ElementAt(i)->index = pindex;
					pindex++;
				}
			}
			if(pcates>1)
			{
				MenuAppendOwnerItem(pPopup, MF_POPUP, (UINT)submenu.m_hMenu, pl->m_strCategoryName);
				submenu.Detach();
			}
		}
	}
}

void CMainFrame::OnOptionsUseIeProxy() 
{
	if (m_strIeProxy.IsEmpty())
	{
		MSGBOX(IDS_NO_IE_PROXY);
		return;
	}
	//
	if (m_bEnProxy && m_strCurProxyName == g_strIeProxy)
	{
		m_bEnProxy = FALSE;
	}
	else
	{
		m_bEnProxy = TRUE;
		m_strCurProxy = m_strIeProxy;
		m_strCurProxyName = g_strIeProxy;
		m_strProxyByPass = m_strIeProxyByPass;
		//m_nCurProxyList = 0;
	}
	UpdateProxy();
}

void CMainFrame::OnUpdateOptionsUseIeProxy(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bEnProxy && m_strCurProxy == m_strIeProxy);
}

void CMainFrame::UpdateProxy()
{
	INTERNET_PROXY_INFO ipi;
	DWORD dwSize2 = sizeof(ipi);
	if (m_bEnProxy)
	{
		if(m_strCurProxy.IsEmpty())
		{
			if (m_strIeProxy.IsEmpty())
			{
				MSGBOX(IDS_NO_PROXY);
				m_bEnProxy = FALSE;
				return;
			}
			else
			{
				m_strCurProxy = m_strIeProxy;
				m_strCurProxyName = g_strIeProxy;
				m_strProxyByPass = m_strIeProxyByPass;
				//m_nCurProxyList = 0;
			}
		}
		//
		ipi.dwAccessType = INTERNET_OPEN_TYPE_PROXY ;
		ipi.lpszProxy = m_strCurProxy;
		ipi.lpszProxyBypass = m_strProxyByPass;
		if(GF_SET_PROXY && !InternetSetOption(NULL, INTERNET_OPTION_PROXY, &ipi, dwSize2))
		{
			MSGBOX(IDS_PROXY_FAIL);
			return;
		}
	}
	else
	{
		ipi.dwAccessType = INTERNET_OPEN_TYPE_DIRECT ;
		if(GF_SET_PROXY && !InternetSetOption(NULL, INTERNET_OPTION_PROXY, &ipi, dwSize2))
			MSGBOX(IDS_PROXY_FAIL);
	}
}

void CMainFrame::GetIeProxy()
{
	CString strRet;
	
	m_strIeProxy = _RegGetString(HKEY_CURRENT_USER,
		"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings",
		"ProxyServer");
	m_strIeProxyByPass = _RegGetString(HKEY_CURRENT_USER,
		"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings",
		"ProxyOverride");
	
	if (m_strIeProxy.GetLength()<1)
	{
		GetProxySetting(m_strIeProxy,m_strIeProxyByPass);
	}
	//update
	if (m_strCurProxyName == g_strIeProxy)
	{
		m_strCurProxy = m_strIeProxy;
		//m_strCurProxyName = g_strIeProxy;
		m_strProxyByPass = m_strIeProxyByPass;
		UpdateProxy();
	}
}

void CMainFrame::LoadTypedSearch()
{
	COMBOBOXEXITEM item;
	item.mask = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE ;
	item.iImage = 1;
	item.iSelectedImage = 1;
	
	char Keyname[10] = "key";
	int i=0;
	CString buf;
	CWinApp* app = AfxGetApp();
	while(1)
	{
		itoa(i, Keyname+3, 10);
		buf = _app->GetProfileString("SearchKey", Keyname, "");
		if (buf.GetLength()<=0)
			break;
		m_astrTypedSearch.AddTail(buf);
		//
		item.iItem = i;//i-1;
		item.pszText = (LPSTR)(LPCTSTR)buf;
		m_wndSearch->InsertItem(&item);//m_wndSearch->InsertString(i,buf);
		i++;
	}
	m_wndSearch->SetCurSel(0);
}

void CMainFrame::BuildSearchMenu(BOOL bMenuBreak)
{
	CSearch *item;
	CString strName;
	
	CMenu * pPopup = SMP_SEARCH;
	DestroyMenuData(pPopup->m_hMenu,3);
	while(pPopup->DeleteMenu(3, MF_BYPOSITION));
	//
	for(int i=0;i<=m_SearchList.m_Searchs.GetUpperBound();i++)
	{
		item = m_SearchList.m_Searchs.GetAt(i);
		strName = item->name;
		AdjustMenuWidth(strName);
		if (strName == "--")
			MenuAppendSeparator(pPopup);
		else
		{
			MenuAppendOwnerItem(pPopup, MF_STRING, B_SEARCH + i, strName);
			if (item->state)//if (item->name==m_strCurrSearchName)
				pPopup->CheckMenuItem(B_SEARCH + i, MF_CHECKED | MF_BYCOMMAND );
		}
	}
	//
	if (bMenuBreak)
		AddMenuBreak(pPopup, -1);
}

void CMainFrame::BuildTextDropSearchMenu(CMenu *pPopup)
{
	DestroyMenuData(pPopup->m_hMenu,0);
	while(pPopup->DeleteMenu(0, MF_BYPOSITION));
	
	CSearch *item;
	CString strName;
	for(int i=0;i<=m_SearchList.m_Searchs.GetUpperBound();i++)
	{
		item = m_SearchList.m_Searchs.GetAt(i);
		strName = item->name;
		AdjustMenuWidth(strName);
		if (strName == "--")
			pPopup->AppendMenu(MF_SEPARATOR);
		else
		{
			pPopup->AppendMenu( MF_ENABLED, B_SEARCH_DRAG + i , strName);
			if (item->state)//if (item->name==m_strCurrSearchName)
				pPopup->CheckMenuItem(B_SEARCH_DRAG + i, MF_CHECKED );
		}
	}
	AddMenuBreak(pPopup, 0);//FALSE);
}

void CMainFrame::OnNewSearchEnter()//edit input
{
	try{
		
		if (m_nSearchBarAction)
		{
			switch (m_nSearchBarAction)
			{
			case 1:OnSearch(0);break;//search string
			case 2:OnFindSearchBar();break;//find next
				//case 3:break;//find prev
			case 4:OnHighLightSearchBar();break;//high light
			}
			m_nSearchBarAction = 0;
			return;
		}
		//must do this for "Search All"
		g_nSearchType = ST_SEARCHBAR;
		//
		short ks=GetKeyState(VK_CONTROL), ss=GetKeyState(VK_SHIFT);
		if (ks<0 || ss<0)
		{
			if (ks<0 && ss<0)
				OnHighLightSearchBar();
			else
				OnFindSearchBar();
		}
		else
			OnSearch(0);
		
	}catch(...){ASSERT(false);}
}

void CMainFrame::OnSearch(UINT nID)
{
	try{
		int i=0;
		int index;
		CString keyw,url;
		CSearch *item;
		
		if (m_SearchList.m_Searchs.GetSize()<1)
		{
			MSGBOX(IDS_NO_SEARCH_ENGINE);
			return;
		}
		//get index and search type
		if (nID >= B_SEARCH_DRAG)
		{
			index = nID - B_SEARCH_DRAG;
			g_nSearchType = ST_DRAGMENU;
		}
		else
		{
			index = nID - B_SEARCH;
		}
		//
		if (g_nSearchType==ST_ADDRBAR)
		{
			m_wndAddress->GetEditCtrl()->GetWindowText(keyw);
			keyw.TrimLeft();keyw.TrimRight();
			if (keyw.GetLength())
				m_wndSearch->GetEditCtrl()->SetWindowText(keyw);
		}
		else if (g_nSearchType==ST_DRAGMENU)//"all search" of text drag have to use g-lasttext
		{
			keyw = g_strLastText;
			keyw.TrimLeft();keyw.TrimRight();
			if (keyw.GetLength())
				m_wndSearch->GetEditCtrl()->SetWindowText(keyw);
		}
		else
			m_wndSearch->GetEditCtrl()->GetWindowText(keyw);
		g_nSearchType=ST_SEARCHBAR;
		keyw.TrimLeft();
		keyw.TrimRight();
		if (keyw.GetLength() && m_bNotSaveKeyword==FALSE)//only for edit input
		{
			//save typed keyword
			POSITION pos = m_astrTypedSearch.Find(keyw);
			if (pos==NULL)
			{
				m_astrTypedSearch.AddHead(keyw);	//add at head
				//
				COMBOBOXEXITEM comboitem;
				comboitem.mask = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE ;
				comboitem.iImage = 1;
				comboitem.iSelectedImage = 1;
				comboitem.iItem = 0;
				comboitem.pszText = (LPTSTR)(LPCTSTR)keyw;
				m_wndSearch->InsertItem(&comboitem);
			}
		}
		//search
		if (nID < B_SEARCH)
		{
			for(i=0;i<=m_SearchList.m_Searchs.GetUpperBound();i++)
			{
				item = m_SearchList.m_Searchs.GetAt(i);
				if (item && item->state)
				{
					url = item->url;//not change item's url
					SearchKey(keyw, url);
				}
			}
		}
		else//click the pop menu
		{
			item = m_SearchList.m_Searchs.GetAt(index);
			if (item)
			{
				if (PRESS_CTRL)//press CTRL, only change the item state
				{
					item->state = 1 - item->state;
					m_wndSearch->GetEditCtrl()->SetFocus();
				}
				else if (PRESS_SHIFT)//press CTRL, only change the item state
				{
					//clean all item selected state
					CSearch *itemTemp;
					for(i=0;i<=m_SearchList.m_Searchs.GetUpperBound();i++)
					{
						itemTemp = m_SearchList.m_Searchs.GetAt(i);
						itemTemp->state = 0 ;
					}
					//only selected current
					item->state = 1 ;
					m_wndSearch->GetEditCtrl()->SetFocus();
				}
				else //if (keyw.GetLength()>0)
				{
					url = item->url;//not change item's url
					SearchKey(keyw, url);
				}
			}
		}
		//
		if (GetFocus() == (CWnd*)m_wndSearch->GetEditCtrl())
		{
			GET_ACTIVE
				tcf->m_pView->SetFocus();
		}
		
	}catch(...)	{}
}

void CMainFrame::SearchKey(CString keyw, CString url)
{
	try{
		url.TrimLeft();
		url.TrimRight();
		if ( url.Find('|') == -1 )
		{
			DO_SEARCH_STR(url,keyw)
				NewChildWindow(1,2,url);
		}
		else
		{
			CString url2,keyw2;
			int pos = 0;
			int pos2 = 0;
			int urlLength = url.GetLength(); 
			while(pos<urlLength)
			{
				pos2 = url.Find('|',pos);
				if (pos2==-1)
					pos2 = urlLength ;
				url2 = url.Mid(pos, pos2-pos);
				url2.TrimLeft();
				url2.TrimRight();
				keyw2 = keyw;
				if (url2.GetLength()>2)//at least %s
				{
					DO_SEARCH_STR(url2,keyw2)
						NewChildWindow(1,2,url2);
				}
				pos = pos2+1;
			}
		}
	}catch(...){}
}

void CMainFrame::AddTrayIcon()
{
	NOTIFYICONDATA nid;
	nid.cbSize=sizeof(nid);
	nid.hWnd=m_hWnd;
	nid.uID=IDR_MAINFRAME + g_nTrayIconID;
	nid.uFlags=NIF_ICON|NIF_TIP|NIF_MESSAGE;
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PAGE));
	nid.uCallbackMessage=WSM_TRAY;
	strcpy(nid.szTip, CGfAppString::GetString(AFX_IDS_APP_TITLE) );
	Shell_NotifyIcon(NIM_ADD, &nid);
}

void CMainFrame::DelTrayIcon()
{
	NOTIFYICONDATA nid;
	nid.cbSize=sizeof(nid);
	nid.hWnd=m_hWnd;
	nid.uID=IDR_MAINFRAME + g_nTrayIconID;
	nid.uFlags=NIF_ICON|NIF_TIP|NIF_MESSAGE;
	nid.uCallbackMessage=WSM_TRAY;
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

void CMainFrame::StopView(int nTabID)
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	m_wndTab.GetItem(nTabID, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	
	if (tcf==NULL || tcf->m_pView==NULL)
		return;
	CMyIE9View *pvw = tcf->m_pView;
	pvw->Stop();
	pvw->m_nProgress = -10;
	if(pvw->m_bIsActive)
		g_nPercent = -1;
	//change icon
	if (pvw->m_nOldIcon<TI_1 || pvw->m_nOldIcon>TI_3)
		return;
	if(pvw->m_bIsActive)
		SetTabIcon( TI_NO, nTabID, pvw );
	else
		SetTabIcon( TI_100, nTabID, pvw );
	//update tab bar tab0
	if(m_nTabStyle==2)
		PostMessage(WM_UPDATE_TAB);
}

void CMainFrame::CloseAllTabDirect() 
{
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	
	try{
		CChildFrame* tcf;
		int n = m_wndTab.GetItemCount();
		TabCtrlItem.mask = TCIF_PARAM;
		int l =n-1;
		for(int i=n-1;i>=0;i--)
		{
			m_wndTab.GetItem(l, &TabCtrlItem);
			tcf = ((CChildFrame*)TabCtrlItem.lParam);
			if(tcf != NULL)
			{
				if(!tcf->m_bLock && !tcf->m_pView->m_bToClose)
				{
					tcf->m_pView->m_bToClose = TRUE;
					tcf->SendMessage(WM_CLOSE);//!! must send msg
				}
			}
			l--;
		}
	}catch(...){}
}

void CMainFrame::OnUpdateViewAutohide(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ToolBarSetCheck(pCmdUI, m_bAutoHideMode);
}

void CMainFrame::OnViewSysMenuBar() 
{
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
	int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_SYSMENU_BAR);
	m_wndReBar.GetReBarCtrl().GetBandInfo(nb, &rbbi);
	
	if( rbbi.fStyle & RBBS_HIDDEN )
	{
		m_SysMenuBar.ShowWindow(SW_SHOW);
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb, TRUE);
		GetMenu()->CheckMenuItem(ID_SYSMENU_BAR, MF_CHECKED);
	}
	else
	{
		m_SysMenuBar.ShowWindow(SW_HIDE);
		m_wndReBar.GetReBarCtrl( ).ShowBand(nb,FALSE);
		GetMenu()->CheckMenuItem(ID_SYSMENU_BAR, MF_UNCHECKED);
	}
}

void CMainFrame::OnToolsMonitor() 
{
	// TODO: Add your command handler code here
	try{
		if(m_pMonitor==NULL)
		{
			m_pMonitor = new CMonitor();
			if(m_pMonitor!=NULL)
			{
				m_pMonitor->Create(IDD_MONITOR);
				CWinApp* app = AfxGetApp();
				RECT rect;
				int top = rect.top = _app->GetProfileInt("Monitor", "top", 180);
				int left = rect.left = _app->GetProfileInt("Monitor", "left", 653);
				rect.bottom = top+32;
				rect.right = left+32;
				m_pMonitor->SendMessage(WM_SIZE, 0, ((32)<<16)+32);   
				m_pMonitor->MoveWindow(&rect);
				
				//register pmonitor as CAddDropTarget will made monitor OnDropFiles not work
				//if (!m_pMonitor->ms_dropTarget.Register(m_pMonitor)) {
				//	TRACE("Fail in registing drop target\n");
				//}
				
				m_pMonitor->ShowWindow(SW_SHOW);
				SetForegroundWindow();
			}
		}
		else if(!IsMonitorVisible())
		{
			m_pMonitor->ShowWindow(SW_SHOW);
			SetForegroundWindow();
		}
		else
		{
			m_pMonitor->SendMessage(WM_CLOSE);
		}
	}catch(...){}
}

void CMainFrame::DropFiles(HDROP hDropInfo)
{
	if (!IsWindowVisible())
		OnHotKey(0,0);
	else
		SetForegroundWindow();
	OnDropFiles(hDropInfo);
}

void CMainFrame::OnUpdateToolsMonitor(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( IsMonitorVisible() );
}

void CMainFrame::CollectorDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect)
{
	POINT point={0,0};
	m_pCollectDlg->ms_dropTarget.OnDrop((CWnd*)m_pCollectDlg,pDataObject,dropEffect,point);
}

void CMainFrame::AddToPopList(CString url, int type)
{
	m_nPopFiltered++;
	//
	CString strMsg;
	if (type==0)//auto pop ad window
	{LOADSTR(strMsg ,IDS_AUTO_POPUP_FILTER);}
	//else if (type==1)//content window
	//	{LOADSTR(strMsg ,IDS_URL_FILTER);}
	else if (type==2)//user filter ad window
	{LOADSTR(strMsg ,IDS_USER_LIST_FILTER);}
	else if (type==3)//dialog filter
	{LOADSTR(strMsg ,IDS_DIALOG_FILTER);}
	strMsg += url;
	m_pCollectDlg->ms_dropTarget.SaveText(strMsg, 11);
}

void CMainFrame::AddToCollector(CString strMsg, int type)
{
	m_pCollectDlg->ms_dropTarget.SaveText(strMsg, type);
}

void CMainFrame::DoGroup()
{
	CRect rect;
	m_wndToolBar.GetToolBarCtrl().GetRect(ID_GROUP, &rect);
	rect.top = rect.bottom;
	m_wndToolBar.ClientToScreen( &rect.TopLeft());
	CMenu* pPopup;
	pPopup = SMP_GROUP;
	
	g_bRTab=0;
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
}

void CMainFrame::DoFont()
{
	CRect rect;
	m_wndToolBar.GetToolBarCtrl().GetRect(ID_FONT_DROPDOWN, &rect);
	rect.top = rect.bottom;
	m_wndToolBar.ClientToScreen( &rect.TopLeft());
	
	CMenu menu;
	CMenu* pPopup;
	pPopup = SMP_FONTS;
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
}

int CMainFrame::IsMonitorVisible()
{
	if (m_pMonitor)
		return m_pMonitor->IsWindowVisible();
	else
		return FALSE;
}

//this will be call when window not shown and restart windows
void CMainFrame::OnEndSession(BOOL bEnding) 
{
	g_bExitAtOnce = TRUE;//not popup dlg
	OnClose();
	CMDIFrameWnd::OnEndSession(bEnding);
	// TODO: Add your message handler code here
}

void CMainFrame::OnToolsSearchText() 
{
	// TODO: Add your command handler code here
	CString str;
	GetHtmlSelectedText(str);
	if (str.GetLength())
	{
		m_wndSearch->GetEditCtrl()->SetWindowText(str);
		PostMessage(WM_COMMAND, ID_SEARCH_OK);
	}
}

void CMainFrame::OnLastVisited() 
{
	// TODO: Add your command handler code here
	CSelectURL dlg;
	dlg.DoModal();
}

BOOL CMainFrame::ConfirmPopupFilter(CString strUrl, BOOL bConfirm)
{
	BOOL bNeedFilter = TRUE;
	if (m_bPlayPopupFilterSound)
	{
		CString strFile = theApp.m_strRoot+"Resource\\PopFilter.wav";
		if (_FileIsExist(strFile))
			::sndPlaySound(strFile, SND_ASYNC);
	}
	//only confirm auto pop window
	//if in user list ad, not pop
	if(m_bNotConfirmPopupFilter) //gfpop (!bConfirm)
		return bNeedFilter;
	
	//gf AfxMessageBox( CGfAppString::GetString(IDS_CONFIRM_POP_BLOCK), MB_OKCANCEL );
	
	//gfpop del
	//	//if in user list ad, not pop
	//	if (m_astrPopup.Find(strUrl) || PopFilterStar(m_astrPopup,strUrl))
	//		return bNeedFilter;
	
	if (!m_bNotConfirmPopupFilter)
	{
		if (strUrl == "about:blank")
			return bNeedFilter;
		CPopupConfirm dlg;
		dlg.m_strURL = strUrl;
		if (dlg.DoModal()==IDCANCEL)
			bNeedFilter = FALSE;
		else
		{
			//gfpop del
			//			if (dlg.m_bAddToPopList)
			//			{
			//				if(m_astrPopup.Find(dlg.m_strURL)==NULL)
			//				{
			//					m_astrPopup.AddTail(dlg.m_strURL);
			//					m_bSaveConfig = TRUE;
			//				}
			//			}
		}
		
		SAVEINT(dlg.m_bNotConfirmPopupFilter,m_bNotConfirmPopupFilter,"Settings", "NotConfirmPopupFilter");
	}
	return bNeedFilter;
}

DWORD CMainFrame::GetDefaultProperty()
{
	DWORD dwProperty = 0;
	
	//load internet setting
	HKEY            hKey;
	TCHAR           sz[MAX_PATH];
	DWORD           dwSize = MAX_PATH;
	
	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Main"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Main setting not found\n");
		return 0;
	}
	dwSize = sizeof(sz);
	RegQueryValueEx(hKey, _T("Display Inline Images"), NULL, NULL, (LPBYTE)sz, &dwSize);
	if(strcmp(sz, "yes")==0)
		dwProperty |=DLCTL_DLIMAGES;
	
	dwSize = sizeof(sz);
	RegQueryValueEx(hKey, _T("Display Inline Videos"), NULL, NULL, (LPBYTE)sz, &dwSize);
	if(strcmp(sz, "yes")==0)
		dwProperty |=DLCTL_VIDEOS;
	
	dwSize = sizeof(sz);
	RegQueryValueEx(hKey, _T("Play_Background_Sounds"), NULL, NULL, (LPBYTE)sz, &dwSize);
	if(strcmp(sz, "yes")==0)
		dwProperty |=DLCTL_BGSOUNDS;
	
	RegCloseKey(hKey);
	
	return dwProperty ;
}

void CMainFrame::SetMessageText(LPCTSTR lpszText)
{
	if (strchr(lpszText, 0x01))
	{
		if (strstr(lpszText,"http://")==lpszText)
		{
			CString strMsg,strTip;
			LOADSTR(strTip ,IDS_TIP_01URL);
			strMsg = lpszText ;
			strMsg += " [" + strTip + "]";
			m_wndStatusBar.SetPaneText(0, strMsg, TRUE);
			return;
		}
	}
	//
	m_wndStatusBar.SetPaneText(0, lpszText, TRUE);
}

CString CMainFrame::GetNetStat(int kind)
{
	double delta1;
	CString strRet;
	delta1 = (double)m_cTrafficClass[kind].GetInterfaceTotalTraffic(0) / (1024.0*1024.0);
	if (delta1<10)
		strRet.Format("%.2fM",delta1);
	else if (delta1<100)
		strRet.Format("%.1fM",delta1);
	else
		strRet.Format("%.0fM",delta1);
	return strRet;
}

CString CMainFrame::GetModemSpeed() 
{
	static CString strSpeed="";
	//
	if(glo_fInitializing)
		return strSpeed;
	
	float interval = 1.0;
	if(m_bNoModemSpeed)
		interval = 5.0;
	
	try{
		LARGE_INTEGER liFinish;
		QueryPerformanceCounter(&liFinish);
		LARGE_INTEGER liFreq;
		QueryPerformanceFrequency(&liFreq);
		float secs = (float)(liFinish.LowPart - m_tPreBytes.LowPart)/(float)liFreq.LowPart;
		if(secs >= interval)
		{
			if(!glo_fInitialized)
			{
				AfxBeginThread(TInitialMonitor, NULL);
				return strSpeed;
			}
			long bytes; 
			GetRASSpeed((DWORD*)&bytes);
			if(bytes == -999)
				m_bNoModemSpeed = TRUE;
			else
				m_bNoModemSpeed = FALSE;
			
			m_tPreBytes = liFinish;
			//get ras BPs
			float bps = ((float)(bytes - m_lPreBytes))/secs;
			if(bytes>=0)
				m_lPreBytes = bytes;
			bps = bps/1024;
			if (bps<10)
				strSpeed.Format("%.2f",bps);
			else if (bps<100)
				strSpeed.Format("%.1f",bps);
			else
				strSpeed.Format("%4.0f",bps);
			return strSpeed;
		}
	}catch(...){}
	
	return strSpeed;
}

#define NETUPDATESPEED	1000	// Value for the statistics timer
CString CMainFrame::GetNetSpeed(int kind)
{
	double delta1;
	CString strRet;
	double traffic = m_cTrafficClass[kind].GetTraffic(0);
	double divisor = (1000.0/(double)NETUPDATESPEED);
	delta1 = (double)(traffic * divisor) / 1024.0;
	if (delta1<10)
		strRet.Format("%.2f",delta1);
	else if (delta1<100)
		strRet.Format("%.1f",delta1);
	else
		strRet.Format("%4.0f",delta1);
	return strRet;
}

#define GFSR_SYSTEMRESOURCES	0
#define GFSR_GDIRESOURCES		1
#define GFSR_USERRESOURCES		2

MEMORYSTATUS  ms;

CString CMainFrame::GetMemory() 
{
	CTime t;
	CTimeSpan ts;
	static CString strRes="";
	
	try{
		t = CTime::GetCurrentTime();
		ts = t - m_tLastRes;  
		int mini = 5;
		if(ts.GetTotalSeconds()>= mini || strRes.GetLength()<1)//m_bWin9x && 
		{
			m_tLastRes = t;
			if (m_hResInst != NULL && g_pGetRes != NULL)
			{
				long lSysRes = (*g_pGetRes)(GFSR_SYSTEMRESOURCES);
				strRes.Format("%d%%", lSysRes);
				return strRes;
			}
			else
			{
				GlobalMemoryStatus(&ms);
				strRes.Format("%dM", ms.dwAvailPhys/1024/1024);
				return strRes;
			}
		}
	}catch(...){}
	
	return strRes;
}

CString CMainFrame::GetLocalIP()
{
	struct hostent * pHostent;
	pHostent = NULL;
	static CString strLastIP="";
	
	try{
		CTime t;
		CTimeSpan ts;
		t = CTime::GetCurrentTime();
		ts = t - m_tLastIPUpdate;  
		int mini = 5, sec = ts.GetTotalSeconds();
		if(sec>mini && strLastIP!="127.0.0.1")
			mini=20;
		if(sec > mini || strLastIP.GetLength()<1)
		{
			m_tLastIPUpdate = t;
			try{
				int i=0,j=0;
				pHostent=gethostbyname(g_szAddr);
				if(pHostent!=NULL && pHostent->h_addr_list[0]!= NULL)
				{
					int i = 0;
					while(pHostent->h_addr_list[i]!= NULL)
					{
						strLastIP.Empty();
						CString strAddr = "";
						for( j = 0; j < pHostent->h_length; j++ )
						{
							if( j > 0 )
								strLastIP += ".";
							strAddr.Format("%u", (unsigned int)((unsigned char*)pHostent->h_addr_list[i])[j]);
							strLastIP += strAddr;
						}
						i++;
					}
					return strLastIP;
				}
		}catch(...){}
		}
	}catch(...){}
	
	return strLastIP;
}

CString CMainFrame::GetOnlineTime()
{
	CString strTime;
	CTimeSpan ts;
	ts = CTime::GetCurrentTime() - m_tStartTime;
	strTime.Format("%02d:%02d:%02d",ts.GetHours(), ts.GetMinutes(), ts.GetSeconds());
	return strTime;
}

void CMainFrame::OnUpdateDisp(CCmdUI* pCmdUI) 
{
	

	try{
		static CString strStatusOld="";
		SYSTEMTIME time;
		::GetLocalTime(&time);
		//
		CString strStatus,strTemp;
		strStatus.Empty();
		TCHAR ch ;
		int i0=0;
		int i=0;
		int len;
		int formatLen = m_strStatusStringFormat.GetLength();
		while(formatLen)//(1)
		{
			ch = m_strStatusStringFormat.GetAt(i);
			if (ch>='A' && ch<='Z')
			{
				len = i - i0;
				if (len)
					strStatus += m_strStatusStringFormat.Mid( i0, i-i0 );
				i0 = i+1;
				switch (ch)
				{
				case 'A': strTemp.Format("%d",time.wYear); strStatus += strTemp; break;
				case 'B': strTemp.Format("%d",time.wMonth); strStatus += strTemp; break;
				case 'C': strTemp.Format("%d",time.wDay); strStatus += strTemp; break;
				case 'D': strTemp.Format("%02d",time.wHour); strStatus += strTemp; break;
				case 'E': strTemp.Format("%02d",time.wMinute); strStatus += strTemp; break;
				case 'F': strTemp.Format("%02d",time.wSecond); strStatus += strTemp; break;
				case 'G': LOADSTR(strTemp ,IDS_SUNDAY+time.wDayOfWeek);strStatus += strTemp; break;
					//
				case 'H': strStatus += GetOnlineTime(); break;
				case 'I': strTemp.Format("%d",m_nWin); strStatus += strTemp; break;
				case 'J': strTemp.Format("%d",m_nPopFiltered); strStatus += strTemp; break;
				case 'K': strStatus += GetLocalIP(); break;
				case 'L': strStatus += GetMemory(); break;
				case 'M': strStatus += GetNetSpeed(0); break;//incoming
				case 'N': strStatus += GetNetSpeed(1); break;//outgoing
				case 'O': strStatus += GetNetSpeed(2); break;//total
				case 'P': strStatus += GetNetStat(0); break;//incoming
				case 'Q': strStatus += GetNetStat(1); break;//outgoing
				case 'R': strStatus += GetNetStat(2); break;//total
				case 'S': strStatus += GetModemSpeed(); break;//total
					//
				case 'T': if (m_bEnProxy) strStatus += m_strCurProxyName; break;
				case 'U': if (m_bEnProxy) strStatus += m_strCurProxy; break;
					//
				default: break;
				}
			}
			i++;
			if (i>=formatLen)
			{
				if (i>i0)
					strStatus += m_strStatusStringFormat.Mid( i0, i-i0 );
				break;
			}
		}
		if (strStatus.GetLength() != strStatusOld.GetLength())
		{
			m_wndStatusBar.SetPaneInfo(3, ID_INDICATOR_DISP, SBPS_NORMAL|SBPS_NOBORDERS, m_nBaseWidth*strStatus.GetLength());
			m_wndStatusBar.AdjustToolbarPosition();
			m_wndStatusBar.AdjustProgressBarPosition();
		}
		//
		if (strStatusOld != strStatus)
		{
			strStatusOld = strStatus;
			pCmdUI->SetText(strStatus);
		}
		
	}catch(...){}
}

BOOL CMainFrame::IsImageType(LPCTSTR lpszText)
{
	BOOL bImage=FALSE;
	char *temp = strrchr(lpszText,'.');
	if (temp)
	{
		if (stricmp(temp, ".gif")==0 ||
			stricmp(temp, ".jpg")==0 ||
			stricmp(temp, ".jpeg")==0 ||
			stricmp(temp, ".bmp")==0 ||
			stricmp(temp, ".ico")==0 ||
			stricmp(temp, ".pcx")==0 ||
			stricmp(temp, ".tif")==0 ||
			stricmp(temp, ".png")==0 ||
			stricmp(temp, ".wmf")==0
			)
			bImage = TRUE;
	}
	return bImage;
}

HBITMAP	CMainFrame::GetBitmap(LPCTSTR lpszText)
{
	CMyIE9App* app;
	app = (CMyIE9App*)AfxGetApp();
	CString filename = _app->m_strSkinPath + m_strSkinName + "\\" + lpszText;
	if (!_FileIsExist(filename))
	{
		filename = _app->m_strSkinPath + "default" + "\\" + lpszText;
		if (!_FileIsExist(filename))
			return NULL;
	}
	HBITMAP hbmp = (HBITMAP)LoadImage(
		NULL,
		filename,
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE
		);
	return hbmp;
}

void CMainFrame::InitTaskBarImage()
{
	CImageList img;
	HBITMAP hbmp;
	
	img.Create(16, 16, ILC_COLOR32|ILC_MASK, TI_COUNT,1);
	hbmp = GetBitmap("TaskBar.bmp");
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);
	
	//if bmp not right, get default bmp
	int nCount = img.GetImageCount();
	if (nCount<TI_COUNT)
	{
		CImageList imgDefault;
		imgDefault.Create(16, 16, ILC_COLORDDB|ILC_MASK, TI_COUNT,1);
		hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_TASKBAR));
		ImageList_AddMasked(imgDefault.GetSafeHandle(), hbmp, RGB(255,0,255));
		DeleteObject(hbmp);
		//
		img.SetImageCount(TI_COUNT);
		//
		int nBegin;
		if (nCount==8)
			nBegin = 8;
		else
			nBegin = 0;
		//
		HICON hIcon;
		for (int i=nBegin; i<TI_COUNT; i++)
		{
			hIcon = imgDefault.ExtractIcon(i);
			img.Replace(i,hIcon);
		}
		imgDefault.Detach();
	}
	
	CImageList *pImg = m_wndTab.GetImageList();
	if (pImg)
	{
		HICON hIcon;
		for (int i=0; i<TI_COUNT; i++)
		{
			hIcon = img.ExtractIcon(i);
			pImg->Replace(i,hIcon);
		}
	}
	else
		m_wndTab.SetImageList(&img);
	//
	img.Detach();

	//gfx
	{
		
		CImageList img;
		HBITMAP hbmp;
		
		img.Create(16, 16, ILC_COLOR32|ILC_MASK, GF_TI_COUNT_X_ICONS,1);
		hbmp = GetBitmap("TabClose.bmp");
		
		ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
		DeleteObject(hbmp);
		
		
		CImageList *pImg = m_wndTab.GetImageList();
		if (pImg)
		{
			HICON hIcon;
			for (int i=0, des=TI_COUNT; i<GF_TI_COUNT_X_ICONS; i++, des++)
			{
				hIcon = img.ExtractIcon(i);
				int iRet = pImg->Replace(des, hIcon);
				
				if (-1 == iRet)
				{
					pImg->Add(hIcon);
					
				}
			}
		}
		
		//
		img.Detach();
		
	}


	//gfb
	{
		DeleteTabBkImgs();

		try
		{
		 hbTabBkTop=GetBitmap("TabNormal.bmp");
		 hbTabBkTopActive = GetBitmap("TabActive.bmp");

		 hbTabBkBottom = GetBitmap("TabNormalBottom.bmp");
		 hbTabBkBottomActive = GetBitmap("TabActiveBottom.bmp");

		}
		catch (...) {ASSERT(false);}

		BOOL bExistTabImg = hbTabBkBottomActive && hbTabBkBottom && hbTabBkTopActive && hbTabBkTop;

		if (!bExistTabImg && (m_nTabStyle == CBaseTabCtrl::SKIN_TAB) )
		{
			m_nTabStyle = CBaseTabCtrl::FLAT_TAB; 
		}

	}

}

void CMainFrame::InitGoImage()
{
	CImageList *oldimg;
	oldimg = m_AddressBar.GetToolBarCtrl().GetImageList();
	if(oldimg!=NULL)
		oldimg->DeleteImageList();
	
	CImageList img;
	HBITMAP hbmp;
	
	img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 1, 1);
	hbmp = GetBitmap("Go.bmp");
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);
	m_AddressBar.SendMessage(TB_SETIMAGELIST, 0, (LPARAM)img.m_hImageList);
	img.Detach();
}

void CMainFrame::InitMenuBarImage()
{
	HBITMAP hbmp;
	
	//image list
	//ImageList_RemoveAll(m_wndMenuBar.m_ImgList.GetSafeHandle());
	m_wndMenuBar.m_ImgList.DeleteImageList();
	//
	hbmp = GetBitmap("FavBar.bmp");
	m_wndMenuBar.m_ImgList.Create(16, 16, ILC_COLORDDB|ILC_MASK, 2, 1);
	ImageList_AddMasked(m_wndMenuBar.m_ImgList.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);
	
	//image menu
	//ImageList_RemoveAll(m_wndMenuBar.m_ImgMenu.GetSafeHandle());
	m_wndMenuBar.m_ImgMenu.DeleteImageList();
	//
	hbmp = GetBitmap("MainTool16.bmp");
	m_wndMenuBar.m_ImgMenu.Create(16, 16, ILC_COLORDDB|ILC_MASK, 80, 1);
	ImageList_AddMasked(m_wndMenuBar.m_ImgMenu.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);
	hbmp = GetBitmap("SystemBar.bmp");
	ImageList_AddMasked(m_wndMenuBar.m_ImgMenu.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);
	hbmp = GetBitmap("MainMenu.bmp");
	ImageList_AddMasked(m_wndMenuBar.m_ImgMenu.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);
}

void CMainFrame::InitSystemBarImage()
{
	CImageList *oldimg;
	oldimg = m_SysMenuBar.GetToolBarCtrl().GetImageList();
	if(oldimg!=NULL)
		oldimg->DeleteImageList();
	
	CImageList img;
	HBITMAP hbmp;
	
	hbmp = GetBitmap("SystemBar.bmp");
	img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 6, 1);
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);
	m_SysMenuBar.GetToolBarCtrl().SetImageList(&img);
	img.Detach();
}

void CMainFrame::InitLinkBarImage()
{
	CImageList *oldimg;
	oldimg = m_LinkBar.GetToolBarCtrl().GetImageList();
	if(oldimg!=NULL)
		oldimg->DeleteImageList();
	//
	if (g_bCompactFavBar)
	{
		m_LinkBar.GetToolBarCtrl().SetImageList(NULL);
		return;
	}
	
	CImageList img;
	HBITMAP hbmp;
	img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 2, 1);
	hbmp = GetBitmap("FavBar.bmp");
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);
	m_LinkBar.GetToolBarCtrl().SetImageList(&img);
	img.Detach();
}

void CMainFrame::InitFavBarImage()
{
	CImageList *oldimg;
	oldimg = m_FavBar.GetToolBarCtrl().GetImageList();
	if(oldimg!=NULL)
		oldimg->DeleteImageList();
	//
	if (g_bCompactFavBar)
	{
		m_FavBar.GetToolBarCtrl().SetImageList(NULL);
		return;
	}
	
	CImageList img;
	HBITMAP hbmp;
	img.Create(16, 16, ILC_COLORDDB|ILC_MASK, 2, 1);
	hbmp = GetBitmap("FavBar.bmp");
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	DeleteObject(hbmp);
	m_FavBar.GetToolBarCtrl().SetImageList(&img);
	img.Detach();
}

void CMainFrame::InitMainAnimIcon()
{
	CImageList *oldimg;
	oldimg = m_animIcon.GetImageList();
	if(oldimg!=NULL)
		oldimg->DeleteImageList();
	
	HBITMAP hbmp;
	hbmp = GetBitmap("MainAnimIcon.bmp");
	m_animIcon.SetImageList(hbmp,8,RGB(255,0,255));
}

void CMainFrame::OnWindowManager() 
{
	// TODO: Add your command handler code here
	CSelectURL dlg;
	dlg.m_nUrlType = 2;
	dlg.DoModal();
}

void CMainFrame::OnUpdateWindowManager(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
		CMenu* pPopupMenu=pCmdUI->m_pMenu;
		if(pPopupMenu != NULL)
		{
			//window list
			DestroyMenuData(pPopupMenu->m_hMenu,9);
			while(pPopupMenu->DeleteMenu(9, MF_BYPOSITION));
			
			CChildFrame* tcf;
			m_nWin=m_wndTab.GetItemCount();
			int n = m_nWin;
			int sel = m_wndTab.GetCurSel();
			CString strm;
			TCITEM TabCtrlItem;
			n=n>250?250:n;
			
			CMenuData * md;
			TabCtrlItem.mask = TCIF_PARAM|TCIF_IMAGE;
			for(int i=0;i<n;i++)
			{
				TabCtrlItem.lParam = 0;
				TabCtrlItem.iImage = 0;
				m_wndTab.GetItem(i, &TabCtrlItem);
				tcf = ((CChildFrame*)TabCtrlItem.lParam);
				if(tcf != NULL && tcf->m_pView != NULL)
				{
					md = NULL;
					md= new CMenuData;
					if(md != NULL)
					{
						md->m_pImgList = NULL;//&m_TabImgList;
						//md->m_nBitmap=TabCtrlItem.iImage;
						GET_TAB_TITLE(tcf,strm);
						AdjustMenuWidth(strm);
						if(strm.IsEmpty())
							strm = "about:blank";
						SetMenuText(strm, md);
						pPopupMenu->AppendMenu ( MF_ENABLED|MF_OWNERDRAW, B_WINDOW + i , (char*)md );
						if(sel == i)
							pPopupMenu->SetDefaultItem(B_WINDOW+i);
					}
				}
			}
			AddMenuBreak(pPopupMenu, 0);
		}
	}catch(...){}
}

UINT _cdecl SaveExUtilsList(LPVOID con)
{
	CMyIE9App* app;
	app = (CMyIE9App*)AfxGetApp();
	char Keyname1[10], Keyname2[11], Keyname3[11], Keyname4[11], Keyname5[11];
	char Keyname11[10], Keyname12[11];
	int i = 1;
	WritePrivateProfileSection("ExUtils", NULL, theApp.m_strProfile);
	strcpy(Keyname1, "name"); 
	strcpy(Keyname2, "cmds");
	strcpy(Keyname11, "para"); 
	strcpy(Keyname12, "path");
	strcpy(Keyname3, "sta");
	strcpy(Keyname4, "cls");
	strcpy(Keyname5, "run");
	CExternalUtilItem* eui;
	for (int ir=0;ir <= pmf->m_ExternalUtilList.m_UtilList.GetUpperBound();ir++)
	{
		eui = pmf->m_ExternalUtilList.m_UtilList.GetAt(ir);
		itoa(ir+1, Keyname1+4, 10);
		itoa(ir+1, Keyname2+4, 10);
		itoa(ir+1, Keyname11+4, 10);
		itoa(ir+1, Keyname12+4, 10);
		
		itoa(ir+1, Keyname3+3, 10);
		itoa(ir+1, Keyname4+3, 10);
		itoa(ir+1, Keyname5+3, 10);
		app->WriteProfileString("ExUtils", Keyname1, eui->m_strUtilName);
		app->WriteProfileString("ExUtils", Keyname2, eui->m_strUtilCmd);
		if (eui->m_strUtilPara.GetLength())
			app->WriteProfileString("ExUtils", Keyname11, eui->m_strUtilPara);
		if (eui->m_strUtilPath.GetLength())
			app->WriteProfileString("ExUtils", Keyname12, eui->m_strUtilPath);
		if (eui->m_bUtilStart)
			app->WriteProfileInt("ExUtils", Keyname3, eui->m_bUtilStart);
		if (eui->m_bUtilClose)
			app->WriteProfileInt("ExUtils", Keyname4, eui->m_bUtilClose);
		if (eui->m_bUtilRun)
			app->WriteProfileInt("ExUtils", Keyname5, eui->m_bUtilRun);
	}
	return 0;
}

void CMainFrame::OnToolsExternalUtilitymanager() 
{
	// TODO: Add your command handler code here
	CExternalUtils dlg;
	if(dlg.DoModal()==IDOK)
	{
		AfxBeginThread(SaveExUtilsList, NULL);
		BuildUtilMenu();
	}
}

void CMainFrame::BuildUtilMenu()
{
	CMenu *		pMenu;
	BOOL		bMax;
	// first get rid of bogus submenu items.
	MDIGetActive(&bMax);
	// for the favorties popup, just steal the menu from the main window
	pMenu = SMP_EXTOOLS;
	DestroyMenuData(pMenu->m_hMenu,4);
	while(pMenu->DeleteMenu(4, MF_BYPOSITION));
	
	//remove all link buttons
	int b = m_ExternalTool.GetToolBarCtrl().GetButtonCount();
	for(int i = 0; i<b;i++)
		m_ExternalTool.GetToolBarCtrl().DeleteButton(0);
	m_ExternalTool.SetButtons(NULL,m_ExternalUtilList.m_UtilList.GetSize());
	
	//add menus d00-df0
	int nPos = 0;
	SHFILEINFO shFinfo;
	int iIcon;
	CString  strTemp;
	CExternalUtilItem* eui;
	BOOL bIsPlugIn ;
	CString strPlugInComments;
	for (i=0;i <= m_ExternalUtilList.m_UtilList.GetUpperBound();i++)
	{
		eui = m_ExternalUtilList.m_UtilList.GetAt(i);
		if(eui==NULL)
			goto _DoNext;
		if (eui->m_strUtilName.Left(2) == "--")
		{
			MenuAppendSeparator(pMenu);
			m_ExternalTool.SetButtonInfo(nPos,NULL,TBSTYLE_SEP,0);
			goto _DoNext;
		}
		MenuAppendOwnerItem( pMenu, MF_STRING | MF_ENABLED, B_EX_TOOL + nPos, eui->m_strUtilName);
		if(eui->m_bUtilRun)
			pMenu->CheckMenuItem(B_EX_TOOL + nPos, MF_CHECKED);
		
		strTemp = eui->m_strUtilCmd;
		DO_CMD_STR(strTemp);
		
		bIsPlugIn = false;
		if (_GetPlugInType(strTemp))
		{
			bIsPlugIn = true;
			strPlugInComments = _GetPlugInComments(strTemp);
			strTemp = _GetPlugInIconPath(strTemp);
		}
		if ( strTemp.Right(1) != '\\' )
			strTemp += "\\";
		//
		if ( SHGetFileInfo( strTemp,
			0,
			&shFinfo,
			sizeof( shFinfo ),
			SHGFI_ICON | 
			SHGFI_SMALLICON ) )
		{
			
			iIcon = shFinfo.iIcon;
			// we only need the index from the system image list
			DestroyIcon( shFinfo.hIcon );
		}
		else
			iIcon = -1;
		//
		m_ExternalTool.SetButtonInfo(nPos,B_EX_TOOL + nPos,TBSTYLE_BUTTON,iIcon);
		
		strTemp = eui->m_strUtilName;
		if(eui->m_bUtilStart)
			strTemp = "*"+strTemp;
		if (bIsPlugIn)
		{
			if (strPlugInComments.GetLength())
			{
				strTemp += "\n";
				strPlugInComments.Replace("\\n","\r\n");
				strTemp += strPlugInComments;
			}
		}
		else
		{
			strTemp += "\n";
			strTemp += eui->m_strUtilCmd;
		}
		m_ExternalTool.GetToolBarCtrl().GetToolTips()->UpdateTipText(strTemp, &m_ExternalTool, B_EX_TOOL+nPos);
		//
_DoNext:
		nPos++;
	}
	
	CRect rectToolBar;
	REBARBANDINFO rbbi;
	m_ExternalTool.GetItemRect(0, &rectToolBar);
	
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE /*| RBBIM_SIZE*/ |RBBIM_ID ;
	rbbi.wID = ID_VIEW_EX_TOOL;
	rbbi.cxMinChild = 0;//rectToolBar.Width()+2;
	if(rectToolBar.Height()<22)
		rbbi.cyMinChild = 22;
	else
		rbbi.cyMinChild = rectToolBar.Height();
	
	int bc=m_ExternalTool.GetToolBarCtrl().GetButtonCount();
	if(bc>0)
	{
		m_ExternalTool.GetToolBarCtrl().GetItemRect(bc-1,&rectToolBar);
		/*rbbi.cx =*/ rbbi.cxIdeal = rectToolBar.right;
	}
	else
		/*rbbi.cx =*/ rbbi.cxIdeal = 2;
		
		int bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_EX_TOOL);
	m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
	//
	AddMenuBreak(pMenu, 0);
}

void CMainFrame::OnViewExTool() 
{
	try{
		
		// TODO: Add your command handler code here
		REBARBANDINFO rbbi;
		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
		int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(ID_VIEW_EX_TOOL);
		m_wndReBar.GetReBarCtrl().GetBandInfo(nb, &rbbi);
		
		if( rbbi.fStyle & RBBS_HIDDEN  || !m_ExternalTool.IsWindowVisible())
		{
			m_ExternalTool.ShowWindow(SW_SHOW);
			m_wndReBar.GetReBarCtrl( ).ShowBand(nb, TRUE);
			GetMenu()->CheckMenuItem(ID_VIEW_EX_TOOL, MF_CHECKED);
			
		}
		else
		{
			m_wndReBar.GetReBarCtrl( ).ShowBand(nb,FALSE);
			m_ExternalTool.ShowWindow(SW_HIDE);
			GetMenu()->CheckMenuItem(ID_VIEW_EX_TOOL, MF_UNCHECKED);
		}
		
	}catch (...) {ASSERT(false);	}
}

void CMainFrame::OnUtils(UINT nID)
{
	try{
		
		RunUtils(nID,NULL);
	}catch (...) {ASSERT(false);	}
}

void CMainFrame::RunUtils(UINT nID,CChildFrame* tcf)
{
	int id = nID-B_EX_TOOL;
	if(	id <= m_ExternalUtilList.m_UtilList.GetUpperBound() && id>=0)
	{
		CExternalUtilItem* eui;
		eui = m_ExternalUtilList.m_UtilList.GetAt(id);
		if (!eui)
			return;
		//only for click external item
		if (!tcf)
		{
			short cs=GetKeyState(VK_CONTROL), ss=GetKeyState(VK_SHIFT);
			if ( cs<0 && ss>=0 )
			{
				eui->m_bUtilRun = !eui->m_bUtilRun;
				BuildUtilMenu();
				return;
			}
			else if ( cs>=0 && ss<0 )
			{
				//clear all
				CExternalUtilItem* euiTemp;
				for (int i=0;i <= m_ExternalUtilList.m_UtilList.GetUpperBound();i++)
				{
					euiTemp = m_ExternalUtilList.m_UtilList.GetAt(i);
					if (euiTemp)
						euiTemp->m_bUtilRun = FALSE;
				}
				//select this
				eui->m_bUtilRun = TRUE;
				BuildUtilMenu();
				return;
			}
		}
		//
		if (!eui->m_strUtilCmd)
			return;
		CString strTemp = eui->m_strUtilCmd;
		
		DO_CMD_STR(strTemp);
		//must after do cmd str, for relative path
		int nType = _GetPlugInType(strTemp);
		if (nType)
		{
			CString strFileName = _GetPlugInFileName(strTemp);
			strFileName = GET_ROOT(strTemp) + strFileName;
			//
			if (nType==1)
			{
				char *tmp = _FileGetBuffer(strFileName);
				CString strCode = tmp;
				FREENULL(tmp);
				if (strCode.GetLength())
				{
					if (tcf)
						RunOneScript(strCode,tcf);
					else
						RunScript(strCode);
				}
				return;
			}
			else if (nType==2)
				strTemp = strFileName;//go next code
		}
		//
		CString path;
		//
		if (eui->m_strUtilPath.GetLength()>1)
			path = eui->m_strUtilPath;
		else
		{
			path = strTemp;
			int i = path.ReverseFind('\\');
			path = path.Left(i+1);
		}
		CString strPara = eui->m_strUtilPara;
		if (strPara=="$current_url")
		{
			CChildFrame* tcf = (CChildFrame*)MDIGetActive();
			if (!tcf || !tcf->m_pView)
				strPara.Empty();
			else
			{
				strPara = tcf->m_pView->m_lpszUrl;
				//some app can not accept file:
				if (strPara.Left(8)=="file:///")
				{
					strPara = strPara.Mid(8);
					strPara.Replace('/','\\');
					strPara.Replace("%20"," ");
				}
			}
		}
		if((UINT)ShellExecute(NULL, "open", strTemp , strPara, path, SW_SHOW )<=32)
			MSGBOX(IDS_UTIL_FAIL);
	}
}

void CMainFrame::CloseExternalUtil()
{
	try{
		CExternalUtilItem* eui;
		for (int i=0;i <= m_ExternalUtilList.m_UtilList.GetUpperBound();i++)
		{
			eui = m_ExternalUtilList.m_UtilList.GetAt(i);
			if(eui!=NULL)
			{
				if(eui->m_iProcess!=0 && eui->m_bUtilClose)
				{
					CString strTemp = eui->m_strUtilCmd;
					
					DO_CMD_STR(strTemp);
					
					if (_GetPlugInType(strTemp)!=1)
						TerminateApp(eui->m_iProcess, 500);
				}
			}
		}
	}catch(...){}
}

void CMainFrame::StartUtil(int i)
{
	CExternalUtilItem* eui = m_ExternalUtilList.m_UtilList.GetAt(i);
	
	if(eui!=NULL)
	{
		CString cmd = eui->m_strUtilCmd;
		DO_CMD_STR(cmd);
		if (_GetPlugInType(cmd)==1)
			return;
		STARTUPINFO si; 
		PROCESS_INFORMATION pi; 
		ZeroMemory ( &si, sizeof ( STARTUPINFO));
		si.cb = sizeof ( STARTUPINFO); 
		si.dwFlags = STARTF_USESHOWWINDOW; 
		si.wShowWindow = SW_SHOWNORMAL; // use SW_SHOWNORMAL if you want the spawned app to be visible 
		
		BOOL bRes = CreateProcess ( NULL, 
			(LPSTR)(LPCTSTR)cmd, // command line 
			NULL, 
			NULL, 
			TRUE, 
			NORMAL_PRIORITY_CLASS, 
			GetEnvironmentStrings (), 
			NULL, 
			&si, 
			&pi 
			);
		CloseHandle( pi.hProcess); 
		CloseHandle( pi.hThread); 
		
		eui->m_iProcess = pi.dwProcessId;
	}
}

void CMainFrame::OnFileNewSelectedLink() 
{
	GET_ACTIVE
		//
		IHTMLDocument2 * pDoc = NULL;
	IHTMLSelectionObject *pSelection;
	IDispatch *pRange;
	IHTMLTxtRange *pText;
	//
	IHTMLBodyElement * pBody = NULL;
	IHTMLElement *pElemBody = NULL;
	IHTMLTxtRange *pTextLink;//link text rang
	//
	IHTMLElementCollection  * pAllElem = NULL;
	
	IHTMLElement *pElem = NULL;
	IHTMLAnchorElement * pAnchor = NULL;
	
	VARIANT_BOOL vRet;
	long p;
	BSTR bstrSrc;
	VARIANT name;
	name.vt = VT_I4;
	
	try{
		pDoc = (IHTMLDocument2*)(tcf->m_pView->GetHtmlDocument());
		if(pDoc!=NULL)
		{
			pDoc->get_selection(&pSelection);
			if (pSelection)
			{
				pSelection->createRange(&pRange);
				if (pRange)
					pRange->QueryInterface(&pText);
			}
			pDoc->get_body(&pElemBody);
			if(pElemBody!=NULL)
			{
				pElemBody->QueryInterface(&pBody);
				if (pBody)
					pBody->createTextRange(&pTextLink);
			}
			//
			pDoc->get_links(&pAllElem);
			if(pAllElem!=NULL && pText && pTextLink)
			{
				pAllElem->get_length(&p);
				if(p>0)
				{
					for(int i=0; i<p; i++)
					{
						name.lVal = i;
						if(pAllElem->item(name, name, (LPDISPATCH*)&pElem)==S_OK)
						{
							if(pElem != NULL)
							{
								pTextLink->moveToElementText(pElem);
								pText->inRange(pTextLink,&vRet);
								if (vRet!=FALSE)
								{
									pElem->QueryInterface(&pAnchor);
									if(pAnchor!=NULL)
									{
										pAnchor->get_href(&bstrSrc);
										CString s1(bstrSrc);
										s1.TrimLeft();s1.TrimRight();
										SysFreeString(bstrSrc);
										if (m_bActiveFileNew)
											NewChildWindow(1,2, s1,NULL,TRUE);
										else
											NewChildWindow(1,2, s1);
										//
										pAnchor->Release();
									}							
								}
								pElem->Release();
							}
						}
					}
				}
			}
		}
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
	
	try{
		if (pDoc)	pDoc->Release();
		if (pSelection)	pSelection->Release();
		if (pRange)	pRange->Release();
		if (pText)	pText->Release();
		if (pBody)	pBody->Release();
		if (pElemBody)	pElemBody->Release();
		if (pTextLink)	pTextLink->Release();
		if (pAllElem)	pAllElem->Release();
	}catch(...){}
}

BOOL CMainFrame::Gf_FindSearchBar(const CString& keyw, CMyIE9View * pView) 
{
	BOOL bRet = TRUE;
	
	int iOldFindIndex = pView->m_nFindIndex;
	CString strOldFindKey = pView->m_strFindKey;
	int nOldFindMax = pView->m_nFindMax;
	
	//
	if (pView->m_strFindKey != keyw)
	{
		if (!m_bSearchMatchCase && keyw.CompareNoCase(pView->m_strFindKey)==0)
			;
		else
		{
			pView->m_nFindIndex = -1;
			
			pView->m_nFindMax = GetPageKeyWordCount(keyw);
			if (pView->m_nFindMax>0)
				pView->m_nFindMax--;
		}
		pView->m_strFindKey = keyw;
	}		
	//
	IHTMLDocument2 * pDoc = NULL;
	IHTMLElement *pElem = NULL;
	IHTMLBodyElement * pBody = NULL;
	
	try{	
		pDoc = (IHTMLDocument2*)(pView->GetHtmlDocument());
		if(pDoc!=NULL)
		{
			pElem = NULL;
			pDoc->get_body(&pElem);
			if(pElem!=NULL)
			{
				pElem->QueryInterface(&pBody);
				if(pBody!=NULL)
				{
					IHTMLTxtRange *pTxtRang;
					pBody->createTextRange(&pTxtRang);
					if (pTxtRang!=NULL)
					{
						BSTR bstrKey;
						bstrKey = keyw.AllocSysString();
						//
						CString msg;
						char strCount[20];
						int i=0;
						VARIANT_BOOL vRet;
						if (PRESS_SHIFT)
							pView->m_bFindPrev = TRUE;
						//
						if (pView->m_bFindPrev)
						{
							if (pView->m_nFindIndex == -1)
								pView->m_nFindIndex = pView->m_nFindMax;
							else
								pView->m_nFindIndex--;
						}
						else
						{
							pView->m_nFindIndex++;
						}
						//free the flag
						int bFindPrevOld = pView->m_bFindPrev;
						pView->m_bFindPrev = FALSE;
						//go
						while(1)
						{
							pTxtRang->findText(bstrKey, TRUE, m_bSearchMatchWhole*2 | m_bSearchMatchCase*4, &vRet);
							if (vRet==FALSE)
							{
								if (pView->m_nFindIndex==0)//not found
								{
									//										LOADSTR(msg ,IDS_FIND_KEY_INDEX);
									//										msg += "0/0";
									//										SetMessageText(msg);
									
									//gf
									bRet = FALSE;
									break;
								}
								else//search first again
								{
									pView->m_nFindIndex=-1;
									PostMessage(WM_COMMAND, ID_FIND_SEARCH_BAR);
									pView->m_bFindPrev = bFindPrevOld;
									break;
								}
							}
							if (pView->m_nFindIndex==i)
							{
								pTxtRang->select();
								pTxtRang->scrollIntoView(1);
								
								//
								LOADSTR(msg ,IDS_FIND_KEY_INDEX);
								itoa(i+1,strCount,10);
								msg += strCount;
								msg += "/";
								itoa(pView->m_nFindMax+1,strCount,10);
								msg += strCount;
								SetMessageText(msg);
								break;
							}
							i++;
							pTxtRang->collapse(false);
						}
						SysFreeString(bstrKey);
						pTxtRang->Release();
					}
				}
			}
		}
	}catch(...){}
	//
	try{
		if(pBody != NULL)
			pBody->Release();
		if(pDoc != NULL)
			pDoc->Release();
		if(pElem != NULL)
			pElem->Release();
	}catch(...){}
	
	if (!bRet)
	{	
		pView->m_nFindIndex = iOldFindIndex;
		pView->m_strFindKey = strOldFindKey;
		pView->m_nFindMax = nOldFindMax;
		
	}
	
	return bRet;
}

void CMainFrame::OnFindSearchBar() 
{
	try
	{
		
		// TODO: Add your command handler code here
		GET_ACTIVE
			
			CMyIE9View * pView = NULL;
		pView = tcf->m_pView;
		
		CString keyw;
		m_wndSearch->GetEditCtrl()->GetWindowText(keyw);
		keyw.TrimLeft();
		keyw.TrimRight();
		if (keyw.GetLength()<=0)
		{
			MSGBOX(IDS_NO_SEARCH_KEYWORD);
			return;
		}
		//save typed keyword
		if (keyw.GetLength() && m_bNotSaveKeyword==FALSE)
		{
			POSITION pos = m_astrTypedSearch.Find(keyw);
			if (pos==NULL)
			{
				m_astrTypedSearch.AddHead(keyw);	//add at head
				//
				COMBOBOXEXITEM comboitem;
				comboitem.mask = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE ;
				comboitem.iImage = 1;
				comboitem.iSelectedImage = 1;
				comboitem.iItem = 0;
				comboitem.pszText = (LPTSTR)(LPCTSTR)keyw;
				m_wndSearch->InsertItem(&comboitem);
			}
		}
		
		
		BOOL bRet = FALSE;
		
		BOOL bOldFindPrev = pView->m_bFindPrev;
		
		bRet = Gf_FindSearchBar(keyw, pView);
		if (bRet)
		{
			return;
		}
		
		CStringArray stlKeys;
		CHelperStr::SplitStr(stlKeys, keyw, " ");
		for (int i = 0; i< stlKeys.GetSize(); i++)
		{
			CString strItem = stlKeys[i];
			
			CHelperStr::Trim(strItem);
			if (strItem.IsEmpty())
			{
				continue;
			}
			
			pView->m_bFindPrev = bOldFindPrev;
			bRet = Gf_FindSearchBar(strItem, pView);
			if (bRet)
			{
				break;
			}
		}
		
		if (!bRet)
		{
			CString msg = "";
			LOADSTR(msg ,IDS_FIND_KEY_INDEX);
			msg += "0/0";
			SetMessageText(msg);
		}
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::Gf_HighLightSearchBar(const CString& keyw, CMyIE9View * pView) 
{
	IHTMLDocument2 * pDoc = NULL;
	IHTMLElement *pElem = NULL;
	IHTMLBodyElement * pBody = NULL;
	
	try{	
		pDoc = (IHTMLDocument2*)(pView->GetHtmlDocument());
		if(pDoc!=NULL)
		{
			pElem = NULL;
			pDoc->get_body(&pElem);
			if(pElem!=NULL)
			{
				pElem->QueryInterface(&pBody);
				if(pBody!=NULL)
				{
					IHTMLTxtRange *pTxtRang;
					pBody->createTextRange(&pTxtRang);
					if (pTxtRang!=NULL)
					{
						BSTR bstrKey;
						bstrKey = keyw.AllocSysString();
						//
						BSTR bstr1 = SysAllocString(L"BackColor");
						BSTR bstr2 = SysAllocString(L"ForeColor");
						VARIANT v1;
						v1.vt = VT_BSTR;
						if (!pView->m_bHighLighted)
							v1.bstrVal = SysAllocString(L"yellow");
						else
							v1.bstrVal = SysAllocString(L"");						
						VARIANT v2;
						v2.vt = VT_BSTR;
						if (!pView->m_bHighLighted)
							v2.bstrVal = SysAllocString(L"red");
						else
							v2.bstrVal = SysAllocString(L"");
						//
						int i=0;
						VARIANT_BOOL vRet;
						BOOL bFirstOne = TRUE;
						while(1)
						{
							pTxtRang->findText(bstrKey, TRUE, m_bSearchMatchWhole*2 | m_bSearchMatchCase*4, &vRet);
							if (vRet==FALSE)
								break;
							i++;
							pTxtRang->execCommand(bstr1,FALSE,v1, &vRet);
							pTxtRang->execCommand(bstr2,FALSE,v2, &vRet);
							pTxtRang->collapse(false);
							if (bFirstOne && !pView->m_bHighLighted)
							{
								pTxtRang->scrollIntoView(1);
								bFirstOne = FALSE;
							}
						}
						SysFreeString(bstrKey);
						SysFreeString(bstr1);
						SysFreeString(bstr2);
						SysFreeString(v1.bstrVal);
						SysFreeString(v2.bstrVal);
						pTxtRang->Release();
						
						//gf
						i += pView->m_iHighLightedCount;
						pView->m_iHighLightedCount = i;
						
						
						CString msg;
						char strCount[20];
						LOADSTR(msg ,IDS_KEYWORD_COUNT);
						itoa(i,strCount,10);
						if (pView->m_bHighLighted)
						{
							if (i>0)
								msg += "-"; //means not highlight
						}
						msg += strCount;
						SetMessageText(msg);
						
						//gf
						// 			if (GetFocus() == (CWnd*)m_wndSearch->GetEditCtrl())
						//							{
						//								tcf->ViewSetFocus();
						// 							}
					}
				}
			}
		}
	}catch(...){}
	
	try{
		if(pBody != NULL)
			pBody->Release();
		if(pDoc != NULL)
			pDoc->Release();
		if(pElem != NULL)
			pElem->Release();
	}catch(...){}
	
}

void CMainFrame::OnHighLightSearchBar() 
{
	try
	{
		
		// TODO: Add your command handler code here
		GET_ACTIVE
			
			CMyIE9View * pView = NULL;
		pView = tcf->m_pView;
		
		CString keyw;
		if (g_nSearchType==ST_ADDRBAR)
		{
			m_wndAddress->GetEditCtrl()->GetWindowText(keyw);
			keyw.TrimLeft();keyw.TrimRight();
			if (keyw.GetLength())
				m_wndSearch->GetEditCtrl()->SetWindowText(keyw);
		}
		else
			m_wndSearch->GetEditCtrl()->GetWindowText(keyw);
		g_nSearchType=ST_SEARCHBAR;
		keyw.TrimLeft();
		keyw.TrimRight();
		if (keyw.GetLength()<=0)
		{
			MSGBOX(IDS_NO_SEARCH_KEYWORD);
			return;
		}
		//save typed keyword
		if (keyw.GetLength() && m_bNotSaveKeyword==FALSE)
		{
			POSITION pos = m_astrTypedSearch.Find(keyw);
			if (pos==NULL)
			{
				m_astrTypedSearch.AddHead(keyw);	//add at head
				//
				COMBOBOXEXITEM comboitem;
				comboitem.mask = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE ;
				comboitem.iImage = 1;
				comboitem.iSelectedImage = 1;
				comboitem.iItem = 0;
				comboitem.pszText = (LPTSTR)(LPCTSTR)keyw;
				m_wndSearch->InsertItem(&comboitem);
			}
		}
		//
		if (pView->m_strHighLightKey != keyw)
		{
			pView->m_strHighLightKey = keyw;
			pView->m_bHighLighted = 0;
		}
		
		//gf
		pView->m_iHighLightedCount = 0;
		
		Gf_HighLightSearchBar(keyw, pView);
		
		if (pView->m_iHighLightedCount > 0)
		{
			pView->m_bHighLighted = 1- pView->m_bHighLighted;
			
			if (GetFocus() == (CWnd*)m_wndSearch->GetEditCtrl())
			{
				tcf->ViewSetFocus();
			}
			
			return;
		}
		
		CStringArray stlList;
		CHelperStr::SplitStr(stlList, keyw, " ");
		
		for (int i = 0; i < stlList.GetSize(); i++)
		{
			CString strItem = stlList[i];
			if (strItem.IsEmpty())
			{
				continue;
			}
			
			Gf_HighLightSearchBar(strItem, pView);
		}
		
		pView->m_bHighLighted = 1- pView->m_bHighLighted;
		
		
		if (GetFocus() == (CWnd*)m_wndSearch->GetEditCtrl())
		{
			tcf->ViewSetFocus();
		}
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

//count key word number in the page
int CMainFrame::GetPageKeyWordCount(CString keyw) 
{
	// TODO: Add your command handler code here
	int i=0;
	//gf	CString keyw;
	if (g_nSearchType==ST_ADDRBAR)
		m_wndAddress->GetEditCtrl()->GetWindowText(keyw);
	else
	{
		//gf m_wndSearch->GetEditCtrl()->GetWindowText(keyw);
	}
	g_nSearchType=ST_SEARCHBAR;
	
	keyw.TrimLeft();
	keyw.TrimRight();
	
	if (keyw.GetLength()<=0)
	{
		return i;
	}
	//
	CMyIE9View * pView = NULL;
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (tcf!=NULL)
		pView = tcf->m_pView;
	if(pView == NULL)
		return i;
	//
	IHTMLDocument2 * pDoc = NULL;
	IHTMLElement *pElem = NULL;
	IHTMLBodyElement * pBody = NULL;
	
	try{	
		pDoc = (IHTMLDocument2*)(pView->GetHtmlDocument());
		if(pDoc!=NULL)
		{
			pElem = NULL;
			pDoc->get_body(&pElem);
			if(pElem!=NULL)
			{
				pElem->QueryInterface(&pBody);
				if(pBody!=NULL)
				{
					IHTMLTxtRange *pTxtRang;
					pBody->createTextRange(&pTxtRang);
					if (pTxtRang!=NULL)
					{
						BSTR bstrKey;
						bstrKey = keyw.AllocSysString();
						//
						VARIANT_BOOL vRet;
						while(1)
						{
							pTxtRang->findText(bstrKey, TRUE, m_bSearchMatchWhole*2 | m_bSearchMatchCase*4, &vRet);
							if (vRet==FALSE)
								break;
							i++;
							pTxtRang->collapse(false);
						}
						SysFreeString(bstrKey);
						pTxtRang->Release();
					}
				}
			}
		}
	}catch(...){}
	
	try{
		if(pBody != NULL)
			pBody->Release();
		if(pDoc != NULL)
			pDoc->Release();
		if(pElem != NULL)
			pElem->Release();
	}catch(...){}
	//
	return i;
}

void CMainFrame::OnFileAutoSaveAll() 
{
	// TODO: Add your command handler code here
	try	{
		int n = m_wndTab.GetItemCount();
		CChildFrame* tcf;
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		int l =n-1;
		
		for(int i=n-1;i>=0;i--)
		{
			m_wndTab.GetItem(l, &TabCtrlItem);
			tcf = ((CChildFrame*)TabCtrlItem.lParam);
			if(VALID_TCF(tcf))
			{
				tcf->m_pView->PostMessage(WM_COMMAND, ID_FILE_AUTOSAVE);
			}
			l--;
		}
	}catch(...){}
}

void CMainFrame::OnFileNewCurrentHomePage() 
{
	// TODO: Add your command handler code here
	try{
		GET_ACTIVE
			CString strUrl = _StringGetRootUrl(tcf->m_pView->m_lpszUrl);
		if (strUrl.GetLength())
		{
			if (m_bActiveFileNew)
				NewChildWindow(1,2,strUrl,NULL,TRUE);
			else
				NewChildWindow(1,2,strUrl);
		}
	}catch(...){}
}

void CMainFrame::SaveAliasConfig() 
{
	CWinApp* app = AfxGetApp();
	CString alias, url;
	POSITION pos = m_mapAlias.GetStartPosition();
	int i=0;
	char key[9];
	while(pos!=NULL)
	{
		m_mapAlias.GetNextAssoc(pos, alias, url);
		key[0]='A';
		itoa(i, key+1, 10);
		app->WriteProfileString("Alias", key, alias);
		key[0]='U';
		itoa(i, key+1, 10);
		app->WriteProfileString("Alias", key, url);
		i++;
	}
	key[0]='A';
	itoa(i, key+1, 10);
	app->WriteProfileString("Alias", key, "");
	key[0]='U';
	itoa(i, key+1, 10);
	app->WriteProfileString("Alias", key, "");
}

void CMainFrame::OnViewExplorebarResource() 
{
	// TODO: Add your command handler code here
	CString str,str1;
	try{
		if(m_wndInstantBar.GetSafeHwnd()==NULL)
			BuildExpBar("");
		BOOL bNeedInI=false;
		if(m_pwndExpObj == NULL)
			bNeedInI = true;
		else
			LOADSTR(str ,IDS_TITLE_RESOURCE);
		m_wndInstantBar.GetWindowText(str1);
		if(bNeedInI || str1!=str)
		{
			RemoveExp();
			try
			{
				m_pwndExpObj = new CResourcebar;
			}
			catch(...)
			{
				m_pwndExpObj = NULL;
				return;
			}
			
			m_pwndExpObj->Create(&m_wndInstantBar);
			m_pwndExpObj->Initialize();
			str = m_pwndExpObj->GetTitle();
			
			CRect rect;
			m_wndInstantBar.GetClientRect(&rect);
			m_wndInstantBar.SendMessage(WM_SIZE, 0, ((rect.Height())<<16)+rect.Width());
			
			m_wndInstantBar.SetWindowText(str);
			//if(!m_wndInstantBar.IsVisible())
			ShowControlBar(&m_wndInstantBar, TRUE, FALSE);
		}
		else
		{
			if(m_wndInstantBar.IsVisible())
				m_pwndExpObj->Update(1);
			else
				m_pwndExpObj->Update(0);
			
			ShowControlBar(&m_wndInstantBar, !m_wndInstantBar.IsVisible(), FALSE);
		}	
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnFileNewCurrentUpPage() 
{
	GET_ACTIVE
		CString strURL;
	strURL = _StringGetUpUrl(tcf->m_pView->m_lpszUrl);
	if (strURL != tcf->m_pView->m_lpszUrl)
	{
		if (m_bActiveFileNew)
			NewChildWindow(1,2,strURL,NULL,TRUE);
		else
			NewChildWindow(1,2,strURL);
	}
}

void CMainFrame::DoWithImgUrl(int type)
{
	g_strLastUrl = GetImageUrlByPoint(m_ptMouseDown);
	g_nDragType = DT_IMAGE;
	
	switch (type)
	{
	case 3:
		{
			CMenu menu;
			CMenu* pPopup;
			
			CPoint pt;
			GetCursorPos(&pt);
			menu.LoadMenu(IDR_DRAG_IMG);
			pPopup = menu.GetSubMenu(0);
			LOADMENU(pPopup,"MenuDragImage");
			pPopup->TrackPopupMenu(
				TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, 
				pt.x + 2, pt.y, this
				);  //gf +2 以便出现右双击事件
		}
		break;
	case 4:	OnUrlShow();	break;
	case 5:	OnUrlFilter(); break;
	case 6:	OnUrlEdit();	break;
	case 7:	OnUrlCopy(); break;
	case 8:	OnUrlShowImg();break;
	case 9:	OnUrlOpen();break;
	}
}

void CMainFrame::DoWithLinkUrl(int type,CString strUrl,CString strText)
{
	//not strUrl.TrimLeft,Right,for that may copy
	g_strLastUrl = strUrl;
	g_strLastText = strText;
	g_nDragType = DT_LINK;
	
	switch (type)
	{
	case 2:
		{
			CMenu menu;
			CMenu *pPopup, *pGroups;
			
			CPoint pt;
			GetCursorPos(&pt);
			menu.LoadMenu(IDR_DRAG_LINK);
			pPopup = menu.GetSubMenu(0);
			LOADMENU(pPopup,"MenuDragLink");
			pGroups = pPopup->GetSubMenu(11);
			MenuLoadGroupList(pGroups, 0);
			g_bRTab = 2;
			
			pPopup->TrackPopupMenu(
				TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, 
				pt.x, pt.y, this
				);
		}
		break;
	case 3:	OnUrlOpen();	break;
	case 4:	OnUrlOpenActive();	break;
	case 5:	OnUrlOpenIe();	break;
	case 6:	OnUrlOpenJump(); break;
	case 7:	OnTextEdit(); break;
	case 8: OnTextCopy(); break;
	case 9:	OnUrlEdit(); break;
	case 10:OnUrlCopy(); break;
	case 11:OnUrlFilter();break;
	case 12:OnUrlAddtoFav();break;
	case 13:OnUrlOpenInactive();break;
	case 14:OnUrlOpenOtherSide();break;
	case 15:OnUrlShowImg();break;
	case 16:OnEditImgUrl();break;
	}
}

void CMainFrame::OnEditImgUrl() 
{
	// TODO: Add your command handler code here
	g_strLastUrl = GetImageUrlByPoint(m_ptMouseDown);
	OnUrlEdit();
}

void CMainFrame::DoWithText(int type,CString strUrl)
{
	//not strUrl.TrimLeft,Right,for that may copy
	g_strLastUrl = strUrl;
	g_strLastText = strUrl;
	g_nDragType = DT_TEXT;
	
	switch (type)
	{
	case 2:
		{
			CMenu menu;
			CMenu* pPopup,*pSearch;
			
			CPoint pt;
			GetCursorPos(&pt);
			menu.LoadMenu(IDR_DRAG_TEXT);
			pPopup = menu.GetSubMenu(0);
			LOADMENU(pPopup,"MenuDragText");
			pSearch = pPopup->GetSubMenu(14);
			BuildTextDropSearchMenu(pSearch);
			
			pPopup->TrackPopupMenu(
				TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, 
				pt.x, pt.y, this
				);
		}
		break;
	case 3:	OnUrlOpen();	break;
	case 4:	OnUrlOpenActive();	break;
	case 5:	OnUrlOpenIe();	break;
	case 6:	OnTextSearch(); break;
	case 7:	OnTextHighLight();	break;
	case 8:	OnTextFind(); break;
	case 9:	OnTextSave();	break;
	case 10:OnTextAutoSave(); break;
	case 11:OnTextEdit();	break;
	case 12:OnTextCopy(); break;
	case 13:OnTextOpenLinks();break;
	case 14:OnUrlFilter();break;
	case 15:OnUrlOpenInactive();break;
	case 16:OnUrlOpenOtherSide();break;
	}
}

void CMainFrame::OpenJumpUrl(CString strUrl)
{
	try{
		//file:///	http://		ftp://	https://
		int pos;
		pos = strUrl.Find("://",6);
		if (pos>0)
		{
			if ( strUrl.Find("http://",6)>0 )
				strUrl = strUrl.Right(strUrl.GetLength()-strUrl.Find("http://",6));
			else if ( strUrl.Find("https://",6)>0 )
				strUrl = strUrl.Right(strUrl.GetLength()-strUrl.Find("https://",6));
			else if ( strUrl.Find("ftp://",6)>0 )
				strUrl = strUrl.Right(strUrl.GetLength()-strUrl.Find("ftp://",6));
			else if ( strUrl.Find("file://",6)>0 )
				strUrl = strUrl.Right(strUrl.GetLength()-strUrl.Find("file://",6));
		}
		NewChildWindow(1,2,strUrl);
		
	}catch(...){}
}

void CMainFrame::OnGoStartPage() 
{
	// TODO: Add your command handler code here
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (!tcf || !tcf->m_pView || g_bHomeInNewWindow)
		OnFileNew();
	else
		tcf->m_pView->GoHome();
}

void CMainFrame::OnScSetFormData() 
{
	try
	{
		
		// TODO: Add your command handler code here
		if (m_strFormMainPwd.GetLength())
		{
			CInputPassword dlg;
			dlg.m_bRemPsw = false;
			if (dlg.DoModal()==IDOK)
			{
				CString strEncode;
				Encoding2(dlg.m_strPassword, strEncode);
				if (m_strFormMainPwd != strEncode)
				{
					MSGBOX(IDS_PASSWORD_ERROR);
					return;
				}
			}
			else
				return;
		}
		CSetFormDataDlg dlg;
		dlg.m_bEncryptStringItem = m_bEncryptStringItem;
		if (dlg.DoModal()==IDOK)
		{
			if (m_bEncryptStringItem != dlg.m_bEncryptStringItem)
			{
				m_bEncryptStringItem = dlg.m_bEncryptStringItem ;
				theApp.WriteProfileInt("Settings", "EncryptStringItem", m_bEncryptStringItem);
			}
		}
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

BOOL CMainFrame::GetScriptLangCode(CString &strCode, CString &strLang)
{
	//gf
	{
		strCode.TrimLeft();
		if (IS_SCRIPT(strCode))
		{
			int nComma=strCode.Find(":");
			strLang=strCode.Left(nComma);
			strCode=strCode.Right(strCode.GetLength()-nComma -1);
			return TRUE;
		}
	}

	//check < >
	int pos1 = strCode.Find("<");
	int pos2 = strCode.Find(">");
	int pos3 ;
	CString strTemp;
	if (pos1<0 || pos2<0 || pos2<=pos1)
		goto _ScriptError;
	//get language	
	strTemp = strCode.Mid(pos1, pos2-pos1+1);
	strTemp.MakeLower();
	if (strTemp.Find("javascript")>0)//<script language="jscript">
		strLang = "javascript";
	else if (strTemp.Find("jscript")>0)
		strLang = "jscript";
	else if (strTemp.Find("vbscript")>0)
		strLang = "vbscript";
	else
		goto _ScriptError;
	//remove <script language="jscript">
	strCode = strCode.Right(strCode.GetLength()-(pos2+1));
	//remove </script>
	strTemp = strCode;
	strTemp.MakeLower();
	pos3 = strTemp.Find("</script>");
	if (pos3<=0)
		goto _ScriptError;
	else
		strCode = strCode.Left(pos3);
	
	return TRUE;
	
_ScriptError:
	MSGBOX(IDS_SCRIPT_ERR);
	return FALSE;
}

void CMainFrame::RunScript(CString strCode)
{
	try{
		CChildFrame *tcf = (CChildFrame*)MDIGetActive();
		if(!tcf || !tcf->m_pView )
			return;
		CString strLang;
		if (!GetScriptLangCode(strCode,strLang))
			return;
		//
		if ( PRESS_CTRL && PRESS_SHIFT )
		{
			int n = m_wndTab.GetItemCount();
			if(n<=0)
				return;
			//
			CChildFrame* tcf;
			TCITEM TabCtrlItem;
			TabCtrlItem.mask = TCIF_PARAM;
			
			for(int i=n-1;i>=0;i--)
			{
				m_wndTab.GetItem(i, &TabCtrlItem);
				tcf = ((CChildFrame*)TabCtrlItem.lParam);
				if(tcf)
					RunScriptByFrame(strCode,strLang,tcf);
			}
		}
		else
			RunScriptByFrame(strCode,strLang,tcf);
		//
	}catch(...){}
}

void CMainFrame::RunOneScript(CString strCode,CChildFrame* tcf)
{
	try{
		CString strLang;
		if (!GetScriptLangCode(strCode,strLang))
			return;
		RunScriptByFrame(strCode,strLang,tcf);
	}catch(...){}
}

void CMainFrame::RunScriptByFrame(CString strCode,CString strLang,CChildFrame* tcf)
{
	// TODO: Add your message handler code here and/or call default
	if (!tcf || !tcf->m_pView)
		return ;
	//
	LPDISPATCH pDisp = NULL;
	IHTMLDocument2 *pHTMLDoc = NULL;
	IHTMLWindow2 *pHTMLWnd = NULL;
	
	try{
		if (!(::IsWindow(tcf->m_pView->m_hWnd)))//!! must do this, for m_hWnd=0xcccc
			return;
		pDisp = tcf->m_pView->GetHtmlDocument();
		if( pDisp )
		{
			if (SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
			{
				if(SUCCEEDED(pHTMLDoc->get_parentWindow( &pHTMLWnd )))
				{
					BSTR bstrCode = strCode.AllocSysString();
					BSTR bstrLang = strLang.AllocSysString();
					VARIANT vRet;
					vRet.vt = VT_BOOL;
					//set flag, may create new window, not pop filter 
					//set right click is better than left click, google search will pop a input dlg
					//QueryPerformanceCounter(&(tcf->m_pView->m_tLastRClick));
					tcf->m_pView->m_bRClick = RC_NOT_FILTER;
					BOOL bRet = SUCCEEDED(pHTMLWnd->execScript(bstrCode,bstrLang,&vRet));
					tcf->m_pView->m_bRClick = 0;
					if (bRet==FALSE && m_bShowScriptResult)
					{
						CString strMsg;
						LOADSTR(strMsg ,IDS_SCRIPT_RUN_ERR);
						SetMessageText(strMsg);
					}
					SysFreeString(bstrCode);
					SysFreeString(bstrLang);
				}
			}
		}
	}catch(...){}
	
	try{
		if(pHTMLWnd != NULL)	pHTMLWnd->Release();
		if(pHTMLDoc != NULL)	pHTMLDoc->Release();
		if(pDisp != NULL)		pDisp->Release();
	}catch(...){}
}

//kind 0: url, 1:title
void CMainFrame::CloseAllKindPage(CChildFrame *tcf, int kind)
{
	if (!tcf || !tcf->m_pView)
		return;
	CInputDlg dlg;
	CString strMsg;
	LOADSTR(strMsg , kind==0 ? IDS_INPUT_URL:IDS_INPUT_TITLE);
	dlg.m_strMsg = strMsg;
	dlg.m_strOutput = (kind==0 ? tcf->m_pView->m_lpszUrl : tcf->m_strTitle);
	
	if(dlg.DoModal() == IDOK)
	{
		CString strUrl = dlg.m_strOutput;
		//
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		int n = m_wndTab.GetItemCount();
		int l =n-1;
		int bCanClose;
		for(int i=n-1;i>=0;i--)
		{
			m_wndTab.GetItem(l, &TabCtrlItem);
			tcf = ((CChildFrame*)TabCtrlItem.lParam);
			if(VALID_TCF(tcf))
			{
				if (kind==0)
					bCanClose = (tcf->m_pView->m_lpszUrl.Find(strUrl)>=0 ? TRUE : FALSE);
				else
					bCanClose = (tcf->m_strTitle.Find(strUrl)>=0 ? TRUE : FALSE);
				if (bCanClose)
				{
					CLOSE_PAGE(tcf);
				}
			}
			l--;
		}
		
	}
}

void CMainFrame::OnViewFontsIncrease() 
{
	// TODO: Add your command handler code here
	GET_ACTIVE
		CMyIE9View* pvw = tcf->m_pView ;
	if (pvw->m_nFontSize<=0)
		return;
	pvw->m_nFontSize--;
	g_nDefFontSize = pvw->m_nFontSize;
	long font = 4- g_nDefFontSize;
	COleVariant vaZoomFactor(font);
	pvw->ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		&vaZoomFactor, NULL);
}

void CMainFrame::OnViewFontsDecrease() 
{
	// TODO: Add your command handler code here
	GET_ACTIVE
		CMyIE9View* pvw = tcf->m_pView ;
	if (pvw->m_nFontSize>=4)
		return;
	pvw->m_nFontSize++;
	g_nDefFontSize = pvw->m_nFontSize;
	long font = 4- g_nDefFontSize;
	COleVariant vaZoomFactor(font);
	pvw->ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		&vaZoomFactor, NULL);
}

void CMainFrame::OnViewMarkKind() 
{
	// TODO: Add your command handler code here
	try{
		CChildFrame *tcf;
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		if (g_bRTab)
		{
			m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
			tcf = (CChildFrame*)TabCtrlItem.lParam;
		}
		else
			tcf = (CChildFrame*)MDIGetActive();
		
		if (!tcf || !tcf->m_pView)
			return ;
		
		CInputDlg dlg;
		CString strMsg;
		LOADSTR(strMsg ,IDS_INPUT_URL);
		dlg.m_strMsg = strMsg;
		dlg.m_strOutput = tcf->m_pView->m_lpszUrl;
		
		if(dlg.DoModal() == IDOK)
		{
			CString strUrl = dlg.m_strOutput;
			CString strTitle;
			//
			int n = m_wndTab.GetItemCount();
			int l =n-1;
			
			for(int i=n-1;i>=0;i--)
			{
				m_wndTab.GetItem(l, &TabCtrlItem);
				tcf = ((CChildFrame*)TabCtrlItem.lParam);
				if(VALID_TCF(tcf))
				{
					if (tcf->m_pView->m_lpszUrl.Find(strUrl)>=0)
					{
						MarkTab(tcf);
					}
				}
				l--;
			}
		}
	}catch(...){}
}

void CMainFrame::OnRtabMark() 
{
	// TODO: Add your command handler code here
	try{
		CChildFrame *tcf;
		if (g_bRTab)
		{
			TCITEM TabCtrlItem;
			TabCtrlItem.mask = TCIF_PARAM;
			m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
			tcf = (CChildFrame*)TabCtrlItem.lParam;
		}
		else
			tcf = (CChildFrame*)MDIGetActive();
		//
		if (tcf)
			MarkTab(tcf);
	}catch(...){}
}

void CMainFrame::OnViewMark() 
{
	// TODO: Add your command handler code here
	g_bRTab = 0;
	OnRtabMark();
}

void CMainFrame::OnToolsNoBackground() 
{
	// TODO: Add your command handler code here
	m_bDisableBackground = !m_bDisableBackground;
	ShowBackground(m_bDisableBackground);
}

void CMainFrame::OnUpdateToolsNoBackground(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bDisableBackground);
}

void CMainFrame::OnViewSetUndoClose() 
{
	// TODO: Add your command handler code here
	CSelectURL dlg;
	dlg.m_nUrlType = 3;
	dlg.DoModal();
}

void CMainFrame::OnAddtoIe() 
{
	// TODO: Add your command handler code here
	try{
		CChildFrame *tcf;
		if (g_bRTab)
		{
			TCITEM TabCtrlItem;
			TabCtrlItem.mask = TCIF_PARAM;
			m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
			tcf = (CChildFrame*)TabCtrlItem.lParam;
		}
		else
			tcf = (CChildFrame*)MDIGetActive();	
		if (VALID_TCF(tcf) && tcf->m_pView->m_lpszUrl)
		{
			ShellExecute(NULL, "open", "IEXPLORE.EXE", tcf->m_pView->m_lpszUrl, "", SW_SHOWNORMAL );				
		}
		
	}catch(...){}
}

void CMainFrame::OnFileCloseAlikeUrl() 
{
	// TODO: Add your command handler code here
	try{
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		CChildFrame *cur;
		if (g_bRTab)
		{	
			m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
			cur = (CChildFrame*)TabCtrlItem.lParam;
		}
		else
			cur = (CChildFrame*)MDIGetActive();
		
		if (!cur || !cur->m_pView)
			return ;
		CloseAllKindPage(cur,0);
		
	}catch(...){}
}

void CMainFrame::OnFileCloseAlikeTitle() 
{
	// TODO: Add your command handler code here
	try{
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		CChildFrame *cur;
		if (g_bRTab)
		{	
			m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
			cur = (CChildFrame*)TabCtrlItem.lParam;
		}
		else
			cur = (CChildFrame*)MDIGetActive();
		
		if (!cur || !cur->m_pView)
			return ;
		
		CloseAllKindPage(cur,1);
		
	}catch(...){}
}

void CMainFrame::OnFileCloseAllLeft() 
{
	// TODO: Add your command handler code here
	CChildFrame* tcf;
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	if (!g_bRTab)
	{
		tcf = (CChildFrame*)MDIGetActive();
		m_nRTabID = FindTab(tcf);
	}
	int i;
	for(i=0;i<m_nRTabID;i++)
	{
		TabCtrlItem.lParam = NULL;
		m_wndTab.GetItem(i, &TabCtrlItem);
		tcf = ((CChildFrame*)TabCtrlItem.lParam);
		CLOSE_PAGE(tcf);
	}
}

void CMainFrame::OnFileCloseAllRight() 
{
	// TODO: Add your command handler code here
	CChildFrame* tcf;
	int n = m_wndTab.GetItemCount();
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	if (!g_bRTab)
	{
		tcf = (CChildFrame*)MDIGetActive();
		m_nRTabID = FindTab(tcf);
	}
	int i;
	for(i=m_nRTabID+1;i<n;i++)
	{
		TabCtrlItem.lParam = NULL;
		m_wndTab.GetItem(i, &TabCtrlItem);
		tcf = ((CChildFrame*)TabCtrlItem.lParam);
		CLOSE_PAGE(tcf);
	}
}

void CMainFrame::SaveSearchKey() 
{
	char name[10] = "key";
	POSITION pos = m_astrTypedSearch.GetHeadPosition();
	int i = 0;
	CString strKey;
	WritePrivateProfileSection("SearchKey", NULL, theApp.m_strProfile);
	while(pos!=NULL && i<20)
	{
		itoa(i, name+3, 10);
		strKey = m_astrTypedSearch.GetAt(pos);
		if ( strKey.Find(0x0a) >= 0 )
		{
			strKey.Remove(0x0d);
			strKey.Remove(0x0a);
		}
		theApp.WriteProfileString("SearchKey", name, strKey);
		m_astrTypedSearch.GetNext(pos);
		i++;
	}
}

void CMainFrame::OnOptionsAllowAnimate() 
{
	// TODO: Add your command handler code here
	try{
		CString strRet;
		
		strRet = _RegGetString(HKEY_CURRENT_USER,
			"Software\\Microsoft\\Internet Explorer\\Main",
			"Play_Animations");
		
		strRet = (strRet=="yes" ? "no" : "yes");
		
		_RegSetString(HKEY_CURRENT_USER,
			"Software\\Microsoft\\Internet Explorer\\Main",
			"Play_Animations",strRet);
		
	}catch(...){}
}

void CMainFrame::OnUpdateOptionsAllowAnimate(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
		CString strRet;
		
		strRet = _RegGetString(HKEY_CURRENT_USER,
			"Software\\Microsoft\\Internet Explorer\\Main",
			"Play_Animations");
		
		pCmdUI->SetCheck(strRet=="yes" ? 0 : 1);
		
	}catch(...){}
}

void CMainFrame::OnOptionsDisableFlash() 
{
	// TODO: Add your command handler code here
	try{
		DWORD dwRet;
		
		dwRet = _RegGetDword(HKEY_LOCAL_MACHINE,
			"Software\\Microsoft\\Internet Explorer\\ActiveX Compatibility\\{D27CDB6E-AE6D-11cf-96B8-444553540000}",
			"Compatibility Flags"
			);
		
		dwRet = (dwRet==0x400 ? 0 : 0x400);
		
		_RegSetDword(HKEY_LOCAL_MACHINE,
			"Software\\Microsoft\\Internet Explorer\\ActiveX Compatibility\\{D27CDB6E-AE6D-11cf-96B8-444553540000}",
			"Compatibility Flags",
			dwRet
			);
		
	}catch(...){}
}

void CMainFrame::OnUpdateOptionsDisableFlash(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	try{
		DWORD dwRet;
		dwRet = _RegGetDword(HKEY_LOCAL_MACHINE,
			"Software\\Microsoft\\Internet Explorer\\ActiveX Compatibility\\{D27CDB6E-AE6D-11cf-96B8-444553540000}",
			"Compatibility Flags"
			);
		//0x400 means diable
		pCmdUI->SetCheck(dwRet==0x400 ? 1 : 0);
		
	}catch(...){}
}


void CMainFrame::OnToolsSearchCase() 
{
	// TODO: Add your command handler code here
	m_bSearchMatchCase = !m_bSearchMatchCase;
	//
	GET_ACTIVE
		tcf->m_pView->m_strFindKey = "";
}

void CMainFrame::OnUpdateToolsSearchCase(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bSearchMatchCase );
}

void CMainFrame::OnToolsSearchWhole() 
{
	// TODO: Add your command handler code here
	m_bSearchMatchWhole = !m_bSearchMatchWhole;
	//
	GET_ACTIVE
		tcf->m_pView->m_strFindKey = "";
}

void CMainFrame::OnUpdateToolsSearchWhole(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bSearchMatchWhole );
}

void CMainFrame::RunDownManagerScript(CString strNavigate, CChildFrame* tcf)
{
	CString strUrl,strInfo;
	strUrl.Empty();
	strInfo.Empty();
	
	if (VALID_TCF(tcf))
	{
		//		LPDISPATCH pDisp; 
		//		IHTMLDocument2* pHTMLDoc=NULL;
		//	IHTMLElement* pElem = NULL;
		CComPtr<IHTMLElement> pElem;
		
		try{
			//gf			pDisp = tcf->m_pView->GetHtmlDocument();
			//当点击一个要新开窗口的zip链接时,pDisp==NULL
			//gf	if(pDisp==NULL)
			{
				tcf->PostMessage(WM_CLOSE);//close it
				//
				tcf = (CChildFrame*)MDIGetActive();
				if (!tcf || !tcf->m_pView)
					return;
				//gf pDisp = tcf->m_pView->GetHtmlDocument();
			}
			//
			//if (pDisp!=NULL)
			{
				//if(SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)))
				{
					//if(pHTMLDoc!=NULL)
					{
						POINT pt;
						GetCursorPos(&pt);
						tcf->m_pView->MyScreenToClient(&pt);
						//gf						if(SUCCEEDED(pHTMLDoc->elementFromPoint(pt.x, pt.y, &pElem)))
						if(SUCCEEDED((tcf->m_pView)->GetHitElement(pt, &pElem) ))
						{
							CString strTagName;
							BSTR bstrSrc;
							//
							pElem->get_tagName(&bstrSrc);
							strTagName = bstrSrc;
							SysFreeString(bstrSrc);
							if (strTagName=="A")
							{
								pElem->toString(&bstrSrc);
								strUrl=bstrSrc;
								SysFreeString(bstrSrc);
								pElem->get_innerText(&bstrSrc);
								strInfo=bstrSrc;
								SysFreeString(bstrSrc);
							}
						}
					}
				}
			}
		}catch(...){ASSERT(false);}
		
		//		try{
		//			RELEASE(pElem)
		//				RELEASE(pHTMLDoc)
		//				RELEASE(pDisp)
		// 			}catch(...){}
	}
	else//run one current frame
	{
		tcf = (CChildFrame*)MDIGetActive();
		if (!tcf || !tcf->m_pView)
			return;
	}
	//
	if (strUrl.GetLength()<1)
		strUrl = strNavigate;
	if (strUrl.GetLength()<1)
		return;
	//
	CString strScript=m_strManagerScript;
	strScript.Replace("%download_url",strUrl);
	strScript.Replace("%download_info",strInfo);
	//
	RunOneScript(strScript,tcf);
}

BOOL CMainFrame::IsDownloadAble(CString strUrl)
{
	if (PRESS_CTRL)
		return FALSE;
	int i = strUrl.ReverseFind('.');
	if (i<0)
		return FALSE;
	CString ext;
	ext = strUrl.Mid(i);
	ext.MakeUpper();
	int pos = m_strManagerType.Find(ext);
	if (pos<0)
		return FALSE;
	TCHAR ch = m_strManagerType.GetAt(pos+ext.GetLength());
	if (ch==' ' || ch==';' || ch==0)
		return TRUE;
	return FALSE;
}

void CMainFrame::OnUpdateToolsQuickSearch(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BuildSearchMenu(pCmdUI->m_pMenu == pCmdUI->m_pParentMenu);
}

void CMainFrame::OnToolsExternalAutorun() 
{
	// TODO: Add your command handler code here
	m_bAutoRunExternal = !m_bAutoRunExternal;
}

void CMainFrame::OnUpdateToolsExternalAutorun(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bAutoRunExternal);
}
void CMainFrame::RunSelectedExternal(CChildFrame* tcf, BOOL bAutoRun)
{
	int i;
	BOOL bCanRun;
	CExternalUtilItem* eui;
	for (i=0;i <= m_ExternalUtilList.m_UtilList.GetUpperBound();i++)
	{
		eui = m_ExternalUtilList.m_UtilList.GetAt(i);
		if(eui==NULL)
			break;
		if (eui->m_bUtilRun)
		{
			//auto run only run script
			bCanRun = TRUE;
			if (bAutoRun)
			{
				CString strTemp = eui->m_strUtilCmd;
				DO_CMD_STR(strTemp);
				if (_GetPlugInType(strTemp)!=1)
					bCanRun = FALSE;
			}
			if (bCanRun)
				RunUtils(B_EX_TOOL+i,tcf);
		}
	}
}

void CMainFrame::OnViewCompletelyRefresh() 
{
	// TODO: Add your command handler code here
	try{
		
		GET_ACTIVE
			tcf->m_pView->m_bForbidAutoNavigate = 0;
		tcf->m_pView->Refresh2(REFRESH_COMPLETELY);
		
	}catch(...){}
}

BOOL CMainFrame::GetProxySetting(CString &proxy, CString &bypass)
{
	LPINTERNET_PROXY_INFO lpipi=NULL;
	DWORD dwSize;
	
	try{
		InternetQueryOption(NULL,INTERNET_OPTION_PROXY,NULL,&dwSize);
		try
		{
			lpipi = (LPINTERNET_PROXY_INFO)new char[dwSize];
		}
		catch(...)
		{
			return FALSE;
		}
		if(lpipi !=NULL)
		{
			//Call InternetQueryOption again with the buffer provided
			InternetQueryOption(NULL, INTERNET_OPTION_PROXY,lpipi,&dwSize);
			
			if(lpipi->dwAccessType == INTERNET_OPEN_TYPE_PROXY)
			{
				//save the proxy setting and clear it.
				proxy = lpipi->lpszProxy;
				bypass = lpipi->lpszProxyBypass;
				delete[] lpipi;
				lpipi = NULL;
				return TRUE;
			}
		}
	}catch(...){}
	
	try{
		if(lpipi) delete[] lpipi;
	}catch(...){}
	
	return FALSE;
}

CString CMainFrame::GetImageUrlByPoint(CPoint pt)
{
	CString strUrl;
	strUrl.Empty();
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (!tcf || !tcf->m_pView)
		return strUrl;
	
	CGf_html::IsPointImage((tcf->m_pView)->GetWebBrowser(), (tcf->m_pView)->GetPointInView(pmf->m_ptMouseDown), strUrl);
	
	return strUrl;
	
}

void CMainFrame::OnUrlShow() 
{
	
	//gfhidepic
	GET_ACTIVE
		CWnd* pWnd;
	POINT pt = m_ptMouseDown;
	tcf->m_pView->ClientToScreen(&pt);
	pWnd = tcf->m_pView->WindowFromPoint(pt);
	pWnd->ScreenToClient(&pt);
	//
	
	keybd_event( VK_CONTROL ,0, 0, 0 );
	
	pWnd->SendMessage(WM_RBUTTONDOWN, 0, MAKELONG(pt.x, pt.y)); 
	pWnd->SendMessage(WM_RBUTTONUP, 0, MAKELONG(pt.x, pt.y)); 
	
	keybd_event( VK_CONTROL,0, KEYEVENTF_KEYUP, 0 );
	
	// TODO: Add your command handler code here
	//gf SetMessageText(g_strLastUrl);
}

void CMainFrame::OnUrlEdit() 
{
	CInputDlg dlg;
	CString strMsg;
	LOADSTR(strMsg ,IDS_TIP_URL);
	dlg.m_strMsg = strMsg;
	dlg.m_strOutput = g_strLastUrl;
	dlg.m_nOKType = 1;
	if (dlg.DoModal()==IDOK)
	{
		if (dlg.m_strOutput.GetLength())
		{
			g_strLastText = dlg.m_strOutput;
			DragDropUrl(dlg.m_strOutput,FALSE);
		}
	}
}

void CMainFrame::OnTextEdit() 
{
	CInputDlg dlg;
	CString strMsg;
	LOADSTR(strMsg ,IDS_TIP_TEXT);
	dlg.m_strMsg = strMsg;
	dlg.m_strOutput = g_strLastText;
	dlg.m_nOKType = 1;
	if (dlg.DoModal()==IDOK)
	{
		if (dlg.m_strOutput.GetLength())
		{
			g_strLastText = dlg.m_strOutput;//must do this
			DragDropUrl(dlg.m_strOutput,FALSE);
		}
	}
}

void CMainFrame::OnUrlCopy() 
{
	
	_SetClipString(g_strLastUrl);
	
}

void CMainFrame::OnUrlShowImg() 
{
	
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (!tcf || !tcf->m_pView)
		return;
	
	CString strUrl;
	CGf_html::IsPointImage((tcf->m_pView)->GetWebBrowser(), (tcf->m_pView)->GetPointInView(pmf->m_ptMouseDown), strUrl, TRUE);
	
	return;
	//gf
	//
	//	// TODO: Add your command handler code here
	//	GET_ACTIVE
	//		CWnd* pWnd;
	//	POINT pt = m_ptMouseDown;
	//	tcf->m_pView->ClientToScreen(&pt);
	//	pWnd = tcf->m_pView->WindowFromPoint(pt);
	//	pWnd->ScreenToClient(&pt);
	//	//
	//	pWnd->PostMessage(WM_RBUTTONDOWN, 0, MAKELONG(pt.x, pt.y)); 
	//	pWnd->PostMessage(WM_RBUTTONUP, 0, MAKELONG(pt.x, pt.y)); 
	//	keybd_event('H', MapVirtualKey('H', 0), 0, 0);
}

//1: only new 2:new and active
void CMainFrame::ClickPoint(int nInNewWindow)
{
	try{
		GET_ACTIVE
			//set flag, may create new window, not pop filter 
			QueryPerformanceCounter(&(tcf->m_pView->m_tLastClick));
		tcf->m_pView->m_bLClick = TRUE;
		//
		CWnd* pWnd;
		POINT pt = m_ptMouseDown;
		tcf->m_pView->ClientToScreen(&pt);
		pWnd = tcf->m_pView->WindowFromPoint(pt);
		pWnd->ScreenToClient(&pt);
		//
		tcf->m_pView->m_nInNewWindow = nInNewWindow;
		pWnd->PostMessage(WM_LBUTTONDOWN, 0, MAKELONG(pt.x, pt.y)); 
		pWnd->PostMessage(WM_LBUTTONUP, 0, MAKELONG(pt.x, pt.y));
		
	}catch(...){}
}

void CMainFrame::OnUrlFilter() 
{
	try
	{
		
		// TODO: Add your command handler code here
		CInputDlg dlg;
		CString strMsg;
		LOADSTR(strMsg ,IDS_TIP_ADDTO_CONTENT_FILTER);
		dlg.m_strMsg = strMsg;
		//
		if (g_nDragType==DT_LINK)
		{
			CString strUrl;
			strUrl = GetImageUrlByPoint(m_ptMouseDown);
			if (strUrl.GetLength())
				g_strLastUrl = strUrl;
		}
		dlg.m_strOutput = g_strLastUrl;
		if (dlg.DoModal()==IDOK)
		{
			CString tmp;
			for(int i = 0; i<=m_astrUrlFilter.GetUpperBound(); i++)
			{
				tmp = m_astrUrlFilter.GetAt(i);
				if (tmp==dlg.m_strOutput)
					break;
			}
			if (i > m_astrUrlFilter.GetUpperBound() )
			{
				m_astrUrlFilter.Add(dlg.m_strOutput);
				//
				m_bUrlFilterDirty = TRUE;
				InitialUrlFilter();
				
				SaveFilterList2File(1);
			}
			
			pmf->m_bUseUrlFilter = TRUE;

			OnUrlShowImg();
		}
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

//gf
void CMainFrame::OnImageSaveAs() 
{
	try
	{
		
		OnUrlShow(); //show right menu
		
		keybd_event( 'S' ,0, 0, 0 );
		keybd_event( 'S',0, KEYEVENTF_KEYUP, 0 );
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
	
}

void CMainFrame::OnUrlOpen() 
{
	try
	{
		
		// TODO: Add your command handler code here
		if (g_nDragType==DT_LINK)
			DragDropLink(g_strLastUrl,INW_NORMAL);
		else
			DragDropUrl(g_strLastUrl,FALSE);
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnUrlOpenActive() 
{
	// TODO: Add your command handler code here
	if (g_nDragType==DT_LINK)
		DragDropLink(g_strLastUrl,INW_ACTIVE);
	else
		DragDropUrl(g_strLastUrl,TRUE);
}

void CMainFrame::OnUrlOpenInactive() 
{
	// TODO: Add your command handler code here
	if (g_nDragType==DT_LINK)
		DragDropLink(g_strLastUrl,INW_INACTIVE);
	else
	{
		BOOL bTemp = m_bActiveNewWindow;
		m_bActiveNewWindow = 0;
		DragDropUrl(g_strLastUrl,FALSE);
		m_bActiveNewWindow = bTemp;
	}
}

void CMainFrame::OnUrlOpenOtherSide() 
{
	// TODO: Add your command handler code here
	if (g_nDragType==DT_LINK && m_bSimuDragLink)
		DragDropLink(g_strLastUrl,INW_NORMAL|INW_OTHER_SIDE);
	else
	{
		BOOL bTemp = m_nNewSequence;
		m_nNewSequence = (m_nNewSequence==1 ? 2 : 1);
		DragDropUrl(g_strLastUrl,FALSE);
		m_nNewSequence = bTemp;
	}
}

void CMainFrame::OnUrlOpenIe() 
{
	// TODO: Add your command handler code here
	ShellExecute(NULL, "open", "IEXPLORE.EXE", g_strLastUrl, "", SW_SHOWNORMAL );
}

void CMainFrame::OnUrlOpenJump() 
{
	// TODO: Add your command handler code here
	OpenJumpUrl(g_strLastUrl);
}

void CMainFrame::OnTextSearch() 
{
	//not use DragDropUrl, must force search
	SEARCH_STR(g_strLastUrl) //DragDropUrl(g_strLastUrl,FALSE);
}

void CMainFrame::OnTextCopy() 
{
	// TODO: Add your command handler code here
	_SetClipString(g_strLastText);
}

void CMainFrame::OnTextSave()
{
	if (g_strLastText.GetLength()<1)
		return;
	CString str;
	LOADSTR(str ,IDS_TYPE_COLLECTOR);
	CString strFileName;
	GET_FILE_NAME(strFileName,g_strLastText)
		CFileDialog dlg(FALSE, "txt", strFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str);
	if (dlg.DoModal()==IDOK)
	{
		CFile f;
		if(f.Open(dlg.GetPathName(), CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		{
			f.Write((void*)(LPCSTR)g_strLastText, g_strLastText.GetLength());
			f.Close();
		}
	}
}

void CMainFrame::OnTextAutoSave()
{
	GET_ACTIVE
		CMyIE9View *pView = tcf->m_pView;
	
	CString filename,htmlname;
	char tmp[4];
	htmlname.Empty();
	pView->GetLocationURL(filename);
	
	m_strAutoSavePath = theApp.m_strDefaultDir;
	if(m_bCateWithSite)
	{
		CString site;
		pView->GetLocationURL(site);
		GetSiteName(site);//?
		m_strAutoSavePath += site;
		m_strAutoSavePath += "\\";
	}
	_CreateFullDir(m_strAutoSavePath);
	//
	filename = tcf->m_strTitle;
	GET_URL_NAME(filename)
		REPLACE_ERR_PATH_CHAR(filename)
		filename = m_strAutoSavePath + filename;
	int l = 0;
	HFILE hf; OFSTRUCT of;
	htmlname = filename + ".txt";
	hf = OpenFile(htmlname, &of, OF_EXIST);
	while(hf!=HFILE_ERROR)
	{
		l++;
		htmlname = filename;
		htmlname += '[';
		itoa(l, tmp, 10);
		htmlname += tmp;
		htmlname += ']';
		htmlname += ".txt";
		hf = OpenFile(htmlname, &of, OF_EXIST);
	}
	//
	CFile f;
	if(f.Open(htmlname, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
	{
		f.Write((void*)(LPCSTR)g_strLastText, g_strLastText.GetLength());
		f.Close();
	}
	//
	CString strMsg;
	LOADSTR(strMsg ,IDS_TEXT_SAVED);
	if (htmlname.GetLength()>0)
		strMsg+= htmlname;
	else
		strMsg+= m_strAutoSavePath;
	//add local url
	strMsg += " <= ";
	strMsg += tcf->m_pView->m_lpszUrl;
	
	AddToCollector(strMsg,14);
}

void CMainFrame::OnTextHighLight() 
{
	// TODO: Add your command handler code here
	m_wndSearch->GetEditCtrl()->SetWindowText(g_strLastText);
	m_nSearchBarAction = 4;
	SendMessage(WM_COMMAND, ID_SEARCH_OK);
}

void CMainFrame::OnTextFind() 
{
	// TODO: Add your command handler code here
	m_wndSearch->GetEditCtrl()->SetWindowText(g_strLastText);
	m_nSearchBarAction = 2;
	SendMessage(WM_COMMAND, ID_SEARCH_OK);
}

void CMainFrame::OnTextOpenLinks() 
{
	// TODO: Add your command handler code here
	OnFileNewSelectedLink();
}

void CMainFrame::OnUrlAddtoFav() 
{
	try{
		if (g_strLastText.GetLength()<1)
			g_strLastText = g_strLastUrl;
		REPLACE_ERR_PATH_CHAR(g_strLastText)
			
			VARIANT vtTitle;
		vtTitle.vt=VT_BSTR;
		vtTitle.bstrVal = g_strLastText.AllocSysString();
		
		BSTR URL = g_strLastUrl.AllocSysString();
		AddtoFavorite(URL, &vtTitle);
		
		SysFreeString(URL);
		SysFreeString(vtTitle.bstrVal);
		
	}catch(...){}
}

void CMainFrame::OnUrlCollector() 
{
	// TODO: Add your command handler code here
	CString strMsg;
	strMsg = g_strLastText;
	strMsg += " => ";
	strMsg += g_strLastUrl;
	strMsg += "\r\n";
	m_pCollectDlg->ms_dropTarget.SaveText(strMsg, 0);
}

void CMainFrame::OnTextCollector() 
{
	// TODO: Add your command handler code here
	CString strMsg;
	strMsg = g_strLastText;
	strMsg += "\r\n";
	m_pCollectDlg->ms_dropTarget.SaveText(strMsg, 0);
}

void CMainFrame::OnToolsSearchDelete() 
{
	// TODO: Add your command handler code here
	g_nStartPage = 16;
	OnOptionsMyIE9options();
}

void CMainFrame::OnProxyExport() 
{
	// TODO: Add your command handler code here
	try{
		//select category first
		CSelProxyCatDlg dlg;
		dlg.m_bClear = FALSE;
		dlg.m_bDisableClear = TRUE;
		int i=0;
		if(dlg.DoModal()==IDOK)
		{
			CProxyList * pl=NULL;
			for(int l = 0;l <= m_aProxyCategories.m_ProxyCategory.GetUpperBound();l++)
			{
				pl = m_aProxyCategories.m_ProxyCategory.GetAt(l);
				if(!pl->m_bIsWebProxy)
				{
					if(i == dlg.m_SelCates)
						break;
					if (dlg.m_SelCates<0 && pl->m_strCategoryName == dlg.m_strCatName)
						break;
					i++;
				}
			}
			if(pl!=NULL)
			{
				CString str;
				LOADSTR(str ,IDS_TYPE_COLLECTOR);
				CFileDialog dlg(FALSE, "txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str);
				if (dlg.DoModal()!=IDOK)
					return;
				CString filename = dlg.GetPathName();
				CStdioFile file;
				if(file.Open(filename, CFile::modeCreate|CFile::modeWrite))
				{
					CString line, type;
					for( l=0;l <= pl->m_Proxies.GetUpperBound();l++)
					{
						line = pl->m_Proxies.ElementAt(l)->m_strProxy;
						if(line.Left(2) != "--")
						{
							i = line.Find('=');
							if(i>0)
							{
								type = line.Left(i);
								type.MakeUpper();
								line = line.Mid(i+1);
								line = line + '@';
								line = line + type;
							}
							else
							{
								line = line + '@';
								line = line + "HTTP";
							}
							type=pl->m_Proxies.ElementAt(l)->m_strProxyName;
							line += '#';
							line += type;
							line += "\n";
							file.WriteString(line);
						}
					}
					file.Close();
				}
			}
		}
	}catch(...){}
}

void CMainFrame::OnProxyImport() 
{
	// TODO: Add your command handler code here
	try{
		CString str;
		LOADSTR(str ,IDS_TYPE_COLLECTOR);
		CFileDialog fdlg(TRUE, "txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str);
		if (fdlg.DoModal()!=IDOK)
			return;
		CSelProxyCatDlg dlg;
		dlg.m_bClear = FALSE;
		dlg.m_bDisableClear = FALSE;
		int i=0, p;
		if(dlg.DoModal()==IDOK)
		{
			//save orignal settings first
			theApp.SaveConfg();
			CProxyItem* pi;
			CProxyList * pl=NULL;
			if (dlg.m_SelCates<0)
			{
				if (dlg.m_strCatName.GetLength())
				{
					pl = new CProxyList;
					pl->m_bIsWebProxy = FALSE;
					pl->m_strCategoryName = dlg.m_strCatName;
					pl->m_strProxyByPass = "<local>";
					m_aProxyCategories.m_ProxyCategory.Add(pl);
				}
			}
			else
			{
				for(int l = 0;l <= m_aProxyCategories.m_ProxyCategory.GetUpperBound();l++)
				{
					pl = m_aProxyCategories.m_ProxyCategory.GetAt(l);
					if(!pl->m_bIsWebProxy)
					{
						if(i == dlg.m_SelCates)
							break;
						i++;
					}
				}
			}
			//
			if(pl!=NULL)
			{
				if(dlg.m_bClear)//re==IDYES
				{
					CProxyItem* pi;
					for (int i=0;i <= pl->m_Proxies.GetUpperBound();i++)
					{
						pi = pl->m_Proxies.GetAt(i);
						delete pi;
					}
					pl->m_Proxies.RemoveAll();
				}
				
				CString filename = fdlg.GetPathName();
				CStdioFile file;
				int  state;
				if(file.Open(filename, CFile::modeRead|CFile::shareDenyWrite))
				{
					CString line, addr, type, timing, status, annotation;
					int i;
					char c;
					while(file.ReadString(line))
					{
						i = line.FindOneOf("@$&#");
						if(i>0)
						{
							c = line.GetAt(i);
							addr = line.Left(i);
							while(i>0)
							{
								line = line.Mid(i+1);
								i = line.FindOneOf("@$&#");
								switch(c)
								{
								case '@': //type
									if(i>0)
										type = line.Left(i);
									else
										type = line;
									break;
								case '$': //status
									if(i>0)
										status = line.Left(i);
									else
										status = line;
									break;
								case '&': //timing
									if(i>0)
										timing = line.Left(i);
									else
										timing = line;
									break;
								case '#': //annotation
									if(i>0)
										annotation = line.Left(i);
									else
										annotation = line;
									break;
								}
								if(i>=0)
									c = line.GetAt(i);
							}
						}
						else
						{
							addr = line;
						}
						
						line = "";
						addr.TrimLeft();
						if(!addr.IsEmpty())
						{
							pi = new CProxyItem;
							if(!type.IsEmpty() && type!="HTTP")
							{
								type.MakeLower();
								line = type + '=';
							}
							line += addr;
							
							p = pl->Find(line);
							if(p == -1)
							{
								pi->m_strProxy = line;
							}
							
							if(annotation.IsEmpty())
								annotation = addr;
							if(p<0)
							{
								pi->m_strProxyName = annotation;
							}
							else
							{
								pl->m_Proxies.ElementAt(p)->m_strProxyName = annotation;
							}
							
							if(status == "6" || status.IsEmpty())
								state = 3;
							else
								state = 0;
							if(p<0)
							{
								pi->m_nProxySpeed = state;
							}
							else
							{
								pl->m_Proxies.ElementAt(p)->m_nProxySpeed = state;
							}
							
							try{
								if(p<0)
									pl->m_Proxies.Add(pi);
								else
									delete pi;
						}catch(...){}
						}
						
						line.Empty();
						type.Empty();
						status.Empty();
						annotation.Empty();
						addr.Empty();
				}
				file.Close();
			}
		}
	}
	m_bSaveConfig = TRUE;
	g_bProxyDirty = TRUE;
	
	}catch(...){}
}

//close all unmarked url
void CMainFrame::OnFileCloseMarkedUrl() 
{
	// TODO: Add your command handler code here
	try{	
		CChildFrame* tcf;
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		
		int n = m_wndTab.GetItemCount();
		int l =n-1;
		for(int i=n-1;i>=0;i--)
		{
			for(int i=n-1;i>=0;i--)
			{
				m_wndTab.GetItem(l, &TabCtrlItem);
				tcf = ((CChildFrame*)TabCtrlItem.lParam);
				if (VALID_TCF(tcf) && !MARKED(tcf->m_pView))
				{
					CLOSE_PAGE(tcf);
				}
				l--;
			}
		}
		
	}catch(...){}
}

void CMainFrame::OnToolsExternalRun() 
{
	// TODO: Add your command handler code here
	GET_ACTIVE
		RunSelectedExternal(tcf,FALSE);
}

void CMainFrame::OnRtabSortSave() 
{
	// TODO: Add your command handler code here
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	
	m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
	CChildFrame* tcf = (CChildFrame*)TabCtrlItem.lParam;
	if(VALID_TCF(tcf))
		tcf->m_pView->PostMessage(WM_COMMAND, ID_FILE_AUTOSAVE_SORT);
}

void CMainFrame::ShowBarLabel() 
{
	CString str;
	int bandid;
	REBARBANDINFO rbbi;	
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_TEXT|RBBIM_ID;
	//address
	LOADSTR(str,IDS_ADDRESS);
	if (m_bShowToolbarLabel)
		rbbi.lpText = (LPSTR)(LPCTSTR)str;
	else
		rbbi.lpText = "";
	rbbi.wID = ID_VIEW_ADDRESSBAR;
	bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_ADDRESSBAR);
	m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
	//search
	LOADSTR(str,IDS_SEARCH);
	if (m_bShowToolbarLabel)
		rbbi.lpText = (LPSTR)(LPCTSTR)str;
	else
		rbbi.lpText = "";
	rbbi.wID = ID_VIEW_SEARCH;
	bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_SEARCH);
	m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
	//fav
	LOADSTR(str,IDS_TITLE_FAVORITE);
	if (m_bShowToolbarLabel)
		rbbi.lpText = (LPSTR)(LPCTSTR)str;
	else
		rbbi.lpText = "";
	rbbi.wID = ID_VIEW_FAVS;
	bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_FAVS);
	m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
	//link
	LOADSTR(str,IDS_LINKS);
	if (m_bShowToolbarLabel)
		rbbi.lpText = (LPSTR)(LPCTSTR)str;
	else
		rbbi.lpText = "";
	rbbi.wID = ID_VIEW_LINKS;
	bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_LINKS);
	m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
	//external
	LOADSTR(str,IDS_EX_TOOL);
	if (m_bShowToolbarLabel)
		rbbi.lpText = (LPSTR)(LPCTSTR)str;
	else
		rbbi.lpText = "";
	rbbi.wID = ID_VIEW_EX_TOOL;
	bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_VIEW_EX_TOOL);
	m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
}

void CMainFrame::OnViewSelectLang() 
{
	// TODO: Add your command handler code here
	CSelectLang dlg;
	if (dlg.DoModal()==IDOK)
	{
		CToolMenu* pMenu;
		CMenuBar& mb = m_wndMenuBar;
		pMenu = (CToolMenu*)mb.GetMenu();
		pMenu->DestroyMenu();
		mb.LoadMenu(IDR_MAINFRAME);
		pMenu = (CToolMenu*)mb.GetMenu();
		_LoadMenuStringOwnerDraw(pMenu, "MenuMain",&(mb.m_ImgMenu));
		//re calc the cxideal
		{
			CRect rectToolBar;
			REBARBANDINFO rbbi;
			rbbi.cbSize = sizeof(rbbi);
			rbbi.fMask = RBBIM_IDEALSIZE | RBBIM_ID ;
			rbbi.wID = ID_MENU_BAR;
			int bc=m_wndMenuBar.GetToolBarCtrl().GetButtonCount();
			if(bc>0)
			{
				m_wndMenuBar.GetToolBarCtrl().GetItemRect(bc-1,&rectToolBar);
				rbbi.cxIdeal = rectToolBar.right;
			}
			else
				rbbi.cxIdeal = 2;
			int bandid = m_wndReBar.GetReBarCtrl().IDToIndex(ID_MENU_BAR);
			m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
		}
		ReloadTabMenu();
		//
		DestroyMenuData(m_TrayMenu.m_hMenu,0);
		m_TrayMenu.LoadMenu(IDR_TRAYMENU);
		_LoadMenuStringOwnerDraw(m_TrayMenu.GetSubMenu(0),"MenuTray", &(mb.m_ImgMenu));
		//
		if (m_bShowToolbarLabel)
			ShowBarLabel();
		//build new menu
		UpdateUndoMenu();
		m_bFavDirty=TRUE;
		InitGroups();
		//BuildSearchMenu();
		BuildUtilMenu();
		if (m_nTextLabel)
			InitToolBar();
	}
}

void CMainFrame::OnRtabNewWindow() 
{
	// TODO: Add your command handler code here
	try{
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
		
		CChildFrame *tcf = (CChildFrame*)TabCtrlItem.lParam;
		if (!tcf || !tcf->m_pView)
			return ;
		NewChildWindow(1,2,tcf->m_pView->m_lpszUrl);
		
	}catch(...){}
}

void CMainFrame::OnGoPageUp() 
{
	// TODO: Add your command handler code here
	//PostMessage(WM_KEYDOWN,VK_PRIOR,1);
	keybd_event(VK_PRIOR, MapVirtualKey(VK_PRIOR, 0), 0, 0);
}

void CMainFrame::OnGoPageDown() 
{
	// TODO: Add your command handler code here
	//PostMessage(WM_KEYDOWN,VK_NEXT,1);
	keybd_event(VK_NEXT, MapVirtualKey(VK_NEXT, 0), 0, 0);
}
void CMainFrame::OnGoPageTop() 
{
	// TODO: Add your command handler code here
	//PostMessage(WM_KEYDOWN,VK_HOME,1);
	keybd_event(VK_HOME, MapVirtualKey(VK_HOME, 0), 0, 0);
}
void CMainFrame::OnGoPageBottom() 
{
	// TODO: Add your command handler code here
	//PostMessage(WM_KEYDOWN,VK_END,1);
	keybd_event(VK_END, MapVirtualKey(VK_END, 0), 0, 0);
}
void CMainFrame::OnGoPageLeft() 
{
	// TODO: Add your command handler code here
	GET_ACTIVE
		tcf->m_pView->WBScrollHPage(1);
}
void CMainFrame::OnGoPageRight() 
{
	// TODO: Add your command handler code here
	GET_ACTIVE
		tcf->m_pView->WBScrollHPage(0);
}

void CMainFrame::ReloadTabMenu()
{
	CMenuBar& mb = m_wndMenuBar;
	DestroyMenuData(m_TabMenu.m_hMenu,0);

	if (m_TabMenu.m_hMenu)
	{
		m_TabMenu.DestroyMenu();
	}
	
	if(m_bTopTab)
	{
		m_TabMenu.LoadMenu(IDR_RTAB);
	}
	else
	{
		m_TabMenu.LoadMenu(IDR_RTAB_TOP); //gf
	}
	
	_LoadMenuStringOwnerDraw(m_TabMenu.GetSubMenu(0), "MenuTab", &(mb.m_ImgMenu));
	
	//gf
	//	if(m_bTopTab)
	//	{
	//		CMenu *pPopup = m_TabMenu.GetSubMenu(0);
	//		
	//
	//		CString str;
	//		pPopup->GetMenuString(ID_FILE_CLOSE_2, str, MF_BYCOMMAND);
	//		CString str2;
	//		pPopup->GetMenuString(ID_RTAB_CLOSE_KEEPCUR, str2, MF_BYCOMMAND);
	//
	//		//delete
	//		DestroyMenuData(pPopup->m_hMenu,19);
	//		while(pPopup->DeleteMenu(19, MF_BYPOSITION));
	//		//
	//		MenuInserOwnerItem(pPopup,0,MF_BYPOSITION, ID_FILE_CLOSE_2, str,42);
	//		MenuInsertSeparator(pPopup,1);
	//		MenuInserOwnerItem(pPopup,2,MF_BYPOSITION, ID_RTAB_CLOSE_KEEPCUR, str2,-1);
	//		MenuInsertSeparator(pPopup,3);
	//	}
}

void CMainFrame::OnHighlightSelectedText() 
{
	try
	{
		
		// TODO: Add your command handler code here
		CString str;
		GetHtmlSelectedText(str);
		if (str.GetLength())
		{
			m_wndSearch->GetEditCtrl()->SetWindowText(str);
			m_nSearchBarAction = 4;
			SendMessage(WM_COMMAND, ID_SEARCH_OK);
		}
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnFindPrevSelectedText() 
{
	// TODO: Add your command handler code here
	GET_ACTIVE
		tcf->m_pView->m_bFindPrev=TRUE;
	OnFindNextSelectedText();
}

void CMainFrame::OnFindNextSelectedText() 
{
	try
	{
		
		// TODO: Add your command handler code here
		CString str;
		GetHtmlSelectedText(str);
		if (str.GetLength())
		{
			m_wndSearch->GetEditCtrl()->SetWindowText(str);
			m_nSearchBarAction = 2;
			SendMessage(WM_COMMAND, ID_SEARCH_OK);
		}
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnOpenLink() 
{
	try
	{
		
		// TODO: Add your command handler code here
		GET_ACTIVE
			tcf->m_pView->OpenLinkUnderMouse(0);
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnOpenLinkActive() 
{
	try
	{
		
		// TODO: Add your command handler code here
		GET_ACTIVE
			tcf->m_pView->OpenLinkUnderMouse(1);
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}

void CMainFrame::OnOpenLinkDeactive() 
{
	// TODO: Add your command handler code here
	GET_ACTIVE
		tcf->m_pView->OpenLinkUnderMouse(-1);
}

void CMainFrame::OnNotSaveKey() 
{
	// TODO: Add your command handler code here
	m_bNotSaveKeyword = !m_bNotSaveKeyword;
}

void CMainFrame::OnUpdateNotSaveKey(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bNotSaveKeyword);
}

void CMainFrame::OnUrlOpenFtp() 
{
	try
	{
		
		// TODO: Add your command handler code here
		if (g_strLastUrl.GetLength())
			_OpenFtpUrl(g_strLastUrl);
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
	}
}
void CMainFrame::OnViewForbidAutoNav() 
{
	// TODO: Add your command handler code here
	g_bRTab = 0;
	OnRtabForbidAutoNav();
}

void CMainFrame::OnRtabForbidAutoNav() 
{
	// TODO: Add your command handler code here
	try{
		CChildFrame *tcf;
		if (g_bRTab)
		{
			TCITEM TabCtrlItem;
			TabCtrlItem.mask = TCIF_PARAM;
			m_wndTab.GetItem(m_nRTabID, &TabCtrlItem);
			tcf = (CChildFrame*)TabCtrlItem.lParam;
		}
		else
			tcf = (CChildFrame*)MDIGetActive();
		//
		if (VALID_TCF(tcf))
			tcf->m_pView->m_bForbidAutoNavigate = !tcf->m_pView->m_bForbidAutoNavigate;
	}catch(...){}
}

LRESULT CMainFrame::OnAppCommand(WPARAM wParam,LPARAM lParam)
{
	try
	{
		
		CChildFrame* tcf = (CChildFrame*)MDIGetActive();
		if (!VALID_TCF(tcf))
			return DefWindowProc(WM_APPCOMMAND,wParam,lParam);
		switch ( GET_APPCOMMAND_LPARAM(lParam) )
		{
		case APPCOMMAND_BROWSER_BACKWARD:	tcf->m_pView->GoBack(); break;
		case APPCOMMAND_BROWSER_FORWARD:	tcf->m_pView->GoForward(); break;
		case APPCOMMAND_BROWSER_REFRESH:	tcf->m_pView->PowerRefresh(); break;
		case APPCOMMAND_BROWSER_STOP:		PostMessage(WM_COMMAND, ID_VIEW_STOP); break;
		case APPCOMMAND_BROWSER_HOME:		PostMessage(WM_COMMAND, ID_FILE_NEW_S); break;
		case APPCOMMAND_FIND:	PostMessage(WM_COMMAND, ID_EDIT_FIND); break;
		case APPCOMMAND_NEW:	PostMessage(WM_COMMAND, ID_FILE_NEW_BLANK); break;
		case APPCOMMAND_OPEN:	PostMessage(WM_COMMAND, ID_FILE_OPENF); break;
		case APPCOMMAND_CLOSE:	PostMessage(WM_COMMAND, ID_FILE_CLOSE_1); break;
		case APPCOMMAND_SAVE:	PostMessage(WM_COMMAND, ID_FILE_SAVEAS); break;
		case APPCOMMAND_PRINT:	PostMessage(WM_COMMAND, ID_FILE_PRINT); break;
		case APPCOMMAND_COPY:	PostMessage(WM_COMMAND, ID_EDIT_COPY); break;
		case APPCOMMAND_CUT:	PostMessage(WM_COMMAND, ID_EDIT_CUT); break;
		case APPCOMMAND_PASTE:	PostMessage(WM_COMMAND, ID_EDIT_PASTE); break;
		default:
			return DefWindowProc(WM_APPCOMMAND,wParam,lParam);
		}
		return TRUE;
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
		return TRUE;
	}
}

void CMainFrame::WriteProfileReBarBandsState()
{
	char band[10]="band",size[10]="size",style[10]="style";
	UINT i=0;
	
	CWinApp* app = AfxGetApp();
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_SIZE | RBBIM_STYLE | RBBIM_CHILD|RBBIM_ID;
	for(i=0; i< m_wndReBar.GetReBarCtrl().GetBandCount(); i++)
	{
		itoa(i,band+4,10);
		itoa(i,size+4,10);
		itoa(i,style+5,10);
		m_wndReBar.GetReBarCtrl().GetBandInfo(i, &rbbi);
		app->WriteProfileInt("BandBar",band,rbbi.wID);
		app->WriteProfileInt("BandBar",size,rbbi.cx);
		app->WriteProfileInt("BandBar",style,rbbi.fStyle);
	}
	itoa(i, band+4, 10);
	app->WriteProfileInt("BandBar", band, 0);
}

void SetBandInfo(UINT nIndex,UINT nID,UINT fStyle,UINT cx)
{
	int count = countof(g_BandInfo);
	for (int i=0;i<count;i++)
	{
		if (g_BandInfo[i].nID == nID)
		{
			g_BandInfo[i].nIndex = nIndex;
			g_BandInfo[i].fStyle = fStyle;
			g_BandInfo[i].cx = cx;
			return;
		}
	}
}

BandInfo* GetBandInfo(UINT nIndex)
{
	int count = countof(g_BandInfo);
	for (int i=0;i<count;i++)
	{
		if (g_BandInfo[i].nIndex == nIndex)
			return &g_BandInfo[i];
	}
	return NULL;
}

//英文m_wndMenuBar这些指针无法放到g_BandInfo中(编译找不到),所以只好用函数
CWnd* CMainFrame::GetBandWnd(int id)
{
	switch(id)
	{
	case ID_MENU_BAR:		return &m_wndMenuBar;
	case ID_SYSMENU_BAR:	return &m_SysMenuBar;
	case ID_VIEW_TOOLBAR:	return &m_wndToolBar;
	case ID_VIEW_ADDRESSBAR:return &m_AddressBar;
	case ID_VIEW_LINKS:		return &m_LinkBar;
	case ID_VIEW_FAVS:		return &m_FavBar;
	case ID_VIEW_EX_TOOL:	return &m_ExternalTool;
	case ID_VIEW_SEARCH:	return &m_SearchBar;
	case ID_VIEW_TASKBAR:	return &m_wndTab;
	case ID_VIEW_STATUS_BAR:return &m_wndStatusBar;//must add this for autohide and fullscreen
	}
	return NULL;
}

CString CMainFrame::GetBandTitle(int id)
{
	CString str;
	str.Empty();
	
	if (!m_bShowToolbarLabel)
		return str;
	int strId=0;
	switch(id)
	{
	case ID_VIEW_ADDRESSBAR:strId = IDS_ADDRESS; break;
	case ID_VIEW_LINKS:		strId = IDS_LINKS; break;
	case ID_VIEW_FAVS:		strId = IDS_TITLE_FAVORITE; break;
	case ID_VIEW_EX_TOOL:	strId = IDS_EX_TOOL; break;
	case ID_VIEW_SEARCH:	strId = IDS_SEARCH; break;
	default:
		return str;
	}
	LOADSTR(str,strId);
	return str;
}

void CMainFrame::GetProfileReBarBandsState()
{
	CWinApp* app = AfxGetApp();
	char band[10]="band",size[10]="size",style[10]="style";
	long nBand,nSize,nStyle;
	int i=0;
	while(1)
	{
		itoa(i,band+4,10);
		nBand = _app->GetProfileInt("BandBar", band, 0);
		if (nBand<1)
			break;
		itoa(i,size+4,10);
		itoa(i,style+5,10);
		nSize = _app->GetProfileInt("BandBar", size, 100);
		nStyle = _app->GetProfileInt("BandBar", style, 0);
		SetBandInfo(i,nBand,nStyle,nSize);
		i++;
	}
	//add bar
	BandInfo* pBi;
	i=0;
	int count = countof(g_BandInfo); 
	while(i<count)
	{
		//###############################################################
		pBi = GetBandInfo(i);
		UINT nID = pBi->nID;
		if (!m_bTopTab && nID == ID_VIEW_TASKBAR)
		{
			i++;
			continue;
		}
		UINT nStyle = pBi->fStyle;
		BOOL bNewRow = nStyle & RBBS_BREAK ? TRUE : FALSE;
		int cxWidth = pBi->cx;
		CWnd* pBar = GetBandWnd(nID);
		HWND hWndBand = pBar->m_hWnd;
		
		LPTSTR lpstrTitle = 0;
		CString strTitle = GetBandTitle(nID);
		int nLen = strTitle.GetLength();
		if (nLen)
			lpstrTitle = strTitle.GetBuffer(strTitle.GetLength());
		//
		REBARBANDINFO rbBand;
		rbBand.cbSize = sizeof(REBARBANDINFO);
		rbBand.fMask = RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_ID | RBBIM_SIZE | RBBIM_IDEALSIZE;
		if(lpstrTitle != NULL)
			rbBand.fMask |= RBBIM_TEXT;
		//
		rbBand.fStyle = nStyle;
		if(bNewRow)
			rbBand.fStyle |= RBBS_BREAK;
		
		rbBand.lpText = lpstrTitle;
		rbBand.hwndChild = hWndBand;
		rbBand.wID = nID;
		
		// Calculate the size of the band
		RECT rcTmp;
		int nBtnCount = (int)::SendMessage(hWndBand, TB_BUTTONCOUNT, 0, 0L);
		if(nBtnCount > 0)
		{
			((CToolBar*)pBar)->GetItemRect(nBtnCount - 1, &rcTmp);
			rbBand.cx = cxWidth ? cxWidth : rcTmp.right;
			rbBand.cyMinChild = rcTmp.bottom - rcTmp.top;
			if (rbBand.cyMinChild <15)
				rbBand.cyMinChild = 22;
		}
		else	// no buttons, either not a toolbar or really has no buttons
		{
			::GetWindowRect(hWndBand, &rcTmp);
			rbBand.cx = (cxWidth != 0) ? cxWidth : (rcTmp.right - rcTmp.left);
			rbBand.cyMinChild = max (rcTmp.bottom - rcTmp.top,22 );
			if (rcTmp.bottom <= rcTmp.top)
				((CToolBar*)pBar)->GetItemRect(0,&rcTmp);
		}
		rbBand.cxIdeal = rcTmp.right;//rbBand.cx is not good.
		
		rbBand.cxMinChild = 0;
		//some special
		if (nID == ID_VIEW_ADDRESSBAR)
			rbBand.cxMinChild = 100;
		else if (nID == ID_VIEW_SEARCH)
		{
			if (m_SearchList.m_nIconCount)
			{
				CRect rect1;
				m_SearchBar.GetItemRect(3,&rect1);
				rbBand.cxMinChild = 150 + m_SearchList.m_nIconCount * rect1.Width();
			}
			else
				rbBand.cxMinChild = 150;
		}
		else if (nID == ID_SYSMENU_BAR)
		{
			rbBand.cxMinChild = rcTmp.right;
			rbBand.cyMinChild = 22;//force 22
		}
		else if (nID == ID_VIEW_EX_TOOL)
			rbBand.cyMinChild = 22;//force 22
		
		m_wndReBar.AddBar(pBar, lpstrTitle, NULL, rbBand.fStyle);		
		m_wndReBar.GetReBarCtrl().SetBandInfo(i, &rbBand);
		GetMenu()->CheckMenuItem(nID, nStyle&RBBS_HIDDEN ? MF_UNCHECKED : MF_CHECKED);
		//
		i++;
	}
}

BOOL CMainFrame::ActiveViewSetFocus()
{
	CChildFrame* tcf;
	tcf = (CChildFrame*)MDIGetActive();
	if (VALID_TCF(tcf) && tcf->m_pView->m_lpszUrl)
	{
		tcf->ViewSetFocus();
		return TRUE;
	}
	return FALSE;
}

BOOL CMainFrame::OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	try{
		
		//I don't know why, link bar folder will show a blank tip
		if (!theApp.m_bUseLngFile || pNMHDR->idFrom==ID_LINK_POPUP || pNMHDR->idFrom==ID_FAV_POPUP)
			return CMDIFrameWnd::OnToolTipText(id,pNMHDR,pResult);
		
		CString str;
		LOADSTR(str ,pNMHDR->idFrom);
		if (str.Find('\n')>=0)
			str = str.Right(str.GetLength()-str.Find('\n')-1);
		//
		TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
		TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
		if (pNMHDR->code == TTN_NEEDTEXTA)
		{
			strcpy(pTTTA->szText,str.GetBuffer(str.GetLength()));
		}
		else if (pNMHDR->code == TTN_NEEDTEXTW)
		{
			int len = str.GetLength();
			len = MultiByteToWideChar(CP_ACP,0,str,len, (LPWSTR)pTTTW->szText,80);
			pTTTW->szText[len] = 0;
		}
		return TRUE;
		
	}catch (...) {ASSERT(false); return FALSE;	}
}

void CMainFrame::OnToolsFlyFilter() 
{
	try{
		
		// TODO: Add your command handler code here
		m_bUseFlyingFilter = !m_bUseFlyingFilter;
		if(m_bUseFlyingFilter)
		{
			RefreshKillFlyingCode();
			GET_ACTIVE
				RunKillFlying(tcf);
		}
	}catch (...) {ASSERT(false); 	}
}

void CMainFrame::OnUpdateToolsFlyFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bUseFlyingFilter);
}

void CMainFrame::RefreshKillFlyingCode()
{
	g_strKillFlyingCode = STR_KILL_FLYING;
	g_strKillFlyingLang = STR_JAVASCRIPT;

	//保护关键资产
// 	char* tmp = _FileGetBuffer(theApp.m_strRoot+"Resource\\KillFly.htm");
// 	if (!tmp || strlen(tmp) < 40)
// 	{
// 		g_strKillFlyingCode = STR_KILL_FLYING;
// 		g_strKillFlyingLang = STR_JAVASCRIPT;
// 	}
// 	else
// 	{
// 		g_strKillFlyingCode = tmp;
// 		GetScriptLangCode(g_strKillFlyingCode,g_strKillFlyingLang);
// 	}
// 	FREENULL(tmp);
}

void CMainFrame::RunKillFlying(CChildFrame* tcf)
{
	m_bShowScriptResult = false;
	RunScriptByFrame(g_strKillFlyingCode,g_strKillFlyingLang,tcf);
	m_bShowScriptResult = true;
}

void CMainFrame::RunKillBlank(CChildFrame* tcf)
{
	m_bShowScriptResult = false;
	RunScriptByFrame(g_strKillBlankCode,STR_JAVASCRIPT,tcf);
	m_bShowScriptResult = true;
}

void CMainFrame::RunReloadUnloadImage(CChildFrame* tcf)
{
	m_bShowScriptResult = false;
	RunScriptByFrame(STR_RELOAD_UNSHOWN_IMAGE,STR_JAVASCRIPT,tcf);
	m_bShowScriptResult = true;
}

void CMainFrame::RefreshKillBlankCode()
{
	if (m_bKillBlank)
	{
		g_strKillBlankCode = STR_KILL_BLANK;
		g_strKillBlankCode.Replace("$AD", m_strBlankContent);
	}
	else
		g_strKillBlankCode.Empty();
	
}

void CMainFrame::OnToolsDisDownActivex() 
{
	// TODO: Add your command handler code here
	m_bDisDownActivex = !m_bDisDownActivex;
}

void CMainFrame::OnUpdateToolsDisDownActivex(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bDisDownActivex);
}

LRESULT CMainFrame::OnSetMessageString(WPARAM wParam,LPARAM lParam)
{
	try
	{
		return CFrameWnd::OnSetMessageString(wParam, lParam);
	}
	catch (...) {
		ASSERT(false);
		return 0;
	}
}

LRESULT CMainFrame::OnMenuRButtonUp(WPARAM w,LPARAM l)
{
	try
	{
		
		UINT nID=0;
		//
		CMenu menu;
		menu.Attach((HMENU)l);
		
		MENUITEMINFO mii; 
		mii.fMask = MIIM_DATA|MIIM_TYPE|MIIM_ID|MIIM_SUBMENU;
		mii.dwTypeData = NULL;
		mii.cch = 0;
		mii.dwItemData = NULL;
		mii.cbSize = sizeof(mii);
		menu.GetMenuItemInfo(w, &mii, TRUE);
		CMenuData* pmd = (CMenuData*)(mii.dwItemData);
		menu.Detach();
		nID = mii.wID;
		//fav menu
		if (nID>=B_FAV && nID<=E_FAV)
		{
			CString strUrl;
			if (pmd)
				strUrl = pmd->m_szURL;
			if (strUrl.IsEmpty())
			{
				//for chev menu item
				::GetMenuItemInfo(GetMenu()->m_hMenu, nID, FALSE, &mii);
				pmd = (CMenuData*)(mii.dwItemData);
				if (pmd)
					strUrl = pmd->m_szURL;
			}
			if(strUrl.GetLength())
			{
				m_bAutoMenuEnable  = FALSE;
				CPoint p;
				GetCursorPos(&p);
				ScreenToClient(&p);
				UINT nCmd = DoExplorerMenu(m_hWnd,strUrl,p,TRUE);
				if (nCmd==18 && !_FileIsExist(strUrl))//delete
				{
					int nButtonIndex = 0;
					int nBarType = 0;
					nButtonIndex = m_LinkBar.CommandToIndex(nID);
					if (nButtonIndex >=0)
						nBarType = 1;
					else
					{
						nButtonIndex = m_FavBar.CommandToIndex(nID);
						if (nButtonIndex >=0)
							nBarType = 2;
					}
					DeleteFav(mii.hSubMenu, nID, nButtonIndex, nBarType);
				}
				nID = nCmd;//!! must do this for WM_CANCELMODE
				m_bAutoMenuEnable  = TRUE;
			}
		}
		//group menu
		else if (!m_bGroupMenuShowMember && nID>=B_OPEN_GROUP && nID<=E_OPEN_GROUP)
		{
			g_nGroupSize = 0;
			//
			CMenu menu;
			menu.CreatePopupMenu();
			CString strGroupName;
			int nIndex = nID - B_OPEN_GROUP;
			GetGroupNameByIndex(nIndex, strGroupName);
			MenuLoadGroupURL(&menu, strGroupName, nIndex);
			CPoint p;
			GetCursorPos(&p);
			nID = TrackPopupMenuEx( menu.m_hMenu,
				TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD| TPM_RECURSE,
				p.x, p.y, m_hWnd,NULL );
			if (nID)
			{
				if (nID>=B_GROUP && nID<=E_GROUP)
				{
					MENUITEMINFO mii;
					mii.fMask = MIIM_DATA|MIIM_TYPE;    // get the state of the menu item
					mii.dwTypeData = NULL;
					mii.cch = 0;
					mii.dwItemData = NULL;
					mii.cbSize = sizeof(mii);
					::GetMenuItemInfo(menu.m_hMenu, nID, FALSE, &mii); 
					CMenuData* pmd = (CMenuData*)(mii.dwItemData);
					if (pmd)
						OpenMenuUrl(nID, pmd->m_szURL, pmd->m_dwProperty,0);
				}
				else
					PostMessage(WM_COMMAND, nID);
			}
			DestroyMenuData(menu.m_hMenu,0);
			menu.DestroyMenu();
		}
		//go up menu
		else if (nID>=B_GO_UP && nID<=E_GO_UP)
		{
			NewChildWindow(1,2,g_strGoUpUrl[nID-B_GO_UP]);
		}
		//load image
		else if (nID==ID_OPTIONS_LOADIMAGE)
		{
			CChildFrame* tcf = (CChildFrame*)MDIGetActive();
			if ( VALID_TCF(tcf))
			{
				m_dwDefaultProperty ^= DLCTL_DLIMAGES;//gf

				tcf->m_pView->m_dwProperty ^= DLCTL_DLIMAGES;
				tcf->m_pView->ChangeProperty();
				//not refresh
			}
		}
		else
		{
			PostMessage(WM_COMMAND, nID);
		}
		//
		if (nID)
			PostMessage(WM_CANCELMODE);
		return TRUE;
		
		
	}
	catch (...) {
		CHelperException::ErrorOcurred();
		return FALSE;
	}
}

void CMainFrame::UpdateBandInfo(CToolBar *pBar, UINT uBandID)
{
	REBARBANDINFO rbbi;
	
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask  = RBBIM_IDEALSIZE;
	
	int bc = pBar->GetToolBarCtrl().GetButtonCount();
	if(bc>0)
	{
		CRect rectToolBar;
		pBar->GetToolBarCtrl().GetItemRect(bc-1,&rectToolBar);
		rbbi.cxIdeal = rectToolBar.right;
	}
	else
		rbbi.cxIdeal = 2;
	int bandid = m_wndReBar.GetReBarCtrl().IDToIndex(uBandID);
	m_wndReBar.GetReBarCtrl().SetBandInfo(bandid, &rbbi);
}

void CMainFrame::DeleteFav(HMENU hSubMenu, UINT nID, int nButtonIndex, int nBarType)
{
	//delete fav menu
	if (hSubMenu)
	{
		DestroySubMenu(SMH_FAV, hSubMenu);
	}
	else
	{
		DestroyMenuItemData(SMH_FAV, nID, FALSE);
		SMP_FAV->DeleteMenu(nID, MF_BYCOMMAND);
	}
	//delete toolbar button
	if (nBarType==1)
	{
		m_LinkBar.GetToolBarCtrl().DeleteButton(nButtonIndex);
		UpdateBandInfo(&m_LinkBar,ID_VIEW_LINKS);
		g_astrLinks.RemoveAt(nButtonIndex,1);
	}
	else if (nBarType==2)
	{
		m_FavBar.GetToolBarCtrl().DeleteButton(nButtonIndex);
		UpdateBandInfo(&m_FavBar,ID_VIEW_FAVS);
		g_astrFavs.RemoveAt(nButtonIndex,1);
	}
}

void CMainFrame::OnToolsDialogFilter() 
{
	// TODO: Add your command handler code here
	m_bUseDialogFilter = (m_bUseDialogFilter ? 0 : 1);
	InitialNameSpace();
}

void CMainFrame::OnUpdateToolsDialogFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bUseDialogFilter);
}

BOOL CMainFrame::PopFilterStar(CStringList &astr, const CString& url)
{
	CString str, tmp, tmp2;
	
	try
	{	
		POSITION pos = astr.GetHeadPosition();
		int i = 0, i2=0, start = 0, start2 = 0;//start - str, start2 - tmp
		BOOL cmode = TRUE; //compare mode;
		BOOL isame = FALSE;
		while(pos!=NULL && !isame)
		{
			str = astr.GetAt(pos);

			//gf
			if (str=="*")
			{
				return TRUE;
			}

			//if (str.Find('*')>=0 && str!="*")
			if (str.Find('*')>=0 )
			{
				//star filter
				i = 0; i2 = 0; start = 0; start2 = 0;
				isame = TRUE;cmode = TRUE;
				while(start<str.GetLength() && isame)
				{
					//get the string before *
					i=str.Find('*', start);
					if(i<0)
					{
						tmp2 = str.Mid(start);
						i=str.GetLength();
					}
					else
						tmp2 = str.Mid(start,i-start);
					if(tmp2!="")
					{
						if(cmode)
						{
							tmp = url.Mid(start2, i-start);
							if(tmp != tmp2)
								isame = FALSE;
						}
						else
						{
							i2=url.Find(tmp2, start2);
							if( i2<0)
								isame = FALSE;
						}
					}
					cmode = FALSE;
					start = i+1;
					start2 = i2 + tmp2.GetLength();
				}
			}
			astr.GetNext(pos);
		}
		return isame;
	}
	catch(...)
	{
		return FALSE;
	}
}

BOOL CMainFrame::NeedPopFilter(CString strUrl)
{
	if (PRESS_CTRL)
		return FALSE;
	if (m_astrPopup.Find(strUrl) || PopFilterStar(m_astrPopup,strUrl))
		return NeedUnPopFilter(strUrl);
	else
		return FALSE;
}

BOOL CMainFrame::NeedUnPopFilter(CString strUrl)
{
	//gfpop
	CString strUrlMain = CHelperStr::GetTopUrl(strUrl);
	
	strUrlMain.MakeLower();
	
	if (m_astrUnPopup.Find(strUrlMain) || PopFilterStar(m_astrUnPopup,strUrlMain))
		return FALSE;
	else
		return TRUE;
}

BOOL CMainFrame::NeedFlyFilter(const CString& strUrl)
{
	//gf	if (PRESS_CTRL)
	//		return FALSE;
	if (m_astrFly.Find(strUrl) || PopFilterStar(m_astrFly,strUrl))
		return NeedUnFlyFilter(strUrl);
	else
		return FALSE;
}

BOOL CMainFrame::NeedUnFlyFilter(const CString& strUrl)
{	
	if (m_astrUnFly.Find(strUrl) || PopFilterStar(m_astrUnFly,strUrl))
		return FALSE;
	else
		return TRUE;
}


inline BOOL CMainFrame::UrlFilterStar(CStringArray &pattern, CString url)
{
	CString str, tmp, tmp2;
	
	try{
		
		int i = 0, i2=0, start = 0, start2 = 0;//start - str, start2 - tmp
		BOOL cmode = TRUE; //compare mode;
		BOOL isame = FALSE;
		BOOL final = FALSE;
		int p = 0;
		while(p<=pattern.GetUpperBound() && !isame)
		{
			str = pattern.GetAt(p);
			//star filter
			i = 0; i2 = 0; start = 0; start2 = 0;
			isame = TRUE;cmode = TRUE;final = FALSE;
			while(start<str.GetLength() && isame)
			{
				//get the string before *
				i=str.Find('*', start);
				if(i<0)
				{
					tmp2 = str.Mid(start);
					i=str.GetLength();
					final = TRUE;
				}
				else
					tmp2 = str.Mid(start,i-start);
				if(tmp2!="")
				{
					if(cmode)
					{
						tmp = url.Mid(start2, i-start);
						if(tmp != tmp2)
							isame = FALSE;
					}
					else if(final)
					{
						tmp = url.Right(tmp2.GetLength());
						if(tmp != tmp2)
							isame = FALSE;
					}
					else
					{
						i2=url.Find(tmp2, start2);
						if( i2<0)
							isame = FALSE;
					}
				}
				cmode = FALSE;
				start = i+1;
				start2 = i2 + tmp2.GetLength();
			}
			p++;
		}
		return isame;
	}
	catch(...)
	{
		return FALSE;
	}
}

BOOL CMainFrame::NeedUrlFilter(CString strUrl)
{
	void* tmp;
	if(g_mapUrlFilter.Lookup(strUrl, tmp) || UrlFilterStar(g_aUrlPattern, strUrl))
		return NeedUnUrlFilter(strUrl);
	else
		return FALSE;
}

BOOL CMainFrame::NeedUnUrlFilter(CString strUrl)
{
	void* tmp;
	if(g_mapUrlUnFilter.Lookup(strUrl, tmp) || UrlFilterStar(g_aUrlUnPattern, strUrl))
		return FALSE;
	else
		return TRUE;
}

void CMainFrame::UpdateFilter(BOOL bEnable)
{
	if (bEnable)
	{
		if (m_nEnableFilter&1)	g_bKillPop = TRUE;
		if (m_nEnableFilter&2)	m_bUseUrlFilter = TRUE;
		if (m_nEnableFilter&4)	m_bUseDialogFilter = TRUE;
		if (m_nEnableFilter&8)	m_bUseFlyingFilter = TRUE;
	}
	else
	{
		g_bKillPop = FALSE;
		m_bUseUrlFilter = FALSE;
		m_bUseDialogFilter = FALSE;
		m_bUseFlyingFilter = FALSE;
	}
	//
	InitialNameSpace();
	if(m_bUseFlyingFilter)
	{
		RefreshKillFlyingCode();
		GET_ACTIVE
			RunKillFlying(tcf);
	}
}

void CMainFrame::OnTopMost() 
{
	_bTopMost=!_bTopMost;

	if (_bTopMost)
	{
		::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE);

	}
	else
	{
		::SetWindowPos(GetSafeHwnd(), HWND_NOTOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE);
	}
}

void CMainFrame::OnUpdateTopMost(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(_bTopMost);
}


void CMainFrame::OnEnableFilter() 
{
	// TODO: Add your command handler code here
	BOOL bEnable;
	if (IS_FILTER_ENABLE)
	{
		bEnable = FALSE;
		//
		m_nEnableFilter = 0;
		if (g_bKillPop)			m_nEnableFilter |= 1;
		if (m_bUseUrlFilter)	m_nEnableFilter |= 2;
		if (m_bUseDialogFilter)	m_nEnableFilter |= 4;
		if (m_bUseFlyingFilter)	m_nEnableFilter |= 8;
	}
	else
		bEnable = TRUE;
	UpdateFilter(bEnable);
}

void CMainFrame::OnUpdateEnableFilter(CCmdUI* pCmdUI) 
{
	ToolBarSetCheck(pCmdUI,IS_FILTER_ENABLE);
}


//drag drop link
void CMainFrame::DragDropLink(CString str, BOOL bActive)
{
	if (IS_SCRIPT_MAILTO(str))
		ClickPoint(bActive);
	//can not get url of some link 
	else if (NOT_SEARCH_STR(str) || str.GetLength()==0)
	{
		if (GetChildFrameByUrl(NULL,str))
			return;
		//http://www.sina.com/# add fav link
		if (m_bSimuDragLink || str.Right(1)=="#")
			ClickPoint(bActive);
		else
			NewChildWindow(1,2,str,NULL,FALSE);
	}
	else
	{
		SEARCH_STR(str)
	}
}

//drag drop text, edit text
void CMainFrame::DragDropUrl(CString str, BOOL bActive)
{
	if (IS_SCRIPT_MAILTO(str))
		MSGBOX(IDS_NOT_DRAG_SCRIPT);
	else if (NOT_SEARCH_STR(str))
	{
		if (GetChildFrameByUrl(NULL,str))
			return;
		NewChildWindow(1,2,str,NULL,bActive);
	}
	else
	{
		SEARCH_STR(str)
	}
}

void CMainFrame::OnSortSaveItem(UINT nID)
{
	GET_ACTIVE
		//get menu text
		CString strUrl;
	MENUITEMINFO mii;
	mii.fMask = MIIM_DATA|MIIM_TYPE;
	mii.dwTypeData = NULL;
	mii.cch = 0;
	mii.dwItemData = NULL;
	mii.cbSize = sizeof(mii);
	::GetMenuItemInfo(GetMenu()->m_hMenu, nID, FALSE, &mii); 
	CMenuData* pmd = (CMenuData*)(mii.dwItemData);
	if (!pmd || !pmd->m_szMenuText)
		return;
	//
	BOOL bTemp = m_bShowSortSaveDlg;
	m_bShowSortSaveDlg = FALSE;
	//
	m_strSortSaveFolder = pmd->m_szMenuText;
	m_strSortSaveFolder = m_strSortSaveFolder.Mid(m_strSortSaveFolder.Find(' ')+1);
	//must use send, next will restore m_bShowSortSaveDlg
	tcf->m_pView->SendMessage(WM_COMMAND, ID_FILE_AUTOSAVE_SORT);
	//
	m_bShowSortSaveDlg = bTemp;
}

//use autosaveall to update sort item
void CMainFrame::OnUpdateFileAutoSaveAll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	OnUpdateMenu(pCmdUI);
	
	//update item menu
	try{
		char tmp[5];
		
		CString strTemp;
		CString strPath = theApp.m_strDefaultDirSort;
		strPath += "*.*";
		
		CMenu* pmenu = SMP_SAVE;
		DestroyMenuData(pmenu->m_hMenu,7);
		while(pmenu->DeleteMenu(7, MF_BYPOSITION));
		
		int i=0;
		WIN32_FIND_DATA wfd;
		HANDLE h = FindFirstFile(strPath, &wfd);
		if(h != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				{
					if(lstrcmp(wfd.cFileName, _T(".")) == 0 || lstrcmp(wfd.cFileName, _T("..")) == 0 || lstrcmpi(wfd.cFileName, _T("default")) == 0)
						continue;
					itoa(i+1, tmp, 10);
					strTemp = tmp;
					strTemp += ". ";
					strTemp += wfd.cFileName;
					MenuAppendOwnerItem( pmenu, MF_ENABLED, B_SORT_SAVE + i , strTemp);
					i++;
				}
			}
			while(FindNextFile(h, &wfd) && i<256);
		}
		FindClose(h);
		AddMenuBreak(pmenu, 0);
		
	}catch(...){}
}

CChildFrame* CMainFrame::GetChildFrameByTarget(CChildFrame* tcfCurrent, CString strTarget)
{
	// TODO: Add your command handler code here
	int n = m_wndTab.GetItemCount();
	if (n<1)
		return NULL;
	CChildFrame* tcf;
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	int l =n-1;
	
	try{
		for(int i=n-1;i>=0;i--)
		{
			m_wndTab.GetItem(l, &TabCtrlItem);
			tcf = ((CChildFrame*)TabCtrlItem.lParam);
			if(tcf && tcf != tcfCurrent)
			{
				if (tcf->m_pView->m_strTarget == strTarget)
					return tcf;
			}
			l--;
		}
	}catch(...){}
	
	return NULL;
}

CChildFrame* CMainFrame::GetChildFrameByUrl(CChildFrame* tcfCurrent, CString strUrl)
{
	if (g_bKillPop && g_bKillDupWin)
	{
		int dup = FindDupURL(strUrl, NULL);
		if ( dup>=0 )
		{
			TCITEM TabCtrlItem;
			TabCtrlItem.mask = TCIF_PARAM;
			m_wndTab.GetItem(dup, &TabCtrlItem);
			if(TabCtrlItem.lParam)
			{
				MDIActivate((CChildFrame*)TabCtrlItem.lParam);
				return (CChildFrame*)TabCtrlItem.lParam;
			}
		}
	}
	return NULL;
}

BOOL CMainFrame::PromptMenuCommand(UINT nID)
{
	CString strName;
	_GetMenuItemString(strName, nID);
	int nPos = strName.Find('(');
	if (nPos>0)
		strName = strName.Left(nPos);
	
	CString strMsg;
	LOADSTR( strMsg, IDS_DO_YOU_WANT);
	strMsg += strName;
	strMsg += " ?";
	if (MSGBOX(strMsg, MB_YESNO|MB_ICONQUESTION)==IDYES)
		return TRUE;
	else
		return FALSE;
}

void CMainFrame::OnFavoritesResetOrder()
{
	// TODO: Add your command handler code here
	if (PromptMenuCommand(ID_FAVORITES_RESET_ORDER))
	{
		_RegDeletePath(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\MenuOrder\\Favorites\\");
		m_bFavDirty = TRUE;
	}
}


void CMainFrame::AdjustMenuWidth(CString &str, int nMenuWidth)
{
	if(str.GetLength()<=nMenuWidth)
		return;

	str = str.Left(nMenuWidth);
	int i=0;
	int nCount=0;
	for (i=0;i<nMenuWidth;i++)
	{
		if (str.GetAt(i) & 0x80)
			nCount++;
	}
	if (nCount % 2)
		str.Delete(nMenuWidth-1,1);
	str += "...";
}

BOOL CMainFrame::DoSpecialUrl(CString str)
{
	//down manager must before ftp, for ex: ftp://123/asd.zip
	if (m_nManagerIndex)
	{
		if (IsDownloadAble(str))
		{
			RunDownManagerScript(str,NULL);
			return TRUE;
		}
	}
	if (m_nFtpOpenType)
	{
		if (IS_FTP(str))
		{
			_OpenFtpUrl(str);
			return TRUE;
		}
	}
	DO_ENV_URL(str);
	DO_RELATIVE_URL(str);
	//must after env and relative
	if (m_bOpenFolderOut)
	{
		if (IS_LOCAL_FOLDER(str))
		{
			ShellExecute(NULL, "open", str , "", NULL, SW_SHOW );
			return TRUE;
		}
	}
	//is exe?
	if (IS_DISK_STR(str))
	{
		if(str.Right(4)==".EXE" || str.Right(4)==".exe")
		{
			ShellExecute(NULL, "open", str , "", NULL, SW_SHOW );
			return TRUE;
		}
	}
	
	//no use do res url
	//no use do quick url
	
	return FALSE;
}

#define MAXBUF	4000

#define CLOSEDELNULL(obj);	if(obj != NULL)	{obj->Close(); delete obj; obj= NULL;}

UINT DownloadThreadProc(LPVOID pParam)
{
	char *szUrl = (char*)pParam;
	
	try{
		TCHAR sz[MAXBUF];
		CInternetSession session;
		CStdioFile* pFile = NULL;
		pFile = session.OpenURL(
			szUrl,
			0,
			INTERNET_FLAG_TRANSFER_BINARY |	INTERNET_FLAG_KEEP_CONNECTION
			);
		while(pFile->Read(sz, MAXBUF)>0)
			;
		CLOSEDELNULL(pFile);
		session.Close();
		
		//add to list
		CString strRoot = szUrl;
		strRoot = strRoot.Left(strRoot.GetLength()-11);//remove favicon.ico
		int nIcon = CGf_IE::FindCacheFavIcon(strRoot, pmf->m_wndTab.GetImageList());
		//even not get the icon, must insert the flag to list
		pmf->m_FavIconList.Insert(strRoot,nIcon);
		//update all same root tab icon
		if (IS_FAVICON(nIcon))
			pmf->SetAllTabFavIcon(strRoot,nIcon);
		
	}catch(...){}
	
	FREENULL(szUrl);
	return 0;
}

//gf
//CString _GetCacheFilePath(CString strUrl)
//{
//	CString strPath;
//	strPath.Empty();
//	
//	DWORD dwEntrySize=0;
//	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry;
//	if (!GetUrlCacheEntryInfo(strUrl,NULL,&dwEntrySize))
//	{
//		if (GetLastError()==ERROR_INSUFFICIENT_BUFFER)
//		{
//			lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
//			if (GetUrlCacheEntryInfo(strUrl,lpCacheEntry,&dwEntrySize))
//			{
//				strPath = lpCacheEntry->lpszLocalFileName;						
//			}
//			delete lpCacheEntry;
//			lpCacheEntry = NULL;
//		}
//	}
//	return strPath;
//}

void CMainFrame::SetAllTabFavIcon(CString strRoot, int nIcon)
{
	CChildFrame* tcf;
	int n = m_wndTab.GetItemCount();
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	CString strTemp;
	
	for(int i=0;i<n;i++)
	{
		m_wndTab.GetItem(i, &TabCtrlItem);
		tcf = ((CChildFrame*)TabCtrlItem.lParam);
		if(VALID_TCF(tcf))
		{
			strTemp = _StringGetTopUrl(tcf->m_pView->m_lpszUrl);
			if (strTemp == strRoot)
				SetTabIcon(nIcon,i,tcf->m_pView);
		}
	}
}

//int CMainFrame::FindCacheFavIcon(CString strRoot, CImageList* pImg)
//{
//	//get favicon from cache
//	CString strIcon = _GetCacheFilePath( strRoot+"favicon.ico");
//	if (strIcon.IsEmpty())
//		return FI_NOT_EXIST;
//	//
//	HICON hIconSm;
//	hIconSm = (HICON)LoadImage(NULL, 
//		strIcon,
//		IMAGE_ICON,
//		16, 16,
//		LR_LOADFROMFILE
//		);
//	if (hIconSm)
//	{
//		//gf CImageList* pImg = m_wndTab.GetImageList();
//		int nIcon = pImg->Add(hIconSm);
//		DestroyIcon(hIconSm);
//		return nIcon;
//	}
//	
//	return FI_NOT_EXIST;
//}

int CMainFrame::GetFavIcon(CString strUrl)
{
	if (!IS_HTTP(strUrl))
		return -1;
	
	int nIcon;
	CString strRoot = _StringGetTopUrl(strUrl);
	nIcon = m_FavIconList.Find(strRoot);
	if (nIcon != FI_NOT_IN_LIST)
		return nIcon;
	//get favicon from cache
	nIcon = CGf_IE::FindCacheFavIcon(strRoot, m_wndTab.GetImageList() );
	if (IS_FAVICON(nIcon))
	{
		m_FavIconList.Insert(strRoot,nIcon);
		return nIcon;
	}
	//set downloading flag, avoid repeat download
	m_FavIconList.Insert(strRoot,FI_DOWNLOADING);
	//download favicon
	strRoot += "favicon.ico";
	char *szUrl = (char*)malloc(strRoot.GetLength()+1);
	STRNCPY(szUrl, strRoot, strRoot.GetLength());
	AfxBeginThread(DownloadThreadProc, szUrl);
	//must free the szUrl in thread
	
	return -1;
}

void CMainFrame::AddToAddressList(const CString &strUrl, const CString &strTitle)
{
	
	if (!m_bAddressTyped)
	{
		return;
	}
	
	m_bAddressTyped = FALSE;
	
	
	if ( (!CGFBusiness::IsValidAdressList(strUrl) ) || CHelperStr::IsTitleBad(strTitle))
	{
		return;
	}
	
	CString str = CGFBusiness::GetAddressShowStr(m_strAddressTyped, strTitle);
	
	POSITION pos = m_slAddressTypedUrls.Find(m_strAddressTyped);
	
	if( NULL != pos)
	{
		m_slAddressTypedUrls.RemoveAt(pos);
		
		if(GF_ADD_ADRRESS_TITLE)
			m_AddressTypedUrlTitles.RemoveKey(m_strAddressTyped);
		
		{
			
			int iPos = m_AddressBar.FindStringExact(-1, CString(m_strAddressTyped) ); 
			if (iPos != CB_ERR)
			{
				m_wndAddress->DeleteItem(iPos);
			}	
		}
		
		{
			
			int iPos = m_AddressBar.FindStringExact(-1, str); 
			if(iPos != CB_ERR)
			{
				m_wndAddress->DeleteItem(iPos);
			}
			
		}
		
	}
	
	//	if (str.Right(1)=='/')
	//		str = str.Left(str.GetLength()-1);
	//	else
	//		str += '/';
	
	
	{
		COMBOBOXEXITEM item;
		item.pszText = (LPTSTR)(LPCTSTR)str;
		item.mask = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE ;
		item.iItem = 0;
		item.iImage = 1;
		item.iSelectedImage = 1;
		m_wndAddress->InsertItem(&item);
		m_slAddressTypedUrls.AddHead(m_strAddressTyped);
		
		if (GF_ADD_ADRRESS_TITLE)
		{
			m_AddressTypedUrlTitles.SetAt(m_strAddressTyped, strTitle);
		}
		
	}
	
}

CString CMainFrame::GetAddressTypedUrlFromList(int iPos)
{
	return CHelperStr::GetStrFromList(m_slAddressTypedUrls, iPos);
}

void CMainFrame::SetAddressText(LPCTSTR lpszUrl)
{
	if ( 0 == StrCmp("about:blank", lpszUrl) )
	{
		m_wndAddress->SetWindowText( "" );
	}
	else
	{
		m_wndAddress->SetWindowText( lpszUrl );
	}

	//m_wndAddress->SetWindowText( CGf_TranslateUrl::DealUrlToShowToUser(lpszUrl) );

	

	if ( (GetTickCount() - CGf_AppStatic::_instance._dwStart) > 300000) //60000=1min 避免启动时不全选
	{
		::SendMessage(m_wndAddress->GetEditCtrl()->m_hWnd, EM_SETSEL, 0, 0); //gf http显示在前面
	}
    
	
}



BOOL CMainFrame::IsInUnFilter(const CString &strUrl)
{
	
	if (!NeedUnPopFilter(strUrl) || IS_FILE(strUrl)) //for 本地导航页
	{
		return TRUE;
	}
	
	return FALSE;
}

//gf
void CMainFrame::AddToUndoList(const CString &strName, const CString &strUrl)
{
	if ( ("about:blank" == strUrl) || ("http:///" == strUrl) ) //gf
	{
		return;
	}
	
	if ( (!m_strUndoURL.IsEmpty() ) && (strUrl == m_strUndoURL.GetHead() ) )
	{
		return;
	}
	
	if (m_strUndoName.GetCount() > MAX_UNDO)
	{
		
		m_strUndoURL.RemoveTail();
		m_strUndoName.RemoveTail();
		
	}
	
	m_strUndoURL.AddHead(strUrl);
	m_strUndoName.AddHead(strName);
	
}

void CMainFrame::Gf_delayLoad()
{

// 	CGf_UI::SetIconForEmptyCombo(m_wndSearch, 1);
// 	CGf_UI::SetIconForEmptyCombo(m_wndAddress, 1);

//	m_wndAddress->GetEditCtrl()->SetFocus(); //gf

	CRegHelper::SetMaxHttp();

	CRegHelper::SetPCID();
	
//	CMyIE9App* app = (CMyIE9App*)AfxGetApp();
	
	
	CGf_AppStatic::_instance._bEnableMixContentAlert = _app->GetProfileInt("Settings", "EnableMixContentAlert", FALSE);
	if (CGf_AppStatic::_instance._bEnableMixContentAlert)
	{
		CRegHelper::EnableMixContentAlert();
	}
	else
	{
		CRegHelper::DisableMixContentAlert();
	}

	CGf_AppStatic::_instance._bDirectOpenForOneWord = _app->GetProfileInt("Settings", "DirectOpenForOneWord", TRUE);
	
	CGf_AppStatic::_instance._bAlwaysIEMenuRightClickImg = _app->GetProfileInt("Settings", "AlwaysIEMenuRightClickImg", CGf_AppStatic::_instance._bAlwaysIEMenuRightClickImg);

	CGf_AppStatic::_instance._bEnableIE8Mode = _app->GetProfileInt("Settings", "EnableIE8Mode", FALSE);
	if (CGf_AppStatic::_instance._bEnableIE8Mode)
	{
		CRegHelper::EnableIE8Mode();
	}
	else
	{
		CRegHelper::DisableIE8Mode();
	}
	
	CRegHelper::AllowActiveX(FALSE); // pmf->m_bDisDownActivex);
	
	RefreshKillFlyingCode();

// 	if (!CPathHelper::IsExist(CGfPathManager::GetUserStartPage() ) )
// 	{
// 		CPathHelper::CopyDirectory(CGfPathManager::GetResStartPage(), CGfPathManager::GetUserStartPage());
// 	}
	
	CFilterSites m_oFileFilter; //便于起其内的线程
	m_oFileFilter.Initial(CGfPathManager::GetBadSiteFile(), CGfPathManager::GetNotBadSiteFileSystem(), CGfPathManager::GetNotBadSiteFileUser(), CGfPathManager::GetUserBadSiteFile() );
	
	if (!CGf_AppStatic._instance._bForbidAutoUpdate)
	{
		CGf_Update::Execute();
	}	
	
	
}

BOOL CMainFrame::IsPointImage()
{
	
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();
	if (!tcf || !tcf->m_pView)
		return FALSE;
	
	return CGf_html::IsPointImage((tcf->m_pView)->GetWebBrowser(), (tcf->m_pView)->GetPointInView(m_ptMouseDown) );
}

void CMainFrame::DeleteTabBkImgs()
{
	try{
	

		if (hbTabBkTop)
		{
			DeleteObject(hbTabBkTop);
			hbTabBkTop = NULL;
		}

		
		if (hbTabBkTopActive)
		{
			DeleteObject(hbTabBkTopActive);
			hbTabBkTopActive = NULL;
		}
		
		if (hbTabBkBottom)
		{
			DeleteObject(hbTabBkBottom);
			hbTabBkBottom=NULL;
		}
		
		if (hbTabBkBottomActive)
		{
			DeleteObject(hbTabBkBottomActive);
			hbTabBkBottomActive=NULL;
		}

	}catch (...) {ASSERT(false);}
}

CString CMainFrame::GetCurrentUrl()
{
	CString strUrl = "";
	CChildFrame* tcf = (CChildFrame*)MDIGetActive();	
	if (!tcf || !tcf->m_pView) return strUrl;
	
	tcf->m_pView->GetLocationURL(strUrl);
	
	return strUrl;
	
}

CString CMainFrame::GetDomain()
{
	CString strUrl;
	
	m_wndAddress->GetEditCtrl()->GetWindowText(strUrl);

	return CHelperStr::GetTopUrl(strUrl);

}

void CMainFrame::OnNcMouseMove(UINT nHittest, CPoint point)
{
	GF_TRY
	{
	

	if (IsInSysCloseButton(point) )
	{
		_ttSys.Activate(TRUE);

		MSG msg;
		msg.hwnd=this->GetSafeHwnd();
		msg.message=WM_MOUSEMOVE;
		msg.pt=point;

		_ttSys.RelayEvent(&msg);
	}
	else
	{
		_ttSys.Activate(FALSE);
	}

	
	}
	GF_CATCH
}

void CMainFrame::GetSysCloseButtonRect(CRect& rt)
{
	
	CRect rtTitle, rtWnd;
	
	GetWindowRect(&rtWnd);
	
	rtTitle.left=GetSystemMetrics(SM_CXFRAME);
	rtTitle.top=GetSystemMetrics(SM_CYFRAME);
	rtTitle.right=rtWnd.right-rtWnd.left- GetSystemMetrics(SM_CXFRAME);
	rtTitle.bottom=rtTitle.top+GetSystemMetrics(SM_CYSIZE);
	
	rt.left=rtTitle.right-22;
	rt.top=rtTitle.top;
	rt.right=rtTitle.right;
	rt.bottom=rtTitle.top+24;
}

void CMainFrame::OnNcRButtonUp(UINT nHittest, CPoint point)
{
	GF_TRY
	{
		
		if (!IsInSysCloseButton(point))
		{
			return;
		}
		
		
		if((g_bTopCloseExitApp) && (m_wndTab.GetItemCount() > 0) )
		{
			OnFileClose();
			return;
		}
		else
		{
			DoClose();
		}
		
	}
	GF_CATCH
		
		
}

BOOL CMainFrame::IsInSysCloseButton(const CPoint &point)
{

	CRect rtWnd;
	GetWindowRect(&rtWnd);

	CPoint ptClient = point;
	ptClient.x=point.x-rtWnd.left;
	ptClient.y=point.y-rtWnd.top;
		

	CRect rt;
	GetSysCloseButtonRect(rt);

	return (::PtInRect(&rt, ptClient) );

}

CString CMainFrame::GetSysXTip()
{
	CString strSysXTip;
	if (g_bTopCloseExitApp)
	{
		strSysXTip = CGfAppString::GetString(IDS_SYS_X_TIP_SETTED);
	}
	else
	{
		strSysXTip = CGfAppString::GetString(IDS_SYS_X_TIP);
	}
	
	return strSysXTip;
	
}

void CMainFrame::OnGoBackAndClose()
{
	try{
		if(m_wndTab.GetItemCount() < 1)
			return;

		GET_ACTIVE
		
		if(tcf->m_pView->m_bBack)
			tcf->m_pView->GoBack();
		else if(m_wndTab.GetItemCount() > 1) //gfback
		{
			OnFileClose();
		}
	}catch(...){ASSERT(false);}
}

CChildFrame* CMainFrame::NewChildWindowInBack(const CString &url)
{
	BOOL btmp=m_bActiveNewWindow;
	m_bActiveNewWindow=0;
	CChildFrame* rt=NewChildWindow(1,2,url);
	m_bActiveNewWindow=btmp;

	return rt;
}

CChildFrame* CMainFrame::NewChildWindowInBack(int nIniLevel)
{
	BOOL btmp=m_bActiveNewWindow;
	m_bActiveNewWindow=0;
	CChildFrame* rt=NewChildWindow(nIniLevel);
	m_bActiveNewWindow=btmp;
	
	return rt;
}

BOOL CMainFrame::OnCreateMovedToDelay()
{

	// link and fav menu
	// 	g_bCompactFavBar = _app->GetProfileInt("Settings", "CompactFavBar", FALSE);
	// 	m_nMenuWidth = _app->GetProfileInt("Settings", "MenuSize", 8);
	// 	CGf_AppStatic::_instance._nMenuWidthOther = _app->GetProfileInt("Settings", "MenuSizeOther", CGf_AppStatic::_instance._nMenuWidthOther);
	// 	m_bDisplayOpenAll = _app->GetProfileInt("Settings", "DisplayOpenAll", TRUE);
	// 	m_bDisplayAddFav = _app->GetProfileInt("Settings", "DisplayAddFav", TRUE);
	// 	m_bShowHiddenFav = _app->GetProfileInt("Settings", "ShowHiddenFav", FALSE);
	// 	g_bSingleMenu = _app->GetProfileInt("Settings", "SingleMenu", FALSE);
	// 	m_bUseDefaultFavPath = _app->GetProfileInt("Settings", "UseDefaultFavPath", TRUE);
	// 	m_strFavPath = _app->GetProfileString("Settings", "FavPath", "");
	// 	m_bUseDefaultLinkPath = _app->GetProfileInt("Settings", "UseDefaultLinkPath", TRUE);
	// 	m_strLinkPath = _app->GetProfileString("Settings", "LinkPath", "");
	// 	m_bFavShowUrl = _app->GetProfileInt("Settings", "FavShowUrl", TRUE);
	
	if (m_bUseDefaultFavPath) //gff 
	{
		CString strDefaultFavPath;
		LOADSTR(strDefaultFavPath, IDS_DEFAULT_FAV_PATH);
		
		CString strFavFromReg="";
		CString csFavoritePath="";
		
		if (CGf_AppStatic::_instance._bStartFirstTime && CPathHelper::IsNeedChangeFavDir(strDefaultFavPath, strFavFromReg, csFavoritePath))
		{
			CString strMsgFav,strMsg;
			LOADSTR(strMsgFav ,IDS_FAV_ASK);
			
			strMsg.Format(strMsgFav, csFavoritePath, strFavFromReg);
			
			if (MSGBOX(strMsg, MB_YESNO|MB_ICONQUESTION)==IDYES)
			{
				
				CPathHelper::CreateFullDir(csFavoritePath);
				
				CPathHelper::CopyDirectory(strFavFromReg, csFavoritePath);
				
				CRegHelper::SetFavoriteFolder(csFavoritePath);
			}
			
		}
		
	}
	

	
	GF_TRY
	{
		
		InitFavorites(TRUE);
	}
	GF_CATCH

	
	//better place here, not flash when delay load config
	CString wp;
	CExternalUtilItem* eui;
	wp = _app->GetProfileString("ExUtils", "name1", NULL);
	if(wp == "" || wp.IsEmpty())
	{
	}
	else
	{
		//new format
		char Keyname1[10] = "name", Keyname2[10] = "cmds", Keyname3[10] = "sta", Keyname4[10] = "cls", Keyname5[10] = "run";
		char Keyname11[10] = "para", Keyname12[10] = "path";
		CString cmd,para,path;
		BOOL bstart, bClose,bRun;
		int i=1;
		cmd = _app->GetProfileString("ExUtils", "cmds1", NULL);
		para = _app->GetProfileString("ExUtils", "para1", NULL);
		path = _app->GetProfileString("ExUtils", "path1", NULL);
		bstart = _app->GetProfileInt("ExUtils", "sta1", FALSE);
		bClose = _app->GetProfileInt("ExUtils", "cls1", bstart);
		bRun = _app->GetProfileInt("ExUtils", "run1", FALSE);
		while(wp != "" && !wp.IsEmpty())
		{
			eui = new CExternalUtilItem;
			eui->m_strUtilName = wp;
			eui->m_strUtilCmd = "\""+cmd+"\"";
			eui->m_strUtilPara = para;
			eui->m_strUtilPath = path;
			eui->m_bUtilStart = bstart;
			eui->m_bUtilClose = bClose;
			eui->m_bUtilRun = bRun;
			m_ExternalUtilList.m_UtilList.Add(eui);
			i++;
			itoa(i, Keyname1+4, 10);
			itoa(i, Keyname2+4, 10);
			itoa(i, Keyname11+4, 10);
			itoa(i, Keyname12+4, 10);
			//
			itoa(i, Keyname3+3, 10);
			itoa(i, Keyname4+3, 10);
			itoa(i, Keyname5+3, 10);
			wp = _app->GetProfileString("ExUtils", Keyname1, NULL);
			cmd = _app->GetProfileString("ExUtils", Keyname2, NULL);
			para = _app->GetProfileString("ExUtils", Keyname11, NULL);
			path = _app->GetProfileString("ExUtils", Keyname12, NULL);
			bstart = _app->GetProfileInt("ExUtils", Keyname3, FALSE);
			bClose = _app->GetProfileInt("ExUtils", Keyname4, bstart);
			bRun = _app->GetProfileInt("ExUtils", Keyname5, FALSE);
		}
	}

	m_ExternalTool.ShowWindow(SW_HIDE);

	BuildUtilMenu();

	ShowToolbarIfShow(m_ExternalTool, ID_VIEW_EX_TOOL);
	
	
	DelayLoadIniUrgent();
	
	
//	InitFavorites();
	
	//load win buttons
	char key[5]="W";
	for(int i=0; i<6; i++)
	{
		itoa(i, key+1, 10);
		m_abWinButtons[i] = _app->GetProfileInt("WinButtons", key, 1);
	}
	InitSystemBarImage();
	BuildWinButtons();
	
	//
	m_bMenuLoadLastClose = _app->GetProfileInt("Settings", "MenuLoadLastClose", 1);
	if (m_bMenuLoadLastClose)
		OpenLastClose();


	CGf_AppStatic::_instance._bForbidAutoUpdate = _app->GetProfileInt("Settings", "ForbidAutoUpdate", FALSE); //gfsize
	CGf_AppStatic::_instance._bKeepPageSize = _app->GetProfileInt("Settings", "KeepPageSize", FALSE);
	CGf_AppStatic::_instance._iPageSize = _app->GetProfileInt("Settings", "PageSize", 100);
	
	LoadTypedSearch();

	return TRUE;
}

void CMainFrame::DelayMustInUI()
{
	
	//register drag
	ms_TabDropTarget = new CTabDropTarget;
	ms_TabDropTarget->Register(&m_wndTab);
	
	//namespace handle
	m_pSession = NULL;
	m_pFactory = NULL;
	InitialNameSpace();

	OnToolsCollector(FALSE);//must create collector first!

	//gfxtip
	EnableToolTips(TRUE);
	_ttSys.Create(this);
	
	_ttSys.AddTool(this, GetSysXTip() );
	
	_ttSys.SetDelayTime(2000);
	_ttSys.SetDelayTime(TTDT_INITIAL, 400);
	
}

void CMainFrame::DelayLoadIniUrgent()
{
	LoadTypedURL();
	
	m_bOpenInNew = _app->GetProfileInt("Settings", "OpenInNew", 1);
				
	//always new window
	g_bAddressInNewWindow = _app->GetProfileInt("Settings", "AddressInNewWindow", 1);
	g_bLinkInNewWindow = _app->GetProfileInt("Settings", "LinkInNewWindow", 0);
	g_bFavInNewWindow = _app->GetProfileInt("Settings", "FavInNewWindow", TRUE);
	g_bHisInNewWindow = _app->GetProfileInt("Settings", "HisInNewWindow", TRUE);
	g_bFileInNewWindow = _app->GetProfileInt("Settings", "FileInNewWindow", TRUE);
	g_bHomeInNewWindow = _app->GetProfileInt("Settings", "HomeInNewWindow", TRUE);
	
	m_bActiveFileNew = _app->GetProfileInt("Settings", "ActiveFileNew", TRUE);
	m_bGoProxyMenu = _app->GetProfileInt("Settings", "GoProxyMenu2", TRUE);
	
	//
	g_bClickClose = _app->GetProfileInt("Settings","ClickClose", 0);
	//confirm when exit
	g_bConfirmExit = _app->GetProfileInt("Settings", "ConfirmExit", FALSE);
	//use shortcuts
	m_bUseSC = _app->GetProfileInt("Settings", "UseSC", FALSE);
	//use alias
	m_bUseAlias = _app->GetProfileInt("Settings", "UseAlias2", TRUE);	
	//sequence
	m_nCloseSequence = _app->GetProfileInt("Settings", "CloseSequence", 1); //gf 2
	//confirm
	g_bConfirmCloseAll  = _app->GetProfileInt("Settings", "ConfirmCloseAll", FALSE);
	g_bConfirmCloseAllKeepCur  = _app->GetProfileInt("Settings", "ConfirmCloseAllKeepCur", FALSE);
	g_bConfirmCloseLock = _app->GetProfileInt("Settings", "ConfirmCloseLock", TRUE); //gf
	//new window
	m_nNewWindow = _app->GetProfileInt("Settings", "NewWindow", 0);
	SMP_NEW->SetDefaultItem(m_nNewWindow, TRUE); //gfm
	//allow double click to close window
	m_nAllowDBC = _app->GetProfileInt("Settings", "AllowDBC", 1);
	m_nAllowMC = _app->GetProfileInt("Settings", "AllowMC", 5); //gf 4
	m_nAllowRC = _app->GetProfileInt("Settings", "AllowRC2", 0); //gf 5
	m_nLDbClickTabBlank = _app->GetProfileInt("Settings", "LDbClickTabBlank", ID_FILE_NEW_BLANK);
	m_nRDbClickTabBlank = _app->GetProfileInt("Settings", "RDbClickTabBlank", ID_FILE_NEW_CURRENTPAGE);
	
	//mouse
	m_nLDbClick = _app->GetProfileInt("Mouse", "LDbClick", 0);
	m_nRDbClick = _app->GetProfileInt("Mouse", "RDbClick", 0); // //gf
	m_nLButtonDown = _app->GetProfileInt("Mouse", "LButtonDown", 0);
	m_nMButtonDown = _app->GetProfileInt("Mouse", "MButtonDown", ID_OPEN_LINK_DEACTIVE);
	m_nRButtonDown = _app->GetProfileInt("Mouse", "RButtonDown", 0);
	m_nLeftRight = _app->GetProfileInt("Mouse", "LeftRight", 0); //gf 0
	m_nRightLeft = _app->GetProfileInt("Mouse", "RightLeft", ID_FILE_CLOSE_1); //0
	m_bShowGesture = _app->GetProfileInt("Mouse", "ShowGesture", TRUE);
	//
	m_bWheelTab = _app->GetProfileInt("Mouse", "WheelTab", 1);
	m_bWheelOnTab = _app->GetProfileInt("Mouse", "WheelOnTab", 0);
	//
	m_nImgUp = _app->GetProfileInt("Mouse", "ImgUp", 6);
	m_nImgDown = _app->GetProfileInt("Mouse", "ImgDown", 1);
	m_nImgLeft = _app->GetProfileInt("Mouse", "ImgLeft", 3);
	m_nImgRight = _app->GetProfileInt("Mouse", "ImgRight", 1);
	//
	m_nTextUp = _app->GetProfileInt("Mouse", "TextUp", 7);
	m_nTextDown = _app->GetProfileInt("Mouse", "TextDown", 9);
	m_nTextLeft = _app->GetProfileInt("Mouse", "TextLeft", 2);
	m_nTextRight = _app->GetProfileInt("Mouse", "TextRight", 3);
	//
	m_nLinkUp = _app->GetProfileInt("Mouse", "LinkUp", 9);
	m_nLinkDown = _app->GetProfileInt("Mouse", "LinkDown", 3);
	m_nLinkLeft = _app->GetProfileInt("Mouse", "LinkLeft", 2);
	m_nLinkRight = _app->GetProfileInt("Mouse", "LinkRight", 3);
	//
	m_bSimuDragLink = _app->GetProfileInt("Mouse", "SimuDragLink", TRUE);
	//
	m_bRMouseDrag = _app->GetProfileInt("Mouse", "RMouseDrag", 0);
	m_bRMouseGesture = _app->GetProfileInt("Mouse", "RMouseGesture2", 1);
	//
	/*m_nMenuStyle = _app->GetProfileInt("Settings", "MenuStyle", 1);*/
	//
	m_bBlankMonitorClip = _app->GetProfileInt("Settings", "BlankMonitorClip2", TRUE);
	
	
	m_GestureList.LoadGestureList();

	//load alias
	TCHAR key[10];
	CString url, alias;
	int i=0; //int j;
	BOOL r = TRUE;
	while(r)
	{
		key[0] = 'A';
		itoa(i, key+1, 10);
		alias = _app->GetProfileString("Alias", key);
		if(alias=="" || alias.IsEmpty())
			r=FALSE;
		else
		{
			alias.TrimLeft();
			alias.TrimRight();
			key[0] = 'U';
			itoa(i, key+1, 10);
			url = _app->GetProfileString("Alias", key);
			if(url=="" || url.IsEmpty())
				r=FALSE;
			else
				m_mapAlias.SetAt(alias, url);
		}
		i++;
	}

	//Domains
	m_strCE1 = _app->GetProfileString("Domains", "CE1", "http://www.");
	m_strCE2 = _app->GetProfileString("Domains", "CE2", ".com");
	m_strSE1 = _app->GetProfileString("Domains", "SE1", "http://www.");
	m_strSE2 = _app->GetProfileString("Domains", "SE2", ".net");
	m_strCSE1 = _app->GetProfileString("Domains", "CSE1", "http://www.");
	m_strCSE2 = _app->GetProfileString("Domains", "CSE2", ".com.cn");

	
	m_strStatusStringFormat = _app->GetProfileString("Settings", "StatusDispFormat", "L_J_D:E:F");
	//
	m_bCloseCleanCache = _app->GetProfileInt("CloseMode", "CloseCleanCache", 0);
	m_bCloseCleanCookies = _app->GetProfileInt("CloseMode", "CloseCleanCookies", 0);
	m_bCloseCleanFormData = _app->GetProfileInt("CloseMode", "CloseCleanFormData", 0);
	m_bCloseCleanHistory = _app->GetProfileInt("CloseMode", "CloseCleanHistory", 0);
	m_bCloseCleanTypedAddress = _app->GetProfileInt("CloseMode", "CloseCleanTypedAddress", 0);
	m_bCloseCleanTypedSearch = _app->GetProfileInt("CloseMode", "CloseCleanTypedSearch", 0);
	m_bCloseCleanUndoClose = _app->GetProfileInt("CloseMode", "CloseCleanUndoClose", 0);

	CGf_AppStatic::_instance._bAutoHideFavSideBar = _app->GetProfileInt("Settings", "AutoHideFavSideBar", FALSE);

	
	m_bGroupMenuShowMember = _app->GetProfileInt("Settings", "GroupMenuShowMember2", TRUE);
	m_bCloseAllBeforeNewGroup = _app->GetProfileInt("Settings", "CloseAllBeforeNewGroup", FALSE);
	InitGroups();
		
}

void CMainFrame::ShowToolbarIfShow(CToolBar& bar, UINT uBandID )
{
	
		REBARBANDINFO rbbi;
		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask = RBBIM_ID|RBBIM_STYLE;
		int nb = m_wndReBar.GetReBarCtrl( ).IDToIndex(uBandID);
		m_wndReBar.GetReBarCtrl().GetBandInfo(nb, &rbbi);
		
		if(!( rbbi.fStyle & RBBS_HIDDEN) )
		{
			bar.ShowWindow(SW_SHOW);
			//m_wndReBar.GetReBarCtrl( ).ShowBand(nb, TRUE);
		}			
		
}

void CMainFrame::DoCloseSave()
{
	if (!CGf_AppStatic::_instance._bDelayLoaded)
	{
		return;
	}

	
	int iStartMode=CGf_StartupManager::GetStartMode(CGfPathManager::GetUserConfig());
	if (STARTUP_MYIE9_START==iStartMode)
	{
		CGf_StartPageMgr::_ins.EnableWrite();
	}
	
	//if (bGoHome==2 || bGoHome==3)
	SaveLastVisit(0);
	
	//gf is not load last visit start mode
	CWinApp* app = AfxGetApp();
	if (STARTUP_LAST != iStartMode ) 
	{
		try
		{
			int n = m_wndTab.GetItemCount();
			for( int i=0; i<n; i++)
			{
				CChildFrame* tcf;
				TCITEM TabCtrlItem;
				TabCtrlItem.mask = TCIF_PARAM;
				m_wndTab.GetItem(i, &TabCtrlItem);
				tcf = ((CChildFrame*)TabCtrlItem.lParam);
				if(VALID_TCF(tcf))
				{
					if(!tcf->m_pView->m_lpszUrl.IsEmpty() && !tcf->m_pView->m_bToClose)
					{
						if (tcf->m_pView->m_lpszUrl != "about:blank")
						{
							
							CString strName;
							GET_TAB_TITLE(tcf,strName);
							AddToUndoList(strName, tcf->m_pView->m_lpszUrl);				
							
						}
					}
				}
			}
			
		}catch (...) {
			ASSERT(false);
		}
		
		
	}
	
	//if (m_bMenuLoadLastClose)
	SaveLastClose();
	
	//better place here
	if (m_bCloseCleanCache)
		OnToolsCache();
	if (m_bCloseCleanCookies)
		OnToolsCleanCookie();
	if (m_bCloseCleanFormData)
		OnToolCleanMyIE9form();		
	if (m_bCloseCleanHistory)
		OnToolsHistory();
	if (m_bCloseCleanTypedAddress)
		OnToolsCleanaddr();
	if (m_bCloseCleanTypedSearch)
		OnCleanTypedSearch();
	if (m_bCloseCleanUndoClose)
		OnToolsCleanUndoClose();
	//
	//CloseExternalUtil();
	
	//use may disable filter temp, must before saveconf
	if (IS_FILTER_ENABLE)
	{
		m_nEnableFilter = 0;
		if (g_bKillPop)			m_nEnableFilter |= 1;
		if (m_bUseUrlFilter)	m_nEnableFilter |= 2;
		if (m_bUseDialogFilter)	m_nEnableFilter |= 4;
		if (m_bUseFlyingFilter)	m_nEnableFilter |= 8;
	}		
	//save other config
	theApp.SaveConfg();
	
	SaveTypedURL();
	
	CGf_StartPageMgr::_ins.Write();
	
	//UninitialNameSpace();
	//ClearSpeedMonitor();
	
	if (CGf_AppStatic::_instance._bRestoreConfig)
	{
		CGfPathManager::RestoreUserConfig();
	} 
	else
	{
		CGf_UpdateFilterIniControl::Do();
	}

}
