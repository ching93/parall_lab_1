//#include "stdafx.h" 
#include <iostream> 
//#include <conio.h> 
#include <string> 
#include <omp.h> 
#include <ctime>
#include "filter.h"
#include <memory>

using namespace std;


int main()
{
	clock_t start = clock();
	
	filter *f = new filter(12, 12);
	printf("creating initial matrix...\n");
	f->randomize();
	printf("computing filtered matrix...\n");
	int times[20];
	for (int i = 0; i < 20; i++) {
	f->filter_image();
		printf("total time: %dms\n", f->compute_time);
	}
	system("pause");
}