#if !defined(AFX_GF_CCOMBOBOXEX_H__7F2CCF4C_9CCE_4A8E_97EB_F5C76F567B5A__INCLUDED_)
#define AFX_GF_CCOMBOBOXEX_H__7F2CCF4C_9CCE_4A8E_97EB_F5C76F567B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// gf_ccomboboxex.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGf_CComboBoxEx window

class CGf_CComboBoxEx : public CComboBoxEx
{
// Construction
public:
	CGf_CComboBoxEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGf_CComboBoxEx)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGf_CComboBoxEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGf_CComboBoxEx)
	afx_msg void OnReditPasteEnter();
	afx_msg void OnReditDelete();
	afx_msg void OnReditCopy();
	afx_msg void OnReditUndo();
	afx_msg void OnUpdateReditUndo(CCmdUI* pCmdUI);
	afx_msg void OnReditPaste();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	BOOL IsEmpty();
	void SelAllWhenEmpty();
	void Enter();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GF_CCOMBOBOXEX_H__7F2CCF4C_9CCE_4A8E_97EB_F5C76F567B5A__INCLUDED_)
