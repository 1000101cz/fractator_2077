#include "aliasing.h"

void aliasing_horizontal(global_data *all_data, uint8_t *buffer1, uint8_t *buffer2, int strenght)
{
  int position;
  uint8_t red, green, blue;
  for (int i = 0; i < all_data->height; i++) {
    for (int j = 0; j < (all_data->width - 2); j++) {
      position = 3 * i * (all_data->width - 2) + 3 * j;
      red   = (5 * buffer1[position    ] + (5.0/strenght) * buffer1[position + 3] + (5.0/strenght)*buffer1[position + 6])/(5 + 2 * (5.0/strenght));
      green = (5 * buffer1[position + 1] + (5.0/strenght) * buffer1[position + 4] + (5.0/strenght)*buffer1[position + 7])/(5 + 2 * (5.0/strenght));
      blue  = (5 * buffer1[position + 2] + (5.0/strenght) * buffer1[position + 5] + (5.0/strenght)*buffer1[position + 8])/(5 + 2 * (5.0/strenght));
      buffer2[position    ] = red;
      buffer2[position + 1] = green;
      buffer2[position + 2] = blue;
    }
    buffer2[position + 3]=buffer1[position + 3];
    buffer2[position + 4]=buffer1[position + 4];
    buffer2[position + 5]=buffer1[position + 5];
  }
}

void aliasing_vertical(global_data *all_data, uint8_t *buffer1, uint8_t *buffer2, int strenght)
{
  int position;
  uint8_t red, green, blue;
  for (int i = 0; i < (all_data->height - 2); i++) {
    for (int j = 0; j < all_data->width; j++) {
      position = 3 * i * all_data->width + 3 * j;
      red   = (5 * buffer1[position    ] + (5.0/strenght) * buffer1[position + 0 + all_data->width * 3] + (5.0/strenght) * buffer1[position + 0 + all_data->width * 6])/(5 + 2* (5.0/strenght));
      green = (5 * buffer1[position + 1] + (5.0/strenght) * buffer1[position + 1 + all_data->width * 3] + (5.0/strenght) * buffer1[position + 1 + all_data->width * 6])/(5 + 2 * (5.0/strenght));
      blue  = (5 * buffer1[position + 2] + (5.0/strenght) * buffer1[position + 2 + all_data->width * 3] + (5.0/strenght) * buffer1[position + 2 + all_data->width * 6])/(5 + 2 * (5.0/strenght));
      buffer2[position    ] = red;
      buffer2[position + 1] = green;
      buffer2[position + 2] = blue;
    }
    buffer2[position + 3]=buffer1[position + 3];
    buffer2[position + 4]=buffer1[position + 4];
    buffer2[position + 5]=buffer1[position + 5];
  }
}

void aliasing(global_data *all_data, uint8_t *buffer1, uint8_t *buffer2, int strenght)
{
  aliasing_horizontal(all_data, buffer1, buffer2, strenght);
  aliasing_vertical(all_data, buffer1, buffer2, strenght);
}
