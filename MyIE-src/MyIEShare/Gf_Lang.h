// Gf_Lang.h: interface for the CGf_Lang class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_LANG_H__B7134380_E487_410D_BE58_6B1143D4BC68__INCLUDED_)
#define AFX_GF_LANG_H__B7134380_E487_410D_BE58_6B1143D4BC68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGf_Lang  
{
public:
	static CString GetStr(int id);
	
	CGf_Lang();
	virtual ~CGf_Lang();

};

#endif // !defined(AFX_GF_LANG_H__B7134380_E487_410D_BE58_6B1143D4BC68__INCLUDED_)
