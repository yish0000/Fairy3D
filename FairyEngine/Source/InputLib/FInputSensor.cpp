/*
 * ------------------------------------------------------------------------
 *  Name:   FInputSensor.cpp
 *  Desc:   This file define a base class for Sensor device of mobile.
 *  Author: Yish
 *  Date:   2015/2/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FInputSensor.h"

/** Constructor.
*/
FInputSensor::FInputSensor() : FInputDevice(IDT_SENSOR)
{
}

/** Destructor.
*/
FInputSensor::~FInputSensor()
{
}

/** Initialize the Sensor device for mobiles.
*/
bool FInputSensor::Init()
{
	return true;
}

/** Update the Sensor device for mobiles.
*/
void FInputSensor::Update()
{
}