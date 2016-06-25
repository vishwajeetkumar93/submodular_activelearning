/*
 *	A tutorial for SOTK.
 *	Author: Pankaj Singh
 *	Email: pr.pankajsingh@gmail.com
 *
 * */


#include <functional> 
#include <cctype>
#include <locale>

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
#include <map>

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "time.h"
#include <stdarg.h>
#include <assert.h>
#include <queue>
// Utilities
#include "Utils/error.h"
#include "Utils/sort.h"
#include "Utils/utils.h"
#include "Utils/fileHandling.h"
#include "Utils/totalOrder.h"
#include "Utils/pattern.h"

using namespace std;

//Submodular Functions
#include "Functions/setFunctions.h"

#include "Functions/submodularFunctions.h"
#include "Functions/patternCoverage.h"
#include "Functions/combineSubmodularFunctions.h"
#include "Functions/scaleSubmodularFunctions.h"
#include "Functions/modularFunctions.h"
#include "Functions/translateSubmodularFunctions.h"
#include "Functions/combineSubmodularModularFunctions.h"


//Algorithms
#include "Algorithms/lazyGreedyMax.h"
#include "Algorithms/lazyGreedySC.h"
#include "Algorithms/randomizedGreedyMax.h"
#include "Algorithms/bidirGreedyMax.h"
#include "Algorithms/naiveGreedyMax.h"
#include "Algorithms/revlazyGreedyMax.h"
#include "Algorithms/localSearchMax.h"

#define SMALLEST_NUMBER -1e70
#define LARGEST_NUMBER 1e70

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}


