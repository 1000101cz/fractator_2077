/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  STEPAN MAROUSEK                   *
 * date:    05/16/2021                        *
 **********************************************/

#ifndef __GLOBAL_DATA_H__
#define __GLOBAL_DATA_H__


#include <stdint.h>

/* structure containing all fractal specifications */
typedef struct {
	int width;		// width in pixels
	int height;		// height in pixels
	int number_of_iterations;
	double c_real;		// real aspect of c
	double c_imag;		// imaginary aspect of c
	double min_real;
	double max_real;
	double min_imag;
	double max_imag;

	double step_real;	// real difference between two columns
	double step_imag;	// imag difference between two rows

	int real_chunk_size;
	int imag_chunk_size;

	double current_real;
	double current_imag;

	int current_chunk_id;
	_Bool first_compute;

	_Bool nucleo_connected;
	_Bool save_pictures;

	_Bool thread1c;
	_Bool thread2c;
	_Bool thread3c;
	_Bool thread4c;

	int prediction;
	int prediction_10_steps;

	int animation_frame;

	int menuPosition;

	_Bool audio;
} global_data;

/* structure containing picture buffers */
typedef struct {
	int *iterations_buffer;	// iterations for each pixel
	uint8_t *picture_buffer;	// picture (3 positions for each pixel)
} global_buffer;

#endif
