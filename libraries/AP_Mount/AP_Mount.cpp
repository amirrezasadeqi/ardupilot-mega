// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: t -*-

#include <FastSerial.h>
#include <AP_Common.h>
#include <AP_Param.h>
#include <AP_Mount.h>

const AP_Param::GroupInfo AP_Mount::var_info[] PROGMEM = {
	// @Param: MODE
	// @DisplayName: Mount operation mode
	// @Description: Camera or antenna mount operation mode
	// @Values: 0:retract,1:neutral,2:MavLink_targeting,3:RC_targeting,4:GPS_point
	// @User: Standard
    AP_GROUPINFO("MODE",       0, AP_Mount, _mount_mode), // see MAV_MOUNT_MODE at ardupilotmega.h

    // @Param: RETRACT
	// @DisplayName: Mount retract angles
	// @Description: Mount angles when in retract operation mode
	// @Units: centi-Degrees
	// @Range: -18000 17999
	// @Increment: 1
	// @User: Standard
    AP_GROUPINFO("RETRACT",    1, AP_Mount, _retract_angles),

	// @Param: NEUTRAL
	// @DisplayName: Mount neutral angles
	// @Description: Mount angles when in neutral operation mode
	// @Units: centi-Degrees
	// @Range: -18000 17999
	// @Increment: 1
	// @User: Standard
    AP_GROUPINFO("NEUTRAL",    2, AP_Mount, _neutral_angles),

    // @Param: CONTROL
	// @DisplayName: Mount control angles
	// @Description: Mount angles when in MavLink or RC control operation mode
	// @Units: centi-Degrees
	// @Range: -18000 17999
	// @Increment: 1
	// @User: Standard
    AP_GROUPINFO("CONTROL",    3, AP_Mount, _control_angles),

	// @Param: STAB_ROLL
	// @DisplayName: Stabilize mount roll
	// @Description:enable roll stabilisation relative to Earth
	// @Values: 0:Disabled,1:Enabled
	// @User: Standard
    AP_GROUPINFO("STAB_ROLL",  4, AP_Mount, _stab_roll),

	// @Param: STAB_TILT
	// @DisplayName: Stabilize mount tilt
	// @Description: enable tilt (pitch) stabilisation relative to Earth
	// @Values: 0:Disabled,1:Enabled
	// @User: Standard
    AP_GROUPINFO("STAB_TILT", 5, AP_Mount, _stab_tilt),

	// @Param: STAB_PAN
	// @DisplayName: Stabilize mount pan
	// @Description: enable pan (yaw) stabilisation relative to Earth
	// @Values: 0:Disabled,1:Enabled
	// @User: Standard
    AP_GROUPINFO("STAB_PAN",   6, AP_Mount, _stab_pan),

	// @Param: ROLL_RC_IN
	// @DisplayName: roll RC input channel
	// @Description: 0 for none, any other for the RC channel to be used to control roll movements
	// @Values: 0:Disabled,5:RC5,6:RC6,7:RC7,8:RC8
	// @User: Standard
	AP_GROUPINFO("ROLL_RC_IN",  7, AP_Mount, _roll_rc_in),

	// @Param: ROLL_ANGLE_MIN
	// @DisplayName: Minimum roll angle
	// @Description: Minimum physical roll angular position of mount.
	// @Units: centi-Degrees
	// @Range: -18000 17999
	// @Increment: 1
	// @User: Standard
	AP_GROUPINFO("ROLL_ANGMIN", 8, AP_Mount, _roll_angle_min),

	// @Param: ROLL_ANGLE_MAX
	// @DisplayName: Maximum roll angle
	// @Description: Maximum physical roll angular position of the mount
	// @Units: centi-Degrees
	// @Range: -18000 17999
	// @Increment: 1
	// @User: Standard
	AP_GROUPINFO("ROLL_ANGMAX", 9, AP_Mount, _roll_angle_max),

	// @Param: TILT_RC_IN
	// @DisplayName: tilt (pitch) RC input channel
	// @Description: 0 for none, any other for the RC channel to be used to control tilt (pitch) movements
	// @Values: 0:Disabled,5:RC5,6:RC6,7:RC7,8:RC8
	// @User: Standard
	AP_GROUPINFO("TILT_RC_IN",  10, AP_Mount, _tilt_rc_in),

	// @Param: TILT_ANGLE_MIN
	// @DisplayName: Minimum tilt angle
	// @Description: Minimum physical tilt (pitch) angular position of mount.
	// @Units: centi-Degrees
	// @Range: -18000 17999
	// @Increment: 1
	// @User: Standard
	AP_GROUPINFO("TILT_ANGMIN", 11, AP_Mount, _tilt_angle_min),

	// @Param: TILT_ANGLE_MAX
	// @DisplayName: Maximum tilt angle
	// @Description: Maximum physical tilt (pitch) angular position of the mount
	// @Units: centi-Degrees
	// @Range: -18000 17999
	// @Increment: 1
	// @User: Standard
	AP_GROUPINFO("TILT_ANGMAX", 12, AP_Mount, _tilt_angle_max),

	// @Param: PAN_RC_IN
	// @DisplayName: pan (yaw) RC input channel
	// @Description: 0 for none, any other for the RC channel to be used to control pan (yaw) movements
	// @Values: 0:Disabled,5:RC5,6:RC6,7:RC7,8:RC8
	// @User: Standard
	AP_GROUPINFO("PAN_RC_IN",  13, AP_Mount, _pan_rc_in),

	// @Param: PAN_ANGLE_MIN
	// @DisplayName: Minimum pan angle
	// @Description: Minimum physical pan (yaw) angular position of mount.
	// @Units: centi-Degrees
	// @Range: -18000 17999
	// @Increment: 1
	// @User: Standard
	AP_GROUPINFO("PAN_ANGMIN",  14, AP_Mount, _pan_angle_min),

	// @Param: PAN_ANGLE_MAX
	// @DisplayName: Maximum pan angle
	// @Description: Maximum physical pan (yaw) angular position of the mount
	// @Units: centi-Degrees
	// @Range: -18000 17999
	// @Increment: 1
	// @User: Standard
	AP_GROUPINFO("PAN_ANGMAX",  15, AP_Mount, _pan_angle_max),
/*
	Must be commented out because the framework does not support more than 16 parameters
	// @Param: JOYSTICK_SPEED
	// @DisplayName: mount joystick speed
	// @Description: 0 for position control, small for low speeds, 10 for max speed
	// @Range: 0 10
	// @Increment: 1
	// @User: Standard
	AP_GROUPINFO("JOYSTICK_SPEED",  16, AP_Mount, _joystick_speed),
*/
	AP_GROUPEND
};

