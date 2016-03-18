#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include "util.h" // contains constants
#include "biology.h"
//macro for random number between 0 and 1.
#define RANDOM_NUM_ ((float)rand()/(RAND_MAX+1.0))

using namespace std;

int main (){
	int Breaker;
	char Choice;
	enum Seach_protocol{check_one,check_All};
	srand (int(time(NULL))); // seeding srand
	static int GenCounter=0;
	float goal;
	float totalFitness;
	int i=0;
	int PopPopper=0;
	chromo_type Population[POP_SIZE];
	totalFitness = 0.0f;
		for (i=0;i<POP_SIZE;++i){
			Population[i].bits = GenerateRandomBits(chromo_length);
			Population[i].fitness = 0.0f;
		}
		cout << "Goal value number: (Be careful with unreachable numbers!!) " << endl;
		cin >> goal;
		cout << "Do you want to find first solution or a " << POP_SIZE << " individuals sized population ?" << endl;
		cout << "0. First solution " << endl;
		cout << "1. Population of solutions " << endl;
		cin >> Breaker;
		while (true){  //the loop goes until we reach maximum fitness, assigned by user.
			totalFitness =0;
			for (i=0;i<POP_SIZE;++i){
				Population[i].fitness = FitnessValue(Population[i].bits, goal);
			}
			totalFitness = fitnessSum(Population);
			switch (Breaker){
				case check_one:
					if(check_solve(Population)){
						cout << "(Solution found in " << GenCounter << "th Generation " << "for goal = " << goal << ")"<< endl;
						cin >> Breaker;
						return 0;
					}
				case check_All:
					if(check_all(Population)){
						cout << "It took " << GenCounter << " generations to find the BEST population!!!" <<endl;
						cout << "Here are all the " << POP_SIZE << " solutions we found (some may be equal):" << endl << endl;
						for (i=0;i<POP_SIZE;++i){
							cout << i+1 << "th solution : ";
							PrintSymbolSequence(Population[i].bits);
						}
						cout << "Press any button to exit." << endl;
						cin >> Breaker;
						return 0;
				default:
					break;
			}
			}
			 //if we don't find a solution, we generate another population.
				chromo_type tempPop[POP_SIZE];
				PopPopper=0;
				while (PopPopper < POP_SIZE){
					string child1 = Roulette(totalFitness, Population);  // Note: must add genre discrimination into chromo_type struct 
					string child2 = Roulette(totalFitness, Population);  // and adapt the algorithm to sexualized non-hermaphrodite reproduction.
					Crossover(child1,child2);
					Mutate(child1);
					Mutate(child2);
					tempPop[PopPopper].bits = child1;
					tempPop[PopPopper++].fitness = 0.0f;
					tempPop[PopPopper].bits = child2;
					tempPop[PopPopper++].fitness = 0.0f;
				}	
				for (i=0;i<POP_SIZE;++i){
					Population[i].bits = tempPop[i].bits;
					Population[i].fitness = tempPop[i].fitness;
				}
				GenCounter++;
			}
	return 0;
}