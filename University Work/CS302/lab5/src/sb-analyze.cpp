//Lab 5A: Superball Challenge - Analyze
//Alexander Yu - CS302
//March 1st, 2022

/*This portion of the lab focuses on the use of disjoint sets to implement a Superball board analysis. It takes a Superball board in text format and finds all possible scoring location on the board, using disjoint sets*/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <vector>
#include <algorithm> //used for find
#include "disjoint_set.hpp"
using plank::Disjoint_Set;
using namespace std;

#define talloc(type, num) (type *) malloc(sizeof(type)*(num))

class Superball { //class taken from sb-read
  public:
    Superball(int argc, char **argv);
	void analyze_superball(); //augmented from sb-read
    int r;
    int c;
    int mss;
    int empty;
    vector <int> board;
    vector <int> goals;
    vector <int> colors;
	Disjoint_Set d; //augmented from sb-read
};

void usage(const char *s) //taken from sb-read
{
  fprintf(stderr, "usage: sb-analyze rows cols min-score-size colors\n");
  if (s != NULL) fprintf(stderr, "%s\n", s);
  exit(1);
}

Superball::Superball(int argc, char **argv) //taken from sb-read
{
  int i, j;
  string s;

  if (argc != 5) usage(NULL);

  if (sscanf(argv[1], "%d", &r) == 0 || r <= 0) usage("Bad rows");
  if (sscanf(argv[2], "%d", &c) == 0 || c <= 0) usage("Bad cols");
  if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0) usage("Bad min-score-size");

  colors.resize(256, 0);

  for (i = 0; (size_t) i < strlen(argv[4]); i++) {
    if (!isalpha(argv[4][i])) usage("Colors must be distinct letters");
    if (!islower(argv[4][i])) usage("Colors must be lowercase letters");
    if (colors[argv[4][i]] != 0) usage("Duplicate color");
    colors[argv[4][i]] = 2+i;
    colors[toupper(argv[4][i])] = 2+i;
  }

  board.resize(r*c);
  goals.resize(r*c, 0);

  empty = 0;

  for (i = 0; i < r; i++) {
    if (!(cin >> s)) {
      fprintf(stderr, "Bad board: not enough rows on standard input\n");
      exit(1);
    }
    if (s.size() != (size_t) c) {
      fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
      exit(1);
    }
    for (j = 0; j < c; j++) {
      if (s[j] != '*' && s[j] != '.' && colors[s[j]] == 0) {
        fprintf(stderr, "Bad board row %d - bad character %c.\n", i, s[j]);
        exit(1);
      }
      board[i*c+j] = s[j];
      if (board[i*c+j] == '.') empty++;
      if (board[i*c+j] == '*') empty++;
      if (isupper(board[i*c+j]) || board[i*c+j] == '*') {
        goals[i*c+j] = 1;
        board[i*c+j] = tolower(board[i*c+j]);
      }
    }
  }
}

void Superball::analyze_superball() //method used to analyze board
{
  size_t i;
  const vector <int> *siz; //grants access to protected Sizes vector of Superball
  vector <int> roots; //stores used roots
  d.Initialize(r*c); //initializes disjoint set
  siz = d.Get_Sizes();
  for(i = 0; i < board.size(); i++) //traverses through the board
  {
	if((i % c) + 1 != (size_t) c) //if i isn't currently at the last column, checks the entry after i if they have the same non-'.' or '*' character
	{
		if(board[i] == board[i+1] && board[i] != '.' && board[i] != '*' && d.Find(i) != d.Find(i+1)) //also ensures i and i+1 don't have the same root
		{
			d.Union(d.Find(i), d.Find(i+1)); //calls union on both entries if both are the same
		}
	}
	if((int) i < (r * c) - c) //if i isn't currently at the last row, checks the entry under i if they have the same non-'.' or '*' character
	{
		if(board[i] == board[i+c] && board[i] != '.' && board[i] != '*' && d.Find(i) != d.Find(i+c))
		{
			d.Union(d.Find(i), d.Find(i+c));
		}
	}
  }
  printf("Scoring sets:\n"); //prints out each disjoint set which is scorable
  for(i = 0; i < goals.size(); i++)
  {
	if(goals[i] == 1) //finds the goal spaces
	{
		if(siz->at(d.Find(i)) >= mss && find(roots.begin(), roots.end(), d.Find(i)) == roots.end() && board[i] != '*') //prints set if the size of the set is greater than min-score-size, set's root isn't in roots vector, and board at i is not '*'
		{		
			printf("  Size:%3d  Char: %c  Scoring Cell: %d,%d\n", siz->at(d.Find(i)), board[i], (int) (i-(i%c))/c, (int) i%c); 
			roots.push_back(d.Find(i)); //inserts root into vector to denote usage
		}
	}
  }
}

int main(int argc, char **argv) //main method
{
  Superball *s;
  s = new Superball(argc, argv);
  s->analyze_superball();
  exit(0);
}

