// Gf_StringList.h: interface for the CGf_StringList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_STRINGLIST_H__E3353A91_8838_4FC8_B8E2_32CBD516C85B__INCLUDED_)
#define AFX_GF_STRINGLIST_H__E3353A91_8838_4FC8_B8E2_32CBD516C85B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGf_StringList : public CStringList
{
public:
	void Remove(LPCTSTR str);
	int FindInt(const CString& str);
	CString GetAt(int index);
	void Remove(int index);
	CGf_StringList();
	virtual ~CGf_StringList();

};

#endif // !defined(AFX_GF_STRINGLIST_H__E3353A91_8838_4FC8_B8E2_32CBD516C85B__INCLUDED_)
