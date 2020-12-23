#ifndef _DISKOBJECT_H_
#define _DISKOBJECT_H_

#define EF_ONLY_FILENAMES	0
#define EF_RELATIVE_NAMES	0
#define EF_FULLY_QUALIFIED	1

class CDiskObject 
{
public:
	static DWORD GetVolumnSerialNumber(const CString& strDisk);
	// ctor(s)
	CDiskObject();
	CDiskObject( CWnd* feedbackWindow );

	// dtor
	~CDiskObject();

	// - - -
	// Operations

	// Files

	BOOL FileExists( const CString& file );
	BOOL FileInformation( const CString& file, BY_HANDLE_FILE_INFORMATION &file_info);
	BOOL CreateFile( const CString& file );
	BOOL CopyFile( const CString& sourceFile, const CString& destDirectory );
	BOOL CopyFile( const CString& sourceFile, const CString& destDirectory, const CString& destFile );
	BOOL RemoveFile( const CString& sourceFile );
	BOOL RenameFile( const CString& sourceFile, const CString& destFile );
	BOOL MoveFile( const CString& sourceFile, const CString& destFile );

	BOOL CopyFiles( const CString& sourceDirectory, const CString& destDirectory );
	BOOL CopyFiles( CStringArray& files, const CString& destDirectory );

	// Directories
	BOOL DirectoryExists( const CString& file );

	BOOL CreateDirectory( const CString& directory );
	BOOL CopyDirectory( const CString& sourceDirectory, const CString& destDirectory );
	BOOL EmptyDirectory( const CString& directory );
	BOOL RemoveDirectory( const CString& directory );

	BOOL CopyDirectories( const CString& sourceDirectory, const CString& destDirectory );
	BOOL EmptyDirectories( const CString& directory );
	BOOL RemoveDirectories( const CString& directory );

	BOOL EnumFilesInDirectoryWithFilter( const CString& filter, const CString& sourceDirectory, CStringArray& files, int mode = EF_ONLY_FILENAMES );
	BOOL EnumAllDirectories( const CString& sourceDirectory, CStringArray& directories, int mode = EF_FULLY_QUALIFIED );
	BOOL EnumDirectories( const CString& sourceDirectory, CStringArray& directories, int mode = EF_ONLY_FILENAMES );
	BOOL EnumFilesInDirectory( const CString& sourceDirectory, CStringArray& files, int mode = EF_ONLY_FILENAMES );
	BOOL EnumAllFiles( const CString& sourceDirectory, CStringArray& files, int mode = EF_FULLY_QUALIFIED );
	BOOL EnumAllFilesWithFilter( const CString& filter, const CString& sourceDirectory, CStringArray& files, int mode = EF_FULLY_QUALIFIED );

	BOOL RenameDirectory( const CString& oldDirectory, const CString& newName );
	BOOL MoveDirectory( const CString& oldDirectory, const CString& newDirectory );

	// Error handling
	CString GetErrorMessage();	

private:
	// Error handling
	CString m_errorMessage;

	void	ClearError();
	void	SetInternalErrorMessage();
	void	SetSystemErrorMessage( int error, const CString& elements = _T( "" ) );

	BOOL	EnumAllDirs( const CString& sourceDirectory, CStringArray& directories );
	BOOL	EnumAllFilesWF( const CString& filter,const CString& sourceDirectory, CStringArray& files );

	// Helpers

	CWnd*	m_feedbackWindow;
	BOOL	QualifyPath( CString& str );
	BOOL	QualifyFile( CString& str );
	CString GetFileName( const CString& str );
	CString GetDirectoryName( const CString& str );
	BOOL	TestPart( const CString& str );

};

#endif // _DISKOBJECT_H_
