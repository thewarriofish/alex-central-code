//Alexander Yu - CS202
//Completed August 26, 2021
//Lab 1-2: Moonglow


#include <iostream>
#include <string>
using namespace std;

int main()
{
	string temp; //define temp
	string name = " "; //define name; set to null
	double total = 0; //define total; set to 0
	double score = 0; //define score; set to 0
	double average = 0; //define average; set to 0
	int counter = 0; //define counter for num. of numbers used for average; set to 0
	while(1)
	{
			if(cin >> score) //reads if input is a double
			{
				temp = " "; //sets temp to null for later if statements
				total += score; //adds double to total
				score = 0; //resets score to zero to prevent multiple additions
			}
			else //acts if input is not a double, i.e. a string
			{
				cin.clear(); //clears cin error state
				cin >> temp; //reads string
				if(temp == "NAME") //checks if temp is NAME
				{
					cin >> name; //inputs following word into name
				}
				if(temp == "AVERAGE") //checks if temp is AVERAGE
				{
					while(cin >> score) //checks if following input is a double
					{
						average += score; //adds double into average
						counter++; //increases amount of numbers used
					}
					cin.clear(); //clears cin error state
					if(counter != 0) //checks if counter is 0 to prevent divide by zero
					{
						average /= counter; //divides value in average by number of numbers used
						total += average; //adds average into total
						average = 0; //resets average for later use
						counter = 0; //resets counter for later use
						score = 0; //resets score for later use
					}
				}
			}
			if(cin.eof()) //checks for end-of-file
			{
				break;
			}
	}
	cout << name << " " << total << endl; //print name and total score
	return 0;
}
