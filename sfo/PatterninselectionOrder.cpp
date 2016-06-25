#include<stdio.h>
#include<iostream>
#include<vector>


#include "Utils/pattern.h"

using namespace std;

const char* orderedPatternIdFile="patternIds.txt";
const char* patternFile="Data/pattern";

vector<Pattern> patternCollection;

int main()
{
    ifstream infile2;
	infile2.open (patternFile);
	
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
			Triplet t=Triplet(tripletstr);
			p.occurances.push_back(t);
			s>>ignoreChar;
		}
		patternCollection.push_back(p);
	}
	infile2.close();
	//cout << patternCollection[0].patternText;
	
	ifstream infile;
	infile.open(orderedPatternIdFile);
	
	ofstream offile;
	offile.open("orderedpattern.txt");
	
	while(getline(infile,line))
	{
		//cout << line << endl;
		int id=stoi(line);
		//cout << id << endl;
		Pattern p=patternCollection[id];
		offile << p.patternText << " " << p.occurances.size() << endl;
	}
	offile.close();
	return 0;
}
