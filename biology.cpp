#include <iostream>
#include <cmath>
#include <stdlib.h>
#include "util.h" // contains constants
#include "biology.h"
//macro for random number between 0 and 1.

#define RANDOM_NUM_ ((float)rand()/(RAND_MAX+1.0))

using namespace std;
/*
Bin2Dec takes a chunk of zeroes and ones and turn them
into human-friendly numbers. In this case is from 0 to 13 that matters.
This code is stricted to 0 to 9 algarisms plus basic operands. You can
implement a more complex one by changing the values in functions represented
below. For that, I recommend rewriting the code and understanding what
is going on and the basic ideas behind it.
*/

int Bin2Dec(string bits){
	int value = 0;
	int baseval=1;
	for (int i = bits.length();i>0;--i){
		if (bits.at(i-1)=='1'){
			value+=baseval;
		}
	baseval*=2;
	}
	return value;
}
/*
Mutate runs a rng-based mutation. mutation_rate must be veeery low,
otherwise everytime we run, we increase the chaotic level
*/
void Mutate(string &bits){
	for (int i=0; i<bits.length();++i){ //must: test backwards run fx
		if (RANDOM_NUM_<= mutation_rate){ //must: test diff between < and <=
			if (bits.at(i) == '1')
				bits.at(i) = '0';
			else
				bits.at(i) = '1';
		}
	}
}

/*Crossover function chooses a random position on a chromossome and chrosses two chromossomes.*/

void Crossover(string &child1,string &child2){
	if(RANDOM_NUM_> crossover_rate){
		int Crosspoint = (RANDOM_NUM_)*(chromo_length);
		string temp1;
		string temp2;
		temp1 = child1.substr(0,Crosspoint) + child2.substr(Crosspoint,chromo_length);
		temp2 = child2.substr(0,Crosspoint) + child1.substr(Crosspoint,chromo_length);
		child1 = temp1;
		child2 = temp2;
	}
}

/*
ValCounterBits returns the number of value operands (operators and numbers)
and stores them into a buffer.  We could still use a global buffer and vanish with it every 
time we use the buffer.
*/

int ValCounterBits (string bits, int* buffer){
	bool lfOperator = true; // "am I looking for an operator, sir ?" (asks the program)
	int current_gene = 0; //the name explains itself
	int iBuff = 0; //buffer index
	for (int i=0; i<chromo_length;i+=gene_length){ //gene length-wise linear reading loop
		current_gene = Bin2Dec(bits.substr(i,gene_length)); // translates genes to number||rsymbols
		if(lfOperator){
			if ((current_gene<10 or current_gene >13)) //ifnot an operator, keeps looking for one.
				continue;
			else { //if it reaches here, current gene is operator. put the gene into buffer and sums1
					// to index.
				lfOperator = false; // then we are now not looking for an operator anymore.
				buffer[iBuff++]= current_gene; // and the gene is restored and the indext updated
				continue; // now we can go that the job is done for this chunk of genes.
			}
		}
		else {
			if (current_gene > 9) //not looking for an operator. If it is an operator, we continue
				continue;         // looking for it.
			else {  // if it is not an operator, we store it, update index and turn the
				lfOperator = true;  // looking for operator condition false.
				buffer[iBuff++]= current_gene;
				continue;
			}
		}

	}
	for (int i=0; i<iBuff; i++){
		if ( (buffer[i] == 13) && (buffer[i+1] == 0) )	
			buffer[i] = 10;
	}

	return iBuff;
}

/*
FitnessValue returns the fitness given by 1/(goal-current_fitness),which is "infinite" if the goal actually
reaches infinity. This way we won't have problems by judging really close values as equal.
*/

