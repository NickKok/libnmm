#pragma once
#ifndef __OPTIMIZATION_HH__
#define __OPTIMIZATION_HH__

#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef OPTIMIZATION



#include <optimization/webots.hh> //to include the optimizer framework
typedef optimization::Webots& optimizer;
typedef optimization::messages::task::Task::Parameter optimizerParameters;
#else
//#error fake it
//typedef bool optimizerParameters;

class optimizerParameters{
public:
	bool value(){
		return false;
	}
	double operator()()
 	{
		return false;
	}
};


#include <map>
#include <string>
class FakeOpti;
typedef FakeOpti& optimizer;


class FakeOpti{
private:
public:
	static FakeOpti fake;
	static optimizer Instance(){
		return fake;
	}
	void Respond(std::map<std::string, double> fitnesses){
	}
	bool Setting(std::string, std::string){
		return false;
	}
	bool Setting(std::string){
		return false;
	}
	bool Parameter(std::string a, optimizerParameters&){
		return false;
	}
	FakeOpti(){
	}
	operator int() const
	{ 
		return 0; 
		
	}
};


#endif

optimizer tryGetOpti();

class Timer{
    struct timeval tvBegin, tvEnd, tvDiff;
public:
Timer(){}
void Start();
void End();
void Diff();
    
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1);
void timeval_print(struct timeval *tv);
};

#endif
