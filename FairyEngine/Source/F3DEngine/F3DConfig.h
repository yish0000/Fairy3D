/*
 * ------------------------------------------------------------------------
 *  Name:   F3DConfig.h
 *  Desc:   This file define the global config data for engine.
 *  Author: Yish
 *  Date:   2012/12/22
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_CONFIG_H__
#define __F3D_CONFIG_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////

/** Config data of the engine.
*/
class FAIRY_API F3DConfig
{
public:
	~F3DConfig() {}

    // Load the config data from file.
    bool LoadConfigFile( const char* filename );
    // Save the config data to file.
    void SaveConfigFile( const char* filename );

	static F3DConfig& GetInstance();

private:
	F3DConfig() {}
	F_DISABLE_COPY_ASSIGNMENT(F3DConfig);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_CONFIG_H__