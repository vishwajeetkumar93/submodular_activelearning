/*
 *	Implementation of the Majorization-Minimization Modular-Modular Algorithm for minimizing the difference between submodular functions.
	The problem is: \min_X f(X) - g(X), where f and g are submodular function.
	Uses the idea of iteratively minimizing upper bounds computed via subgradients and supergradients, and performs modular  at every round (Iyer-Bilmes, UAI-2012). It iteratively computes subgradients h_g^{X^t} and supergradients m^f_{X^t} at round t, and sets X^{t+1} = \min_X m^f_{X^t}(X) - h_g^{X^t}(X), which is a modular minimization!
	The parameter 'mode' in the algorithm gives the choice of the subgradient (0 is a random subgradient, and 1 (default) is an efficient heiristic for the subgradient).
	Author: Rishabh Iyer 
	Email: rkiyer@uw.edu
	Melodi Lab, University of Washington, Seattle
 *
 */
#ifndef __MMINModMod_INCLUDED__
#define __MMINModMod_INCLUDED__
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
	for(unordered_set<int>::const_iterator it = f.getGroundSet().begin(); it != f.getGroundSet().end(); it++){
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

vector<double> getEfficientSubgradient(submodularFunctions& f, setFunctions& h, unordered_set<int>& sset){
// Outputs an efficient subgradient based on a heuristic of arranging the gains of the difference function h.
	srand(time(NULL));
	vector<double> orderedVec(f.getSize());
	vector<double> subgradient;
	totalOrder<double> order(f.getSize()) ;
	for(unordered_set<int>::const_iterator it = f.getGroundSet().begin(); it != f.getGroundSet().end(); it++){
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

unordered_set<int> modularmindiff(vector<double> mupper, vector<double> mlower, double lambda){
	unordered_set<int> optset = unordered_set<int>();
	for (int i = 0; i < mupper.size(); i++){
		if(mupper[i] - lambda*mlower[i] <= 0)
			optset.insert(i);
	}
	return optset;
}

std::unordered_set<int> mminModMod(submodularFunctions& f, submodularFunctions& g, double lambda, int verbosity = 1, bool mode = 1, unordered_set<int> startSet = unordered_set<int>()){
	differenceSubmodularFunctions h (&f, &g, 1, lambda);
	std::vector<double> muppera;
	std::vector<double> mupperb;
	vector<double> subgradient;
	double moffseta = 0;
	double moffsetb = 0;
	muppera.assign(f.getSize(),0);
	mupperb.assign(f.getSize(),0);
	std::unordered_set<int> currset = startSet;
	std::unordered_set<int> newset;
	std::unordered_set<int> newseta;
	std::unordered_set<int> newsetb;
	while(1){
		if(mode == 1){
			subgradient = getEfficientSubgradient(g, h, currset);
		}
		else{
			subgradient = getRandomSubgradient(g, currset);
		}
		f.modularUpperBoundA(currset, muppera, moffseta);
		f.modularUpperBoundB(currset, mupperb, moffsetb);
		
		newseta = modularmindiff(muppera, subgradient, lambda);
		newsetb = modularmindiff(mupperb, subgradient, lambda);
		if (h.eval(newseta) < h.eval(newsetb))
			newset = newseta;
		else
			newset = newsetb;
		if(verbosity > 0) cout<<"Current Objective value is "<<h.eval(newset)<<"\n";
		if(h.eval(newset) - h.eval(currset) < -EPSILON){
			currset = newset;
		}
		else{
			break;
		}
	}
	return currset;
}
#endif


