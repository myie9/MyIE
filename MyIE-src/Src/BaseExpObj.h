// BaseExpObj.h: interface for the CBaseExpObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEEXPOBJ_H__718537D3_14E5_11D4_9BCE_0000E85300AE__INCLUDED_)
#define AFX_BASEEXPOBJ_H__718537D3_14E5_11D4_9BCE_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyIE9Helper.h"

//the base object of explore object.
class CBaseExpObj  
{
public:
	virtual BOOL Update(int nLevel) =0;
	virtual void Destroy() =0;
	virtual BOOL Initialize() =0;
	virtual BOOL Create(CWnd* pParent) =0;
	virtual CWnd* GetWindow() =0;
	virtual LPCSTR GetTitle() = 0;
	CBaseExpObj();
	virtual ~CBaseExpObj();

};

#endif // !defined(AFX_BASEEXPOBJ_H__718537D3_14E5_11D4_9BCE_0000E85300AE__INCLUDED_)
