//Alexander Yu - CS202
//October 14, 2021
//Lab 7: Servers

/*This lab focuses on the implementation of a server, albeit somewhat primitive, as it uses files and input rather than networking. It utilizes maps and sets to manage information, like users and prizes. Pointers and iterators are also used heavily in this lab.*/


#include "code_processor.hpp"
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cstdlib>
using namespace std;

unsigned int djb_hash(const string &s) //hash function to be used for codes
{
  size_t i;
  unsigned int h;

  h = 5381;

  for (i = 0; i < s.size(); i++) {
    h = (h << 5) + h + s[i];
  }
  return h;
}

bool Code_Processor::New_Prize(const string &id, const string &description, int points, int quantity) //adds a prize into the Prizes map
{
	Prize *p; //creates a pointer for a Prize object and allocates memory
	p = new Prize;
	map<string, Prize *>::iterator pit; //iterator for Prizes
	pit = Prizes.find(id); //checks if prize already exists in map, as well as if parameters are correct
	if(pit != Prizes.end() || points <= 0 || quantity <= 0)
	{
		delete p;
		return false;
	}
	p->id = id; //sets Prize object's attributes
	p->description = description;
	p->points = points;
	p->quantity = quantity;
	Prizes.insert(make_pair(id, p)); //inserts prize into map, using the id as the key for the pointer
	return true;
}

bool Code_Processor::New_User(const string &username, const string &realname, int starting_points) //adds a user into the Names map
{
	User *u; //creates a pointer for a User object and allocates memory
	u = new User;
	map<string, User *>::iterator uit; //iterator for Names
	uit = Names.find(username); //checks if user already exists in map, as well as if parameters are correct
	if(uit != Names.end() || starting_points < 0)
	{
		delete u;
		return false;
	}
	u->username = username; //sets attributes of User object
	u->realname = realname;
	u->points = starting_points;
	Names.insert(make_pair(username, u)); //inserts user into map, using username as key for pointer
	return true;
}

bool Code_Processor::Delete_User(const string &username) //removes a user from the Names map
{
	map<string, User *>::iterator uit; //iterator for Users
	map<string, User *>::iterator phit; //iterator for Phones
	set<string>::iterator phnit; //iterator for username's phone_numbers set
	uit = Names.find(username); //checks if username exists in map
	if(uit == Names.end())
	{
		return false;
	}
	for(phnit = uit->second->phone_numbers.begin(); phnit != uit->second->phone_numbers.end(); phnit++) //erases all phone numbers associated with User from Phones map
	{
		phit = Phones.find(*phnit);
		Phones.erase(phit);
	}
	delete uit->second; //deletes allocated memory for User and erases the value from the map
	Names.erase(uit);
	return true;
}

bool Code_Processor::Add_Phone(const string &username, const string &phone) //adds a phone number into the Phones map, as well as adding it to the associated user
{
	map<string, User *>::iterator uit; //iterator for Names
	map<string, User *>::iterator phit; //iterator for Phones
	uit = Names.find(username); //checks if name exists in Names and phone does NOT exist in Phones
	phit = Phones.find(phone);
	if(uit == Names.end() || phit != Phones.end())
	{
		return false;
	}
	uit->second->phone_numbers.insert(phone); //adds the phone number to the User with username's phone_numbers set
	Phones.insert(make_pair(phone, uit->second)); //adds the phone number into Phones map, using phone as a key for the User with username's pointer
	return true;
}

bool Code_Processor::Remove_Phone(const string &username, const string &phone) //removes a phone number from the Phones map
{
	map<string, User *>::iterator uit; //iterator for Names
	map<string, User *>::iterator phit; //iterator for Phones
	uit = Names.find(username);
	phit = Phones.find(phone);
	if(uit == Names.end() || phit == Phones.end() || phit->second != uit->second) //checks if name and phone exists in their respective maps, as well as checking that phone is associated with username
	{
		return false;
	}
	Phones.erase(phit); //removes phone number from both Phones map and the phone_numbers set associated with username
	uit->second->phone_numbers.erase(phone);
	return true;
}

string Code_Processor::Show_Phones(const string &username) const //returns all phone numbers associated with username
{
	map<string, User *>::const_iterator uit; //iterator for Names
	set<string>::const_iterator phit; //iterator for Phones
	string x; //holds phone numbers
	uit = Names.find(username); //checks if user exists in Names
	if(uit == Names.end())
	{
		return "BAD USER";
	}
	if(uit->second->phone_numbers.empty()) //if username has no associated phone numbers, returns an empty string
	{
		return "";
	}
    for(phit = uit->second->phone_numbers.begin(); phit != uit->second->phone_numbers.end(); phit++) //adds each phone number of the set into x
	{
		x += *phit + "\n";
	}
	return x;
}

int Code_Processor::Enter_Code(const string &username, const string &code) //enters and redeems a code for username
{
	set<string>::iterator cit; //iterator for Codes
	map<string, User *>::iterator uit; //iterator for Names
	unsigned int hash; //code's hash
	cit = Codes.find(code); //checks if code does NOT exist in Codes and username does exist in Names
	uit = Names.find(username);
	if(cit != Codes.end() || uit == Names.end())
	{
		return -1;
	}
	hash = djb_hash(code); //finds hash of code using djb
	if(hash % 17 == 0) //if hash is divisible by 17, 10 points are added to username's account and code is inserted into set
	{
		uit->second->points += 10;
		Codes.insert(code);
		return 10;
	}
	if(hash % 13 == 0) //if hash is divisible by 13, 3 points are added instead
	{
		uit->second->points += 3;
		Codes.insert(code);
		return 3;
	}
	return 0; //if hash isn't divisible by 17 or 13, considered invalid; no points are given, no code is inserted
}

