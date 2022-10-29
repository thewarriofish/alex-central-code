//Lab AB: Spell Path - Dynamic Programming
//Alexander Yu - CS302
//April 22nd, 2022

/*This is part 2 of the dynamic programming lab. This part implements a pathfinding algorithm for an online browser game called Spellseeker.
 * Unlike the other parts, this part does not require memoization, only the implementation of a recursive algorithm.*/

#include <vector>
#include <iostream>
using namespace std;

class Path //class definition for Path
{
	public:
		int MaxPathLen(int r, int c, vector <char> grid);
		int col;
		int row;
		int max_length; 
};

int Path::MaxPathLen(int r, int c, vector <char> grid) //recursive algorithm to find longest path
{
	char hold; //holds iteration's current node
	int index, len, max_len;
	max_len = 0; //keeps track of which path is the longest for the current node
	len = 0; //holds the length of the current path
	index = col * r + c;
	hold = grid[index];
	grid[index] = '-'; //removes character from grid
	if(r != 0) //checks node above current node
	{
		if(grid[index-col] == hold + 1 || grid[index-col] == hold - 1)
		{
			len = MaxPathLen((index-col) / col, (index-col) % col, grid);
		}
		if(max_len < len)
		{
			max_len = len;
		}
	}
	if(c != 0) //checks node previous current node
	{
		if(grid[index-1] == hold + 1 || grid[index-1] == hold - 1)
		{
			len = MaxPathLen((index-1) / col, (index-1) % col, grid);
		}
		if(max_len < len)
		{
			max_len = len;
		}
	}
	if(c != col - 1) //checks node after current node
	{
		if(grid[index+1] == hold + 1 || grid[index+1] == hold - 1)
		{
			len = MaxPathLen((index+1) / col, (index+1) % col, grid);
		}
		if(max_len < len)
		{
			max_len = len;
		}
	}
	if(r != row - 1) //checks node below current node
	{
		if(grid[index+col] == hold + 1 || grid[index+col] == hold - 1)
		{
			len = MaxPathLen((index+col) / col, (index+col) % col, grid);
		}
		if(max_len < len)
		{
			max_len = len;
		}
	}
	if(c % 2 == 0) //if node is in an even column, checks nodes diagonally below
	{
		if(r != row - 1) //no nodes diagonally below if last row
		{
			if(c != col - 1) //no nodes bottom right if last column
			{
				if(grid[(index+col) + 1] == hold + 1 || grid[(index+col) + 1] == hold - 1)
				{
					len = MaxPathLen(((index+col)+1) / col, ((index+col)+1) % col, grid);
				}
				if(max_len < len)
				{
					max_len = len;
				}
			}
			if(c != 0) //no nodes bottom left if first column
			{
				if(grid[(index+col)-1] == hold + 1 || grid[(index+col)-1] == hold - 1)
				{
					len = MaxPathLen(((index+col)-1) / col, ((index+col)-1) % col, grid);
				}
				if(max_len < len)
				{
					max_len = len;
				}
			}
		}
	}
	else //if node is in an odd column, checks nodes diagonally above
	{
		if(r != 0) //no nodes above if first row
		{
			if(c != 0) 
			{
				if(grid[(index-col)-1] == hold + 1 || grid[(index-col)-1] == hold - 1)
				{
					len = MaxPathLen(((index-col)-1) / col, ((index-col)-1) % col, grid);
				}
				if(max_len < len)
				{
					max_len = len;
				}
			}
			if(c != col - 1)
			{
				if(grid[(index-col)+1] == hold + 1 || grid[(index-col)+1] == hold - 1)
				{
					len = MaxPathLen(((index-col)+1) / col, ((index-col)+1) % col, grid);
				}
				if(max_len < len)
				{
					max_len = len;
				}
			}
		}
	}
	grid[index] = hold; //returns character to node
	return max_len + 1; //includes current node in length
}

int main()
{
	int i, len;
	string x;
	vector <char> grid;
	Path p;
	p.row = 0;
	p.max_length = 0;
	while(cin >> x) //creates grid
	{
		for(i = 0; i < (int) x.size(); i++)
		{
			grid.push_back(x[i]);
		}
		p.row++; //initializes no. of rows by using no. of while loop iterations
	}
	p.col = (int) x.size(); //initializes no. of cols by using size of x, since all lines are going to be same size
	for(i = 0; i < (int) grid.size(); i++) //calls MaxPathLen on every node in the grid; every node must be tested as the start of the path
	{
		len = p.MaxPathLen(i / p.col, i % p.col, grid);
		if(p.max_length < len)
		{
			p.max_length = len;
		}
	}
	cout << p.max_length << endl;
	return 0;
}
