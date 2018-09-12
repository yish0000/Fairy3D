//
//  FInputSensorIOS.mm
//  InputLib
//
//  Created by Yish on 16/7/20.
//  Copyright © 2016年 YishSoft. All rights reserved.
//

#include "FInputSensor.h"

#import <Foundation/Foundation.h>

class FInputSensorIOS : public FInputSensor
{
public:
    FInputSensorIOS() {}
    
    // Initialize the touch device.
    bool Init()
    {
        if( !FInputSensor::Init() )
            return false;
        
        m_bValid = true;
        return true;
    }
    
    void Update()
    {
    }
};

template <> FInputDevice* CreateInputDevice<IDT_SENSOR>()
{
    return new FInputSensorIOS();
}