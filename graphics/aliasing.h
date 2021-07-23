/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  STEPAN MAROUSEK                   *
 * date:    2021/07/24                        *
 **********************************************/

#ifndef __ALIASING_H__
#define __ALIASING_H__

#include <stdint.h>
#include "../data/global_data.h"


void aliasing_horizontal(global_data *all_data, uint8_t *buffer1, uint8_t *buffer2, int strenght);
void aliasing_vertical(global_data *all_data, uint8_t *buffer1, uint8_t *buffer2, int strenght);
void aliasing(global_data *all_data, uint8_t *buffer1, uint8_t *buffer2, int strenght);

#endif
