//Lab AA: Dice or No Dice - Dynamic Programming
//Alexander Yu - CS302
//April 21st, 2022

/*This lab is the 1st of three parts, each concerning dynamic programming. 
 *This part uses dynamic programming to calculate probabilities of rolling 
 dice with results matching a set of parameters.*/

#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

class Dice //defines a Dice class for function and cache storage
{
	public:
		double diceCalculation(int s, int t, int dice_roll);
		vector < double > cache;
};

double Dice::diceCalculation(int s, int t, int dice_roll) //function to calculate probability
{
	double average;
	vector <double> temp; //holds the 'next level' of probabilities before it becomes the new cache
	/*i.e. for xth iteration, temp holds probabilities for rolling dice correctly x+1 times*/
	int i, j, k; //iterating variables
	average = 0;
	for(i = 0; i < t - 2; i++) //cache is modified until it is one level under t
	{
		temp.clear(); //clears and resizes temp vector
		temp.resize(s, 0);
		for(j = 0; j < (int) cache.size(); j++)
		{
			for(k = 0; k < (int) cache.size(); k++) //for each entry in the cache, adds all entries that are not itself or adjacent to corresponding location in temp
			{
				if(k != j && k != j - 1 && k != j + 1)
				{
					temp[j] += cache[k];
				}
			}
			temp[j] /= s; //divides by s as per probability formula
		}
		cache = temp; //sets new cache
	}
	if(dice_roll == -1) //if dice_roll is -1, finds probability of rolling s-sided dice t times
	{
		for(i = 0; i < (int) cache.size(); i++) //sums all values of cache
		{
			average += cache[i];
		}
	}
	else //else, finds probability of getting a correct roll if previous roll was dice_roll
	{
		for(i = 0; i < (int) cache.size(); i++)
		{
			if(i != dice_roll && i != dice_roll - 1 && i != dice_roll + 1) //sums all values that are not dice_roll or adjacent to dice_roll
			{
				average += cache[i];
			}
		}
	}
	average /= s; //divides by s as per probability calculation
	return average;
}

int main(int argc, char** argv) //main function 
{
	int i, s, t, last_roll;
	double chance;
	Dice d;
	if(argc != 4) //error checks for correct input
	{
		cerr << "usage: diceCalculation -- s t last-roll" << endl;
		return -1;
	}
	s = atoi(argv[1]);
	t = atoi(argv[2]);
	last_roll = atoi(argv[3]);
	if(s <= 0 || t <= 0) //error checks for valid s and t; invalid last_roll is simply set to 0
	{
		cerr << "s and t must be greater than zero" << endl;
		return -1;
	}
	chance = (double) (s - 2) / (double) s;
	d.cache.push_back(chance); //sets up the cache for probabilities after rolling once
	for(i = 0; i < (s - 2); i++)
	{
		chance = (double) (s - 3) / (double) s;
		d.cache.push_back(chance);
	}
	chance = (double) (s - 2) / (double) s;
	d.cache.push_back(chance);
	if(t == 1) //if t is 1, cache is already set up
	{
		if(last_roll == -1) //the first roll will always be correct
		{
			cout << 1 << endl;
		}
		else
		{
			cout << d.cache[last_roll] << endl;
		}
	}
	else //calculates probability
	{
		cout << d.diceCalculation(s, t, last_roll) << endl;
	}
	return 0;

}
