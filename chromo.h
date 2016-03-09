#include <string.h>
#ifndef CHROMO_H
#define CHROMO_H

struct chromo_type{ //we create a struct type for chromossomes
 	std::string bits;
 	float fitness;

 	chromo_type():bits(" "),fitness(0.0f){}; // init if uncasted
 	chromo_type(std::string bts,float ftns):bits(bts),fitness(ftns){} //init if casted
 };


 #endif