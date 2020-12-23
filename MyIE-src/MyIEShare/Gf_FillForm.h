// Gf_FillForm.h: interface for the CGf_FillForm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_FILLFORM_H__A6C42166_EFE4_4012_A9D2_0E9C389E7D04__INCLUDED_)
#define AFX_GF_FILLFORM_H__A6C42166_EFE4_4012_A9D2_0E9C389E7D04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Gf_AbstractHtml.h"

class CGf_FillForm : public CGf_AbstractTravelHtml  
{
public:
	int Execute(BOOL bEncryptStringItem, CComQIPtr<IHTMLDocument2> pDoc, const CString& base, const CString& filename );
	CGf_FillForm();
	virtual ~CGf_FillForm();

protected:
	virtual void DealOneElement(CComQIPtr<IHTMLElement>& pElem);

private:
	BOOL m_bEncryptStringItem;
	CString base;
	CString filename;

	char buf[2048];

	CComQIPtr<IHTMLFormElement> pForm;
	BOOL bAutoEnter;
	BOOL bIsJinSiDomain;
};

#endif // !defined(AFX_GF_FILLFORM_H__A6C42166_EFE4_4012_A9D2_0E9C389E7D04__INCLUDED_)
