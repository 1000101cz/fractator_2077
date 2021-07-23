/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  STEPAN MAROUSEK                   *
 * date:    2021/07/24                        *
 **********************************************/

#include <assert.h>
#include <time.h>

#include <SDL.h>

#include <SDL_image.h>

#include "sdl_window.h"
#include "../data/global_data.h"

static SDL_Window *win;

SDL_Surface *surface;

static unsigned char icon_32x32_bits[] = {
	 2, 12, 40, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0,
	    10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0,
	    10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0,
	    10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0,
	    10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0,
	    11, 43, 0, 11, 43, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0,
	    10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0,
	    10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0,
	    11, 43, 0, 12, 48, 0, 14, 57, 0, 12, 50, 0, 13, 52, 0, 15, 62, 0, 15, 62, 0, 13, 55, 0, 10, 39, 0,
	    10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0,
	    10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0,
	    10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 11, 43, 0, 14, 60, 0, 16, 66, 0, 16, 67, 0, 16, 67, 0,
	    16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 15, 62, 0, 15, 61, 0, 14, 57, 0, 12, 48, 0, 11, 43, 0,
	    10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0,
	    10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 1, 11, 39, 0,
	    13, 52, 0, 16, 66, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0,
	    16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 65, 0, 15, 65, 0, 13, 55, 0, 12, 48, 0, 10, 39, 0, 10, 39, 0,
	    10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0,
	    10, 39, 0, 10, 39, 0, 10, 40, 0, 14, 55, 0, 15, 61, 0, 16, 65, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0,
	    16, 67, 0, 16, 67, 0, 16, 68, 0, 17, 70, 0, 17, 69, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0,
	    16, 67, 0, 16, 67, 0, 16, 66, 0, 14, 58, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0,
	    10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 42, 0, 14, 59, 0, 16, 66, 0,
	    16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 17, 72, 0, 20, 83, 0, 22, 88, 0, 22, 91, 2, 32, 104,
	    12, 66, 127, 1, 27, 96, 0, 20, 81, 0, 18, 73, 0, 16, 69, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16,
	    67, 0, 15, 63, 0, 11, 46, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10,
	    39, 0, 10, 39, 0, 11, 44, 0, 15, 62, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 68, 0, 18, 73, 0, 21,
	    84, 0, 23, 92, 0, 23, 92, 0, 25, 97, 8, 36, 116, 41, 80, 150, 104, 145, 170, 46, 81, 143, 0, 28,
	    111, 0, 23, 94, 0, 22, 90, 0, 18, 75, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0,
	    13, 54, 0, 10, 41, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 12, 51, 0, 15, 61, 0,
	    16, 67, 0, 16, 68, 0, 17, 70, 0, 18, 74, 0, 22, 86, 0, 23, 94, 0, 24, 96, 0, 25, 99, 21, 49, 115,
	    12, 51, 132, 36, 68, 149, 88, 152, 173, 120, 178, 169, 57, 118, 176, 11, 51, 141, 14, 39, 113, 0,
	    24, 94, 0, 23, 92, 0, 22, 86, 0, 17, 72, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 15, 60, 0,
	    11, 45, 0, 10, 40, 0, 10, 39, 0, 10, 39, 0, 14, 58, 0, 16, 65, 0, 16, 66, 0, 16, 67, 0, 18, 75, 3,
	    27, 93, 1, 43, 123, 0, 29, 114, 0, 29, 112, 0, 31, 118, 0, 35, 129, 85, 115, 150, 116, 135, 152,
	    50, 137, 186, 107, 164, 168, 156, 186, 109, 118, 183, 171, 92, 140, 167, 64, 85, 141, 0, 29, 113,
	    0, 25, 98, 0, 23, 92, 0, 22, 88, 0, 19, 78, 0, 17, 69, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 15, 62,
	    0, 11, 46, 0, 10, 39, 0, 10, 39, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 19, 79, 0, 25, 98, 26, 59,
	    132, 123, 123, 138, 78, 125, 154, 64, 84, 147, 10, 44, 148, 104, 102, 148, 91, 151, 186, 117, 175,
	    184, 36, 158, 217, 73, 158, 205, 117, 195, 164, 88, 174, 200, 105, 183, 182, 82, 128, 160, 0, 37,
	    135, 0, 30, 116, 0, 27, 107, 0, 26, 103, 0, 24, 96, 0, 21, 83, 0, 17, 71, 0, 16, 67, 0, 16, 67, 0,
	    16, 66, 0, 14, 60, 0, 10, 39, 0, 10, 39, 0, 16, 67, 0, 16, 67, 0, 17, 72, 0, 25, 98, 8, 55, 130,
	    52, 116, 172, 110, 169, 172, 87, 174, 193, 71, 145, 185, 32, 87, 183, 71, 160, 194, 84, 185, 192,
	    126, 180, 174, 74, 174, 204, 57, 159, 214, 42, 153, 219, 10, 142, 225, 54, 168, 205, 63, 135, 178,
	    0, 44, 152, 0, 37, 136, 0, 33, 125, 0, 32, 121, 5, 34, 123, 17, 42, 117, 5, 29, 95, 0, 19, 76, 0,
	    16, 68, 0, 16, 67, 0, 16, 67, 0, 12, 51, 0, 10, 39, 0, 16, 67, 0, 16, 67, 1, 19, 71, 35, 82, 136,
	    58, 126, 170, 103, 178, 177, 99, 176, 186, 20, 141, 223, 18, 112, 216, 81, 149, 191, 145, 189,
	    139, 150, 205, 108, 129, 221, 122, 136, 198, 130, 104, 167, 191, 4, 137, 225, 0, 118, 224, 27,
	    114, 213, 15, 82, 192, 0, 55, 171, 13, 57, 162, 25, 111, 175, 42, 86, 168, 65, 114, 172, 84, 137,
	    171, 33, 65, 129, 0, 25, 98, 0, 20, 82, 0, 16, 68, 0, 16, 67, 0, 15, 65, 0, 12, 48, 0, 16, 67, 0,
	    16, 67, 14, 34, 77, 82, 140, 163, 117, 164, 177, 162, 159, 94, 118, 215, 146, 80, 156, 203, 60,
	    155, 209, 123, 197, 157, 173, 157, 85, 167, 85, 29, 197, 154, 68, 121, 214, 136, 106, 191, 170,
	    72, 152, 207, 5, 113, 221, 2, 98, 215, 21, 94, 204, 41, 118, 196, 78, 160, 186, 89, 172, 195, 90,
	    172, 193, 106, 151, 181, 125, 162, 154, 54, 102, 158, 1, 34, 120, 0, 26, 100, 0, 20, 81, 0, 16,
	    68, 0, 16, 66, 0, 15, 61, 0, 16, 67, 0, 16, 67, 6, 24, 70, 70, 113, 136, 120, 170, 174, 153, 160,
	    135, 101, 179, 175, 47, 137, 216, 82, 173, 198, 133, 205, 136, 182, 131, 67, 140, 58, 18, 190,
	    139, 60, 140, 197, 122, 134, 178, 136, 94, 151, 192, 9, 121, 223, 27, 120, 218, 73, 157, 197, 103,
	    189, 173, 118, 199, 165, 127, 201, 157, 94, 182, 187, 85, 168, 201, 85, 169, 197, 57, 120, 186, 4,
	    47, 149, 11, 59, 136, 22, 56, 117, 0, 20, 80, 0, 16, 66, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 17,
	    68, 37, 68, 114, 80, 131, 164, 72, 121, 171, 54, 104, 192, 46, 135, 216, 67, 164, 209, 128, 197,
	    153, 170, 186, 94, 173, 174, 82, 162, 188, 117, 102, 206, 161, 85, 169, 194, 30, 135, 221, 0, 133,
	    225, 74, 171, 199, 104, 204, 162, 156, 193, 122, 183, 153, 76, 202, 160, 70, 135, 203, 131, 88,
	    182, 190, 32, 139, 219, 44, 117, 203, 89, 143, 174, 84, 155, 172, 80, 120, 158, 10, 35, 96, 0, 16,
	    67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 7, 33, 99, 15, 46, 116, 3, 41, 135, 32, 79, 178,
	    98, 176, 190, 106, 168, 190, 61, 171, 211, 120, 188, 176, 86, 198, 186, 91, 194, 178, 63, 150,
	    204, 43, 118, 215, 0, 114, 222, 80, 161, 202, 144, 147, 105, 143, 201, 126, 183, 152, 79, 126, 66,
	    21, 160, 55, 22, 156, 200, 117, 91, 194, 177, 51, 146, 217, 89, 189, 181, 167, 148, 102, 148, 177,
	    141, 92, 157, 193, 84, 111, 132, 0, 16, 67, 0, 16, 67, 0, 15, 64, 0, 16, 67, 0, 16, 67, 0, 17, 70,
	    0, 24, 94, 0, 28, 109, 6, 53, 139, 112, 151, 163, 138, 159, 148, 79, 161, 193, 103, 166, 185, 102,
	    161, 173, 54, 100, 191, 17, 80, 198, 2, 91, 210, 1, 107, 220, 29, 138, 221, 90, 180, 194, 98, 217,
	    156, 177, 189, 100, 190, 116, 45, 194, 143, 60, 133, 197, 145, 93, 156, 195, 74, 149, 210, 93,
	    207, 164, 159, 157, 83, 144, 162, 151, 78, 156, 197, 57, 98, 129, 0, 16, 67, 0, 16, 67, 0, 11, 46,
	    0, 15, 63, 0, 16, 67, 0, 16, 68, 0, 19, 78, 0, 22, 90, 0, 27, 108, 68, 104, 155, 64, 126, 166, 22,
	    69, 162, 45, 110, 165, 7, 54, 158, 0, 48, 162, 28, 80, 182, 47, 124, 205, 18, 125, 221, 0, 133,
	    225, 52, 157, 217, 131, 189, 153, 130, 208, 138, 138, 207, 132, 135, 192, 144, 77, 171, 181, 28,
	    111, 210, 43, 144, 216, 85, 173, 194, 113, 174, 173, 70, 131, 171, 48, 88, 149, 15, 37, 93, 0, 16,
	    67, 0, 16, 67, 0, 10, 39, 0, 11, 43, 0, 15, 62, 0, 16, 67, 0, 16, 68, 0, 17, 71, 0, 20, 83, 12,
	    38, 104, 2, 37, 118, 0, 31, 117, 0, 32, 121, 0, 34, 129, 0, 39, 141, 57, 102, 166, 87, 168, 193,
	    43, 157, 219, 81, 163, 205, 46, 157, 218, 64, 161, 209, 88, 176, 194, 86, 180, 194, 100, 166, 181,
	    37, 90, 176, 52, 110, 177, 88, 162, 186, 122, 178, 166, 64, 123, 175, 13, 54, 133, 0, 26, 104, 0,
	    17, 72, 0, 16, 67, 0, 16, 67, 0, 10, 39, 0, 10, 39, 0, 11, 47, 0, 16, 65, 0, 16, 67, 0, 16, 67, 0,
	    16, 68, 0, 18, 75, 0, 22, 88, 0, 25, 98, 0, 25, 100, 0, 28, 107, 0, 33, 124, 59, 91, 149, 122,
	    166, 162, 119, 173, 183, 116, 209, 148, 106, 188, 177, 42, 143, 214, 105, 175, 182, 111, 161, 171,
	    71, 102, 153, 0, 43, 144, 26, 59, 138, 57, 92, 143, 88, 117, 146, 25, 68, 134, 0, 25, 99, 0, 20,
	    83, 0, 16, 66, 0, 16, 66, 0, 15, 61, 0, 10, 39, 0, 10, 39, 0, 10, 40, 0, 13, 54, 0, 16, 66, 0, 16,
	    67, 0, 16, 67, 0, 16, 67, 0, 17, 71, 0, 21, 82, 0, 23, 91, 0, 23, 93, 0, 26, 100, 31, 59, 127, 70,
	    101, 160, 93, 166, 180, 164, 160, 89, 143, 187, 140, 49, 111, 184, 63, 112, 165, 62, 106, 157, 6,
	    40, 129, 0, 30, 115, 0, 28, 110, 0, 28, 107, 3, 41, 106, 1, 28, 88, 0, 18, 74, 0, 16, 69, 0, 15,
	    65, 0, 13, 53, 0, 11, 43, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 40, 0, 13, 52, 0, 16, 65, 0, 16,
	    67, 0, 16, 67, 0, 16, 67, 0, 16, 68, 0, 20, 79, 0, 23, 90, 0, 23, 92, 0, 25, 98, 0, 32, 122, 3,
	    72, 163, 89, 174, 182, 89, 171, 182, 44, 77, 150, 0, 33, 125, 0, 28, 108, 0, 23, 94, 0, 23, 92, 0,
	    22, 90, 0, 21, 85, 0, 17, 71, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 15, 61, 0, 10, 39, 0, 10, 39, 0,
	    10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 40, 0, 12, 48, 0, 15, 61, 0, 16, 65, 0, 16, 67, 0,
	    16, 67, 0, 16, 67, 0, 17, 71, 0, 20, 80, 0, 21, 86, 0, 25, 101, 27, 55, 129, 84, 145, 163, 44, 82,
	    155, 5, 38, 117, 0, 25, 100, 0, 23, 92, 0, 22, 89, 0, 21, 85, 0, 18, 75, 0, 16, 67, 0, 16, 67, 0,
	    16, 67, 0, 16, 66, 0, 15, 61, 0, 11, 44, 0, 10, 39, 4, 14, 40, 0, 10, 39, 11, 19, 43, 0, 10, 39,
	    0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 11, 42, 0, 12, 51, 0, 15, 60, 0, 16, 67, 0, 16, 67, 0, 16, 67,
	    0, 16, 67, 0, 16, 68, 0, 17, 71, 0, 21, 86, 7, 39, 91, 2, 27, 89, 0, 19, 79, 0, 18, 75, 0, 18, 73,
	    0, 17, 69, 0, 16, 68, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 14, 58, 0, 11, 45, 0, 10, 39,
	    0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39,
	    0, 10, 39, 0, 13, 54, 0, 16, 66, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67,
	    0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67,
	    0, 15, 64, 0, 12, 48, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39,
	    0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 13, 53, 0, 14, 59,
	    0, 16, 65, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67,
	    0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 16, 67, 0, 15, 61, 0, 12, 48, 0, 10, 39, 0, 10, 39, 0, 10, 39,
	    0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39,
	    0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 14, 58, 0, 14, 57, 0, 14, 60,
	    0, 15, 64, 0, 16, 67, 0, 16, 66, 0, 16, 66, 0, 16, 66, 0, 15, 64, 0, 15, 61, 0, 13, 55, 0, 13, 52,
	    0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39,
	    0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39,
	    0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 11, 43, 0, 14, 56, 0, 12, 50, 0, 12, 48,
	    0, 12, 48, 0, 12, 48, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39,
	    0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39,
	    0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39,
	    0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39,
	    0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39,
	    0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39,
	    0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39,
	    0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39,
	    0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39,
	    0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39,
	    0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39,
	    0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39, 0, 10, 39,
	    0, 10, 39
};

