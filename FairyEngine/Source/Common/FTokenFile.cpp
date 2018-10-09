/*
 * ------------------------------------------------------------------------
 *  Name:   FTokenFile.cpp
 *  Desc:   本文件实现了基于单词序列的简单脚本解析类。
 *  Author: Yish
 *  Date:   2011/11/27
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FTokenFile.h"
#include "FVFile.h"
#include "FLogManager.h"

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FTokenFile
//  
///////////////////////////////////////////////////////////////////////////

/** 构造函数
*/
FTokenFile::FTokenFile()
{
    m_szToken[0] = '\0';
    m_pInfo = NULL;
    m_iCurLine = 0;
}

/** 析构函数
*/
FTokenFile::~FTokenFile()
{
    Close();
}

/** 关闭文件
*/
void FTokenFile::Close()
{
    if( m_pInfo )
    {
        F_FREE( m_pInfo->pStart );
        delete m_pInfo;
		m_pInfo = NULL;
    }
}

/** 加载脚本文件
*/
bool FTokenFile::Load( const char* filename )
{
    // 关闭当前文件
    Close();

    FVFile file;
    if( !file.Open(filename,FVFile::VFILE_OPENEXIST) )
    {
        FLOG_WARNINGF( "FTokenFile::Load, Load token file(%s) failed!",filename );
        return false;
    }

	size_t nSize = file.GetFileSize();

	// 如果有BOM，则跳过
	if( file.HasUTF8BOM() )
	{
		file.Seek(3, FVFile::START);
		nSize -= 3;
	}

    m_pInfo = new TOKENINFO;
    m_pInfo->pStart = (char*)F_MALLOC( nSize+1 );
    file.Read( m_pInfo->pStart,nSize );
    file.Close();

    m_pInfo->pCur = m_pInfo->pStart;
    m_pInfo->pEnd = m_pInfo->pStart + nSize;
    *m_pInfo->pEnd = '\0';
    m_iCurLine = 1;

    return true;
}

