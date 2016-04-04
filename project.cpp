#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <vector>

using namespace std;

bool check_for_symmetricity(vector<vector<int> > mat, int size){
	for(int i=0;i<size;i++){
		for(int j=i+1;j<size;j++){
			if(mat[i][j] != mat[j][i]){
				return false;
			}
		}
	}
	return true;
}

int main(){
	int N;
	cin >> N;
	
	vector<vector<int> > mat(N, vector<int> (N));

	cout << "Enter Elements Row-wise" << endl;
	for(int i=0;i<N;i++){
		cout << "Enter row: " << i << endl; 
		for(int j=0;j<N;j++){
			int x;
			cin >> x;
			mat[i][j] = x;
		}
	}

	//check whether the matrix is symmetric or not
	if(!check_for_symmetricity(mat, N)){
		cout << "Error: The entered matrix is not symmetric!!" << endl;
		return 0;
	}

	return 0;
}