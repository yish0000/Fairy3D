/*
 * ------------------------------------------------------------------------
 *  Name:   FVFilePack.cpp
 *  Desc:   This file define the base class for file package.
 *  Author: Yish
 *  Date:   2010/12/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FVFilePack.h"
#include "FVFileSystem.h"


/** Constructor.
*/
FVFilePack::FVFilePack( const AString& name,const AString& packType )
    : m_sName(name),m_sType(packType),m_bReadonly(false),m_bDirty(false)
{
}

/** Destructor.
*/
FVFilePack::~FVFilePack(void)
{
}