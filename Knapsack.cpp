#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

struct object{
	string name;
	double work;
	double value;
};

double helper(vector<object> classes, double workload, double best){
	if(classes.size() == 0)
		return best;

	double max = best;

	for(unsigned int i = 0; i < classes.size(); i++){
		vector<object> copy(classes);
		copy.erase(copy.begin() + i);
		if(workload - classes[i].work > 0){
			double val = helper(copy, workload - classes[i].work, best + classes[i].value);
			if(val > max)
				max = val;
		}
	}

	return max;
}



int main(int arg, char **argv){
	ifstream infile(argv[1]);

	vector<object> classes;

	string first;
	getline(infile, first);

	stringstream ss(first);

	double number;
	double workload;

	ss >> number;
	ss >> workload;

	string dummy;

	string name;

	for(int counter = 0; counter <= number; counter++){
		getline(infile, dummy);
		stringstream xx(dummy);
		object m;
		classes.push_back(m);
		xx >> classes[counter].name;
		xx >> classes[counter].work;
		xx >> classes[counter].value;
	}

	cout << helper(classes, workload, 0) << endl;

	return 0;

}
