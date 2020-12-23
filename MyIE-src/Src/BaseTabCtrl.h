#if !defined(AFX_BASETABCTRL_H__1E6E4FE9_BE01_4DA1_AFA9_A98527A3769B__INCLUDED_)
#define AFX_BASETABCTRL_H__1E6E4FE9_BE01_4DA1_AFA9_A98527A3769B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaseTabCtrl window

class CBaseTabCtrl : public CTabCtrl
{
// Construction
public:
	enum{
		FLAT_TAB = 1,
		SKIN_TAB = 3
		};

	CBaseTabCtrl();
	void SetTabStyle(int nType);
	void SetTabPos(int bTopTab); 
	void SetUnSelColor(COLORREF color);
	void SetSelColor(COLORREF color);
	void SetSelFontColor(COLORREF crSelFontColor);
	void SetUnSelFontColor(COLORREF crUnSelFontColor);
	void SetSepColor(COLORREF crSepColor);

protected:
	COLORREF m_crUnSelColor;
	COLORREF m_crSelColor; //gf
	COLORREF m_crUnSelFontColor;
	COLORREF m_crSelFontColor;
	COLORREF m_crSepColor;

	int m_nTabStyle;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBaseTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBaseTabCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawMainBorder(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawItemBorder(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual COLORREF GetTabColor(BOOL bSelected = FALSE);
	virtual COLORREF GetTabTextColor(BOOL bSelected = FALSE);
	virtual void PreSubclassWindow();

	inline BOOL IsSkin() 
	{
//		BOOL bExistTabImg = pmf->hbTabBkBottomActive && pmf->hbTabBkBottom && pmf->hbTabBkTopActive && pmf->hbTabBkTop;

	//	return (m_nTabStyle == SKIN_TAB) && bExistTabImg ;
			return (m_nTabStyle == SKIN_TAB);
	};

//	inline BOOL IsFlat() 
//	{
//		BOOL bExistTabImg = pmf->hbTabBkBottomActive && pmf->hbTabBkBottom && pmf->hbTabBkTopActive && pmf->hbTabBkTop;
//
//		return (m_nTabStyle == 1) || (!bExistTabImg && (m_nTabStyle == SKIN_TAB) );
// 	};
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASETABCTRL_H__1E6E4FE9_BE01_4DA1_AFA9_A98527A3769B__INCLUDED_)
