// PathHelper.h: interface for the CPathHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATHHELPER_H__487C406D_CE89_4CA7_82E0_C564698F3E08__INCLUDED_)
#define AFX_PATHHELPER_H__487C406D_CE89_4CA7_82E0_C564698F3E08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPathHelper  
{
public:
	static BOOL CopyDirectory( const CString& sourceDirectory, const CString& destDirectory );
	static char GetAnotherDiskDir(const char& cSysRoot);
	static void CreateFullDir(const char *dir);
	static BOOL IsExist(const CString& strPathFileOrDir); 

	static char GetSysRootDir(); //大写的盘符

	static IsNeedChangeFavDir(const CString& strDefaultPath, CString& strFavFromReg, CString& csFavoritePath); //gff favorite 设置收藏夹为第一个非系统盘的根目录下的“收藏夹”文件夹内:csFavoritePath。
private:

	//****************************************************************************
	//Func:	格式化目录字符串，最后只留一个'\\'
	//Parm:	IN | LPCTSTR | pszDir | 需格式化目录字符串
	//Ret:	[0-+++] | 成功,格式化后的字符串长度
	//		-1 | 失败
	//****************************************************************************
	static	int	FmtDir(IN LPTSTR pszDir);
	//****************************************************************************
	//Func:	根据上层目录路径和当前目录名组合成当前目录路径
	//Parm:	IN | LPCTSTR | pszDir | 上层目录路径
	//		IN | LPCTSTR | pszDirName | 当前目录名
	//		OUT | LPTSTR | pszRetDirPath | 当前目录路径
	//Ret:	TRUE | 成功
	//		FALSE | 失败
	//****************************************************************************
	static BOOL MixDirPathName(IN LPCTSTR pszDir, IN LPCTSTR pszDirName, OUT LPTSTR pszRetDirPath);
	//****************************************************************************
	//Func:	根据上层目录路径和当前文件名组合成当前文件路径
	//Parm:	IN | LPCTSTR | pszDir | 上层目录路径
	//		IN | LPCTSTR | pszFileName | 当前文件名
	//		OUT | LPTSTR | pszRetFilePath | 当前文件路径
	//Ret:	TRUE | 成功
	//		FALSE | 失败
	//****************************************************************************
	static BOOL MixFilePathName(IN LPCTSTR pszDir, IN LPCTSTR pszFileName, OUT LPTSTR pszRetFilePath);

	/*************
	C:\
	*************/
	static BOOL FindAllDrivers(CStringArray& saDrivers); 

	//windows支持的 硬盘格式
	static BOOL IsWinCompatibleDisk(const CString& strDriveName);

	//NTFS FAT32
	static void GetDriveFormat(const CString& strDriveName,CString& strDriveFormat);



public:
	static CString GetExePath();
	static CString GetExeDir();
	static CString GetTmpDir();
	static CString GetUniqueTmpFilePath();
	CPathHelper();
	virtual ~CPathHelper() = 0;

};

#endif // !defined(AFX_PATHHELPER_H__487C406D_CE89_4CA7_82E0_C564698F3E08__INCLUDED_)