extern RC_Channel_aux* g_rc_function[RC_Channel_aux::k_nr_aux_servo_functions];	// the aux. servo ch. assigned to each function
extern RC_Channel* rc_ch[NUM_CHANNELS];

AP_Mount::AP_Mount(const struct Location *current_loc, GPS *&gps, AP_AHRS *ahrs):
_gps(gps)
{
	_ahrs = ahrs;
	_current_loc = current_loc;

	// startup with the mount retracted
	set_mode(MAV_MOUNT_MODE_RETRACT);

	// default to zero angles
	_retract_angles = Vector3f(0,0,0);
	_neutral_angles = Vector3f(0,0,0);
	_control_angles = Vector3f(0,0,0);

	// default unknown mount type
	_mount_type = k_unknown;

	// default manual rc channel to disabled
	_pan_rc_in = 0;
	_tilt_rc_in= 0;
	_roll_rc_in= 0;

	_pan_angle_min  = -4500; // assume -45 degrees min deflection
	_pan_angle_max  =  4500; // assume  45 degrees max deflection
	_tilt_angle_min = -4500; // assume -45 degrees min deflection
	_tilt_angle_max =  4500; // assume  45 degrees max deflection
	_roll_angle_min = -4500; // assume -45 degrees min deflection
	_roll_angle_max =  4500; // assume  45 degrees max deflection
}