int main(int argc, char** argv){
	//Testing of config file......
	/******************************************************************************************************/

	/******************************************************************************************************/
	if(argc != 3)
	{
		cout << "\n\nProvided "<<(argc-1) << " parameters\n\n";
		cout << "usage : "<<argv[0]<<" fraction verbosity \n";
		exit(1);
	}

	int n; // GroundSet size
	unordered_set<int> groundSet = unordered_set<int>(); // GroundSet.
	unordered_set<int> emptyset = unordered_set<int>();
	int numWords;
/* ***********************************************START************************************************************************
 * */

// LOAD CORPUS
	char* corpusFile = "../sfo/data/CorpusFile.txt";
    ifstream infile1;
	infile1.open (corpusFile);
	int senId=0;
	int wCount=0;
	int total_word=0;
	vector<vector<bool>> bitCorpus = vector<vector<bool>>();
	//inserting dummy sentence 0
	vector<bool> dummySen=vector<bool>();
	bitCorpus.push_back(dummySen);
    while(!infile1.eof()) // To get you all the lines.
    {
	   infile1 >> senId >>wCount;
	   //cout << wCount << "\t";
	   vector<bool> a=vector<bool>(wCount,false);
	   bitCorpus.push_back(a);
	   total_word+=wCount;
    }
	infile1.close();
	//cout << "There are "<<total_word<<" words in the corpus\n";
//LOAD PATTERN  sample entry(  NP NP aforesaid by CAT22208 : ( 280,15,21 ) ( 501,15,21 ) )
	//TO_DO 1- aim of NP the : (2662,8,11)(3345,17,20)(3549,15,18)
	char* patternFile = "../sfo/data/pattern";
    ifstream infile2;
	infile2.open (patternFile);
	vector<Pattern> patternCollection= vector<Pattern>();
	
	std::string line;
	while (std::getline(infile2, line))
	{
		
		int patternid = stoi(line.substr(0, line.find("- ")));
		line = line.substr(line.find("- ")+2);
		size_t lastColon=line.find_last_of(':');
		std::string patternTxt=line.substr(0,lastColon).c_str();
		trim(patternTxt);
		Pattern p=Pattern(patternid, patternTxt);
		std::string patternLoc=line.substr(lastColon+1).c_str();
		std::stringstream s(patternLoc);
		char ignoreChar;
		while(s>>ignoreChar){
			std::string tripletstr;
			s>>tripletstr;
			Triplet t=Triplet(tripletstr);
			p.occurances.push_back(t);
			s>>ignoreChar;
		}
		//cout<<endl;
//		cout<< patternLoc<<endl;
        //cout<<p.tostring();
		patternCollection.push_back(p);
	}
	infile2.close();
// GET PATTER MODULAR COST*************************************************************************************************
	char* predFile = "svm_predictions";
    	ifstream infile9;
	infile9.open (predFile);
	string value; 
	int i = 0;
	while (std::getline(infile9, value)) {
		patternCollection[i].patternCost = stof(value.c_str());
		i++;
	}
	char* finalFile = "../finalset/final";
	infile2.open (finalFile);
	vector<Pattern> finalpatternCollection= vector<Pattern>();
	
	while (std::getline(infile2, line) && !line.empty())
	{
		size_t lastColon=line.substr(line.find("- ")+2).find_last_of(':');
		std::string patternTxt=line.substr(0,lastColon).c_str();
		Pattern p=Pattern(stoi(line.substr(0, line.find("- "))), patternTxt);
		std::string patternLoc=line.substr(lastColon+1).c_str();
		std::stringstream s(patternLoc);
		char ignoreChar;
		while(s>>ignoreChar){
			std::string tripletstr;
			s>>tripletstr;
			Triplet t=Triplet(tripletstr);
			p.occurances.push_back(t);
			s>>ignoreChar;
		}
		//cout<<endl;
//		cout<< patternLoc<<endl;
        //cout<<p.tostring();
		finalpatternCollection.push_back(p);

	}
/* ************************************************END************************************************************************
 * */

	int PATTERNCOUNT=patternCollection.size();
	n = PATTERNCOUNT;
	time_t start, end;
	double timetaken;
	// use SVM margin as f(X)
	vector<double> costList;
	for(int i = 0; i < n; ++i){
		double c=0;
		c=patternCollection[i].getModularCost();
		costList.push_back(c);
	}

	for (int i = 0; i < n; ++i){
		groundSet.insert(i);
	}
	std::unordered_set<int> optSet;
	unordered_set<int> randomSet;

	patternCoverage pC(groundSet,patternCollection,bitCorpus,finalpatternCollection);
	double maxCoverageMagnitude= pC.eval(groundSet);
	scaleSubmodularFunctions pCscaled(groundSet, &pC, 1/maxCoverageMagnitude);


	std::unordered_set<int> emptySet = unordered_set<int>();
	std::unordered_set<int> newemptySet = unordered_set<int>();

	start = time(0);

	int verbosity=atof(argv[2]);
	//optSet = localSearchMax(fComb,emptySet,verbosity);
	double cover = atof(argv[1]);
	//double fraction = atof(argv[1]);
	//double budget=n*fraction; //fraction is 0.9 then 1000*0.9 = 900
	//optSet = lazyGreedyMax(pCscaled, costList, budget, newemptySet, verbosity);
	optSet = lazyGreedySC(pCscaled, costList, cover, verbosity, 1);
	//optSet = localSearchMax(newfComb,newemptySet,verbosity);
	end = time(0);
	timetaken = difftime(end, start);

	unordered_set<int>::const_iterator it;
	for( it = optSet.begin(); it != optSet.end(); it++ ){
		//cout <<patternCollection[*it].patternText<<" : "<<patternCollection[*it].occurances.size()<<endl;
		//cout <<patternCollection[*it].patternText<<endl;
		cout << patternCollection[*it].patternid << "- ";
		cout << patternCollection[*it].patternText <<" : ";
		for(int i=0;i<patternCollection[*it].occurances.size();i++)
		{
			Triplet triple = patternCollection[*it].occurances[i];
			cout << "( " << triple.sid <<","<<triple.beginPos<<","<<triple.endPos<<" ) ";
		}
		cout << endl;
	}
}
