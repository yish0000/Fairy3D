/*
 * ------------------------------------------------------------------------
 *  Name:   FWinAPI.cpp
 *  Desc:   This file implements some windows-related functions.
 *  Author: Yish
 *  Date:   2013/11/1
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FSysAPI.h"
#include "FStrUtil.h"
#include <DbgHelp.h>
#include <Shlwapi.h>
#include <io.h>
#include <sys/stat.h>

// Access the specified file.
int FSysAPI::AccessFile(const char* filename, int mode)
{
    return _waccess(F_UTF8ToUnicode(filename).c_str(), mode);
}

// Does the file exist ?
bool FSysAPI::IsFileExist(const char* filename)
{
	if( INVALID_FILE_ATTRIBUTES == GetFileAttributesW(F_UTF8ToUnicode(filename).c_str()) )
		return false;
	else
		return true;
}

// Get the time stamp of the file.
time_t FSysAPI::GetFileTimeStamp(const char* filename)
{
	struct _stat st;
	_wstat(F_UTF8ToUnicode(filename).c_str(), &st);
	return st.st_mtime;
}

// Get the size of the file.
uint32 FSysAPI::GetFileSize(const char* filename)
{
	struct _stat st;
	_wstat(F_UTF8ToUnicode(filename).c_str(), &st);
	return (uint32)st.st_size;
}

// Is the specified a directory.
bool FSysAPI::IsDirectory(const char* filename)
{
	DWORD dwRet = GetFileAttributesW(F_UTF8ToUnicode(filename).c_str());
	return (dwRet & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

// Whether the specified target is a normal file.
bool FSysAPI::IsNormalFile(const char* filename)
{
	DWORD dwRet = GetFileAttributesW(F_UTF8ToUnicode(filename).c_str());
	return (dwRet & FILE_ATTRIBUTE_ARCHIVE) != 0;
}

// Get the current directory.
AString FSysAPI::GetCurrentDir()
{
    wchar_t cur_path[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, cur_path);
    return F_UnicodeToUTF8(cur_path);
}

// Get the document directory.
AString FSysAPI::GetDocumentDir()
{
	return GetCurrentDir();
}

// Move the specified file.
bool FSysAPI::FileMove(const char* filename, const char* dest)
{
	return MoveFileW(F_UTF8ToUnicode(filename).c_str(), F_UTF8ToUnicode(dest).c_str()) != 0;
}

// Copy the specified file.
bool FSysAPI::FileCopy(const char* filename, const char* dest, bool bCover /* = false */)
{
	return CopyFileW(F_UTF8ToUnicode(filename).c_str(), F_UTF8ToUnicode(dest).c_str(), bCover ? FALSE : TRUE) != 0;
}

// Delete the specified file.
bool FSysAPI::RemoveFile(const char* filename)
{
	return DeleteFileW(F_UTF8ToUnicode(filename).c_str()) != 0;
}

// Rename the specified file.
bool FSysAPI::Rename(const char* oldfile, const char* newfile)
{
	return MoveFileW(F_UTF8ToUnicode(oldfile).c_str(), F_UTF8ToUnicode(newfile).c_str()) != 0;
}

// Create the specified folder.
bool FSysAPI::CreateFolder(const char* szFolder, bool bRecurse /* = true */)
{
	if( bRecurse )
	{
		AString strDir = szFolder;
		std::replace(strDir.begin(), strDir.end(), '/', '\\');

		if( strDir[strDir.length()-1] == '\\' )
			strDir[strDir.length()-1] = '\0';

		wchar_t szCurDir[MAX_PATH];
		GetCurrentDirectoryW(MAX_PATH, szCurDir);

		AString strTemp;
		size_t nPos = strDir.find_first_of('\\', 0);
		while( nPos != AString::npos )
		{
			strTemp = strDir.substr(0, nPos);
			CreateDirectoryW(F_UTF8ToUnicode(strTemp).c_str(), NULL);
			nPos = strDir.find_first_of('\\', nPos + 1);
		}

		CreateDirectoryW(F_UTF8ToUnicode(szFolder).c_str(), NULL);

		// Restore the current directory
		SetCurrentDirectoryW(szCurDir);
		return true;
	}
	else
	{
		return CreateDirectoryW(F_UTF8ToUnicode(szFolder).c_str(), NULL) != 0;
	}
}

