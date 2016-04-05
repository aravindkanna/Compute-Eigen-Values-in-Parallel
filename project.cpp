#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <vector>
#include <math.h>
#include <stdlib.h>

#include "omp.h"

#define PI 3.14159265

using namespace std;

void print_mat(vector<vector<double> > mat, int size);

bool check_for_symmetricity(vector<vector<double> > mat, int size){
	for(int i=0;i<size;i++){
		for(int j=i+1;j<size;j++){
			if(mat[i][j] != mat[j][i]){
				return false;
			}
		}
	}
	return true;
}//eof

vector<vector<double> > transpose(vector<vector<double> > A, int size){
	vector<vector<double> > B(size, vector<double> (size));
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			B[i][j] = A[j][i];
		}
	}
	return B;
}

vector<vector<double> > mat_mul(vector<vector<double> > A, vector<vector<double> > B, 
	int size){
	//multiplies A with B returns A*B
	int i, j, m;
	vector<vector<double> > C(size, vector<double> (size));
	//#pragma omp parallel
	#pragma omp parallel for private(m, j)
	for(i=0;i<size;i++) {
		for(j=0;j<size;j++) {
			C[i][j]=0.; // set initial value of resulting matrix C = 0
			for(m=0;m<size;m++) {
				C[i][j]=A[i][m]*B[m][j]+C[i][j];
				if((C[i][j] < cos(90*PI/180) && C[i][j] > 0) 
					|| (C[i][j] > -cos(90*PI/180) && C[i][j] < 0)){
					C[i][j] = 0.0;
				}
			}
		}
	}
	return C;
}//eof

pair<vector<vector<double> >, vector<vector<double> > > qr_decomp(vector<vector<double> > mat,
 int size){
	vector<vector<double> > q(size, vector<double> (size));//Orthogonal Matrix
	vector<vector<double> > r(size, vector<double> (size));//Upper Triangular Matrix

	int flag = 0;
	for(/*int i=size-1;i>0;i--*/int j=0;j<size-1;j++){
		for(/*int j=0;j<size-1;j++*/int i=size-1;i>j;i--){
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

				/*double theta = atan(-mat[l][j]/mat[h][j])*180/PI;
				c = cos(theta);
				s = sin(theta);*/

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
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			/*if(abs(mat[i][j]) < cos(90*PI/180)){
				cout << '0' << "     ";
			}
			else*/
				cout << mat[i][j] << "     ";
		}
		cout << endl;
	}
}

int main(){
	int N;
	cin >> N;
	
	vector<vector<double> > mat(N, vector<double> (N));

	cout << "Enter Elements Row-wise" << endl;
	for(int i=0;i<N;i++){
		cout << "Enter row: " << i << endl; 
		for(int j=0;j<N;j++){
			double x;
			cin >> x;
			mat[i][j] = x;
		}
	}

	//check whether the matrix is symmetric or not
	if(!check_for_symmetricity(mat, N)){
		cout << "Error: The entered matrix is not symmetric!!" << endl;
		return 0;
	}

	vector<vector<double> > original(N, vector<double> (N));
	original = mat;

	//mat = mat_mul(mat, mat, N);
	//print_mat(mat, N);

	pair<vector<vector<double> >, vector<vector<double> > > p = qr_decomp(mat, N);
	print_mat(p.first, N);
	cout << endl ;
	cout << endl;
	print_mat(p.second, N);
	cout << endl;
	cout << endl;

	return 0;
}