float FitnessValue(string bits,float goal){
	int Buffer[(int)(chromo_length)/(gene_length)];
	int numberofelements;
	float result=0.0f;
	numberofelements = ValCounterBits(bits,Buffer);
	for (int i = 0 ; i<numberofelements-1;i+=2){
		switch(Buffer[i]){
			case 10:
				result+=Buffer[i+1];
				break;
			case 11:
				result-=Buffer[i+1];
				break;
			case 12:
				result*=Buffer[i+1];
				break;
			case 13:
				if(Buffer[i+1]!=0){
				result/=Buffer[i+1];
				break;
				}
		}
	}
	if (result == goal){
		return 999.0;  // max val returned (could be anything bigger than 1)
	}
	else{
		return fabs(1/(float)(goal-result)); //fabs makes sure it is absolute
	}
}

/*fitnessSum returns total fitness of a population*/

float fitnessSum(chromo_type* Population){
	float Result=0.0;
	for (int i=0;i<POP_SIZE;++i){
		Result+=Population[i].fitness;
	}
	return Result;
}


/* The name explains itself. Returns a string with random zeroes and ones */

string GenerateRandomBits(int length){
	string bits;
	for (int i=0;i<length;++i){
		if (RANDOM_NUM_>0.5f){
			bits+="0"; //concatenate 0
		}
		else{
			bits+="1"; //concatenate 1
		}
	} //this function should be casted the fewer.
	return bits;
}

/*Roulette Wheel method to select a specimen to breed. It's a method
biased to select the ones with the highest fitness evaluation.*/

string Roulette (float fitnessSum, chromo_type* Population){
	float CutValue = (RANDOM_NUM_)*(fitnessSum);
	float fitnesscounter = 0.0f;

	for (int i=0;i<POP_SIZE;++i){
		fitnesscounter+=Population[i].fitness;
		if (fitnesscounter>= CutValue){
			return Population[i].bits;
		}
	}
	return Roulette (fitnessSum, Population);
}

/*Genes are bunch of numbers within a certain range. This
function prints the symbol represented by certain value.
In case of numbers, the symbol is the number itself*/

void PrintGeneSymbol (int val){
	if (val<10){
		cout << val << " ";
		return ;
	}
	else{
		switch(val){
			case 10:
				cout<<"+";
				break;
			case 11:
				cout <<"-";
				break;
			case 12:
				cout <<"*";
				break;
			case 13:
				cout<<"/";
				break;
		}// end switch
	cout << " ";
	return ;
	}
}

/*PrintChromoprints a single chromossome*/

void PrintChromo(string bits){ //prints chromossomes
	for (int i=0;i<bits.length();++i){
		cout << bits.at(i) ;
		if ((i+1)%gene_length==0 and i!=0)
			cout << " ";
	}
	cout << "\n";
	return ;
}

/* Prints the sequence operator-number-operator of a certain
specimen*/

void PrintSymbolSequence(string bits){
	int i,numberofelements;
	int buffer[BUFF_MAX];
	numberofelements = ValCounterBits(bits, buffer);
	cout << "0 " ; // every sum starts with zero.
	for (i= 0; i<numberofelements;++i){
		PrintGeneSymbol(buffer[i]);
	}
	cout << endl;
	return;
}

/* check_solve checks if a single chromossome is a solution,
returning a boolean value */

bool check_solve(chromo_type* Population){
	int breaker;
	for (int i=0;i<POP_SIZE;++i){
		if (Population[i].fitness >= 999.0f){
			cout << "ALERT!! ALERT !! We have found an evolved specimen:" << endl << endl;
			PrintChromo(Population[i].bits);
			cout << "\nWhich represents the solution :" << endl;
			PrintSymbolSequence(Population[i].bits);
			cout << "\nPress any button to exit." << endl;
			cin >> breaker;
			return true;
		}
	}
	return false;
}

/* check_all checks if a whole population is a population of
solutions, returning a boolean value */

bool check_all (chromo_type* Population){
	int breaker;
	for (int i=0;i<POP_SIZE;++i){
		if (Population[i].fitness < 990.9f){
			return false;
		}
	}
	cout << "HOLY SMOKES!!! YOUR POPULATION IS ULTRA EVOLVED!!!! " << endl;
	return true;
}