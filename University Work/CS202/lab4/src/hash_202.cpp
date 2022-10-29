//Alexander Yu - CS202
//September 18, 2021
//Lab 4: Hash Tables and Open Addressing

/*This lab focuses on the implementation of a hash table, hash functions Last7 and XOR, and collision resolution strategies linear probing and double hashing*/

#include <hash_202.hpp>
#include <string>
#include <iostream>
#include <cctype>
#include <vector>
#include <sstream>
#include <cstdio>
using namespace std;

int Last7(string key); //prototypes to allow usage of functions
int XOR(string key);

string Hash_202::Set_Up(size_t table_size, const string &fxn, const string &collision) //sets up a hash table using given parameters
{
	string s; //variable to hold error message
	Nkeys = 0; //sets inserted keys to zero
	Nprobes = 0; //sets number of probes to zero
	if(Keys.size() != 0) //checks if hash table has been set up already
	{
		s = "Hash table already set up";
		return s;
	}
	if(table_size <= 0) //checks table_size for negative or 0
	{
		s = "Bad table size";
		return s;
	}
	if(fxn != "Last7" && fxn != "XOR") //checks for valid hash function
	{
		s = "Bad hash function";
		return s;
	}
	if(collision != "Linear" && collision != "Double") //checks for valid collision strategy
	{
		s = "Bad collision resolution strategy";
		return s;
	}
	Keys.resize(table_size, ""); //resizes Keys vector and set each value to null, to signify that a part of the table is empty
	Vals.resize(table_size, ""); //ditto, for Values vector
	if(fxn == "Last7") //sets hash function to correct mode; L for Last7, X for XOR
	{
		Fxn = 'L';
	}
	else if(fxn == "XOR")
	{
		Fxn = 'X';
	}
	if(collision ==  "Linear") //sets collision strategy to correct mode; L for linear, D for double
	{
		Coll = 'L';
	}
	else if(collision == "Double")
	{
		Coll = 'D';
	}
	return ""; //returns null if setup was successful
}

string Hash_202::Add(const string &key, const string &val)
{
	string s; //variable to hold error
	int hash, index, doubleHash, start, temp; //initializing int variables 
	size_t i; //initializing i for loops
	if(Keys.size() == 0) //checks if hash table is set up
	{
		s = "Hash table not set up";
		return s;
	}
	if(key.size() == 0) //checks &key for non-empty input
	{
		s = "Empty key";
		return s;
	}
	if(Fxn == 'L') //sets up the hash depending on function
	{
		hash = Last7(key);
	}
	else if(Fxn == 'X')
	{	
		hash = XOR(key);
	}
	for(i = 0; i < key.size(); i++)	//checks each char of key if it's a valid hex digit
	{
		if(!(isxdigit(key[i])))
		{
			s = "Bad key (not all hex digits)";
			return s;
		}	
	}
	if(val.size() == 0) //checks &val for non-empty input
	{
		s = "Empty val";
		return s;
	}
	if(Nkeys == Keys.size()) //checks if hash table is full by comparing the number of keys inserted to the size of the hash table
	{
		s = "Hash table full";
		return s;
	}
	if(Coll == 'D') //if collision strategy is double, checks if key can be inserted into table
	{
		start = hash % Keys.size(); //finds starting location
		temp = hash; //retains original hash
		if(Fxn == 'L') //sets doubleHash using other hash function
		{
			doubleHash = XOR(key) % Keys.size();
		}
		else if(Fxn == 'X')
		{
			doubleHash = Last7(key) % Keys.size();
		}
		if(doubleHash == 0) //sets doubleHash to 1 if it was zero using a hash function
		{
			doubleHash = 1;
		}
		index = start; //sets index to starting position; starting position for double hashing
		while(1)
		{
			if(Keys[index] == "" && Vals[index] == "") //checks if possible double hash location is empty; breaks if true
			{
				break;
			}
			hash += doubleHash; //if location was full, adds doubleHash into hash, then modulos with hash table size
			index = hash % Keys.size();
			if(index == start) 
				//if true, the loop has circled back to the starting position, and therefore all possible locations are full
			{
				s = "Cannot insert key";
				return s;
			}
		}
		hash = temp;
	}
	for(i = 0; i < Keys.size(); i++) //checks if the key is already present in hash table
	{
		if(Keys[i] == key)
		{
			s = "Key already in the table";
			return s;
		}
	}
	index = hash % Keys.size(); //sets starting position for hashing
	while(1) //hashing process
	{
		if(Vals[index] == "" && Keys[index] == "") //if position is empty, fills position with key and value, increment Nkeys inserted, ends the function
		{
			Vals[index] = val;
			Keys[index] = key;
			Nkeys++;
			return "";
		}
		else if(Coll == 'L') //linear probing; increments index, then modulo to ensure it doesn't go over hash table size
		{
			index++;
			index %= Keys.size();
		}
		else if(Coll == 'D') //double hashing; adds double hash value into hash, then finds index
		{
			hash += doubleHash;
			index = hash % Keys.size();
		}
	}
	return ""; //this return statement should never be processed; only here to prevent errors and warnings
}

