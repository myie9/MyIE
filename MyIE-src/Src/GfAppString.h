// GfAppString.h: interface for the CGfAppString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GFAPPSTRING_H__2F6AD8C3_8C6C_4A97_BB09_63EB8C3C6520__INCLUDED_)
#define AFX_GFAPPSTRING_H__2F6AD8C3_8C6C_4A97_BB09_63EB8C3C6520__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGfAppString  
{
public:
	static CString GetString(int id);
	CGfAppString();
	virtual ~CGfAppString();

};

#endif // !defined(AFX_GFAPPSTRING_H__2F6AD8C3_8C6C_4A97_BB09_63EB8C3C6520__INCLUDED_)
