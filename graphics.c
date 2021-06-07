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

int thr_1_start;
int thr_1_end;

int thr_2_start;
int thr_2_end;

int thr_3_start;
int thr_3_end;

int thr_4_start;
int thr_4_end;

_Bool thread1c;
_Bool thread2c;
_Bool thread3c;
_Bool thread4c;

global_data local_all_data;
global_buffer local_all_buffers;

float getT(int k, int number_of_iterations)
{
	float t = 0.98 * k / number_of_iterations;
	return t;
}

uint8_t getR(float t)
{
	float r = 0;
	return (uint8_t) r;
}

uint8_t getG(float t)
{
	float g = 1700 * (1 - t) * (1 - t) * (1 - t) * t;
	return (uint8_t) g;
}

uint8_t getB(float t)
{
	float b = 0;
	return (uint8_t) b;
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
	int iter1;	// this is going to save to iterations_buffer
	int iter2;
	int iter3;

	float real;
	float imag;

	real = min_real;
	imag = max_imag;
	iter1 = iter_function(number_of_iterations, real, imag, c_real, c_imag);
	float t = getT(iter1,number_of_iterations);
	local_all_buffers.picture_buffer[0] = getR(t);
	local_all_buffers.picture_buffer[1] = getG(t);
	local_all_buffers.picture_buffer[2] = getB(t);

	for (int i = 1; i < local_width * local_height; i++) {
		if ((i+1)%local_width == 0) {
			int position = 3 * i;
			real = min_real + (i%local_width)*step_real;
			imag = max_imag + (i/local_width)*step_imag;
			iter2 = iter_function(number_of_iterations, real, imag, c_real, c_imag);
			float t = getT(iter2,number_of_iterations);
			local_all_buffers.picture_buffer[position] = getR(t);
			local_all_buffers.picture_buffer[position + 1] = getG(t);
			local_all_buffers.picture_buffer[position + 2] = getB(t);
		}
		else {
			int position = 3 * (i + 1);
			real = min_real + ((i+1)%local_width)*step_real;
			imag = max_imag + ((i+1)/local_width)*step_imag;
			iter3 = iter_function(number_of_iterations, real, imag, c_real, c_imag);
			float t = getT(iter3,number_of_iterations);
			local_all_buffers.picture_buffer[position] = getR(t);
			local_all_buffers.picture_buffer[position + 1] = getG(t);
			local_all_buffers.picture_buffer[position + 2] = getB(t);

			if (iter3 == iter1) {
				int position = 3 * (i);
				local_all_buffers.picture_buffer[position] = getR(t);
				local_all_buffers.picture_buffer[position + 1] = getG(t);
				local_all_buffers.picture_buffer[position + 2] = getB(t);
			}
			else {
				int position = 3 * i;
				real = min_real + (i%local_width)*step_real;
				imag = max_imag + (i/local_width)*step_imag;
				iter2 = iter_function(number_of_iterations, real, imag, c_real, c_imag);
				float t = getT(iter2,number_of_iterations);
				local_all_buffers.picture_buffer[position] = getR(t);
				local_all_buffers.picture_buffer[position + 1] = getG(t);
				local_all_buffers.picture_buffer[position + 2] = getB(t);
			}
			i++;
			iter1 = iter3;
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

	thr_1_start = 0;
	thr_1_end = all_data->width * all_data->height;

	// compute
	compute_function(thr_1_start, thr_1_end, local_all_data.width, local_all_data.height, local_all_data.c_real, local_all_data.c_imag, local_all_data.number_of_iterations, local_all_data.min_real, local_all_data.max_imag, local_all_data.step_real, local_all_data.step_imag);

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
	def_color();
	fprintf(stderr,"\nCurrent fractal settings:\n  Number of iterations: %d\n  Real: %5f   %5f\n  Imag: %5f  %5f\n  C:    %5f   %5f\n",
	     all_data->number_of_iterations, all_data->min_real,
	     all_data->max_real, all_data->min_imag, all_data->max_imag,
	     all_data->c_real, all_data->c_imag);
}
