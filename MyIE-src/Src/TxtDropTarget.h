#if !defined(AFX_TXTDROPTARGET_H__1D7D80A0_279E_44C1_A994_2D26BD2E42BC__INCLUDED_)
#define AFX_TXTDROPTARGET_H__1D7D80A0_279E_44C1_A994_2D26BD2E42BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TxtDropTarget.h : header file
//


#include <afxole.h>

class CTxtDropTarget : public COleDropTarget
{
public:
    CTxtDropTarget()
    {
    }

    BOOL        OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
    DROPEFFECT  OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    void        OnDragLeave(CWnd* pWnd);
    DROPEFFECT  OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    DROPEFFECT  OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point);
	void		SaveText(CString str, int type);
private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TXTDROPTARGET_H__1D7D80A0_279E_44C1_A994_2D26BD2E42BC__INCLUDED_)
