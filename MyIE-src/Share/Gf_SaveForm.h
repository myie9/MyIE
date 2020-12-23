// Gf_SaveForm.h: interface for the CGf_SaveForm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_SAVEFORM_H__6C76C3F6_1A88_4324_B349_5D7D4A08D368__INCLUDED_)
#define AFX_GF_SAVEFORM_H__6C76C3F6_1A88_4324_B349_5D7D4A08D368__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Gf_AbstractHtml.h"

class CGf_SaveForm : public CGf_AbstractTravelHtml  
{
public:
	CGf_SaveForm();
	virtual ~CGf_SaveForm();

	int Execute(CComQIPtr<IHTMLDocument2> pDoc, const CString& base, const CString& filename );

protected:
	virtual void DealOneElement(CComQIPtr<IHTMLElement>& pElem);
	
private:
	CString base;
	CString filename;
	
	BOOL bSelectMenuSaved;
	BOOL bOptionMenuSaved;
};

#endif // !defined(AFX_GF_SAVEFORM_H__6C76C3F6_1A88_4324_B349_5D7D4A08D368__INCLUDED_)
