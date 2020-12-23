// PathHelper.cpp: implementation of the CPathHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PathHelper.h"


#include <sys\stat.h>	//for struct stat
#include <direct.h>		//for mkdir

#include "DiskObject.h"

#include "RegHelper.h"

//#include "macro.h"
#define STRNCPY(to, head, len) strncpy(to, head, len); *(to+len) = 0;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//#define _INI_VAR_FAVORITE_PATH__D _T("D:\\收藏夹")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPathHelper::CPathHelper()
{
	
}

CPathHelper::~CPathHelper()
{
	
}

BOOL CPathHelper::IsNeedChangeFavDir(const CString& strDefaultPath, CString& strFavFromReg, CString& csFavoritePath)
{
	char cSysRoot = CPathHelper::GetSysRootDir();
	
	CRegHelper::GetFavoriteFolder(strFavFromReg);
	
	bool bIsInSameDisk = toupper(strFavFromReg[0]) == cSysRoot;
	bool bIsNotSameAsDefault = false;
	if(strncmp( (LPCTSTR)strFavFromReg, strDefaultPath, MAX_PATH) !=0)
		bIsNotSameAsDefault = true;
	
	csFavoritePath = strDefaultPath;
	
	if ( bIsInSameDisk || bIsNotSameAsDefault )
	{
		//CalculateFavoritePath
			
			csFavoritePath.SetAt(0, CPathHelper::GetAnotherDiskDir(cSysRoot) );
			
			return TRUE;
	}
	else if(!CPathHelper::IsExist(csFavoritePath))
		CPathHelper::CreateFullDir(csFavoritePath);
	
	return FALSE;
}

char CPathHelper::GetSysRootDir()
{
	TCHAR szPath[MAX_PATH] = {'\0'};
	::GetSystemDirectory(szPath, MAX_PATH);
	return toupper(szPath[0]);
}



BOOL CPathHelper::IsExist(const CString& strPathFileOrDir)
{
	CString strPath = strPathFileOrDir;
	//必须这样做,因为 末尾带 \ 的 FindFirstFile stat 中不能用
	int nSlash = strPath.ReverseFind('\\');
	if( nSlash == strPath.GetLength()-1)
		strPath = strPath.Left(nSlash);
		/*
		WIN32_FIND_DATA fd;
		HANDLE hFind = FindFirstFile( strPath, &fd );
		if ( hFind != INVALID_HANDLE_VALUE )
		FindClose( hFind );
		return hFind != INVALID_HANDLE_VALUE;
	*/
	struct stat stat_buf;
	if (stat(strPath, &stat_buf) >= 0)
		return TRUE;
	else
		return FALSE;
}

void CPathHelper::CreateFullDir(const char *dir)
{
    struct stat stat_buf;
	char *p;
	char parent[MAX_PATH];
	int length;
	
	if (CPathHelper::IsExist(dir))
		return;
    if ( strlen(dir) == 0) 
		return;
	p = strchr(dir,'\\');
	while(p)
	{
		length = p-dir;
		STRNCPY(parent,dir,length);
		//make dir
		if (stat(parent, &stat_buf) < 0 && strlen(parent) > 2)//can not mkdir("c:")
		{
			if ( mkdir(parent)!=0 )
				return;
		}
		//move to next
		if (*(p+1)==0)
			return;
		p = strchr(p+1,'\\');
	}
	//for c:\\aa\bb\cc\\dd,mow create dd
	if (stat(dir, &stat_buf) < 0 && strlen(dir) > 2)//can not mkdir("c:")
	{
		if ( mkdir(dir)!=0 )
			return;
	}
	
}

