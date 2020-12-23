// ExternalUtilItem.h: interface for the CExternalUtilItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXTERNALUTILITEM_H__E3F56C43_80F9_40FF_ADE3_8CAACEB3B42E__INCLUDED_)
#define AFX_EXTERNALUTILITEM_H__E3F56C43_80F9_40FF_ADE3_8CAACEB3B42E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CExternalUtilItem  
{
public:
	CExternalUtilItem();
	virtual ~CExternalUtilItem();

	CString m_strUtilCmd;
	CString m_strUtilName;
	CString m_strUtilPara;
	CString m_strUtilPath;
	BOOL m_bUtilStart, m_bUtilClose, m_bUtilRun;
	DWORD m_iProcess;
};


class CExternalUtilList  
{
public:
	void Clone(CExternalUtilList* peul);
	void RemoveAll();
	CExternalUtilList();
	virtual ~CExternalUtilList();
	CArray<CExternalUtilItem*, CExternalUtilItem*> m_UtilList;
};

#endif // !defined(AFX_EXTERNALUTILITEM_H__E3F56C43_80F9_40FF_ADE3_8CAACEB3B42E__INCLUDED_)
