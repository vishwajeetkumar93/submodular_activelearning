/*
 *	Implementation of the Majorization-Minimization Supermodular-Submodular Algorithm for minimizing the difference between submodular functions.
	The problem is: \min_X f(X) - \lambda g(X), where f and g are submodular function. \lambda controls the tradeoff between f and g!
	Uses the idea of iteratively minimizing upper bounds computed via supergradients, and performs submodular maximization at every round (Iyer-Bilmes, UAI-2012, Narasimhan-Bilmes, UAI-2005). It iteratively computes subgradients h_g^{X^t} at round t, and sets X^{t+1} = \min_X f(X) - h_g^{X^t}(X), which is a submodular minimization!
	The parameter 'mode' in the algorithm controls the choice of the submodular maximization algorithm. mode = 0 (default) is the bidirectional greedy algorithm, and mode = 1 is the local search algorithm.
	Author: Rishabh Iyer 
	Email: rkiyer@uw.edu
	Melodi Lab, University of Washington, Seattle
 *
 */
#ifndef __MMINSupSub_INCLUDED__
#define __MMINSupSub_INCLUDED__
#define EPSILON 1e-10
#define LARGE_NUM 1e50

std::unordered_set<int> mminSupSub(submodularFunctions& f, submodularFunctions& g, double lambda, int verbosity = 1, bool mode = 0, unordered_set<int> startSet = unordered_set<int>()){
	differenceSubmodularFunctions h (&f, &g, 1, lambda);
	std::vector<double> muppera;
	std::vector<double> mupperb;
	double moffseta = 0;
	double moffsetb = 0;
	muppera.assign(f.getSize(),0);
	mupperb.assign(f.getSize(),0);
	std::unordered_set<int> currset = startSet;
	std::unordered_set<int> newset;
	std::unordered_set<int> newseta;
	std::unordered_set<int> newsetb;
	vector<double> defCosts(f.getSize(), 0);
	while(1){
		f.modularUpperBoundA(currset, muppera, moffseta);
		f.modularUpperBoundB(currset, mupperb, moffsetb);
		modularFunctions mfa (f.getGroundSet(), muppera, moffseta);
		modularFunctions mfb (f.getGroundSet(), mupperb, moffsetb);
		combineSubmodularModularFunctions cfa (&g, &mfa, lambda, -1);
		combineSubmodularModularFunctions cfb (&g, &mfb, lambda, -1);
		if(mode == 0){
			newseta = localSearchMax(cfa, currset, 0);
			newsetb = localSearchMax(cfb, currset, 0);
		}
		else if (mode == 1){
			newseta = lazyGreedyMax(cfa, defCosts, LARGE_NUM, currset, 0);
			newsetb = lazyGreedyMax(cfa, defCosts, LARGE_NUM, currset, 0);
		}
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
