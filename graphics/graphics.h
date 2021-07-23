/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  STEPAN MAROUSEK                   *
 * date:    2021/07/24                        *
 **********************************************/

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <stdbool.h>
#include <stdint.h>
#include "../data/global_data.h"

void save_picture(global_data *all_data);

uint8_t getButton(int buttor_number, int position, int subpixel, int menuPosition);

float getT(int k, int number_of_iterations);

uint8_t getR(float t);

uint8_t getG(float t);

uint8_t getB(float t);

void save_pixel(int position, uint8_t red, uint8_t green, uint8_t blue);

void display_buffer(global_buffer * all_buffers, global_data * all_data);

void erase_buffer(global_buffer * all_buffers, global_data * all_data);

int iter_function(int number_of_iterations, float real, float imag, double c_real, double c_imag);

void compute_line(bool upperBool, global_buffer * all_buffers, global_data * all_data);

void compute_column(bool leftBool, global_buffer * all_buffers, global_data * all_data);

void compute_function(int cycle_start, int cycle_end, int local_width, int local_height, double c_real, double c_imag, int number_of_iterations, double min_real, double max_imag, double step_real, double step_imag);

void cpu_compute(global_buffer * all_buffers, global_data * all_data);

#endif
