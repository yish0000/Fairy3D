/*
 * ------------------------------------------------------------------------
 *  Name:   FNStreamIO.h
 *  Desc:   ���ļ�������������TCP���ӵ�IO�����
 *  Author: Yish
 *  Date:   2012/3/4
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FNET_STREAMIO_H__
#define __FNET_STREAMIO_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FNPollIO.h"
#include "FNSession.h"

///////////////////////////////////////////////////////////////////////////


namespace FNET {

    /** �������ӵ�IO���
    */
    class _FNET_EXPORT StreamIO : public ConnectedIO
    {
    public:
        StreamIO( SOCKET sk,Session* s );
        virtual ~StreamIO();

    protected:
        int UpdateEvent();
        void PollIn();
        void PollOut();
    };
}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FNET_STREAMIO_H__