// Crypt.cpp: implementation of the CCrypt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyIE9.h"
#include "Crypt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//#pragma optimize( "s", on)

char InnKey1[9]="7eOp@m0D";
char InnKey2[9]="4#76fBU&";

void EncodingByKey(LPCSTR src, CString& dest,char InnKey[9])
{
	int l=0;
	char tmp;
	BYTE t, t2;
	dest.Empty();
	for(UINT i = 0; i<strlen(src); i++)
	{
		t = src[i] ^ InnKey[l];
		t2 = t & 0xF;
		t = t >> 4;
		tmp = 'A' + t;
		dest += tmp;
		tmp = 'A' + t2;
		dest += tmp;

		l++;
		l=l%8;
	}
}

void DecodingByKey(LPCSTR src, CString& dest,char InnKey[9])
{
	int l=0, v;
	BYTE t,	c=0;
	dest.Empty();
	for(UINT i=0; i<strlen(src); i++)
	{
		t=src[i];
		if(t>='A' && t<='Q')
			v = t-'A';
		else
			v = 0;
	
		c = c << 4;
		c |= v;
		if(i%2==1)
		{
			c = c ^ InnKey[l];
			dest += (char)c;
			c=0;
			l++;
			l = l%8;
		}
	}
}

void Encoding(LPCSTR src, CString& dest)
{
	EncodingByKey(src,dest,InnKey1);
}
void Decoding(LPCSTR src, CString& dest)
{
	DecodingByKey(src,dest,InnKey1);
}
void Encoding2(LPCSTR src, CString& dest)
{
	EncodingByKey(src,dest,InnKey2);
}
void Decoding2(LPCSTR src, CString& dest)
{
	DecodingByKey(src,dest,InnKey2);
}
//#pragma optimize( "s", off)