char CPathHelper::GetAnotherDiskDir(const char &cSysRoot)
{
	char cDefault = cSysRoot + 1;
	CStringArray saDrivers;
	if(!FindAllDrivers(saDrivers) )
	{
		CString strDefault(cDefault);
		if( CPathHelper::IsWinCompatibleDisk(strDefault) )
			return cDefault;
	}
	
	CString strSysRoot;
	strSysRoot.Format("%c:\\", cSysRoot);
	for(int i =0; i< saDrivers.GetSize(); i++)
	{
		if(saDrivers[i] == strSysRoot || saDrivers[i] == "C:\\" )
			continue;
		
		if( CPathHelper::IsWinCompatibleDisk( saDrivers[i] ) )
		{
			return saDrivers[i][0];
		}
		
	}
	
	
	return cSysRoot;
}

BOOL CPathHelper::IsWinCompatibleDisk(const CString& strDriveName)
{
	CString strDriveFormat;
	CPathHelper::GetDriveFormat(strDriveName, strDriveFormat);
	
	if(strDriveFormat.CompareNoCase("NTFS") == 0 || strDriveFormat.CompareNoCase("FAT32") == 0 || strDriveFormat.CompareNoCase("FAT") == 0 || strDriveFormat.CompareNoCase("FAT16") == 0  )
	{
		return TRUE;
	}
	else
		return FALSE;
	
}

void CPathHelper::GetDriveFormat(const CString& strDriveName,CString& strDriveFormat) 
{
	if( ('A' == strDriveName[0] )||('a' == strDriveName[0] )) //slow,need 5 seconds
	{
		strDriveFormat= "";
		return;
	}
	
	//获得驱动器文件系统
	LPCTSTR lpRootPathName = (LPCTSTR)strDriveName;
	TCHAR VolumeNameBuffer[12];
	DWORD nVolumeNameSize=12;
	DWORD VolumeSerialNumber;
	DWORD MaximumComponentLength;
	DWORD FileSystemFlags;
	TCHAR szFileSystemNameBuffer[10]= {'\0'};
	DWORD nFileSystemNameSize=10;
	
	GetVolumeInformation(lpRootPathName,
		VolumeNameBuffer, nVolumeNameSize,
		&VolumeSerialNumber, 
		&MaximumComponentLength,
		&FileSystemFlags,
		szFileSystemNameBuffer, nFileSystemNameSize);
	
	strDriveFormat.Format("%s", szFileSystemNameBuffer);
	//显示驱动器序列号
	//CString str;
	//str.Format("驱动器%s的文件系统为%s",strRootPathName,FileSystemNameBuffer);
	//AfxMessageBox(str);
}



BOOL CPathHelper::FindAllDrivers(CStringArray& saDrivers)
{
	
	DWORD dwNumBytesForDriveStrings;//实际存储驱动器号的字符串长度
	HANDLE hHeap;
	LPSTR lp;
	
	//获得实际存储驱动器号的字符串长度
	dwNumBytesForDriveStrings=GetLogicalDriveStrings(0,NULL)*sizeof(TCHAR);
	
	//如果字符串不为空，则表示有正常的驱动器存在
	if (dwNumBytesForDriveStrings!=0) {
		//分配字符串空间
		hHeap=GetProcessHeap();
		lp=(LPSTR)HeapAlloc(hHeap,HEAP_ZERO_MEMORY,
			dwNumBytesForDriveStrings);
		
		//获得标明所有驱动器的字符串
		GetLogicalDriveStrings(HeapSize(hHeap,0,lp),lp);
		
		//将驱动器一个个放到下拉框中
		while (*lp!=0) {
			//Driver->AddString(lp);
			saDrivers.Add(lp);
			lp=_tcschr(lp,0)+1;
		}
	}
	else 
	{
		
		//	AfxMessageBox("Can't Use The Function GetLogicalDriveStrings!");
		return FALSE;
	}
	
	return TRUE;
}

BOOL CPathHelper::CopyDirectory( const CString& sourceDirectory, const CString& destDirectory )
{
	CDiskObject oDisk;
	return oDisk.CopyDirectories(sourceDirectory, destDirectory );
}


