// SDocEvtHandler.h: interface for the SDocEvtHandler class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#import <mshtml.tlb>

class SDocEvtHandler : public CCmdTarget
{
  DECLARE_DYNAMIC(SDocEvtHandler)
public:
  SDocEvtHandler();
  virtual ~SDocEvtHandler();

  // 消息处理函数
  void OnClick(MSHTML::IHTMLEventObjPtr pEvtObj);

  DECLARE_MESSAGE_MAP()
  DECLARE_DISPATCH_MAP()
  DECLARE_INTERFACE_MAP()
};
