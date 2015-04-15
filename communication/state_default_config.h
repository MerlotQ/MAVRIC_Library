/*******************************************************************************
 * Copyright (c) 2009-2014, MAV'RIC Development Team
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

/*******************************************************************************
 * \file state_default_config.h
 * 
 * \author MAV'RIC Team
 * \author Gregoire Heitz
 *   
 * \brief Default configuration for the state module
 *
 ******************************************************************************/


#ifndef STATE_DEFAULT_CONFIG_H_
#define STATE_DEFAULT_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "state.h"


static inline state_t state_default_config()
{
	state_t conf                 = {};

	conf.mav_mode                = {};
	conf.mav_mode.byte           = MAV_MODE_SAFE;
	conf.mav_state               = MAV_STATE_BOOT;
	conf.simulation_mode         = HIL_OFF;
	conf.autopilot_type          = MAV_TYPE_QUADROTOR;
	conf.autopilot_name          = MAV_AUTOPILOT_MAVRIC;
	conf.sensor_present          = 0b1111110000100111;
	conf.sensor_enabled          = 0b1111110000100111;
	conf.sensor_health           = 0b1111110000100111;
	conf.remote_active           = 1;
	conf.battery                 = {}; 	
	conf.battery.type            = BATTERY_LIPO_3S;
	conf.battery.low_level_limit = 13.3;
	conf.source_mode             = REMOTE;

	return conf;
};

#ifdef __cplusplus
}
#endif

#endif // STATE_DEFAULT_CONFIG_H_