/// Auto-detect the mount gimbal type depending on the functions assigned to the servos
void
AP_Mount::update_mount_type()
{
	if ((g_rc_function[RC_Channel_aux::k_mount_roll] == NULL) && (g_rc_function[RC_Channel_aux::k_mount_tilt] != NULL) && (g_rc_function[RC_Channel_aux::k_mount_pan] != NULL))
	{
		_mount_type = k_pan_tilt;
	}
	if ((g_rc_function[RC_Channel_aux::k_mount_roll] != NULL) && (g_rc_function[RC_Channel_aux::k_mount_tilt] != NULL) && (g_rc_function[RC_Channel_aux::k_mount_pan] == NULL))
	{
		_mount_type = k_tilt_roll;
	}
	if ((g_rc_function[RC_Channel_aux::k_mount_roll] != NULL) && (g_rc_function[RC_Channel_aux::k_mount_tilt] != NULL) && (g_rc_function[RC_Channel_aux::k_mount_pan] != NULL))
	{
		_mount_type = k_pan_tilt_roll;
	}
}

/// sets the servo angles for retraction, note angles are in degrees
void AP_Mount::set_retract_angles(float roll, float tilt, float pan)
{
	_retract_angles = Vector3f(roll, tilt, pan);
}

//sets the servo angles for neutral, note angles are in degrees
void AP_Mount::set_neutral_angles(float roll, float tilt, float pan)
{
	_neutral_angles = Vector3f(roll, tilt, pan);
}

/// sets the servo angles for MAVLink, note angles are in degrees
void AP_Mount::set_control_angles(float roll, float tilt, float pan)
{
	_control_angles = Vector3f(roll, tilt, pan);
}

/// used to tell the mount to track GPS location
void AP_Mount::set_GPS_target_location(Location targetGPSLocation)
{
	_target_GPS_location=targetGPSLocation;
}

