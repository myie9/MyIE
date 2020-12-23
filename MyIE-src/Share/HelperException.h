// HelperException.h: interface for the CHelperException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELPEREXCEPTION_H__AFD8D8AB_E63A_4239_9B8C_305AA03EA041__INCLUDED_)
#define AFX_HELPEREXCEPTION_H__AFD8D8AB_E63A_4239_9B8C_305AA03EA041__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHelperException  
{
public:
	CHelperException();
	virtual ~CHelperException();

static	void ErrorOcurred(const CString& strMsg = "");

};

#endif // !defined(AFX_HELPEREXCEPTION_H__AFD8D8AB_E63A_4239_9B8C_305AA03EA041__INCLUDED_)
