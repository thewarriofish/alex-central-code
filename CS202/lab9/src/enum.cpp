//Alexander Yu - CS202
//November 3rd, 2021
//Lab 9-1: Enumeration

/*This lab focuses on the use of recursion to implement a procedure to enumerate a string using command line arguments.*/

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

void do_enumeration(string &s, int index, int n_ones) //procedure for enumeration
{
	string x;
	if((size_t) index == s.size()) //checks if index has reached end of string
	{
		if(n_ones == 0) //checks if all ones have been used, prints if true
		{
			cout << s << endl;
		}
		return; //ends recursion
	}
	x = s; //sets x to current enumeration
	if(n_ones < int (s.size() - index) || n_ones == 0) //sets x[index] to 0 if n_ones is less than the number of characters remaining in string or if n_ones is 0
	{
		x[index] = '0';
		do_enumeration(x, index + 1, n_ones); //recursion with new string
	}
	if(n_ones != 0) //checks if there are still ones needed to insert
	{
		x[index] = '1';
		do_enumeration(x, index + 1, n_ones - 1); //recursion with new string
	}
}

int main(int argc, char **argv) //main procedure to take command line arguments, with error checking to ensure valid parameters; uses do_enumeration with valid parameters
{
	istringstream ss;
	int length, ones;
	string x;

	if(argc != 3)
	{
		cerr << "Usage: bin/enum length ones" << endl;
		return 1;
	}
	
	ss.str(argv[1]);
	if(!(ss >> length))
	{
		cerr << "Invalid length" << endl;
		return 1;
	}
	ss.clear();
	ss.str(argv[2]);
	if(!(ss >> ones) || ones > length)
	{
		cerr << "Invalid ones" << endl;
		return 1;
	}
	x.resize(length, '-');
	do_enumeration(x, 0, ones);
}
