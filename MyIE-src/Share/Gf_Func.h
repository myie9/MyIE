// Gf_Func.h: interface for the Gf_Func class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GF_FUNC_H__A84F7952_9A72_44D6_9918_F5FF7E55FE7C__INCLUDED_)
#define AFX_GF_FUNC_H__A84F7952_9A72_44D6_9918_F5FF7E55FE7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//########################################################
CString _StringGetUpUrl(CString strUrl);
CString _StringGetRootUrl(CString strUrl);
CString _StringGetTopUrl(CString strUrl);


//########################################################
void _Ascii2Utf8(CString& strAscii);
void _AsciiFormat(CString& strAscii);
//########################################################

char* _FileGetBuffer(CString filename);

//########################################################
BOOL	_FileIsExist(CString strPath);
void	_CreateFullDir(const char *dir);

//########################################################
void	_SetClipString(CString str);
CString _GetClipString();

//########################################################

#endif // !defined(AFX_GF_FUNC_H__A84F7952_9A72_44D6_9918_F5FF7E55FE7C__INCLUDED_)
