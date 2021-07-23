/*
 * File name: event_queue.h
 * Date:      2017/04/15 12:41
 * Author:    Jan Faigl
 */

#ifndef __EVENT_QUEUE_H__
#define __EVENT_QUEUE_H__

typedef enum {
	EV_NUCLEO,
	EV_KEYBOARD,
	EV_NUM
} event_source;

typedef enum {
	EV_COMPUTE,		// request compute on nucleo with particular
	EV_RESET_CHUNK,		//reset the chunk id
	EV_ABORT,
	EV_GET_VERSION,
	EV_THREAD_EXIT,
	EV_QUIT,
	EV_SERIAL,
	EV_SET_COMPUTE,
	EV_COMPUTE_CPU,
	EV_CLEAR_BUFFER,
	EV_REFRESH,
	EV_TYPE_NUM,
	EV_RESIZE_PLUS,
	EV_RESIZE_MINUS,
	EV_MOVE_UP,
	EV_MOVE_DOWN,
	EV_MOVE_RIGHT,
	EV_MOVE_LEFT,
	EV_INCREASE_ITER,
	EV_DECREASE_ITER,
	EV_CHANGE_1,
	EV_CHANGE_2,
	EV_CHANGE_3,
	EV_CHANGE_4,
	EV_ANIMATION,
	EV_SAVE_IMAGE
} event_type;

typedef struct {
	int param;
} event_keyboard;

typedef struct {
	event_source source;
	event_type type;
	union {
		int param;
	} data;
} event;

void queue_init(void);
void queue_cleanup(void);

event queue_pop(void);

void queue_push(event ev);

#endif

/* end of event_queue.h */
