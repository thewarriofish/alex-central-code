//Alexander Yu - CS202
//October 7th, 2021
//Lab 6-1: Fraction

/*This program implements the Fraction class, to be used with the Keno program. It stores numbers into multisets that represent the numerator and denominator. In addition, this class also supports factorials and binominals.*/

#include <set>
#include "fraction.hpp"
#include <iostream>
using namespace std;

void Fraction::Clear() //clears the fraction, making it 1
{
	numerator.clear();
	denominator.clear();
}

bool Fraction::Multiply_Number(int n) //adds a number into the numerator
{
	if(n <= 0) //checks that n is nonzero positive
	{
		return false;
	}
	if(n == 1) //function does nothing if n = 1
	{
		return true;
	}
	if(denominator.find(n) == denominator.end()) //checks if number is in denominator; removes from denominator if true, adds to numerator if false
	{
		numerator.insert(n);
	}
	else
	{
		denominator.erase(denominator.find(n));
	}
	return true;
}
bool Fraction::Divide_Number(int n) //adds a number into the denominator
{
	if(n <= 0) //checks that n is nonzero positive
	{
		return false;
	}
	if(n == 1) //function does nothing if n = 1
	{
		return true;
	}
	if(numerator.find(n) == numerator.end()) //checks if number is in numerator; removes from numerator if true, adds to denominator if false
	{
		denominator.insert(n);
	}
	else
	{
		numerator.erase(numerator.find(n));
	}
	return true;

}

bool Fraction::Multiply_Factorial(int n) //adds a factorial into numerator
{
	int i; //loop variable
	if(n <= 0) //checks parameters
	{
		return false;
	}
	for(i = 2; i <= n; i++) //skips 1 to remove extraneous output, uses Multiply_Number from 2 to n to simulate factorial
	{
		Multiply_Number(i);
	}
	return true;

}
bool Fraction::Divide_Factorial(int n) //adds a factorial into denominator
{
	int i; //loops variable
	if(n <= 0) //parameter check
	{
		return false;
	}
	for(i = 2; i <= n; i++) //skips 1 to remove extraneous output, uses Divide_Number from 2 to n to simulate factorial
	{
		Divide_Number(i);
	}
	return true;
}

bool Fraction::Multiply_Binom(int n, int k) //adds binominal into fraction with format n!/(k!*(n-k)!)
{
	int i; //loop variable
	if(n <= 0 || k < 0) //checks parameters
	{
		return false;
	}
	if(k == 0 || n == k) //if k is 0 or n = k, binomial is 1, so nothing happens
	{
		return true;
	}
	for(i = n - k + 1; i <= n; i++) //uses Multiply_Number on numbers that are present after division
	{
		Multiply_Number(i);
	}
	for(i = 2; i <= k; i++) //uses Divide_Number from 2 to k, which are number present in denominator after division
	{
		Divide_Number(i);
	}
	return true;
}
bool Fraction::Divide_Binom(int n, int k) //adds binominal into fraction with format (k!*(n-k)!)/n!
{
	int i; //loop variable
	if(n <= 0 || k < 0) //parameter check
	{
		return false;
	}
	if(k == 0 || n == k) //if k = 0 or n = k, binominal is 1
	{
		return true;
	}
	for(i = 2; i <= k; i++) //uses Multiply_Number from 2 to k, which are numbers present in numerator after division
	{
		Multiply_Number(i);
	}
	for(i = n - k + 1; i <= n; i++) //uses Divide_Number on numbers present after cancellation
	{
		Divide_Number(i);
	}
	return true;
}

void Fraction::Invert() //switches numerator and denominator
{
	numerator.swap(denominator);
}

void Fraction::Print() const //prints out numerator and denominator
{
	multiset <int>::const_iterator fit; //iterator loop variable
	if(numerator.empty()) //code for when numerator is empty, and therefore 1
	{
		cout << 1;
		if(denominator.empty()) //ends output if denominator is also empty
		{
			cout << endl;
		}
		else //prints any non-one number in denominator as well as / inbetween numbers
		{
			for(fit = denominator.begin(); fit != denominator.end(); fit++)
			{
				if(*fit != 1)
				{
					cout << " / " << *fit;
				}
			}
			cout << endl;
		}
	}
	else if(!(numerator.empty())) //code for when numerator isn't empty
	{
		fit = numerator.begin();
		while(*fit == 1) //skips any 1s at the beginning
		{
			fit++;
		}
		cout << *fit; //prints the first number of fraction, which has nothing before it
		fit++; //increments to next number
		for(fit = fit; fit != numerator.end(); fit++) //prints any non-one number in numerator as well as * inbetween numbers
		{
			if(*fit != 1)
			{
				cout << " * " << *fit;
			}

		}
		if(!(denominator.empty())) //prints non-one numbers in denominator as well as / between numbers
	    {
			for(fit = denominator.begin(); fit != denominator.end(); fit++)
			{
				if(*fit != 1)
				{
					cout << " / " << *fit;
				}
			}
		}
		cout << endl;
	}
}

double Fraction::Calculate_Product() const //calculates product of fraction
{
	multiset <int>::const_iterator fit; //iterator
	double product, denom; //variables to use
	product = 1; //set variables to one for multiplication
	denom = 1;
	if(numerator.empty() && denominator.empty()) //returns one if numerator and denominator are empty
	{
		return product;
	}
	for(fit = numerator.begin(); fit != numerator.end(); fit++) //multiplies all numbers in numerator
	{
		product *= *fit;
	}
	for(fit = denominator.begin(); fit != denominator.end(); fit++) //multiplies all numbers in denominator
	{
		denom *= *fit;
	}
	product /= denom; //divides numerator product by denominator product
	return product;
}
