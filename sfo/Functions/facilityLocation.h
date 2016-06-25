/*
 *	Header file for defining the facility location function: f(X) = \sum_{i \in V} \max_{j \in X} s_{ij}.
	Assumes that the kernel S is symmetric.
	Author: Rishabh Iyer.
	Email: rkiyer@uw.edu
	Melodi Lab, University of Washington, Seattle
 *
 */
class facilityLocation: public submodularFunctions{
	protected:
	vector<vector <float>> kernel; // The matrix s_{ij}_{i \in V, j \in V}
	vector<double> preCompute; // Precomputed statistics of length 2*n. For a given set X, preCompute[i] = max_{j \in X} s_{ij} and preCompute[n+i] = 2max_{j \in X} s_{ij}, where 2max stands for the second max. This preComputed statistics is used in several algorithms for speed ups.
	int sizepreCompute;// size of the precompute statistics (in this case, 2*n).
	unordered_set<int> preComputeSet; // This points to the preComputed Set for which the statistics p_X is calculated.
	public:
	// Functions
	facilityLocation(std::unordered_set<int>& groundSet, char* graphFilet);
	facilityLocation(std::unordered_set<int>& groundSet, vector<vector <float>> kernel);
	void readKernelfromFile(char* graphFile);
	double eval(const unordered_set<int>& sset);
	double evalFast(const unordered_set<int>& sset, bool safe);
	double evalGainsadd(const unordered_set<int>& sset, int item);
	double evalGainsremove(const unordered_set<int>& sset, int item);
	double evalGainsaddFast(const unordered_set<int>& sset, int item, bool safe);
	double evalGainsremoveFast(const unordered_set<int>& sset, int item, bool safe);
	void updateStatisticsAdd(const unordered_set<int>& sset, int item, bool safe);
	void updateStatisticsRemove(const unordered_set<int>& sset, int item, bool safe);
	void setpreCompute(const unordered_set<int>& ssett);
	void clearpreCompute();
	double currentCoverage() {return 0.0;}
	void initializeFinalSet() {}
};

facilityLocation::facilityLocation(std::unordered_set<int>& groundSet, char* graphFile): submodularFunctions(groundSet){
	readKernelfromFile(graphFile);
	sizepreCompute = 2*n;
	preCompute.assign(sizepreCompute, 0); //precomputed statistics for speeding up greedy
}

