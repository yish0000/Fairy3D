/*
 * ------------------------------------------------------------------------
 *  Name:   FVFileStream.cpp
 *  Desc:   This file define the base class of the file stream.
 *  Author: Yish
 *  Date:   2010/10/11
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FVFileStream.h"
#include "FStrUtil.h"
#include "FTemplate.h"

/** Default constructor.
*/
FVFileStream::FVFileStream(void)
	: m_nStreamSize(0)
{
}

/** Constructor.
@Param Name of the file stream.
*/
FVFileStream::FVFileStream( const AString& name )
    : m_sName(name),m_nStreamSize(0)
{
}

/** Destructor.
*/
FVFileStream::~FVFileStream(void)
{
}

/** 读取文件流中的一行字符串
@Param 存储字符串的缓冲区
@Param 字符串的最大长度
@Param 字符串换行的分隔符
@Return 本次调用所读取的数据量
*/
size_t FVFileStream::ReadLine( char* pBuf, size_t maxLen, const AString& delimiter/* = */ )
{
    const size_t STREAM_BLOCK_SIZE = 128;

    size_t totalSize = 0;
    char tempBuf[STREAM_BLOCK_SIZE];

    bool trimCR = false;
    if( delimiter.find_first_of('\n') != AString::npos )
        trimCR = true;

    size_t readCount;
    size_t chunkSize = F_Min( maxLen,STREAM_BLOCK_SIZE-1 );
    while( chunkSize && (readCount = Read(tempBuf,chunkSize)) )
    {
        tempBuf[readCount] = '\0';

        // 查找分隔符
        size_t pos = strcspn( tempBuf,delimiter.c_str() );

        if( pos < readCount )
        {
            // 回卷多读的数据
            Skip( (long)(pos + 1 - readCount) );
        }

        memcpy( pBuf+totalSize,tempBuf,pos );
        totalSize += pos;

        if( pos < readCount )
        {
            // 删除换行符\r
            if( trimCR && totalSize && pBuf[totalSize-1] == '\r' )
                totalSize--;

            break;
        }

        chunkSize = F_Min( maxLen-totalSize,STREAM_BLOCK_SIZE-1 );
    }

    pBuf[totalSize] = '\0';

    return totalSize;
}

/** 跳过文件流的当前行字符串
@Param 字符串换行的分隔符
@Return 本次调用所跳过的数据量
*/
size_t FVFileStream::SkipLine( const AString& delimiter/* = */ )
{
    const size_t STREAM_BLOCK_SIZE = 128;

    size_t totalSize = 0;
    char tempBuf[STREAM_BLOCK_SIZE];

    size_t readCount;
    while( (readCount = Read(tempBuf,STREAM_BLOCK_SIZE-1)) )
    {
        tempBuf[readCount] = '\0';

        size_t pos = strcspn( tempBuf,delimiter.c_str() );
        if( pos < readCount )
        {
            // 回卷多读的数据
            Skip( (long)(pos + 1 - readCount) );
            totalSize += pos + 1;
            break;
        }

        totalSize += readCount;
    }

    return totalSize;
}

/** 读取文件流中的一行字符串，必须以'\n'为分隔符
@Param 是否将字符串后面的空格清除
@Return 本次调用所读取的数据量
*/
size_t FVFileStream::GetLine( AString& retStr, bool bTrimLine/* =true */ )
{
    const size_t STREAM_BLOCK_SIZE = 128;

    size_t readCount = 0;
    char tempBuf[STREAM_BLOCK_SIZE];
    retStr = "";

    size_t i;
    while( (i = Read(tempBuf,STREAM_BLOCK_SIZE-1)) != 0 )
    {
        readCount += i;
        tempBuf[i] = '\0';

        char* p = strchr( tempBuf,'\n' );
        if( p != NULL )
        {
            // 回卷多读的数据
            Skip( (long)(p+1-tempBuf-readCount) );
            readCount += (long)(p+1-tempBuf-readCount);
            *p = '\0';
        }

        retStr += tempBuf;

        if( p != NULL )
        {
            // 将转行的字符\r删除
            if( retStr.length() && retStr[retStr.length()-1] == '\r' )
            {
                retStr.erase( retStr.length()-1,1 );
            }

            break;
        }
    }

    // 将字符串中的空格去掉
    if( bTrimLine )
    {
        AStringUtil::Trim( retStr );
    }

    return readCount;
}