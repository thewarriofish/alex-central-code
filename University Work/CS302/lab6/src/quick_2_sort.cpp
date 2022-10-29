//Alexander Yu - CS302
//Lab 6B: Quick Sort
//March 21st, 2022

/*This part of the lab focuses on the implementation of quick sort using recursion*/

#include <iostream>
#include <vector>
#include <cstdio>
#include "sorting.hpp"
using namespace std;

void recursive_sort(vector <double> &v, int start, int size, int print);

void sort_doubles(vector <double> &v, bool print) //uses recursive method to sort
{
	if(print)
	{
		recursive_sort(v, 0, (int) v.size(), 1);
		printf("                    "); //prints padding
		for(int i = 0; i < (int) v.size(); i++) //prints vector values
		{
			printf("%5.2f", v[i]);
		}
		cout << endl;
	}
	else
	{
		recursive_sort(v, 0, (int) v.size(), 0);
	}
}

void recursive_sort(vector <double> &v, int start, int size, int print) //recursive quicksort method
{
	int i, left, right;
	if(size == 1 || size <= 0) //base case 1: if size is 1 just return 
	{
		return;
	}
	if(print == 1) //prints vector values before any sorting
	{
		printf("S:%6d%6d%11.2f", start, size, v[0]);
		for(i = 1; i < (int) v.size(); i++)
		{
			printf("%5.2f", v[i]);
		}
		cout << endl;
	}
	if(size == 2) //base case 2: if size is 2, sorts directly
	{
		double tmp;
		if(v[start] > v[start+1])
		{
			tmp = v[start];
			v[start] = v[start+1];
			v[start+1] = tmp;
			return;
		}
		else
		{
			return;
		}
	}
	double t;
	/*if middle is median of start, middle, end, swaps start and middle*/
	if((v[start+size/2] > v[start+size-1] && v[start] > v[start+size/2]) || (v[start+size/2] < v[start+size-1] && v[start] < v[start+size/2])) 
	{
		t = v[start];
		v[start] = v[start+size/2];
		v[start+size/2] = t;
	}
	/*if end is median of start middle end, swaps start and end; also accounts for when middle and end are equal and unequal to start*/
	else if((v[start+size-1] > v[start] && v[start+size-1] < v[start+size/2]) || (v[start+size-1] < v[start] && v[start+size-1] > v[start+size/2]) || (v[start+size-1] == v[start+size/2] && v[start+size-1] != v[start]))
	{
		t = v[start];
		v[start] = v[start+size-1];
		v[start+size-1] = t;
	}
	if(print == 1) //prints vector with pivot value
	{
		printf("M:%6d%6d%6.2f", start, size, v[start]);
		for(i = 0; i < (int) v.size(); i++)
		{
			printf("%5.2f", v[i]);
		}
		cout << endl;
	}
	left = start + 1; 
	right = (start + size) - 1;
	while(left < right) //iterates until left has passed right, in which case all values have been sorted relative to pivot
	{
		while(v[left] < v[start] && left != (start+size)) //iterates left until v[left] is greater than or equal to pivot, or left has reached end of section
		{
			left++;
		}
		while(v[right] > v[start] && right != start) //iterates right until v[right] is less than or equal to pivot, or right has reached beginning of section
		{
			right--;
		}
		if(right <= left) //if left is past right, breaks to prevent unneeded swaps
		{
			break;
		}
		t = v[left]; //swaps values 
		v[left] = v[right];
		v[right] = t;
		left++; //increments and decrements respective values
		right--;
	}
	if(left == right) //if left equals right, changes one of the values depending on surrounding values
	{
		if(v[start] >= v[right-1] && v[right] > v[start]) //if the previous value is less than or equal to the pivot and the current value is greater than the pivot, decrements right
		{
			right--;
		}
		else //if the next value is greater than pivot, increments left
		{
			left++;
		}
	}
	t = v[start]; //swaps value at pivot with value at right
	v[start] = v[right];
	v[right] = t;
	if(print == 1) //prints sorted vector
	{
		printf("P:%6d%6d%6d", start, size, right);
		for(i = 0; i < (int) v.size(); i++)
		{
			printf("%5.2f", v[i]);
		}
		cout << endl;
	}
	recursive_sort(v, start, right - start, print); //recursively sorts 1st half
	recursive_sort(v, left, size - (left-start), print); //recursively sorts 2nd half
}
