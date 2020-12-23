// HelperFile.h: interface for the CHelperFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELPERFILE_H__B141B750_4901_46B5_9EE9_FD2925D769A2__INCLUDED_)
#define AFX_HELPERFILE_H__B141B750_4901_46B5_9EE9_FD2925D769A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHelperFile  
{
public:
	static CString Read(const CString& path);

	static void Write(const CString &path, const CString & all);	
	static void Write(const CString& path, const CString& section, const CString& key, const CStringList& slIn);
	static void Write(const CString& path, const CStringList& slIn);
	static void AddToStrList(const CString& path, CStringList& slIn);
	static void AddToStrList(const CString& path, const CString& section, const CString& key, CStringList& slIn, BOOL bUnique=TRUE);
	static void AddToStrArray(const CString& path, CStringArray& slIn);

	static void SaveMapKeyToFile(const CMapStringToString &maps, const CString &strFile);
	static void AppendLine(const CString &strFile, const CString strLine);
	CHelperFile();
	virtual ~CHelperFile();

};

#endif // !defined(AFX_HELPERFILE_H__B141B750_4901_46B5_9EE9_FD2925D769A2__INCLUDED_)

