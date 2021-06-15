/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  S. MAROUSEK                       *
 * date:    2021/6/15                         *
 **********************************************/

/* EXECUTE OPTIONS
*    -r             set width resolution
*    -i             set height resolution
*    -s             enable saving pc generated pictures to /tmp
*    -p             set pixel prediction
*    -a             enable audio animation
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
#include "graphics/xwin_sdl.h"
#include "data/messages.h"
#include "system/execute.h"

#define MESSAGE_SIZE (sizeof(message))
#define SERIAL_READ_TIMOUT_MS 500	//timeout for reading from serial port 500

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

	welcome_message(all_data.language); // display welcome message

	global_buffer all_buffers;

	data_t data = {.quit = false,.fd = -1, };

	execute_parameters(argc, argv, &all_data, &all_buffers); // handle execution options

	enum { INPUT, SDLTHRD, PYTHONTHREAD, NUM_THREADS };
	void *(*thr_functions[])(void *) = { input_thread, sdl_thread, python_thread };
	pthread_t threads[NUM_THREADS];

	create_window(&all_data);

	call_termios(0);

	for (int i = 0; i < NUM_THREADS; ++i) {
		pthread_create(&threads[i], NULL, thr_functions[i], &data);
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
	close_window();
	return EXIT_SUCCESS;
}

/* thread handeling input */
void *input_thread(void *d)
{
	data_t *data = (data_t *) d;
	xwin_poll_events();
	event ev = {.source = EV_KEYBOARD };
	keyboard_input(data, &ev);
	return NULL;
}

/* thread handeling SDL window */
void *sdl_thread(void *d)
{
	while(!end_thr) {
		xwin_poll_events();
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
