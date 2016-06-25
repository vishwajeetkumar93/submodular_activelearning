/*
 *	The reverse greedy algorithm starting with the groundset and greedily removing elements until there is no benefit in removing elements any more. 	
	This is an accelerated version with priority queue (Minoux 1976).
	Solves the problem \max_{X \subseteq V} f(X), where f is a non-monotone submodular function.
	Anthor: Rishabh Iyer
	Melodi Lab, University of Washington, Seattle
 *
 */

#define SMALLEST_NUMBER -1e70
#define LARGEST_NUMBER 1e70

#ifndef __INCREMENT__
#define __INCREMENT__
class Increment{
	public: 
		Increment(){};
		Increment(double x, int i){ value=x; index=i; }
		bool operator< (const Increment&) const;
	
		int get_index() const { return index; }
		double get_value() const{ return value; }
	private:
		int index;
		double value;
};

bool Increment::operator< (const Increment& right) const
{
	return value < right.value;
}
#endif

std::unordered_set<int> revlazyGreedyMax(submodularFunctions& f, std::unordered_set<int> startSet, int verbosity=1){
	int nSelected = 0 ; // number of items selected
	std::unordered_set<int> selectedSet = startSet;
	bool* hashSelected = new bool[f.getSize()]; // an index table to show whether an item is selected
	for(int i=0;i<f.getSize();i++) hashSelected[i]=false;	
    	double maxV;
	double newV;
    	double preV = f.eval(startSet);
    	double currentCost = 0;
	f.setpreCompute(selectedSet); // clear the precomputed statistics, in case it is already not cleared, and set it to the selectedSet.
	// accelerated greedy algorithm implementation
	priority_queue <Increment> rho;
	// initilize the priority queue
	for (unordered_set<int>::iterator it = startSet.begin(); it != startSet.end(); it++) { 
		rho.push(Increment(-f.evalGainsremoveFast(selectedSet, *it),*it)); 
	}
	int sort_cnt = 0;
	while (! rho.empty()) {
		int topid = rho.top().get_index();
		rho.pop();
		maxV = preV - f.evalGainsremoveFast(selectedSet, topid);
		newV = - f.evalGainsremoveFast(selectedSet, topid); // return normalized gain 
		if (verbosity >= 5) printf("max gain = %.6e, rho->top() = %.6e\n",newV,rho.top().get_value());
		if (newV < rho.top().get_value()) {
			rho.push(Increment(newV, topid)); // if condition not satisfied, push back and re-sort
			sort_cnt++;
			if (verbosity >= 10) printf("Condition not met, re-sorting queue (%d iterations)\n",sort_cnt);
		}
		else {
			// guaranteed to be optimal because of submodularity
			if(maxV - preV < 0) break;
		    	hashSelected[topid] = true;
			selectedSet.erase(topid);
			nSelected++;
			if(verbosity > 0) printf("Selecting %dth sample: preV = %.10e, curV = %.10e\n", topid, preV, maxV);
			preV = maxV;
			f.updateStatisticsRemove(selectedSet, topid);
			sort_cnt = 0;
		}    
	}	
	return selectedSet;
}
