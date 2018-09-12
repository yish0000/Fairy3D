/*
 * ------------------------------------------------------------------------
 *  Name:   CommandLine.h
 *  Desc:   ���ļ�������һ�������г��������ȡ�ࡣ
 *  Author: Yish
 *  Date:   2013/12/28
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __COMMAND_LINE_H__
#define __COMMAND_LINE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include <FBaseType.h>

///////////////////////////////////////////////////////////////////////////


class CommandLine
{
public:
	CommandLine(const AString& sep = "-");
	virtual ~CommandLine();

	bool Init(int argc, const char* argv[]);
	bool Init(const AString& cmdLine);

	bool HaveArgument(const AString& szKey);
	AString GetArgument(const AString& szKey);
	size_t GetArgCount() const { return m_Args.size(); }

protected:
	AString m_strSep;
	std::map<AString, AString> m_Args;
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __COMMAND_LINE_H__