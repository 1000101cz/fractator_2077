#include "prediction.h"
#include "graphics.h"

/* simple prediction */
void compute_function_predict_1(int cycle_start, int cycle_end, int local_width, int local_height, double c_real, double c_imag, int number_of_iterations, double min_real, double max_imag, double step_real, double step_imag)
{
	int iter1;	// this is going to save to iterations_buffer
	int iter3;

	iter1 = iter_function(number_of_iterations, min_real, max_imag, c_real, c_imag);
	float t = getT(iter1,number_of_iterations);
	save_pixel(0, getR(t), getG(t), getB(t));

	for (int i = 1; i < local_width * local_height; i++) {
		if ((i+1)%local_width == 0) {
			float t = getT(iter_function(number_of_iterations, min_real + (i%local_width)*step_real, max_imag + (i/local_width)*step_imag, c_real, c_imag),number_of_iterations);
			save_pixel(3 * i, getR(t), getG(t), getB(t));
		}
		else {
			iter3 = iter_function(number_of_iterations, min_real + ((i+1)%local_width)*step_real, max_imag + ((i+1)/local_width)*step_imag, c_real, c_imag);
			float t = getT(iter3,number_of_iterations);
			save_pixel(3 * (i + 1), getR(t), getG(t), getB(t));

			if (iter3 == iter1) {
				save_pixel(3 * i, getR(t), getG(t), getB(t));
			}
			else {
				float t = getT(iter_function(number_of_iterations, min_real + (i%local_width)*step_real, max_imag + (i/local_width)*step_imag, c_real, c_imag),number_of_iterations);
				save_pixel(3 * i, getR(t), getG(t), getB(t));
			}
			i++;
			iter1 = iter3;
		}
	}
}

/* fast prediction */
void compute_function_predict_2(int cycle_start, int cycle_end, int local_width, int local_height, double c_real, double c_imag, int number_of_iterations, double min_real, double max_imag, double step_real, double step_imag)
{
	int iter1;	// this is going to save to iterations_buffer
	int iter2;
	int iter3;
	int iter4;
	int iter5;

	float real;
	float imag;

	for (int i = 0; i < (local_width * local_height)/25; i++) {
		float t;
		int position;
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		real = min_real + ((i*5)%local_width)*step_real;
		imag = max_imag + ((i*5)/local_width)*(step_imag*5);

		iter1 = iter_function(number_of_iterations, real, imag, c_real, c_imag);
		position = 15 * i + (i/(local_width/5))*(12*local_width);
		t = getT(iter1,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position + 3, red, green, blue);
		save_pixel(position + 3*local_width, red, green, blue);

		iter2 = iter_function(number_of_iterations, real+4*step_real, imag, c_real, c_imag);
		position = 15 * i + (i/(local_width/5))*(12*local_width) + 12;
		t = getT(iter2,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position - 3, red, green, blue);
		save_pixel(position + 3*local_width, red, green, blue);

		iter3 = iter_function(number_of_iterations+4*step_imag, real, imag, c_real, c_imag);
		position = 15 * i + (i/(local_width/5))*(12*local_width) + 12 * local_width;
		t = getT(iter3,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position + 3, red, green, blue);
		save_pixel(position - 3*local_width, red, green, blue);

		iter4 = iter_function(number_of_iterations+4*step_imag, real+4*step_real, imag, c_real, c_imag);
		position = 15 * i + (i/(local_width/5))*(12*local_width) + 12 * local_width + 12;
		t = getT(iter4,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position - 3, red, green, blue);
		save_pixel(position - 3*local_width, red, green, blue);

		iter5 = iter_function(number_of_iterations, real+2*step_real, imag+2*step_imag, c_real, c_imag);
		position = 15 * i + (i/(local_width/5))*(12*local_width) + 6 * local_width + 6;
		t = getT(iter5,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position + 3, red, green, blue);
		save_pixel(position - 3, red, green, blue);
		save_pixel(position + 3*local_width, red, green, blue);
		save_pixel(position - 3*local_width, red, green, blue);


		// + pixels between
		int iter = (iter2 + iter4 + iter5)/3;
		t = getT(iter,number_of_iterations);
		save_pixel(position + 6, getR(t), getG(t), getB(t));

		iter = (iter1 + iter3 + iter5)/3;
		t = getT(iter,number_of_iterations);
		save_pixel(position - 6, getR(t), getG(t), getB(t));

		iter = (iter1 + iter2 + iter5)/3;
		t = getT(iter,number_of_iterations);
		save_pixel(position - 6*local_width, getR(t), getG(t), getB(t));

		iter = (iter3 + iter4 + iter5)/3;
		t = getT(iter,number_of_iterations);
		save_pixel(position + 6*local_width, getR(t), getG(t), getB(t));

		iter = (iter1 + iter5)/2;
		t = getT(iter,number_of_iterations);
		save_pixel(position - 3*local_width - 3, getR(t), getG(t), getB(t));

		iter = (iter2 + iter5)/2;
		t = getT(iter,number_of_iterations);
		save_pixel(position - 3*local_width + 3, getR(t), getG(t), getB(t));

		iter = (iter3 + iter5)/2;
		t = getT(iter,number_of_iterations);
		save_pixel(position + 3*local_width - 3, getR(t), getG(t), getB(t));

		iter = (iter4 + iter5)/2;
		t = getT(iter,number_of_iterations);
		save_pixel(position + 3*local_width + 3, getR(t), getG(t), getB(t));
	}
}

