/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  STEPAN MAROUSEK                   *
 * date:    2021/07/24                        *
 **********************************************/

#include "graphics.h"
#include "prediction.h"
#include "aliasing.h"
#include "../data/global_data.h"
#include "../system/terminal_colors.h"
#include "sdl_window.h"
#include "buttons.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>

global_data local_all_data;
global_buffer local_all_buffers;
global_buffer local_all_buffers2;

/* Save without SDL */
void save_picture(global_data *all_data)
{
	if (all_data->save_pictures) {
		char filenameppm[40];
		snprintf(filenameppm, 40, "fractal-%d.ppm", all_data->animation_frame);
		FILE *pictureOutput;
		pictureOutput = fopen(filenameppm, "w");
		fprintf(pictureOutput,"P6\n%d %d\n255\n", all_data->width, all_data->height);

		for (int i = 0; i < 3 * all_data->width * (all_data->height); i++) {
			fputc(local_all_buffers.picture_buffer[i], pictureOutput);
		}
		fclose(pictureOutput);
		all_data->animation_frame++;
	}
}

uint8_t getButton(int buttor_number, int position, int subpixel, int menuPosition)
{
	uint8_t outputPixel = 255;
	switch (buttor_number){
		case 1:
			outputPixel = buttonOne(3 * position + subpixel, menuPosition);
			break;

		case 2:
			outputPixel = buttonTwo(3 * position + subpixel, menuPosition);
			break;

		case 3:
			outputPixel = buttonThree(3 * position + subpixel, menuPosition);
			break;

		case 4:
			outputPixel = buttonFour(3 * position + subpixel, menuPosition);
			break;

		case 5:
			outputPixel = buttonFive(3 * position + subpixel, menuPosition);
			break;
	}
	return outputPixel;
}

float getT(int k, int number_of_iterations)
{
	float t = 0.96875 * k / number_of_iterations;
	return t;
}

uint8_t getR(float t)
{
	float r = 9.0 * (1 - t) * t * t * t * 255;
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
	local_all_buffers.picture_buffer[++position] = green;
	local_all_buffers.picture_buffer[++position] = blue;
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
	window_redraw(all_buffers->picture_buffer);
}

/* erase all values stored in iterations buffer */
void erase_buffer(global_buffer * all_buffers, global_data * all_data)
{
	for (int i = 0; i < all_data->width * all_data->height; i++) {
		all_buffers->iterations_buffer[i] = 1;
	}
	display_buffer(all_buffers, all_data);
}

/* return number of iterations for pixel */
int iter_function(int number_of_iterations, float real, float imag, double c_real, double c_imag)
{
	float newreal, newimag;
	int j = 1;

	for (j = 1; j <= number_of_iterations; j++) {
		newreal = real * real - imag * imag + c_real;
		newimag = 2 * real * imag + c_imag;
		real = newreal;
		imag = newimag;

		if ((real * real + imag * imag) >= 4) {
			break;
		}
	}
	return j;
}

/* compute line of pixels when moving up/down */
void compute_line(bool upperBool, global_buffer * all_buffers, global_data * all_data) {
	if (upperBool) {
		int firstIndexA1 = (all_data->height - 1) * 3 * all_data->width;
		for (int i = 0; i < all_data->height - 1; i++) {
			int firstIndexA =  firstIndexA1 - 3 * all_data->width * i;
			int firstIndexB = firstIndexA1 - 3 * all_data->width * (i + 1);
			for (int j = 0; j < all_data->width * 3; j++) {
				all_buffers->picture_buffer[firstIndexA + j] = all_buffers->picture_buffer[firstIndexB + j];
			}
		}

		int iter;
		float real = all_data->min_real;
		float imag = all_data->max_imag;

		for (int i = 0; i < all_data->width; i++) {
			iter = iter_function(all_data->number_of_iterations, real, imag, all_data->c_real, all_data->c_imag);
			float t = getT(iter,all_data->number_of_iterations);
			save_pixel(i * 3, getR(t), getG(t), getB(t));
			real = real + all_data->step_real;
		}

	} else {
		for (int i = 0; i < all_data->height - 1; i++) {
			int secondIndexA = i * 3 * all_data->width;
			int secondIndexB = (i + 1) * 3 * all_data->width;
			for (int j = 0; j < all_data->width * 3; j++) {
				all_buffers->picture_buffer[secondIndexA + j] = all_buffers->picture_buffer[secondIndexB + j];
			}
		}

		int iter;
		float real = all_data->min_real;
		float imag = all_data->min_imag;

		int thirdIndex = 3 * all_data->width * (all_data->height - 1);
		for (int i = 0; i < all_data->width; i++) {
			iter = iter_function(all_data->number_of_iterations, real, imag, all_data->c_real, all_data->c_imag);
			float t = getT(iter,all_data->number_of_iterations);
			save_pixel(thirdIndex + i * 3, getR(t), getG(t), getB(t));
			real = real + all_data->step_real;
		}
	}
}

