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
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include "execute.h"
#include "../data/messages.h"

void execute_parameters(int argc, char *argv[], global_data *all_data, global_buffer *all_buffers)
{
  int user_width = 0;
	int user_height = 0;

	int opt;

	while ((opt = getopt(argc, argv, ":if:lcrpalsx")) != -1) {
		switch (opt) {
		case 'r':
			print_enter_x_size(all_data->language);
			scanf("%d", &user_width);
			break;

		case 'i':
			print_enter_y_size(all_data->language);
			scanf("%d", &user_height);
			break;

		case 's':
			print_saving_enabled(all_data->language);
			all_data->save_pictures = true;
			break;

		case 'a':
			all_data->audio = 1;
			print_audio_animation(all_data->language);
			break;

		case 'p':
			print_prediction_precision(all_data->language);
			scanf("%d", &all_data->prediction);
			if (!(all_data->prediction == 0 || all_data->prediction == 1 || all_data->prediction == 2 || all_data->prediction == 3 || all_data->prediction == 4 || all_data->prediction == 5 || all_data->prediction == 10 || all_data->prediction == 11 || all_data->prediction == 12 || all_data->prediction == 13)) {
				print_invalid_value(all_data->language);
				all_data->prediction = 1;
			}
			if (all_data->prediction == 10 || all_data->prediction == 11 || all_data->prediction == 12 || all_data->prediction == 13) {
				print_distance_pixels(all_data->language);
				scanf("%d", &all_data->prediction_10_steps);
				if (all_data->prediction_10_steps < 1) {
					print_invalid_value(all_data->language);
					all_data->prediction_10_steps = 1;
				}
			}
			break;

    case 'l':
      print_aliasing_setup(all_data->language);
      scanf("%d", &all_data->aliasing);

		case ':':
			print_option_needs_value(all_data->language);
			break;

		case '?':
			print_unknown_argument(all_data->language, optopt);
			break;
		}
	}

	if (user_width != 0) {
		all_data->width = user_width;
	}

	if (user_height != 0) {
		all_data->height = user_height;
	}

	all_data->step_real = (fabs(all_data->max_real) + fabs(all_data->min_real)) / all_data->width;
	all_data->step_imag = -(all_data->max_imag - all_data->min_imag) / all_data->height;


	all_buffers->iterations_buffer = malloc(sizeof(int) * all_data->width * all_data->height);
	all_buffers->picture_buffer = malloc(3 * sizeof(int) * all_data->width * (all_data->height+50));
}
