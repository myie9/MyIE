// Gf_SaveForm.cpp: implementation of the CGf_SaveForm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyIE9.h"
#include "Gf_SaveForm.h"


#include "macro.h"
#include "function.h"
#include "Gf_IniFile.h"

#include "GFBusiness.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_SaveForm::CGf_SaveForm()
{
	
}

CGf_SaveForm::~CGf_SaveForm()
{
	
}


int CGf_SaveForm::Execute( CComQIPtr<IHTMLDocument2> pDoc, const CString &baseIn, const CString &filenameIn)
{
	this->base=baseIn;
	this->filename=filenameIn;
	
	
	
	bSelectMenuSaved = FALSE;
	bOptionMenuSaved = FALSE;
		  
	int nRet=this->TravelElements(pDoc);

	
	if (GENERAL_SECTION != base)
	{
		CString strLastTime;
		strLastTime.Format("%ld", CTime::GetCurrentTime().GetTime() );
		::WritePrivateProfileString(base, MYIE9_FORM_LAST_TIME, strLastTime, filename);
	}
	
	
	return nRet;
}

void CGf_SaveForm::DealOneElement(CComQIPtr<IHTMLElement>& pElem)
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
	CComBSTR bstrName,bstrID,bstrValue,bstrType;
	VARIANT_BOOL bRet;
	
	
	BOOL bNeedEnc = FALSE;
	
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

		pInput->get_value(&bstrValue);
		strValue = bstrValue;
		
		bNeedEnc = TRUE;									
		
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

		pTextArea->get_value(&bstrValue);
		strValue = bstrValue;
		strValue.Replace("\r","\\r");
		strValue.Replace("\n","\\n");
		
		bNeedEnc = TRUE;
		
								
	}
	else  if(pOption=pElem) //SUCCEEDED(pElem->QueryInterface(&pOption)))
	{
		pOption->get_name(&bstrName);
		strName = bstrName ;
		
		pOption->get_checked(&bRet);
		strValue = ( bRet ? "1" : "0" );
		//
		pOption->get_type(&bstrType);
		strType = bstrType ;
								
		
		//防止后面的同名字段覆盖前面的。用户真正输入的是前面的
		if (bOptionMenuSaved)
		{
			strName="";
			strValue="";
		}
		
		if (!strName.IsEmpty() && !strValue.IsEmpty())
		{
			bOptionMenuSaved = TRUE;
		}
	}
	else  if(pSelect=pElem) //SUCCEEDED(pElem->QueryInterface(&pSelect)))
	{
		pSelect->get_name(&bstrName);
		strName = bstrName ;
		pSelect->get_selectedIndex(&nSelectedIndex);
		strValue.Format("%d",nSelectedIndex);
		
		//防止后面的同名字段覆盖前面的。用户真正输入的是前面的
		if (bSelectMenuSaved)
		{
			strName="";
			strValue="";
		}
		
		if (!strName.IsEmpty() && !strValue.IsEmpty())
		{
			bSelectMenuSaved = TRUE;
		}
	}
	else
		goto _Release;
	//do special for radio
	if (strType=="radio")
	{	
		if (strName!=strNameOld)
		{
			nRadioIndex = 0;
			strNameOld = strName;
			if (strValue=="0")
				::WritePrivateProfileString(base, strName, "-1", filename);
			//CGf_IniFile::EncAndWriteString(base, strName, "-1", filename);
			else
				::WritePrivateProfileString(base, strName, "0", filename);
			//CGf_IniFile::EncAndWriteString(base, strName, "0", filename);
			
			
			inputs++;
		}
		else
		{
			nRadioIndex++;
			if (strValue=="1")
			{
				char strCount[20];
				itoa(nRadioIndex,strCount,10);
				::WritePrivateProfileString(base, strName, strCount, filename);
				//gf CGf_IniFile::EncAndWriteString(base, strName, strCount, filename);
			}
		}
	}
	else if (strName.GetLength() && strValue.GetLength())
	{								
		
		if (bNeedEnc)
		{
			CGf_IniFile::EncAndWriteString(base, strName, strValue, filename);
		}
		else
		{
			::WritePrivateProfileString(base, strName, strValue, filename);
		}
		
		bNeedEnc = FALSE;
		
		
		inputs++;
	}
_Release:
	;
	
				
	
}
