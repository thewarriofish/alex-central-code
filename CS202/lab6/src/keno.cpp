//Alexander Yu - CS202
//October 7th, 2021
//Lab 6-2: Keno

/*This program simulates a game of Keno, taking standard input as parameters. This program uses the Fraction class.*/

#include <iostream>
#include <cstdio>
#include <vector>
#include "fraction.hpp"
using namespace std;

int main()
{
	Fraction p; //variables to use
	size_t i;
	int balls, win;
	double payout, bet;
	vector <double> prizes, prob;
	vector <int> winners;
	cin >> bet >> balls; //inputs the bet amount and amount of balls
	while(cin >> win >> payout) //inputs the prizes and amount of balls needed for each prize; using only push_back maintains order
	{
		winners.push_back(win);
		prizes.push_back(payout);
	}
    for(i = 0; i < winners.size(); i++) //calculates the probabilities of each prize being earned using Fraction class
	{
		p.Multiply_Binom(80-balls, 20-winners[i]);
		p.Multiply_Binom(balls, winners[i]);
		p.Divide_Binom(80, 20);
		prob.push_back(p.Calculate_Product());
		p.Clear();
	}
	payout = 0;
	for(i = 0; i < prizes.size(); i++) //calculates the total return; payout starts at zero and is summed with the product of prob and prizes
	{
		payout += prob[i] * prizes[i];
	}
	payout -= bet;
	printf("Bet: %.2f\n", bet); //prints with formatting: bet (padded to 2 decimals), balls picked and probabilities of each outcome
	printf("Balls Picked: %i\n", balls);
	for(i = 0; i < winners.size(); i++)
	{
		cout << "  Probability of catching " << winners[i] << " of " << balls << ": " << prob[i] << " -- " << "Expected return: " << prob[i] * prizes[i] << endl;
	}
	printf("Your return per bet: %.2f\n", payout + 0.0); //total return of bet, padded to 2 decimals; adding 0.0 prevents negative zeroes
	printf("Normalized: %.2f\n", (payout/bet) + 0.0); //normalized return, padded to 2 decimals; acquired by dividing total return with the bet amount
	return 0;
}