/** 获取下一个单词
@Param 如果为true,则只读取本行的单词
*/
bool FTokenFile::GetNextToken( bool bCrossLine )
{
    if( !m_pInfo )
	{
		FASSERT(0);
		return false;
	}

    // 读取本行单词
    char* pDestChar = m_szToken;
    bool bTokenStart = false;   // 已经开始记录单词
    int iTokenSetFlag = 0;      // 在"",'',() 内部的单词
    bool bInComment = false;    // 进入注释段内部

    while( m_pInfo->pCur <= m_pInfo->pEnd )
    {
        char cur = *m_pInfo->pCur;
        if( IsSeperators(cur) )
        {
            if( cur == '\n' )
			{
				m_iCurLine++;
				if( !bTokenStart && !bCrossLine ) return false;
			}

            if( bTokenStart && !iTokenSetFlag )
            {
                *pDestChar++ = '\0';
                bTokenStart = false;
				m_pInfo->pCur++;
                return true;
            }
        }
        else if( cur == '\"' )
        {
            if( iTokenSetFlag == 1 )
            {
                if( bTokenStart )
                {
                    *pDestChar++ = '\0';
                    bTokenStart = false;
                    m_pInfo->pCur++;
                    return true;
                }
            }
            else if( !iTokenSetFlag && !bInComment )
            {
                iTokenSetFlag = 1;
                if( !bTokenStart ) bTokenStart = true;
				m_pInfo->pCur++;
            }
        }
        else if( cur == '\'' )
        {
            if( iTokenSetFlag == 2 )
            {
                if( bTokenStart )
                {
                    *pDestChar++ = '\0';
                    bTokenStart = false;
                    m_pInfo->pCur++;
                    return true;
                }
            }
            else if( !iTokenSetFlag && !bInComment )
            {
                iTokenSetFlag = 2;
                if( !bTokenStart ) bTokenStart = true;
				m_pInfo->pCur++;
            }
        }
        else if( cur == '(' )
        {
            if( !iTokenSetFlag && !bInComment )
            {
                iTokenSetFlag = 3;
                if( !bTokenStart ) bTokenStart = true;
				m_pInfo->pCur++;
            }
        }
        else if( cur == ')' )
        {
            if( iTokenSetFlag == 3 && bTokenStart )
            {
                *pDestChar++ = '\0';
                bTokenStart = false;
                m_pInfo->pCur++;
                return true;
            }
        }
        else if( cur == '/' )
        {
            if( !iTokenSetFlag && m_pInfo->pEnd - m_pInfo->pCur >= 2 )
            {
                bool bComment = false;
                char next = *(m_pInfo->pCur + 1);
                if( next == '/' )
                {
                    SkipLine();
                    if( !bTokenStart && !bCrossLine ) return false;
					bComment = true;
                }
                else if( next == '*' )
                {
                    bInComment = true;
                    m_pInfo->pCur += 2;
                    bComment = true;
                }

                if( bTokenStart && bComment )
                {
                    *pDestChar++ = '\0';
                    bTokenStart = false;
                }
				else if( !bComment && !bInComment )
					bTokenStart = true;
            }
        }
        else if( cur == '*' )
        {
            if( !iTokenSetFlag && m_pInfo->pEnd - m_pInfo->pCur >= 2 )
            {
                char next = *(m_pInfo->pCur + 1);
                if( next == '/' && bInComment )
                {
                    bInComment = false;
                    m_pInfo->pCur += 2;
					continue;
                }
				else if( !bInComment )
					bTokenStart = true;
            }
        }
		else if( cur == '\\' )
		{
			if( iTokenSetFlag && m_pInfo->pEnd - m_pInfo->pCur >= 2 )
				m_pInfo->pCur++;
			else if( !bInComment )
				bTokenStart = true;
		}
        else
        {
            if( !bInComment )
                bTokenStart = true;
        }

        if( bTokenStart )
        {
            if( iTokenSetFlag )
            {
                *pDestChar++ = *m_pInfo->pCur;
            }
            else
            {
                if( !IsSeperators(*m_pInfo->pCur) )
                    *pDestChar++ = *m_pInfo->pCur;
            }
        }

        m_pInfo->pCur++;
    }

    return false;
}

/** 获取下一个单词，但不移动指针
*/
bool FTokenFile::PeekNextToken( bool bCrossLine )
{
    bool bRet = false;
    if( m_pInfo )
    {
        char* pSave = m_pInfo->pCur;
        bRet = GetNextToken( bCrossLine );
        m_pInfo->pCur = pSave;
    }

    return bRet;
}

/** 跳过下一行
*/
void FTokenFile::SkipLine()
{
    if( !m_pInfo )
	{
		FASSERT(0);
		return;
	}

    char* pNewLine = strchr( m_pInfo->pCur,'\n' );
    if( pNewLine )
    {
        m_pInfo->pCur = pNewLine + 1;
        m_iCurLine++;
    }
}

/** 向下寻找符合的单词，未找到则返回false
@Param 指定的单?
@Param 是否大小写敏感
*/
bool FTokenFile::MatchToken( const char* szToken,bool bCaseSensitive )
{
    while( GetNextToken(true) )
    {
        if( bCaseSensitive )
        {
            if( !strcmp(m_szToken, szToken) )
                return true;
        }
        else
        {
            if( !strcmp_nocase(m_szToken, szToken) )
                return true;
        }
    }

    return false;
}

/** 是否到达文件末尾
*/
bool FTokenFile::IsEnd() const
{
    if( m_pInfo )
        return m_pInfo->pCur >= m_pInfo->pEnd;
	else
	{
		FASSERT(0);
		return true;
	}
}

/** 检测指定字符是否分隔符
*/
bool FTokenFile::IsSeperators( char ch )
{
    return (ch == ' ') || (ch == '\t') || (ch == '\n') || (ch == '\r') ||
        (ch == ',') || (ch == ';') || (ch == '\0');
}