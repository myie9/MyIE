#include "stdafx.h"
#include "Menuorder.h"

char MENU_ORDER_KEY[] = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\MenuOrder\\Favorites\\";

DWORD GetDWORD(LPBYTE& lpb, LPBYTE lpend)
{
	if(lpb+4>lpend && lpend!=NULL)
		return 0;

	DWORD dw = *((DWORD*)lpb);
	lpb+=4;
	return dw;
}

WORD GetWORD(LPBYTE& lpb, LPBYTE lpend)
{
	if(lpb+2>lpend && lpend!=NULL)
		return 0;

	WORD w = *((WORD*)lpb);
	lpb+=2;
	return w;
}

char* GetString(LPBYTE& lpb, LPBYTE lpend)
{
	int sl = strlen((char*)lpb);
	if(lpb+sl>lpend && lpend!=NULL)
		return NULL;

	if(sl>0)
	{
		char * lptemp;
		try
		{
			lptemp = new char[sl+1];
		}
		catch(...)
		{
			return NULL;
		}

		if(lptemp!=NULL)
		{
			strcpy(lptemp, (char*)lpb);
		}
		lpb+=sl;
		while(*lpb==0 && (lpb<=lpend || lpend==NULL))//5.6.27,lpb++;
			lpb++;
		return lptemp;
	}
	else
		return NULL;
}

char* GetStringW(LPBYTE& lpb, LPBYTE lpend)
{
	int sl = wcslen((wchar_t*)lpb);
	sl = sl*2;
	if(lpb+sl>lpend && lpend!=NULL)
		return NULL;

	if(sl>0)
	{
		char * lptemp;
		try
		{
			lptemp = new char[sl+2];
		}
		catch(...)
		{
			return NULL;
		}

		if(lptemp!=NULL)
		{
			WideCharToMultiByte(CP_OEMCP, 0, (LPCWSTR)lpb, -1, lptemp, sl+2, NULL, NULL); 
			//strcpy(lptemp, (char*)lpb);
		}
		lpb+=sl;
		while(*lpb==0 && (lpb<=lpend || lpend==NULL))
			lpb++;
		return lptemp;
	}
	else
		return NULL;
}

void PutWORD(WORD w, LPBYTE& lpb)
{
	*lpb = LOBYTE(w);
	lpb ++;
	*lpb = HIBYTE(w);
	lpb++;
}

void PutDWORD(DWORD dw, LPBYTE& lpb)
{
	PutWORD(LOWORD(dw), lpb);
	PutWORD(HIWORD(dw), lpb);
}

void PutString(char* lpstr, LPBYTE& lpb)
{
	if(lpstr==NULL)
		return;
	strcpy((char*)lpb, lpstr);
	lpb += strlen(lpstr);
	lpb++;
}

BOOL CMenuOrder::LoadMenuOrder(LPCSTR sFolderPath)
{
	if(mois)
		delete[] mois;
	if(index)
		delete[] index;
	mois = NULL;
	index = NULL;
	len = 0;

	HKEY hKey;
	BOOL ret = FALSE;
	lpSubKey = MENU_ORDER_KEY;
	lpSubKey += sFolderPath;
	HRESULT hr = RegOpenKey(HKEY_CURRENT_USER, lpSubKey, &hKey);
	if(hr == ERROR_SUCCESS)
	{
		BYTE* lpBuf = NULL;
		DWORD type = 0;
		DWORD size = 0;
		hr = RegQueryValueEx(hKey, "Order", NULL, &type, NULL, &size);
		if(size>0)
		{
			try
			{
				lpBuf = new BYTE[size];
			}
			catch(...)
			{
				RegCloseKey(hKey);
				return FALSE;
			}

			hr = RegQueryValueEx(hKey, "Order", NULL, &type, lpBuf, &size);

			if(type == REG_BINARY)
			{
				BYTE* lpData = lpBuf;
				lpData += 8;
				cbsize = GetDWORD(lpData, NULL);
				reserved = GetDWORD(lpData, NULL);
				len = GetDWORD(lpData, NULL);
				if(len>0)
				{
					try
					{
						mois = new CMenuOrderItem[len];
						index = new int[len];
						ZeroMemory(index, len*sizeof(int));
					}
					catch(...)
					{
						len = 0;
					}
				}
				int len1, len2;
				BOOL bShortFirst;
				for(int i =0; i< len && mois!= NULL; i++)
				{
					BYTE* lpend = lpBuf+size;
					BYTE* lpbt = lpData;
					len1 = mois[i].cbsize = GetDWORD(lpData, lpend);

					lpend = lpbt + len1;
					mois[i].order = GetDWORD(lpData, lpend);
					len2 = mois[i].len = GetWORD(lpData, lpend);
					mois[i].itemtype = GetWORD(lpData, lpend);
					mois[i].filesize = GetDWORD(lpData, lpend);
					mois[i].filedate = GetWORD(lpData, lpend);
					mois[i].filetime = GetWORD(lpData, lpend);
					mois[i].filetype = GetWORD(lpData, lpend);
					//get first name
					bShortFirst=FALSE;
					if((mois[i].itemtype & 4) != 0)
						mois[i].shortname = GetStringW(lpData, lpend);
					else
						mois[i].shortname = GetString(lpData, lpend);
					//is short first
					if (strstr(mois[i].shortname,"\x03\x04"))
						bShortFirst = TRUE;
					if (memcmp(lpData+1, "\x00\x03\x00\x04",4)==0)
					{
						lpData += 20;
						bShortFirst = TRUE;
					}
					//get next name
					if (bShortFirst)
						mois[i].longname = GetStringW(lpData, lpend);
					else
					{
						mois[i].longname = mois[i].shortname;
						mois[i].shortname = GetString(lpData, lpend);
					}
					//
					if(mois[i].order>=0 && mois[i].order<len)
						index[mois[i].order] = i;

					lpData = lpbt + len1;
					ret = TRUE;
				}
			}
			delete[] lpBuf;
		}
		RegCloseKey(hKey);
	}
 
	return ret;
}

