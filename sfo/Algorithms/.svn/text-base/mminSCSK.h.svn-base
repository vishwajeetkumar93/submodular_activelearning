/*
 *	Implementation of the Majorization-Minimization Supergradient Algorithm for Submodular Cost Submodular Knapsack (SCSK) Problem.
	The problem is: \max_{X: f(X) \leq B} g(X), where f and g are monotone submodular function.
	Uses the idea of iteratively minimizing upper bounds computed via supergradients (Iyer-Jegelka-Bilmes, ICML-2013)
	Author: Rishabh Iyer
	Melodi Lab, University of Washington, Seattle
 *
 */
#ifndef __MMINSCSK_INCLUDED__
#define __MMINSCSK_INCLUDED__
#define EPSILON 1e-10

std::unordered_set<int> mminSCSK(submodularFunctions& f, submodularFunctions& g, double budget, int verbosity = 1, unordered_set<int> startSet = unordered_set<int>()){
	std::vector<double> muppera;
	std::vector<double> mupperb;
	double moffseta = 0;
	double moffsetb = 0;
	muppera.assign(f.getSize(),0);
	mupperb.assign(f.getSize(),0);
	std::unordered_set<int> emptyset = unordered_set<int>();
	std::unordered_set<int> currset = startSet;
	std::unordered_set<int> newset;
	std::unordered_set<int> newseta;
	std::unordered_set<int> newsetb;
	int iter = 0;
	double newVal = 0;
	double currVal = 0;
	while(1){
		f.modularUpperBoundA(currset, muppera, moffseta);
		f.modularUpperBoundB(currset, mupperb, moffsetb);
		newseta = lazyGreedyMax(g, muppera, budget - moffseta, emptyset, 0, 1);
		newsetb = lazyGreedyMax(g, mupperb, budget - moffsetb, emptyset, 0, 1);
		if (g.eval(newseta) > g.eval(newsetb))
			newset = newseta;
		else
			newset = newsetb;
		currVal = g.eval(currset);
		newVal = g.eval(newset);
		if(verbosity > 0) cout<<"Current value is "<< newVal<<" in iteration "<<iter<<"\n"<<flush;
		if (iter == 0){
			currset = newset;
		}
		else if (newVal - currVal > EPSILON){
			currset = newset;
		}
		else{
			break;
		}
		iter++;
	}
	return currset;
}

#endif


