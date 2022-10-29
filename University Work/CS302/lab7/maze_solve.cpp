//Lab 7: Depth First Search - Maze Solver
//Alexander Yu - CS302
//March 28th, 2022

/*This lab focuses on the implementation and use of graphs and Depth First Search. It uses both of these concepts to create an automatic maze solver*/

#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;


class Node //class definition of a node
{
	public:
		bool visited; //determines if a node is visited or not
		list <int> adj; //holds all edges of a node; uses a list for quick deletion 
};

class Graph //class definition of a graph
{
	public:
		int siz; //size of graph
		vector <Node *> nodes; //holds all nodes of a graph
		vector <int> paths; //holds all edges in the graph, which serve as the maze pathway
		bool DFS(int index); //recursive method for depth first search
};

bool Graph::DFS(int index) //Depth First Search implementation; uses a boolean to determine if a path if found
{
	if(index == siz - 1) //base case 1: DFS has reached end of maze
	{
		paths.push_back(index); //begins inserting indexes of path into paths vector
		return true;
	}
	Node *n;
	list <int>::iterator lit; //iterator for adjacency list
	
	n = nodes[index];
	if(n->visited) //base case 2: node has been visited already
	{
		return false;
	}
	n->visited = true; //sets n to visited so DFS ignores node later
	for(lit = n->adj.begin(); lit != n->adj.end(); lit++) //iterates through node's adjacency list
	{
		if(DFS(*lit)) //if edge leads to valid path, inserts index into paths vector
		{
			paths.push_back(index);
			return true;
		}
	}
	return false; //if this false is reached, no path is found out of the maze
}

int main()
{
	int r, c, i, first, second;
	string x;
	Graph g;
	list <int>::iterator lit;
	Node *n;
	cin >> x; //reads in parameters through standard input
	if(x == "ROWS")
	{
		cin >> r;
	}
	cin >> x;
	if(x == "COLS")
	{
		cin >> c;
	}
	g.siz = r*c; //sets g's size equal to rows * columns
	cout << "ROWS " << r << " COLS " << c << endl; 
	for(i = 0; i < g.siz; i++) //initializes g's nodes vector
	{
		n = new Node;
		n->visited = false;
		if((i+1) % c != 0) //if i is not in last column, links index to right of i to i
		{
			n->adj.push_back(i+1);
		}
		if(i < (r*c) - c) //if i is not in last row, links index below i to i
		{
			n->adj.push_back(i+c);
		}
		if(i >= c) //if i is not in first row, links index above i to i
		{
			n->adj.push_back(i-c);
		}
		if(i % c != 0) //if i is not in first column, links index behind i to i
		{
			n->adj.push_back(i-1);
		}
		g.nodes.push_back(n);
	}
	cin >> x;
	while(x == "WALL" && !(cin.fail())) //processes all WALLs
	{
		cin >> first >> second;
		/*finds and deletes second in first's adjacency list, and vice versa*/
		lit = find(g.nodes[first]->adj.begin(), g.nodes[first]->adj.end(), second);
		if(lit != g.nodes[first]->adj.end())
		{
			g.nodes[first]->adj.erase(lit);
		}
		lit = find(g.nodes[second]->adj.begin(), g.nodes[second]->adj.end(), first);
		if(lit != g.nodes[second]->adj.end())
		{
			g.nodes[second]->adj.erase(lit);
		}
		cout << "WALL " << first << " " << second << endl;
		cin >> x;
	}
	if(g.DFS(0)) //if a path is found, prints out all steps in path
	{
		for(i = (int) g.paths.size() - 1; i >= 0; i--) //iterates backwards since paths' values were inserted in reverse
		{
			cout << "PATH " << g.paths[i] << endl;
		}
	}
	/*deletes memory to prevent leaks*/
	for(i = 0; i < (int) g.nodes.size(); i++)
	{
		delete g.nodes[i];
	}
	return 0;
}
