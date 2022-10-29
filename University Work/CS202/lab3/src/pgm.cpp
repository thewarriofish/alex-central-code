//Alexander Yu - CS202
//September 11, 2021
//Lab 3: PGM Implementation

/*This lab focuses on implementing various methods involving a .pgm file, like writing, rotating, duplicating, cropping, and adding pixels*/

#include "pgm.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

bool Pgm::Read(const std::string &file) //pre-written 
{
  ifstream fin;
  string s;
  size_t i, j, r, c, v;

  fin.open(file.c_str());
  if (fin.fail()) return false;
  
  if (!(fin >> s)) return false;
  if (s != "P2") return false;
  if (!(fin >> c) || c <= 0) return false;
  if (!(fin >> r) || r <= 0) return false;
  if (!(fin >> i) || i != 255) return false;

  Pixels.resize(r);
  for (i = 0; i < r; i++) {
    Pixels[i].clear();
    for (j = 0; j < c; j++) {
      if (!(fin >> v) || v > 255) return false;
      Pixels[i].push_back(v);
    }
  }
  if (fin >> s) return false;
  fin.close();
  return true;
}

bool Pgm::Write(const std::string &file) const //a function that writes a stored pixel vector into a .pgm file
{
	size_t i, j; //initialization of variables to be used, as well as file stream
	int count = 0;
	ofstream fout;

	fout.open(file.c_str());
	if(fout.fail()) //opens and checks if file is able to be opened
	{
		return false;
	}
	if(Pixels.size() == 0 || Pixels[0].size() == 0) //checks if pixel vector is filled
	{
		return false;
	}
	fout << "P2\n";
	fout << Pixels[0].size() << " " << Pixels.size() << "\n"; //adds pgm header into file
	fout << "255\n";
	for(i = 0; i < Pixels.size(); i++) //adds pixels into file
	{
		for(j = 0; j < Pixels[i].size(); j++)
		{
			if(Pixels[i][j] < 0 || Pixels[i][j] > 255) //checks pixel to ensure each is valid
			{
				return false;
			}
			if(count % 20 == 0) //if pixel is beginning of row, no space is added
			{
				fout << Pixels[i][j];
				count++;
			}
			else
			{
				fout << " " << Pixels[i][j];
				count++;
			}
			if(count % 20 == 0 && count != 0) //if pixel is 20th of the row, adds a newline
			{
				fout << "\n";
			}
		}
	}
	if(count % 20 != 0) //adds a newline at the end of pixels if totals pixels is not divisible by 20
	{
		fout << "\n";
	}
	fout.close(); //closes file stream
	return true;
}


bool Pgm::Create(size_t r, size_t c, size_t pv)
{
	size_t i, j; //initializes variables for loops
	if(r < 1 || c < 1 ||  pv > 255) //error checks for valid arguments
	{
		return false;
	}
	Pixels.resize(r); //Pixel array is resized to number of rows
	for(i = 0; i < r; i++) //adds pixel of value pv equal to r * c
	{
		for(j = 0; j < c; j++)
		{
			Pixels[i].push_back(pv);
		}
	}
	return true;
}
        
