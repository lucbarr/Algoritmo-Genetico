#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>

#define crossover_rate 0.8
#define mutation_rate 0.007 // that's a high mutation rate compared to real rates.
#define POP_SIZE 40
#define chromo_length 100
#define gene_length 4
#define max_allowable_generations 1000
#define BUFF_MAX 256
//macro for random number between 0 and 1.
#define RANDOM_NUM_ ((float)rand()/(RAND_MAX+1.0))

using namespace std;

struct chromo_type{ //we create a struct type for chromossomes
 	string bits;
 	float fitness;

 	chromo_type():bits(" "),fitness(0.0f){}; // init if uncasted
 	chromo_type(string bts,float ftns):bits(bts),fitness(ftns){} //init if casted
 };

 void proto_print(string bits);
 bool check_solve(chromo_type* Population);
 bool check_all (chromo_type* Population);

 void PrintSymbolSequence(string bits);
 void PrintGeneSymbol(int value); // prints the gene of a certain value (1,2,3,...,+,-,...)
 void PrintChromos(string bits);
 void Mutate(string &bits);
 void Crossover(string &child1, string &child2);
 float FitnessValue(string bits,float goal);
 float fitnessSum(chromo_type* Population);
 string Roulette(float fitnessSum, chromo_type* Population);
 string GenerateRandomBits(int length);
 int Bin2Dec(string bits);
 int ValCounterBits (string bits, int* buffer);

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
				cout << Population[i].fitness << endl;
			}
			totalFitness = fitnessSum(Population);
			cout << endl << endl;
			cout << totalFitness << endl;
			cout << endl << endl;
			switch (Breaker){
				case check_one:
					if(check_solve(Population)){
						cin >> Breaker;
						return 0;
					}
				case check_All:
					if(check_all(Population)){
						cout << "It took " << GenCounter << " generations to find the BEST population!!!" <<endl;
						cout << "Here are all the " << POP_SIZE << " solutions we found:" << endl << endl;
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

/*proto_print prints a single chromossome*/

void proto_print(string bits){ //prints chromossomes
	for (int i=0;i<bits.length();++i){
		cout << bits.at(i) ;
		if ((i+1)%gene_length==0 and i!=0)
			cout << " ";
	}
	cout << "\n";
	return ;
}
void PrintSymbolSequence(string bits){
	int i,numberofelements;
	int buffer[BUFF_MAX];
	numberofelements = ValCounterBits(bits, buffer);
	for (i= 0; i<numberofelements;++i){
		PrintGeneSymbol(buffer[i]);
	}
	cout << endl;
	return;
}

bool check_solve(chromo_type* Population){
	int breaker;
	for (int i=0;i<POP_SIZE;++i){
		if (Population[i].fitness >= 999.0f){
			cout << "ALERT!! ALERT !! We have found an evolved specimen:" << endl << endl;
			proto_print (Population[i].bits);
			cout << "\nWhich represents the solution :" << endl;
			PrintSymbolSequence(Population[i].bits);
			cout << "\nPress any button to exit." << endl;
			cin >> breaker;
			return true;
		}
	}
	return false;
}

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