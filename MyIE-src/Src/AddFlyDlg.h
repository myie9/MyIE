// AddFlyDlg.h: interface for the CAddFlyDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADDFLYDLG_H__B8516442_EE81_4C99_895D_1E6367A2909C__INCLUDED_)
#define AFX_ADDFLYDLG_H__B8516442_EE81_4C99_895D_1E6367A2909C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAddFlyDlg : public CDialog
{
// Construction
public:
	CAddFlyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddFlyDlg)
	enum { IDD = IDD_ADD_FLY };
	CString	m_strUrl;
	BOOL	m_bFilterCurrent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddFlyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddFlyDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_ADDFLYDLG_H__B8516442_EE81_4C99_895D_1E6367A2909C__INCLUDED_)
