// Implementation of MMin-I/ II for unconstrained submodular minimization.
// This acts as a preprocessing step for general purpose SFM algorithms.
// Author: Rishabh Iyer (rkiyer@u.washington.edu)

void mminUC(submodularFunctions& f, unordered_set<int>& minStartSet, unordered_set<int>& maxStartSet){
	minStartSet = unordered_set<int>();
	maxStartSet = f.getGroundSet();
	unordered_set<int> addSet = unordered_set<int>();
	unordered_set<int> removeSet = unordered_set<int>();
	bool stuck;
	while(1){
		stuck = 1;
		addSet.clear();
		f.setpreCompute(minStartSet);
		for (unordered_set<int>::iterator it = f.getGroundSet().begin(); it != f.getGroundSet().end(); it++){
			if( (minStartSet.find(*it) == minStartSet.end()) && (f.evalGainsaddFast(minStartSet, *it) < 0)){ // *it is not in minStartSet and adding it to minStartSet reduces value
				addSet.insert(*it);
				stuck = 0;
			}
		}
		for(unordered_set<int>::iterator it = addSet.begin(); it != addSet.end(); it++){//Union addSet and minStartSet
			minStartSet.insert(*it);
		}
		if(stuck == 1) break;
	}

	while(1){
		stuck = 1;
		removeSet.clear();
		f.setpreCompute(maxStartSet);
		for (unordered_set<int>::iterator it = f.getGroundSet().begin(); it != f.getGroundSet().end(); it++){
			if( (maxStartSet.find(*it) != maxStartSet.end()) && (f.evalGainsremoveFast(maxStartSet, *it) >= 0)){ // *it is in minSTartSet and removing it from maxStartSet reduces value 
				removeSet.insert(*it);
				stuck = 0;
			}
		}
		for(unordered_set<int>::iterator it = removeSet.begin(); it != removeSet.end(); it++){//Union addSet and minStartSet
			maxStartSet.erase(*it);
		}
		if(stuck == 1) break;
	}
}

