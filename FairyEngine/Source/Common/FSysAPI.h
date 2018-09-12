/*
 * ------------------------------------------------------------------------
 *  Name:   FSysAPI.h
 *  Desc:   This file define some platform-related functions for engine.
 *  Author: Yish
 *  Date:   2013/11/1
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_SYSAPI_H__
#define __FAIRY_SYSAPI_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

enum EMsgBoxType
{
	MSGBOX_OK		= 0x01,
	MSGBOX_OKCANCEL	= 0x02,
	MSGBOX_YESNO	= 0x04,

	MSGBOX_ERROR	= 0x10,
	MSGBOX_WARNING	= 0x20,
	MSGBOX_INFO		= 0x40,
};

enum EMsgBoxReturn
{
	MSGBOX_RET_OK,
	MSGBOX_RET_CANCEL,
	MSGBOX_RET_YES,
	MSGBOX_RET_NO,

	MSGBOX_RET_OTHER,
};

class FSysAPI
{
public:
    
    typedef bool (*CB_ENUMFILEOP) (const AString& path, void* param);

public:
    // Get the current directory.
    static AString GetCurrentDir();
    // Get the document directory. (Writable for IOS)
    static AString GetDocumentDir();
    // Get the cache directory.
    static AString GetCacheDir();
    // Get the temp directory.
    static AString GetTempDir();
	// Get the name of the package.
	static AString GetPackageName();
	// Get the version of the application.
	static AString GetVersion();
	// Is the device jail break ?
	static bool IsJailBreak();
	// Get the version of the OS.
	static AString GetOSVersion();

    // Access the specified file.
    static int AccessFile(const char* filename, int mode);
	// Is the specified file existed ?
	static bool IsFileExist(const char* filename);
	// Is the specified file readonly ?
	static bool IsFileReadonly(const char* filename) { return AccessFile(filename, 2) == 0; }
	// Get the time stamp of the file.
	static time_t GetFileTimeStamp(const char* filename);
	// Get the size of the file.
	static uint32 GetFileSize(const char* filename);
	// Whether the specified file is a directory.
	static bool IsDirectory(const char* filename);
	// Whether the specified target is a normal file.
	static bool IsNormalFile(const char* filename);
	// Move a file
	static bool FileMove(const char* filename, const char* dest);
	// Copy a file
	static bool FileCopy(const char* filename, const char* dest, bool bCover = true);
	// Remove a file
	static bool RemoveFile(const char* filename);
	// Rename a file
	static bool Rename(const char* oldfile, const char* newfile);
	// Create a specified directory
	static bool CreateFolder(const char* szFolder, bool bRecurse = true);
	// Delete a specified directory
	static bool DeleteFolder(const char* szFolder);
	// Search files from the specified folder.
	static bool SearchFile(const char* szFolder, const char* szToken, AStringList& fileList, bool bRecurse = true);
    // Search files to do something.
    static bool SearchFile(const char* szFolder, const char* szToken, CB_ENUMFILEOP op, void* param, bool bRecurse = true);
    
    // Get the device dpi scale.
    static float GetDeviceDPIScale();

	// Show a system message box
	static EMsgBoxReturn ShowMsgBox(const char* szMsg, const char* szTitle, uint32 mbType = MSGBOX_OK);
	// Open a specified URL by the external explorer.
	static void OpenURL(const char* url);
	// Get the time zone.
	static int GetTimeZone();

	// Make the current thread sleep
	static void Sleep(uint32 nMilliSeconds);
	// Exit the process.
	static void Exit();
	// Get the current tick count.
	static uint32 GetMilliSecond();
	static uint64 GetMicroSecond();

	// Output debug message to IDE
	static void OutputDebugInfo(const char* szMsg, ...);
	// Export the symbol info from a specified address
	static void ExportSymbolInfo(void* pAddr, char* szMsg);
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_SYSAPI_H__