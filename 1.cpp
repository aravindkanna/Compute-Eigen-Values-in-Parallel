#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "omp.h"
#include <ctime>

using namespace std;

void call(){
	#pragma omp parallel num_threads(20)
	#pragma omp for ordered schedule(static)
	for(int i=0;i<10000000;i++){
		int id=omp_get_thread_num();
		cout << id << "    " << i << endl;
	}
}

int main(){
	clock_t start = clock();
	call();
	clock_t end = clock();
	cout << end - start << endl;
	return 0;
}