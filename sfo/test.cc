/*
 *	A tutorial for SOTK.
 *	Author: Pankaj Singh
 *	Email: pr.pankajsingh@gmail.com
 *
 * */

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
#include "Functions/patternOverlap.h"
#include "Functions/combineSubmodularFunctions.h"
#include "Functions/scaleSubmodularFunctions.h"
#include "Functions/modularFunctions.h"
#include "Functions/translateSubmodularFunctions.h"
#include "Functions/combineSubmodularModularFunctions.h"


//Algorithms
#include "Algorithms/lazyGreedyMax.h"
#include "Algorithms/randomizedGreedyMax.h"
#include "Algorithms/bidirGreedyMax.h"
#include "Algorithms/naiveGreedyMax.h"
#include "Algorithms/revlazyGreedyMax.h"
#include "Algorithms/localSearchMax.h"

#define SMALLEST_NUMBER -1e70
#define LARGEST_NUMBER 1e70

/*
class Triplet{
	public:
	int instanceId;
	int sid;
	int beginPos;
	int endPos;
	//extract ids from comma separated string e.g. 280,15,21 
	Triplet(string str)
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
	string tostring()
	{
		return std::to_string(sid)+","+std::to_string(beginPos)+","+std::to_string(endPos);
	}

};

class Pattern{
	public:
	std::string patternText;
	vector<Triplet> occurances;
	Pattern(string str){
		patternText=str;
		occurances = vector<Triplet>();
	}
	string tostring()
	{
		std::string str=patternText;
		for(std::vector<Triplet>::iterator it = occurances.begin(); it != occurances.end(); ++it) {
			str=str+" "+(*it).tostring();
		}
		return str;
	}

};
**/
int main(int argc, char** argv){
	//Testing of config file......
	/******************************************************************************************************/

	/******************************************************************************************************/
	//if(argc != 6)
	//{
		//cout << "\n\nProvided "<<(argc-1) << " parameters\n\n";
		//cout << "usage : "<<argv[0]<<" Coverageweight OverlapWeight patternRelevanceWeight fraction verbosity\n";
		//exit(1);
	//}

	int n; // GroundSet size
	unordered_set<int> groundSet = unordered_set<int>(); // GroundSet.
	unordered_set<int> emptyset = unordered_set<int>();
	int numWords;
/* ***********************************************START************************************************************************
 * */

// LOAD CORPUS

	char* corpusFile = "Data/corpusFile.txt";
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
  cout << "There are "<<total_word<<" words in the corpus\n";

//LOAD PATTERN  sample entry(  NP NP aforesaid by CAT22208 : ( 280,15,21 ) ( 501,15,21 ) )

	char* patternFile = "Data/pattern";
    ifstream infile2;
	infile2.open (patternFile);
	vector<Pattern> patternCollection= vector<Pattern>();
	
	std::string line;
	while (std::getline(infile2, line))
	{
		
		size_t lastColon=line.find_last_of(':');
		std::string patternTxt=line.substr(0,lastColon).c_str();
		Pattern p=Pattern(patternTxt);
		std::string patternLoc=line.substr(lastColon+1).c_str();
		std::stringstream s(patternLoc);
		char ignoreChar;
		while(s>>ignoreChar){
			std::string tripletstr;
			s>>tripletstr;
			//cout<<" Triplet "<< tripletstr<<"\t";
			Triplet t=Triplet(tripletstr);
			p.occurances.push_back(t);
			s>>ignoreChar;
		}
		//cout<<endl;
//		//cout<< patternLoc<<endl;
        //cout<<p.tostring();
		patternCollection.push_back(p);
	}
	infile2.close();

/* ************************************************END************************************************************************
 * */
	int PATTERNCOUNT=patternCollection.size();
	cout <<"there are "+to_string(PATTERNCOUNT)+" patterns \n";	
	int WORDCOUNT=71732;
	n = PATTERNCOUNT;
	time_t start, end;
	double timetaken;
	for (int i = 0; i < n; ++i){
		groundSet.insert(i);
	}
	// currently all patterns are equally costly
	vector<double> costList(n, 1);
	std::unordered_set<int> optSet;
	unordered_set<int> randomSet;


	patternCoverage pC(groundSet,patternCollection,bitCorpus);
	cout << "pattern coverage function has been intialised\n";
	double maxCoverageMagnitude= pC.eval(groundSet);
	cout << "Maximum coverage is : "<<maxCoverageMagnitude<<endl;

}
