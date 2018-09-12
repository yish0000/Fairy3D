/*
 * ------------------------------------------------------------------------
 *  Name:   FNTimer.h
 *  Desc:   ���ļ�������Ϊ����ⶨ����һ����ʱ����
 *  Author: Yish
 *  Date:   2012/3/10
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FNET_TIMER_H__
#define __FNET_TIMER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FNBase.h"
#include <time.h>

///////////////////////////////////////////////////////////////////////////


namespace FNET {

	/** ʱ�Ӷ���
	*/
	class _FNET_EXPORT Timer
	{
    public:

        /** ʱ�ӵļ�����
        */
        class _FNET_EXPORT Observer
        {
        public:
            virtual ~Observer() {}
            virtual void Update() = 0;
        };

        typedef std::vector<Observer*> ObserverList;

	public:
        Timer() : t(NOW) {}

        static void Attach( Observer* o )
        {
            Observers.push_back( o );
        }

		static void Update()
        {
            time_t ct = time(NULL);
            if( ct > NOW )
            {
                ObserverList::iterator it;
                for( it=Observers.begin();it!=Observers.end();++it )
                    (*it)->Update();
                NOW = ct;
            }
        }

        static time_t GetTime() { return NOW; }

        int Elapse() const { return NOW - t; }
        void Reset() { t = NOW; }

    private:
        time_t t;
        static ObserverList Observers;
        static time_t NOW;
	};
}

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FNET_TIMER_H__