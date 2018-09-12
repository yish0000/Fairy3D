/*
 * ------------------------------------------------------------------------
 *  Name:   FPerformance.h
 *  Desc:   Utility class to check the performance.
 *  Author: Yish
 *  Date:   2011/12/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_PERFORMANCE_H__
#define __FAIRY_PERFORMANCE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

// Callback function to output the performance information.
typedef void (*CB_OUTPUTPERFORMANCE) (int id, const AString& name, int64 iiElapsedTime);

/** Performance checker.
*/
class FPerformance
{
    struct _Item
    {
        AString name;		// Name of the monitor point.
        int64 iiStartCnt;   // Start time.
		int64 iiCount;      // Elapsed Time. (Microsecond)
    };

    typedef std::map<int, _Item> PerformanceMap;

public:
    FPerformance();
    virtual ~FPerformance();

    // Register the performance monitor point.
    bool RegisterPerf( int id, const char* name );
    void UnregisterPerf( int id );

	// Record the begin time and end time.
    void BeginPerformance( int id );
    void EndPerformance( int id );
    void ResetRecord();

	// Get the elapsed time of the specified monitor point.
	int64 GetPerformance(int id) const;
	// Output the performance informations.
	void OutputPerformance(CB_OUTPUTPERFORMANCE pFunc);

	// Get the singleton class.
	static FPerformance& GetInstance();

private:
	PerformanceMap m_Perfs;
};

// Wrapper class to check the performance.
class FPerformanceWrapper
{
public:
    FPerformanceWrapper(int id);
    ~FPerformanceWrapper();

    void BeginPerformance();
    void EndPerformance();

protected:
    int m_iPerfId;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_PERFORMANCE_H__