//****************************************************************************
//Func:	根据上层目录路径和当前目录名组合成当前目录路径
//Parm:	IN | LPCTSTR | pszDir | 上层目录路径
//		IN | LPCTSTR | pszDirName | 当前目录名
//		OUT | LPTSTR | pszRetDirPath | 当前目录路径
//Ret:	TRUE | 成功
//		FALSE | 失败
//****************************************************************************
BOOL CPathHelper::MixDirPathName(IN LPCTSTR pszDir, IN LPCTSTR pszDirName, OUT LPTSTR pszRetDirPath)
{
	strcpy(pszRetDirPath, pszDir);
	FmtDir(pszRetDirPath);
	strcat(pszRetDirPath, pszDirName);
	strcat(pszRetDirPath, "\\");
	return TRUE;
}
//****************************************************************************
//Func:	根据上层目录路径和当前文件名组合成当前文件路径
//Parm:	IN | LPCTSTR | pszDir | 上层目录路径
//		IN | LPCTSTR | pszFileName | 当前文件名
//		OUT | LPTSTR | pszRetFilePath | 当前文件路径
//Ret:	TRUE | 成功
//		FALSE | 失败
//****************************************************************************
BOOL CPathHelper::MixFilePathName(IN LPCTSTR pszDir, IN LPCTSTR pszFileName, OUT LPTSTR pszRetFilePath)
{
	strcpy(pszRetFilePath, pszDir);
	FmtDir(pszRetFilePath);
	strcat(pszRetFilePath, pszFileName);
	return TRUE;
}

//****************************************************************************
//Func:	格式化目录字符串，最后只留一个'\'
//Parm:	IN | LPCTSTR | pszDir | 需格式化目录字符串
//Ret:	[0-+++] | 成功,格式化后的字符串长度
//		-1 | 失败
//****************************************************************************
int CPathHelper::FmtDir(IN LPTSTR pszDir)
{
	BOOL bDir = FALSE;
	int nLen = strlen(pszDir);
	for (int i=nLen-1; i>=0; i--)
	{
		if (pszDir[i] != '\\')
		{
			pszDir[i+1] = '\0';
			break;
		}
	}
	strcpy(pszDir, "\\");
	
	return (i+2);
}

CString CPathHelper::GetUniqueTmpFilePath()
{
	//dir
	CString strTmpDir = GetTmpDir();
	
	TCHAR szTmpFile[MAX_PATH];
	UINT uiResult = ::GetTempFileName(strTmpDir, "~GF", 0, szTmpFile);
	ASSERT(uiResult);
	
	return szTmpFile;
	
}

CString CPathHelper::GetTmpDir()
{
	
	TCHAR szTmpDir[MAX_PATH];
	
	DWORD dwResult = ::GetTempPath(MAX_PATH, szTmpDir);
	
	ASSERT(dwResult);
	
	return szTmpDir;
}

CString CPathHelper::GetExeDir()
{
	
//	HINSTANCE hInstance = AfxGetResourceHandle();
 //   ASSERT(hInstance != NULL);
	LPTSTR lpszModule;
	
	try
	{
		lpszModule = new TCHAR[_MAX_PATH];
    }
	catch(...)
	{
		ASSERT(false);
		return "";
	}
	
	CString strRoot ="";
	
	if (GetModuleFileName(NULL, lpszModule, _MAX_PATH)) //hInstance
    {
		strRoot = lpszModule;
		
		int i = strRoot.ReverseFind('\\');
		strRoot= strRoot.Left(i+1);
	}
	
	delete [] lpszModule;
	
	ASSERT("" != strRoot);
	
	return strRoot;
}

CString CPathHelper::GetExePath()
{

		char lpszModule[_MAX_PATH];
		
		HINSTANCE hInstance = AfxGetInstanceHandle();
		if (!GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
		{
			ASSERT(false);
			return "";
		}

		CString strRet;
		strRet.Format("%s", lpszModule);

		return strRet;
}