// Delete the specified folder.
bool FSysAPI::DeleteFolder(const char* szFolder)
{
	AString strFind = AString(szFolder) + "\\*.*";

	WIN32_FIND_DATAW data;
	HANDLE hFind = FindFirstFileW(F_UTF8ToUnicode(strFind).c_str(), &data);
	BOOL bFound = (hFind != INVALID_HANDLE_VALUE) ? TRUE : FALSE;
	while( bFound )
	{
		if( !wcscmp(data.cFileName, L".") ||
			!wcscmp(data.cFileName, L"..") )
			continue;

		if( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			AString tempDir = AString(szFolder) + "\\" + F_UnicodeToUTF8(data.cFileName);
			DeleteFolder(tempDir.c_str());
		}
		else
		{
			AString tempFile = AString(szFolder) + "\\" + F_UnicodeToUTF8(data.cFileName);
			DeleteFileW(F_UTF8ToUnicode(tempFile).c_str());
		}

		bFound = FindNextFileW(hFind, &data);
	}

	FindClose(hFind);
	return RemoveDirectoryW(F_UTF8ToUnicode(szFolder).c_str()) != 0;
}

// Search files from the specified folder.
bool FSysAPI::SearchFile(const char* szFolder, const char* szToken, AStringList& fileList, bool bRecurse)
{
	AString strFind = AString(szFolder) + "\\" + szToken;

	WIN32_FIND_DATAW data;
	HANDLE hFind = FindFirstFileW(F_UTF8ToUnicode(strFind).c_str(), &data);
	BOOL bFound = (hFind != INVALID_HANDLE_VALUE) ? TRUE : FALSE;
	while( bFound )
	{
		if( !wcscmp(data.cFileName, L".") ||
			!wcscmp(data.cFileName, L"..") )
			continue;

		if( (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		{
            if( !bRecurse ) continue;
			AString tempDir = AString(szFolder) + "\\" + F_UnicodeToUTF8(data.cFileName);
			SearchFile(tempDir.c_str(), szToken, fileList);
		}
		else
		{
			AString tempFile = AString(szFolder) + "\\" + F_UnicodeToUTF8(data.cFileName);
			fileList.push_back(tempFile);
		}

		bFound = FindNextFileW(hFind, &data);
	}

	FindClose(hFind);
	return true;
}

// Search files from the specified folder.
bool FSysAPI::SearchFile(const char* szFolder, const char* szToken, CB_ENUMFILEOP op, void* param, bool bRecurse /* = true */)
{
	if( !op || !param )
		return false;

	bool bFail = false;
	AString strFind = AString(szFolder) + "\\" + szToken;

	WIN32_FIND_DATAW data;
	HANDLE hFind = FindFirstFileW(F_UTF8ToUnicode(strFind).c_str(), &data);
	BOOL bFound = (hFind != INVALID_HANDLE_VALUE) ? TRUE : FALSE;
	while( bFound )
	{
		if( !wcscmp(data.cFileName, L".") ||
			!wcscmp(data.cFileName, L"..") )
			continue;

		if( (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		{
			if( !bRecurse ) continue;
			AString tempDir = AString(szFolder) + "\\" + F_UnicodeToUTF8(data.cFileName);
			if( !SearchFile(tempDir.c_str(), szToken, op, param, bRecurse) )
			{
				bFail = true;
				break;
			}
		}
		else
		{
			AString tempFile = AString(szFolder) + "\\" + F_UnicodeToUTF8(data.cFileName);
			if( !op(tempFile, param) )
			{
				bFail = true;
				break;
			}
		}

		bFound = FindNextFileW(hFind, &data);
	}

	FindClose(hFind);
	return !bFail;
}

// Show a system message box.
EMsgBoxReturn FSysAPI::ShowMsgBox(const char* szMsg, const char* szTitle, uint32 mbType /* = MSGBOX_OK */)
{
	UINT nMBType = MB_OK;
	if( mbType & MSGBOX_OKCANCEL )
		nMBType = MB_OKCANCEL;
	else if( mbType & MSGBOX_YESNO )
		nMBType = MB_YESNO;

	if( mbType & MSGBOX_ERROR )
		nMBType |= MB_ICONERROR;
	else if( mbType & MSGBOX_WARNING )
		nMBType |= MB_ICONWARNING;
	else if( mbType & MSGBOX_INFO )
		nMBType |= MB_ICONINFORMATION;

	UINT nMBRet = ::MessageBoxW(NULL, F_UTF8ToUnicode(szMsg).c_str(), F_UTF8ToUnicode(szTitle).c_str(), nMBType);
	if( nMBRet == IDOK )
		return MSGBOX_RET_OK;
	else if( nMBRet == IDCANCEL )
		return MSGBOX_RET_CANCEL;
	else if( nMBRet == IDYES )
		return MSGBOX_RET_YES;
	else if( nMBRet == IDNO )
		return MSGBOX_RET_NO;

	return MSGBOX_RET_OTHER;
}

// Output the debug string to IDE.
void FSysAPI::OutputDebugInfo(const char* szMsg, ...)
{
	char szBuf[1024];

	va_list argList;
	va_start(argList, szMsg);
	vsnprintf(szBuf, 1024, szMsg, argList);
	va_end(argList);

	strcat(szBuf, "\n");
	::OutputDebugStringW(F_UTF8ToUnicode(szBuf).c_str());
}

// Get the current tick count.
uint32 FSysAPI::GetMilliSecond()
{
	return (uint32)(GetMicroSecond() / 1000);
}

// Get the current tick count.
uint64 FSysAPI::GetMicroSecond()
{
	static LARGE_INTEGER frequency;
	static bool bFirstTime = true;

	if (bFirstTime)
	{
		::QueryPerformanceFrequency(&frequency);
		bFirstTime = false;
	}

	LARGE_INTEGER counter;
	::QueryPerformanceCounter(&counter);

	uint64 nSecond = counter.QuadPart / frequency.QuadPart;
	uint64 nRemainder = counter.QuadPart % frequency.QuadPart;
	return nSecond * 1000000 + nRemainder * 1000000 / frequency.QuadPart;
}

// Exit the process.
void FSysAPI::Exit()
{
	::TerminateProcess(::GetCurrentProcess(), -1);
}

///////////////////////////////////////////////////////////////////////////
//  
//  Debug Symbol Dump
//  
///////////////////////////////////////////////////////////////////////////

struct SymbolInfo
{
	const void* pAddr;
	char szName[128];
	char szFileName[MAX_PATH];
	uint32 nLineNumber;
};

// Load the debug symbols for current process.
static void* _LoadSymbolForCurProcess(const char* szSearchFolder)
{
	static void* hProcess;
	static char szLastPath[MAX_PATH];

	if( !hProcess || (szSearchFolder && strncmp(szLastPath, szSearchFolder, MAX_PATH-1)) )
	{
		if( szSearchFolder )
			strncpy(szLastPath, szSearchFolder, MAX_PATH-1);
		else
			szLastPath[0] = '\0';
		szLastPath[MAX_PATH-1] = '\0';

		hProcess = GetCurrentProcess();
		DWORD dwOldFlag = SymGetOptions();
		DWORD dwFlags = SymSetOptions(dwOldFlag | SYMOPT_LOAD_LINES);
		DWORD dwError = GetLastError();
		SymInitialize(hProcess, (char*)szSearchFolder, TRUE);
		return hProcess;
	}

	return hProcess;
}

// Get the debug symbol by a specified address.
static SymbolInfo _GetSymbolFromAddress(const void* pAddr, HANDLE hProcess)
{
	SymbolInfo info;
	memset(&info, 0, sizeof(info));
	info.pAddr = pAddr;

	DWORD error;
	DWORD64 dwAddress = (DWORD64)pAddr;
	ULONG64 buffer[(sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR) + sizeof(ULONG64) - 1) / sizeof(ULONG64)];
	PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;
	pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	pSymbol->MaxNameLen = MAX_SYM_NAME;

	DWORD64 dw64Offset;
	if( SymFromAddr(hProcess, dwAddress, &dw64Offset, pSymbol) )
		strncpy(info.szName, pSymbol->Name, sizeof(info.szName));
	else
		error = GetLastError();

	IMAGEHLP_LINE line;
	line.SizeOfStruct = sizeof(IMAGEHLP_LINE);
	DWORD dwOffset = (DWORD)dw64Offset;
	if( SymGetLineFromAddr(hProcess, dwAddress, &dwOffset, &line) )
	{
		strncpy(info.szFileName, line.FileName, sizeof(info.szFileName));
		info.nLineNumber = line.LineNumber;
	}
	else
	{
		error = GetLastError();
	}

	return info;
}

// Extract the symbol information by address.
void FSysAPI::ExportSymbolInfo(void* pAddr, char* szMsg)
{
	char szFileName[MAX_PATH];
	GetModuleFileNameA(NULL, szFileName, sizeof(szFileName));

	LPSTR pStr = PathFindFileNameA(szFileName);
	if( pStr ) *pStr = '\0';
	strcat(szFileName, "pdb\\");

	HANDLE hProcess = _LoadSymbolForCurProcess(szFileName);
	DWORD dwError = GetLastError();
	SymbolInfo si = _GetSymbolFromAddress(pAddr, hProcess);

	if( si.nLineNumber == 0 )
		sprintf(szMsg, "%s -- 0x%p", si.szName, si.pAddr);
	else
		sprintf(szMsg, "%s (%d): %s -- 0x%p", si.szFileName, si.nLineNumber, si.szName, si.pAddr);
}

// Sleep current thread.
void FSysAPI::Sleep(uint32 nMilliSeconds)
{
	::Sleep(nMilliSeconds);
}