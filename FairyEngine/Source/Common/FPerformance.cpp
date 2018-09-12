/*
 * ------------------------------------------------------------------------
 *  Name:   FPerformance.cpp
 *  Desc:   Utility class to check the performance.
 *  Author: Yish
 *  Date:   2011/12/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FPerformance.h"
#include "FSysAPI.h"
#include "FLogManager.h"

/** Constructor.
*/
FPerformance::FPerformance()
{
}

/** Destructor.
*/
FPerformance::~FPerformance()
{
}

/** Get the singleton instance.
*/
FPerformance& FPerformance::GetInstance()
{
	static FPerformance obj;
	return obj;
}

/** Register a performance monitor point.
*/
bool FPerformance::RegisterPerf( int id, const char* name )
{
    PerformanceMap::iterator it = m_Perfs.find( id );
	if (it != m_Perfs.end())
	{
		FLOG_WARNINGF("FPerformance::RegisterPerf, The specified performance monitor point has existed! (%d:%s)", id, name);
		return false;
	}

    _Item item;
    item.name = name;
    item.iiStartCnt = item.iiCount = 0;
    m_Perfs.insert(PerformanceMap::value_type(id, item));
    return true;
}

/** Remove the specified performance monitor point.
*/
void FPerformance::UnregisterPerf( int id )
{
    PerformanceMap::iterator it = m_Perfs.find(id);
    if( it != m_Perfs.end() )
		m_Perfs.erase(it);
}

/** Record the start time of specified monitor point.
*/
void FPerformance::BeginPerformance( int id )
{
    PerformanceMap::iterator it = m_Perfs.find(id);
    if( it != m_Perfs.end() )
		it->second.iiStartCnt = FSysAPI::GetMicroSecond();
}

/** Record the end time of specified monitor point.
*/
void FPerformance::EndPerformance( int id )
{
    PerformanceMap::iterator it = m_Perfs.find(id);
    if( it != m_Perfs.end() )
		it->second.iiCount += FSysAPI::GetMicroSecond() - it->second.iiStartCnt;
}

/** Output the performance informations.
*/
void FPerformance::OutputPerformance(CB_OUTPUTPERFORMANCE pFunc)
{
	if (!pFunc)
		return;

	PerformanceMap::iterator it = m_Perfs.begin();
	while (it != m_Perfs.end())
	{
		_Item& item = it->second;
		pFunc(it->first, item.name, item.iiCount);
		++it;
	}
}

///////////////////////////////////////////////////////////////////////////
// 
// Implement the class FPerformanceWrapper
// 
///////////////////////////////////////////////////////////////////////////

/** Constructor.
*/
FPerformanceWrapper::FPerformanceWrapper(int id)
{
    m_iPerfId = id;
    FPerformance::GetInstance().BeginPerformance(m_iPerfId);
}

/** Destructor.
*/
FPerformanceWrapper::~FPerformanceWrapper()
{
	FPerformance::GetInstance().EndPerformance(m_iPerfId);
}

/** Record the start time of specified monitor point.
*/
void FPerformanceWrapper::BeginPerformance()
{
	FPerformance::GetInstance().BeginPerformance(m_iPerfId);
}

/** Record the end time of specified monitor point.
*/
void FPerformanceWrapper::EndPerformance()
{
	FPerformance::GetInstance().EndPerformance(m_iPerfId);
}