bool Pgm::Clockwise()
{
	int i, j; //variables for loops
	vector < vector <int> > CW; //vector to store rotated image
	if(Pixels.size() == 0 || Pixels[0].size() == 0) //error checks for pixel vector
	{
		return false;
	}
	CW.resize(Pixels[0].size()); //rotated image is given size equal to no. of columns in non-rotated image
	for(i = 0; i < (int) Pixels[0].size(); i++) //loop begins at first column, last row for clockwise
	{
		for(j = Pixels.size() - 1; j >= 0; j--)
		{
			CW[i].push_back(Pixels[j][i]);
		}
	}
	Pixels.resize(CW.size()); //replaces stored image with rotated image
	for(i = 0; i < (int) Pixels.size(); i++)
	{
		Pixels[i].resize(CW[i].size());
		for(j = 0; j < (int) Pixels[i].size(); j++)
		{
			Pixels[i][j] = CW[i][j];
		}
	}
	return true;
}
bool Pgm::Cclockwise()
{
	int i, j, count; //variables for loops
	count = 0; //counter variable
	vector < vector <int> > CCW; //vector to store rotated image
	if(Pixels.size() == 0 || Pixels[0].size() == 0) //error checks for pixel vector
	{
		return false;
	}
	CCW.resize(Pixels[0].size()); //rotated image is given size equal to no. of columns in non-rotated image
	for(i = Pixels[0].size() - 1; i >= 0; i--) //loop begins at last column, first row for counter-clockwise
	{
		for(j = 0; j < (int) Pixels.size(); j++)
		{
			CCW[count].push_back(Pixels[j][i]);
		}
		count++;
	}
	Pixels.resize(CCW.size()); //replaces stored image with rotated image
	for(i = 0; i < (int) Pixels.size(); i++)
	{
		Pixels[i].resize(CCW[i].size());
		for(j = 0; j < (int) Pixels[i].size(); j++)
		{
			Pixels[i][j] = CCW[i][j];
		}
	}
	return true;
}
bool Pgm::Pad(size_t w, size_t pv)
{
	size_t i, j;
	vector < vector <int> > pad; //vector to hold padded image
	vector <int> border; //vector to hold upper and lower padding
	if(w == 0 || pv > 255) //error checks valid arguments
	{
		return false;
	}
	for(i = 0; i < Pixels[0].size() + (2*w); i++) //adds enough pixels for correct border
	{
		border.push_back(pv);
	}
	for(i = 0; i < w; i++) //adds upper padding
	{
		pad.push_back(border);
	}
	pad.resize(Pixels.size() + w); //resizes pad to fit all pixels plus bottom padding
	for(i = 0; i < Pixels.size(); i++) //adds pixels per row with padding
	{
		for(j = 0; j < w; j++) //adds front padding
		{
			pad[i+w].push_back(pv);
		}
		for(j = 0; j < Pixels[i].size(); j++) //adds pixels
		{
			pad[i+w].push_back(Pixels[i][j]);
		}
		for(j = 0; j < w; j++) //adds back padding
		{
			pad[i+w].push_back(pv);
		}
	}
	for(i = 0; i < w; i++) //add bottom padding
	{
		pad.push_back(border);
	}
	Pixels.resize(pad.size()); //replaces stored image with padded image
	for(i = 0; i < Pixels.size(); i++)
	{
		Pixels[i].resize(pad[i].size());
		for(j = 0; j < Pixels[i].size(); j++)
		{
			Pixels[i][j] = pad[i][j];
		}
	}
	
	return true;
}
bool Pgm::Panel(size_t r, size_t c)
{
	vector < vector <int> > panel; //vector to hold paneled image
	size_t i, j, k; //variables for loops
	if(r == 0 || c == 0 || Pixels.size() == 0 || Pixels[0].size() == 0) //error checks for arguments and stored image
	{
		return false;
	}
	panel.resize(Pixels.size()); //resizes panel to amount of rows of stored image
	for(i = 0; i < Pixels.size(); i++) //the image is replicated c times and placed horizontally
	{
		for(j = 0; j < c; j++)
		{
			for(k = 0; k < Pixels[i].size(); k++)
			{
				panel[i].push_back(Pixels[i][k]);
			}
		}
	}	
	for(i = 1; i < r; i++) //the newly-created row of images is replicated r times
	{
		for(j = 0; j < Pixels.size(); j++)
		{
			panel.push_back(panel[j]);
		}
	}
	Pixels.resize(panel.size()); //replaces stored image with paneled image
	for(i = 0; i < Pixels.size(); i++)
	{
		Pixels[i].resize(panel[i].size());
		for(j = 0; j < Pixels[i].size(); j++)
		{
			Pixels[i][j] = panel[i][j];
		}
	}
	return true;
}

bool Pgm::Crop(size_t r, size_t c, size_t rows, size_t cols)
{	
	size_t i; //variable for loops
	if(rows + r > Pixels.size() || cols + c > Pixels[0].size() || rows == 0 || cols == 0) //error checks for valid parameters
	{
		return false;
	}
	if(r != 0) //if crop does not begin at row 1, removes all pixels in stored image that is before the starting row
	{
		Pixels.erase(Pixels.begin(), Pixels.begin() + r);
	}
	if(c != 0) //if crop does not begin at column 1, removes all pixels in stored image that is before the starting column
	{
		for(i = 0; i < Pixels.size(); i++)
		{
			Pixels[i].erase(Pixels[i].begin(), Pixels[i].begin() + c);
		}
	}
	if(rows != Pixels.size()) //if crop does not end at the last row, removes all pixels after the length of crop
	{
		Pixels.erase(Pixels.begin() + rows, Pixels.end());
	}
	if(cols != Pixels[0].size()) //if crop does not end at the last column, removes all pixels after the width of crop
	{
		for(i = 0; i < Pixels.size(); i++)
		{
			Pixels[i].erase(Pixels[i].begin() + cols, Pixels[i].end());
		}
	}
	return true;	
}
