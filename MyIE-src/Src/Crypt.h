// Crypt.h: interface for the CCrypt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRYPT_H__57247F42_5DAE_11D5_9BB1_DDA62D507110__INCLUDED_)
#define AFX_CRYPT_H__57247F42_5DAE_11D5_9BB1_DDA62D507110__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

void Encoding(LPCSTR src, CString& dest);
void Decoding(LPCSTR src, CString& dest);
void Encoding2(LPCSTR src, CString& dest);
void Decoding2(LPCSTR src, CString& dest);

#endif // !defined(AFX_CRYPT_H__57247F42_5DAE_11D5_9BB1_DDA62D507110__INCLUDED_)
