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

#include "../system/terminal_colors.h"
#include "../data/global_data.h"
#include "../graphics/graphics.h"
#include "../graphics/sdl_window.h"
#include "animations.h"
#include "../events/event_keyboard.h"

#define MUSIC_FPS 30
#define MUSIC_HZ 44100

void *music_thread(void *);

void pure_animation(global_data * all_data, global_buffer * all_buffers)
{
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

void audio_animation(global_data * all_data, global_buffer * all_buffers)
{
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
	audioIn = fopen("data/audioAnimation.wav","r");
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

		all_data->c_real = -1.0 + frame_volume*(MUSIC_FPS/30.0)*frame_volume*(MUSIC_FPS/30.0)/120000000000.0;
		all_data->c_imag = 0.4 - frame_volume*(MUSIC_FPS/30.0)/400000.0;

		if (amplitudes != 0) {
			all_data->number_of_iterations = (40 + 2000000/(int)(amplitudes*(MUSIC_FPS/30.0)*amplitudes*(MUSIC_FPS/30.0)*amplitudes*(MUSIC_FPS/30.0)))%200;
			if (all_data->number_of_iterations < 40) {
				all_data->number_of_iterations = 40;
			}
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

/* thread handeling input */
void *music_thread(void *d)
{
	system("play data/audioAnimation.wav");
	return NULL;
}
