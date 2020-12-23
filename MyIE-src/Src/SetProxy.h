#if !defined(AFX_SETPROXY_H__CA53E441_10A9_11D4_9BB0_847715C6807B__INCLUDED_)
#define AFX_SETPROXY_H__CA53E441_10A9_11D4_9BB0_847715C6807B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SetProxy.h : header file
//

#include "ProxyList.h"

/////////////////////////////////////////////////////////////////////////////
// CSetProxy dialog

class CSetProxy : public CDialog
{
// Construction
public:
	void FillProxyList(int index);
	int m_nList;
	int m_nItem;
	CString m_SelProxy;
	int m_nSelProxyList;
	CString m_SelWebProxy;
	CSetProxy(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetProxy()
	{
		if(m_pDragImage)
			delete m_pDragImage;
	};
	CProxyCategory* m_tmpProxyCategory;

// Dialog Data
	//{{AFX_DATA(CSetProxy)
	enum { IDD = IDD_SET_PROXY };
	CEdit	m_ByPass;
	CListCtrl	m_CateList;
	CListCtrl	m_ProxyList;
	BOOL	m_bUseWebProxy;
	BOOL	m_bUseProxy;
//	CString	m_strByPass;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetProxy)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetProxy)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnProxyNew();
	afx_msg void OnProxyDel();
	afx_msg void OnEndlabeleditProxyList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnModifyproxy();
	afx_msg void OnItemchangedCates(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditCates(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNewcate();
	afx_msg void OnChangeBypass();
	afx_msg void OnDelcate();
	afx_msg void OnItemchangedProxyList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragProxyList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDblclkProxyList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CImageList* m_pDragImage;
	BOOL		m_bDragging;
	int			m_nDragIndex, m_nDropIndex;
	CWnd*		m_pDropWnd;

	BOOL m_bUnUpdateProxy;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETPROXY_H__CA53E441_10A9_11D4_9BB0_847715C6807B__INCLUDED_)