/* very-fast prediction */
void compute_function_predict_3(int cycle_start, int cycle_end, int local_width, int local_height, double c_real, double c_imag, int number_of_iterations, double min_real, double max_imag, double step_real, double step_imag)
{
	int iter1;	// this is going to save to iterations_buffer
	int iter2;
	int iter3;
	int iter4;
	int iter5;
	int iter6;
	int iter7;
	int iter8;

	float real;
	float imag;

	for (int i = 0; i < (local_width * local_height/100); i++) {
		float t;
		int position;
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		real = min_real + ((i*10)%local_width)*step_real;
		imag = max_imag + ((i*10)/local_width)*(step_imag*10);

		iter1 = iter_function(number_of_iterations, real, imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width);
		t = getT(iter1,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position + 3, red, green, blue);
		save_pixel(position + 6, red, green, blue);
		save_pixel(position + 9, red, green, blue);
		save_pixel(position + 3*local_width, red, green, blue);
		save_pixel(position + 3*local_width + 3, red, green, blue);
		save_pixel(position + 3*local_width + 6, red, green, blue);
		save_pixel(position + 6*local_width, red, green, blue);
		save_pixel(position + 6*local_width + 3, red, green, blue);
		save_pixel(position + 9*local_width, red, green, blue);

		iter2 = iter_function(number_of_iterations, real+3*9*step_real, imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*9;
		t = getT(iter2,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position - 3, red, green, blue);
		save_pixel(position - 6, red, green, blue);
		save_pixel(position - 9, red, green, blue);
		save_pixel(position + 3*local_width, red, green, blue);
		save_pixel(position + 3*local_width - 3, red, green, blue);
		save_pixel(position + 3*local_width - 6, red, green, blue);
		save_pixel(position + 6*local_width, red, green, blue);
		save_pixel(position + 6*local_width - 3, red, green, blue);
		save_pixel(position + 9*local_width, red, green, blue);

		iter3 = iter_function(number_of_iterations, real, imag+3*9*step_imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*local_width*9;
		t = getT(iter3,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position + 3, red, green, blue);
		save_pixel(position + 6, red, green, blue);
		save_pixel(position + 9, red, green, blue);
		save_pixel(position - 3*local_width, red, green, blue);
		save_pixel(position - 3*local_width + 3, red, green, blue);
		save_pixel(position - 3*local_width + 6, red, green, blue);
		save_pixel(position - 6*local_width, red, green, blue);
		save_pixel(position - 6*local_width + 3, red, green, blue);
		save_pixel(position - 9*local_width, red, green, blue);

		iter4 = iter_function(number_of_iterations, real, imag+3*9*step_imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*9 + 3*local_width*9;
		t = getT(iter4,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position - 3, red, green, blue);
		save_pixel(position - 6, red, green, blue);
		save_pixel(position - 9, red, green, blue);
		save_pixel(position - 3*local_width, red, green, blue);
		save_pixel(position - 3*local_width - 3, red, green, blue);
		save_pixel(position - 3*local_width - 6, red, green, blue);
		save_pixel(position - 6*local_width, red, green, blue);
		save_pixel(position - 6*local_width - 3, red, green, blue);
		save_pixel(position - 9*local_width, red, green, blue);

		iter5 = iter_function(number_of_iterations, real, imag+3*9*step_imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*2 + 3*local_width*2;
		t = getT(iter5,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position + 3, red, green, blue);
		save_pixel(position + 6, red, green, blue);
		save_pixel(position - 3*local_width + 3, red, green, blue);
		save_pixel(position - 3*local_width + 6, red, green, blue);
		save_pixel(position - 6*local_width + 6, red, green, blue);
		save_pixel(position + 3*local_width - 3, red, green, blue);
		save_pixel(position + 3*local_width, red, green, blue);
		save_pixel(position + 3*local_width + 3, red, green, blue);
		save_pixel(position + 3*local_width + 6, red, green, blue);
		save_pixel(position + 6*local_width - 6, red, green, blue);
		save_pixel(position + 6*local_width - 3, red, green, blue);
		save_pixel(position + 6*local_width, red, green, blue);
		save_pixel(position + 6*local_width + 3, red, green, blue);
		save_pixel(position + 6*local_width + 6, red, green, blue);

		iter6 = iter_function(number_of_iterations, real, imag+3*9*step_imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*7 + 3*local_width*2;
		t = getT(iter6,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position - 3, red, green, blue);
		save_pixel(position - 6, red, green, blue);
		save_pixel(position - 3*local_width - 3, red, green, blue);
		save_pixel(position - 3*local_width - 6, red, green, blue);
		save_pixel(position - 6*local_width - 6, red, green, blue);
		save_pixel(position + 3*local_width + 3, red, green, blue);
		save_pixel(position + 3*local_width, red, green, blue);
		save_pixel(position + 3*local_width - 3, red, green, blue);
		save_pixel(position + 3*local_width - 6, red, green, blue);
		save_pixel(position + 6*local_width - 6, red, green, blue);
		save_pixel(position + 6*local_width - 3, red, green, blue);
		save_pixel(position + 6*local_width, red, green, blue);
		save_pixel(position + 6*local_width + 3, red, green, blue);
		save_pixel(position + 6*local_width + 6, red, green, blue);

		iter7 = iter_function(number_of_iterations, real, imag+3*9*step_imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*2 + 3*local_width*7;
		t = getT(iter7,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position + 3, red, green, blue);
		save_pixel(position + 6, red, green, blue);
		save_pixel(position + 3*local_width + 3, red, green, blue);
		save_pixel(position + 3*local_width + 6, red, green, blue);
		save_pixel(position + 6*local_width + 6, red, green, blue);
		save_pixel(position - 3*local_width - 3, red, green, blue);
		save_pixel(position - 3*local_width, red, green, blue);
		save_pixel(position - 3*local_width + 3, red, green, blue);
		save_pixel(position - 3*local_width + 6, red, green, blue);
		save_pixel(position - 6*local_width - 6, red, green, blue);
		save_pixel(position - 6*local_width - 3, red, green, blue);
		save_pixel(position - 6*local_width, red, green, blue);
		save_pixel(position - 6*local_width + 3, red, green, blue);
		save_pixel(position - 6*local_width + 6, red, green, blue);

		iter8 = iter_function(number_of_iterations, real, imag+3*9*step_imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*7 + 3*local_width*7;
		t = getT(iter8,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		save_pixel(position, red, green, blue);
		save_pixel(position - 3, red, green, blue);
		save_pixel(position - 6, red, green, blue);
		save_pixel(position + 3*local_width - 3, red, green, blue);
		save_pixel(position + 3*local_width - 6, red, green, blue);
		save_pixel(position + 6*local_width - 6, red, green, blue);
		save_pixel(position - 3*local_width + 3, red, green, blue);
		save_pixel(position - 3*local_width, red, green, blue);
		save_pixel(position - 3*local_width - 3, red, green, blue);
		save_pixel(position - 3*local_width - 6, red, green, blue);
		save_pixel(position - 6*local_width - 6, red, green, blue);
		save_pixel(position - 6*local_width - 3, red, green, blue);
		save_pixel(position - 6*local_width, red, green, blue);
		save_pixel(position - 6*local_width + 3, red, green, blue);
		save_pixel(position - 6*local_width + 6, red, green, blue);
	}
}

/* super-fast prediction */
void compute_function_predict_4(int cycle_start, int cycle_end, int local_width, int local_height, double c_real, double c_imag, int number_of_iterations, double min_real, double max_imag, double step_real, double step_imag)
{
	int iter1;	// this is going to save to iterations_buffer
	int iter2;
	int iter3;
	int iter4;

	float real;
	float imag;

	for (int i = 0; i < (local_width * local_height/100); i++) {
		float t;
		int position;
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		real = min_real + ((i*10)%local_width)*step_real;
		imag = max_imag + ((i*10)/local_width)*(step_imag*10);

		iter1 = iter_function(number_of_iterations, real, imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width);
		t = getT(iter1,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		for (uint8_t j = 0; j < 5; j ++) {
			for (uint8_t k = 0; k < 5; k ++) {
				save_pixel(position + j * 3 * local_width + k * 3, red, green, blue);
			}
		}

		iter2 = iter_function(number_of_iterations, real+3*9*step_real, imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*9;
		t = getT(iter2,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		for (uint8_t j = 0; j < 5; j ++) {
			for (uint8_t k = 0; k < 5; k ++) {
				save_pixel(position + j * 3 * local_width - k * 3, red, green, blue);
			}
		}

		iter3 = iter_function(number_of_iterations, real, imag+3*9*step_imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*local_width*9;
		t = getT(iter3,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		for (uint8_t j = 0; j < 5; j ++) {
			for (uint8_t k = 0; k < 5; k ++) {
				save_pixel(position - j * 3 * local_width + k * 3, red, green, blue);
			}
		}

		iter4 = iter_function(number_of_iterations, real, imag+3*9*step_imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 3*9 + 3*local_width*9;
		t = getT(iter4,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		for (uint8_t j = 0; j < 5; j ++) {
			for (uint8_t k = 0; k < 5; k ++) {
				save_pixel(position - j * 3 * local_width - k * 3, red, green, blue);
			}
		}
	}
}

/* ultra-fast prediction */
void compute_function_predict_5(int cycle_start, int cycle_end, int local_width, int local_height, double c_real, double c_imag, int number_of_iterations, double min_real, double max_imag, double step_real, double step_imag)
{
	int iter1;	// this is going to save to iterations_buffer
	int iter2;

	float real;
	float imag;

	for (int i = 0; i < (local_width * local_height/100); i++) {
		float t;
		int position;
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		real = min_real + ((i*10)%local_width)*step_real;
		imag = max_imag + ((i*10)/local_width)*(step_imag*10);

		iter1 = iter_function(number_of_iterations, real, imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width);
		t = getT(iter1,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		for (uint8_t j = 0; j < 10; j++) {
			save_pixel(position + j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 8; j++) {
			save_pixel(position + 3*local_width + j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 8; j++) {
			save_pixel(position + 6*local_width + j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 6; j++) {
			save_pixel(position + 9*local_width + j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 6; j++) {
			save_pixel(position + 12*local_width + j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 4; j++) {
			save_pixel(position + 15*local_width + j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 4; j++) {
			save_pixel(position + 18*local_width + j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 2; j++) {
			save_pixel(position + 21*local_width + j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 2; j++) {
			save_pixel(position + 24*local_width + j*3, red, green, blue);
		}

		iter2 = iter_function(number_of_iterations, real+27*step_real, imag+27*step_imag, c_real, c_imag);
		position = 3*((10 * i)%local_width) + ((10*i)/local_width)*(30*local_width) + 27*local_width + 27;
		t = getT(iter2,number_of_iterations);
		red = getR(t);
		green = getG(t);
		blue = getB(t);
		for (uint8_t j = 0; j < 10; j++) {
			save_pixel(position - j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 8; j++) {
			save_pixel(position - 3*local_width - j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 8; j++) {
			save_pixel(position - 6*local_width - j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 6; j++) {
			save_pixel(position - 9*local_width - j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 6; j++) {
			save_pixel(position - 12*local_width - j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 4; j++) {
			save_pixel(position - 15*local_width - j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 4; j++) {
			save_pixel(position - 18*local_width - j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 2; j++) {
			save_pixel(position - 21*local_width - j*3, red, green, blue);
		}
		for (uint8_t j = 0; j < 2; j++) {
			save_pixel(position - 24*local_width - j*3, red, green, blue);
		}
	}
}
