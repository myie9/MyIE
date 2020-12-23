// SDocEvtHandler.cpp: implementation of the SDocEvtHandler class.
//
//////////////////////////////////////////////////////////////////////


// DocEvtHandler.cpp
// SDocEvtHandler ��Ϣ������ʵ�� by ��������

#include "stdafx.h"
#include "SDocEvtHandler.h"
#include "mshtmdid.h"

IMPLEMENT_DYNAMIC(SDocEvtHandler, CCmdTarget)

SDocEvtHandler::SDocEvtHandler()
{
  EnableAutomation();  // ��Ҫ������ IDispatch
}

SDocEvtHandler::~SDocEvtHandler()
{
}

BEGIN_MESSAGE_MAP(SDocEvtHandler, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(SDocEvtHandler, CCmdTarget)
  DISP_FUNCTION_ID(SDocEvtHandler,"HTMLELEMENTEVENTS2_ONCLICK",
  DISPID_HTMLELEMENTEVENTS2_ONCLICK, OnClick,
  VT_EMPTY, VTS_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(SDocEvtHandler, CCmdTarget)
  INTERFACE_PART(SDocEvtHandler,
  DIID_HTMLButtonElementEvents2, Dispatch)
END_INTERFACE_MAP()

void SDocEvtHandler::OnClick(MSHTML::IHTMLEventObjPtr pEvtObj)
{
  // ������������...����½�
	  MSHTML::IHTMLElementPtr pElement =
    pEvtObj->GetsrcElement(); // �¼������Ķ���Ԫ��
  while(pElement) // ������ϼ��
  {
    _bstr_t strTagname;
    pElement->get_tagName(&strTagname.GetBSTR());
    if(_bstr_t("a") == strTagname || _bstr_t("A") == strTagname)
    {
      // ���ҵ� "a" ��ǩ��������д��Ӧ����
      // ��1��ȡ��Ŀ���ַ��
      _variant_t vHref = pElement->getAttribute("href", 0);
      // ��2��ȡ���������ֹת��Ŀ��ҳ��
     // pEvtObj->put_returnValue(_variant_t(VARIANT_FALSE, VT_BOOL));
      break;
    }
    pElement = pElement->GetparentElement();
  }

}

