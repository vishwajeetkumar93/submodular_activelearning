/*
 *	Combining two submodular functions h(X) = \sum_{i = 1}^k \lambda_i f_i(X), for two submodular functions f and g.
	Melodi Lab, University of Washington, Seattle
 *
 */

class scaleSubmodularFunctions: public submodularFunctions{
	protected:
	submodularFunctions* f;
	double lambda;
	public:
	// Functions
	scaleSubmodularFunctions(unordered_set<int>& groundSet, submodularFunctions* f, double lambda);
	double eval(const unordered_set<int>& sset);
	double evalFast(const unordered_set<int>& sset, bool safe);
	double evalGainsadd(const unordered_set<int>& sset, int item);
	double evalGainsremove(const unordered_set<int>& sset, int item);
	double evalGainsaddFast(const unordered_set<int>& sset, int item, bool safe);
	double evalGainsremoveFast(const unordered_set<int>& sset, int item, bool safe);
	void updateStatisticsAdd(const unordered_set<int>& sset, int item, bool safe);
	void updateStatisticsRemove(const unordered_set<int>& sset, int item, bool safe);
	void setpreCompute(const unordered_set<int>& sset); // Set preCompute for removing elements.
	void clearpreCompute();
	double currentCoverage();
	void initializeFinalSet();
};

scaleSubmodularFunctions::scaleSubmodularFunctions(unordered_set<int>& groundSet, submodularFunctions* f, double lambda): submodularFunctions(groundSet), f(f), lambda(lambda){}

double scaleSubmodularFunctions::eval(const unordered_set<int>& sset){
	return lambda*f->eval(sset);
}

double scaleSubmodularFunctions::evalFast(const unordered_set<int>& sset, bool safe = 0){
	return lambda*f->evalFast(sset, safe);
}

double scaleSubmodularFunctions::evalGainsadd(const unordered_set<int>& sset, int item){
	return lambda*f->evalGainsadd(sset, item);
}

double scaleSubmodularFunctions::evalGainsremove(const unordered_set<int>& sset, int item){
	return lambda*f->evalGainsremove(sset, item);
}

double scaleSubmodularFunctions::evalGainsaddFast(const unordered_set<int>& sset, int item, bool safe = 0){
	return lambda*f->evalGainsaddFast(sset, item, safe);
}

double scaleSubmodularFunctions::evalGainsremoveFast(const unordered_set<int>& sset, int item, bool safe = 0){
	return lambda*f->evalGainsremoveFast(sset, item, safe);
}

void scaleSubmodularFunctions::updateStatisticsAdd(const unordered_set<int>& sset, int item, bool safe = 0){
	f->updateStatisticsAdd(sset, item, safe);
}

void scaleSubmodularFunctions::updateStatisticsRemove(const unordered_set<int>& sset, int item, bool safe = 0){
	f->updateStatisticsRemove(sset, item, safe);
}

void scaleSubmodularFunctions::clearpreCompute(){
	f->clearpreCompute();
}

void scaleSubmodularFunctions::setpreCompute(const unordered_set<int>& sset){
	f->setpreCompute(sset);
}

double scaleSubmodularFunctions::currentCoverage()
{
	return lambda*f->currentCoverage();
}
void scaleSubmodularFunctions::initializeFinalSet()
{
	f->initializeFinalSet();
}

