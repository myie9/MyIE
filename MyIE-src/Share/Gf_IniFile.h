// Gf_IniFile.h: interface for the CGf_IniFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_INIFILE_H__251E8E96_81BF_4642_9FAC_1CF8B420AF02__INCLUDED_)
#define AFX_GF_INIFILE_H__251E8E96_81BF_4642_9FAC_1CF8B420AF02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define GENERAL_SECTION "General"
#define MYIE9_FORM_SECTION_TITLE "MyIE9__Title"
#define MYIE9_FORM_LAST_TIME "MyIE9__Last_Time"

#define ENC_KEY_HOME1_0 0x02
#define ENC_KEY_HOME1_1 0x01

#define ENC_KEY_DATA1_0 0x01
#define ENC_KEY_DATA1_1 0x02

#define ENC_KEY_DATA_CURRENT_0 ENC_KEY_DATA1_0
#define ENC_KEY_DATA_CURRENT_1 ENC_KEY_DATA1_1

#define INI_SETTINGS "Settings"

class CGf_IniFile  
{
public:
	static BOOL GetSections(const CString& filename, CStringArray& saSections);
	static CString DecodeJust(const CString &base, const CString &strName, const CString &filename);
	static BOOL GetPrivateProfileStringJinSi (BOOL bIsJinSiDomain, LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName );
	static CString GetPCIDMiYao(BYTE flag1, BYTE flag2);
	static BOOL EncAndWriteString(const CString &base, const CString &strName, const CString &strValue, const CString &filename, BYTE btEncType0=ENC_KEY_DATA_CURRENT_0, BYTE btEncType1=ENC_KEY_DATA_CURRENT_1);
	static BOOL Decode(CString &strValue, const CString &base, const CString &strName, const CString &filename);
	static BOOL IsEncValue(const CString& base, const CString& strName, const CString& filename);
	static BOOL IsExistSection(const CString& section, const CString& filepath);
	CGf_IniFile();
	virtual ~CGf_IniFile();

};

#endif // !defined(AFX_GF_INIFILE_H__251E8E96_81BF_4642_9FAC_1CF8B420AF02__INCLUDED_)
