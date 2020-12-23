// MyIE9Helper.h: interface for the CMyIE9Helper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MyIE9HELPER_H__74460E23_1598_11D4_9BCE_0000E85300AE__INCLUDED_)
#define AFX_MyIE9HELPER_H__74460E23_1598_11D4_9BCE_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyIE9View.h"

CMyIE9View* GetCurrentView(int nFlag = 0, BOOL bInNewWindow = FALSE);
CMyIE9View * CreateNewView();

#endif // !defined(AFX_MyIE9HELPER_H__74460E23_1598_11D4_9BCE_0000E85300AE__INCLUDED_)
