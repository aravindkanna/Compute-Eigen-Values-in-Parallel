#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int main(){
	int N;
	cin >> N;

	vector<vector<int> > vec(N, vector<int> (N));

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
			cout << line << endl;
			j=0;
			istringstream iss(line);
			string token;
			while(getline(iss, token, ' ')){
				//cout << token << "                " ;
				stringstream ss(token);
				ss >> vec[i][j];
				j++;
			}
			i++;
			//cout << endl;
		}
		myfile.close();
	}

	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			cout << vec[i][j] << "   ";
		}
		cout << endl;
	}
	return 0;
}