int window_init(int w, int h)
{
	int r;
	r = SDL_Init(SDL_INIT_VIDEO);
	win =
	    SDL_CreateWindow("Fractal", SDL_WINDOWPOS_UNDEFINED,
			     SDL_WINDOWPOS_UNDEFINED, w, h+50, SDL_WINDOW_SHOWN);  // h + 50
	SDL_SetWindowTitle(win, "FRACTATOR 2077");
	surface = SDL_CreateRGBSurfaceFrom(icon_32x32_bits, 32, 32, 24, 96, 0xff, 0xff00, 0xff0000, 0);
	SDL_SetWindowIcon(win, surface);
	SDL_FreeSurface(surface);
	return r;
}

void window_close()
{
	SDL_DestroyWindow(win);
	SDL_Quit();
}

void window_redraw(uint8_t *img)
{
	SDL_Surface *scr = SDL_GetWindowSurface(win);
	for (int y = 0; y < (scr->h); ++y) {  // h + 50
		for (int x = 0; x < scr->w; ++x) {
			int idx = (y * scr->w + x) * scr->format->BytesPerPixel;
			uint8_t *px = (uint8_t *) scr->pixels + idx;
			*(px + scr->format->Rshift / 8) = *(img++);
			*(px + scr->format->Gshift / 8) = *(img++);
			*(px + scr->format->Bshift / 8) = *(img++);
		}
	}
	SDL_UpdateWindowSurface(win);
}

void window_poll_events(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {}
}

void window_resize(global_data * all_data)
{
	SDL_SetWindowSize(win, all_data->width, all_data->height+50);
}
