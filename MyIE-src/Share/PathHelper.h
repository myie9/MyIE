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

	static char GetSysRootDir(); //��д���̷�

	static IsNeedChangeFavDir(const CString& strDefaultPath, CString& strFavFromReg, CString& csFavoritePath); //gff favorite �����ղؼ�Ϊ��һ����ϵͳ�̵ĸ�Ŀ¼�µġ��ղؼС��ļ�����:csFavoritePath��
private:

	//****************************************************************************
	//Func:	��ʽ��Ŀ¼�ַ��������ֻ��һ��'\\'
	//Parm:	IN | LPCTSTR | pszDir | ���ʽ��Ŀ¼�ַ���
	//Ret:	[0-+++] | �ɹ�,��ʽ������ַ�������
	//		-1 | ʧ��
	//****************************************************************************
	static	int	FmtDir(IN LPTSTR pszDir);
	//****************************************************************************
	//Func:	�����ϲ�Ŀ¼·���͵�ǰĿ¼����ϳɵ�ǰĿ¼·��
	//Parm:	IN | LPCTSTR | pszDir | �ϲ�Ŀ¼·��
	//		IN | LPCTSTR | pszDirName | ��ǰĿ¼��
	//		OUT | LPTSTR | pszRetDirPath | ��ǰĿ¼·��
	//Ret:	TRUE | �ɹ�
	//		FALSE | ʧ��
	//****************************************************************************
	static BOOL MixDirPathName(IN LPCTSTR pszDir, IN LPCTSTR pszDirName, OUT LPTSTR pszRetDirPath);
	//****************************************************************************
	//Func:	�����ϲ�Ŀ¼·���͵�ǰ�ļ�����ϳɵ�ǰ�ļ�·��
	//Parm:	IN | LPCTSTR | pszDir | �ϲ�Ŀ¼·��
	//		IN | LPCTSTR | pszFileName | ��ǰ�ļ���
	//		OUT | LPTSTR | pszRetFilePath | ��ǰ�ļ�·��
	//Ret:	TRUE | �ɹ�
	//		FALSE | ʧ��
	//****************************************************************************
	static BOOL MixFilePathName(IN LPCTSTR pszDir, IN LPCTSTR pszFileName, OUT LPTSTR pszRetFilePath);

	/*************
	C:\
	*************/
	static BOOL FindAllDrivers(CStringArray& saDrivers); 

	//windows֧�ֵ� Ӳ�̸�ʽ
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
