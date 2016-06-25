/*
 *	Implementation of the Majorization-Minimization Submodular-Supermodular Algorithm for minimizing the difference between submodular functions.
	The problem is: \min_X f(X) - \lambda g(X), where f and g are submodular function. \lambda controls the tradeoff between f and g!
	Uses the idea of iteratively minimizing upper bounds computed via subgradients, and performs submodular minimization at every round (Iyer-Bilmes, UAI-2012, Narasimhan-Bilmes, UAI-2005). It iteratively computes subgradients h_g^{X^t} at round t, and sets X^{t+1} = \min_X f(X) - h_g^{X^t}(X), which is a submodular minimization!
	The parameter 'mode' in the algorithm gives the choice of the subgradient (0 is a random subgradient, and 1 (default) is an efficient heiristic for the subgradient).
	Author: Rishabh Iyer 
	Email: rkiyer@uw.edu
	Melodi Lab, University of Washington, Seattle
 *
 */
#ifndef __MMINSubSup_INCLUDED__
#define __MMINSubSup_INCLUDED__
#define EPSILON 1e-10
#define LARGE_NUM 1e50

#ifndef __EFFSUBGRAD__
#define __EFFSUBGRAD__
vector<double> getRandomSubgradient(submodularFunctions& f, unordered_set<int>& sset){
// Outputs a random subgradient at sset.
	srand(time(NULL));
	vector<double> orderedVec(f.getSize());
	vector<double> subgradient;
	totalOrder<double> order(f.getSize()) ;
	for(unordered_set<int>::iterator it = f.getGroundSet().begin(); it != f.getGroundSet().end(); it++){
		if(sset.find(*it) != sset.end()){ // *it belongs to sset
			orderedVec[*it] = -LARGE_NUM - rand()%(f.getSize());
		}
		else{
			orderedVec[*it] = -rand()%(f.getSize());
		}
	}
	order.resetOrder(orderedVec);
	f.getSubgradient(order, sset, subgradient);
	return subgradient;	
}

vector<double> getEfficientSubgradient(submodularFunctions& f, setFunctions& h, const unordered_set<int>& sset){
// Outputs an efficient subgradient based on a heuristic of arranging the gains of the difference function h.
	srand(time(NULL));
	vector<double> orderedVec(f.getSize());
	vector<double> subgradient;
	totalOrder<double> order(f.getSize()) ;
	for(unordered_set<int>::iterator it = f.getGroundSet().begin(); it != f.getGroundSet().end(); it++){
		if(sset.find(*it) != sset.end()){ // *it belongs to sset
			orderedVec[*it] = -LARGE_NUM + h.evalGainsremoveFast(sset, *it);
		}
		else{
			orderedVec[*it] = h.evalGainsaddFast(sset, *it);
		}
	}
	order.resetOrder(orderedVec);
	f.getSubgradient(order, sset, subgradient);
	return subgradient;	
}
#endif

std::unordered_set<int> mminSubSup(submodularFunctions& f, submodularFunctions& g, double lambda, int verbosity = 1, bool mode = 1, unordered_set<int> startSet = unordered_set<int>()){
	differenceSubmodularFunctions h (&f, &g, 1, lambda);
	std::unordered_set<int> currset = startSet;
	std::unordered_set<int> newset1 = currset;
	std::unordered_set<int> newset2 = currset;
	std::vector<double> xstar;
	unordered_set<int> groundSet = f.getGroundSet();
	while(1){
		vector<double> subgradient;
		if(mode == 1)
			subgradient = getEfficientSubgradient(g, f, currset);
		else
			subgradient = getRandomSubgradient(g, currset);
		modularFunctions mgh (groundSet, subgradient, 0);
		combineSubmodularModularFunctions cfg (&f, &mgh, 1, -lambda);
		minNormPoint(cfg, newset1, newset2, xstar, 1);
		if(verbosity > 0) cout<<"Current Objective value is "<<h.eval(newset1)<<"\n";
		if(h.eval(newset1) - h.eval(currset) < -EPSILON){
			currset = newset1;
		}
		else{
			break;
		}
		//delete gh;
		//delete cfg;
	}
	return currset;
}
#endif
