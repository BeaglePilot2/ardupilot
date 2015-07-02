// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

/// @file	AP_MotorsSub.h
/// @brief	Motor control class for Submarine

#ifndef __AP_MOTORS_SUB_H__
#define __AP_MOTORS_SUB_H__

#include <AP_Common.h>
#include <AP_Math.h>        // ArduPilot Mega Vector/Matrix math Library
#include <RC_Channel.h>     // RC Channel Library
#include "AP_MotorsMatrix.h"    // Parent Motors Matrix library

/// @class      AP_MotorsSub
class AP_MotorsSub : public AP_MotorsMatrix {
public:

    /// Constructor
    AP_MotorsSub(uint16_t loop_rate, uint16_t speed_hz = AP_MOTORS_SPEED_DEFAULT) :
        AP_MotorsMatrix(loop_rate, speed_hz)
    { };

    // setup_motors - configures the motors for a submarine
    virtual void        setup_motors();

protected:

};

#endif  // AP_MOTORSSUB

