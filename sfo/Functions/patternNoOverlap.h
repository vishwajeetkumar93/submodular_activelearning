/*
 *
 * Author: Pankaj Singh
 * Email: pr.pankajsingh@gmail.com
 * IIT Bombay, Mumbai
 * NOTE: here pattern id 1 is mapped to pattern id 0 (as index starts from 0)
 *
 *
 * */
class patternNoOverlap: public submodularFunctions{
	protected:
	vector<vector <int>> patternsData; // The matrix contains the occurence information of each patterns here patternID can be infered by adding 1 to index (ie patterns are stored from 0 index )
	vector<int> preCompute; // Precomputed statistics of length equal to total number of words in file. For a given set X, preCompute[i] is int value stating the number of times that word is being covered by this set. This preComputed statistics is used in several algorithms for speed ups.
	vector<int> pattern2Length; // this stores the length of pattern
	int sizepreCompute;// size of the precompute statistics (in this case, length of total words WORDCOUNT).
	unordered_set<int> preComputeSet; // This points to the preComputed Set for which the statistics p_X is calculated.
	public:
	// Functions
	patternNoOverlap(std::unordered_set<int>& groundSet,int wordCount, char* graphFilet);
	patternNoOverlap(std::unordered_set<int>& groundSet,int wordCount, vector<vector <int>> kernel,vector<int> pattern2Length);
	void readfromFile(char* graphFile);
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
	void testDisplay();
	virtual ~patternNoOverlap(){}
};

patternNoOverlap::patternNoOverlap(std::unordered_set<int>& groundSet,int w, char* graphFile): submodularFunctions(groundSet){
	readfromFile(graphFile);
	sizepreCompute = w;
	preCompute.assign(sizepreCompute, 0); //precomputed statistics for speeding up greedy
}

patternNoOverlap::patternNoOverlap(std::unordered_set<int>& groundSet,int w, vector<vector <int>> kernel,vector<int> pattern2Length) : submodularFunctions(groundSet), patternsData(kernel),pattern2Length(pattern2Length){
	sizepreCompute = w;
	preCompute.assign(sizepreCompute, 0); //precomputed statistics for speeding up greedy
}
void patternNoOverlap::readfromFile(char* graphFile){
	patternsData.resize(n);
	pattern2Length.resize(n);
	string tmp;
	ifstream iFile;
	int count = 0; // row count
	FILE* fp;
	double tmpd; float tmpf;
	//printf("Loading pattern data file from %s... for the pattern overlap Function...\n",graphFile);
	if (!(fp=fopen(graphFile,"rb"))){
		printf("ERROR: cannot open file %s\n",graphFile);
	}
	int nRow = n;
	int freq=0;
	int pLen=0;
	int  tmpi=0;
	for (int i = 0; i < nRow; i++){
	fscanf(fp,"%d",&freq);
	patternsData[i].resize(freq);
	fscanf(fp,"%d",&pLen);
	pattern2Length[i]=pLen;
		for(int j=0;j<freq;j++){
			fscanf(fp,"%d",&tmpi);
			patternsData[i][j]=tmpi;
		}
	}
	fclose(fp);
}


void patternNoOverlap::testDisplay(){
	for(int i=0;i<n;i++){
		int freq=patternsData[i].size();
		cout <<"pattern is "<<i<<endl;
		cout <<"freq "<<freq<<endl;
		for(int j=0;j<freq;j++)
		cout << patternsData[i][j]<<"\t";
		cout << "\n";
	}
}

double patternNoOverlap::eval(const unordered_set<int>& sset){
	double pSum=0;
	int pLen=0;
	int i=0;
	int tmp;
	vector<int> wordList(sizepreCompute,0);
	unordered_set<int>::const_iterator it;
	for( it = sset.begin(); it != sset.end(); it++ ){
		pLen=pattern2Length[*it];
		for(std::vector<int>::iterator itt = patternsData[*it].begin(); itt != patternsData[*it].end(); ++itt) {
			tmp=(*itt);
			for(i=0;i<pLen;++i){
	//			pSum=pSum+wordList[tmp+i];   def. of overlap is changed
				++wordList[tmp+i];
			}
		}
	}

	for(i=0;i<sizepreCompute;++i){
		if(wordList[i]==1){
			pSum++;
		}
	}
	return pSum;
}

double patternNoOverlap::evalFast(const unordered_set<int>& sset, bool safe=0){
	int pSum=0;
	if(safe == 1){
		if(sset != preComputeSet)
		setpreCompute(sset);
		}
	int count=0;
	for(int i=0;i<sizepreCompute;++i){
		if(preCompute[i]==1){
			pSum=pSum+1;
		}
	}
	return pSum;
	}

