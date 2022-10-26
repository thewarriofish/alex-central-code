#include <iostream>
using namespace std;

int main()
{
	char temp;
	int total;
	while (cin >> temp)
	{
		temp -= 64;
		if(temp >= 1 && temp <= 26)
		{
			total += temp;
		}
	}
	
	cout << total << endl;

	return 0;
}
