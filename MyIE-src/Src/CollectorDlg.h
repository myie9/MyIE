#if !defined(AFX_COLLECTORDLG_H__1D7D80A0_279E_44C1_A994_2D26BD2E42BC__INCLUDED_)
#define AFX_COLLECTORDLG_H__1D7D80A0_279E_44C1_A994_2D26BD2E42BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CollectorDlg.h : header file
//
#include "TxtDropTarget.h"

/////////////////////////////////////////////////////////////////////////////
// CCollectorDlg dialog

class CCollectorDlg : public CDialog
{
// Construction
public:
	void CleanOutputCollector();
	void OnOpenCursorLine();
	CEdit	*m_edit;//current edit
	void OnCollectorTxt();
	void OnCollectorOutput();
	void OnCollectorScript();
	void CollectorSetTrans(int mode);
	int  CollectorTrans(int mode);
	void OnCollectorClean();
	void OnCollectorSave();
	void OnCollectorMode();
	void OnRunScript();
	void OnOpenSelectedText();
	CCollectorDlg(CWnd* pParent = NULL);   // standard constructor
	CTxtDropTarget	ms_dropTarget;
	int		m_nOldMode;
// Dialog Data
	//{{AFX_DATA(CCollectorDlg)
	enum { IDD = IDD_COLLECTOR };
	CEdit	m_txt;
	CEdit	m_output;
	CEdit	m_script;
	//}}AFX_DATA
	CToolBar m_ToolBar;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCollectorDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:
	BOOL _bInited;

	// Generated message map functions
	//{{AFX_MSG(CCollectorDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SaveCollector();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLLECTORDLG_H__1D7D80A0_279E_44C1_A994_2D26BD2E42BC__INCLUDED_)
