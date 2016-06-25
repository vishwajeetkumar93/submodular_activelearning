/*
*	The Bi-directional Greedy Algorithm for non-monotone submodular Maximization by Buchbinder et al (2007). This gives a deterministic 1/3 approximation, and a randomized 1/2 approximation for unconstrained maximization.
	Solves the problem \max_{X \subseteq V} f(X), where f is a non-monotone submodular function. 
	Anthor: Rishabh Iyer
	Melodi Lab, University of Washington, Seattle
	The variable mode decides on the algorithm (randomized or deterministic greedy).
 *
 */
 #ifndef __BIDIRGREEDY_INCLUDED__
#define __BIDIRGREEDY_INCLUDED__

#define LARGE_NUM 1e50
#define EPSILON 1e-10

std::unordered_set<int> bidirGreedyMax(submodularFunctions& f, submodularFunctions& fcopy, totalOrder<int> initialOrdering, int verbosity = 1, int mode = 1){
	srand(time(NULL));
	unordered_set<int> incSet = unordered_set<int>();
	unordered_set<int> decSet = f.getGroundSet();
	// Create a copy of f as fcopy
	double alpha;
	double beta;
	f.setpreCompute(incSet);
	fcopy.setpreCompute(decSet);
	for(int i = 0; i < f.getSize(); i++){
		if (mode == 0){ // The deterministic BD Greedy
			alpha = f.evalGainsaddFast(incSet, initialOrdering[i]);
			beta = -fcopy.evalGainsremoveFast(decSet, initialOrdering[i]);
			if (alpha >= beta){
				f.updateStatisticsAdd(incSet, initialOrdering[i]);
				incSet.insert(initialOrdering[i]);
			}
			else {
				fcopy.updateStatisticsRemove(decSet, initialOrdering[i]);
				decSet.erase(initialOrdering[i]);
			}
		}
		else{ // The randomized BD Greedy
			alpha = max(f.evalGainsaddFast(incSet, initialOrdering[i]), 0.0);
			beta = max(-fcopy.evalGainsremoveFast(decSet, initialOrdering[i]), 0.0);
			double p = alpha/(alpha + beta);
			bool TrueFalse = (rand() % 100) < 100*p;
			if(TrueFalse == 1){
				f.updateStatisticsAdd(incSet, initialOrdering[i]);
				incSet.insert(initialOrdering[i]);
			}
			else {
				fcopy.updateStatisticsRemove(decSet, initialOrdering[i]);
				decSet.erase(initialOrdering[i]);
			}
		}
	}
	return incSet;
}
#endif
