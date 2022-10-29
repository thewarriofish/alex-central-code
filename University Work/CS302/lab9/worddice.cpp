//Alexander Yu - CS302
//Lab 9: Network Flow
//April 10th, 2022

/*This lab focuses on the implementation of a network flow solution using word dice as an example.
 * It uses the Edmonds-Karp algorithm to find augmenting paths, an algorithm which uses breath-first search extensively.
 * As such BFS is also implemented in this lab.*/

#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <string>
using namespace std;

class Node //class definition for a single node
{
	public:
		string name;
		int index;
		class Edge *backedge = NULL;
		vector <class Edge *> adj;
		vector <class Edge *> backlinks; //stores edges that link backward; relevant for letter and sink nodes
		bool visited;
		int ascii[256]; //sets a 1 if a character is in dice/letter
};

class Edge //class defintion for a single edge
{
	public:
		int original; //indicates flow; for Edmonds-Karp, will always be 1
		int residue;
		Edge *reverse = NULL; //an edge's reverse has original/residue and to/from swapped
		Node *to;
		Node *from;

};

int main(int argc, char** argv)
{
	string x;
	int dice, j, a;
	size_t i;
	Node *n, *sink;
	Edge *e, *r, *current;
	vector <Node *> nodes;
	queue <Node *> bfs;
	if(argc != 3)
	{
		cout << "usage: ./worddice dice-file word-list" << endl;
		return false;
	}
	ifstream fin;
	n = new Node; //initializes the SOURCE node
	n->name = "SOURCE";
	n->visited = true;
	n->index = 0;
	nodes.push_back(n);
	dice = 0;
	fin.open(argv[1]);
	if(fin.is_open()) //initializes all nodes for dice
	{
		while(getline(fin, x))
		{
			n = new Node;
			n->name = x;
			n->index = (int) nodes.size();
			n->visited = false;
			for(i = 0; i < x.size(); i++) //sets node's ascii chart to correct values
			{
				a = x[i];
				n->ascii[a] = 1;
			}
			e = new Edge; //links from source to dice
			e->original = 1;
			e->residue = 0;
			e->from = nodes[0];
			e->to = n;
			nodes[0]->adj.push_back(e);
			nodes.push_back(n);
			dice++; //keeps track of how many dice are being used
		}
	}
	fin.close();
	sink = new Node; //initializes SINK node
	sink->name = "SINK";
	sink->visited = false;
	fin.open(argv[2]);
	if(fin.is_open())
	{
		while(getline(fin, x)) //determines if word can be spelled with dice
		{
			for(i = 0; i < x.size(); i++) //initializes nodes for each letter
			{
				n = new Node; 
				n->name = x[i];
				n->index = (int) nodes.size();
				for(j = 1; j <= dice; j++)
				{
					a = x[i];
					if(nodes[j]->ascii[a] == 1) //connects letter to dice if letter's value in dice's ascii chart is set
					{
						e = new Edge; //links from dice to letter
						e->original = 1;
						e->residue = 0;
						e->from = nodes[j];
						e->to = n;
						nodes[j]->adj.push_back(e);
						r = new Edge; //links from letter to dice
						r->original = 0;
						r->residue = 1;
						r->from = n;
						r->to = nodes[j];
						n->adj.push_back(r);
						e->reverse = r;
						r->reverse = e;
						n->backlinks.push_back(e);
					}
				}
				n->visited = false;
				e = new Edge; //links from letter to sink
				e->original = 1;
				e->residue = 0;
				e->from = n;
				e->to = sink;
				n->adj.push_back(e);
				nodes.push_back(n);
			}
			sink->index = (int) nodes.size(); //sets index and pushes sink into nodes vector
			nodes.push_back(sink);

			for(i = 0; i < nodes[0]->adj.size(); i++) //calls BFS for each dice
			{
				nodes[0]->visited = true;
				bfs.push(nodes[0]);
				while(!bfs.empty())
				{
					for(j = 0; j < (int) bfs.front()->adj.size(); j++)
					{
						if(bfs.front()->adj[j]->original == 1 && bfs.front()->adj[j]->to->visited == false) //proceeds through path only if edge still has flow available
						{
							bfs.front()->adj[j]->to->visited = true;
							bfs.front()->adj[j]->to->backedge = bfs.front()->adj[j];
							bfs.push(bfs.front()->adj[j]->to);
						}
					}
					bfs.pop();
					if(sink->backedge != NULL) //if sink's backedge isn't NULL, a path is found
					{
						sink->backlinks.push_back(sink->backedge); //keeps track of how many letters have connected to sink
						current = sink->backedge;
						while(current != NULL) //swaps original and residue values for each edge in the path
						{
							current->residue++;
							current->original--;
							if(current->reverse != NULL) //only edges between dice and letters have reverse versions
							{
								current->reverse->residue--;
								current->reverse->original++;
							}
								current = current->from->backedge;
						}
						break; //ends BFS
					}
				}
				while(!bfs.empty()) //resets queue for next BFS run
				{
					bfs.pop();
				}
				for(j = 0; j < (int) nodes.size(); j++) //resets edges and visited values for next BFS run
				{
					nodes[j]->backedge = NULL;
					nodes[j]->visited = false;
				}
			}
			if(sink->backlinks.size() != x.size() || (int) x.size() > dice) //a word cannot be spelled if there isn't a letter connected to the sink or there aren't enough dice
			{
				cout << "Cannot spell " << x << endl;
			}
			else
			{
				for(i = dice + 1; i < nodes.size() - 1; i++)
				{
					for(j = 0; j < (int) nodes[i]->backlinks.size(); j++)
					{
						if(nodes[i]->backlinks[j]->original == 0) //only prints if flow is being used
						{
							cout << nodes[i]->backlinks[j]->from->index - 1; //prints index
							if(i == nodes.size() - 2)
							{
								cout << ": ";
							}
							else
							{
								cout << ",";
							}
						}
					}
				}
				cout << x << endl; //prints word
			}
			for(i = 1; (int) i <= dice; i++) //removes edges connecting to dice nodes
			{
				nodes[i]->adj.clear();
			}
			for(i = 0; i < nodes[0]->adj.size(); i++) //resets flow values for edges connecting to source
			{
				nodes[0]->adj[i]->original = 1;
				nodes[0]->adj[i]->residue = 0;
			}
			for(i = nodes.size() - 1; (int) i > dice; i--) //removes letter nodes
			{
				nodes.pop_back();
			}
			sink->backlinks.clear(); //resets sink node
			sink->adj.clear();
		}
	}
	for(i = 0; i < nodes.size(); i++) //deletes allocated memory
	{
		for(j = 0; j < (int) nodes[i]->adj.size(); j++)
		{
			delete nodes[i]->adj[j];
		}
		delete nodes[i];
	}
	return 0;
}
