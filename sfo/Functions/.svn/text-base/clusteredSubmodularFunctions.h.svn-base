/*
 *	A helper function, which takes in a submodular function f and a set of sets A_1, A_2, ... A_k and computes the function h(X) = \sum_{i =1}^k f(X \cap A_i).
	Melodi Lab, University of Washington, Seattle
 *
 */

combineSubmodularFunctions clusteredSubmodularFunctions(submodularFunctions& f, vector<unordered_set<int>>& clusters){
	vector<submodularFunctions*> fvec = vector<submodularFunctions*>();
	vector<double> lambda(clusters.size(), 1);
	for (int i = 0; i < clusters.size(); i++){
		truncateSubmodularFunctions fTrunc(&f, clusters[i]);
		fvec.push_back(&fTrunc);
	}
	combineSubmodularFunctions fComb(f.getGroundSet(), fvec, lambda);
	return fComb;
}