facilityLocation::facilityLocation(std::unordered_set<int>& groundSet, vector<vector <float>> kernel) : submodularFunctions(groundSet), kernel(kernel){
	sizepreCompute = 2*n;
	preCompute.assign(sizepreCompute, 0); //precomputed statistics for speeding up greedy
}
// A specific implementation to read a binary matrix stored as double floats.
void facilityLocation::readKernelfromFile(char* graphFile){
	int unitsize=4;
	kernel.resize(n);
	for(int i=0;i<n;i++) {kernel[i].resize(n);}
	string tmp;
	ifstream iFile;	
	int count = 0; // row count
	FILE* fp;
	double tmpd; float tmpf;
	printf("Loading graph from %s... for the Facility Location Function...\n",graphFile);
	if (!(fp=fopen(graphFile,"rb"))){
		printf("ERROR: cannot open file %s\n",graphFile);
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

double facilityLocation::eval(const unordered_set<int>& sset){
	double sum = 0;
	double maxvali;
	unordered_set<int>::const_iterator it;
	for(int i=0;i<n;i++){
		maxvali = 0;
		for( it = sset.begin(); it != sset.end(); it++ ){
			if(kernel[i][*it] > maxvali){
				maxvali = kernel[i][*it];
			}
		}
		sum+=maxvali;
	}
	return sum;
}

double facilityLocation::evalFast(const unordered_set<int>& sset, bool safe = 0){
	if(safe == 1){
		if(sset != preComputeSet)
			setpreCompute(sset);
	}
	double sum=0;
	for(int i=0;i<n;i++){
		sum+=preCompute[i];
	}
	return sum;	
}

double facilityLocation::evalGainsadd(const unordered_set<int>& sset, int item){
	//if(sset.find(item) != sset.end()){
	//%	cout<<"Error in using evalGainsadd: the provided item already belongs to the subset\n";
	//	return 0;
	//}
	double gains = 0;
	unordered_set<int>::const_iterator it;
	for(int i=0;i<n;i++){
		double maxvali= 0;
		for( it = sset.begin(); it != sset.end(); it++ ){
			if(kernel[i][*it] > maxvali){
				maxvali = kernel[i][*it];
			}
		}
		if (maxvali < kernel[i][item]){
			gains+=(kernel[i][item]-maxvali);
		}
	}
	return gains;
}

double facilityLocation::evalGainsremove(const unordered_set<int>& sset, int item){
	if(sset.find(item) == sset.end()){
		cout<<"Error in using evalGainsremove: the provided item does not belong to the subset\n";
		return 0;
	}
	double sum = 0;
	double sumd = 0;
	double maxvali;
	unordered_set<int>::const_iterator it;
	for(int i=0;i<n;i++){
		maxvali = 0;
		for( it = sset.begin(); it != sset.end(); it++ ){
			if(kernel[i][*it] > maxvali){
				maxvali = kernel[i][*it];
			}
		}
		sum+=maxvali;
		if(*it != item){ sumd+=maxvali;}
	}
	return (sum - sumd);
}

double facilityLocation::evalGainsaddFast(const unordered_set<int>& sset, int item, bool safe = 0){
// Fast evaluation of Adding gains using the precomputed statistics. This is used, for example, in the implementation of the forward greedy algorithm.
// The safety parameter 'safe' controls the safety and speed of this procedure. If not sure how to use it, set it to it's default value, viz. 1
// If safe = 0, (for the sake of speed) we do not check if item does not belong to the subset. We also do not check if the preComputed statistics actually points to the set sset. You would need to check both these.
	if (safe == 1){
		if(sset.find(item) != sset.end()){
			cout<<"Error in using evalGainsaddFast: the provided item already belongs to the subset\n";
			return 0;
		}
		if(sset != preComputeSet)
			setpreCompute(sset);
	}
	double gains=0;
	for(int i=0;i<n;i++){
		if(preCompute[i]<kernel[item][i])
			gains+=(kernel[item][i]-preCompute[i]);
	}
	return gains;	
}

double facilityLocation::evalGainsremoveFast(const unordered_set<int>& sset, int item, bool safe = 0){
// Fast evaluation of Removing gains using the precomputed statistics. This is used, for example, in the implementation of the reverse greedy algorithm.
// The safety parameter 'safe' controls the safety and speed of this procedure. If not sure how to use it, set it to it's default value, viz. 1
// If safe = 0, (for the sake of speed) we do not check if item belong to the subset. We also do not check if the preComputed statistics actually points to the set sset. You would need to check both these.
	if (safe == 1){
		if(sset.find(item) == sset.end()){
			cout<<"Error in using evalGainsremoveFast: the provided item does not belong to the subset\n";
			return 0;
		}
		if(sset != preComputeSet)
			setpreCompute(sset);
	}
	double gains=0;
	for(int i=0;i<n;i++){
		if(preCompute[i]==kernel[item][i])
			gains+=(preCompute[i] - preCompute[n+i]);
		
	}
	return gains;	
}

void facilityLocation::updateStatisticsAdd(const unordered_set<int>& sset, int item, bool safe = 0){
// Update statistics for algorithms sequentially adding elements (for example, the greedy algorithm).
// The safety parameter 'safe' controls the safety and speed of this procedure. If not sure how to use it, set it to it's default value, viz. 1
// If safe = 0, (for the sake of speed) we do not check if item does not belong to the subset. We also do not check if the preComputed statistics actually points to the set sset. You would need to check both these.
	if (safe == 1){
		if(sset.find(item) != sset.end()){
			cout<<"Error in using updateStatisticsAdd: the provided item belongs to the sset already\n";
			return;
		}
		if(sset != preComputeSet)
			setpreCompute(sset);
	}

	for (int i = 0; i < n; i++) {
		if(kernel[i][item]>preCompute[i]){
			preCompute[n+i] = preCompute[i];
			preCompute[i]=kernel[i][item];
		}	
	}
	preComputeSet.insert(item);
}

void facilityLocation::updateStatisticsRemove(const unordered_set<int>& sset, int item, bool safe = 0){
// Update statistics for algorithms sequentially removing elements (for example, the reverse greedy algorithm).
// The safety parameter 'safe' controls the safety and speed of this procedure. If not sure how to use it, set it to it's default value, viz. 1
// If safe = 0, (for the sake of speed) we do not check if item belong to the subset. We also do not check if the preComputed statistics actually points to the set sset. You would need to check both these.
	if (safe == 1){
		if(sset.find(item) == sset.end()){
			cout<<"Error in using updateStatisticsRemove: the provided item does not belong to the subset\n";
			return;
		}
		if(sset != preComputeSet)
			setpreCompute(sset);
	}

	for (int i = 0; i < n; i++) {
		if(kernel[i][item]==preCompute[i]){ // We obtained the largest value, we need to recompute the statistics. Else, it remains the same.
			preCompute[i] = 0; preCompute[n+i] = 0;
			unordered_set<int>::const_iterator it;
			for (it = sset.begin(); it!= sset.end(); it++){
				if((*it != item)&&(kernel[i][*it] > preCompute[i])){
					preCompute[n+i] = preCompute[i];
					preCompute[i] = kernel[i][*it];				
				}
			}
		}	
	}
	preComputeSet.erase(item);
}

void facilityLocation::clearpreCompute(){
	for (int i = 0; i < sizepreCompute; i++) { preCompute[i] = 0; }
	preComputeSet.clear();
	}

void facilityLocation::setpreCompute(const unordered_set<int>& sset){
	clearpreCompute();
	unordered_set<int>::const_iterator it;
	for( it = sset.begin(); it != sset.end(); it++ ){
		updateStatisticsAdd(sset, *it);
	}
}

