/* Alex Yu */
/* Bitset Lab */
/* TAs: Chris Muncey and Zach McMichael */
/* Completed 2/4/2021 */
//https://www.youtube.com/watch?v=icCqsBIUL_Y

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

//initializes the bitset class
class BITSET
{
	private:
		vector<int> sets;
	public:
		BITSET() : sets(1, 0) {}

		bool Test(int);
		void Set(int);
		void Clear(int);

		int getNumSets();
		int getSet(int);
};

string ToBinary(int, int);

int main(int argc, char** argv)
{
	//variables to be used
	char cmd;
	int bit = -1; //initalizes as a sentinel value
	stringstream ss;
	string s;
	BITSET bitset;

	//user input
	for(cmd = 0; getline(cin, s); cmd != 'q')
	{
		ss.str(s);

		ss >> cmd >> bit;
		
		if(cmd == 'n')
		{
			cout << bitset.getNumSets() << endl;
		}
		else if(cmd == 'q')
		{
			break;
		}
		else if(bit <= -1) //error message if no # or negative # is used
		{
			cout << "Invalid bit or command" << endl;
		}
		else if(cmd == 't')
		{
			cout << bitset.Test(bit) << endl;
		}
		else if(cmd == 's')
		{
			bitset.Set(bit);
		}
		else if(cmd == 'c')
		{
			bitset.Clear(bit);
		}
		else if(cmd == 'g')
		{
			cout << ToBinary(bit, 4) << endl;
		}
		else //error message if bad command is used
		{
			cout << "Invalid bit or command" << endl;
		}
		bit = -1; //resets the bit for the next command
		ss.clear();
	}
	return(0);
}

//function to test single bit
bool BITSET::Test(int bit)
{
	bool x; //return variable needed for erasure code to work
	int num = bit/32; //finds which index in the bitset to use
	if(sets.size() - 1 < num) //resizes bitset if given bit is not currently in it
	{
		sets.resize(num + 1, 0);
	}
	x = (sets[num] >> bit) & 1; //sets return variable with test code
	for(int i = sets.size() - 1; i > 0; --i) //deletes any excess zeroes in the bitset
	{
		if(sets[i] == 0)
		{
			sets.erase(sets.begin() + i);
		}
		else
		{
			break;
		}
	}
	return x;
}

//function to set single bit
void BITSET::Set(int bit)
{
	int num = bit/32; //finds which index in the bitset to use
	if(sets.size() - 1 < num) //resizes bitset if given bit is not currently in it
	{
		sets.resize(num + 1, 0);
	}
	sets[num] |= (1 << bit); //sets bit to 1
}

void BITSET::Clear(int bit)
{
	int num = bit/32; //finds which index in the bitset to use
	if(sets.size() - 1 < num) //resizes bitset if given bit is not currently in it
	{
		sets.resize(num + 1, 0);
	}
	sets[num] &= ~(1 << bit); //sets bit to 0
	for(int i = sets.size() - 1; i > 0; --i) //deletes any excess zeroes in the bitset
	{
		if(sets[i] == 0)
		{
			sets.erase(sets.begin() + i);
		}
		else
		{
			break;
		}
	}
}

//function to get how many sets in bitset
int BITSET::getNumSets()
{
	return sets.size();
}

//function to get value of a certain set in bitset
int BITSET::getSet(int index)
{
	if(index < 0 | index >= sets.size()) //checks to see if given index is within bitset; returns 0 if not
	{
		return 0;
	}
	else
	{
		return sets[index];
	}
}

//function to convert and return value to binary
string ToBinary(int index, int space)
{
	string x = "";
	int counter = 0;
	for(int i = 31; i > -1; --i) //code to put bits into string
	{
		counter++;
		if((index >> i) & 1) //tests bit
		{
			x += "1";
		}
		else
		{
			x += "0";
		}
		if(counter == space) //code to put a space after 'space' iterations
		{
			x += " ";
			counter = 0;
		}
	}
	return x;
}