string Hash_202::Find(const string &key) //finds key in hash table, and counts how many probes were needed to find the key
{
	int hash, doubleHash, i, probes, index, start; //variables to use
	probes = 0;
	if(Keys.size() == 0) //checks if hash table is set up
	{
		return "";
	}
	for(i = 0; i < (int) key.size(); i++) //checks if &key has valid hex digits	
	{
		if(!(isxdigit(key[i])))
		{
			return "";
		}
	}
	if(Fxn == 'L') //establishes hash depending on function
	{
		hash = Last7(key);
	}
	else if(Fxn == 'X')
	{
		hash = XOR(key);
	}
	start = hash % Keys.size(); //finds starting position
	index = start;
	if(Keys[index] == key) //checks starting position to see if key is present
	{
		return Vals[index];
	}
	else if(Coll == 'L') //linear probing
	{
		index++; //sets index to next possible position
		index %= Keys.size();
		probes++; //Find has done one probe at this point
		while(1)
		{
			if(Keys[index] == key) //checks position to see if key is present
			{
				Nprobes += probes; //adds probes into total number of probes (used by Total_Probes)
				return Vals[index];
			}
			probes++; //increments probes
			index++; //sets index to next possible position
			index %= Keys.size();
			if(index == start)
				/*if true, probing has cycled without finding key, so key is not present, and Find returns nothing*/
			{
				return "";
			}
		}
	}
	else if(Coll == 'D') //double hashing
	{
		probes++; //Find has probed once at this point
		if(Fxn == 'L') //sets double hash to other function
		{
			doubleHash = XOR(key) % Keys.size();
		}
		else if(Fxn == 'X')
		{
			doubleHash = Last7(key) % Keys.size();
		}
		if(doubleHash == 0) //set double hash to 1 if it was 0
		{
			doubleHash = 1;
		}
		hash += doubleHash; //sets next possible index
		index = hash % Keys.size();
		while(1)
		{
			if(Keys[index] == key) //checks position for key
			{
				Nprobes += probes;
				return Vals[index];
			}
			probes++; //increments probes
			hash += doubleHash; //sets next possible index
			index = hash % Keys.size();
			if(index == start)
				//if true, index has cycled to original index without finding key, so returns nothing
			{
				return "";
			}
		}
	}
	return "";
}

void Hash_202::Print() const //prints hash table
{
	size_t i;
	for(i = 0; i < Keys.size(); i++) //loops through table, printing only filled positions
	{
		if(Keys[i] != "" && Vals[i] != "")
		{
			printf("%5d %s %s\n", (int) i, Keys[i].c_str(), Vals[i].c_str()); //casts strings to c-style strings to allow use in printf
		}
	}
}

size_t Hash_202::Total_Probes() //returns the amount of probes needed to fill hash table
{
	size_t i;
	Nprobes = 0;
	for(i = 0; i < Keys.size(); i++) //loops through hash table, and uses Find with each non-empty key to count how many probes
	{
		if(Keys[i] != "")
		{
			Find(Keys[i]);
		}
	}
	return Nprobes; 
}

int Last7 (string key) //hash function Last7
{
	istringstream ss; //stringstream to convert from string to int
	int hash; //variable to use
	if(key.length() > 7) //sets key to the last 7 hex digits only if the key is larger than 7 digits
	{
		key = key.substr(key.length() - 7);
	}
	ss.str(key);
	ss >> hex >> hash; //inserts key as hexadecimal into hash
	ss.clear();
	return hash;
}

int XOR (string key) //hash function XOR
{
	istringstream ss; //stringstream to convert from string to int
	int hash, tmp, size, i; //variables to use
	string temp; //temporary string variable
	if(key.size() > 7) //if key is bigger than 7, proceeds through hash table
	{
		size = key.size() % 7;
		if(size != 0) //if key isn't dividable by 7, takes any extra digits off the right end of the key and makes it the starting number
		{
			temp = key.substr(key.length() - size);
			key = key.substr(0, key.length() - size);
			ss.str(temp);
			ss >> hex >> hash;
			ss.clear();
		}
		else //if key is dividable by 7, starting number is 0
		{
			hash = 0; 
		}
		size = key.size() / 7; //sets how many times the loop must iterate
		for(i = 0; i < size; i++) //XORs the seven right digits of key with the number in hash 
		{
			temp = key.substr(key.length() - 7);
			ss.str(temp);
			ss >> hex >> tmp;
			hash = hash ^ tmp;
			key = key.substr(0, key.length() - 7);
			ss.clear();
		}
	}
	else //if key has less than 7 digits, uses the key as the hash
	{
		ss.str(key);
		ss >> hex >> hash;
		ss.clear();
	}
	return hash;
}

	
