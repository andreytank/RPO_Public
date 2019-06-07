/*
 * RPOLocalSearch.cpp
 *
 * File defining the methods of the class RPOLocalSearch.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#include <RPOLocalSearch.h>
#include <RPOInstance.h>
#include <RPOSolution.h>
#include <RPONeighExplorer.h>
#include <RPOObjectAssignmentOperation.h>

RPOLocalSearch::RPOLocalSearch() {
}

RPOLocalSearch::~RPOLocalSearch() {
}

void RPOLocalSearch::optimise(RPOInstance& instance,
		RPONeighExplorer& explorer, RPOSolution& solution) {

	_results.clear();
	_results.push_back(solution.getFitness());
	RPOObjectAssignmentOperation operation;
	explorer.findOperation(instance, solution, operation);

	operation.apply(solution);
	_results.push_back(solution.getFitness());

	while(explorer.findOperation(instance, solution, operation)){
		operation.apply(solution);
		_results.push_back(solution.getFitness());
	}

	/**
	 * 1. Apply once the exploration of the neighborhood and store whether you have
	 *    been able to improve the solution or not
	 *
	 * 2. While you have been able to improve the solution
	 *   a. Apply the exploration of the change found in the previous exploration
	 *   b. Store the new fitness of the solution in _results
	 *   c. Apply a new exploration of the neighborhood
	 */

}
