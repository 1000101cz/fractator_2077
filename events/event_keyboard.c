/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  STEPAN MAROUSEK                   *
 * date:    2021/6/15                         *
 **********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <termios.h>
#include <unistd.h>
#include <SDL.h>
#include <SDL_image.h>
#include <pthread.h>
#include <time.h>
#include <inttypes.h>
#include <math.h>

#include "event_queue.h"
#include "event_keyboard.h"
#include "../system/terminal_colors.h"
#include "../data/global_data.h"
#include "../graphics/graphics.h"
#include "../graphics/sdl_window.h"
#include "../animation/animations.h"


/* start animation */
void animation(global_data * all_data, global_buffer * all_buffers)
{
	if (all_data->audio) {
		audio_animation(all_data, all_buffers);
	}


	else {  // audio animation OFF
		pure_animation(all_data, all_buffers);
	}
}

/* handle keyboard events */
void event_keyboard_ev(event * ev, data_t * data,
		       _Bool *computing, uint16_t * chunk_id,
		       global_data * all_data, global_buffer * all_buffers)
{

	double middle_imag;
	double middle_real;
	double distance_imag;
	double distance_real;

	switch (ev->type) {
	case EV_QUIT: // prepare abort packet
		free(all_buffers->picture_buffer);
	  free(all_buffers->iterations_buffer);
		green_col();
		fprintf(stderr, "\nINFO:  Quit\n");
		break;

	case EV_RESET_CHUNK:
		if (!*computing) {
			green_col();
			fprintf(stderr, "\nINFO:  Chunk reset request\n");
			chunk_id = 0;
		} else {
			yellow_col();
			fprintf(stderr,
				"WARN:  Chunk reset request discarded, it is currently computing\n");
		}
		break;

	case EV_COMPUTE_CPU: // compute fractal on pc
		cpu_compute(all_buffers, all_data);
		if (all_data->prediction == 10) {
			for (int i = 1; i < all_data->prediction_10_steps; i++){
				cpu_compute(all_buffers, all_data);
			}
		}
		green_col();
		fprintf(stderr, "INFO:  PC computation done\n");
		break;

	case EV_CLEAR_BUFFER: // erase picture buffers
		erase_buffer(all_buffers, all_data);
		def_color();
		fprintf(stderr, "INFO:  Buffer erased\n");
		break;

	case EV_REFRESH: // display current buffer data to SDL window
		display_buffer(all_buffers, all_data);
		def_color();
		fprintf(stderr,
			"\nINFO:  Display reseted with actual buffer values\n");
		break;

	case EV_RESIZE_PLUS: // increase SDL window size
		all_data->width = all_data->width * (6.0 / 5.0);
		all_data->height = all_data->height * (6.0 / 5.0);
		all_data->step_real = (fabs(all_data->max_real) + fabs(all_data->min_real)) / all_data->width;
		all_data->step_imag = -(fabs(all_data->max_imag) + fabs(all_data->min_imag)) / all_data->height;

		free(all_buffers->iterations_buffer);
		free(all_buffers->picture_buffer);

		all_buffers->iterations_buffer = malloc(sizeof(int) * all_data->width * all_data->height);
		all_buffers->picture_buffer = malloc(3 * sizeof(int) * all_data->width * all_data->height);

		window_resize(all_data);
		cpu_compute(all_buffers, all_data);
		if (all_data->prediction == 10) {
			for (int i = 1; i < all_data->prediction_10_steps; i++){
				cpu_compute(all_buffers, all_data);
			}
		}
		break;

	case EV_RESIZE_MINUS: // decrease SDL window size
		if ((5.0 / 6.0) * all_data->width > 500) {
			all_data->width = all_data->width * (5.0 / 6.0);
			all_data->height = all_data->height * (5.0 / 6.0);
			all_data->step_real = (fabs(all_data->max_real) + fabs(all_data->min_real)) / all_data->width;
			all_data->step_imag = -(fabs(all_data->max_imag) + fabs(all_data->min_imag)) / all_data->height;

			free(all_buffers->iterations_buffer);
			free(all_buffers->picture_buffer);

			all_buffers->iterations_buffer = malloc(sizeof(int) * all_data->width * all_data->height);
			all_buffers->picture_buffer = malloc(3 * sizeof(int) * all_data->width * all_data->height);

			window_resize(all_data);
			cpu_compute(all_buffers, all_data);
			if (all_data->prediction == 10) {
				for (int i = 1; i < all_data->prediction_10_steps; i++){
					cpu_compute(all_buffers, all_data);
				}
			}
		} else {
			yellow_col();
			fprintf(stderr,
				"WARN:  Cannot decrease size of window\n");
		}
		break;

	case EV_MOVE_UP: // move picture
		for (int i = 0; i < 1; i++) {
			all_data->max_imag = all_data->max_imag - all_data->step_imag;
			all_data->min_imag = all_data->min_imag - all_data->step_imag;
			compute_line(1, all_buffers, all_data);
		}
		window_redraw(all_buffers->picture_buffer);
		break;

	case EV_MOVE_DOWN: // move picture
		for (int i = 0; i < 1; i++) {
			all_data->max_imag = all_data->max_imag + all_data->step_imag;
			all_data->min_imag = all_data->min_imag + all_data->step_imag;
			compute_line(0, all_buffers, all_data);
		}
		window_redraw(all_buffers->picture_buffer);
		break;

	case EV_MOVE_LEFT: // move picture
		for (int i = 0; i < 1; i++) {
			all_data->max_real = all_data->max_real - all_data->step_real;
			all_data->min_real = all_data->min_real - all_data->step_real;
			compute_column(1, all_buffers, all_data);
		}
		window_redraw(all_buffers->picture_buffer);
		break;

	case EV_MOVE_RIGHT: // move picture
		for (int i = 0; i < 1; i++) {
			all_data->max_real = all_data->max_real + all_data->step_real;
			all_data->min_real = all_data->min_real + all_data->step_real;
			compute_column(0, all_buffers, all_data);
		}
		window_redraw(all_buffers->picture_buffer);
		break;

	case EV_INCREASE_ITER: // increase number of iterations
		all_data->number_of_iterations = all_data->number_of_iterations + 5;
		cpu_compute(all_buffers, all_data);
		if (all_data->prediction == 10) {
			for (int i = 1; i < all_data->prediction_10_steps; i++){
				cpu_compute(all_buffers, all_data);
			}
		}
		break;

	case EV_DECREASE_ITER: // decrease number of iterations
		all_data->number_of_iterations = all_data->number_of_iterations - 5;
		cpu_compute(all_buffers, all_data);
		if (all_data->prediction == 10) {
			for (int i = 1; i < all_data->prediction_10_steps; i++){
				cpu_compute(all_buffers, all_data);
			}
		}
		break;

	case EV_CHANGE_1: // increase c value
		all_data->c_real = all_data->c_real + 0.025;
		all_data->c_imag = all_data->c_imag + 0.035;
		cpu_compute(all_buffers, all_data);
		if (all_data->prediction == 10) {
			for (int i = 1; i < all_data->prediction_10_steps; i++){
				cpu_compute(all_buffers, all_data);
			}
		}
		break;

	case EV_CHANGE_2: // zoom out
		middle_imag = (all_data->max_imag + all_data->min_imag) / 2;
		middle_real = (all_data->max_real + all_data->min_real) / 2;
		distance_imag = fabs(all_data->max_imag - middle_imag);
		distance_real = fabs(all_data->max_real - middle_real);

		all_data->max_imag = middle_imag + distance_imag * 2;
		all_data->min_imag = middle_imag - distance_imag * 2;
		all_data->max_real = middle_real + distance_real * 2;
		all_data->min_real = middle_real - distance_real * 2;
		all_data->step_imag = - (distance_imag * 4) / all_data->height;
		all_data->step_real = (distance_real * 4) / all_data->width;
		all_data->current_real = all_data->min_real;
		all_data->current_imag = all_data->max_imag;
		cpu_compute(all_buffers, all_data);
		if (all_data->prediction == 10) {
			for (int i = 1; i < all_data->prediction_10_steps; i++){
				cpu_compute(all_buffers, all_data);
			}
		}
		break;

	case EV_CHANGE_3: // decrease c value
		all_data->c_real = all_data->c_real - 0.025;
		all_data->c_imag = all_data->c_imag - 0.035;
		cpu_compute(all_buffers, all_data);
		if (all_data->prediction == 10) {
			for (int i = 1; i < all_data->prediction_10_steps; i++){
				cpu_compute(all_buffers, all_data);
			}
		}
		break;

	case EV_CHANGE_4: // zoom in
		middle_imag = (all_data->max_imag + all_data->min_imag) / 2;
		middle_real = (all_data->max_real + all_data->min_real) / 2;
		distance_imag = fabs(all_data->max_imag - middle_imag);
		distance_real = fabs(all_data->max_real - middle_real);

		all_data->max_imag = middle_imag + distance_imag / 2;
		all_data->min_imag = middle_imag - distance_imag / 2;
		all_data->max_real = middle_real + distance_real / 2;
		all_data->min_real = middle_real - distance_real / 2;
		all_data->step_imag = - distance_imag / all_data->height;
		all_data->step_real = distance_real / all_data->width;
		all_data->current_real = all_data->min_real;
		all_data->current_imag = all_data->max_imag;
		cpu_compute(all_buffers, all_data);
		if (all_data->prediction == 10) {
			for (int i = 1; i < all_data->prediction_10_steps; i++){
				cpu_compute(all_buffers, all_data);
			}
		}
		break;

	case EV_ANIMATION: // start animation
		animation(all_data, all_buffers);
		break;

	default:
		break;
	}
}
