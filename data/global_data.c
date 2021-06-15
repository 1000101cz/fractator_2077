/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  STEPAN MAROUSEK                   *
 * date:    2021/6/15                         *
 **********************************************/
 
#include "global_data.h"
#include <math.h>

void init_all_data(global_data * all_data)
{
	all_data->width = 660;
	all_data->height = 480;
	all_data->number_of_iterations = 60;
	all_data->max_real = 1.6;
	all_data->min_real = -1.6;
	all_data->max_imag = 1.1;
	all_data->min_imag = -1.1;
	all_data->c_real = -0.66;
	all_data->c_imag = 0.385;
	all_data->current_real = all_data->min_real;
	all_data->current_imag = all_data->max_imag;
	all_data->step_real = (fabs(all_data->max_real) + fabs(all_data->min_real)) / all_data->width;
	all_data->step_imag = -(all_data->max_imag - all_data->min_imag) / all_data->height;
	all_data->first_compute = 1;
	all_data->save_pictures = 0;
	all_data->thread1c = 0;
	all_data->thread2c = 0;
	all_data->thread3c = 0;
	all_data->thread4c = 0;
	all_data->prediction = 1;
	all_data->prediction_10_steps = 9;
	all_data->animation_frame = 1;
	all_data->menuPosition = 0;
	all_data->audio = 0;
	all_data->language = 0; // EN
}
