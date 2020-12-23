// Gf_AbstractHtml.h: interface for the CGf_AbstractTravelHtml class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_ABSTRACTHTML_H__0167C18A_94E0_44FA_8734_65F42F536706__INCLUDED_)
#define AFX_GF_ABSTRACTHTML_H__0167C18A_94E0_44FA_8734_65F42F536706__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mshtml.h>

class CGf_AbstractTravelHtml  
{
public:
	CGf_AbstractTravelHtml();
	virtual ~CGf_AbstractTravelHtml()=0;
protected:
	int TravelElements(CComQIPtr<IHTMLDocument2> pDoc);

	int inputs;
	virtual void DealOneElement(CComQIPtr<IHTMLElement>& pElem)=0;
	
private:
	int nFrames;
	void RecurseFrames(CComQIPtr<IHTMLDocument2> pDocIn);
	void DealOneDoc(CComQIPtr<IHTMLDocument2> pDoc);
	
	

};

#endif // !defined(AFX_GF_ABSTRACTHTML_H__0167C18A_94E0_44FA_8734_65F42F536706__INCLUDED_)
