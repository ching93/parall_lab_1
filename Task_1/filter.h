#pragma once
#include "bitmap_image.hpp"
#define NTHREADS 2

class filter
{
public:
	float PI = 3.14159;
	int kernel_size = 5;
	float teta = PI/3;
	float fi = 0;
	float gamma = 0.3;
	int compute_time;
	
	filter();
	filter(int width, int height);
	~filter();
	//int load_image(char *path);
	int save_result(char *path);
	int filter_image();
	void randomize();
private:
	//bitmap_image *image;
	int **matrix;
	float **kernel;
	float **res_matrix;
	int width;
	int height;
	int set_kernel();
	float multiply(int x0, int y0);

	float lambda;
	float sigma;

};

