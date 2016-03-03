#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define crossover_rate 0.42
#define mutation_rate 0.005 // that's a high mutation rate compared to real rates.
#define POP_SIZE 60
#define chromo_length 100
#define gene_length 4
#define max_allowable_generations 1000
//macro for random number between 0 and 1.
#define RANDOM_NUM_ ((float)rand()/(RAND_MAX+1.0))

using namespace std;

void proto_print(string bits); // debugging function. Must delete at final tests.

 void PrintGeneSymbol(int value); // prints the gene of a certain value (1,2,3,...,+,-,...)
 void PrintChromos(string bits);
 void Mutate(string &bits);
 void Crossover(string &child1,string &child2);
 float FitnessValue(string bits, float goal);
 string Roulette();
 string GenerateRandomBits(int length);
 int Bin2Dec(string bits);
 int ReadBitsval (string bits, int* buffer);

 struct chromo_type{ //we create a struct type for chromossomes
 	string bits;
 	float fitness;

 	chromo_type():bits(" "),fitness(0.0f){}; // init if uncasted
 	chromo_type(string bts,float ftns):bits(bts),fitness(ftns){} //init if casted
 };


int main (){
	srand (int(time(NULL))); // seeding srand

	while (true){  //the loop goes until we reach maximum fitness, assigned by user.

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
	return iBuff;
}

/*
FitnessValue returns the fitness given by 1/(goal-current_fitness),which is "infinite" if the goal actually
reaches infinity. This way we won't have problems by judging really close values as equal.
*/

float FitnessValue(string bits,int goal){
	int Buffer[(int)(chromo_length)/(gene_length)];
	int numberofelements;
	float result=(float)0.0;
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
				result/=Buffer[i+1];
				break;
		}
	}
	if (result == goal){
		return 999.0;  // max val returned (could be anything bigger than 1)
	}
	else{
		return fabs(1/(float)(goal-result)); //fabs makes sure it is absolute
	}

}


// The name explains itself. Returns a string with random 0 and ones
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
void proto_print(string bits){ //prints chromossomes
	for (int i=0;i<bits.length();++i){
		cout << bits.at(i) ;
		if ((i+1)%gene_length==0 and i!=0) //debugging
			cout << " ";
	}
	cout << "\n";
	return ;
}
