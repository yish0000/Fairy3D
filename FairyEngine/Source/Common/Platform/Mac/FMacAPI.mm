/*
 * ------------------------------------------------------------------------
 *  Name:   FMacAPI.mm
 *  Desc:   Implement the system API for Mac.
 *  Author: Yish
 *  Date:   2013/11/1
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FSysAPI.h"
#include "FStrUtil.h"

#include <unistd.h>
#include <sys/stat.h>
#include <glob.h>
#include <sys/time.h>

#import <Foundation/Foundation.h>

#if FAIRY_PLATFORM == FAIRY_PLATFORM_IOS
    #import <UIKit/UIKit.h>
#else
    #import <AppKit/AppKit.h>
#endif

///////////////////////////////////////////////////////////////////////////
//// Some local functions.

NSString* CString2NSString(const char* str)
{
    size_t len = strlen(str);
    NSString* outStr = [[NSString alloc] initWithBytes:str length:len encoding:NSUTF8StringEncoding];
    return outStr;
}

AString NSString2CString(NSString* str)
{
    if( str == nil )
        return "";
    return [str UTF8String];
}

///////////////////////////////////////////////////////////////////////////

// Access the specified file.
int FSysAPI::AccessFile(const char *filename, int mode)
{
    return access(filename, mode);
}

// Check whether the file exist.
bool FSysAPI::IsFileExist(const char* filename)
{
    if( access(filename, 0) != 0 )
        return false;
    else
        return true;
}

// Get the time stamp of the file.
time_t FSysAPI::GetFileTimeStamp(const char* filename)
{
    struct stat st;
    stat(filename, &st);
    return st.st_mtime;
}

// Get the size of the file.
uint32 FSysAPI::GetFileSize(const char* filename)
{
    struct stat st;
    stat(filename, &st);
    return (uint32)st.st_size;
}

// Whether the specified file is a directory.
bool FSysAPI::IsDirectory(const char* filename)
{
    struct stat st;
    stat(filename, &st);
    return S_ISDIR(st.st_mode);
}

// Whether the specified target is a normal file.
bool FSysAPI::IsNormalFile(const char* filename)
{
    struct stat st;
    stat(filename, &st);
    return S_ISREG(st.st_mode);
}

// Move the file to another place.
bool FSysAPI::FileMove(const char* filename, const char* dest)
{
    if( !FileCopy(filename, dest, true) )
        return false;
    
    if( !RemoveFile(filename) )
        return false;
    
    return true;
}

// Copy the specified file.
bool FSysAPI::FileCopy(const char* filename, const char* dest, bool bCover /* = false */)
{
    const int BUFSIZE = 1024;
    
    FILE* pSrc = NULL;
    FILE* pDest = NULL;
    char buffer[BUFSIZE];
    size_t bytes_read = 0;
    size_t bytes_write = 0;
    
    if( !bCover && access(dest, 0) == 0 )
        return false;
    
    // Open the source file.
    pSrc = fopen(filename, "r");
    if( !pSrc ) return false;
    
    // Create the dest file.
    pDest = fopen(filename, "wb");
    if( !pDest )
    {
        fclose(pSrc);
        return false;
    }
    
    while( (bytes_read = fread(buffer, 1, BUFSIZE, pSrc)) )
    {
        if( bytes_read == -1 && errno == EINTR )
            break;
        else if( bytes_read == 0 )
            break;
        else
        {
            bytes_write = fwrite(buffer, 1, BUFSIZE, pDest);
            FASSERT(bytes_write == bytes_read);
        }
    }
    
    fclose(pSrc);
    fclose(pDest);
    return true;
}

// Remove the specified file.
bool FSysAPI::RemoveFile(const char* filename)
{
    if( remove(filename) == -1 )
        return false;
    
    return true;
}

// Rename the specified file.
bool FSysAPI::Rename(const char* oldfile, const char* newfile)
{
    if( rename(oldfile, newfile) == -1 )
        return false;
    
    return true;
}

