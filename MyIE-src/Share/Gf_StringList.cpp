// Gf_StringList.cpp: implementation of the CGf_StringList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gf_StringList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGf_StringList::CGf_StringList()
{

}

CGf_StringList::~CGf_StringList()
{

}

CString CGf_StringList::GetAt(int index)
{
	POSITION pos = FindIndex(index);
	if (NULL == pos)
	{
		return "";
	}

	return CStringList::GetAt(pos);
}

void CGf_StringList::Remove(int index)
{
	POSITION pos = FindIndex(index);
	if (NULL == pos)
	{
		ASSERT(false);
		return;
	}

	CStringList::RemoveAt(pos);
}

int CGf_StringList::FindInt(const CString &str)
{
	POSITION posGot=Find(str);

	if (posGot==NULL)
	{
		return -1;
	}

	int i=0;
	for (POSITION pos=GetHeadPosition(); (pos!=NULL) && (pos!=posGot) ;)
	{
		GetNext(pos);
		i++;
	}

	return i;

}

void CGf_StringList::Remove(LPCTSTR str)
{	
	POSITION pos=Find(str);
	if (pos)
	{
		RemoveAt(pos);
	}
}
