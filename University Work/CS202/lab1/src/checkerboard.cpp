//Alexander Yu - CS202
//Completed August 25, 2021
//Lab 1-1: Checkerboard

#include <iostream>
using namespace std;

int main() 
{
	int r; //define rows
	int c; //define columns
	char oc; //define originalCharacter, character given by input
	char sc; //define startingCharacter, character beginning every row
	char cc; //define currentCharacter, character iteration is currently on
	int cs; //define cycleSize
	int w; //define width
	int cw; //define currentWidth

	if(!(cin >> r) || !(cin >> c) || !(cin >> oc) || !(cin >> cs) || !(cin >> w)) //if any variable fails to gain an input, return an error message
	{
		cerr << "usage: checkerboard  - stdin should contain R, C, SC, CS and W" << endl;
		return -1;
	}

	if(r < 1 || c < 1 || oc < 1 || (oc + cs) > 127 || cs < 1 || w < 1) //exits silently if variables aren't within specified numbers
	{
		return -1;
	}

	cc = oc; //sets currentCharacter with originalCharacter
	sc = oc; //sets startingCharacter with originalCharacter
	cw = w; //sets currentWidth with width
	for(int i = 0; i < (r * w); ++i) //iterating rows
	{
		for(int j = 0; j < (c * w); ++j) //iterating columns
		{
			cout << cc; //prints currentCharacter
			if(cw == 1) //if currentWidth == 1, currentCharacter does not need to be repeated
			{
				cc++; //increase currentCharacter
				cw = w; //reset currentWidth
			}
			else //else, currentCharacter needs to be repeated
			{
				cw--; //decrease currentWidth
			}
			if(cc == (oc + cs)) //if currentCharacter exceeds cycleSize, resets currentCharacter
			{
				cc = oc;
			}
		}
			cout << "\n"; //newLine
			if((i+1) % w == 0) //checks if the next row requires a different startingCharacter
			{
				sc++;
				if(sc >= (oc + cs)) //if startingCharacter is greater than cycleSize, resets startingCharacter
				{
					sc = oc;
					cc = sc;
				}
				else //else, set currentCharacter to startingCharacter
				{
					cc = sc;
				}
			}
			else //sets currentCharacter to startingCharacter if no other changes are required
			{
				cc = sc;
			}

	}
	return 0;
}

