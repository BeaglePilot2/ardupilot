// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *       AP_MotorsSub.cpp - APMsubmarine motors library
 *       Based on AP_MotorsQuad.cpp by RandyMackay. DIYDrones.com
 *
 */

#include "AP_MotorsSub.h"

// setup_motors - configures the motors for a submarine
void AP_MotorsSub::setup_motors()
{
    // call parent
    AP_MotorsMatrix::setup_motors();

    // hard coded config for OpenROV based frames
    add_motor_raw(AP_MOTORS_MOT_1,  0.0f,  0.0f, AP_MOTORS_MATRIX_YAW_FACTOR_CW,  1); //Port
    add_motor_raw(AP_MOTORS_MOT_2,  0.0f,  0.0f, AP_MOTORS_MATRIX_YAW_FACTOR_CCW, 2); //Starboard
    add_motor_raw(AP_MOTORS_MOT_3,  0.0f,  0.0f, 0,  3);  //Vertical, no yaw contribution
}