int Code_Processor::Text_Code(const string &phone, const string &code) //enters and redeems a code for a phone number
{
	set<string>::iterator cit; //iterator for Codes
	map<string, User *>::iterator phit; //iterator for Phones
	unsigned int hash; //code's hash
	cit = Codes.find(code); //checks if code does NOT exist in Codes and phone does exist in Phones
	phit = Phones.find(phone);
	if(cit != Codes.end() || phit == Phones.end())
	{
		return -1;
	}
	hash = djb_hash(code);
	if(hash % 17 == 0) //if hash is divisible by 17, 10 points are added to the User associated with phone and code is inserted into set
	{
		phit->second->points += 10;
		Codes.insert(code);
		return 10;
	}
	else if(hash % 13 == 0) //if hash is divisible by 13, 3 points are added instead
	{
		phit->second->points += 3;
		Codes.insert(code);
		return 3;
	}
	return 0; //code is invalid otherwise
}

bool Code_Processor::Mark_Code_Used(const string &code) //inserts code into Codes, thus signifying that they have been used
{
	set<string>::iterator cit; //iterator for Codes
	unsigned int c; //holds hash
	c = djb_hash(code); 
	cit = Codes.find(code); //checks if code does NOT exist in Codes and that code is valid
	if(cit != Codes.end() || (c % 17 != 0 && c % 13 != 0))
	{
		return false;
	}
	Codes.insert(code); //code is inserted into set
	return true;
}

int Code_Processor::Balance(const string &username) const //returns the balance of username
{
	map<string, User *>::const_iterator uit; //iterator for Names
	uit = Names.find(username); //checks if username exists in Names
	if(uit == Names.end())
	{
		return -1;
	}
	return uit->second->points; //returns the points variable associated with username
}

bool Code_Processor::Redeem_Prize(const string &username, const string &prize) //redeems a prize for username
{
	map<string, User *>::iterator uit; //iterator for Names
	map<string, Prize *>::iterator pit; //iterator for Prizes
	uit = Names.find(username); //checks if username and prize exist in their respective maps, as well as check if username has enough points to redeem prize
	pit = Prizes.find(prize);
	if(uit == Names.end() || pit == Prizes.end() || pit->second->points > uit->second->points)
	{
		return false;
	}
	uit->second->points -= pit->second->points; //subtracts the prize's price from username's points
	pit->second->quantity--; //decrements one from prize's quantity
	if(pit->second->quantity == 0) //if quantity is zero, removes prize from the map and delete allocated memory associated with its pointer
	{
		delete pit->second;
		Prizes.erase(pit);
	}
	return true;
}

Code_Processor::~Code_Processor() //deletes any allocated memory to ensure no memory leaks after program termination
{
	map<string, User *>::iterator uit; //iterator for Names
	map<string, Prize *>::iterator pit; //iterator for Prizes
	for(uit = Names.begin(); uit != Names.end(); uit++) //deletes every pointer from the Names map
	{
		delete uit->second;
	}
	for(pit = Prizes.begin(); pit != Prizes.end(); pit++) //delets every pointer from the Prizes map
	{
		delete pit->second;
	}
}

bool Code_Processor::Write(const string &filename) const //writes a "save state" into a file
{
	size_t i; //used to manage MARK_USED
	ofstream fout; //filestream
	map <string, Prize *>::const_iterator pit; //iterator for Prizes
	map <string, User *>::const_iterator uit; //iterator for Names, Phones
	set <string>::const_iterator sit; //iterator for phone_numbers, Codes
	fout.open(filename.c_str());
	if(fout.fail()) //checks if file was valid
	{
		return false;
	}
	for(pit = Prizes.begin(); pit != Prizes.end(); pit++) //writes each prize to file, as well as its id, price, amount and description
	{
		fout << "PRIZE " << pit->first << " " << pit->second->points << " " << pit->second->quantity << " " << pit->second->description << "\n";
	}
	for(uit = Names.begin(); uit != Names.end(); uit++) //writes each user to file, as well as username, balance and realname
	{
		fout << "ADD_USER " << uit->first << " " << uit->second->points << " " << uit->second->realname << "\n";
	}
	for(uit = Names.begin(); uit != Names.end(); uit++) //writes each phone_number to file, as well as associated username
	{
		for(sit = uit->second->phone_numbers.begin(); sit != uit->second->phone_numbers.end(); sit++)
		{
			fout << "ADD_PHONE " << uit->first << " " << *sit << "\n";
		}
	}
	i = 19; //starts at 19 to prevent unintended newlines
	for(sit = Codes.begin(); sit != Codes.end(); sit++) //writes each code to file; nineteen to a line, so i is used to keep track of how many codes have been written
	{
		if(i % 19 == 0)
		{
			fout << "\nMARK_USED ";
		}
		fout << *sit << " ";
		i++;
	}
	fout << "\n";
	fout.close();
	return true;
}

