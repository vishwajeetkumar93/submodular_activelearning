
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
#include <cmath>
// Utilities
#include "Utils/error.h"
#include "Utils/sort.h"
#include "Utils/utils.h"
#include "Utils/fileHandling.h"
#include "Utils/totalOrder.h"
#include "Utils/pattern.h"


#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock



int main(){
char* patternFile = "Data/pattern";
    ifstream infile2;
	infile2.open (patternFile);
	vector<Pattern> patternCollection= vector<Pattern>();
	string line;
	int requiredCount=100;
	long totalInstances=0;
	long totalPattern=0;
	srand (time(NULL));
	
	while (getline(infile2, line))
	{
		
		size_t lastColon=line.find_last_of(':');
		string patternTxt=line.substr(0,lastColon).c_str();
		Pattern p=Pattern(patternTxt);
		string patternLoc=line.substr(lastColon+1).c_str();
		stringstream s(patternLoc);
		char ignoreChar;
		while(s>>ignoreChar){
			string tripletstr;
			s>>tripletstr;
			//cout<<" Triplet "<< tripletstr<<"\t";
			Triplet t=Triplet(tripletstr);
			p.occurances.push_back(t);
			s>>ignoreChar;
			totalInstances++;
		}
		totalPattern++;
		//cout<<endl;
		//cout<< patternLoc<<endl;
        //cout<<p.tostring();
		patternCollection.push_back(p);
	}
	infile2.close();

    
	vector<int> pList;
	for(int i=0;i<totalPattern;i++)pList.push_back(i);
	//cout<<"size of pList is +"<< pList.size() <<"\n";
  // obtain a time-based seed:
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

  shuffle (pList.begin(), pList.end(), std::default_random_engine(seed));


	long instanceCount=0;
	
	int randNumber;
	cout<<"enter required instance : ";
	cin>>requiredCount;
	if(requiredCount>totalInstances){
		cout << "requiredCount is greater than totalInstances count"<<endl;
		return 1;
	}
	int i=0;
	while(instanceCount<requiredCount){
		randNumber=pList[i];
		Pattern p=patternCollection[randNumber];
		cout << p.tostring() << endl;
		instanceCount=instanceCount+p.occurances.size();
		i++;
	}
	
}
