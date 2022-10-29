//Alexander Yu - CS302
//Lab 4: Enumeration
//February 17th, 2022

/*This lab focuses on the use of recursion to implement enumeration of matrices. Both permutations and combinations are implemented in this manner.*/

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
using namespace std;

class Matrix //class definition taken from writeup
{
	public:
		int W;
		int E;
		int F;
		vector <int> Perm;
		vector <int> Non_X;
		vector <int> E_ID;
		void Print();
		void Permute(int index);
		void Choose(int index);
};

void Matrix::Print() //method to print matrix
{
	int i, j, k, h;
	j = 0;
	k = 0;
	if(F == 'x') //prints in format 'x', so 'X', 'E' and '.' are used
	{
		for(i = 0; i < W * W; i++) //loops through entire matrix
		{
			if(i % W == 0 && i != 0) //line breaks if a row is size W
			{
				cout << endl;
			}
			if(j < (int) Perm.size() && i == Perm[j] + (j*W)) //prints 'X' if i + (column * W) is found in Perm
			{
				cout << 'X';
				j++;
			}
			else if(E <= (int) Non_X.size()/2 || Non_X.size() == E_ID.size()) //this code is performed if there are more '.' than 'E'
			{
				if(k < (int) E_ID.size() && i == E_ID[k]) //prints 'E' if i is found in E_ID
				{
					cout << 'E';
					k++;
				}
				else //prints '.' in all other cases
				{
					cout << '.';
				}
			}
			else //this code is performed if there are more 'E' than '.'
			{
				if(k < (int) E_ID.size() && i == E_ID[k]) //prints '.' if i is found in E_ID
				{
					cout << '.';
					k++;
				}
				else //prints 'E' in all other cases
				{
					cout << 'E';
				}
			}
		}
		cout << endl << endl;
	}
	else if(F == 'h') //if format is 'h', prints hex digits corresponding to matrix rows
	{
		if(E <= (int) Non_X.size()/2 || Non_X.size() == E_ID.size()) //this code is performed if there are more '.' than 'E'
		{
			for(i = 0; i < W; i++)
			{
				h = 0;
				h |= (1ULL << Perm[i]); //sets bit at Perm[i] to 1
				for(j = 0; j < (int) E_ID.size(); j++)
				{
					if(E_ID[j] >= i * W && E_ID[j] < (i+1) * W) //sets bit at E_ID[j] if it's less than index of beginning of next row and more than i
					{
						h |= (1ULL << (E_ID[j] % W));
					}
				}
				cout << hex << h << endl; //prints hex 
			}
		}
		else //this code is performed if there are more 'E' than '.'
		{
			for(i = 0; i < W; i++) 
			{
				h = 0;
				h = ~h; //changes all bits to 1
				for(j = 0; j < (int) E_ID.size(); j++)
				{
					if(E_ID[j] >= i * W && E_ID[j] < (i+1) * W) //if E_ID[j] is less than index of beginning of next row and more than i, clears bit
					{	
						h &= (~(1ULL << (E_ID[j] % W)));
					}	
				}
				h &= (1ULL << W) - 1; //masks h to acquire last W bits
				cout << hex << h << endl;
			}
		}
		cout << endl;
	}
}

void Matrix::Permute(int index) //recursive method to permute
{
	int i, j, tmp;
	Matrix ma; //creates new Matrix
	if(index == (int) Perm.size()) //base case
	{
		ma.W = W; //sets new Matrix parameters to current Matrix parameters
		ma.E = E;
		ma.F = F;
		ma.Perm = Perm;
		j = 0;
		for(i = 0; i < W * W; i++) //creates new Non_X vector using Perm vector
		{
			if(i != ma.Perm[j] + (j * W)) //adds value to Non_X if it is not found in Perm
			{
				ma.Non_X.push_back(i);
			}
			else
			{
				j++;
			}
		}
		if((int) ma.Non_X.size() == ma.E) //if empty spaces equal to E, sets E_ID to Non_X
		{
			ma.E_ID = ma.Non_X;
		}
		ma.Choose(0); //calls recursive Choose
		return;
	}

	for(i = index; i < (int) Perm.size(); i++) //recursive method, implemented from Enumeration writeup
	{
		tmp = Perm[i];
		Perm[i] = Perm[index];
		Perm[index] = tmp;

		Permute(index+1);

		tmp = Perm[i];
		Perm[i] = Perm[index];
		Perm[index] = tmp;
	}
}

void Matrix::Choose(int index) //recursive method to combine
{
	if(E <= (int) Non_X.size()/2 || Non_X.size() == E_ID.size()) //this code performs if there are less 'E's than '.'s
	{
		if((int) E_ID.size() == E) //base case; calls print
		{
			Print();
			return;
		}
	}
	else //this code performs if there are less '.'s than 'E's
	{
		int N = (W*W) - W - E; //finds amount of spaces that are '.' 
		if((int) E_ID.size() == N) //base case
		{
			Print();
			return;
		}
	}
	if(1 > (int) Non_X.size() - index) //2nd base case; ends recursion
	{
		return;
	}
	E_ID.push_back(Non_X[index]); //recursive combination implemented from Enumeration writeup
	Choose(index+1);
	E_ID.pop_back();
	Choose(index+1);
}

int main(int argc, char* argv[])
{
	Matrix m;
	size_t i;
	string s, x;
	stringstream ss;
	int W, E;
	char F;
	x = "";
	if(argc != 4) //checks command line for correct input
	{
		cout << "usage: ./matrix_enum W E F" << endl;
		return -1;
	}
	for(i = 1; i < (size_t) argc; i++) //adds input into string for stringstream
	{
		s = argv[i];
		x += s + " ";
	}
	ss.str(x);
	if(!(ss >> W >> E >> F)) //checks if input is valid
	{
		cout << "Invalid input" << endl;
		return -1;
	}
	if(W < 1 || E < 0 || (F != 'x' && F != 'h'))
	{
		cout << "Invalid input" << endl;
		return -1;
	}
	m.W = W;
	m.E = E;
	m.F = F;
	for(i = 0; i < (size_t) m.W; i++) //creates first permutation
	{
		m.Perm.push_back(i);
	}
	m.Permute(0); //calls recursive permute
	return 0;
}
