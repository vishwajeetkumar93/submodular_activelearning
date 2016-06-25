#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <set>
#include <algorithm>
using namespace std;
#include <string.h>
#include "utils.h"
#include "error.h"


int checkline(char *line,unsigned num) {

  if (strlen(line) == MAXLEN && line[strlen(line)-1] != '\n') {
    error("Line %li too long\n",num);
  }
  else {
    line[strlen(line)-1] = '\0';
  }
  return strlen(line);
}

void printset(std::set<int> sset){
	set<int>::iterator it;
	for( it = sset.begin(); it != sset.end(); it++ ) {
		cout<<*it<<" ";
	}              	
	cout<<"\n";
}



	
  

  
  