/// This one should be called periodically
void AP_Mount::update_mount_position()
{
	switch((enum MAV_MOUNT_MODE)_mount_mode.get())
	{
		// move mount to a "retracted position" or to a position where a fourth servo can retract the entire mount into the fuselage
		case MAV_MOUNT_MODE_RETRACT:
		{
			Vector3f vec = _retract_angles.get();
			_roll_angle  = vec.x;
			_tilt_angle  = vec.y;
			_pan_angle   = vec.z;
			break;
		}

		// move mount to a neutral position, typically pointing forward
		case MAV_MOUNT_MODE_NEUTRAL:
		{
			Vector3f vec = _neutral_angles.get();
			_roll_angle  = vec.x;
			_tilt_angle  = vec.y;
			_pan_angle   = vec.z;
			break;
		}

		// point to the angles given by a mavlink message
		case MAV_MOUNT_MODE_MAVLINK_TARGETING:
		{
			Vector3f vec = _control_angles.get();
			_roll_control_angle  = radians(vec.x);
			_tilt_control_angle  = radians(vec.y);
			_pan_control_angle   = radians(vec.z);
			stabilize();
			break;
		}

		// RC radio manual angle control, but with stabilization from the AHRS
		case MAV_MOUNT_MODE_RC_TARGETING:
		{
/*			if (_joystick_speed) {  // for spring loaded joysticks
				// allow pilot speed position input to come directly from an RC_Channel
				if (_roll_rc_in && (rc_ch[_roll_rc_in-1])) {
					//_roll_control_angle += angle_input(rc_ch[_roll_rc_in-1], _roll_angle_min, _roll_angle_max) * 0.00001 * _joystick_speed;
					_roll_control_angle += rc_ch[_roll_rc_in-1]->norm_input() * 0.00001 * _joystick_speed;
					if (_roll_control_angle < radians(_roll_angle_min*0.01)) _roll_control_angle = radians(_roll_angle_min*0.01);
					if (_roll_control_angle > radians(_roll_angle_max*0.01)) _roll_control_angle = radians(_roll_angle_max*0.01);
				}
				if (_tilt_rc_in && (rc_ch[_tilt_rc_in-1])) {
					//_tilt_control_angle += angle_input(rc_ch[_tilt_rc_in-1], _tilt_angle_min, _tilt_angle_max) * 0.00001 * _joystick_speed;
					_tilt_control_angle += rc_ch[_tilt_rc_in-1]->norm_input() * 0.00001 * _joystick_speed;
					if (_tilt_control_angle < radians(_tilt_angle_min*0.01)) _tilt_control_angle = radians(_tilt_angle_min*0.01);
					if (_tilt_control_angle > radians(_tilt_angle_max*0.01)) _tilt_control_angle = radians(_tilt_angle_max*0.01);
				}
				if (_pan_rc_in && (rc_ch[_pan_rc_in-1])) {
					//_pan_control_angle += angle_input(rc_ch[_pan_rc_in-1], _pan_angle_min, _pan_angle_max) * 0.00001 * _joystick_speed;
					_pan_control_angle += rc_ch[_pan_rc_in-1]->norm_input() * 0.00001 * _joystick_speed;
					if (_pan_control_angle < radians(_pan_angle_min*0.01)) _pan_control_angle = radians(_pan_angle_min*0.01);
					if (_pan_control_angle > radians(_pan_angle_max*0.01)) _pan_control_angle = radians(_pan_angle_max*0.01);
				}
			} else {
*/				// allow pilot position input to come directly from an RC_Channel
				if (_roll_rc_in && (rc_ch[_roll_rc_in-1])) {
					_roll_control_angle = angle_input_rad(rc_ch[_roll_rc_in-1], _roll_angle_min, _roll_angle_max);
				}
				if (_tilt_rc_in && (rc_ch[_tilt_rc_in-1])) {
					_tilt_control_angle = angle_input_rad(rc_ch[_tilt_rc_in-1], _tilt_angle_min, _tilt_angle_max);
				}
				if (_pan_rc_in && (rc_ch[_pan_rc_in-1])) {
					_pan_control_angle = angle_input_rad(rc_ch[_pan_rc_in-1], _pan_angle_min, _pan_angle_max);
				}
//			}
			stabilize();
			break;
		}

		// point mount to a GPS point given by the mission planner
		case MAV_MOUNT_MODE_GPS_POINT:
		{
			if(_gps->fix){
				calc_GPS_target_angle(&_target_GPS_location);
				stabilize();
			}
			break;
		}
		default:
			//do nothing
			break;
	}

	// write the results to the servos
	move_servo(g_rc_function[RC_Channel_aux::k_mount_roll], _roll_angle*10, _roll_angle_min*0.1, _roll_angle_max*0.1);
	move_servo(g_rc_function[RC_Channel_aux::k_mount_tilt], _tilt_angle*10, _tilt_angle_min*0.1, _tilt_angle_max*0.1);
	move_servo(g_rc_function[RC_Channel_aux::k_mount_pan ],  _pan_angle*10,  _pan_angle_min*0.1,  _pan_angle_max*0.1);
}

void AP_Mount::set_mode(enum MAV_MOUNT_MODE mode)
{
	_mount_mode = (int8_t)mode;
}

/// Change the configuration of the mount
/// triggered by a MavLink packet.
void AP_Mount::configure_msg(mavlink_message_t* msg)
{
	__mavlink_mount_configure_t packet;
	mavlink_msg_mount_configure_decode(msg, &packet);
	if (mavlink_check_target(packet.target_system, packet.target_component)) {
		// not for us
		return;
	}
	set_mode((enum MAV_MOUNT_MODE)packet.mount_mode);
	_stab_roll  = packet.stab_roll;
	_stab_tilt  = packet.stab_pitch;
	_stab_pan   = packet.stab_yaw;
}

