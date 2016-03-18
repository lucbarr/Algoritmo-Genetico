#ifndef BIOLOGY_H
#define BIOLOGY_H
#include "chromo.h"
 bool check_solve(chromo_type* Population);
 bool check_all (chromo_type* Population);

 void PrintSymbolSequence(std::string bits);
 void PrintGeneSymbol(int value);
 void PrintChromo(std::string bits);
 void Mutate(std::string &bits);
 void Crossover(std::string &child1, std::string &child2);
 float FitnessValue(std::string bits,float goal);
 float fitnessSum(chromo_type* Population);
 std::string Roulette(float fitnessSum, chromo_type* Population);
 std::string GenerateRandomBits(int length);
 int Bin2Dec(std::string bits);
 int ValCounterBits (std::string bits, int* buffer);

#endif