#if !defined(AFX_ORGGROUPDLG_H__4D1B9EF3_D552_11D4_9B6A_0000E85300AE__INCLUDED_)
#define AFX_ORGGROUPDLG_H__4D1B9EF3_D552_11D4_9B6A_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OrgGroupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COrgGroupDlg dialog

class COrgGroupDlg : public CDialog
{
// Construction
public:
	BOOL m_bInit;
	CString m_strStartGroup;
	int m_nLastSelItemID;
	void SaveUrlList(int index);
	COrgGroupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COrgGroupDlg()
	{
		if(m_pDragImage)
			delete m_pDragImage;
	};

// Dialog Data
	//{{AFX_DATA(COrgGroupDlg)
	enum { IDD = IDD_ORG_GROUP };
	CButton	m_CloseAllBeforeNewGroup;
	CButton	m_GroupShowMember;
	CButton	m_btnInsert;
	CListCtrl	m_UrlList;
	CListCtrl	m_GroupList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrgGroupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COrgGroupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNewGroup();
	afx_msg void OnDelGroup();
	afx_msg void OnDelete();
	afx_msg void OnEndlabeleditGroupList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnInsert();
	afx_msg void OnUpdate();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickGroupList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragUrlList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDblclkUrlList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void GetUnqBlankName(CString &newblnk);
	void FillUrlList(int index);

	CImageList* m_pDragImage;
	BOOL		m_bDragging;
	int			m_nDragIndex, m_nDropIndex;
	CWnd*		m_pDropWnd;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORGGROUPDLG_H__4D1B9EF3_D552_11D4_9B6A_0000E85300AE__INCLUDED_)
