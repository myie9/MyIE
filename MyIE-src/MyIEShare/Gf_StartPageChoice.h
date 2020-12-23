// Gf_StartPageChoice.h: interface for the CGf_StartPageChoice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_STARTPAGECHOICE_H__13FF4035_C76C_4407_8A8C_742EDB58BCBC__INCLUDED_)
#define AFX_GF_STARTPAGECHOICE_H__13FF4035_C76C_4407_8A8C_742EDB58BCBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGf_StartPageChoice  
{
public:
	void LoadNames();
	CString GetMyIE9StartPage();
	void SetSel(int npos);
	int GetOldSel();
	CString GetName(int i);
	int GetNamesSize();
	static CGf_StartPageChoice& GetIns();
	CGf_StartPageChoice();
	virtual ~CGf_StartPageChoice();

private:
	void LoadFiles();
	CStringArray _names,_files;
};

#endif // !defined(AFX_GF_STARTPAGECHOICE_H__13FF4035_C76C_4407_8A8C_742EDB58BCBC__INCLUDED_)
