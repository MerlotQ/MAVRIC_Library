/**
 * \page The MAV'RIC License
 *
 * The MAV'RIC Framework
 *
 * Copyright © 2011-2014
 *
 * Laboratory of Intelligent Systems, EPFL
 */


/**
 * \file adaptive_parameter.c
 * 
 * This file automatically adapts the existing parameters based on given control variable
 */


#include "adaptive_parameter.h"
#include "maths.h"

adaptive_parameter_set_t adapt_param_set;


//------------------------------------------------------------------------------
// PUBLIC FUNCTIONS IMPLEMENTATION
//------------------------------------------------------------------------------

adaptive_parameter_set_t* adaptive_parameter_get_param_set(void)
{
	return &adapt_param_set;
}

void adaptive_parameter_init(void)
{
	adapt_param_set.param_count = 0;
}


int32_t adaptive_parameter_add(float* control_variable, float* parameter, 
							int32_t nb_setpoints, float setpoints[], float setvalues[])
{
	if (adapt_param_set.param_count >= MAX_ADAPT_PARAM_COUNT)
	{
		return 0;
	}
	else
	{			
		adaptive_parameter_t new_param;
		new_param.control_variable = control_variable;
		new_param.parameter = parameter;
		new_param.nb_setpoints = nb_setpoints;
		int32_t i;
		for (i = 0; i < nb_setpoints; ++i)
		{
			new_param.setpoints[i] = setpoints[i];
			new_param.setvalues[i] = setvalues[i];
		}

		adapt_param_set.parameters[adapt_param_set.param_count] = new_param;
		adapt_param_set.param_count += 1;
		return 1;
	}
}


void adaptive_parameter_update(adaptive_parameter_t param)
{
	if (*param.control_variable <= param.setpoints[0])
	{
		*param.parameter = param.setvalues[0];
	}
	else if (*param.control_variable >= param.setpoints[param.nb_setpoints - 1])
	{
		*param.parameter = param.setvalues[param.nb_setpoints - 1];
	}
	else
	{
		int32_t i;
		for (i = 0; i < param.nb_setpoints; ++i)
		{
			if (*param.control_variable >= param.setpoints[i])
			{
				*param.parameter = maths_interpolate(*param.control_variable, 
												param.setpoints[i], param.setpoints[i + 1],
												param.setvalues[i], param.setvalues[i + 1]);
				break;
			}
		}
	}
}


void adaptive_parameter_update_all(void)
{
	int32_t i;
	for (i = 0; i < adapt_param_set.param_count; ++i)
	{
		adaptive_parameter_update(adapt_param_set.parameters[i]);
	}
}
