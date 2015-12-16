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
 * \file main.cpp
 * 
 * \author MAV'RIC Team
 *   
 * \brief Main file
 *
 ******************************************************************************/
 
#include "central_data.hpp"
#include "megafly_rev4.hpp"
#include "mavlink_telemetry.hpp"
#include "tasks.hpp"

#include "file_dummy.hpp"
#include "file_flash_avr32.hpp"
#include "serial_usb_avr32.hpp"

#include "dynamic_model_quad_diag.hpp"
#include "simulation.hpp"
#include "adc_dummy.hpp"
#include "pwm_dummy.hpp"

extern "C" 
{
	#include "time_keeper.hpp"
	#include "print_util.h"
	#include "piezo_speaker.h"
	#include "delay.h"

	#include "conf_imu.hpp"
}

#include "dbg.hpp"

int main (void)
{
	// -------------------------------------------------------------------------
	// Create board
	// -------------------------------------------------------------------------
	megafly_rev4_conf_t board_config 	= megafly_rev4_default_config();
	// board_config.imu_config 			= imu_config();							// Load custom imu config (cf conf_imu.h)
	Megafly_rev4 board = Megafly_rev4( board_config );


	// -------------------------------------------------------------------------
	// Create simulation
	// -------------------------------------------------------------------------
	// Simulated servos
	Pwm_dummy pwm[4];
	Servo sim_servo_0(pwm[0], servo_default_config_esc());
	Servo sim_servo_1(pwm[1], servo_default_config_esc());
	Servo sim_servo_2(pwm[2], servo_default_config_esc());
	Servo sim_servo_3(pwm[3], servo_default_config_esc());
	
	// Simulated dynamic model
	Dynamic_model_quad_diag sim_model 	= Dynamic_model_quad_diag(sim_servo_0, sim_servo_1, sim_servo_2, sim_servo_3);
	Simulation sim 						= Simulation(sim_model);
	
	// Simulated battery
	Adc_dummy 	sim_adc_battery = Adc_dummy(11.1f);
	Battery 	sim_battery 	= Battery(sim_adc_battery);

	// Simulated IMU
	Imu 		sim_imu 		= Imu(  sim.accelerometer(),
										sim.gyroscope(),
										sim.magnetometer() );


	// -------------------------------------------------------------------------
	// Create central data
	// -------------------------------------------------------------------------
	// Create central data using real sensors
	Central_data cd = Central_data( MAVLINK_SYS_ID,
									board.imu, 
									board.bmp085,
									board.gps_ublox, 
									// sim.gps(), 
									board.sonar_i2cxl,		// Warning:
									// sim.sonar(),				// this is simulated
									board.uart0,
									board.spektrum_satellite,
									board.green_led,
									board.file_flash,
									board.battery,
									// sim_battery,
									board.servo_0,
									board.servo_1,
									board.servo_2,
									board.servo_3 );


	// Create central data with simulated sensors
	// Central_data cd = Central_data( MAVLINK_SYS_ID,
	// 								sim_imu, 
	// 								sim.barometer(),
	// 								sim.gps(), 
	// 								sim.sonar(),
	// 								board.uart0, 				// mavlink serial
	// 								board.spektrum_satellite,
	// 								board.green_led,
	// 								board.file_flash,
	// 								sim_battery,
	// 								board.servo_0,
	// 								board.servo_1,
	// 								board.servo_2,
	// 								board.servo_3 );

	// -------------------------------------------------------------------------
	// Initialisation
	// -------------------------------------------------------------------------
	bool init_success = true;

	// Board initialisation
	init_success &= board.init();

	// Init central data
	init_success &= cd.init();

	init_success &= mavlink_telemetry_add_onboard_parameters(&cd.mavlink_communication.onboard_parameters, &cd);

	print_util_dbg_print("onboard_parameters\r\n");
	delay_ms(150);

	// Try to read from flash, if unsuccessful, write to flash
	if( onboard_parameters_read_parameters_from_storage(&cd.mavlink_communication.onboard_parameters) == false )
	{
		onboard_parameters_write_parameters_to_storage(&cd.mavlink_communication.onboard_parameters);
		init_success = false; 
	}

	init_success &= mavlink_telemetry_init(&cd);

	print_util_dbg_print("mavlink_telemetry_init\r\n");
	delay_ms(150);

	cd.state.mav_state = MAV_STATE_STANDBY;	
	
	init_success &= tasks_create_tasks(&cd);

	print_util_dbg_print("tasks_create_tasks\r\n");
	delay_ms(150);

	if (init_success)
	{
		piezo_speaker_quick_startup();
		
		// Switch off red LED
		board.red_led.off();
	}
	else
	{
		piezo_speaker_critical_error_melody();
	}

	print_util_dbg_print("[MAIN] OK. Starting up.\r\n");

	// -------------------------------------------------------------------------
	// Main loop
	// -------------------------------------------------------------------------
	while (1 == 1) 
	{
		scheduler_update(&cd.scheduler);
	}

	return 0;
}
