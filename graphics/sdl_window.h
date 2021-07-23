/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  STEPAN MAROUSEK                   *
 * date:    2021/07/24                        *
 **********************************************/

#ifndef __XWIN_SDL_H__
#define __XWIN_SDL_H__

#include "../data/global_data.h"

int window_init(int w, int h);
void window_close();
void window_redraw(uint8_t *img);
void window_poll_events(void);
void window_resize(global_data * all_data);

#endif
