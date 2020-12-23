// Gf_FillForm.cpp: implementation of the CGf_FillForm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyIE9.h"
#include "Gf_FillForm.h"
#include "macro.h"
#include "function.h"
#include "Gf_IniFile.h"
#include "Crypt.h"
#include "GFBusiness.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_FillForm::CGf_FillForm()
{
	
}

CGf_FillForm::~CGf_FillForm()
{
	
}

int CGf_FillForm::Execute(BOOL bEncryptStringItem, CComQIPtr<IHTMLDocument2> pDoc, const CString &baseIn, const CString &filenameIn)
{
	this->m_bEncryptStringItem=bEncryptStringItem;
	this->base=baseIn;
	this->filename=filenameIn;
	
	bAutoEnter=FALSE;
	BOOL r = ::GetPrivateProfileString(base, "FormAutoSubmit", _T(""), buf, 2048, filename);
	if (r && *buf=='1')
		bAutoEnter = TRUE;
	//
	if (PRESS_SHIFT)
		bAutoEnter = FALSE;
	else if (PRESS_CTRL)
		bAutoEnter = TRUE;
	
	
	
	
	bIsJinSiDomain = FALSE;
	
	long p = GetPrivateProfileSection(base, buf, 20, filename);
	if(p==0)
	{
		//base = _StringGetRootUrl(base);
		base = _GetSameFormUrl(base);
		p = GetPrivateProfileSection(base, buf, 20, filename);
		if (p==0)
		{
			base = _GetSameFormUrl(base, TRUE);
			p = GetPrivateProfileSection(base, buf, 20, filename);	
			if (p==0)
			{
				base = GENERAL_SECTION;
				p = GetPrivateProfileSection(base, buf, 20, filename);
				if(p==0)
					return 0;
				else
				{
					bIsJinSiDomain=TRUE;
				}
			}
		}
	}
		  
		  
	int nRet=this->TravelElements(pDoc);

	if (bAutoEnter && pForm)
	{
	  pForm->submit();
  
	}

	if (GENERAL_SECTION != base)
	{
		CString strLastTime;
		strLastTime.Format("%ld", CTime::GetCurrentTime().GetTime() );
		::WritePrivateProfileString(base, MYIE9_FORM_LAST_TIME, strLastTime, filename);
	}
	

	return nRet;
}

