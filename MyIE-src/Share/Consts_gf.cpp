// Consts.cpp: implementation of the CConsts class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Consts_gf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/*
const CString CConsts::_saGoodSites = "\
	baidu.com\
	google.com\
	sina.com\
	sina.com.cn\
	sohu.com\
	yahoo.com.cn\
	taobao.com\
	doubleclick.net\
	localhost\
	tencent.com\
	qq.com\
	";
	*/

const CString CConsts::_saFullUrlBadSites = "";

const CString CConsts::_strSpaceBetweenTitleAndUrl = _T("   ");

const  int CConsts::_iAdressListMaxItems = 24;  // 12  items
const  int CConsts::_iAdressListUIHight = 450;  //gfl list of address. 450   124/6 155/8 220/12

const  int CConsts::_iTitleShow = 10;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConsts::CConsts()
{

}

CConsts::~CConsts()
{

}
