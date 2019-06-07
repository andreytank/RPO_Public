/*
 * RPOIteratedGreedy.cpp
 *
 * File defining the methods of the class RPOIteratedGreedy.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#include <RPOIteratedGreedy.h>
#include <RPOObjectAssignmentOperation.h>
#include <RPOSolution.h>
#include <iostream>
#include "math.h"

using namespace std;

void RPOIteratedGreedy::chooseOperation(
		RPOObjectAssignmentOperation& operation) {

	int bestParam = 0;
	int bestValue = _sol->parameterValue(bestParam);
	double bestDensity = 0;
	double bestDeltaFitness = 0;
	bool initialisedBestDensity = false;
	unsigned numParam = _instance->getNumParameters();;

	/**
	 *
	 * Iterate over the objects which are not in any knapsack (0)
	 *   Iterate over all the knapsacks
	 *     Obtain difference of fitness of assigning the object to the knapsack and the density
	 *     Store the assignment as the best if it is the one resulting in the highest density
	 */

	for (unsigned i = 0; i < numParam; i++) {

		int indexParam = i;

		if (_sol->parameterValue(indexParam) == 0) { //check that it is not in a knapsack

			for (unsigned j = 0 ; j < _instance->getLengthD(indexParam); j++) { //for all the available knapsacks (do not consider 0)

				//Obtain delta fitness and density (deltaFitness divided by weight)
				int newValue = j;
				if(newValue==_sol->parameterValue(indexParam))
					continue;

				double deltaFitness = RPOEvaluator::computeDeltaFitness(*_instance, *_sol, indexParam, newValue);
				double density = deltaFitness / fabs(_sol->parameterValue(indexParam)-newValue);

				//update the best values if they are improved
				if (density > bestDensity || initialisedBestDensity == false) {
					initialisedBestDensity = true;
					bestDensity = density;
					bestParam = indexParam;
					bestValue = newValue;
					bestDeltaFitness = deltaFitness;
				}
			}
		}
	}

	operation.setValues(bestParam, bestValue, bestDeltaFitness);
}

void RPOIteratedGreedy::rebuild() {

	/** Choose the first operation */
	RPOObjectAssignmentOperation operation;
	chooseOperation(operation);

	/**
	 * While the operation has a positive increase of fitness, operation.getDeltaFitness(),
	 *  1. Apply the operation in _sol
	 *  2. Store the fitness of the solution in _result (for the plots)
	 *  3. Select a new operation
	 */
	while (operation.getDeltaFitness() > 0) {
		operation.apply(*_sol);
		_results.push_back(_sol->getFitness());
		chooseOperation(operation);
	}
}

void RPOIteratedGreedy::destroy() {

	/**
	 *
	 * Iterate over all the objects and take them out from
	 * their knapsack with a probability _alpha
	 */

	unsigned numParam = _instance->getNumParameters();

	for (unsigned i = 0; i < numParam; i++){

		double randSample = ((double)(rand())) / RAND_MAX;

		if (randSample < _alpha){
			_sol->ChangeParameterTo(i,0);
		}
	}

	double fitness = RPOEvaluator::computeFitness(*_instance, *_sol);
	_sol->setFitness(fitness);
	_results.push_back(_sol->getFitness());
}

void RPOIteratedGreedy::initialise(double alpha, RPOInstance& instance) {
	_sol = new RPOSolution(instance);
	_bestSolution = new RPOSolution(instance);
	_bestSolution->copy(*_sol);
	_instance = &instance;
	_alpha = alpha;
}

void RPOIteratedGreedy::run(RPOStopCondition& stopCondition) {

	if (_sol == NULL) {
		cerr << "IG was not initialised" << endl;
		exit(-1);
	}

	/** Obtain the first solution */
	rebuild();

	if (RPOEvaluator::compare(_sol->getFitness(), _bestSolution->getFitness()) > 0)
		_bestSolution->copy(*_sol);

	/**
	 *
	 * While the stop condition is not met
	 *  1. Partially destroy the solution
	 *  2. Rebuild the solution
	 *  3. Store the new fitness of the solution in _results
	 *  4. Update the best solution and use it again if the new
	 *     one is worse
	 */

	while (stopCondition.reached() == false) {
		destroy();
		rebuild();
		_results.push_back(_sol->getFitness());

		if (RPOEvaluator::compare(_sol->getFitness(), _bestSolution->getFitness()) > 0){
			_bestSolution->copy(*_sol);
		}
		else{
			_sol->copy(*_bestSolution);
		}

		stopCondition.notifyIteration();
	}
}


