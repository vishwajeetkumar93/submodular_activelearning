/*
 *	Complement submodular functions h(X) = f(V \ X), a submodular functions f.
	Melodi Lab, University of Washington, Seattle
 *
 */

class complementSubmodularFunctions: public submodularFunctions{
	protected:
	submodularFunctions* f;
	unordered_set<int> preCompute; // preCompute V \ X, for a given set X.
	public:
	// Functions
	complementSubmodularFunctions(submodularFunctions* f);
	unordered_set<int> findComplement(const unordered_set<int>& sset); // Return the complement of the set.
	double eval(const unordered_set<int>& sset);
	double evalFast(const unordered_set<int>& sset, bool safe);
	double evalGainsadd(const unordered_set<int>& sset, int item);
	double evalGainsremove(const unordered_set<int>& sset, int item);
	double evalGainsaddFast(const unordered_set<int>& sset, int item, bool safe);
	double evalGainsremoveFast(const unordered_set<int>& sset, int item, bool safe);
	void updateStatisticsAdd(const unordered_set<int>& sset, int item, bool safe);
	void updateStatisticsRemove(const unordered_set<int>& sset, int item, bool safe);
	void setpreCompute(const unordered_set<int>& sset); 
	void clearpreCompute();
	double currentCoverage() {return 0.0;}
	void initializeFinalSet() {}
};

complementSubmodularFunctions::complementSubmodularFunctions(submodularFunctions* f): submodularFunctions(f->getGroundSet()), f(f){}

unordered_set<int> complementSubmodularFunctions::findComplement(const unordered_set<int>& sset){
	unordered_set<int> csset = unordered_set<int>();
	for(unordered_set<int>::iterator it = groundSet.begin(); it != groundSet.end(); it++ ){
		if(sset.find(*it) == sset.end())
			csset.insert(*it);
	}
	return csset;
}
double complementSubmodularFunctions::eval(const unordered_set<int>& sset){
	unordered_set<int> csset = findComplement(sset);
	return f->eval(csset);
}

double complementSubmodularFunctions::evalFast(const unordered_set<int>& sset, bool safe = 0){
	return f->evalFast(preCompute, 0);
}

double complementSubmodularFunctions::evalGainsadd(const unordered_set<int>& sset, int item){
	unordered_set<int> csset = findComplement(sset);
	return -1*f->evalGainsremove(csset, item);
}

double complementSubmodularFunctions::evalGainsremove(const unordered_set<int>& sset, int item){
	unordered_set<int> csset = findComplement(sset);
	return -1*f->evalGainsadd(csset, item);
}

double complementSubmodularFunctions::evalGainsaddFast(const unordered_set<int>& sset, int item, bool safe = 0){
// If safe = 0, we do a hack in order to avoid the complexity of computing the complement set, since it is often not needed. Use this function in the unsafe mode if you are not sure what to do. This works only in the case when the underlying function uses preComputed statistics, and does not just implement evalGainsadd(). If not sure, just set the parameter 'safe' to it's default value.
	return -1*f->evalGainsremoveFast(preCompute, item, safe);  
}

double complementSubmodularFunctions::evalGainsremoveFast(const unordered_set<int>& sset, int item, bool safe = 0){
// If safe = 0, we do a hack in order to avoid the complexity of computing the complement set, since it is often not needed. Use this function in the unsafe mode if you are not sure what to do. This works only in the case when the underlying function uses preComputed statistics, and does not just implement evalGainsadd(). If not sure, just set the parameter 'safe' to it's default value.
	return -1*f->evalGainsaddFast(preCompute, item, safe);
}

void complementSubmodularFunctions::updateStatisticsAdd(const unordered_set<int>& sset, int item, bool safe = 0){
	// If safe = 0, we do a hack in order to avoid the complexity of computing the complement set, since it is often not needed. Use this function in the unsafe mode if you are not sure what to do. This works only in the case when the underlying function uses preComputed statistics, and does not just implement evalGainsadd(). If not sure, just set the parameter 'safe' to it's default value.
	f->updateStatisticsRemove(preCompute, item);
	preCompute.erase(item);
}

void complementSubmodularFunctions::updateStatisticsRemove(const unordered_set<int>& sset, int item, bool safe = 0){
	// If safe = 0, we do a hack in order to avoid the complexity of computing the complement set, since it is often not needed. Use this function in the unsafe mode if you are not sure what to do. This works only in the case when the underlying function uses preComputed statistics, and does not just implement evalGainsadd(). If not sure, just set the parameter 'safe' to it's default value.
	unordered_set<int> csset = findComplement(sset);
	f->updateStatisticsAdd(preCompute, item);
	preCompute.insert(item);
}

void complementSubmodularFunctions::clearpreCompute(){
	f->clearpreCompute();
	preCompute.clear();
}

void complementSubmodularFunctions::setpreCompute(const unordered_set<int>& sset){
	f->setpreCompute(sset);
	preCompute = findComplement(sset);
}

