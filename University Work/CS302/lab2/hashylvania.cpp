//Alexander YU - CS302
//February 3rd, 2022
//Lab 2: Hashylvania

//This lab is a combination of review and new material. It focuses on maps and hash tables, as well as the proper usage of lower_bound and sprintf.

#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
using namespace std;

struct Person //simple struct for creating Person objects
{
	string name;
	string id;
};

unsigned long long jsp_hash(const string &s) //hash function taken from lab writeup
{
  size_t i;
  unsigned long long h, t;

  h = 0x6bd300f29f1505ULL;

  for (i = 0; i < s.size(); i++) {
    t = h & 0x3ffffffffffffffULL;
    h = (t << 5) ^ h ^ s[i];
  }
  return h;
}

int main()
{
	string cmd, arg1, arg2, arg3, concat; //strings that hold commmands or values to be processed
	size_t i; //loop variable
	stringstream ss; //string stream for reading standard in
	char hex[500]; //buffer to hold hex conversion
	unsigned long long mod, hash, h; //holds number values
	Person *first, *last; 
	vector < Person * > population; //holds all people read from standard in
	map < unsigned long long, map < unsigned long long, Person * > > ordered; //holds ordered population
	map < unsigned long long, Person * > group; //temporary map for creating a new group of people with different key
	map < unsigned long long, map < unsigned long long, Person * > >::iterator oit, toit; //iterators for ordered map; one is a temp. variable
	map < unsigned long long, Person * >::iterator git; //iterator for group
	getline(cin, cmd); //reads standard in and processes commands
	ss.str(cmd);
	ss >> arg1 >> arg2 >> arg3;
	while(arg1 != "QUIT")
	{
		if(arg1 == "PERSON") //inserts a new Person object using parameters as name and id
		{
			Person *p;
			p = new Person;
			p->name = arg2;
			p->id = arg3;
			population.push_back(p);
		}
		else if(arg1 == "ORDER") //creates ordered map
		{
			if(!population.empty()) //does nothing if no people have been entered
			{
				sscanf(arg3.c_str(), "%llu", &mod); //converts 3rd argument of std. in into unsigned long long
				ordered.clear();
				h = jsp_hash(arg2);
				for(i = 0; i < population.size(); i++) //iterates throught population vector
				{
					concat = population[i]->name + population[i]->id + arg2; //hashes for each person is created with the combination of their name, id and string from std. in
					hash = jsp_hash(concat);
					oit = ordered.find(hash % mod);
					if(oit == ordered.end()) //if key with (hash % mod) isn't found, creates a map using said key
					{
						group.clear();
						group.insert(make_pair(hash, population[i])); //current person is first of the group
						ordered.insert(make_pair((hash % mod), group));
					}
					else //inserts person into the correct group
					{
						oit->second.insert(make_pair(hash, population[i]));
					}
				}
				group.clear();
				//algorithm to order population 
				oit = ordered.lower_bound(h % mod); //uses lower_bound to find closest key to (h % mod)
				if(oit != ordered.end() && oit->first != (h % mod)) //if a key is found but is not (h % mod), checks values less than (h % mod)
				{
					toit = oit; //temporary iterator is set to element before oit
					toit--;
					if(ordered.find(toit->first) != ordered.end()) //finds difference of each key with (h%mod); smallest one becomes oit
					{
						if(oit->first - (h % mod) > (h % mod) - toit->first)
						{
							oit = toit;
						}
					}	
				}
				else if(oit == ordered.end()) //if no key was found, sets oit to last element
				{
					oit--;
				}
				git = oit->second.lower_bound(h); //finds closest key to h in a group that is greater or equal to h
				if(git == oit->second.end()) //if no key was found, sets git to last element in group
				{
					git--;
				}
				first = git->second; //sets Person object first to git for later use
				last = git->second;
				sprintf(hex, "%llx", h); //converts h to hex
				arg2 = hex;
				concat = first->id + arg2; //creates new hash for h using combo of first's id and the hex conversion
				h = jsp_hash(concat);
				oit->second.erase(git); //removes git from group
				if(oit->second.empty()) //if group is empty, removes it from ordered
				{
					ordered.erase(oit);
				}
				while(!ordered.empty()) //repeats algorithm until population is empty 
				{
					oit = ordered.lower_bound(h % mod);
					if(oit != ordered.end() && oit->first != (h % mod))
					{
						toit = oit;
						toit--;
						if(ordered.find(toit->first) != ordered.end())
						{
							if(oit->first - (h % mod) > (h % mod) - toit->first)
							{
								oit = toit;
							}
						}
					}
					else if(oit == ordered.end())
					{
						oit--;
					}
					git = oit->second.lower_bound(h);
					if(git == oit->second.end())
					{
						git--;
					}
					last = git->second;
					sprintf(hex, "%llx", h);
					arg2 = hex;
					concat = last->id + arg2;  
					h = jsp_hash(concat);
					oit->second.erase(git);
					if(oit->second.empty())
					{
						ordered.erase(oit);
					}	
				}
				printf("Last: %s %s\n", last->name.c_str(), last->id.c_str()); //prints last in the order and first in the order, as well as names and ids
				printf("First: %s %s\n", first->name.c_str(), first->id.c_str());
			}
		}
		else //exits quietly if invalid command
		{
			break;
		}
		if(ss.eof()) //checks if there are still more commands in line
		{
			ss.clear();
			getline(cin, cmd);
			if(cin.eof()) //exits quietly if no more commands are found
			{
				break;
			}
			ss.str(cmd);
			ss >> arg1 >> arg2 >> arg3;
		}
		else
		{
			ss >> arg1 >> arg2 >> arg3;
		}
	}
	for(i = 0; i < population.size(); i++) //deletes pointers to free memory
	{
		delete population[i];
	}
	population.clear();
	return 0;

}
