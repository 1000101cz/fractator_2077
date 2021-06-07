/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  STEPAN MAROUSEK                   *
 * date:    05/16/2021                        *
 **********************************************/

#ifndef __EVENT_KEYBOARD_H__
#define __EVENT_KEYBOARD_H__

#include "event_queue.h"
#include "global_data.h"

// shared data structure
typedef struct {
	_Bool quit;  // exit threads and loops
	int fd;			// serial port file descriptor
} data_t;

void animation(global_data * all_data, global_buffer * all_buffers);

void event_keyboard_ev(event * ev, data_t * data,
		       _Bool *computing, uint16_t * chunk_id,
		       global_data * all_data, global_buffer * all_buffers);

#endif
