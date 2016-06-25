/*
 *	Header file for defining a family of graph cut based functions: f(X) = \sum_{i \in X} \sum_{j \in V} s_{ij} - \lambda \sum_{i, j \in X} s_{ij}
	Assumes that the kernel S is symmetric.
	Author: Rishabh Iyer 
	Email: rkiyer@uw.edu
	Melodi Lab, University of Washington, Seattle
 *
 */


class graphCutFunctions: public submodularFunctions{
	public:
	vector<vector <float>> kernel; // The matrix s_{ij}_{i \in V, j \in V}
	vector<double> preCompute; // stores p_X(j) = \sum_{i \in X} s_{ij}, for a given X.
	vector<double> modularscores; // a Precomputed quantity: modularscores[i] = sum_{i \in V} s_{ij}.
	double lambda;
	int sizepreCompute;
	// Functions
	graphCutFunctions(std::unordered_set<int>& groundset, char* file, double lambdaval);//read from file
	void readKernelfromFile(char* graphFile);
	double eval(const unordered_set<int>& sset);
	double evalFast(const unordered_set<int>& sset, bool safe);
	double evalGainsadd(const unordered_set<int>& sset, int item); 
	double evalGainsremove(const unordered_set<int>& sset, int item);
	double evalGainsaddFast(const unordered_set<int>& sset, int item, bool safe); 
	double evalGainsremoveFast(const unordered_set<int>& sset, int item, bool safe);
	void updateStatisticsAdd(const unordered_set<int>& sset, int item, bool safe);
	void updateStatisticsRemove(const unordered_set<int>& sset, int item, bool safe);
	void setpreCompute(const unordered_set<int>& sset);
	void clearpreCompute();
	double currentCoverage() {return 0.0;}
	void initializeFinalSet() {}
};

graphCutFunctions::graphCutFunctions(std::unordered_set<int>& groundSet, char* graphFile, double lambda): submodularFunctions(groundSet), lambda(lambda){
	kernel.resize(n);
	for(int i=0;i<n;i++) {kernel[i].resize(n);}
	readKernelfromFile(graphFile);
	sizepreCompute = n;
	preCompute.resize(sizepreCompute); //precomputed statistics for speeding up greedy
	for (int i = 0; i < sizepreCompute; i++) { preCompute[i] = 0; }
	modularscores.resize(n);
    	for(int i = 0; i < n; i++){
    		modularscores[i] = 0;
    		for(int j = 0; j < n; j++){
    			modularscores[i] +=kernel[i][j];
    		}
    	}
}
// A specific implementation to read a binary matrix stored as double floats.
void graphCutFunctions::readKernelfromFile(char* graphFile){
	int unitsize=4;
	string tmp;
	ifstream iFile;	
	int count = 0; // row count
	FILE* fp;
	double tmpd; float tmpf;
	printf("Loading graph from %s...\n",graphFile);
	if (!(fp=fopen(graphFile,"rb"))){
		printf("ERROR: cannot open file %s",graphFile);
	}
	int nRow = long(GetFileSize(graphFile)/n)/unitsize;
	for (int i = 0; i < nRow; i++){
		for (int j = 0; j < n; j++){
			if (unitsize == 8) {
				fread(&tmpd,unitsize,1,fp);
				kernel[count+i][j] = tmpd;
			}
			else {
				fread(&tmpf,unitsize,1,fp);
				kernel[count+i][j] = tmpf;
			}				
		}
	}
	count += nRow;
	fclose(fp);
}

double graphCutFunctions::eval(const unordered_set<int>& sset){
// Evaluation of function valuation.
	double sum = 0;
	for( unordered_set<int>::const_iterator it = sset.begin(); it != sset.end(); it++ ) {
		sum+=modularscores[*it];
		for( unordered_set<int>::const_iterator it2 = sset.begin(); it2!= sset.end(); it2++){
			sum-= lambda*kernel[*it][*it2];
		}
	}
    	return sum;
}

double graphCutFunctions::evalFast(const unordered_set<int>& sset, bool safe = 0){
	double sum = 0;
	unordered_set<int>::const_iterator it;
	for( it = sset.begin(); it != sset.end(); it++ ) {
		sum+= modularscores[*it] - lambda*preCompute[*it];
	}
    	return sum;
}

double graphCutFunctions::evalGainsadd(const unordered_set<int>& sset, int item){
	if(sset.find(item) != sset.end()){
		cout<<"Warning: the provided item belongs to the subset\n";
		return 0;
	}
	double gains = modularscores[item];
	unordered_set<int>::const_iterator it;
	for (it = sset.begin(); it != sset.end(); it++){
		gains-= lambda*(kernel[item][*it] + kernel[*it][item]);
	}
	gains-= lambda*kernel[item][item];
	return gains;
} 

double graphCutFunctions::evalGainsremove(const unordered_set<int>& sset, int item){
	if(sset.find(item) == sset.end()){
		cout<<"Warning: the provided item does not belong to the subset\n";
		return 0;
	}
	double gains = modularscores[item];
	unordered_set<int>::const_iterator it;
	for (it = sset.begin(); it != sset.end(); it++){
		if(*it != item)
			gains-= lambda*(kernel[item][*it] + kernel[*it][item]);
	}
	gains-= lambda*kernel[item][item];
	return gains;
} 

double graphCutFunctions::evalGainsaddFast(const unordered_set<int>& sset, int item, bool safe = 0){
// Fast evaluation of Adding gains using the precomputed statistics. This is used, for example, in the implementation of the forward greedy algorithm.
// For the sake of speed, we do not check if item does not belong to the subset. You should check this before calling this function.
	return modularscores[item] - 2*lambda*preCompute[item] - lambda*kernel[item][item];
} 

double graphCutFunctions::evalGainsremoveFast(const unordered_set<int>& sset, int item, bool safe = 0){
// Fast evaluation of Removing gains using the precomputed statistics. This is used, for example, in the implementation of the reverse greedy algorithm.
// For the sake of speed, we do not check if item belong to the subset. You should check this before calling this function.

	return modularscores[item] - 2*lambda*preCompute[item] + lambda*kernel[item][item];
} 

void graphCutFunctions::updateStatisticsAdd(const unordered_set<int>& sset, int item, bool safe = 0){
	for (int i = 1; i < n; i++)
		preCompute[i]+=kernel[i][item];
}

void graphCutFunctions::updateStatisticsRemove(const unordered_set<int>& sset, int item, bool safe = 0){
	for (int i = 1; i < n; i++)
		preCompute[i]-=kernel[i][item];
}

void graphCutFunctions::setpreCompute(const unordered_set<int>& sset){
	clearpreCompute();
	unordered_set<int>::const_iterator it;
	for( it = sset.begin(); it != sset.end(); it++ ){
		updateStatisticsAdd(sset, *it);
	}
}

void graphCutFunctions::clearpreCompute(){
	for (int i = 0; i < sizepreCompute; i++) { preCompute[i] = 0; }
}

