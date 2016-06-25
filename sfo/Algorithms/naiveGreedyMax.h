/*
 *	The naive greedy algorithm for set function maximization. This algorithm has guarantees if the function is monotone submodular, under cardinality/knapsack or matroid constraints. In general, though, it is a useful heuristic, which seems to work well in practice.
	Solves the problem \max_{c(X) \leq B} f(X), where f is a monotone submodular function and c is a modular cost function.
	Special case of this is the problem, \max_{|X| \leq k} f(X).
	Author: Rishabh Iyer
	Melodi Lab, University of Washington, Seattle

	Note: This algorithm is considerably slower than the accelerated greedy, so if you have a reason to believe your function is submodular (or approximately so), 		please use the lazy greedy algorithm.
 *
 */

#define SMALLEST_NUMBER -1e70
#define LARGEST_NUMBER 1e70

std::unordered_set<int> naiveGreedyMax(setFunctions& f, vector<double>& costList, double budget, std::unordered_set<int> startSet = std::unordered_set<int>(), int verbosity = 1, double alpha = 1){
	if(verbosity > 0) cout<<"Starting the naive greedy algorithm\n"<<flush;
	std::unordered_set<int> sset = startSet;	
    	double currvalbest;
	int curridbest;
	double currentCost;
	int iter = 0;
	f.setpreCompute(startSet); // Set the precomputed statistics.
	unordered_set<int>::iterator it;
	// naive greedy algorithm implementation
	while (1) {
		iter++;
		currvalbest = 0;
		for( it = f.getGroundSet().begin(); it != f.getGroundSet().end(); it++ ) {
			if( (sset.find(*it) == sset.end()) && (f.evalGainsaddFast(sset, *it)/pow(costList[*it],alpha) > currvalbest)){
				currvalbest = f.evalGainsaddFast(sset, *it)/pow(costList[*it],alpha);
				curridbest = *it;
			}
		}
		if((currvalbest > 0) && (currentCost + costList[curridbest] <= budget)){
			f.updateStatisticsAdd(sset, curridbest);
			currentCost+=costList[curridbest];
			sset.insert(sset.end(), curridbest);
			if(verbosity > 0) cout<<"Adding element "<< curridbest<<" in iteration "<<iter<<" and the objective value is "<<f.eval(sset)<<"\n"<<flush;
		}
		else{
			break;
		}
			    
	}	
	return sset;
}