/* compute column of pixels when moving left/right */
void compute_column(bool leftBool, global_buffer * all_buffers, global_data * all_data) {
	if (leftBool) {
		int firstIndexA1 = (all_data->width - 1) * 3;
		for (int i = 0; i < 3 * all_data->width; i++) {
			int firstIndexA = firstIndexA1 - i;
			int firstIndexB = firstIndexA - 3;
			for (int j = 0; j < all_data->height; j++) {
				all_buffers->picture_buffer[firstIndexA + 3 * all_data->width * j] = all_buffers->picture_buffer[firstIndexB + 3 * all_data->width * j];
			}
		}

		int iter;
		float imag = all_data->max_imag;
		float real = all_data->min_real;

		for (int i = 0; i < all_data->height; i++) {
			iter = iter_function(all_data->number_of_iterations, real, imag, all_data->c_real, all_data->c_imag);
			float t = getT(iter,all_data->number_of_iterations);
			save_pixel(i * 3 * all_data->width, getR(t), getG(t), getB(t));
			imag = imag + all_data->step_imag;
		}

	} else {
		for (int i = 0; i < 3 * (all_data->width - 1); i++) {
			for (int j = 0; j < all_data->height; j++) {
				all_buffers->picture_buffer[i + 3 * all_data->width * j] = all_buffers->picture_buffer[i + 3 + 3 * all_data->width * j];
			}
		}

		int iter;
		float imag = all_data->max_imag;
		float real = all_data->max_real;

		int secondIndex = 3 * (all_data->width - 1);
		for (int i = 0; i < all_data->height; i++) {
			iter = iter_function(all_data->number_of_iterations, real, imag, all_data->c_real, all_data->c_imag);
			float t = getT(iter,all_data->number_of_iterations);
			save_pixel(i * 3 * all_data->width + secondIndex, getR(t), getG(t), getB(t));
			imag = imag + all_data->step_imag;
		}
	}
}

/* no prediction */
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

/* choose prediction type and display ui buttons */
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
	local_all_data.prediction_10_steps = all_data->prediction_10_steps;

	local_all_buffers2.picture_buffer = all_buffers->picture_buffer;

	int    a1 = 0;
	int    a2 = local_all_data.width * local_all_data.height;
	int    a3 = local_all_data.width;
	int    a4 = local_all_data.height;
	double a5 = local_all_data.c_real;
	double a6 = local_all_data.c_imag;
	int    a7 = local_all_data.number_of_iterations;
	double a8 = local_all_data.min_real;
	double a9 = local_all_data.max_imag;
	double a10 = local_all_data.step_real;
	double a11 = local_all_data.step_imag;

	int a12 = local_all_data.prediction_10_steps;

	// prediction type
	switch(all_data->prediction) {
		case 0: // no prediction
			compute_function(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
			break;

		case 1: // simple prediction
			compute_function_predict_1(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
			break;

		case 2: // points
			compute_function_predict_2(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
			break;

		case 3: // pentagons
			compute_function_predict_3(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
			break;

		case 4: // squares
			compute_function_predict_4(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
			break;

		case 5: // triangles
			compute_function_predict_5(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
			break;

		case 10:
			compute_function_predict_10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
			break;

		case 11:
			compute_function_predict_11(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
			break;

		case 12:
			compute_function_predict_12(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
			break;

		case 13:
			compute_function_predict_13(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
			break;
	}

	if (all_data->aliasing != 0) {
		aliasing(all_data, local_all_buffers.picture_buffer, local_all_buffers2.picture_buffer, all_data->aliasing);
	}

	save_picture(all_data);

	for (int i = 3 * all_data->width * all_data->height; i < 3 * (all_data->width*all_data->height + 50 * all_data->width); i = i + 3) {
		local_all_buffers.picture_buffer[ i ] = 0;
		local_all_buffers.picture_buffer[i+1] = 7;
		local_all_buffers.picture_buffer[i+2] = 41;
	}
	for (int i = 0; i < 5000; i++) {
		int commonIndex = 3 * (all_data->width*all_data->height) + ((i%100) * 3 + (i/100) * all_data->width * 3);
		local_all_buffers.picture_buffer[commonIndex       ] = getButton(1, i, 0, all_data->menuPosition);
		local_all_buffers.picture_buffer[commonIndex + 1   ] = getButton(1, i, 1, all_data->menuPosition);
		local_all_buffers.picture_buffer[commonIndex + 2   ] = getButton(1, i, 2, all_data->menuPosition);
		local_all_buffers.picture_buffer[commonIndex + 300 ] = getButton(2, i, 0, all_data->menuPosition);
		local_all_buffers.picture_buffer[commonIndex + 301 ] = getButton(2, i, 1, all_data->menuPosition);
		local_all_buffers.picture_buffer[commonIndex + 302 ] = getButton(2, i, 2, all_data->menuPosition);
		local_all_buffers.picture_buffer[commonIndex + 600 ] = getButton(3, i, 0, all_data->menuPosition);
		local_all_buffers.picture_buffer[commonIndex + 601 ] = getButton(3, i, 1, all_data->menuPosition);
		local_all_buffers.picture_buffer[commonIndex + 602 ] = getButton(3, i, 2, all_data->menuPosition);
		local_all_buffers.picture_buffer[commonIndex + 900 ] = getButton(4, i, 0, all_data->menuPosition);
		local_all_buffers.picture_buffer[commonIndex + 901 ] = getButton(4, i, 1, all_data->menuPosition);
		local_all_buffers.picture_buffer[commonIndex + 902 ] = getButton(4, i, 2, all_data->menuPosition);
		local_all_buffers.picture_buffer[commonIndex + 1200] = getButton(5, i, 0, all_data->menuPosition);
		local_all_buffers.picture_buffer[commonIndex + 1201] = getButton(5, i, 1, all_data->menuPosition);
		local_all_buffers.picture_buffer[commonIndex + 1202] = getButton(5, i, 2, all_data->menuPosition);
	}

	//all_data->menuPosition++;
	//all_data->menuPosition = all_data->menuPosition % 11;

	window_redraw(local_all_buffers.picture_buffer);

	/*def_color();
	fprintf(stderr,"\nCurrent fractal settings:\n  Number of iterations: %d\n  Real: %5f   %5f\n  Imag: %5f  %5f\n  C:    %5f   %5f\n",
	     all_data->number_of_iterations, all_data->min_real,
	     all_data->max_real, all_data->min_imag, all_data->max_imag,
	     all_data->c_real, all_data->c_imag);*/
}
