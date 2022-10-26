//Alexander Yu - CS202
//November 3rd, 2021
//Lab 9-2: Shapeshifter

/*This lab focuses on the the implementation of a Shapeshifter game, which originated from Neopets. The main goal of the game was to uses predetermined shapes to change a grid of swords and shields to just swords. This implementation uses recursion to automate the solving process, with shields as 0s and swords as 1s. It takes a grid, composed of 0s and 1s in a vector of strings, as command line arguments and takes standard input as shapes, also made by 0s and 1s, although in a vector of vector of strings.*/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Shifter { //this class defination was taken from the lab write-up
  public:

    /* Read_Grid_And_Shapes() initializes the grid from argc/argv, and the
       reads from standard input to get the shapes. */

    bool Read_Grid_And_Shapes(int argc, const char **argv);

    /* Apply_Shape() applies the shape in Shapes[index] to the grid,
       starting at row r and column c.  You'll note, if you call
       Apply_Shape twice with the same arguments, you'll end up 
       with the same grid as before the two calls. */

	bool Apply_Shape(int index, int r, int c);

    /* Find_Solution() is the recursive procedure.  It tries all possible
       starting positions for Shape[index], and calls Find_Solution() 
       recursively to see if that's a correct way to use Shape[index].
       If a recursive call returns false, then it "undoes" the shape by
       calling Apply_Shape() again. */

    bool Find_Solution(int index);

    /* This prints the solution on standard output. */

    void Print_Solution() const;

  protected:

    /* This is the grid.  I have this be a vector of 0's and 1's, because it makes
       it easy to print the grid out. */

    vector <string> Grid;                   

    /* These are the shapes, in the order in which they appear in standard input. */

    vector < vector <string> > Shapes;     

    /* These two vectors hold the starting positions of the shapes, both as you
       are finding a solution, and when you are done finding a solution. */

    vector <int> Solution_Rows;            
    vector <int> Solution_Cols;            
};

bool Shifter::Read_Grid_And_Shapes(int argc, const char **argv) //reads grid and shapes into the class
{
	istringstream ss; //stringstream to use
	vector <string> shape; //holds the entire shape
	string x, y; //holds parts for the shape
	int i; //loop iterator

	if(argc < 3) //error checks for enough arguments
	{
		cerr << "Usage: bin/ss_solver rows_of_grid ..." << endl;
		return false;
	}
	ss.str(argv[1]); //reads in the first line of the grid from command line
	if(!(ss >> x)) //error checks for valid row
	{
		cerr << "Invalid grid rows" << endl;
		return false;
	}
	Grid.push_back(x); //adds first row to grid
	ss.clear();
	for(i = 2; i < argc; i++) //adds the rest of the rows into the grid
	{
		ss.str(argv[i]);
		if(!(ss >> x) || x.size() != Grid[Grid.size() - 1].size()) //checks that all rows have the same length
		{
			cerr << "Invalid grid rows" << endl;
			return false;
		}
		Grid.push_back(x);
		ss.clear();
	}
	while(getline(cin, x)) //reads in the shapes from standard input
	{
		ss.str(x); //takes a shape as a line
		while(ss >> y) //breaks string into separate words, then placed in a vector of strings
		{
			shape.push_back(y);
		}
		Shapes.push_back(shape); //vector of strings is added to vector of vector of strings
		ss.clear();
		shape.clear();
	}
	return true;
}

bool Shifter::Apply_Shape(int index, int r, int c) //applies the shape at Shapes[index] to position r, c
{
	size_t i, j; //loop variables
	if((Shapes[index].size() - 1) + r >= Grid.size() || (Shapes[index][0].size() - 1) + c >= Grid[0].size()) //checks if shape is within grid
	{
		return false;
	}
	for(i = 0; i < Shapes[index].size(); i++) //iterates through grid starting at r, c
	{
		for(j = 0; j < Shapes[index][i].size(); j++)
		{
			if(Shapes[index][i][j] != Grid[i+r][j+c]) //if bits from shape and grid do not match, bit on grid is 1; otherwise, bit on grid is 0
			{
				Grid[i+r][j+c] = '1';
			}
			else
			{
				Grid[i+r][j+c] = '0';
			}
		}
	}
	return true;
}

bool Shifter::Find_Solution(int index) //recursive solution to shifter
{
	int r, c; //loop variables
	if(index == (int) Shapes.size()) //condition to end recursion; if index == Shapes.size(), all shapes have been placed into the grid
	{
		for(r = 0; r < (int) Grid.size(); r++) 
		{
			for(c = 0; c < (int) Grid[r].size(); c++)
			{
				if(Grid[r][c] != '1') //if at any point a 0 is found, returns false and recursion continues
				{
					return false;
				}
			}
		}
		return true; //if for loop falls out, no 0s have been found, therefore recursion ends
	}
	else
	{
		for(r = 0; r < (int) Grid.size(); r++) //applies each shape at each valid spot in the grid
		{
			for(c = 0; c < (int) Grid[r].size(); c++)
			{
				if(Apply_Shape(index, r, c))
				{
					if(Find_Solution(index + 1)) //once recursion ends, every valid location for each shape is placed into the solution vectors
					{
						Solution_Rows.push_back(r);
						Solution_Cols.push_back(c);
						return true;
					}
					else //if location isn't valid for a solution, reverses shape application by applying it again
					{
						Apply_Shape(index, r, c);
					}
				}
			}
		}
	}
	return false; //if both for loops fall out, the final shape has tried all possible locations without a solution, so this branch is dead
}

void Shifter::Print_Solution() const //prints solution
{
	size_t i, j;
	for(i = 0; i < Shapes.size(); i++)
	{
		for(j = 0; j < Shapes[i].size(); j++) //prints the shape, then its solution
		{
			cout << Shapes[i][j] << " "; 
		}	
		cout << Solution_Rows[(Solution_Rows.size() - 1) - i] << " " << Solution_Cols[(Solution_Cols.size() - 1) - i] << endl; //solution must be printed in reverse order due to recursion going backwards
	}
}

int main(int argc, const char **argv) //main procedure; creates a Shifter object then reads a grid and shapes, then finds and prints solution
{
	Shifter game;
	if(!(game.Read_Grid_And_Shapes(argc, argv)))
	{
		return 1;
	}
	if(game.Find_Solution(0))
	{
		game.Print_Solution();
	}
}
