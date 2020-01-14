//Name: Justin Sindecuse-Hayden
//Email: sindecus@usc.edu
//ID: 9795422763
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
	if(argc != 2){
		return -1;
	}

	ifstream input(argv[1]);

	if(!input){
		cout << "Can't find file." << endl;
		return -1;
	}

	int x = 0;

	input >> x;

	string test1;

	char* line = new char[x];
	char* reverse = new char[x];

	int counter1 = 0;

	getline(input, test1);

	//read in file
	getline(input, test1);

    //parse the string into a dynamically allocated character array
	for(int i=0; i < x; i++){
		line[i] = test1[i];
	}

	int counter2 = 0;

    //reverse the array
	for(int i=x-1; i>=0; i--){
		reverse[counter2] = line[i];
		counter2++;
	}

	for(int i=0; i<x; i++){
		cout << reverse[i];
	}

	cout << endl;

	return 0;

}
