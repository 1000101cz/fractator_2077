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

double getT(int k, int number_of_iterations)
{
	double t = 1.0 * k / number_of_iterations;
	return t;
}

uint8_t getR(double t)
{
	double r = 9.0 * (1 - t) * t * t * t * 255;
	return (uint8_t) r;
}

uint8_t getG(double t)
{
	double g = 15.0 * (1 - t) * (1 - t) * t * t * 255;
	return (uint8_t) g;
}

uint8_t getB(double t)
{
	double b = 8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255;
	return (uint8_t) b;
}

/* compute RGB subpixels and display fractal to SDL window */
void display_buffer(global_buffer * all_buffers, global_data * all_data)
{
	double t;

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

void cpu_compute(global_buffer * all_buffers, global_data * all_data)
{
	int iter;	// this is going to save to iterations_buffer
	float real, newreal;
	float imag, newimag;
	float abs_value;

	for (int i = 0; i < all_data->width * all_data->height; i++) {

		/* find coordinates */
		real = all_data->min_real + i * all_data->step_real;
		imag = all_data->max_imag;

		while (!(real < all_data->max_real)) {
			real = real - all_data->width * all_data->step_real;
			imag = imag + all_data->step_imag;
		}

		/* compute number of iterations */
		for (int j = 1; j <= all_data->number_of_iterations; j++) {
			newreal = real * real - imag * imag + all_data->c_real;
			newimag = 2 * real * imag + all_data->c_imag;
			real = newreal;
			imag = newimag;

			abs_value = real * real + imag * imag;

			if (abs_value >= 4) {
				iter = j;
				break;
			}

			if (j == all_data->number_of_iterations) {
				iter = j;
				break;
			}
		}

		/* RGB colors to picture matrix */
		float t = getT(iter,all_data->number_of_iterations);
		all_buffers->picture_buffer[3 * i + 0] = getR(t);
		all_buffers->picture_buffer[3 * i + 1] = getG(t);
		all_buffers->picture_buffer[3 * i + 2] = getB(t);
	}
	xwin_redraw(all_data->width, all_data->height,
		    all_buffers->picture_buffer);
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