// Create a specified folder.
bool FSysAPI::CreateFolder(const char* szFolder, bool bRecurse /* = true */)
{
    if( bRecurse )
    {
        AString strDir = szFolder;
        std::replace(strDir.begin(), strDir.end(), '\\', '/');
        
        if( strDir.back() == '/' )
            strDir.pop_back();
        
        AString strTemp;
        size_t nPos = strDir.find_first_of('/', 0);
        while( nPos != AString::npos )
        {
            strTemp = strDir.substr(0, nPos);
            mkdir(strTemp.c_str(), S_IRWXU);
            nPos = strDir.find_first_of('/', nPos + 1);
        }
        
        mkdir(szFolder, S_IRWXU);
        return true;
    }
    else
    {
        return mkdir(szFolder, S_IRWXU) != -1;
    }
}

// Delete a specified folder.
bool FSysAPI::DeleteFolder(const char* szFolder)
{
    if( !FSysAPI::IsFileExist(szFolder) )
        return false;
    
    size_t strLength = strlen(szFolder);
    
    if( szFolder[strLength-2] == '.' && szFolder[strLength-1] == '/' )
        return false;
    
    glob_t globbuf;
    struct stat fileinfo;
    AString path(szFolder);
    
    if( path[path.length() - 1] == '/' )
    {
        path += "*";
    }
    else
    {
        path += "/*";
    }
    
    int ret = glob(path.c_str(), GLOB_NOSORT, NULL, &globbuf);
    if( ret != 0 )
    {
        if( ret == GLOB_NOMATCH )
        {
            rmdir(szFolder);
            globfree(&globbuf);
            return true;
        }
        
        globfree(&globbuf);
        return false;
    }
    
    path = szFolder;
    if( path[path.length() - 1] == '/' )
    {
        path += ".*";
    }
    else
    {
        path += "/.*";
    }
    
    ret = glob(path.c_str(), GLOB_APPEND, NULL, &globbuf);
    if( ret != 0 )
    {
        if( ret == GLOB_NOMATCH )
        {
            globfree(&globbuf);
            return true;
        }

        globfree(&globbuf);
        return false;
    }
    
    for(int i=0; i < globbuf.gl_pathc; ++i)
    {
        ret = lstat(globbuf.gl_pathv[i], &fileinfo);
        if( 1 == S_ISDIR(fileinfo.st_mode) )
        {
            if( strstr( globbuf.gl_pathv[i],".*") != NULL )
                continue;
            
            if( strstr( globbuf.gl_pathv[i],"./") != NULL )
                continue;
            
            if( strstr( globbuf.gl_pathv[i],"/.") != NULL )
                continue;
            
            DeleteFolder(globbuf.gl_pathv[i]);
        }
        else
        {
            chmod(globbuf.gl_pathv[i], S_IRWXU);
            RemoveFile(globbuf.gl_pathv[i]);
        }
    }
    
    globfree(&globbuf);
    
    if( rmdir(szFolder) == 0 )
        return true;

    return false;
}

// Get the current directory.
AString FSysAPI::GetCurrentDir()
{
    char cur_path[MAX_PATH];
    getcwd(cur_path, MAX_PATH);
    return cur_path;
}

// Get the document directory. (Writable for IOS)
AString FSysAPI::GetDocumentDir()
{
	NSString* homePath = NSHomeDirectory();
	NSString* docDir = @"/Documents";
	NSString* strDocDir = [homePath stringByAppendingString: docDir];
	return [strDocDir UTF8String];
}

// Search files from the specified folder.
bool FSysAPI::SearchFile(const char* szFolder, const char* szToken, AStringList& fileList, bool bRecurse)
{
    glob_t globbuf;
    struct stat fileinfo;
    AString path(szFolder);
    
    if( path[path.length() - 1] == '/' )
    {
        path += szToken;
    }
    else
    {
        path += "/";
        path += szToken;
    }
    
    int ret = glob(path.c_str(), GLOB_NOSORT, NULL, &globbuf);
    if( ret != 0 )
    {
        if( ret == GLOB_NOMATCH )
        {
            globfree(&globbuf);
            return true;
        }
        
        globfree(&globbuf);
        return false;
    }
    
    for(size_t i=0; i < globbuf.gl_pathc; ++i)
    {
        ret = lstat(globbuf.gl_pathv[i],&fileinfo);
        if( 1 == S_ISDIR(fileinfo.st_mode)  )
        {
            if( !bRecurse ) continue;
            SearchFile(globbuf.gl_pathv[i], szToken, fileList, bRecurse);
        }
        else
        {
            fileList.push_back(globbuf.gl_pathv[i]);
        }
    }

    globfree(&globbuf);
    return true;
}

