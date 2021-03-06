/*******************************************************************************
 * Copyright (c) 2009-2016, MAV'RIC Development Team
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
 * \file ins_telemetry.hpp
 *
 * \author MAV'RIC Team
 * \author Julien Lecoeur
 * \author Basil Huber
 *
 * \brief   Telemetry for Inertial Navigation System
 *
 ******************************************************************************/


#ifndef INS_TELEMETRY_HPP_
#define INS_TELEMETRY_HPP_

#include "sensing/ins.hpp"
#include "communication/mavlink_message_handler.hpp"


/**
 * \brief   Initalize INS telemetry
 * \details Setup callback for receiving SET_GPS_GLOBAL_ORIGIN messages
 *
 * \return success
 */
bool ins_telemetry_init(INS* ins, Mavlink_message_handler* message_handler);

/**
 * \brief   Function to send the MAVLink position estimation message
 *
 * \param   ins                     The pointer to the Inertial Navigation System (INS)
 * \param   mavlink_stream          The pointer to the MAVLink stream
 * \param   msg                     The pointer to the MAVLink message
 */
void ins_telemetry_send_local_position_ned_cov(const INS* ins, const Mavlink_stream* mavlink_stream, mavlink_message_t* msg);


/**
 * \brief   Function to send the MAVLink position estimation message
 *
 * \param   ins                     The pointer to the Inertial Navigation System (INS)
 * \param   mavlink_stream          The pointer to the MAVLink stream structure
 * \param   msg                     The pointer to the MAVLink message
 */
void ins_telemetry_send_local_position_ned(const INS* ins, const Mavlink_stream* mavlink_stream, mavlink_message_t* msg);


/**
 * \brief   Function to send the MAVLink GPS global position message
 *
 * \param   ins                     The pointer to the Inertial Navigation System (INS)
 * \param   mavlink_stream          The pointer to the MAVLink stream structure
 * \param   msg                     The pointer to the MAVLink message
 */
void ins_telemetry_send_global_position_int(const INS* ins, const Mavlink_stream* mavlink_stream, mavlink_message_t* msg);

/**
 * \brief   Function to send the origin (INS::origin_)
 *
 * \param   ins                     The pointer to the Inertial Navigation System (INS)
 * \param   mavlink_stream          The pointer to the MAVLink stream structure
 * \param   msg                     The pointer to the MAVLink message
 */
void ins_telemetry_send_gps_global_origin(const INS* ins, const Mavlink_stream* mavlink_stream, mavlink_message_t* msg);
#endif /* INS_TELEMETRY_HPP_ */
