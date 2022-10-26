//Alexander Yu - CS202
//September 2nd, 2021
//Lab 2-5: Vertical Flip

#include <iostream>
#include <cstdio>
#include <vector>
using namespace std;

int main()
{
	int rows, cols, tmpi, pix, i, j; //define rows, columns, # pixels, temporary variables
	int counter = 0; //initialize a counter at 0
	string tmps; //temporary string
	vector <int> vflip, temp, temp2; //vector to hold flipped pixels

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
	pix = rows * cols;
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
		else //adds the pixel to the temporary vector
		{
			temp.push_back(tmpi);
		}
	}
	cin >> tmps; //input for excess 
	if(!(cin.fail())) //if cin successfully finds input, outputs error
	{
		cerr << "Bad PGM file -- Extra stuff after the pixels" << endl;
		return -1;
	}
	for(i = (int) temp.size() - 1; i >= 0; i--) //fill temp2 with the pixels in reverse order
	{
		temp2.push_back(temp[i]);
	}
	temp.clear(); //clear temp vector for later use
	for(i = 0; i < rows; i++) //for loop to iterate through all pixels
    {
        for(j = 0; j < cols; j++)
        {
            temp.push_back(temp2[counter]); 
			counter++; //keep track of location in temp2
        }
        for(j = (int) temp.size() - 1; j >= 0; j--) //horizontally flip pixels
        {
            vflip.push_back(temp[j]);
        }
        temp.clear();
    }

	cout << "P2" << endl; //prints header of PGM
    cout << cols << " " << rows << endl;
    cout << 255 << endl;

    for(i = 0; i < (int) vflip.size(); i++) //prints flipped pixels
    {
        cout << vflip[i] << endl;
    }
	
	
	return 0;
}
