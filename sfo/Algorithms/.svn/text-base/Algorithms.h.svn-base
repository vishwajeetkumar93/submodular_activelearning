#ifndef __ALGORITHMS__
#define __ALGORITHMS__

// Lazy Greedy Algorithm for Submodular Maximization
std::unordered_set<int> lazyGreedymax(setFunctions& f, vector<double> costList, double budget, int verbosity = 1, double alpha = 1, std::unordered_set<int> startSet = std::unordered_set<int>());

// Lazy Greedy Algorithm for the Submodular Set Cover
std::unordered_set<int> lazyGreedySC(setFunctions& f, vector<double> costList, double cover, int verbosity = 0, double alpha = 1);

// Local Search Algorithm for Submodular Maximization
std::unordered_set<int> localSearchMax(setFunctions* f, int verbosity = 1, std::unordered_set<int> startSet = unordered_set<int>());

// The Minimum Norm Point Algorithm for Unconstrained Submodular Minimization
bool minNormPoint(setFunctions* f, unordered_set<int>& minSolSet, unordered_set<int>& maxSolSet, std::vector<double>& xstar, int verbosity = 1);


#endif
