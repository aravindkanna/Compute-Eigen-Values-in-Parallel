#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector<vector<double> > vec(10, vector<double> (10));
	if(!vec){
		cout << "haha" << endl;
	}
	return 0;
}