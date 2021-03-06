#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <fstream>
#include <ctime>
#include <algorithm>

#include "omp.h"

#define PI 3.14159265
#define CHUNK 10

using namespace std;

//####################################################################################
//UTILITY FUNCTIONS
void print_mat(vector<vector<double> > mat, int size);
void print_vec(vector<double> vec);
bool check_for_symmetricity(vector<vector<double> > mat, int size);
bool is_upper_triangular(vector<vector< double> > mat, int size);
vector<vector<double> > transpose(vector<vector<double> > A, int size);
vector<vector<double> > mat_mul(vector<vector<double> > A, vector<vector<double> > B, 
	int size);
pair<vector<vector<double> >, vector<vector<double> > > qr_decomp(vector<vector<double> > mat,
 int size);
vector<double> find_eigens(vector<vector<double> > mat, int size);
//#####################################################################################

bool check_for_symmetricity(vector<vector<double> > mat, int size){
	int flag=0;
	#pragma omp parallel for schedule(static)
	for(int i=0;i<size;i++){
		for(int j=i+1;j<size;j++){
			if(mat[i][j] != mat[j][i]){
				//return false;
				flag=1;
			}
		}
	}
	if(flag){
		return false;
	}
	return true;
}

bool is_upper_triangular(vector<vector< double> > mat, int size){
	int flag = 0;
	#pragma omp parallel for schedule(static)
	for(int i=1;i<size;i++){
		for(int j=0;j<i;j++){
			if(mat[i][j])
				flag = 1;
				//return false;
		}	
	}
	if(flag){
		return false;
	}
	return true;
}

vector<vector<double> > transpose(vector<vector<double> > A, int size){
	vector<vector<double> > B(size, vector<double> (size));
	#pragma omp parallel for ordered schedule(static)
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			#pragma omp ordered
			B[i][j] = A[j][i];
		}
	}
	return B;
}

vector<vector<double> > mat_mul(vector<vector<double> > A, vector<vector<double> > B, 
	int size){
	//multiplies A with B returns A*B
	int i, j, m;
	//int num_procs = omp_get_num_procs();
	vector<vector<double> > C(size, vector<double> (size));
	#pragma omp parallel shared(A, B, C, size) private(i, j, m) 
	#pragma omp for schedule(dynamic, 1024) ordered //nowait
	//#pragma omp parallel for schedule(static)
	for(i=0;i<size;i++) {
		for(j=0;j<size;j++) {
			C[i][j]=0.; // set initial value of resulting matrix C = 0
			//#pragma omp for schedule(static) ordered
			for(m=0;m<size;m++) {
				#pragma omp ordered
				C[i][j]=A[i][m]*B[m][j]+C[i][j];
				if((C[i][j] < cos(90*PI/180) && C[i][j] > 0) 
					|| (C[i][j] > -cos(90*PI/180) && C[i][j] < 0)){
					C[i][j] = 0.0;
				}
			}
		}
	}
	//int i=0, j=0, k=0;
	//int num_procs = omp_get_num_procs();
	//cout << "  The number of processors available = " << omp_get_num_procs ( ) << "\n";
	//cout << "  The number of threads available    = " << omp_get_max_threads() <<  "\n";
	/*#pragma omp parallel num_threads(12) shared(A, B, C, size) private(i, j, k)
		{
			#pragma omp for ordered schedule(static, CHUNK)
			for(i=0;i<size;i++){
				for(j=0;j<size;j++){
					C[i][j] = 0;
					for(k=0;k<size;k++){
						C[i][j] = C[i][j] + A[i][k]*B[k][j];
					}
				}
			}
		}*/
	

	return C;
}//eof

