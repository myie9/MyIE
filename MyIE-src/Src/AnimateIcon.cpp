// AnimateIcon.cpp : implementation of the CAnimateIcon class
// written by Rajesh Parikh (MCSD)
// email : rparikh@usa.net
// Not protected by any copyright, use it freely
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AnimateIcon.h"
#include "resource.h"

// default constructor
CAnimateIcon::CAnimateIcon()
{
	m_iImageCounter = -1;
	m_iMaxNoOfImages = -99;
	m_imgList.m_hImageList = NULL;
}

// default do nothing destructor
CAnimateIcon::~CAnimateIcon()
{
	if (hPrevIcon) 
		DestroyIcon(hPrevIcon);
}

// This is the first function which needs to be called in order
// to fill the image list
// Parameters :
// ------------
// int IDOfImgListResource - pass the Resource ID of a toolbar resource
//                           containing the image list
// int numberOfImages      - Number of images (16x16) in the toolbar resource
// transparentColor        - RGB value of color you want to be transparent
BOOL CAnimateIcon::SetImageList(HBITMAP hbmp,int numberOfImages,COLORREF transparentColor)
{
	if(numberOfImages <= 0)
		return FALSE;
	m_iMaxNoOfImages = numberOfImages;
	m_imgList.Create(16, 16, ILC_COLORDDB|ILC_MASK, 1, 1);
	ImageList_AddMasked(m_imgList.GetSafeHandle(), hbmp, transparentColor);
	DeleteObject(hbmp);
	return TRUE;
}

BOOL CAnimateIcon::ShowNextImage()
{
	if(m_imgList.m_hImageList == NULL)
		return FALSE;
	m_iImageCounter++;
	if(m_iImageCounter >= m_iMaxNoOfImages)
		m_iImageCounter =0;

	try{
	// extract the icon from imagelist
	hIcon = m_imgList.ExtractIcon(m_iImageCounter);
	// send the message to frame to update icon
	HICON hPrevIcon = (HICON) 	AfxGetMainWnd()->SendMessage(WM_SETICON,TRUE,(LPARAM)hIcon);	
	// Free the previous icon resource
	if (hPrevIcon) 	
	    DestroyIcon(hPrevIcon);
	}catch(...){}

	return TRUE;
}

BOOL CAnimateIcon::ShowFirstImage()
{
	if(m_imgList.m_hImageList == NULL)
		return FALSE;
	m_iImageCounter=0;

	try{
	// extract the icon from imagelist
	hIcon = m_imgList.ExtractIcon(m_iImageCounter);
	// send the message to frame to update icon
	HICON hPrevIcon = (HICON) 	AfxGetMainWnd()->SendMessage(WM_SETICON,TRUE,(LPARAM)hIcon);	
	// Free the previous icon resource
	if (hPrevIcon) 	
	    DestroyIcon(hPrevIcon);
	}catch(...){}

	return TRUE;
}

CImageList* CAnimateIcon::GetImageList()
{
	return &m_imgList;
}
