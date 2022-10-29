//Lab AC: Spellseeker - Dynamic Programming
//Alexander Yu - CS302
//April 25th, 2022

/*This is part 3 of the dynamic programming lab. Similar to part 2 (spellpath), this program finds the longest path in a grid, 
 *however, it also keeps track of the nodes that construct the path. Due to this additional function, memoization is required
 to circumvent long running times.*/

#include <iostream>
#include <vector>
#include <map>
using namespace std;

class Node //class definition for a node
{
	public:
		char name;
		bool visited;
		vector <int> adj; //adjacent nodes based on parameters described by spellseeker
		/*a node is adjacent if it is one more or one less than original node's value*/
		/*in addition, depending on which column it is in, a node can have adjacent
		 * nodes diagonally above or below it*/
};

class Graph //class defintion for a graph
{
	public:
		int graph_size;
		vector <Node *> nodes;
		vector <int> paths; //holds DFS path
		bool DFS(int index);
};

class Answer //class definition for Answer
{
	public:
		int length;
		int r;
		int c;
		string key;
		Answer *nexta; //holds the next node; if node is an end of a pathway, this is NULL
};

class SpellSeeker //class definition for Graph
{
	public:
		vector <char> grid;
		Graph graph; //holds graph representation of grid
		map <string, Answer *> cache; //cache for memoization
		vector <Answer *> dump; //contains all generated answers for easy deletion
		int row;
		int col;

		Answer *Solve(int r, int c);
};

bool Graph::DFS(int index) //depth first search
{
	int i;
	Node *n;
	n = nodes[index];
	if(n->visited == true) //base case: no more reachable nodes that haven't been visited
	{
		return false;
	}
	n->visited = true;
	for(i = 0; i < (int) n->adj.size(); i++)
	{
		if(nodes[n->adj[i]]->name != '-') //DFS is done with regards to the current state of the grid, i.e. how far Solve has iterated
		{
			while(DFS(n->adj[i]))
			{
				paths.push_back(index);
			}
		}
	}
	paths.push_back(index);
	return true; //this returns after the final node in the path has been reached
}

Answer *SpellSeeker::Solve(int r, int c) //recursive function for Solve
{
	int i, index;
	char hold;
	Answer *a, *current, *longest;
	string key;
	map<string, Answer *>::iterator cit;
	longest = NULL;
	key.resize(grid.size(), '-'); //creates key via DFS
	graph.DFS(r * col + c);
	for(i = 0; i < (int) graph.paths.size(); i++)
	{
		key[graph.paths[i]] = 'O';
	}
	key[r * col + c] = 'X';
	graph.paths.clear(); //resets graph elements for later DFS runs
	for(i = 0; i < graph.graph_size; i++)
	{
		graph.nodes[i]->visited = false;
	}
	cit = cache.find(key); //attempts to find key in cache
	if(cit == cache.end()) //creates a new Answer element if key isn't in cache
	{
		a = new Answer; //sets up answer's variables
		a->r = r;
		a->c = c;
		a->key = key;
		hold = grid[r*col+c]; //stores character at grid and replaces it with a '-'
		grid[r*col+c] = '-';
		graph.nodes[r*col+c]->name = '-';
		for(i = 0; i < (int) graph.nodes[r * col + c]->adj.size(); i++) //checks every adjacent node
		{
			index = graph.nodes[r*col+c]->adj[i];
			if(grid[index] != '-') //if the adjacent node isn't a '-', calls Solve on it
			{
				current = Solve(index / col, index % col);
				if((longest == NULL || current->length > longest->length)) //if the resulting Answer has a longer length, becomes longest
				{
					longest = current;
				}
			}
		}
		grid[r*col+c] = hold; //returns original character to grid
		graph.nodes[r*col+c]->name = hold;
		a->nexta = longest;
		if(a->nexta != NULL) 
		{
			a->length = a->nexta->length + 1;
		}
		else //if nexta is NULL, node is end of a path
		{
			a->length = 1;
		}
		cache.insert(make_pair(key, a)); //stores key and answer into cache
		dump.push_back(a);
		return a;
	}
	else //if key in cache, returns its value
	{
		return cit->second;
	}
}

