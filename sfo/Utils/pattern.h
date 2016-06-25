#ifndef PATTERN_H
#define PATTERN_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <string>


#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "time.h"
#include <stdarg.h>
#include <assert.h>
#include <queue>
using namespace std;

class Triplet{
	public:
	int instanceId;
	int sid;
	int beginPos;
	int endPos;
	//extract ids from comma separated string e.g. 280,15,21 
	Triplet(string);
	string tostring();
	

};

Triplet::Triplet(string str)
	{
		std::stringstream ss(str);
		ss>>sid;
		//if(ss.peek==',')
		ss.ignore();
		ss>>beginPos;
		ss.ignore();
		ss>>endPos;
		//cout<<this->tostring()<<endl;
	}
	
string Triplet::tostring()
	{
		return std::to_string(sid)+","+std::to_string(beginPos)+","+std::to_string(endPos);
	}
	
	
class Pattern{
	public:
	int patternid;
	std::string patternText;
	vector<Triplet> occurances;
	double patternCost;
	Pattern(int,string);
	string tostring();
	double getModularCost();

};

Pattern::Pattern(int id,string str){
		patternid = id;
		patternText=str;
		occurances = vector<Triplet>();
	}
	
	string Pattern::tostring()
	{
		std::string str=patternText;
		for(std::vector<Triplet>::iterator it = occurances.begin(); it != occurances.end(); ++it) {
			str=str+" "+(*it).tostring();
		}
		return str;
	}
	
	double Pattern::getModularCost() {
		return patternCost; 
	}



#endif