/// Control the mount (depends on the previously set mount configuration)
/// triggered by a MavLink packet.
void AP_Mount::control_msg(mavlink_message_t *msg)
{
	__mavlink_mount_control_t packet;
	mavlink_msg_mount_control_decode(msg, &packet);
	if (mavlink_check_target(packet.target_system, packet.target_component)) {
		// not for us
		return;
	}

	switch ((enum MAV_MOUNT_MODE)_mount_mode.get())
	{
	case MAV_MOUNT_MODE_RETRACT:  // Load and keep safe position (Roll,Pitch,Yaw) from EEPROM and stop stabilization
		set_retract_angles(packet.input_b*0.01, packet.input_a*0.01, packet.input_c*0.01);
		if (packet.save_position)
		{
			_retract_angles.save();
		}
		break;

	case MAV_MOUNT_MODE_NEUTRAL:  //  Load and keep neutral position (Roll,Pitch,Yaw) from EEPROM
		set_neutral_angles(packet.input_b*0.01, packet.input_a*0.01, packet.input_c*0.01);
		if (packet.save_position)
		{
			_neutral_angles.save();
		}
		break;

	case MAV_MOUNT_MODE_MAVLINK_TARGETING:  // Load neutral position and start MAVLink Roll,Pitch,Yaw control with stabilization
		set_control_angles(packet.input_b*0.01, packet.input_a*0.01, packet.input_c*0.01);
		break;

	case MAV_MOUNT_MODE_RC_TARGETING:  // Load neutral position and start RC Roll,Pitch,Yaw control with stabilization
		{
			Vector3f vec = _neutral_angles.get();
			_roll_angle  = vec.x;
			_tilt_angle = vec.y;
			_pan_angle   = vec.z;
		}
		break;

	case MAV_MOUNT_MODE_GPS_POINT:  // Load neutral position and start to point to Lat,Lon,Alt
		Location targetGPSLocation;
		targetGPSLocation.lat = packet.input_a;
		targetGPSLocation.lng = packet.input_b;
		targetGPSLocation.alt = packet.input_c;
		set_GPS_target_location(targetGPSLocation);
		break;

	case MAV_MOUNT_MODE_ENUM_END:
		break;

	default:
		// do nothing
		break;
	}
}

/// Return mount status information (depends on the previously set mount configuration)
/// triggered by a MavLink packet.
void AP_Mount::status_msg(mavlink_message_t *msg)
{
	__mavlink_mount_status_t packet;
	mavlink_msg_mount_status_decode(msg, &packet);
	if (mavlink_check_target(packet.target_system, packet.target_component)) {
		// not for us
		return;
	}

	switch ((enum MAV_MOUNT_MODE)_mount_mode.get())
	{
	case MAV_MOUNT_MODE_RETRACT:			// safe position (Roll,Pitch,Yaw) from EEPROM and stop stabilization
	case MAV_MOUNT_MODE_NEUTRAL:			// neutral position (Roll,Pitch,Yaw) from EEPROM
	case MAV_MOUNT_MODE_MAVLINK_TARGETING:	// neutral position and start MAVLink Roll,Pitch,Yaw control with stabilization
	case MAV_MOUNT_MODE_RC_TARGETING:		// neutral position and start RC Roll,Pitch,Yaw control with stabilization
		packet.pointing_b = _roll_angle*100;	///< degrees*100
		packet.pointing_a = _tilt_angle*100;	///< degrees*100
		packet.pointing_c = _pan_angle*100;		///< degrees*100
		break;
	case MAV_MOUNT_MODE_GPS_POINT:         // neutral position and start to point to Lat,Lon,Alt
		packet.pointing_a = _target_GPS_location.lat;	///< latitude
		packet.pointing_b = _target_GPS_location.lng;	///< longitude
		packet.pointing_c = _target_GPS_location.alt;	///< altitude
		break;
	case MAV_MOUNT_MODE_ENUM_END:
		break;
	}

	// status reply
	// TODO: is COMM_3 correct ?
	mavlink_msg_mount_status_send(MAVLINK_COMM_3, packet.target_system, packet.target_component,
			packet.pointing_a, packet.pointing_b, packet.pointing_c);
}