int main()
{
	int i;
	SpellSeeker s;
	Answer *a, *longest;
	Node *n;
	longest = NULL;
	string x;
	s.graph.graph_size = 0;
	while(cin >> x) //creates grid for easier creation of graph
	{
		for(i = 0; i < (int) x.size(); i++)
		{
			s.grid.push_back(x[i]);
		}
		s.row++; //every cycle is one row
	}
	s.col = (int) x.size(); //every line in a file has the same amount of characters, so length of one line is columns
	for(i = 0; i < (int) s.grid.size(); i++) //creates the graph 
	{
		n = new Node;
		n->name = s.grid[i];
		n->visited = false;
		/*the following section creates adjacency list for a node. It uses similar logic seen in spellpath.*/
		if(i % s.col != 0 && (s.grid[i - 1] == s.grid[i] + 1 || s.grid[i - 1] == s.grid[i] - 1)) //checks to the left of node
		{
			n->adj.push_back(i-1);
		}
		if(i/s.col != 0 && (s.grid[i - s.col] == s.grid[i] + 1 || s.grid[i - s.col] == s.grid[i] - 1)) //checks above node
		{
			n->adj.push_back(i-s.col);
		}
		if((i % s.col) % 2 == 1) //checks diagonally above node
		{
			if(i / s.col != 0)
			{
				if(i % s.col != s.col - 1 && (s.grid[i - s.col + 1] == s.grid[i] + 1 || s.grid[i - s.col + 1] == s.grid[i] - 1))
				{
					n->adj.push_back(i-s.col+1);
				}	
				if(i % s.col != 0 && (s.grid[i - s.col - 1] == s.grid[i] + 1 || s.grid[i - s.col - 1] == s.grid[i] - 1))
				{
					n->adj.push_back(i-s.col-1);
				}	

			}
		}
		if(i % s.col != s.col - 1 && (s.grid[i + 1] == s.grid[i] + 1 || s.grid[i + 1] == s.grid[i] - 1)) //checks to the right of the node
		{
			n->adj.push_back(i+1);
		}
		if(i / s.col != s.row - 1 && (s.grid[i + s.col] == s.grid[i] + 1 || s.grid[i + s.col] == s.grid[i] - 1)) //checks below node
		{
			n->adj.push_back(i+s.col);
		}
		if((i %  s.col) % 2 == 0) //checks diagonally below node
		{
			if(i / s.col != s.row - 1)
			{
				if(i % s.col != s.col - 1 && (s.grid[i + s.col + 1] == s.grid[i] + 1 || s.grid[i + s.col + 1] == s.grid[i] - 1))
				{
					n->adj.push_back(i+s.col+1);
				}	
				if(i % s.col != 0 && (s.grid[i + s.col - 1] == s.grid[i] + 1 || s.grid[i + s.col - 1] == s.grid[i] - 1))
				{
					n->adj.push_back(i+s.col-1);
				}	
			}
		}
		s.graph.nodes.push_back(n);
		s.graph.graph_size++;
	}
	for(i = 0; i < (int) s.grid.size(); i++) //iterates through grid, calling Solve on each element
	{
		a = s.Solve(i / s.col, i % s.col);
		if(longest == NULL || a->length > longest->length) //if a node has a longer path than the node in longest, replaces longest
		{
			longest = a;
		}
		if(i % s.col == 0 && i != 0) //newline after col amount of cycles
		{
			cout << endl;
		}
		cout << s.grid[i]; //prints char at grid
	}
	cout << endl << "PATH" << endl; //prints path
	while(longest != NULL) //prints spaces in path
	{
		cout << longest->r << " " << longest->c << endl;
		longest = longest->nexta;
	}
	for(i = 0; i < s.graph.graph_size; i++) //deletes all nodes of graph
	{
		delete s.graph.nodes[i];
	}
	for(i = 0; i < (int) s.dump.size(); i++) //deletes all created answers
	{
		delete s.dump[i];
	}
	return 0;
}
