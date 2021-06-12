/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  STEPAN MAROUSEK                   *
 * date:    05/16/2021                        *
 **********************************************/

#include "graphics.h"
#include "global_data.h"
#include "terminal_colors.h"
#include "xwin_sdl.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>

global_data local_all_data;
global_buffer local_all_buffers;

float getT(int k, int number_of_iterations)
{
	float t = 0.96875 * k / number_of_iterations;
	return t;
}

uint8_t getR(float t)
{
	float r = 2295.0 * (1 - t) * t * t * t;
	return (uint8_t) r;
}

uint8_t getG(float t)
{
	float g = 3825.0 * (1 - t) * (1 - t) * t * t;
	return (uint8_t) g;
}

uint8_t getB(float t)
{
	float b = 2167.5 * (1 - t) * (1 - t) * (1 - t) * t;
	return (uint8_t) b;
}

void save_pixel(int position, uint8_t red, uint8_t green, uint8_t blue)
{
	local_all_buffers.picture_buffer[position] = red;
	local_all_buffers.picture_buffer[position + 1] = green;
	local_all_buffers.picture_buffer[position + 2] = blue;
}

/* compute RGB subpixels and display fractal to SDL window */
void display_buffer(global_buffer * all_buffers, global_data * all_data)
{
	float t;

	for (int i = 0; i < all_data->width * all_data->height; i++) {
		t = getT(all_buffers->iterations_buffer[i],
			 all_data->number_of_iterations);
		all_buffers->picture_buffer[3 * i + 0] = getR(t);
		all_buffers->picture_buffer[3 * i + 1] = getG(t);
		all_buffers->picture_buffer[3 * i + 2] = getB(t);
	}
	xwin_redraw(all_data->width, all_data->height,
		    all_buffers->picture_buffer);
}

void create_window(global_data * all_data)
{
	xwin_init(all_data->width, all_data->height);
}

void close_window()
{
	xwin_close();
}

/* erase all values stored in iterations buffer */
void erase_buffer(global_buffer * all_buffers, global_data * all_data)
{

	for (int i = 0; i < all_data->width * all_data->height; i++) {
		all_buffers->iterations_buffer[i] = 1;
	}
	display_buffer(all_buffers, all_data);
}

int iter_function(int number_of_iterations, float real, float imag, double c_real, double c_imag)
{
	int iter;
	float newreal, newimag;
	for (int j = 1; j <= number_of_iterations; j++) {
		newreal = real * real - imag * imag + c_real;
		newimag = 2 * real * imag + c_imag;
		real = newreal;
		imag = newimag;

		if ((real * real + imag * imag) >= 4) {
			iter = j;
			break;
		}

		if (j == number_of_iterations) {
			iter = j;
			break;
		}
	}
	return iter;
}

void compute_function(int cycle_start, int cycle_end, int local_width, int local_height, double c_real, double c_imag, int number_of_iterations, double min_real, double max_imag, double step_real, double step_imag)
{
	int iter;	// this is going to save to iterations_buffer

	float real;
	float imag;

	for (int i = 0; i < local_width * local_height; i++) {
		real = min_real + (i%local_width)*step_real;
		imag = max_imag + (i/local_width)*step_imag;
		iter = iter_function(number_of_iterations, real, imag, c_real, c_imag);
		float t = getT(iter,number_of_iterations);
		save_pixel(i*3, getR(t), getG(t), getB(t));
	}
}

