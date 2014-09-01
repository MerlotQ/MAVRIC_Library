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
 * \file linear_algebra.h
 * 
 * Matrix inversion functions
 */
 

#include "linear_algebra.h"
#include "print_util.h"

matrix_2x2_t  inv2(matrix_2x2_t m) 
{
	
	float det=(m.v[0][0]*m.v[1][1] - m.v[0][1]*m.v[1][0]);
	if (det!=0.0f) 
	{
		det=1.0f/det;
		matrix_2x2_t result={.v={ {m.v[1][1]*det,    -m.v[0][1]*det}, 
								 {-m.v[1][0]*det,     m.v[0][0]*det}}};
		return result;
	} 
	else 
	{
		print_util_dbg_print("matrix inversion 2x2 failed!\r\n");
		return zero_2x2;			
	}
}


matrix_3x3_t inv3(matrix_3x3_t m) 
{
	float det=m.v[0][0]*(m.v[1][1]*m.v[2][2] - m.v[1][2]*m.v[2][1]) 
	        - m.v[0][1]*(m.v[2][2]*m.v[1][0]-m.v[1][2]*m.v[2][0])
			+ m.v[0][2]*(m.v[1][0]*m.v[2][1]-m.v[1][1]*m.v[2][0]);
	
	if (det!=0.0f) 
	{
		det=1.0f/det;
		matrix_3x3_t result={.v={{ (m.v[1][1]*m.v[2][2] - m.v[1][2]*m.v[2][1]) *det,  -(m.v[0][1]*m.v[2][2] - m.v[0][2]*m.v[2][1]) *det  , (m.v[0][1]*m.v[1][2] - m.v[0][2]*m.v[1][1]) *det},
		            {-(m.v[1][0]*m.v[2][2] - m.v[1][2]*m.v[2][0]) *det,   (m.v[0][0]*m.v[2][2] - m.v[0][2]*m.v[2][0]) *det  ,-(m.v[0][0]*m.v[1][2] - m.v[0][2]*m.v[1][0]) *det},
		            { (m.v[1][0]*m.v[2][1] - m.v[1][1]*m.v[2][0]) *det,  -(m.v[0][0]*m.v[2][1] - m.v[0][1]*m.v[2][0]) *det  , (m.v[0][0]*m.v[1][1] - m.v[0][1]*m.v[1][0]) *det}}};
		return result;
	} 
	else 
	{
		print_util_dbg_print("matrix inversion 3x3 failed!\r\n");
		return zero_3x3;
	}
}


