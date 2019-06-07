/**
 * RPOLocalSearch.h
 *
 * File defining the class RPOLocalSearch.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef INCLUDE_RPOLOCALSEARCH_H_
#define INCLUDE_RPOLOCALSEARCH_H_

#include <RPOInstance.h>
#include <RPOSolution.h>
#include <RPONeighExplorer.h>

/**
 * Class implementing an iterative local search applied to a given random solution
 */
class RPOLocalSearch{

	/**
	 * Double vector storing the quality of the last solution accepted
	 */
	vector<double> _results;

public:
	/**
	 * Constructor
	 */
	RPOLocalSearch();

	/**
	 * Destructor
	 */
	~RPOLocalSearch();

	/**
	 * Function optimising the solution applying iteratively the exploration of
	 * its neighbourhood until reaching a local optimum
	 * @param[in] instance Instance of the RPO
	 * @param[in] explorer Operator for exploring the neighbourhood. The idea is that the neighbourhood will be
	 *                     explored in two alternative ways: use the first operator which improves the current
	 *                     solution, use the operator which leads to the best possible improvement.
	 * @param[in,out] solution Initial solution, i.e. starting point from which the solution will be improved.
	 *                     The final solution will be also stored in this reference.
	 */
	void optimise(RPOInstance &instance, RPONeighExplorer &explorer, RPOSolution &solution);

	/**
	 * Function returning a vector with the results of the solutions accepted by the local search in each step.
	 *
	 * @return vector with the accepted solution in each step of the local search.
	 */
	vector<double>& getResults() {
		return _results;
	}
};



#endif /* INCLUDE_RPOLOCALSEARCH_H_ */
