#include "filter.h"
#include <math.h>
#include <ctime>
#include <omp.h>
filter::filter()
{
}
filter::filter(int width, int height) {
	this->width = width;
	this->height = height;
	this->matrix = new int*[this->width];
	for (int i = 0; i < this->width; i++)
		this->matrix[i] = new int[this->height];
}
void filter::randomize() {
	//printf("initial data:\n");
	for (int i = 0; i < this->width; i++) {
		for (int j = 0; j < this->height; j++) {
			srand(i*j);
			matrix[i][j] = rand() * 256 / RAND_MAX;
			//printf("%d ", rand() * 256 / RAND_MAX);
		}
		//printf("\n");
	}

}
//int filter::load_image(char *path) {
//	std::string filename(path);
//	int res = 0;
//	try {
//		this->image = new bitmap_image(filename);
//	}
//	catch (exception e) {
//		res = 1;
//	}
//	return res;
//}
int filter::set_kernel() {
	int flag = 0;
	double cur_val;
	short x, y;
	
	this->kernel = new float*[this->kernel_size];
	//printf("Kernels:\n");
	float cteta = cos(teta);
	float steta = sin(teta);

	for (int i = 0; i < kernel_size; i++)
	{
		kernel[i] = new float[this->kernel_size];
		float x = i - kernel_size / 2;
		for (int j = 0; j < kernel_size; j++) {
			float y = j - kernel_size / 2;
			kernel[i][j] = (float)exp(-(pow(x*cteta + y * steta, 2) + pow(gamma,2)*pow(-x * cteta + y * steta, 2)) / 2 / pow(sigma, 2))*
				cos(2 * PI / lambda * (x*cteta + y * steta) + fi);
			//printf("%f ",kernel[i][j]);
		}
		//printf("\n");
	}
	return flag;
}

float filter::multiply(int x0, int y0) {
	float value = 0;
	for (int x = 0; x < this->kernel_size; x++)
		for (int y = 0; y < this->kernel_size; y++)
			value += kernel[x][y]*this->matrix[x + x0][y + y0];
	//printf("%f ", value);
	return value;
}

int filter::filter_image() {
	this->sigma = 0.0036 * pow(kernel_size, 2) + 0.35 * kernel_size + 0.18;
	this->lambda = sigma / 0.8;
	this->set_kernel();
	omp_set_num_threads(NTHREADS);

	//printf("Result matrix:\n");
	//clock_t start = clock();
	this->res_matrix = new float*[width - kernel_size + 1];
	#pragma omp parallel shared(this->matrix, this->res_matrix, this->kernel)
	#pragma omp for schedule(dynamic, NTHREADS)
	for (int i = 0; i < this->width-kernel_size+1; i++) {
		this->res_matrix[i] = new float[height-kernel_size+1];
		for (int j = 0; j < this->height-kernel_size+1; j++) {
			this->res_matrix[i][j] = (int)this->multiply(i, j);
			//printf("%f ", res_matrix[i][j]);
		}
		//printf("\n");
	}
	//this->compute_time = clock() - start;
	return 1;
}

filter::~filter()
{
	delete[] this->matrix;
	delete[] this->res_matrix;
	delete[] this->kernel;
}
