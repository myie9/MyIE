// Consts.h: interface for the CConsts class.

//

//////////////////////////////////////////////////////////////////////



#if !defined(AFX_CONSTS_H__F4D89EDD_AD60_4FB6_A27E_D15B2FFBC4C0__INCLUDED_)

#define AFX_CONSTS_H__F4D89EDD_AD60_4FB6_A27E_D15B2FFBC4C0__INCLUDED_



#if _MSC_VER > 1000

#pragma once

#endif // _MSC_VER > 1000



#define GF_INIT_SIZE_MAP_NOTBAD 20

#define GF_INIT_SIZE_MAP_BAD 9000

//########################################################
//tab icon
#define TI_NO		-1	//no icon
#define TI_0		0	//blank page
#define TI_1		1	//progress 1	0   ~ 33%
#define TI_2		2	//progress 2	33% ~ 66%
#define TI_3		3	//progress 3	66% ~ 99%
#define TI_LOCK		4
#define TI_100		5	//100%
#define TI_PROTECT	6
#define TI_SAVE		7
#define TI_CLOSE	8
#define TI_READ		9
#define TI_MARK		10
//
//#define TI_MAX		TI_MARK
// #define TI_COUNT	11

#define GF_TI_X 11
#define GF_TI_X_ACTIVE 12
#define TI_MAX GF_TI_X_ACTIVE
#define TI_COUNT GF_TI_X

#define GF_TI_COUNT_X_ICONS 2
#define GF_X_ICON_SIZE 16
#define GF_X_ICON_TOP_PADDING 2


#define IS_TABICON(nIcon)	(nIcon<TI_COUNT)
#define IS_FAVICON(nIcon)	(nIcon>TI_MAX)

#define PIC_MASK RGB(255,0,255)

//
#define FI_NOT_IN_LIST	-1//not in fav list
#define FI_DOWNLOADING	-2//downloading
#define FI_NOT_EXIST	-3//not exist in cache and web site
//##############################

class CConsts  

{

public:
	const static int _iTitleShow;

	CConsts();

	virtual ~CConsts();



//	//status.bmp 中对应的tab 图标

//	const static enum TAB_ICON

//	{

//		TAB_ICON_LOADED = 0,

//		TAB_ICON_SAVED = 6,

//		TAB_ICON_LOADING =7

// 	};

	

	const static CString _saGoodSites;



	const static CString _saFullUrlBadSites;

	

	const static int _iAdressListMaxItems;
	const static int _iAdressListUIHight;

	const static CString _strSpaceBetweenTitleAndUrl;

};

 

#endif // !defined(AFX_CONSTS_H__F4D89EDD_AD60_4FB6_A27E_D15B2FFBC4C0__INCLUDED_)











