void CGf_FillForm::DealOneElement(CComQIPtr<IHTMLElement>& pElem)
{
	
	CComQIPtr<IHTMLInputTextElement> pInput;
	CComQIPtr<IHTMLTextAreaElement> pTextArea;
	CComQIPtr<IHTMLOptionButtonElement> pOption;
	CComQIPtr<IHTMLSelectElement> pSelect;
	
	long nSelectedIndex;
	int nRadioIndex = 0;//radio index
	
	CString strNameOld;
	strNameOld.Empty();
	
	CString strName="",strValue="",strType="";
	CComBSTR bstrName,bstrID, bstrValue,bstrType;
	
//	
//	strName.Empty();
//	strValue.Empty();
//	strType.Empty();

	
	if(pInput=pElem) //SUCCEEDED(pElem->QueryInterface(&pInput)))
	{
		pInput->get_name(&bstrName);
		strName = bstrName;
	
		if (strName.IsEmpty())
		{
			CComQIPtr<IHTMLElement> pElemForId=pInput;

			if (pElemForId)
			{
				
				pElemForId->get_id(&bstrID);
				strName=bstrID;
			}
		}
		
		
		//gf if (::GetPrivateProfileString(base, strName, _T(""), buf, 2048, filename)>0)
		if (CGf_IniFile::GetPrivateProfileStringJinSi(bIsJinSiDomain, base, strName, _T(""), buf, 2048, filename)>0)
		{
			BOOL bIsGfEnc = CGf_IniFile::IsEncValue(base, strName,filename);
			
			if (bIsGfEnc)
			{
				CGf_IniFile::Decode(strValue, base, strName,filename); //out to strValue
			}
			else if (m_bEncryptStringItem)
				Decoding(buf, strValue);
			else
				strValue = buf;
			
			bstrValue = strValue;//.AllocSysString();
			
			//gf
			{
				CComBSTR cbstrValue;
				pInput->get_value(&cbstrValue);
				CString strFieldValue = cbstrValue;
				
				if(CGFBusiness::IsNeedFill(strName, strFieldValue, strValue ))
				{
					pInput->put_value(bstrValue);
				}
			}
			//gf pInput->put_value(bstrValue);
			inputs++;
			//
			if (bAutoEnter && pForm==NULL) pInput->get_form(&pForm);
		}

	}
	else if(pTextArea=pElem) //SUCCEEDED(pElem->QueryInterface(&pTextArea)))
	{
		pTextArea->get_name(&bstrName);
		strName = bstrName;

		
		if (strName.IsEmpty())
		{
			CComQIPtr<IHTMLElement> pElemForId=pTextArea;

			if (pElemForId)
			{
				
				pElemForId->get_id(&bstrID);
				strName=bstrID;
			}
		}
		


		if (CGf_IniFile::GetPrivateProfileStringJinSi(bIsJinSiDomain, base, strName, _T(""), buf, 2048, filename)>0)
		{
			
			BOOL bIsGfEnc = CGf_IniFile::IsEncValue(base, strName,filename);
			
			if (bIsGfEnc)
			{
				CGf_IniFile::Decode(strValue, base, strName,filename); //out to strValue
			}
			else if (m_bEncryptStringItem)
				Decoding(buf, strValue);
			else
				strValue = buf;
			
			strValue.Replace("\\r","\r");
			strValue.Replace("\\n","\n");
			bstrValue = strValue;//.AllocSysString();
			
			//gf
			{
				CComBSTR cbstrValue;
				pTextArea->get_value(&cbstrValue);
				CString strFieldValue = cbstrValue;
				
				if(CGFBusiness::IsNeedFill(strName, strFieldValue, strValue ))
				{
					pTextArea->put_value(bstrValue);
				}
			}
			//gf pTextArea->put_value(bstrValue);
			inputs++;
			//
			if (bAutoEnter && pForm==NULL) pTextArea->get_form(&pForm);
		}

	}
	else if(pOption=pElem) //SUCCEEDED(pElem->QueryInterface(&pOption)))
	{
		pOption->get_name(&bstrName);
		strName = bstrName;
		if (::GetPrivateProfileString(base, strName, _T(""), buf, 2048, filename)>0)
		{
			int bRet = atoi(buf);
			//
			pOption->get_type(&bstrType);
			strType = bstrType ;			
			
			//
			if (strType=="radio")
			{	
				if (strName!=strNameOld)
				{
					nRadioIndex = 0;
					strNameOld = strName;
				}
				else
				{
					nRadioIndex++;
				}
				//
				if (bRet>=0 && nRadioIndex==bRet)
				{
					pOption->put_checked(1);
					inputs++;
				}
			}
			else
			{
				pOption->put_checked(bRet);
				inputs++;
			}
			//
			if (bAutoEnter && pForm==NULL) pOption->get_form(&pForm);
		}

	}
	else if(pSelect=pElem) //SUCCEEDED(pElem->QueryInterface(&pSelect)))
	{
		pSelect->get_name(&bstrName);
		strName = bstrName;
		if (::GetPrivateProfileString(base, strName, _T(""), buf, 2048, filename)>0)
		{
			nSelectedIndex = atol(buf);
			pSelect->put_selectedIndex(nSelectedIndex);
			inputs++;
			//
			if (bAutoEnter && pForm==NULL) pSelect->get_form(&pForm);
		}
								
	}
	else
		goto _Release;
	//
	//inputs++;
	//
_Release:
	;

}
