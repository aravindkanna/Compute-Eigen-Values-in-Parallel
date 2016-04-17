#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>

#include "omp.h"

using namespace std;

int main(){
	int N;
	cin >> N;

	vector<vector<double> > mat(N, vector<double> (N));
	//ofstream file;
	stringstream ss;
	ss << N;
	string s;
	ss >> s;
	s = s+".txt";
	cout << s << endl;
	ofstream mfile (s.c_str());

	
	//file.open(s + ".txt");


	return 0;
}