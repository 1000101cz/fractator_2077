/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  STEPAN MAROUSEK                   *
 * date:    2021/07/24                        *
 **********************************************/

/* EXECUTE OPTIONS
*    -r             set width resolution
*    -i             set height resolution
*    -s             enable saving pc generated pictures to /tmp
*    -p             set pixel prediction
*    -a             enable audio animation
*    -l             set aliasing
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
#include <unistd.h>
#include <math.h>
#include <pthread.h>

#include "events/event_queue.h"
#include "events/event_keyboard.h"
#include "system/terminal_colors.h"
#include "events/keyboard_input.h"
#include "data/global_data.h"
#include "graphics/graphics.h"
#include "graphics/sdl_window.h"
#include "data/messages.h"
#include "system/execute.h"

_Bool end_thr = 0;

void *input_thread(void *);
void *sdl_thread(void *);
void *python_thread(void *);

/* main function */
int main(int argc, char *argv[])
{
	/* local variables storing fractal specifications */
	global_data all_data;
	init_all_data(&all_data);

	global_buffer all_buffers;

	welcome_message(all_data.language); // display welcome message

	execute_parameters(argc, argv, &all_data, &all_buffers); // handle execution options

	enum { INPUT, SDLTHRD, PYTHONTHREAD, NUM_THREADS };  //create threads
	void *(*thr_functions[])(void *) = { input_thread, sdl_thread, python_thread};
	pthread_t threads[NUM_THREADS];

	window_intro();

	window_init(all_data.width, all_data.height);      // create SDL window

	call_termios(0);              // enter raw mode

	data_t data = {.quit = false, .fd = -1};
	for (int i = 0; i < NUM_THREADS; ++i) {    // open threads
		pthread_create(&threads[i], NULL, thr_functions[i], &data);
	}

	/* local variables for computation */
	struct {
		uint16_t chunk_id;
		bool computing;
	} computation = {0, false};
	cpu_compute(&all_buffers, &all_data);

	/* main loop */
	while (!data.quit) {
		event ev = queue_pop();

		if (ev.source == EV_KEYBOARD) {
			event_keyboard_ev(&ev, &data,	// handle keyboard events
					  &computation.computing,
					  &computation.chunk_id, &all_data, &all_buffers);

		}
	}

	/* terminate all threads and exit code */
	end_thr = 1;
	for (int i = 0; i < NUM_THREADS; ++i) {
		pthread_cancel(threads[i]);
		pthread_join(threads[i], NULL);
	}
	call_termios(1);	// restore terminal settings
	window_close();
	return EXIT_SUCCESS;
}

/* thread handeling input */
void *input_thread(void *d)
{
	data_t *data = (data_t *) d;
	event ev = {.source = EV_KEYBOARD };
	keyboard_input(data, &ev);
	return NULL;
}

/* thread handeling SDL window */
void *sdl_thread(void *d)
{
	while(!end_thr) {
		window_poll_events();
		sleep(0.001);
	}
	return NULL;
}

/* thread handeling file conversion */
void *python_thread(void *d)
{
	system("python3 python/picture_compress.py");
	return NULL;
}

/* end of fractator sem */
