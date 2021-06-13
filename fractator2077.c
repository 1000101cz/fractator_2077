/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  S. MAROUSEK                       *
 * date:    2021/6/6                          *
 **********************************************/

/* EXECUTE OPTIONS
*    -r             set width resolution
*    -i             set height resolution
*    -s             enable saving pc generated pictures to /tmp
*    -p             set pixel prediction
*
* examples:  ./Fractator_2077                      (default settings)
*            ./Fractator_2077 -i -r -s -p          (change picture width and height and save pc generated pictures)
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <termios.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

#include "events/event_queue.h"
#include "events/event_keyboard.h"
#include "system/terminal_colors.h"
#include "events/keyboard_input.h"
#include "data/global_data.h"
#include "graphics/graphics.h"
#include "graphics/xwin_sdl.h"

#define MESSAGE_SIZE (sizeof(message))
#define SERIAL_READ_TIMOUT_MS 500	//timeout for reading from serial port 500

_Bool end_thr = 0;

void call_termios(int reset);

void *input_thread(void *);

void *sdl_thread(void *);

void init_all_data(global_data * all_data)
{
	all_data->width = 660;
	all_data->height = 480;
	all_data->number_of_iterations = 60;
	all_data->max_real = 1.6;
	all_data->min_real = -1.6;
	all_data->max_imag = 1.1;
	all_data->min_imag = -1.1;
	all_data->c_real = -0.66;
	all_data->c_imag = 0.385;
	all_data->current_real = all_data->min_real;
	all_data->current_imag = all_data->max_imag;
	all_data->step_real = (fabs(all_data->max_real) + fabs(all_data->min_real)) / all_data->width;
	all_data->step_imag = -(all_data->max_imag - all_data->min_imag) / all_data->height;
	all_data->first_compute = true;
	all_data->save_pictures = false;
	all_data->thread1c = 0;
	all_data->thread2c = 0;
	all_data->thread3c = 0;
	all_data->thread4c = 0;
	all_data->prediction = 1;
	all_data->prediction_10_steps = 9;
	all_data->animation_frame = 1;
	all_data->menuPosition = 0;
}

void welcome_message()
{
	green_col();
	fprintf(stderr,
		"FRACTATOR 2077 - seminar work\n  created by Marousek Stepan 'maroust1'\n  2021 06 06\n  Czech Technical University in Prague\n");
	def_color();
	fprintf(stderr, "*************************************************\n");
	fprintf(stderr, "Controls:                                       *\n");
	fprintf(stderr, "  basic:                                        *\n");
	fprintf(stderr, "   c    compute fractal on computer             *\n");
	fprintf(stderr, "   0    play animation                          *\n");
	fprintf(stderr, "- - - - - - - - - - - - - - - - - - - - - - - - *\n");
	fprintf(stderr, "  movement:                                     *\n");
	fprintf(stderr, "   8    move over the picture (UP)              *\n");
	fprintf(stderr, "   2    move over the picture (DOWN)            *\n");
	fprintf(stderr, "   4    move over the picture (LEFT)            *\n");
	fprintf(stderr, "   6    move over the picture (RIGHT)           *\n");
	fprintf(stderr, "   u    zoom in                                 *\n");
	fprintf(stderr, "   j    zoom out                                *\n");
	fprintf(stderr, "   h    decrease c number values                *\n");
	fprintf(stderr, "   k    increase c number values                *\n");
	fprintf(stderr, "   7    decrease number of iterations           *\n");
	fprintf(stderr, "   9    increase number of iterations           *\n");
	fprintf(stderr, "- - - - - - - - - - - - - - - - - - - - - - - - *\n");
	fprintf(stderr, "  window controls:                              *\n");
	fprintf(stderr, "   l    erase actual picture buffer             *\n");
	fprintf(stderr, "   p    display current buffer (refresh)        *\n");
	fprintf(stderr, "   +    resize window (+)                       *\n");
	fprintf(stderr, "   -    resize window (-)                       *\n");
	fprintf(stderr, "- - - - - - - - - - - - - - - - - - - - - - - - *\n");
	fprintf(stderr, "   q    exit application                        *\n");
	fprintf(stderr, "*************************************************\n");
	fprintf(stderr, "\n");
}

/* main function */
int main(int argc, char *argv[])
{
	welcome_message();
	/* local variables storing fractal specifications */
	global_data all_data;
	init_all_data(&all_data);

	global_buffer all_buffers;

	data_t data = {.quit = false,.fd = -1, };

	int user_width = 0;
	int user_height = 0;

	int opt;

	while ((opt = getopt(argc, argv, ":if:lcrpsx")) != -1) {
		switch (opt) {
		case 'r':
			printf("Enter x axis size: ");
			scanf("%d", &user_width);
			break;

		case 'i':
			printf("Enter y axis size: ");
			scanf("%d", &user_height);
			break;

		case 's':
			printf("Saving PC generated pictures enabled '/tmp'\n");
			all_data.save_pictures = true;
			break;

		case 'p':
			printf("Choose prediction precision [(0-5) / (10-13)]: ");
			scanf("%d", &all_data.prediction);
			if (!(all_data.prediction == 0 || all_data.prediction == 1 || all_data.prediction == 2 || all_data.prediction == 3 || all_data.prediction == 4 || all_data.prediction == 5 || all_data.prediction == 10 || all_data.prediction == 11 || all_data.prediction == 12 || all_data.prediction == 13)) {
				red_col();
				fprintf(stderr,"ERROR: Invalid value!\n");
				all_data.prediction = 1;
			}
			if (all_data.prediction == 10 || all_data.prediction == 11 || all_data.prediction == 12 || all_data.prediction == 13) {
				printf("Choose distance between pixels: ");
				scanf("%d", &all_data.prediction_10_steps);
				if (all_data.prediction_10_steps < 1) {
					red_col();
					fprintf(stderr,"ERROR: Invalid value!\n");
					all_data.prediction_10_steps = 1;
				}
			}
			break;

		case ':':
			printf("option needs a value\n");
			break;

		case '?':
			printf("unknown argument: %c\n", optopt);
			break;
		}
	}

	if (user_width != 0) {
		all_data.width = user_width;
	}

	if (user_height != 0) {
		all_data.height = user_height;
	}

	all_data.step_real = (fabs(all_data.max_real) + fabs(all_data.min_real)) / all_data.width;
	all_data.step_imag = -(all_data.max_imag - all_data.min_imag) / all_data.height;


	all_buffers.iterations_buffer = malloc(sizeof(int) * all_data.width * all_data.height);
	all_buffers.picture_buffer = malloc(3 * sizeof(int) * all_data.width * all_data.height);

	enum { INPUT, SDLTHRD, NUM_THREADS };
	const char *threads_names[] = { "Input", "SDL" };
	void *(*thr_functions[])(void *) = { input_thread, sdl_thread };
	pthread_t threads[NUM_THREADS];

	create_window(&all_data);

	call_termios(0);

	for (int i = 0; i < NUM_THREADS; ++i) {
		int r = pthread_create(&threads[i], NULL, thr_functions[i], &data);
		def_color();
		printf("INFO:  Create thread '%s' %s\n", threads_names[i], (r == 0 ? "OK" : "FAIL"));
	}

	/* local variables for computation */
	struct {
		uint16_t chunk_id;
		uint16_t nbr_tasks;
		uint16_t task_id;
		bool computing;
	} computation = { 0, 0, 0, false };

	/* main loop */
	while (!data.quit) {
		event ev = queue_pop();
		xwin_poll_events();

		if (ev.source == EV_KEYBOARD) {
			event_keyboard_ev(&ev, &data,	// handle keyboard events
					  &computation.computing,
					  &computation.chunk_id, &all_data, &all_buffers);

		}
	}

	/* terminate all threads and exit code */
	end_thr = 1;
	for (int i = 0; i < NUM_THREADS; ++i) {
		def_color();
		fprintf(stderr, "INFO:  Call join to the thread %s\n", threads_names[i]);
		pthread_cancel(threads[i]);
		int r = pthread_join(threads[i], NULL);
		def_color();
		fprintf(stderr, "INFO:  Joining the thread %s has been %s\n",
			threads_names[i], (r == 0 ? "OK" : "FAIL"));
	}
	call_termios(1);	// restore terminal settings
	close_window();
	return EXIT_SUCCESS;
}

/* manage terminal settings */
void call_termios(int reset)
{
	static struct termios tio, tioOld;
	tcgetattr(STDIN_FILENO, &tio);

	if (reset) {
		tcsetattr(STDIN_FILENO, TCSANOW, &tioOld);

	} else {
		tioOld = tio;	//backup
		cfmakeraw(&tio);
		tio.c_oflag |= OPOST;
		tcsetattr(STDIN_FILENO, TCSANOW, &tio);
	}
}

/* thread handeling input */
void *input_thread(void *d)
{
	data_t *data = (data_t *) d;
	xwin_poll_events();
	event ev = {.source = EV_KEYBOARD };
	keyboard_input(data, &ev);
	fprintf(stderr, "INFO:  Exit input thead %p\n", (void *)pthread_self());
	return NULL;
}

/* thread handeling input */
void *sdl_thread(void *d)
{
	while(!end_thr) {
		xwin_poll_events();
		sleep(0.001);
	}
	return NULL;
}


/* end of fractator sem */
