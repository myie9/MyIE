// SimpleSAH.h: interface for the CSimpleSAH class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMPLESAH_H__027A1634_1C18_11D5_9B89_0000E85300AE__INCLUDED_)
#define AFX_SIMPLESAH_H__027A1634_1C18_11D5_9B89_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSimpleSAH  
{
public:
	BOOL AddText(CString& text);
	void Close();
	CStdioFile file;
	BOOL LoadHTML(LPCSTR filename, BOOL bUpdate=TRUE);
	BOOL SetAttribute(LPCSTR lpAttriName, LPCSTR lpNewVal);
	CString GetAttribute(LPCSTR lpAttriName);
	BOOL GetNextTags(CStringList& astrTagNames);
	CSimpleSAH();
	virtual ~CSimpleSAH();
	CString strCurTagBuf;


protected:
	CString strHTML;
	long nStartPoint;
	BOOL m_bUpdate;
};

#endif // !defined(AFX_SIMPLESAH_H__027A1634_1C18_11D5_9B89_0000E85300AE__INCLUDED_)
