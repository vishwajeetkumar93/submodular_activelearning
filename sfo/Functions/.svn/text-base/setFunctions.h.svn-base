/*
 *	Abstract base class for implementing (not necessarily submodular) set functions. This implements all oracle functions of general set functions.
	Author: Rishabh Iyer
	Melodi Lab, University of Washington, Seattle
 *
 */
 
class setFunctions{
	//protected:
	protected:
	int n; // Ground set size
	unordered_set<int>& groundSet; // GroundSet
	double preComputedVal; // preComputed value f(X) for a set X.
	public:
	// Constructors
	//setFunctions(){}
	setFunctions(unordered_set<int>& groundSet): n(groundSet.size()), groundSet(groundSet){preComputedVal = 0;}
	virtual ~setFunctions(){}
	// Get size and GroundSet
	virtual int getSize(){return n;}
	virtual unordered_set<int>& getGroundSet(){return groundSet;}

	// evaluation based functions
	virtual double eval(const unordered_set<int>& sset){return 0;} // Evaluate the function at sset, i.e f(sset)
	virtual double evalFast(const unordered_set<int>& sset, bool safe = 0){return preComputedVal;} // Evaluate the function at sset, i.e f(sset) using preComputed stats
	virtual double evalGainsadd(const unordered_set<int>& sset, int item){unordered_set<int> asset(sset); asset.insert(item); return eval(asset) - eval(sset);} // evaulate the gain of adding item to sset, i.e f(item | sset)
	virtual double evalGainsremove(const unordered_set<int>& sset, int item){unordered_set<int> rsset(sset); rsset.erase(item); return eval(sset) - eval(rsset);} // evaluate the gain of removing item from sset, i.e f(item | sset/item) 
	virtual double evalGainsaddFast(const unordered_set<int>& sset, int item, bool safe = 0){return evalGainsadd(sset, item);} // fast evaluation of gain of adding item to sset, using preComputed statistics. 
	virtual double evalGainsremoveFast(const unordered_set<int>& sset, int item, bool safe = 0){return evalGainsremove(sset, item);} // fast evaluation of gain of adding item to sset, using preComputed statistics.
	
	// Functions managing preComputed statistics.
	virtual void updateStatisticsAdd(const unordered_set<int>& sset, int item, bool safe = 0){preComputedVal += evalGainsaddFast(sset, item, safe);} // updating 'statistics' in sequential algorithms for adding elements. This is really useful for any algorithm which creates a sequence os sets X_0 \subset X_1 \subset .. X_n such that |X_i| = |X_{i-1}| + 1. 
	virtual void updateStatisticsRemove(const unordered_set<int>& sset, int item, bool safe = 0){preComputedVal -= evalGainsremoveFast(sset, item, safe);} // updating 'statistics' in sequential algorithms for removing elements. This is really useful for any algorithm which creates a sequence os sets X_0 \subset X_1 \subset .. X_n such that |X_i| = |X_{i-1}| + 1. 
	virtual void clearpreCompute(){preComputedVal = 0;} // Clears the precompute statistics.
	virtual void setpreCompute(const unordered_set<int>& sset){preComputedVal = eval(sset);} // Compute the precomputed statistics for a given set.
};