BOOL CMenuOrder::WriteMenuOrder(LPCSTR sFolderPath)
{
	//sort dir and files by string
	if(len<=0)
		return FALSE;
	try
	{
		index = new int[len];
	}
	catch(...)
	{
		return FALSE;
	}

	int i, j;
	index[0]=0;
	for(i = 1; i<len; i++)
	{
		for(j=i-1; j>=0; j--)
		{
			if(lstrcmpiA(mois[i].longname, mois[index[j]].longname)<0)
			{
				index[j+1]=index[j];
				if(j==0)
					index[j]=i;
			}
			else
			{
				index[j+1]=i;
				break;
			}
		}
	}

	cbsize = 12;
	reserved = 1;
	for(i =0; i<len; i++)
	{
		//calculate length
		if(mois[i].longname!=NULL && mois[i].shortname!=NULL)
			mois[i].cbsize = strlen(mois[i].longname)+1 + strlen(mois[i].shortname)+1;  
		mois[i].cbsize += 22+6;
		cbsize += mois[i].cbsize;
	}

	HKEY hKey;
	BOOL ret = FALSE;
	lpSubKey = MENU_ORDER_KEY;
	lpSubKey += sFolderPath;
	HRESULT hr = RegCreateKeyEx(HKEY_CURRENT_USER, lpSubKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	int slen;

	if(hr == ERROR_SUCCESS)
	{
		LPBYTE lpbuf;
		try
		{
			lpbuf = new BYTE[cbsize + 8];
		}
		catch(...)
		{
			lpbuf = NULL;
		}

		if(lpbuf!=NULL)
		{
			LPBYTE lpt = lpbuf;
			PutDWORD(0x08, lpt);
			PutDWORD(0x02, lpt);
			PutDWORD(cbsize, lpt);
			PutDWORD(reserved, lpt);
			PutDWORD(len, lpt);
			for(j=0; j<len; j++)
			{
				i = index[j];
				if(mois[i].itemtype != 0x31)
					continue;
				PutDWORD(mois[i].cbsize, lpt);
				PutDWORD(mois[i].order, lpt);
				slen = 0;
				if(mois[i].longname!=NULL)
					slen = strlen(mois[i].longname);
				if(mois[i].shortname!=NULL)
					slen += strlen(mois[i].shortname);
				PutWORD(slen+1+1+14, lpt);
				PutWORD(mois[i].itemtype, lpt);
				PutDWORD(mois[i].filesize, lpt);
				PutWORD(mois[i].filedate, lpt);
				PutWORD(mois[i].filetime, lpt);
				PutWORD(mois[i].filetype, lpt);
				PutString(mois[i].longname, lpt);
				PutString(mois[i].shortname, lpt);
				PutWORD((WORD)0, lpt); // end with two 0
				PutDWORD((DWORD)0, lpt); //item end
			}
			for(j=0; j<len; j++)
			{
				i = index[j];
				if(mois[i].itemtype != 0x32)
					continue;
				PutDWORD(mois[i].cbsize, lpt);
				PutDWORD(mois[i].order, lpt);
				slen = 0;
				if(mois[i].longname!=NULL)
					slen = strlen(mois[i].longname);
				if(mois[i].shortname!=NULL)
					slen += strlen(mois[i].shortname);
				PutWORD(slen+1+1+14, lpt);
				PutWORD(mois[i].itemtype, lpt);
				PutDWORD(mois[i].filesize, lpt);
				PutWORD(mois[i].filedate, lpt);
				PutWORD(mois[i].filetime, lpt);
				PutWORD(mois[i].filetype, lpt);
				PutString(mois[i].longname, lpt);
				PutString(mois[i].shortname, lpt);
				PutWORD((WORD)0, lpt); // end with two 0
				PutDWORD((DWORD)0, lpt); //item end
			}
			//save
			RegSetValueEx(hKey, "Order", 0, REG_BINARY, lpbuf, cbsize+8);
			//release
			delete[] lpbuf;
			ret = TRUE;
		}
	}
	RegCloseKey(hKey);

	return ret;
}
