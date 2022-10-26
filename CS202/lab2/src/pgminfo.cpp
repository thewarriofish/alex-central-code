//Alexander Yu - CS202
//September 1st, 2021
//Lab 2-1: PGMInfo

#include <iostream>
#include <cstdio>
using namespace std;

int main()
{
	double average = 0; //define average
	int rows, cols, pix, tmpi, i; //define rows, columns, # of pixels, and temporary variables
	string tmps; //temporary string

	cin >> tmps; //input first line to check P2
	if(tmps != "P2") //checks if first line is P2
	{
		cerr << "Bad PGM file -- first word is not P2" << endl;
		return -1;
	}
	cin >> cols; //input next 2 numbers for columns and rows
	if(cin.fail() || cols <= 0) //checks if cols are non-negative and real
	{
		cerr << "Bad PGM file -- No column specification" << endl;
		return -1;
	}
	cin >> rows;
	if(cin.fail() || rows <= 0) //checks if cols and rows are non-negative and real
	{
		cerr << "Bad PGM file -- No row specification" << endl;
		return -1;
	}
	pix = rows * cols; //amount of pixels equal to rows times cols
	cin >> tmpi; //input next number to check for 255
	if(tmpi != 255) //checks if 255 follows cols and rows
	{
		cerr << "Bad PGM file -- No 255 following the rows and columns" << endl;
		return -1;
	}
	for(i = 0; i < pix; i++) //for loop to check all pixels
	{
		cin >> tmpi; //input pixel
		if(cin.fail() || tmpi < 0 || tmpi > 255) //checks if pixel is real and is between 0 and 255
		{
			cerr << "Bad PGM file -- pixel " << i << " is not a number between 0 and 255" << endl;
			return -1;
		}
		else //add pixel into average
		{
			average += tmpi;
		}
	}
	average /= (double) pix; //calculate average by dividing by pixel
	cin >> tmps; //input for excess 
	if(!(cin.fail())) //if cin successfully finds input, outputs error
	{
		cerr << "Bad PGM file -- Extra stuff after the pixels" << endl;
		return -1;
	}
	printf("# Rows: %11d\n", rows); //prints all values
	printf("# Columns: %8d\n", cols);
	printf("# Pixels: %9d\n", pix);
	printf("Avg Pixel: %8.3lf\n", average);
	

	return 0;
}