matrix_4x4_t inv4(matrix_4x4_t m) 
{
	matrix_4x4_t result;
	int32_t i, j;	
	float det;
	result.v[0][0] = m.v[1][1]  * m.v[2][2] * m.v[3][3] - 
             m.v[1][1]  * m.v[2][3] * m.v[3][2] - 
             m.v[2][1]  * m.v[1][2]  * m.v[3][3] + 
             m.v[2][1]  * m.v[1][3]  * m.v[3][2] +
             m.v[3][1] * m.v[1][2]  * m.v[2][3] - 
             m.v[3][1] * m.v[1][3]  * m.v[2][2];

    result.v[1][0] = -m.v[1][0]  * m.v[2][2] * m.v[3][3] + 
              m.v[1][0]  * m.v[2][3] * m.v[3][2] + 
              m.v[2][0]  * m.v[1][2]  * m.v[3][3] - 
              m.v[2][0]  * m.v[1][3]  * m.v[3][2] - 
              m.v[3][0] * m.v[1][2]  * m.v[2][3] + 
              m.v[3][0] * m.v[1][3]  * m.v[2][2];

    result.v[2][0] = m.v[1][0]  * m.v[2][1] * m.v[3][3] - 
             m.v[1][0]  * m.v[2][3] * m.v[3][1] - 
             m.v[2][0]  * m.v[1][1] * m.v[3][3] + 
             m.v[2][0]  * m.v[1][3] * m.v[3][1] + 
             m.v[3][0] * m.v[1][1] * m.v[2][3] - 
             m.v[3][0] * m.v[1][3] * m.v[2][1];

    result.v[3][0] = -m.v[1][0]  * m.v[2][1] * m.v[3][2] + 
               m.v[1][0]  * m.v[2][2] * m.v[3][1] +
               m.v[2][0]  * m.v[1][1] * m.v[3][2] - 
               m.v[2][0]  * m.v[1][2] * m.v[3][1] - 
               m.v[3][0] * m.v[1][1] * m.v[2][2] + 
               m.v[3][0] * m.v[1][2] * m.v[2][1];

    result.v[0][1] = -m.v[0][1]  * m.v[2][2] * m.v[3][3] + 
              m.v[0][1]  * m.v[2][3] * m.v[3][2] + 
              m.v[2][1]  * m.v[0][2] * m.v[3][3] - 
              m.v[2][1]  * m.v[0][3] * m.v[3][2] - 
              m.v[3][1] * m.v[0][2] * m.v[2][3] + 
              m.v[3][1] * m.v[0][3] * m.v[2][2];

    result.v[1][1] = m.v[0][0]  * m.v[2][2] * m.v[3][3] - 
             m.v[0][0]  * m.v[2][3] * m.v[3][2] - 
             m.v[2][0]  * m.v[0][2] * m.v[3][3] + 
             m.v[2][0]  * m.v[0][3] * m.v[3][2] + 
             m.v[3][0] * m.v[0][2] * m.v[2][3] - 
             m.v[3][0] * m.v[0][3] * m.v[2][2];

    result.v[2][1] = -m.v[0][0]  * m.v[2][1] * m.v[3][3] + 
              m.v[0][0]  * m.v[2][3] * m.v[3][1] + 
              m.v[2][0]  * m.v[0][1] * m.v[3][3] - 
              m.v[2][0]  * m.v[0][3] * m.v[3][1] - 
              m.v[3][0] * m.v[0][1] * m.v[2][3] + 
              m.v[3][0] * m.v[0][3] * m.v[2][1];

    result.v[3][1] = m.v[0][0]  * m.v[2][1] * m.v[3][2] - 
              m.v[0][0]  * m.v[2][2] * m.v[3][1] - 
              m.v[2][0]  * m.v[0][1] * m.v[3][2] + 
              m.v[2][0]  * m.v[0][2] * m.v[3][1] + 
              m.v[3][0] * m.v[0][1] * m.v[2][2] - 
              m.v[3][0] * m.v[0][2] * m.v[2][1];

    result.v[0][2] = m.v[0][1]  * m.v[1][2] * m.v[3][3] - 
             m.v[0][1]  * m.v[1][3] * m.v[3][2] - 
             m.v[1][1]  * m.v[0][2] * m.v[3][3] + 
             m.v[1][1]  * m.v[0][3] * m.v[3][2] + 
             m.v[3][1] * m.v[0][2] * m.v[1][3] - 
             m.v[3][1] * m.v[0][3] * m.v[1][2];

    result.v[1][2] = -m.v[0][0]  * m.v[1][2] * m.v[3][3] + 
              m.v[0][0]  * m.v[1][3] * m.v[3][2] + 
              m.v[1][0]  * m.v[0][2] * m.v[3][3] - 
              m.v[1][0]  * m.v[0][3] * m.v[3][2] - 
              m.v[3][0] * m.v[0][2] * m.v[1][3] + 
              m.v[3][0] * m.v[0][3] * m.v[1][2];

    result.v[2][2] = m.v[0][0]  * m.v[1][1] * m.v[3][3] - 
              m.v[0][0]  * m.v[1][3] * m.v[3][1] - 
              m.v[1][0]  * m.v[0][1] * m.v[3][3] + 
              m.v[1][0]  * m.v[0][3] * m.v[3][1] + 
              m.v[3][0] * m.v[0][1] * m.v[1][3] - 
              m.v[3][0] * m.v[0][3] * m.v[1][1];

    result.v[3][2] = -m.v[0][0]  * m.v[1][1] * m.v[3][2] + 
               m.v[0][0]  * m.v[1][2] * m.v[3][1] + 
               m.v[1][0]  * m.v[0][1] * m.v[3][2] - 
               m.v[1][0]  * m.v[0][2] * m.v[3][1] - 
               m.v[3][0] * m.v[0][1] * m.v[1][2] + 
               m.v[3][0] * m.v[0][2] * m.v[1][1];

    result.v[0][3] = -m.v[0][1] * m.v[1][2] * m.v[2][3] + 
              m.v[0][1] * m.v[1][3] * m.v[2][2] + 
              m.v[1][1] * m.v[0][2] * m.v[2][3] - 
              m.v[1][1] * m.v[0][3] * m.v[2][2] - 
              m.v[2][1] * m.v[0][2] * m.v[1][3] + 
              m.v[2][1] * m.v[0][3] * m.v[1][2];

    result.v[1][3] = m.v[0][0] * m.v[1][2] * m.v[2][3] - 
             m.v[0][0] * m.v[1][3] * m.v[2][2] - 
             m.v[1][0] * m.v[0][2] * m.v[2][3] + 
             m.v[1][0] * m.v[0][3] * m.v[2][2] + 
             m.v[2][0] * m.v[0][2] * m.v[1][3] - 
             m.v[2][0] * m.v[0][3] * m.v[1][2];

    result.v[2][3] = -m.v[0][0] * m.v[1][1] * m.v[2][3] + 
               m.v[0][0] * m.v[1][3] * m.v[2][1] + 
               m.v[1][0] * m.v[0][1] * m.v[2][3] - 
               m.v[1][0] * m.v[0][3] * m.v[2][1] - 
               m.v[2][0] * m.v[0][1] * m.v[1][3] + 
               m.v[2][0] * m.v[0][3] * m.v[1][1];

    result.v[3][3] = m.v[0][0] * m.v[1][1] * m.v[2][2] - 
              m.v[0][0] * m.v[1][2] * m.v[2][1] - 
              m.v[1][0] * m.v[0][1] * m.v[2][2] + 
              m.v[1][0] * m.v[0][2] * m.v[2][1] + 
              m.v[2][0] * m.v[0][1] * m.v[1][2] - 
              m.v[2][0] * m.v[0][2] * m.v[1][1];

    det = m.v[0][0] * result.v[0][0] + m.v[0][1] * result.v[1][0] + m.v[0][2] * result.v[2][0] + m.v[0][3] * result.v[3][0];

    if (det == 0) 
	{
		print_util_dbg_print("inversion 4x4 failed\r\n");
        return zero_4x4;
	}

    det = 1.0f / det;

    for (i = 0; i < 4; i++) 
	{
		for (j = 0; j < 4; j++) 
		{
	        result.v[i][j] = result.v[i][j] * det;
		}
	}
	return result;
}
