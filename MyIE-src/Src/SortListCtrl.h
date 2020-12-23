#if !defined(AFX_SORTLISTCTRL_H__F342B9FC_D7E9_492D_908D_E16E0F51EFAB__INCLUDED_)
#define AFX_SORTLISTCTRL_H__F342B9FC_D7E9_492D_908D_E16E0F51EFAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SortListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl window

class CSortListCtrl : public CListCtrl
{
// Construction
public:
	CSortListCtrl();
	int FindIndex( DWORD dw );
	void AddSortIndexData();

// Attributes
protected:
	BOOL m_bLastSortAsc;
	int m_nLastSortCol;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSortListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSortListCtrl();

	// Generated message map functions
protected:
	static int CALLBACK CompareEntries( LPARAM lp1, LPARAM lp2, LPARAM lColSpec );
	//{{AFX_MSG(CSortListCtrl)
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SORTLISTCTRL_H__F342B9FC_D7E9_492D_908D_E16E0F51EFAB__INCLUDED_)
