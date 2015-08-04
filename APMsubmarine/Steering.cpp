// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include "Submarine.h"

void Submarine::throttle_slew_limit(int16_t last_throttle)
{
    // if slew limit rate is set to zero then do not slew limit
    if (g.throttle_slewrate && last_throttle != 0) {                   
        // limit throttle change by the given percentage per second
        float temp = g.throttle_slewrate * G_Dt * 0.01f * fabsf(channel_throttle->radio_max - channel_throttle->radio_min);
        // allow a minimum change of 1 PWM per cycle
        if (temp < 1) {
            temp = 1;
        }
        channel_throttle->radio_out = constrain_int16(channel_throttle->radio_out, last_throttle - temp, last_throttle + temp);
    }
}

/*
  check for triggering of start of auto mode
 */
bool Submarine::auto_check_trigger(void)
{
    // only applies to AUTO mode
    if (control_mode != AUTO) {
        return true;
    }

    // check for user pressing the auto trigger to off
    if (auto_triggered && g.auto_trigger_pin != -1 && check_digital_pin(g.auto_trigger_pin) == 1) {
        gcs_send_text_P(SEVERITY_LOW, PSTR("AUTO triggered off"));
        auto_triggered = false;
        return false; 
    }

    // if already triggered, then return true, so you don't
    // need to hold the switch down
    if (auto_triggered) {
        return true;
    }

    if (g.auto_trigger_pin == -1 && is_zero(g.auto_kickstart)) {
        // no trigger configured - let's go!
        auto_triggered = true;
        return true;
    }
 
    if (g.auto_trigger_pin != -1 && check_digital_pin(g.auto_trigger_pin) == 0) {
        gcs_send_text_P(SEVERITY_LOW, PSTR("Triggered AUTO with pin"));
        auto_triggered = true;
        return true;            
    }

    if (!is_zero(g.auto_kickstart)) {
        float xaccel = ins.get_accel().x;
        if (xaccel >= g.auto_kickstart) {
            gcs_send_text_fmt(PSTR("Triggered AUTO xaccel=%.1f"), (double)xaccel);
            auto_triggered = true;
            return true;            
        }
    }

    return false;   
}

void Submarine::calc_throttle(float target_speed)
{  
    if (!auto_check_trigger()) {
        channel_throttle->servo_out = g.throttle_min.get();
        return;
    }

    float throttle_base = (fabsf(target_speed) / g.speed_cruise) * g.throttle_cruise;
    int throttle_target = throttle_base + throttle_nudge;

    throttle = throttle_target;    

    if (in_reverse) {
        channel_throttle->servo_out = constrain_int16(-throttle, -g.throttle_max, -g.throttle_min);
    } else {
        channel_throttle->servo_out = constrain_int16(throttle, g.throttle_min, g.throttle_max);
    }
}

void Submarine::calc_lateral_acceleration()
{
    switch (control_mode) {
    case AUTO:
        nav_controller->update_waypoint(prev_WP, next_WP);
        break;

    case RTL:
    case GUIDED:
    case STEERING:
        nav_controller->update_waypoint(current_loc, next_WP);
        break;
    default:
        return;
    }
    lateral_acceleration = nav_controller->lateral_acceleration();
}

void Submarine::calc_nav_steer()
{
    // add in obstacle avoidance
    lateral_acceleration += (obstacle.turn_angle/45.0f) * g.turn_max_g;

    // constrain to max G force
    lateral_acceleration = constrain_float(lateral_acceleration, -g.turn_max_g*GRAVITY_MSS, g.turn_max_g*GRAVITY_MSS);

    channel_steer->servo_out = steerController.get_steering_out_lat_accel(lateral_acceleration);
}

void Submarine::set_servos(void)
{
    static int16_t last_throttle;

    // support a separate steering channel
    RC_Channel_aux::set_servo_out(RC_Channel_aux::k_steering, channel_steer->pwm_to_angle_dz(0));

	if (control_mode == MANUAL || control_mode == LEARNING) {
        // do a direct pass through of radio values
        channel_steer->radio_out       = channel_steer->read();
        channel_throttle->radio_out    = channel_throttle->read();
        if (failsafe.bits & FAILSAFE_EVENT_THROTTLE) {
            // suppress throttle if in failsafe and manual
            channel_throttle->radio_out = channel_throttle->radio_trim;
        }
	} else {       
        channel_steer->calc_pwm();
        if (in_reverse) {
            channel_throttle->servo_out = constrain_int16(channel_throttle->servo_out, 
                                                          -g.throttle_max,
                                                          -g.throttle_min);
        } else {
            channel_throttle->servo_out = constrain_int16(channel_throttle->servo_out, 
                                                          g.throttle_min.get(), 
                                                          g.throttle_max.get());
        }

        if ((failsafe.bits & FAILSAFE_EVENT_THROTTLE) && control_mode < AUTO) {
            // suppress throttle if in failsafe
            channel_throttle->servo_out = 0;
        }

        // convert 0 to 100% into PWM
        channel_throttle->calc_pwm();

        // limit throttle movement speed
        throttle_slew_limit(last_throttle);
    }

#if HIL_MODE == HIL_MODE_DISABLED || HIL_SERVOS
	// send values to the PWM timers for output
	// ----------------------------------------
    channel_steer->output(); 
    channel_throttle->output();
    RC_Channel_aux::output_ch_all();
#endif
}


