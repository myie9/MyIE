// Tested.h: interface for the Tested class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTED_H__07C28D93_D163_43C5_96F3_7482DD99DE59__INCLUDED_)
#define AFX_TESTED_H__07C28D93_D163_43C5_96F3_7482DD99DE59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Tested  
{
public:
	static void TestSiteFilter();
	static void ChuliBadSites(CString strFromRawFile, CString strNotBadSitesFile, CString strToOKFile, BOOL bIsText = TRUE);
	static void ChuliNotBadSites(CString strFromRawFile, CString strToOKFile, BOOL bIsText = TRUE);
	static void TestGetURL();
	static void Execute();
	Tested();
	virtual ~Tested();

private:
	static void TestSubStr();
};

#endif // !defined(AFX_TESTED_H__07C28D93_D163_43C5_96F3_7482DD99DE59__INCLUDED_)
