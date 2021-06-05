/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  STEPAN MAROUSEK                   *
 * date:    05/16/2021                        *
 **********************************************/

#include <pthread.h>
#include <stdio.h>

#include "event_queue.h"
#include "event_keyboard.h"
#include "keyboard_input.h"
#include "terminal_colors.h"

pthread_mutex_t mtx;
pthread_cond_t cond;

/* handle keyboard input */
void keyboard_input(data_t * data, event * ev)
{

	pthread_mutex_init(&mtx, NULL);	// initialize mutex with default attributes
	pthread_cond_init(&cond, NULL);	// initialize mutex with default attributes
	int c;

	while (!data->quit && (c = getchar())) {
		ev->type = EV_TYPE_NUM;
		switch (c) {
		case 'g':	// get version
			ev->type = EV_GET_VERSION;
			break;

		case 'q':	// quit
			ev->type = EV_QUIT;
			data->quit = true;
			break;

		case 'a':	// abort
			ev->type = EV_ABORT;
			break;

		case 'r':	// reset CID
			ev->type = EV_RESET_CHUNK;
			break;

		case '1':	// compute
			ev->type = EV_COMPUTE;
			ev->data.param = 10;
			break;

		case 's':	// set compute parameters
			ev->type = EV_SET_COMPUTE;
			break;

		case 'l':	// erase buffer
			ev->type = EV_CLEAR_BUFFER;
			break;

		case 'p':	// display buffer
			ev->type = EV_REFRESH;
			break;

		case 'c':	// compute on pc
			ev->type = EV_COMPUTE_CPU;
			break;

		case '+':
			ev->type = EV_RESIZE_PLUS;
			break;

		case '-':
			ev->type = EV_RESIZE_MINUS;
			break;

		case '8':
			ev->type = EV_MOVE_UP;
			break;

		case '2':
			ev->type = EV_MOVE_DOWN;
			break;
		case '4':
			ev->type = EV_MOVE_LEFT;
			break;

		case '6':
			ev->type = EV_MOVE_RIGHT;
			break;

		case '7':
			ev->type = EV_DECREASE_ITER;
			break;

		case '9':
			ev->type = EV_INCREASE_ITER;
			break;

		case 'h':
			ev->type = EV_CHANGE_1;
			break;

		case 'j':
			ev->type = EV_CHANGE_2;
			break;

		case 'k':
			ev->type = EV_CHANGE_3;
			break;

		case 'u':
			ev->type = EV_CHANGE_4;
			break;

		case '0':
			ev->type = EV_ANIMATION;
			break;

		default:	// discard all other keys
			break;
		}

		if (ev->type != EV_TYPE_NUM) {	// new event
			queue_push(*ev);
		}
		pthread_mutex_lock(&mtx);
		data->quit = data->quit || data->quit;	// check for quit
		pthread_mutex_unlock(&mtx);
	}
	ev->type = EV_QUIT;
	queue_push(*ev);
	def_color();
}
