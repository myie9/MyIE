// Gf_Update.h: interface for the CGf_Update class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_UPDATE_H__C4F6A5E2_46D7_4119_A4DA_97174C786C19__INCLUDED_)
#define AFX_GF_UPDATE_H__C4F6A5E2_46D7_4119_A4DA_97174C786C19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGf_Update  
{
public:
	CGf_Update();
	virtual ~CGf_Update();

	static void Execute();

private:
	static  BOOL IsOKToUpdate();

};

#endif // !defined(AFX_GF_UPDATE_H__C4F6A5E2_46D7_4119_A4DA_97174C786C19__INCLUDED_)
