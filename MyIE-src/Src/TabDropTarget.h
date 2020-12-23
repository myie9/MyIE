#if !defined(AFX_TABDROPTARGET_H__93A5A961_EBE6_11D4_9BB1_E3251D198CBF__INCLUDED_)
#define AFX_TABDROPTARGET_H__93A5A961_EBE6_11D4_9BB1_E3251D198CBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabDropTarget.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabDropTarget document

#include <afxole.h>

class CTabDropTarget : public COleDropTarget
{
public:
    CTabDropTarget()
    {
		ms_pCtl=NULL;
    }

    BOOL        Register(CTabCtrl* pCtrl);
    BOOL        OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
    DROPEFFECT  OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    void        OnDragLeave(CWnd* pWnd);
    DROPEFFECT  OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    DROPEFFECT  OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point);

private:
    CTabCtrl*  ms_pCtl;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABDROPTARGET_H__93A5A961_EBE6_11D4_9BB1_E3251D198CBF__INCLUDED_)