void compute_function_predict_1(int cycle_start, int cycle_end, int local_width, int local_height, double c_real, double c_imag, int number_of_iterations, double min_real, double max_imag, double step_real, double step_imag)
{
	int iter1;	// this is going to save to iterations_buffer
	int iter2;
	int iter3;

	float real;
	float imag;

	real = min_real;
	imag = max_imag;
	iter1 = iter_function(number_of_iterations, real, imag, c_real, c_imag);
	float t = getT(iter1,number_of_iterations);
	save_pixel(0, getR(t), getG(t), getB(t));

	for (int i = 1; i < local_width * local_height; i++) {
		if ((i+1)%local_width == 0) {
			int position = 3 * i;
			real = min_real + (i%local_width)*step_real;
			imag = max_imag + (i/local_width)*step_imag;
			iter2 = iter_function(number_of_iterations, real, imag, c_real, c_imag);
			float t = getT(iter2,number_of_iterations);
			save_pixel(position, getR(t), getG(t), getB(t));
		}
		else {
			int position = 3 * (i + 1);
			real = min_real + ((i+1)%local_width)*step_real;
			imag = max_imag + ((i+1)/local_width)*step_imag;
			iter3 = iter_function(number_of_iterations, real, imag, c_real, c_imag);
			float t = getT(iter3,number_of_iterations);
			save_pixel(position, getR(t), getG(t), getB(t));

			if (iter3 == iter1) {
				int position = 3 * (i);
				save_pixel(position, getR(t), getG(t), getB(t));
			}
			else {
				int position = 3 * i;
				real = min_real + (i%local_width)*step_real;
				imag = max_imag + (i/local_width)*step_imag;
				iter2 = iter_function(number_of_iterations, real, imag, c_real, c_imag);
				float t = getT(iter2,number_of_iterations);
				save_pixel(position, getR(t), getG(t), getB(t));
			}
			i++;
			iter1 = iter3;
		}
	}
}

void compute_function_predict_2(int cycle_start, int cycle_end, int local_width, int local_height, double c_real, double c_imag, int number_of_iterations, double min_real, double max_imag, double step_real, double step_imag)
{
	int iter1;	// this is going to save to iterations_buffer
	int iter2;
	int iter3;
	int iter4;
	int iter5;

	float real;
	float imag;

	for (int i = 0; i < (local_width * local_height)/25; i++) {
		float t;
		int position;
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		real = min_real + ((i*5)%local_width)*step_real;
		imag = max_imag + ((i*5)/local_width)*(step_imag*5);

		iter1 = iter_function(number_of_iterations, real, imag, c_real, c_imag);
		position = 15 * i + (i/(local_width/5))*(12*local_width);
		t = getT(iter1,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position + 3, red, green, blue);
		save_pixel(position + 3*local_width, red, green, blue);

		iter2 = iter_function(number_of_iterations, real+4*step_real, imag, c_real, c_imag);
		position = 15 * i + (i/(local_width/5))*(12*local_width) + 12;
		t = getT(iter2,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position - 3, red, green, blue);
		save_pixel(position + 3*local_width, red, green, blue);

		iter3 = iter_function(number_of_iterations+4*step_imag, real, imag, c_real, c_imag);
		position = 15 * i + (i/(local_width/5))*(12*local_width) + 12 * local_width;
		t = getT(iter3,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position + 3, red, green, blue);
		save_pixel(position - 3*local_width, red, green, blue);

		iter4 = iter_function(number_of_iterations+4*step_imag, real+4*step_real, imag, c_real, c_imag);
		position = 15 * i + (i/(local_width/5))*(12*local_width) + 12 * local_width + 12;
		t = getT(iter4,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position - 3, red, green, blue);
		save_pixel(position - 3*local_width, red, green, blue);

		iter5 = iter_function(number_of_iterations, real+2*step_real, imag+2*step_imag, c_real, c_imag);
		position = 15 * i + (i/(local_width/5))*(12*local_width) + 6 * local_width + 6;
		t = getT(iter5,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position + 3, red, green, blue);
		save_pixel(position - 3, red, green, blue);
		save_pixel(position + 3*local_width, red, green, blue);
		save_pixel(position - 3*local_width, red, green, blue);


		// + pixels between
		int iter = (iter2 + iter4 + iter5)/3;
		t = getT(iter,number_of_iterations);
		save_pixel(position + 6, getR(t), getG(t), getB(t));

		iter = (iter1 + iter3 + iter5)/3;
		t = getT(iter,number_of_iterations);
		save_pixel(position - 6, getR(t), getG(t), getB(t));

		iter = (iter1 + iter2 + iter5)/3;
		t = getT(iter,number_of_iterations);
		save_pixel(position - 6*local_width, getR(t), getG(t), getB(t));

		iter = (iter3 + iter4 + iter5)/3;
		t = getT(iter,number_of_iterations);
		save_pixel(position + 6*local_width, getR(t), getG(t), getB(t));

		iter = (iter1 + iter5)/2;
		t = getT(iter,number_of_iterations);
		save_pixel(position - 3*local_width - 3, getR(t), getG(t), getB(t));

		iter = (iter2 + iter5)/2;
		t = getT(iter,number_of_iterations);
		save_pixel(position - 3*local_width + 3, getR(t), getG(t), getB(t));

		iter = (iter3 + iter5)/2;
		t = getT(iter,number_of_iterations);
		save_pixel(position + 3*local_width - 3, getR(t), getG(t), getB(t));

		iter = (iter4 + iter5)/2;
		t = getT(iter,number_of_iterations);
		save_pixel(position + 3*local_width + 3, getR(t), getG(t), getB(t));
	}
}