// Search files from the specified folder.
bool FSysAPI::SearchFile(const char* szFolder, const char* szToken, CB_ENUMFILEOP op, void* param, bool bRecurse)
{
    glob_t globbuf;
    struct stat fileinfo;
    AString path(szFolder);
    
    if( !op ) return false;
    
    if( path[path.length() - 1] == '/' )
    {
        path += szToken;
    }
    else
    {
        path += "/";
        path += szToken;
    }
    
    int ret = glob(path.c_str(), GLOB_NOSORT, NULL, &globbuf);
    if( ret != 0 )
    {
        if( ret == GLOB_NOMATCH )
        {
            globfree(&globbuf);
            return true;
        }
        
        globfree(&globbuf);
        return false;
    }
    
    bool bFail = false;
    for(size_t i=0; i < globbuf.gl_pathc; ++i)
    {
        ret = lstat(globbuf.gl_pathv[i],&fileinfo);
        if( 1 == S_ISDIR(fileinfo.st_mode)  )
        {
            if( !bRecurse ) continue;
            if( !SearchFile(globbuf.gl_pathv[i], szToken, op, param, bRecurse) )
            {
                bFail = true;
                break;
            }
        }
        else
        {
            if( !op(globbuf.gl_pathv[i], param) )
            {
                bFail = true;
                break;
            }
        }
    }
    
    globfree(&globbuf);
    return !bFail;
}

// Output the debug message to IDE.
void FSysAPI::OutputDebugInfo(const char* szMsg, ...)
{
    char szBuf[1024];
    
    va_list argList;
    va_start(argList, szMsg);
    vsnprintf(szBuf, 1024, szMsg, argList);
    va_end(argList);
    
    NSString* nsStr = CString2NSString(szBuf);
    NSLog(nsStr, nil);
}

// Get the current tick count.
uint32 FSysAPI::GetMilliSecond()
{
	timeval tv;
	gettimeofday(&tv, NULL);
	uint64 nTime = ((uint64)tv.tv_sec * 1000000 + tv.tv_usec) / 1000;
	return (uint32)nTime;
}

// Get the current tick count.
uint64 FSysAPI::GetMicroSecond()
{
	timeval tv;
	gettimeofday(&tv, NULL);
	return (uint64)tv.tv_sec * 1000000 + tv.tv_usec;
}

///////////////////////////////////////////////////////////////////////////
//
//  Debug Symbol Dump
//
///////////////////////////////////////////////////////////////////////////

// Get the symbol information of specified address.
void FSysAPI::ExportSymbolInfo(void* pAddr, char* szMsg)
{
}

// Sleep some time.
void FSysAPI::Sleep(uint32 nMilliSeconds)
{
    ::usleep(nMilliSeconds * 1000);
}

///////////////////////////////////////////////////////////////////////////
//
//  Show the system message box.
//
///////////////////////////////////////////////////////////////////////////

#if FAIRY_PLATFORM == FAIRY_PLATFORM_IOS

@interface DialogUIAlertView : NSObject<UIAlertViewDelegate> {
    BOOL isWaiting4Tap;
    UIAlertView* alv;
    int alertViewRetValue;
}
- (id)initWithTitle:(NSString *)title message:(NSString *)message cancelButtonTitle:(NSString *)cancelButtonTitle otherButtonTitles:(NSString *)otherButtonTitles, ...;
- (int)showDialog:(int)timeout;
@end

@interface UIAlertView(Synthesize)
-(void)setRunsModal:(BOOL)modal;
@end

@implementation DialogUIAlertView
- (id)initWithTitle:(NSString *)title message:(NSString *)message cancelButtonTitle:(NSString *)cancelButtonTitle otherButtonTitles:(NSString *)otherButtonTitles, ...
{
    alv = [[UIAlertView alloc] initWithTitle:title message:message delegate:self cancelButtonTitle:cancelButtonTitle otherButtonTitles:otherButtonTitles,nil];
    isWaiting4Tap = YES;
    return self;
}
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    alertViewRetValue = buttonIndex;
    isWaiting4Tap = NO;
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    alertViewRetValue = buttonIndex;
    isWaiting4Tap = NO;
}

