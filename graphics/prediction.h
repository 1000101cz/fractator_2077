/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  STEPAN MAROUSEK                   *
 * date:    2021/07/24                        *
 **********************************************/

#ifndef __PREDICTION_H__
#define __PREDICTION_H__

#include "graphics.h"

void compute_function_predict_1(int cycle_start, int cycle_end, int local_width, int local_height, long double c_real, long double c_imag, int number_of_iterations, long double min_real, long double max_imag, long double step_real, long double step_imag);

void compute_function_predict_2(int cycle_start, int cycle_end, int local_width, int local_height, long double c_real, long double c_imag, int number_of_iterations, long double min_real, long double max_imag, long double step_real, long double step_imag);

void compute_function_predict_3(int cycle_start, int cycle_end, int local_width, int local_height, long double c_real, long double c_imag, int number_of_iterations, long double min_real, long double max_imag, long double step_real, long double step_imag);

void compute_function_predict_4(int cycle_start, int cycle_end, int local_width, int local_height, long double c_real, long double c_imag, int number_of_iterations, long double min_real, long double max_imag, long double step_real, long double step_imag);

void compute_function_predict_5(int cycle_start, int cycle_end, int local_width, int local_height, long double c_real, long double c_imag, int number_of_iterations, long double min_real, long double max_imag, long double step_real, long double step_imag);

void compute_function_predict_10(int cycle_start, int cycle_end, int local_width, int local_height, long double c_real, long double c_imag, int number_of_iterations, long double min_real, long double max_imag, long double step_real, long double step_imag, int prediction_10_steps);

void compute_function_predict_11(int cycle_start, int cycle_end, int local_width, int local_height, long double c_real, long double c_imag, int number_of_iterations, long double min_real, long double max_imag, long double step_real, long double step_imag, int prediction_10_steps);

void compute_function_predict_12(int cycle_start, int cycle_end, int local_width, int local_height, long double c_real, long double c_imag, int number_of_iterations, long double min_real, long double max_imag, long double step_real, long double step_imag, int prediction_10_steps);

void compute_function_predict_13(int cycle_start, int cycle_end, int local_width, int local_height, long double c_real, long double c_imag, int number_of_iterations, long double min_real, long double max_imag, long double step_real, long double step_imag, int prediction_10_steps);

#endif