pair<vector<vector<double> >, vector<vector<double> > > qr_decomp(vector<vector<double> > mat,
 int size){
	vector<vector<double> > q(size, vector<double> (size));//Orthogonal Matrix
	vector<vector<double> > r(size, vector<double> (size));//Upper Triangular Matrix

	int flag = 0;
	#pragma omp parallel for schedule(dynamic, 1024)
	for(int j=0;j<size-1;j++){
		for(int i=size-1;i>j;i--){
			if(mat[i][j]){
				//need to make it zero
				int l = i;
				int h;
				for(int ii=l-1;ii>=0;ii--){
					if(mat[ii][j]){
						h = ii;
						break;
					}
				}

				//find cos as c and sin as s
				double r = sqrt((mat[l][j]*mat[l][j])+(mat[h][j]*mat[h][j]));
				double c = mat[h][j]/r;
				double s = -mat[l][j]/r;

				//create givens rotation matrix for l and h
				vector<vector<double> > G(size, vector<double> (size));
				
				for(int a=0;a<size;a++){
					for(int b=0;b<size;b++){
						if(a==b && a!=l && a!=h){
							G[a][b] = 1;
						}
						else if(a==b && (a==l || a==h)){
							G[a][b] = c;
						}
						else if((a==l || a==h) && (b==h || b==l)){
							if(a < b){
								G[a][b] = -s;
							}
							else{
								G[a][b] = s;
							}
						}
						else{
							G[a][b] = 0;
						}
					}
				}
				if(!flag){
					q = G;
					flag++;
				}
				else{
					q = mat_mul(G, q, size);
				}

				//time to make the element zero
				mat = mat_mul(G, mat, size);
			}
		}
	}
	r = mat;
	q = transpose(q, size);
	return make_pair(q, r);
}

void print_mat(vector<vector<double> > mat, int size){
	#pragma omp parallel for ordered schedule(dynamic, 1024) 
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
				#pragma omp ordered
				cout << mat[i][j] << "     ";
		}
		cout << endl;
	}
}

vector<double> find_eigens(vector<vector<double> > mat, int size){
	while(!is_upper_triangular(mat, size)){
		pair<vector<vector<double> >, vector<vector<double> > > p = qr_decomp(mat, size);
		mat = mat_mul(p.second, p.first, size);
	}	
	vector<double> res(size);
	#pragma omp parallel for ordered schedule(dynamic, 1024)
	for(int i=0;i<size;i++){
		#pragma omp ordered
		res[i] = mat[i][i];
	}
	return res;
}

void print_vec(vector<double> vec){
	int l = vec.size();
	for(int i=0;i<l;i++){
		cout << vec[i] << "  " ;
	}
	cout << endl;
}

int main(){
	int N;
	cin >> N;
	
	vector<vector<double> > mat(N, vector<double> (N));
	//int num_procs = omp_get_num_procs();

	/***
		Taking input from the file of size entered..
		Note that the N.txt file should exist for this to run.
		You can generate that using create.cpp
	***/
	stringstream ss;
	ss << N;
	string s;
	ss >> s;
	s = s+".txt";

	fstream myfile (s.c_str());
	string line;

	if(myfile.is_open()){
		int i=0, j=0;
		while(getline(myfile, line)){
			j=0;
			istringstream iss(line);
			string token;
			while(getline(iss, token, ' ')){
				//cout << token << "                " ;
				stringstream ss(token);
				ss >> mat[i][j];
				j++;
			}
			i++;
		}
		myfile.close();//it is a good way to close a file.
	}

	//check whether the matrix is symmetric or not
	if(!check_for_symmetricity(mat, N)){
		cout << "Error: The entered matrix is not symmetric!!" << endl;
		return 0;
	}

	//res is the vector which stores eigen values
	vector<double> res;
	clock_t start = clock();
	res = find_eigens(mat, N);
	sort(res.begin(), res.end());
	clock_t end = clock();
	cout << "The eigen values are as follows: " << endl;
	print_vec(res);
	cout << end-start << endl;
	/*clock_t start = clock();
	mat = mat_mul(mat, mat, N);
	clock_t end = clock();
	cout << end-start << endl;*/
	return 0;
}