/// Set mount point/region of interest, triggered by mission script commands
void AP_Mount::set_roi_cmd(struct Location *target_loc)
{
	// set the target gps location
	_target_GPS_location = *target_loc;

	// set the mode to GPS tracking mode
	set_mode(MAV_MOUNT_MODE_GPS_POINT);
}

/// Set mount configuration, triggered by mission script commands
void AP_Mount::configure_cmd()
{
	// TODO get the information out of the mission command and use it
}

/// Control the mount (depends on the previously set mount configuration), triggered by mission script commands
void AP_Mount::control_cmd()
{
	// TODO get the information out of the mission command and use it
}

/// returns the angle (degrees*100) that the RC_Channel input is receiving
int32_t
AP_Mount::angle_input(RC_Channel* rc, int16_t angle_min, int16_t angle_max)
{
	return (rc->get_reverse()?-1:1) * (rc->radio_in - rc->radio_min) * (int32_t)(angle_max - angle_min) / (rc->radio_max - rc->radio_min) + (rc->get_reverse()?angle_max:angle_min);
}

/// returns the angle (radians) that the RC_Channel input is receiving
float
AP_Mount::angle_input_rad(RC_Channel* rc, int16_t angle_min, int16_t angle_max)
{
	return radians(angle_input(rc, angle_min, angle_max)*0.01);
}

void
AP_Mount::calc_GPS_target_angle(struct Location *target)
{
	float GPS_vector_x = (target->lng-_current_loc->lng)*cos(ToRad((_current_loc->lat+target->lat)*.00000005))*.01113195;
	float GPS_vector_y = (target->lat-_current_loc->lat)*.01113195;
	float GPS_vector_z = (target->alt-_current_loc->alt);             // baro altitude(IN CM) should be adjusted to known home elevation before take off (Set altimeter).
	float target_distance = 100.0*sqrt(GPS_vector_x*GPS_vector_x + GPS_vector_y*GPS_vector_y);  // Careful , centimeters here locally. Baro/alt is in cm, lat/lon is in meters.
	_roll_control_angle  = 0;
	_tilt_control_angle  = atan2(GPS_vector_z, target_distance);
	_pan_control_angle   = atan2(GPS_vector_x, GPS_vector_y);
}

