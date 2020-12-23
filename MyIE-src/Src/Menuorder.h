#if !defined(MENUORDER_H__19E497C1_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_)
#define MENUORDER_H__19E497C1_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_

extern char MENU_ORDER_KEY[];

class CMenuOrderItem
{
public:
	CMenuOrderItem()
	{
		longname = NULL;
		shortname = NULL;
	};
	~CMenuOrderItem()
	{
		if(longname)
			delete[] longname;
		if(shortname)
			delete[] shortname;
	};

	int cbsize;
	int order;
	WORD len;
	WORD itemtype;
	int filesize;
	WORD filedate;
	WORD filetime;
	WORD filetype;
	char * longname;
	char * shortname;
};

class CMenuOrder
{
public:
	CMenuOrder()
	{
		mois = NULL;
		index = NULL;
	};
	~CMenuOrder()
	{
		if(mois)
			delete[] mois;
		if(index)
			delete[] index;
	};

	BOOL LoadMenuOrder(LPCSTR sFolderPath); //relative path
	BOOL WriteMenuOrder(LPCSTR sFolderPath);
	int cbsize;
	int reserved; 
	int len;
	CMenuOrderItem * mois; //array of menu item
	int * index;
	CString lpSubKey;
};

#endif