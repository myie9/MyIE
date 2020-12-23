// AnimateIcon.h : interface of the CAnimateIcon class
// written by Rajesh Parikh (MCSD)
// email : rparikh@usa.net
// Not protected by any copyright, use it freely
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIMATEICON_H__47E058AD_6F69_11D2_B59C_86DD54033006__INCLUDED_)
#define      AFX_ANIMATEICON_H__47E058AD_6F69_11D2_B59C_86DD54033006__INCLUDED_

class CAnimateIcon
{
protected :
	// variable to hold the image list containing a series
	// of icons
	CImageList  m_imgList;
	// keep track of which image is current
	int m_iImageCounter;
	// store the max nos of images
	int m_iMaxNoOfImages;
	HICON hIcon;
	HICON hPrevIcon ;
		
public:
	BOOL ShowFirstImage();
	CAnimateIcon();
	~CAnimateIcon();
	CImageList* GetImageList();
	BOOL SetImageList(HBITMAP hbmp,int numberOfImages,COLORREF transparentColor);
	BOOL ShowNextImage();
private:
	//HICON m_hAppIcon;
};

#endif //#define AFX_ANIMATEICON_H__47E058AD_6F69_11D2_B59C_86DD54033006__INCLUDED_