/// Stabilizes mount relative to the Earth's frame
/// Inputs:
///    _roll_control_angle   desired roll       angle in radians,
///    _tilt_control_angle   desired tilt/pitch angle in radians,
///    _pan_control_angle    desired pan/yaw    angle in radians
/// Outputs:
///    _roll_angle           stabilized roll       angle in degrees,
///    _tilt_angle           stabilized tilt/pitch angle in degrees,
///    _pan_angle            stabilized pan/yaw    angle in degrees
void
AP_Mount::stabilize()
{
	if (_ahrs) {
		// only do the full 3D frame transform if we are doing pan control
		if (_stab_pan) {
			Matrix3f m;             ///< holds 3 x 3 matrix, var is used as temp in calcs
			Matrix3f cam;           ///< Rotation matrix earth to camera. Desired camera from input.
			Matrix3f gimbal_target; ///< Rotation matrix from plane to camera. Then Euler angles to the servos.
			m = _ahrs->get_dcm_matrix();
			m.transpose();
			cam.from_euler(_roll_control_angle, _tilt_control_angle, _pan_control_angle);
			gimbal_target = m * cam;
			gimbal_target.to_euler(&_roll_angle, &_tilt_angle, &_pan_angle);
			_roll_angle  = _stab_roll?degrees(_roll_angle):degrees(_roll_control_angle);
			_tilt_angle  = _stab_tilt?degrees(_tilt_angle):degrees(_tilt_control_angle);
			_pan_angle   = degrees(_pan_angle);
		} else {
			// otherwise base mount roll and tilt on the ahrs
			// roll/tilt attitude, plus any requested angle
			_roll_angle  = degrees(_roll_control_angle);
			_tilt_angle  = degrees(_tilt_control_angle);
			_pan_angle   = degrees(_pan_control_angle);
			if (_stab_roll) {
				_roll_angle -= degrees(_ahrs->roll);
			}
			if (_stab_tilt) {
				_tilt_angle -= degrees(_ahrs->pitch);
			}
		}
	} else {
		_roll_angle  = degrees(_roll_control_angle);
		_tilt_angle  = degrees(_tilt_control_angle);
		_pan_angle   = degrees(_pan_control_angle);
	}
}
/*
/// For testing and development. Called in the medium loop.
void
AP_Mount::debug_output()
{ 	Serial3.print("current   -     ");
	Serial3.print("lat ");
	Serial3.print(_current_loc->lat);
	Serial3.print(",lon ");
	Serial3.print(_current_loc->lng);
	Serial3.print(",alt ");
	Serial3.println(_current_loc->alt);

	Serial3.print("gps       -     ");
	Serial3.print("lat ");
	Serial3.print(_gps->latitude);
	Serial3.print(",lon ");
	Serial3.print(_gps->longitude);
	Serial3.print(",alt ");
	Serial3.print(_gps->altitude);
	Serial3.println();

	Serial3.print("target   -      ");
	Serial3.print("lat ");
	Serial3.print(_target_GPS_location.lat);
	Serial3.print(",lon ");
	Serial3.print(_target_GPS_location.lng);
	Serial3.print(",alt ");
	Serial3.print(_target_GPS_location.alt);
	Serial3.print(" hdg to targ ");
	Serial3.print(degrees(_pan_control_angle));
	Serial3.println();
}
*/
/// saturate to the closest angle limit if outside of [min max] angle interval
/// input angle is in degrees * 10
int16_t
AP_Mount::closest_limit(int16_t angle, int16_t* angle_min, int16_t* angle_max)
{
	// Make sure the angle lies in the interval [-180 .. 180[ degrees
	while (angle < -1800) angle += 3600;
	while (angle >= 1800) angle -= 3600;

	// Make sure the angle limits lie in the interval [-180 .. 180[ degrees
	while (*angle_min < -1800) *angle_min += 3600;
	while (*angle_min >= 1800) *angle_min -= 3600;
	while (*angle_max < -1800) *angle_max += 3600;
	while (*angle_max >= 1800) *angle_max -= 3600;
	// TODO call this function somehow, otherwise this will never work
	//set_range(min, max);

	// If the angle is outside servo limits, saturate the angle to the closest limit
	// On a circle the closest angular position must be carefully calculated to account for wrap-around
	if ((angle < *angle_min) && (angle > *angle_max)){
		// angle error if min limit is used
		int16_t err_min = *angle_min - angle + (angle<*angle_min?0:3600); // add 360 degrees if on the "wrong side"
		// angle error if max limit is used
		int16_t err_max = angle - *angle_max + (angle>*angle_max?0:3600); // add 360 degrees if on the "wrong side"
		angle = err_min<err_max?*angle_min:*angle_max;
	}

	return angle;
}

/// all angles are degrees * 10 units
void
AP_Mount::move_servo(RC_Channel* rc, int16_t angle, int16_t angle_min, int16_t angle_max)
{
	if (rc) {
		// saturate to the closest angle limit if outside of [min max] angle interval
		rc->servo_out  = closest_limit(angle, &angle_min, &angle_max);
		// This is done every time because the user might change the min, max values on the fly
		rc->set_range(angle_min, angle_max);
		// convert angle to PWM using a linear transformation (ignores trimming because the servo limits might not be symmetric)
		rc->calc_pwm();
		rc->output();
	}
}
