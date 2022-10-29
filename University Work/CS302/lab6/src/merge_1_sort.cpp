//Alexander Yu - CS302
//Lab 6A: Merge Sort
//March 21st, 2022

/*This lab focuses on implementing sorting algorithms. This part of the lab is the implementation of a merge sort, which uses recursion*/

#include <iostream>
#include <vector>
#include <cstdio>
#include "sorting.hpp"
using namespace std;

void recursive_sort(vector <double> &v, vector <double> &temp, int start, int size, int print);

void sort_doubles(vector <double> &v, bool print) //method used by driver
{
	vector <double> temp; //creates empty temp vector for use in recursion
	if(print) 
	{
		recursive_sort(v, temp, 0, (int) v.size(), 1);
		printf("                    "); //prints padding
		for(size_t i = 0; i < v.size(); i++) //prints sorted vector
		{
			printf("%5.2f", v[i]);
		}
		cout << endl;
	}
	else
	{
		recursive_sort(v, temp, 0, (int) v.size(), 0);
	}
}

void recursive_sort(vector <double> &v, vector <double> &temp, int start, int size, int print) //recursive method that sorts via merging
{
	int i, j;
	if(size == 1) //base case 1: if size is 1, vector is sorted
	{
		return;
	}
	else if(size == 2) //base case 2: if size is 2, sorting is done directly
	{
		printf("B:%6d%6d     ", start, size); //prints start index and size of current section
		for(i = 0; i < (int) v.size(); i++) //prints entire vector
		{
			printf("%5.2f", v[i]);
		}
		cout << endl;
		if(v[start] > v[start+1]) //swaps values if first is larger than second
		{
			double tmp;
			tmp = v[start];
			v[start] = v[start+1];
			v[start+1] = tmp;
			if(print == 1) //prints entire vector with sorted section
			{
				printf("E:%6d%6d     ", start, size);
				for(i = 0; i < (int) v.size(); i++)
				{
					printf("%5.2f", v[i]);
				}
				cout << endl;
			}
			return;
		}
		else //simply prints the vector if values are in correct order
		{
			if(print == 1)
			{
				printf("E:%6d%6d     ", start, size);
				for(i = 0; i < (int) v.size(); i++)
				{
					printf("%5.2f", v[i]);
				}
				cout << endl;
			}
			return;
		}
	}
	if(print == 1) //prints vector before sorting
	{
		printf("B:%6d%6d     ", start, size);
		for(i = 0; i < (int) v.size(); i++)
		{
			printf("%5.2f", v[i]);
		}
		cout << endl;
	}
	recursive_sort(v, temp, start, size/2, print); //recursively calls on first half of vector
	recursive_sort(v, temp, start + size/2, size - size/2, print); //recursively calls on second half of vector
	temp.clear(); //clears temporary vector to prevent extraneous values
	i = start; //i starts at beginning of section
	j = start + size/2; //j starts halfway through section
	while(i < start + size/2 || j < start + size) //iterates until i has reached half of section and j has reached end of section
	{
		if(i == start + size/2) //if i is at destination but j isn't, pushes back j into temp
		{
			temp.push_back(v[j]);
			j++;
		}
		else if(j == start + size) //ditto, except vice versa
		{
			temp.push_back(v[i]);
			i++;
		}
		else if(v[i] < v[j]) //if value at i is less than value at j, pushes back i
		{
			temp.push_back(v[i]);
			i++;
		}
		else //otherwise, pushes back j
		{
			temp.push_back(v[j]);
			j++;
		}
	}
	j = 0;
	for(i = start; i < start + size; i++) //replaces values in v with corresponding values in temp
	{
		v[i] = temp[j];
		j++;
	}
	if(print == 1) //prints vector after sorting
	{
		printf("E:%6d%6d     ", start, size);
		for(i = 0; i < (int) v.size(); i++)
		{
			printf("%5.2f", v[i]);
		}
		cout << endl;
	}
	return;
}
