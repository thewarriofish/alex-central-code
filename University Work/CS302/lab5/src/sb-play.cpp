//Lab 5B: Superball Challenge - Player
//Alexander Yu - CS302
//March 4th, 2022

/*This portion of the lab expands upon sb-analyze, and is more or less a self-guided lab in which one has to decide the best strategy for Superball, and how to implement it. 
 * For my implmentation, I used maps and recursion to achieve my goals.*/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "disjoint_set.hpp"
using plank::Disjoint_Set;
using namespace std;

#define talloc(type, num) (type *) malloc(sizeof(type)*(num))

class Superball {
  public:
    Superball(int argc, char **argv);
	void analyze_superball(); //modified from sb-analyze's method
	void find_swaps(int index); //recursive method to find all possible swaps via combinations
	int rate_board(); //creates a numerical score from a board
    int r;
    int c;
    int mss;
    int empty;
	Disjoint_Set d; //stores all group of same characters
    vector <int> board;
    vector <int> goals;
    vector <int> colors;
	vector <int> possible_spaces; //stores all spots which contain a letter
	vector <int> combo; //used in find_swaps to store current combination
	vector < vector<int> > possible_swaps; //stores all possible swaps
	multimap <int, int> scorable; //a map of goal spaces with a scorable set, keyed by size; val is index
	multimap <int, int> goal_sets; //a map of goal spaces with a non-scorable set, keyed by size; val is index
	multimap <int, int> non_goal_scorable; //a map of spaces not in goal space with a set, keyed by size; val is index
};

