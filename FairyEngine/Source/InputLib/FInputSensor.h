/*
 * ------------------------------------------------------------------------
 *  Name:   FInputSensor.h
 *  Desc:   This file define a base class for the sensors of mobile devices
 *  Author: Yish
 *  Date:   2015/2/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_INPUT_SENSOR_H__
#define __FAIRY_INPUT_SENSOR_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FInputDevice.h"

///////////////////////////////////////////////////////////////////////////

/** Sensor device for mobiles.
*/
class FInputSensor : public FInputDevice
{
public:
	enum EInputSensorType
	{
		SENSOR_ROTATION_VECTOR,
		SENSOR_LINEAR_ACCELERATION,
		SENSOR_PRESSURE,
		SENSOR_STEP_DETECTOR,
	};

public:
	FInputSensor();
	virtual ~FInputSensor();

	// Initialize the sensor device.
	virtual bool Init();
	// Update the sensor device.
	virtual void Update();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_INPUT_SENSOR_H__