- (int)showDialog:(int)timeout
{
    uint32 starttime = FSysAPI::GetMilliSecond();
    isWaiting4Tap = YES;

    if( [NSRunLoop mainRunLoop] != [NSRunLoop currentRunLoop])
    {
        [alv performSelectorOnMainThread:@selector(show) withObject:nil waitUntilDone:YES];
    }
    else
    {
        [alv show];
    }
    
    while (isWaiting4Tap)
    {
        BOOL ret = [[NSRunLoop mainRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
        uint32 time = FSysAPI::GetMilliSecond() - starttime;

        if( time > timeout )
        {
            [alv dismissWithClickedButtonIndex:0 animated:YES];
            break; // Timeout, we will break the loop.
        }
    }
    
    // [ alv setRunsModal:YES];
    alv.delegate = nil;
    return alertViewRetValue;
}

- (void)dealloc {
    //[super dealloc];
    //[alv release];
}
@end

// Alert a system message box.
EMsgBoxReturn FSysAPI::ShowMsgBox(const char* szMsg, const char* szTitle, uint32 mbType /* = MSGBOX_OK */)
{
    static const int TIMEOUT = 60;
    
    NSString* strText = CString2NSString(szMsg);
    NSString* strTitle = CString2NSString(szTitle);
    NSString* strCancelButton = @"OK";
    NSString* strOtherButton = nil;
    
    if( mbType == MSGBOX_OKCANCEL )
    {
        strOtherButton = @"Cancel";
    }
    else if( mbType == MSGBOX_YESNO )
    {
        strCancelButton = @"YES";
        strOtherButton = @"NO";
    }
    
    DialogUIAlertView* alert = [[DialogUIAlertView alloc] initWithTitle:strTitle message:strText cancelButtonTitle:strCancelButton otherButtonTitles:strOtherButton, nil];
    int ret = [alert showDialog:TIMEOUT];
    if( ret == 0 )
    {
        if( mbType == MSGBOX_OKCANCEL )
            return MSGBOX_RET_OK;
        else if( mbType == MSGBOX_YESNO )
            return MSGBOX_RET_YES;
        else
            return MSGBOX_RET_OK;
    }
    else if( ret == 1 )
    {
        if( mbType == MSGBOX_OKCANCEL )
            return MSGBOX_RET_CANCEL;
        else if( mbType == MSGBOX_YESNO )
            return MSGBOX_RET_NO;
    }

    return MSGBOX_RET_OTHER;
}

#else

// Alert a system message box.
EMsgBoxReturn FSysAPI::ShowMsgBox(const char* szMsg, const char* szTitle, uint32 mbType /* = MSGBOX_OK */)
{
    NSString* strText = CString2NSString(szMsg);
    NSString* strTitle = CString2NSString(szTitle);
    NSString* strDefaultButton = @"OK";
    NSString* strCancelButton = nil;
    NSString* strOtherButton = nil;
    
    if( mbType == MSGBOX_OKCANCEL )
    {
        strCancelButton = @"Cancel";
    }
    else if( mbType == MSGBOX_YESNO )
    {
        strDefaultButton = @"YES";
        strCancelButton = @"NO";
    }
    
    NSAlert *alert = [NSAlert alertWithMessageText:strTitle
                                     defaultButton:strDefaultButton
                                   alternateButton:strCancelButton
                                       otherButton:strOtherButton
                         informativeTextWithFormat:strText, nil];
    NSUInteger action = [alert runModal];
    if(action == NSAlertDefaultReturn)
    {
        if( mbType == MSGBOX_OKCANCEL )
            return MSGBOX_RET_OK;
        else if( mbType == MSGBOX_YESNO )
            return MSGBOX_RET_YES;
        else
            return MSGBOX_RET_OK;
    }
    else if(action == NSAlertAlternateReturn )
    {
        if( mbType == MSGBOX_OKCANCEL )
            return MSGBOX_RET_CANCEL;
        else if( mbType == MSGBOX_YESNO )
            return MSGBOX_RET_NO;
    }
    
    return MSGBOX_RET_OTHER;
}

#endif