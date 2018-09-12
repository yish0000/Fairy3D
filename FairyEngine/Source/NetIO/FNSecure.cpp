/*
 * ------------------------------------------------------------------------
 *  Name:   FNSecure.cpp
 *  Desc:   ���ļ�ʵ�������������ݼ��ܹ��ܡ�
 *  Author: Yish
 *  Date:   2012/3/5
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include <time.h>
#include "FNSecure.h"
#include "FCRC32.h"
#include "FMD5.h"


namespace FNET {

    // ���徲̬����
    Secure::SecureMap Secure::map;

    ///////////////////////////////////////////////////////////////////////////
    // ��������

    bool RandomSecure::Init()
    {
        srand( time(0) );
        return true;
    }

    Octets& RandomSecure::Update( Octets& o )
    {
        for( size_t i=0;i<o.size();++i )
        {
            ((uchar*)o.begin())[i] = rand() & 0xff;
        }

        return o;
    }

    ///////////////////////////////////////////////////////////////////////////
    // MD5��ϣ���ܷ�
}