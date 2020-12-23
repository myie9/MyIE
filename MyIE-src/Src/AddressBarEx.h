#if !defined(AFX_ADDRESSBAREX_H__CC843194_A55D_11D4_9B47_0000E85300AE__INCLUDED_)
#define AFX_ADDRESSBAREX_H__CC843194_A55D_11D4_9B47_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddressBarEx.h : header file
//
#include <afxole.h>
#include "Gf_CComboBoxEx.h"

class CAddDropTarget : public COleDropTarget
{
public:
    CAddDropTarget()
	{
	}

    BOOL        OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
    DROPEFFECT  OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    void        OnDragLeave(CWnd* pWnd);
    DROPEFFECT  OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    DROPEFFECT  OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point);

private:
};

/////////////////////////////////////////////////////////////////////////////
// CAddressBarEx window

class CAddressBarEx : public CToolBar
{
// Construction
public:
	CAddressBarEx();

// Attributes
public:

private:
	CAddDropTarget	ms_dropTarget;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddressBarEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL ShowGoButton;
	int FindStringExact(int start, CString& str);
	void GetSelText(int index, CString& str);
	CComboBoxEx* GetAddressBox();
	CGf_CComboBoxEx m_wndAddress;
	virtual ~CAddressBarEx();
	void Init();
	void InitImage();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAddressBarEx)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDRESSBAREX_H__CC843194_A55D_11D4_9B47_0000E85300AE__INCLUDED_)
