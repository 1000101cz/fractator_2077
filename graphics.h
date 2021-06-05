/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  STEPAN MAROUSEK                   *
 * date:    05/16/2021                        *
 **********************************************/

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <stdint.h>
#include "global_data.h"

float getT(int k, int number_of_iterations);

uint8_t getR(float t);

uint8_t getG(float t);

uint8_t getB(float t);

void display_buffer(global_buffer * all_buffers, global_data * all_data);

void create_window(global_data * all_data);

void close_window();

void erase_buffer(global_buffer * all_buffers, global_data * all_data);

void cpu_compute(global_buffer * all_buffers, global_data * all_data);

#endif