void usage(const char *s) //taken from sb-read
{
  fprintf(stderr, "usage: sb-play rows cols min-score-size colors\n");
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

  for (i = 0; i < (int) strlen(argv[4]); i++) {
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
    if ((int) s.size() != c) {
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
void Superball::analyze_superball() //based on sb-analyze's method of the same name; instead of output, fills maps
{
  size_t i;
  const vector <int> *siz; //accesses d's Sizes vector
  vector <int> sizes; //stores used roots 
  d.Initialize(r*c);  
  siz = d.Get_Sizes();
  sizes.resize(board.size(), 0); //resets sizes vector
  scorable.clear(); //clears all maps to prevent extraneous information
  goal_sets.clear();
  non_goal_scorable.clear();
  for(i = 0; i < board.size(); i++) //creates disjoint sets in the same way as sb-analyze
  {
    if((i % c) + 1 != (size_t) c) 
    {
        if(board[i] == board[i+1] && board[i] != '.' && board[i] != '*' && d.Find(i) != d.Find(i+1))
        {
            d.Union(d.Find(i), d.Find(i+1));
        }
    }
    if((int) i < (r * c) - c)
    {
        if(board[i] == board[i+c] && board[i] != '.' && board[i] != '*' && d.Find(i) != d.Find(i+c))
        {
            d.Union(d.Find(i), d.Find(i+c));
        }
    }
  }
  for(i = 0; i < goals.size(); i++) //stores sets located at goal spaces
  {
	  if(goals[i] == 1)
	  {
		  if(siz->at(d.Find(i)) >= mss && sizes[d.Find(i)] == 0 && board[i] != '*') //if the set is scorable, puts it into scorable map
		  {
			 scorable.insert(make_pair(siz->at(d.Find(i)), (int) i));
			 sizes[d.Find(i)] = siz->at(d.Find(i));
		  }
		  else if(siz->at(d.Find(i)) > 1 && sizes[d.Find(i)] == 0 && board[i] != '*') //otherwise it is placed into goal_sets; root of set is placed into sizes
		  {
			 goal_sets.insert(make_pair(siz->at(d.Find(i)), (int) i));
			 sizes[d.Find(i)] = siz->at(d.Find(i));
		  }
	  }
  }
  for(i = 0; i < board.size(); i++) //inserts all non-goal sets larger than 2 into non_goal_scorable
  {
	  if(goals[i] != 1 && siz->at(d.Find(i)) > 2 && sizes[d.Find(i)] == 0 && board[i] != '.')
	  {
		   non_goal_scorable.insert(make_pair(siz->at(d.Find(i)), (int) i));
	  	   sizes[d.Find(i)] = siz->at(d.Find(i));
	  }
  }
}

void Superball::find_swaps(int index) //simple recursive combination method
{
	if(combo.size() == 2) //base case 1: combo is filled
	{
		possible_swaps.push_back(combo);
		return;
	}
	if(possible_spaces.size() - index == 0) //base case 2: there are no more possible spaces
	{
		return;
	}
	combo.push_back(possible_spaces[index]); //recursive combination
	find_swaps(index+1);
	combo.pop_back();
	find_swaps(index+1);
}

int Superball::rate_board() //rates a board based on type of sets
{	
  multimap<int,int>::iterator sit;
  int score, i, j, prev;
  score = 0;
  for(sit = scorable.begin(); sit != scorable.end(); sit++) //scorable sets are scored via their size * 10
  {
	  score += 10*sit->first;
  }
  for(sit = goal_sets.begin(); sit != goal_sets.end(); sit++) //sets in goal spaces but aren't scorable have their size multiplied by 5
  {
	  score += 5*sit->first;
  }
  for(sit = non_goal_scorable.begin(); sit != non_goal_scorable.end(); sit++) //other sets simply add their size
  {
	  score += sit->first;
  }
  score -= goal_sets.size() + non_goal_scorable.size(); //subtracts the amount of non-scorable sets
  for(i = 0; i < (int) board.size(); i++) //checks rows and columns for consecutive characters, meant for early boards with less characters
  {
	if(board[i] != '.' && board[i] != '*')
	{
		if(board[i] == prev)
		{
			score++;
		}
		prev = board[i];
	}
	if(i % r == 0)
	{
		prev = '.';
	}
  }
  prev = '.'; //each loop adds 1 to score if there are characters that are consecutive if empty spaces are not considered
  for(i = 0; i < c; i++)
  {
	for(j = 0; j < r; j++)
	{
		if(board[i+j*r] != '.' && board[i+j*r] != '*')
		{
			 if(board[i+j*r] == prev)
			{
				  score++;
			}
			prev = board[i+j*r];
		}
	}
	prev = '.';
  }
  return score;
}

int main(int argc, char **argv) //main method
{
  Superball *s;
  int temp, current_score, swapped_score; 
  size_t i;
  vector <int> swapped;
  s = new Superball(argc, argv);
  s->analyze_superball();
  if(!s->scorable.empty() && (s->empty <= 5 || s->scorable.rbegin()->first >= s->mss + 5)) //to choose SCORE, scorable must be empty, and there are either less than 6 empty spaces left or the largest scorable set is 5 greater than min-score-size
  {
	  int index;
	  index = s->scorable.rbegin()->second;
	  cout << "SCORE " << (index - (index % s->c)) / s->c << " " << index % s->c << endl;
	  return 0;
  }
  for(i = 0; i < s->board.size(); i++) //finds all possible spaces on the board
  {
	  if(s->board[i] != '.' && s->board[i] != '*')
	  {
		  s->possible_spaces.push_back(i);
	  }
  }
  s->find_swaps(0); //find all possible swaps
  current_score = 0;
  for(i = 0; i < s->possible_swaps.size(); i++) //checks every swap
  {
	  if(s->board[s->possible_swaps[i].at(0)] != s->board[s->possible_swaps[i].at(1)]) //only checks a swap if swap is occurring with two different characters
	  {
		  temp = s->board[s->possible_swaps[i].at(0)]; //uses temporary varible to swap
		  s->board[s->possible_swaps[i].at(0)] = s->board[s->possible_swaps[i].at(1)];
		  s->board[s->possible_swaps[i].at(1)] = temp;
		  s->analyze_superball();
		  swapped_score = s->rate_board();
		  if(swapped_score > current_score) //if the score of the swapped board is larger than the current largest score, replaces current score with swapped score
		  {
			  current_score = swapped_score;
			  swapped = s->possible_swaps[i];
		  }
		  temp = s->board[s->possible_swaps[i].at(0)]; //swaps values back for next loop
		  s->board[s->possible_swaps[i].at(0)] = s->board[s->possible_swaps[i].at(1)];
		  s->board[s->possible_swaps[i].at(1)] = temp;
	  }	
  }
  if(swapped.size() == 0) //if all possible swaps resulted in the same board score, chooses a random swap
  {
	  swapped = s->possible_swaps[rand() % s->possible_swaps.size()];
  }
  cout << "SWAP " << (swapped[0] - (swapped[0] % s->c)) / s->c << " " << swapped[0] % s->c << " " << (swapped[1] - (swapped[1] % s->c)) / s->c << " " << swapped[1] % s->c << endl;
  return 0;
}