double patternNoOverlap::evalGainsadd(const unordered_set<int>& sset, int item){
	int count = 0;
	double sum1=0;
	double sum2=0;
	double pSum=0;
	int pLen=0;
	int i=0;
	int tmp;
	vector<int> wordList(sizepreCompute,0);
	unordered_set<int>::const_iterator it;
	for( it = sset.begin(); it != sset.end(); it++ ){
		pLen=pattern2Length[*it];
		for(std::vector<int>::iterator itt = patternsData[*it].begin(); itt != patternsData[*it].end(); ++itt) {
			tmp=(*itt);
			for(i=0;i<pLen;++i){
				++wordList[tmp+i];
			}
		}
	}

	for(i=0;i<sizepreCompute;++i){
		if(wordList[i]==1){
			pSum=pSum+1;
		}
	}
	sum1=pSum;
	pSum=0;
	pLen=pattern2Length[item];
	for(std::vector<int>::iterator itt = patternsData[item].begin(); itt != patternsData[item].end(); ++itt) {
			//tmp=patternsData[item][*itt];
			tmp=(*itt);
			for(i=0;i<pLen;++i){
				++wordList[tmp+i];
			}
		}
	for(i=0;i<sizepreCompute;++i)
	{
		if(wordList[i]==1){
			pSum=pSum+1;
			}
	}
		sum2=pSum;
	return (sum2-sum1);
}

double patternNoOverlap::evalGainsremove(const unordered_set<int>& sset, int item){
		if(sset.find(item) == sset.end()){
		cout<<"Error in using evalGainsremove: the provided item does not belong to the subset\n";
		return 0;
	}
	// remove item from local copy of sset
	unordered_set<int> sset_local(sset);
	sset_local.erase(item);
	int count = 0;
	double sum1=0;
	double sum2=0;
	double pSum=0;
	int pLen=0;
	int i=0;
	int tmp;
	vector<int> wordList(sizepreCompute,0);
	unordered_set<int>::const_iterator it;
	for( it = sset_local.begin(); it != sset_local.end(); it++ ){
		pLen=pattern2Length[*it];
		for(std::vector<int>::iterator itt = patternsData[*it].begin(); itt != patternsData[*it].end(); ++itt) {
			tmp=(*itt);
			for(i=0;i<pLen;++i){
				++wordList[tmp+i];
			}
		}
	}

	for(i=0;i<sizepreCompute;++i){
		if(wordList[i]==1){
			pSum=pSum+1;
		}
	}
	sum1=pSum;
	pSum=0;
	pLen=pattern2Length[item];
	for(std::vector<int>::iterator itt = patternsData[item].begin(); itt != patternsData[item].end(); ++itt) {
			//tmp=patternsData[item][*itt];
			tmp=(*itt);
			for(i=0;i<pLen;++i){
				++wordList[tmp+i];
			}
		}
	for(i=0;i<sizepreCompute;++i)
	{
		if(wordList[i]==1){
			pSum=pSum+1;
			}
	}
		sum2=pSum;
	return (sum1-sum2);
}

double patternNoOverlap::evalGainsaddFast(const unordered_set<int>& sset, int item, bool safe=0){
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
	int tmp,i;
	int pLen=pattern2Length[item];
	int count=0;
	for(std::vector<int>::iterator itt = patternsData[item].begin(); itt != patternsData[item].end(); ++itt) {
			//tmp=patternsData[item][*itt];
			tmp=(*itt);
			for(i=0;i<pLen;++i){
				if(preCompute[tmp+i]==0)++count;
			}
		}
	return -(double(count));

}

double patternNoOverlap::evalGainsremoveFast(const unordered_set<int>& sset, int item, bool safe=0){
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

	int count=0;
	int tmp,i;
	int pLen=pattern2Length[item];
	for(std::vector<int>::iterator itt = patternsData[item].begin(); itt != patternsData[item].end(); ++itt) {
			//tmp=patternsData[item][*itt];
			tmp=(*itt);
			for(i=0;i<pLen;++i){
				//count only those which are not covered by existing set.
				if(preCompute[tmp+i]==1)--count;
			}
		}
	return -(double(count));
}

void patternNoOverlap::updateStatisticsAdd(const unordered_set<int>& sset, int item, bool safe=0){
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

	int tmp,i;
	int pLen=pattern2Length[item];
	for(std::vector<int>::iterator itt = patternsData[item].begin(); itt != patternsData[item].end(); ++itt) {
			//tmp=patternsData[item][*itt];
			tmp=(*itt);
			for(i=0;i<pLen;++i){
				++preCompute[tmp+i];
			}
		}

	preComputeSet.insert(item);
}

void patternNoOverlap::updateStatisticsRemove(const unordered_set<int>& sset, int item, bool safe=0){
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

	int tmp,i;
	int pLen=pattern2Length[item];
	for(std::vector<int>::iterator itt = patternsData[item].begin(); itt != patternsData[item].end(); ++itt) {
			//tmp=patternsData[item][*itt];
			tmp=(*itt);
			for(i=0;i<pLen;++i){
				--preCompute[tmp+i];
			}
		}
	preComputeSet.erase(item);
}

void patternNoOverlap::setpreCompute(const unordered_set<int>& sset){
	clearpreCompute();
	unordered_set<int>::const_iterator it;
	for( it = sset.begin(); it != sset.end(); it++ ){
		updateStatisticsAdd(sset, *it);
	}
}

void patternNoOverlap::clearpreCompute(){
	for (int i = 0; i < sizepreCompute; i++) { preCompute[i] = 0; }
	preComputeSet.clear();
}
