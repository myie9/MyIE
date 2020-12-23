// Gf_PageSideBar.h: interface for the CGf_PageSideBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_PAGESIDEBAR_H__2FC1A88C_231E_436E_88F4_DED415FFDC86__INCLUDED_)
#define AFX_GF_PAGESIDEBAR_H__2FC1A88C_231E_436E_88F4_DED415FFDC86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HtmlWnd.h"
#include "Gf_StringList.h"

class CGf_PageSideBar : public CWnd
{
public:

	void ToNavigate(LPCTSTR lpszURL, DWORD dwFlags = 0,		LPCTSTR lpszTargetFrameName = NULL,		LPCTSTR lpszHeaders = NULL,		LPVOID lpvPostData = NULL,		DWORD dwPostDataLen = 0		);

	CGf_PageSideBar();
	virtual ~CGf_PageSideBar();
	
	CToolBar m_wndRef;
	CComboBox m_fType;
	CHtmlWnd _page;
	
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CShellWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRefresh();
	//}}AFX_MSG
	afx_msg void OnComboSelChange();
	DECLARE_MESSAGE_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	CString GetFirstAddress();
	CString _filepath;
	CGf_StringList _lstPageSiderBarAddress;
	CGf_StringList _lstNames;
	BOOL DeleteCurSel();
	void Populate();
	BOOL _bDirt;
	void SaveAdressList();
	void InitAddressList();
	
	void PadNames();
	CFont m_typeFont;
};

#endif // !defined(AFX_GF_PAGESIDEBAR_H__2FC1A88C_231E_436E_88F4_DED415FFDC86__INCLUDED_)
