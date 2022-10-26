//Alexander Yu - CS202
//September 2, 2021
//Lab 2-2: Big White

#include <iostream>
#include <cstdio>
#include <sstream>
using namespace std;

int main(int argc, char* argv[])
{
	int cols, rows, i; //define variables
	istringstream ss; //initialize stringstream
	if(argc != 3) //check for correct amount of arguments
	{
		cerr << "usage: bigwhite rows cols" << endl;
		return -1;
	}
	ss.str(argv[1]); //stream in first number
	if(ss >> rows) //into rows
	{
		ss.clear(); //clear stream for next use
	}
	else //errors if rows isn't filled
	{
		cerr << "usage: bigwhite rows cols" << endl;
		return -1;
	}
	
	ss.str(argv[2]); //stream in second number
	if(ss >> cols) //into cols
	{
		ss.clear();
	}
	else //errors if cols isn't filled
	{
		cerr << "usage: bigwhite rows cols" << endl;
		return -1;
	}
	
	if(cols < 1 || rows < 1) //checks if cols or rows is non-negative
	{
		cerr << "usage: bigwhite rows cols" << endl;
		return -1;
	}

	cout << "P2" << endl; //prints header of PGM
	cout << cols << " " << rows << endl;
	cout << 255 << endl;
	for(i = 0; i < rows * cols; i++) //prints white pixels
	{
		cout << 255 << endl;
	}
	return 0;
}
