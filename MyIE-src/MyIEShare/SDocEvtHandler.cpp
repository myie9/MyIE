// SDocEvtHandler.cpp: implementation of the SDocEvtHandler class.
//
//////////////////////////////////////////////////////////////////////


// DocEvtHandler.cpp
// SDocEvtHandler 消息处理类实现 by 旧日重来

#include "stdafx.h"
#include "SDocEvtHandler.h"
#include "mshtmdid.h"

IMPLEMENT_DYNAMIC(SDocEvtHandler, CCmdTarget)

SDocEvtHandler::SDocEvtHandler()
{
  EnableAutomation();  // 重要：激活 IDispatch
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
  // 鼠标点击处理代码...详见下节
	  MSHTML::IHTMLElementPtr pElement =
    pEvtObj->GetsrcElement(); // 事件发生的对象元素
  while(pElement) // 逐层向上检查
  {
    _bstr_t strTagname;
    pElement->get_tagName(&strTagname.GetBSTR());
    if(_bstr_t("a") == strTagname || _bstr_t("A") == strTagname)
    {
      // 已找到 "a" 标签，在这里写相应代码
      // 例1：取得目标地址：
      _variant_t vHref = pElement->getAttribute("href", 0);
      // 例2：取消点击，禁止转到目标页面
     // pEvtObj->put_returnValue(_variant_t(VARIANT_FALSE, VT_BOOL));
      break;
    }
    pElement = pElement->GetparentElement();
  }

}

