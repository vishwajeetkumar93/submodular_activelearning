/*
 *	Header file for defining the saturated coverage function: f(X) = \sum_{i \in V} \min\{\sum_{j \in X} s_{ij}, \alpha \sum_{j \in V} s_{ij}\}.
	Author: Rishabh Iyer.
	Melodi Lab, University of Washington, Seattle
 *
 */
class saturatedCoverage: public submodularFunctions{
	public:
	vector<vector <float>> kernel; // similarity kernel defining the feature based functions (s_{ij})_{i, j \in V} (assumed to be symmetric).
	double alpha; // the threshold alpha
	vector<double> modularthresh; // a Precomputed quantity: modularthresh[i] = sum_{i \in V} s_{ij}.
	vector<double> preCompute; // Precomputed statistics. For a given set X, this stores p_X(i) = \sum_{j \in X} s_{ij}.
	int sizepreCompute;// size of the precompute statistics (in this case, n)
	unordered_set<int> preComputeSet; // This points to the preComputed Set for which the statistics p_X is calculated.
	// Functions
	saturatedCoverage(std::unordered_set<int>& groundset, char* graphFile, double alpha);
	saturatedCoverage(std::unordered_set<int>& groundSet, vector<vector <float>> kernel, double alpha);
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

saturatedCoverage::saturatedCoverage(std::unordered_set<int>& groundSet, char* graphFile, double alpha): submodularFunctions(groundSet), alpha(alpha){
	readKernelfromFile(graphFile);
	sizepreCompute = n;
	preCompute.resize(sizepreCompute); //for speeding up greedy with facility location	
    	for (int i = 0; i < sizepreCompute; i++) { preCompute[i] = 0; }
    	modularthresh.resize(n);
    	for(int i = 0; i < n; i++){
    		modularthresh[i] = 0;
    		for(int j = 0; j < n; j++){
    			modularthresh[i] +=kernel[i][j];
    		}
    	}
}

saturatedCoverage::saturatedCoverage(std::unordered_set<int>& groundSet, vector<vector <float>> kernel, double alpha) : submodularFunctions(groundSet), kernel(kernel), alpha(alpha){
	sizepreCompute = n;
	preCompute.assign(sizepreCompute, 0); //precomputed statistics for speeding up greedy
	for (int i = 0; i < sizepreCompute; i++) { preCompute[i] = 0; }
    	modularthresh.resize(n);
    	for(int i = 0; i < n; i++){
    		modularthresh[i] = 0;
    		for(int j = 0; j < n; j++){
    			modularthresh[i] +=kernel[i][j];
    		}
    	}
}

// A specific implementation to read a binary matrix stored as double floats.
void saturatedCoverage::readKernelfromFile(char* graphFile){
	int unitsize=4;
	kernel.resize(n);
	for(int i=0;i<n;i++) {kernel[i].resize(n);}
	string tmp;
	ifstream iFile;	
	int count = 0; // row count
	FILE* fp;
	double tmpd; float tmpf;
	printf("Loading graph from %s... for the Saturated Coverage Function...\n",graphFile);
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

double saturatedCoverage::eval(const unordered_set<int>& sset){
	double sum = 0;
	double sumvali;
	unordered_set<int>::const_iterator it;
	for(int i=0;i<n;i++){
		sumvali = 0;
		for( it = sset.begin(); it != sset.end(); it++ ){
			sumvali += kernel[i][*it];
		}
		if(sumvali < alpha*modularthresh[i])
			sum+=sumvali;
		else{
			sum+=modularthresh[i];
		}
	}
	return sum;
}

double saturatedCoverage::evalFast(const unordered_set<int>& sset, bool safe = 0){
	if(safe == 1){
		if(sset != preComputeSet)
			setpreCompute(sset);
	}
	double sum=0;
	for(int i=0;i<n;i++){
		if(preCompute[i] < alpha*modularthresh[i]) // the ith component is not saturated?
			sum+=preCompute[i];
		else
			sum+=alpha*modularthresh[i];
	}
	return sum;	
}
double saturatedCoverage::evalGainsadd(const unordered_set<int>& sset, int item){
	//if(sset.find(item) != sset.end()){
	//	cout<<"Error in using evalGainsadd: the provided item already belongs to the subset\n";
	//	return 0;
	//}
	double gains = 0;
	double sumvali;
	unordered_set<int>::const_iterator it;
	for(int i=0;i<n;i++){
		sumvali = 0;
		for( it = sset.begin(); it != sset.end(); it++ ){
			sumvali += kernel[i][*it];
		}
		if(sumvali+kernel[i][item] < alpha*modularthresh[i]){
			gains+=kernel[i][item];
		}
	}
	return gains;
}

double saturatedCoverage::evalGainsremove(const unordered_set<int>& sset, int item){
	if(sset.find(item) == sset.end()){
		cout<<"Error in using evalGainsremove: the provided item does not belong to the subset\n";
		return 0;
	}
	double sum = 0;
	double sumd = 0;
	double sumvali;
	unordered_set<int>::const_iterator it;
	for(int i=0;i<n;i++){
		sumvali = 0;
		for( it = sset.begin(); it != sset.end(); it++ ){
			sumvali += kernel[i][*it];
		}
		if(sumvali < alpha*modularthresh[i])
			sum+=sumvali;
			if (*it != item) sumd+=sumvali;
		else{
			sum+=modularthresh[i];
			if (*it != item) sumd+=sumvali;
		}
	}
	return (sum - sumd);
}

double saturatedCoverage::evalGainsaddFast(const unordered_set<int>& sset, int item, bool safe = 0){
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
	}	double gains=0;
	for(int i=0;i<n;i++){
		if( (preCompute[i] < alpha*modularthresh[i])&& (preCompute[i] + kernel[item][i] > alpha*modularthresh[i])) // adding item just saturates component i
			gains+=kernel[item][i] - preCompute[i] + alpha*modularthresh[i];
		else if(preCompute[i] < alpha*modularthresh[i]) // the ith component is not saturated?
			gains+=kernel[item][i];
	}
	return gains;	
}

double saturatedCoverage::evalGainsremoveFast(const unordered_set<int>& sset, int item, bool safe = 0){
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
		if( (preCompute[i] > alpha*modularthresh[i])&& (preCompute[i] - kernel[item][i] < alpha*modularthresh[i])) // removing item just unsaturates component i
			gains+=kernel[item][i] - preCompute[i] + alpha*modularthresh[i];
		else if (preCompute[i] < alpha*modularthresh[i]) // Component i is already unsaturated
			gains+= kernel[item][i];
	}
	return gains;	
}

void saturatedCoverage::updateStatisticsAdd(const unordered_set<int>& sset, int item, bool safe = 0){
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
		preCompute[i]+=kernel[i][item];
	}
	preComputeSet.insert(item);
}

void saturatedCoverage::updateStatisticsRemove(const unordered_set<int>& sset, int item, bool safe = 0){
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
		preCompute[i]-=kernel[i][item];
	}
	preComputeSet.erase(item);
}

void saturatedCoverage::clearpreCompute(){	
	for (int i = 0; i < sizepreCompute; i++) { preCompute[i] = 0; }
	preComputeSet.clear();
	}

void saturatedCoverage::setpreCompute(const unordered_set<int>& sset){
	clearpreCompute();
	unordered_set<int>::const_iterator it;
	for( it = sset.begin(); it != sset.end(); it++ ){
		updateStatisticsAdd(sset, *it);
	}
}

