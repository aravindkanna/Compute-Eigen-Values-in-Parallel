#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>

#include "omp.h"

using namespace std;

int main(){
	int N;
	cin >> N;

	vector<vector<double> > mat(N, vector<double> (N));

	//creating file
	stringstream ss;
	ss << N;
	string s;
	ss >> s;
	s = s+".txt";
	cout << s << " is created!!"<< endl;
	ofstream mfile (s.c_str());

	clock_t c_start = clock();
	#pragma omp parallel for private(i, j)
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			if(i > j){
				mat[i][j] = mat[j][i];
			}
			else{
				mat[i][j] = rand()%100000 + 1;
				//mat[i][j] = j;
			}
			mfile << mat[i][j] << " " ;
		}
		if(i!=N-1)
			mfile << endl;
	}
	clock_t c_end = clock();

	//cout << c_end - c_start << endl;


	

	return 0;
}