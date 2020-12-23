#pragma once


#define GF_MSG_ALREADY_LATEST_VERSION 1 //_T("无须升级，已经是最稳定的版本了：V%s.0")

#define  GF_LINE_1_NOT_CONCERN 2//_T("MyIE9浏览器的自动静默升级程序。\n\n不推荐您调用本程序，系统会在合适的时候自动调用本升级程序，您无需关注升级。")

#define GF_WAITING 3//_T("\n正在升级，请等待... ...")


#define GF_FAILED 4//_T("升级失败。请到网站下载最新版安装包。\n")

#define GF_UPGRADE_OK 5//_T("\n升级成功。重新启动MyIE9软件后生效。\n")
#define GF_FINISHED 6//_T("升级完毕，本窗口在5秒内会自动关闭.")
#define GF_PRESS_ENTER 7

#define GF_URL_VERSION_BEGIN _T("MyIE9Version:")
#define GF_URL_VERSION_END _T(":MyIE9VersionEnd")


//#define GF_URL_DOWNLOAD_NUMBER 1

//#define GF_URL_DOWNLOAD "http://cnupd%d.myie9.cn" //"http://test%d.myie9.cn"

#define GF_URL_VERSION _T("http://myie9.sourceforge.net/d/ver.htm")
#define GF_URL_TEST_VERSION _T("http://myie9.sourceforge.net/d/testver.htm")

#define GF_URL_VERSION2 _T("http://myie9.cn/d/ver.htm")
#define GF_URL_TEST_VERSION2 _T("http://myie9.cn/d/testver.htm")
/////////////////////////
// #define GF_URL_DOWNLOAD "http://enupd%d.myie9.cn" //"http://testen%d.myie9.cn"
// #define GF_URL_VERSION _T("http://veren.myie9.cn")

//

///////////////////////////////////////////

//#define GF_URL_DOWNLOAD_NUMBER 3

/////////////////////////////////////////////////////////////////////////
//#define GF_URL_VERSION_BEGIN _T("/version/v")
//#define GF_URL_VERSION_END _T("/")
//
//#define GF_MSG_ALREADY_LATEST_VERSION _T("No need to upgrade. It's already the most stable version: V%s.0")
//
//#define  GF_LINE_1_NOT_CONCERN _T("MyIE9 silent upgrade.\n\nYou needn't care about upgrade. The upgrade will be executed by MyIE9 at proper time silently.")
//
//#define GF_WAITING _T("\nUpgrading, please wait...")
//
//
//#define GF_FAILED _T("Failed to upgrade. Please download setup file in MyIE9 homepage.\n")
//
//#define GF_UPGRADE_OK _T("\nSucceeded. Please restart MyIE9 to take effect.\n")
//  #define GF_FINISHED _T("Finished. The window will be closed in 5 seconds.")