void compute_function_predict_3(int cycle_start, int cycle_end, int local_width, int local_height, double c_real, double c_imag, int number_of_iterations, double min_real, double max_imag, double step_real, double step_imag)
{
	int iter1;	// this is going to save to iterations_buffer
	int iter2;
	int iter3;
	int iter4;
	int iter5;
	int iter6;
	int iter7;
	int iter8;

	float real;
	float imag;

	for (int i = 0; i < (local_width * local_height/100); i++) {
		float t;
		int position;
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		real = min_real + ((i*10)%local_width)*step_real;
		imag = max_imag + ((i*10)/local_width)*(step_imag*10);

		iter1 = iter_function(number_of_iterations, real, imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width);
		t = getT(iter1,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position + 3, red, green, blue);
		save_pixel(position + 6, red, green, blue);
		save_pixel(position + 9, red, green, blue);
		save_pixel(position + 3*local_width, red, green, blue);
		save_pixel(position + 3*local_width + 3, red, green, blue);
		save_pixel(position + 3*local_width + 6, red, green, blue);
		save_pixel(position + 6*local_width, red, green, blue);
		save_pixel(position + 6*local_width + 3, red, green, blue);
		save_pixel(position + 9*local_width, red, green, blue);

		iter2 = iter_function(number_of_iterations, real+3*9*step_real, imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*9;
		t = getT(iter2,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position - 3, red, green, blue);
		save_pixel(position - 6, red, green, blue);
		save_pixel(position - 9, red, green, blue);
		save_pixel(position + 3*local_width, red, green, blue);
		save_pixel(position + 3*local_width - 3, red, green, blue);
		save_pixel(position + 3*local_width - 6, red, green, blue);
		save_pixel(position + 6*local_width, red, green, blue);
		save_pixel(position + 6*local_width - 3, red, green, blue);
		save_pixel(position + 9*local_width, red, green, blue);

		iter3 = iter_function(number_of_iterations, real, imag+3*9*step_imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*local_width*9;
		t = getT(iter3,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position + 3, red, green, blue);
		save_pixel(position + 6, red, green, blue);
		save_pixel(position + 9, red, green, blue);
		save_pixel(position - 3*local_width, red, green, blue);
		save_pixel(position - 3*local_width + 3, red, green, blue);
		save_pixel(position - 3*local_width + 6, red, green, blue);
		save_pixel(position - 6*local_width, red, green, blue);
		save_pixel(position - 6*local_width + 3, red, green, blue);
		save_pixel(position - 9*local_width, red, green, blue);

		iter4 = iter_function(number_of_iterations, real, imag+3*9*step_imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*9 + 3*local_width*9;
		t = getT(iter4,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position - 3, red, green, blue);
		save_pixel(position - 6, red, green, blue);
		save_pixel(position - 9, red, green, blue);
		save_pixel(position - 3*local_width, red, green, blue);
		save_pixel(position - 3*local_width - 3, red, green, blue);
		save_pixel(position - 3*local_width - 6, red, green, blue);
		save_pixel(position - 6*local_width, red, green, blue);
		save_pixel(position - 6*local_width - 3, red, green, blue);
		save_pixel(position - 9*local_width, red, green, blue);

		iter5 = iter_function(number_of_iterations, real, imag+3*9*step_imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*2 + 3*local_width*2;
		t = getT(iter5,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position + 3, red, green, blue);
		save_pixel(position + 6, red, green, blue);
		save_pixel(position - 3*local_width + 3, red, green, blue);
		save_pixel(position - 3*local_width + 6, red, green, blue);
		save_pixel(position - 6*local_width + 6, red, green, blue);
		save_pixel(position + 3*local_width - 3, red, green, blue);
		save_pixel(position + 3*local_width, red, green, blue);
		save_pixel(position + 3*local_width + 3, red, green, blue);
		save_pixel(position + 3*local_width + 6, red, green, blue);
		save_pixel(position + 6*local_width - 6, red, green, blue);
		save_pixel(position + 6*local_width - 3, red, green, blue);
		save_pixel(position + 6*local_width, red, green, blue);
		save_pixel(position + 6*local_width + 3, red, green, blue);
		save_pixel(position + 6*local_width + 6, red, green, blue);

		iter6 = iter_function(number_of_iterations, real, imag+3*9*step_imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*7 + 3*local_width*2;
		t = getT(iter6,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position - 3, red, green, blue);
		save_pixel(position - 6, red, green, blue);
		save_pixel(position - 3*local_width - 3, red, green, blue);
		save_pixel(position - 3*local_width - 6, red, green, blue);
		save_pixel(position - 6*local_width - 6, red, green, blue);
		save_pixel(position + 3*local_width + 3, red, green, blue);
		save_pixel(position + 3*local_width, red, green, blue);
		save_pixel(position + 3*local_width - 3, red, green, blue);
		save_pixel(position + 3*local_width - 6, red, green, blue);
		save_pixel(position + 6*local_width - 6, red, green, blue);
		save_pixel(position + 6*local_width - 3, red, green, blue);
		save_pixel(position + 6*local_width, red, green, blue);
		save_pixel(position + 6*local_width + 3, red, green, blue);
		save_pixel(position + 6*local_width + 6, red, green, blue);

		iter7 = iter_function(number_of_iterations, real, imag+3*9*step_imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*2 + 3*local_width*7;
		t = getT(iter7,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position + 3, red, green, blue);
		save_pixel(position + 6, red, green, blue);
		save_pixel(position + 3*local_width + 3, red, green, blue);
		save_pixel(position + 3*local_width + 6, red, green, blue);
		save_pixel(position + 6*local_width + 6, red, green, blue);
		save_pixel(position - 3*local_width - 3, red, green, blue);
		save_pixel(position - 3*local_width, red, green, blue);
		save_pixel(position - 3*local_width + 3, red, green, blue);
		save_pixel(position - 3*local_width + 6, red, green, blue);
		save_pixel(position - 6*local_width - 6, red, green, blue);
		save_pixel(position - 6*local_width - 3, red, green, blue);
		save_pixel(position - 6*local_width, red, green, blue);
		save_pixel(position - 6*local_width + 3, red, green, blue);
		save_pixel(position - 6*local_width + 6, red, green, blue);

		iter8 = iter_function(number_of_iterations, real, imag+3*9*step_imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*7 + 3*local_width*7;
		t = getT(iter8,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position - 3, red, green, blue);
		save_pixel(position - 6, red, green, blue);
		save_pixel(position + 3*local_width - 3, red, green, blue);
		save_pixel(position + 3*local_width - 6, red, green, blue);
		save_pixel(position + 6*local_width - 6, red, green, blue);
		save_pixel(position - 3*local_width + 3, red, green, blue);
		save_pixel(position - 3*local_width, red, green, blue);
		save_pixel(position - 3*local_width - 3, red, green, blue);
		save_pixel(position - 3*local_width - 6, red, green, blue);
		save_pixel(position - 6*local_width - 6, red, green, blue);
		save_pixel(position - 6*local_width - 3, red, green, blue);
		save_pixel(position - 6*local_width, red, green, blue);
		save_pixel(position - 6*local_width + 3, red, green, blue);
		save_pixel(position - 6*local_width + 6, red, green, blue);
	}
}

void compute_function_predict_4(int cycle_start, int cycle_end, int local_width, int local_height, double c_real, double c_imag, int number_of_iterations, double min_real, double max_imag, double step_real, double step_imag)
{
	int iter1;	// this is going to save to iterations_buffer
	int iter2;
	int iter3;
	int iter4;

	float real;
	float imag;

	for (int i = 0; i < (local_width * local_height/100); i++) {
		float t;
		int position;
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		real = min_real + ((i*10)%local_width)*step_real;
		imag = max_imag + ((i*10)/local_width)*(step_imag*10);

		iter1 = iter_function(number_of_iterations, real, imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width);
		t = getT(iter1,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		for (uint8_t j = 0; j < 5; j ++) {
			for (uint8_t k = 0; k < 5; k ++) {
				save_pixel(position + j * 3 * local_width + k * 3, red, green, blue);
			}
		}

		iter2 = iter_function(number_of_iterations, real+3*9*step_real, imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*9;
		t = getT(iter2,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		for (uint8_t j = 0; j < 5; j ++) {
			for (uint8_t k = 0; k < 5; k ++) {
				save_pixel(position + j * 3 * local_width - k * 3, red, green, blue);
			}
		}

		iter3 = iter_function(number_of_iterations, real, imag+3*9*step_imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*local_width*9;
		t = getT(iter3,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		for (uint8_t j = 0; j < 5; j ++) {
			for (uint8_t k = 0; k < 5; k ++) {
				save_pixel(position - j * 3 * local_width + k * 3, red, green, blue);
			}
		}

		iter4 = iter_function(number_of_iterations, real, imag+3*9*step_imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*9 + 3*local_width*9;
		t = getT(iter4,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		for (uint8_t j = 0; j < 5; j ++) {
			for (uint8_t k = 0; k < 5; k ++) {
				save_pixel(position - j * 3 * local_width - k * 3, red, green, blue);
			}
		}
	}
}

void compute_function_predict_5(int cycle_start, int cycle_end, int local_width, int local_height, double c_real, double c_imag, int number_of_iterations, double min_real, double max_imag, double step_real, double step_imag)
{
	int iter1;	// this is going to save to iterations_buffer
	int iter2;

	float real;
	float imag;

	for (int i = 0; i < (local_width * local_height/100); i++) {
		float t;
		int position;
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		real = min_real + ((i*10)%local_width)*step_real;
		imag = max_imag + ((i*10)/local_width)*(step_imag*10);

		iter1 = iter_function(number_of_iterations, real, imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width);
		t = getT(iter1,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		for (uint8_t j = 0; j < 10; j++) {
			save_pixel(position + j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 8; j++) {
			save_pixel(position + 3*local_width + j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 8; j++) {
			save_pixel(position + 6*local_width + j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 6; j++) {
			save_pixel(position + 9*local_width + j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 6; j++) {
			save_pixel(position + 12*local_width + j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 4; j++) {
			save_pixel(position + 15*local_width + j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 4; j++) {
			save_pixel(position + 18*local_width + j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 2; j++) {
			save_pixel(position + 21*local_width + j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 2; j++) {
			save_pixel(position + 24*local_width + j*3, red, green, blue);
		}

		iter2 = iter_function(number_of_iterations, real+27*step_real, imag+27*step_imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 27*local_width + 27;
		t = getT(iter2,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		for (uint8_t j = 0; j < 10; j++) {
			save_pixel(position - j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 8; j++) {
			save_pixel(position - 3*local_width - j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 8; j++) {
			save_pixel(position - 6*local_width - j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 6; j++) {
			save_pixel(position - 9*local_width - j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 6; j++) {
			save_pixel(position - 12*local_width - j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 4; j++) {
			save_pixel(position - 15*local_width - j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 4; j++) {
			save_pixel(position - 18*local_width - j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 2; j++) {
			save_pixel(position - 21*local_width - j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 2; j++) {
			save_pixel(position - 24*local_width - j*3, red, green, blue);
		}
	}
}

void cpu_compute(global_buffer * all_buffers, global_data * all_data)
{
	local_all_data.width = all_data->width;
	local_all_data.height = all_data->height;
	local_all_data.min_real = all_data->min_real;
	local_all_data.max_imag = all_data->max_imag;
	local_all_data.step_imag = all_data->step_imag;
	local_all_data.step_real = all_data->step_real;
	local_all_data.number_of_iterations = all_data->number_of_iterations;
	local_all_data.c_real = all_data->c_real;
	local_all_data.c_imag = all_data->c_imag;
	local_all_buffers.picture_buffer = all_buffers->picture_buffer;

	// compute
	if (all_data->prediction == 0) {
		compute_function(0, local_all_data.width * local_all_data.height, local_all_data.width, local_all_data.height, local_all_data.c_real, local_all_data.c_imag, local_all_data.number_of_iterations, local_all_data.min_real, local_all_data.max_imag, local_all_data.step_real, local_all_data.step_imag);
	} else if (all_data->prediction == 1) {
		compute_function_predict_1(0, local_all_data.width * local_all_data.height, local_all_data.width, local_all_data.height, local_all_data.c_real, local_all_data.c_imag, local_all_data.number_of_iterations, local_all_data.min_real, local_all_data.max_imag, local_all_data.step_real, local_all_data.step_imag);
	} else if (all_data->prediction == 2) {
		compute_function_predict_2(0, local_all_data.width * local_all_data.height, local_all_data.width, local_all_data.height, local_all_data.c_real, local_all_data.c_imag, local_all_data.number_of_iterations, local_all_data.min_real, local_all_data.max_imag, local_all_data.step_real, local_all_data.step_imag);
	} else if (all_data->prediction == 3) {
		compute_function_predict_3(0, local_all_data.width * local_all_data.height, local_all_data.width, local_all_data.height, local_all_data.c_real, local_all_data.c_imag, local_all_data.number_of_iterations, local_all_data.min_real, local_all_data.max_imag, local_all_data.step_real, local_all_data.step_imag);
	} else if (all_data->prediction == 4) {
		compute_function_predict_4(0, local_all_data.width * local_all_data.height, local_all_data.width, local_all_data.height, local_all_data.c_real, local_all_data.c_imag, local_all_data.number_of_iterations, local_all_data.min_real, local_all_data.max_imag, local_all_data.step_real, local_all_data.step_imag);
	} else if (all_data->prediction == 5) {
		compute_function_predict_5(0, local_all_data.width * local_all_data.height, local_all_data.width, local_all_data.height, local_all_data.c_real, local_all_data.c_imag, local_all_data.number_of_iterations, local_all_data.min_real, local_all_data.max_imag, local_all_data.step_real, local_all_data.step_imag);
	}

	xwin_redraw(all_data->width, all_data->height,
		    local_all_buffers.picture_buffer);
	if (all_data->save_pictures) {
			def_color();
			fprintf(stderr,"\nINFO:  Saving picture to /tmp ...\n");
			xwin_save_image(all_buffers->picture_buffer);	// Save image
			char filename[40];
			struct tm *timenow;

			time_t now = time(NULL);
			timenow = gmtime(&now);
			strftime(filename, sizeof(filename), "/tmp/fractal_%Y%m%d%H%M%S.png", timenow);
			green_col();
			fprintf(stderr,"INFO:  Picture saved in %s\n",filename);
	}
	/*def_color();
	fprintf(stderr,"\nCurrent fractal settings:\n  Number of iterations: %d\n  Real: %5f   %5f\n  Imag: %5f  %5f\n  C:    %5f   %5f\n",
	     all_data->number_of_iterations, all_data->min_real,
	     all_data->max_real, all_data->min_imag, all_data->max_imag,
	     all_data->c_real, all_data->c_imag);*/
}
