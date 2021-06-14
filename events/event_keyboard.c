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
#include <time.h>
#include <inttypes.h>
#include <math.h>

#include "event_queue.h"
#include "event_keyboard.h"
#include "../system/terminal_colors.h"
#include "../data/global_data.h"
#include "../graphics/graphics.h"
#include "../graphics/xwin_sdl.h"

#define MUSIC_FPS 30
#define MUSIC_HZ 44100

void *music_thread(void *);

/* start animation */
void animation(global_data * all_data, global_buffer * all_buffers)
{
	if (all_data->audio) {
		def_color();
	 	fprintf(stderr,"\nINFO: Audio animation started\n");
		data_t data = {.quit = false,.fd = -1, };

		enum { MUSIC, NUM_THREADS_TWO };
		void *(*thr_functions[])(void *) = { music_thread};
		pthread_t threads[NUM_THREADS_TWO];

		pthread_create(&threads[0], NULL, thr_functions[0], &data);

		long ms1; // microseconds
		long ms2;
		struct timespec spec;

		FILE *audioIn;
		audioIn = fopen("audioAnimation.wav","r");
		int element;
		for (int i = 0; i < 50; i++) {
			element = fgetc(audioIn);
		}

		_Bool last_min = 1;
		_Bool last_min2 = 1;
		int h1,h2;
		long long frame_volume;
		long long frame_volume_increm;
		long long frame_volume_increm2;
		int amplitudes;

		int element1, element2;
		element1 = fgetc(audioIn);
		element2 = fgetc(audioIn);
		while ((element != EOF) && (element1 != EOF) && (element2 != EOF)) {
	    clock_gettime(CLOCK_REALTIME, &spec);
	    ms1 = round(spec.tv_nsec / 1.0e3); // Convert nanoseconds to microseconds
			//fprintf(stderr,"\nmicroseconds: %ld\n",ms);

			cpu_compute(all_buffers, all_data);

			h1 = 127;
			h2 = 127;
			frame_volume = 0;
			frame_volume_increm = 0;
			frame_volume_increm2 = 0;
			amplitudes = 0;
			for (int i = 0; (i < MUSIC_HZ/MUSIC_FPS) && (element1 != EOF) && (element2 != EOF); i++) {
				element1 = fgetc(audioIn);
				element2 = fgetc(audioIn);
				if (element1 == EOF || element2 == EOF) {
					break;
				}
				/* element 1 */
				if (last_min) {
					if (element1 > h1) {
						frame_volume_increm = element1 - h1;
					} else if (element1 < h1) {
						last_min = 0;
						frame_volume = frame_volume + frame_volume_increm;
						amplitudes++;
						frame_volume_increm = 0;
						h1 = element1;
					}
				} else {
					if (element1 < h1) {
						frame_volume_increm = frame_volume_increm + h1 - element1;
					} else if (element1 > h1) {
						last_min = 1;
						frame_volume = frame_volume + frame_volume_increm;
						amplitudes++;
						frame_volume_increm = 0;
						h1 = element1;
					}
				}

				/* element 2 */
				if (last_min2) {
					if (element2 > h2) {
						frame_volume_increm2 = fabs(element2 - h2);
					} else if (element2 < h2) {
						last_min2 = 0;
						frame_volume = frame_volume + fabs(frame_volume_increm2);
						amplitudes++;
						frame_volume_increm2 = 0;
						h2 = element2;
					}
				} else {
					if (element2 < h2) {
						frame_volume_increm2 = frame_volume_increm2 + fabs(h2 - element2);
					} else if (element2 > h2) {
						last_min2 = 1;
						frame_volume = frame_volume + frame_volume_increm2;
						amplitudes++;
						frame_volume_increm2 = 0;
						h2 = element2;
					}
				}
			}
			if (element1 == EOF || element2 == EOF) {
				break;
			}
			//printf("\nFrame volume: %lld\nFrame amplitudes: %d\n\n",frame_volume,amplitudes);

			all_data->c_real = -1.0 + frame_volume*frame_volume/120000000000.0;
			all_data->c_imag = 0.4 - frame_volume/400000.0;

			if (amplitudes != 0) {
				all_data->number_of_iterations = (40 + 2000000/(amplitudes*amplitudes*amplitudes))%200;
			}

			clock_gettime(CLOCK_REALTIME, &spec);
	    ms2 = round(spec.tv_nsec / 1.0e3); // Convert nanoseconds to microseconds
			if (ms2 > ms1) {
				if ((ms2 - ms1) < (1000000/MUSIC_FPS)) {
					usleep((1000000/MUSIC_FPS) - (ms2 - ms1));
				}
				else {
					//fprintf(stderr,"\ncomputation is slow...\n");
				}
			}

		}
		pthread_cancel(threads[0]);
		fclose(audioIn);

		green_col();
		fprintf(stderr,"\nINFO: Animation ended\n");
	}


	else {  // audio animation OFF
		for (int i = 0; i < 960; i++) {
			cpu_compute(all_buffers, all_data);
			all_data->c_real = all_data->c_real + 0.0009765625;
			all_data->c_imag = all_data->c_imag + 0.0009765625;
		}
		cpu_compute(all_buffers, all_data);

		for (int i = 0; i < 480; i++) {
			all_data->c_real = all_data->c_real - 0.001953125;
			all_data->c_imag = all_data->c_imag - 0.001953125;
			cpu_compute(all_buffers, all_data);
		}
		if (all_data->prediction == 10) {
			for (int i = 0; i < all_data->prediction_10_steps; i++) {
				cpu_compute(all_buffers, all_data);
			}
		}
		green_col();
		fprintf(stderr,"\nINFO: Animation ended\n");
	}
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
			if (all_data->prediction == 10) {
				for (int i = 1; i < all_data->prediction_10_steps; i++){
					cpu_compute(all_buffers, all_data);
				}
			}

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
			if (all_data->prediction == 10) {
				for (int i = 1; i < all_data->prediction_10_steps; i++){
					cpu_compute(all_buffers, all_data);
				}
			}

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
			if (all_data->prediction == 10) {
				for (int i = 1; i < all_data->prediction_10_steps; i++){
					cpu_compute(all_buffers, all_data);
				}
			}

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
		all_data->max_imag = all_data->max_imag - all_data->step_imag * 45;
		all_data->min_imag = all_data->min_imag - all_data->step_imag * 45;
		all_data->current_real = all_data->min_real;
		all_data->current_imag = all_data->max_imag;
		cpu_compute(all_buffers, all_data);
		if (all_data->prediction == 10) {
			for (int i = 1; i < all_data->prediction_10_steps; i++){
				cpu_compute(all_buffers, all_data);
			}
		}
		break;

	case EV_MOVE_DOWN: // move picture
		all_data->max_imag = all_data->max_imag + all_data->step_imag * 45;
		all_data->min_imag = all_data->min_imag + all_data->step_imag * 45;
		all_data->current_real = all_data->min_real;
		all_data->current_imag = all_data->max_imag;
		cpu_compute(all_buffers, all_data);
		if (all_data->prediction == 10) {
			for (int i = 1; i < all_data->prediction_10_steps; i++){
				cpu_compute(all_buffers, all_data);
			}
		}
		break;

	case EV_MOVE_LEFT: // move picture
		all_data->max_real = all_data->max_real - all_data->step_real * 45;
		all_data->min_real = all_data->min_real - all_data->step_real * 45;
		all_data->current_real = all_data->min_real;
		all_data->current_imag = all_data->max_imag;
		cpu_compute(all_buffers, all_data);
		if (all_data->prediction == 10) {
			for (int i = 1; i < all_data->prediction_10_steps; i++){
				cpu_compute(all_buffers, all_data);
			}
		}
		break;

	case EV_MOVE_RIGHT: // move picture
		all_data->max_real = all_data->max_real + all_data->step_real * 45;
		all_data->min_real = all_data->min_real + all_data->step_real * 45;
		all_data->current_real = all_data->min_real;
		all_data->current_imag = all_data->max_imag;
		cpu_compute(all_buffers, all_data);
		if (all_data->prediction == 10) {
			for (int i = 1; i < all_data->prediction_10_steps; i++){
				cpu_compute(all_buffers, all_data);
			}
		}
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
		all_data->max_imag = all_data->max_imag * 2;
		all_data->min_imag = all_data->min_imag * 2;
		all_data->step_imag = -(fabs(all_data->max_imag) + fabs(all_data->min_imag)) / all_data->height;
		all_data->max_real = all_data->max_real * 2;
		all_data->min_real = all_data->min_real * 2;
		all_data->step_real = (fabs(all_data->max_real) + fabs(all_data->min_real)) / all_data->width;
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
		all_data->max_imag = all_data->max_imag * 0.5;
		all_data->min_imag = all_data->min_imag * 0.5;
		all_data->step_imag = -(fabs(all_data->max_imag) + fabs(all_data->min_imag)) / all_data->height;
		all_data->max_real = all_data->max_real * 0.5;
		all_data->min_real = all_data->min_real * 0.5;
		all_data->step_real = (fabs(all_data->max_real) + fabs(all_data->min_real)) / all_data->width;
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

/* thread handeling input */
void *music_thread(void *d)
{
	system("play audioAnimation.wav");
	return NULL;
}
