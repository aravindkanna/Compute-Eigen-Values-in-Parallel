#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

int main(){
	string s;
	getline(cin, s);
	vector<int> vec(10);

	istringstream iss(s);
			string token;
			int i=0;
			while(getline(iss, token, ' ')){
				//cout << token << "                " ;
				stringstream ss(token);
				int x;
				ss >> x;
				vec[i]=x;
				i++;
			}
	return 0;
}