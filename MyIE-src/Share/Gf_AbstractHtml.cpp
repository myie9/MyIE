// Gf_AbstractHtml.cpp: implementation of the CGf_AbstractTravelHtml class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyIE9.h"
#include "Gf_AbstractHtml.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_AbstractTravelHtml::CGf_AbstractTravelHtml()
{

	inputs=0;
	nFrames=0;
}

CGf_AbstractTravelHtml::~CGf_AbstractTravelHtml()
{

}

int CGf_AbstractTravelHtml::TravelElements(CComQIPtr<IHTMLDocument2> pDoc)
{
	if (pDoc==NULL)
	{
		return 0;
	}

	nFrames++;

	GF_TRY
	{
		RecurseFrames(pDoc);
	}
	GF_CATCH

//	GF_TRY
	{
		DealOneDoc(pDoc);
	}
//	GF_CATCH
	
	return inputs;

}

void CGf_AbstractTravelHtml::RecurseFrames(CComQIPtr<IHTMLDocument2> pDocIn)
{
	if (nFrames>150)
	{
		return;
	}

	long p=0;
	CComQIPtr<IHTMLFramesCollection2> frames;
	pDocIn->get_frames(&frames);
	if (!frames)
	{
		return;

	}

	frames->get_length(&p);
	if (p<=0)
	{
		return;
	}

	for (int i=0;i<p;i++)
	{
		CComVariant name;
		name.vt=VT_I4;
		name.lVal=i;

		VARIANT frameOut;
		if (frames->item(&name, &frameOut) == S_OK)
		{
			CComQIPtr<IHTMLWindow2> frame=frameOut.pdispVal;
			if (frame!=NULL)
			{
				CComQIPtr<IHTMLDocument2> pDoc;
				frame->get_document(&pDoc);
				if (pDoc)
				{
					TravelElements(pDoc);
				}
			}
		}
	}
}

void CGf_AbstractTravelHtml::DealOneDoc(CComQIPtr<IHTMLDocument2> pDoc)
{

	CComQIPtr<IHTMLElementCollection> pAllElem;
	pDoc->get_all(&pAllElem);

	if (pAllElem==NULL)
	{
		return;
	}

	long p=0;
	pAllElem->get_length(&p);
	if (p<=0)
	{
		return;
	}

	for (int i=0;i<p;i++)
	{
		CComQIPtr<IHTMLElement> pElem;

		CComVariant name;
		name.vt=VT_I4;
		name.lVal=i;

		if (pAllElem->item(name,name,(LPDISPATCH*)&pElem) == S_OK)
		{
			if (pElem)
			{
				DealOneElement(pElem);
			}
		}

	}
}
	
