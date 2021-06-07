/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  STEPAN MAROUSEK                   *
 * date:    05/16/2021                        *
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

#include "event_queue.h"
#include "event_keyboard.h"
#include "terminal_colors.h"
#include "global_data.h"
#include "graphics.h"
#include "xwin_sdl.h"

/* start animation */
void animation(global_data * all_data, global_buffer * all_buffers)
{

	for (int i = 0; i < 960; i++) {
		cpu_compute(all_buffers, all_data);
		all_data->c_real = all_data->c_real + 0.001;
		all_data->c_imag = all_data->c_imag + 0.001;
	}
	cpu_compute(all_buffers, all_data);

	for (int i = 0; i < 640; i++) {
		all_data->c_real = all_data->c_real - 0.0015;
		all_data->c_imag = all_data->c_imag - 0.0015;
		cpu_compute(all_buffers, all_data);
	}
	green_col();
	fprintf(stderr,"\nINFO: Animation ended\n");
}

/* handle keyboard events */
void event_keyboard_ev(event * ev, data_t * data,
		       _Bool *computing, uint16_t * chunk_id,
		       global_data * all_data, global_buffer * all_buffers)
{

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

		if (all_data->width == 660) {
			all_data->width = 840;
			all_data->height = 540;
			all_data->step_real = (fabs(all_data->max_real) + fabs(all_data->min_real)) / all_data->width;
			all_data->step_imag = -(fabs(all_data->max_imag) + fabs(all_data->min_imag)) / all_data->height;

			free(all_buffers->iterations_buffer);
			free(all_buffers->picture_buffer);

			all_buffers->iterations_buffer = malloc(sizeof(int) * all_data->width * all_data->height);
			all_buffers->picture_buffer = malloc(3 * sizeof(int) * all_data->width * all_data->height);

			xwin_resize(all_data, all_buffers->picture_buffer);
			cpu_compute(all_buffers, all_data);

		} else if (all_data->width == 480) {	// change step
			all_data->width = 660;
			all_data->height = 480;
			all_data->step_real = (fabs(all_data->max_real) + fabs(all_data->min_real)) / all_data->width;
			all_data->step_imag = -(fabs(all_data->max_imag) + fabs(all_data->min_imag)) / all_data->height;

			free(all_buffers->iterations_buffer);
			free(all_buffers->picture_buffer);

			all_buffers->iterations_buffer = malloc(sizeof(int) * all_data->width * all_data->height);
			all_buffers->picture_buffer = malloc(3 * sizeof(int) * all_data->width * all_data->height);

			// close window and create bigger one
			xwin_resize(all_data, all_buffers->picture_buffer);
			cpu_compute(all_buffers, all_data);

		} else {
			yellow_col();
			fprintf(stderr,
				"WARN:  Cannot increase size of window\n");
		}
		break;

	case EV_RESIZE_MINUS: // decrease SDL window size
		if (all_data->width == 840) {
			all_data->width = 660;
			all_data->height = 480;
			all_data->step_real = (fabs(all_data->max_real) + fabs(all_data->min_real)) / all_data->width;
			all_data->step_imag = -(fabs(all_data->max_imag) + fabs(all_data->min_imag)) / all_data->height;

			free(all_buffers->iterations_buffer);
			free(all_buffers->picture_buffer);

			all_buffers->iterations_buffer = malloc(sizeof(int) * all_data->width * all_data->height);
			all_buffers->picture_buffer = malloc(3 * sizeof(int) * all_data->width * all_data->height);

			// close window and create bigger one
			xwin_resize(all_data, all_buffers->picture_buffer);
			cpu_compute(all_buffers, all_data);

		} else if (all_data->width == 660) {
			all_data->width = 480;
			all_data->height = 420;
			all_data->step_real = (fabs(all_data->max_real) + fabs(all_data->min_real)) / all_data->width;
			all_data->step_imag = -(fabs(all_data->max_imag) + fabs(all_data->min_imag)) / all_data->height;

			free(all_buffers->iterations_buffer);
			free(all_buffers->picture_buffer);

			all_buffers->iterations_buffer = malloc(sizeof(int) * all_data->width * all_data->height);
			all_buffers->picture_buffer = malloc(3 * sizeof(int) * all_data->width * all_data->height);

			// close window and create bigger one
			xwin_resize(all_data, all_buffers->picture_buffer);
			cpu_compute(all_buffers, all_data);
		} else {
			yellow_col();
			fprintf(stderr,
				"WARN:  Cannot decrease size of window\n");
		}
		break;

	case EV_MOVE_UP: // move picture
		all_data->max_imag = all_data->max_imag - all_data->step_imag * 45;
		all_data->min_imag = all_data->min_imag - all_data->step_imag * 45;
		all_data->current_real = all_data->min_real;
		all_data->current_imag = all_data->max_imag;
		cpu_compute(all_buffers, all_data);
		break;

	case EV_MOVE_DOWN: // move picture
		all_data->max_imag = all_data->max_imag + all_data->step_imag * 45;
		all_data->min_imag = all_data->min_imag + all_data->step_imag * 45;
		all_data->current_real = all_data->min_real;
		all_data->current_imag = all_data->max_imag;
		cpu_compute(all_buffers, all_data);
		break;

	case EV_MOVE_LEFT: // move picture
		all_data->max_real = all_data->max_real - all_data->step_real * 45;
		all_data->min_real = all_data->min_real - all_data->step_real * 45;
		all_data->current_real = all_data->min_real;
		all_data->current_imag = all_data->max_imag;
		cpu_compute(all_buffers, all_data);
		break;

	case EV_MOVE_RIGHT: // move picture
		all_data->max_real = all_data->max_real + all_data->step_real * 45;
		all_data->min_real = all_data->min_real + all_data->step_real * 45;
		all_data->current_real = all_data->min_real;
		all_data->current_imag = all_data->max_imag;
		cpu_compute(all_buffers, all_data);
		break;

	case EV_INCREASE_ITER: // increase number of iterations
		all_data->number_of_iterations = all_data->number_of_iterations + 5;
		cpu_compute(all_buffers, all_data);
		break;

	case EV_DECREASE_ITER: // decrease number of iterations
		all_data->number_of_iterations = all_data->number_of_iterations - 5;
		cpu_compute(all_buffers, all_data);
		break;

	case EV_CHANGE_1: // increase c value
		all_data->c_real = all_data->c_real + 0.025;
		all_data->c_imag = all_data->c_imag + 0.035;
		cpu_compute(all_buffers, all_data);
		break;

	case EV_CHANGE_2: // zoom out
		all_data->max_imag = all_data->max_imag * 2;
		all_data->min_imag = all_data->min_imag * 2;
		all_data->step_imag = -(fabs(all_data->max_imag) + fabs(all_data->min_imag)) / all_data->height;
		all_data->max_real = all_data->max_real * 2;
		all_data->min_real = all_data->min_real * 2;
		all_data->step_real = (fabs(all_data->max_real) + fabs(all_data->min_real)) / all_data->width;
		all_data->current_real = all_data->min_real;
		all_data->current_imag = all_data->max_imag;
		cpu_compute(all_buffers, all_data);
		break;

	case EV_CHANGE_3: // decrease c value
		all_data->c_real = all_data->c_real - 0.025;
		all_data->c_imag = all_data->c_imag - 0.035;
		cpu_compute(all_buffers, all_data);
		break;

	case EV_CHANGE_4: // zoom in
		all_data->max_imag = all_data->max_imag * 0.5;
		all_data->min_imag = all_data->min_imag * 0.5;
		all_data->step_imag = -(fabs(all_data->max_imag) + fabs(all_data->min_imag)) / all_data->height;
		all_data->max_real = all_data->max_real * 0.5;
		all_data->min_real = all_data->min_real * 0.5;
		all_data->step_real = (fabs(all_data->max_real) + fabs(all_data->min_real)) / all_data->width;
		all_data->current_real = all_data->min_real;
		all_data->current_imag = all_data->max_imag;
		cpu_compute(all_buffers, all_data);
		break;

	case EV_ANIMATION: // start animation
		animation(all_data, all_buffers);
		break;

	default:
		break;
	}
}
