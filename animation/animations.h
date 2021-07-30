/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  STEPAN MAROUSEK                   *
 * date:    2021/07/24                        *
 **********************************************/

#ifndef __ANIMATIONS_H__
#define __ANIMATIONS_H__

void zoom_out(global_data * all_data, double zoom_value);

void zoom_in(global_data * all_data, double zoom_value);

void pure_animation(global_data * all_data, global_buffer * all_buffers);

void audio_animation(global_data * all_data, global_buffer * all_buffers);

#endif
