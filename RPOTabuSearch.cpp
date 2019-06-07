/*
 * RPOTabuSearch.cpp
 *
 * File defining the methods of the class RPOTabuSearch.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#include <RPOEvaluator.h>
#include <RPOObjectAssignmentOperation.h>
#include <RPOTabuSearch.h>
#include <initializer_list>
#include <iostream>
#include <utility>
#include <vector>
#include <cstdlib>

using namespace std;

void RPOTabuSearch::initialise(RPOInstance* instance, unsigned tabuTennure) {
	_instance = instance;
	_tabuTennure = tabuTennure;
}

void RPOTabuSearch::setSolution(RPOSolution* solution) {

	if (_solution != NULL){
		cerr << "You should not call RPOTabuSearch::setSolution more than once" << endl;
		exit(1);
	}

	this->_solution = solution;

	if (_bestSolution == NULL) {
		_bestSolution = new RPOSolution(*_instance);
	}

	_bestSolution->copy(*solution);
}

void RPOTabuSearch::run(RPOStopCondition& stopCondition) {
	if (_solution == NULL) {
		cerr << "Tabu search has not been given an initial solution" << endl;
		exit(-1);
	}

	_results.clear();
	unsigned numParam = _instance->getNumParameters();
	unsigned numIterations = 0;

	/**
	 *
	 * While the stop condition is not met
	 *   1. Generate a permutation of the objects
	 *   2. Search for the best non tabu operation assigning an object to a knapsack (0 included)
	 *   3. Apply the operation
	 *   4. Insert the index of the object used in the tabu memory
	 *   4. Update the best so-far solution
	 */

	while (!stopCondition.reached()) {

		vector<int> perm;
		_instance->randomPermutation(numParam, perm);
		double bestDeltaFitness = 0;
		bool initialisedDeltaFitness = false;
		RPOObjectAssignmentOperation bestOperation;

		//Obtain the best non-tabu operation
		for (unsigned i = 0; i < numParam; i++) {
			unsigned indexParam = perm[i];

			//If the object is not tabu (use _shortTermMem_aux.find)
			if (_shortTermMem_aux.find(indexParam) == _shortTermMem_aux.end()) {

				//Try all the knapsacks (including 0) and choose the best option
				for (unsigned j = 0; j < _instance->getLengthD(indexParam); j++) {
					int paramValue=j;
					//Ignore the change which does not modify the object
					if (_solution->parameterValue(indexParam) == paramValue)
						continue;

					//Obtain the difference of fitness of applying the operation
					double deltaFitness = RPOEvaluator::computeDeltaFitness(*_instance, *_solution, indexParam, paramValue);

					//If the difference of fitness is the best up to this moment,
					//store it to apply it later
					if (deltaFitness > bestDeltaFitness
							|| initialisedDeltaFitness == false) {
						initialisedDeltaFitness = true;
						bestDeltaFitness = deltaFitness;
						bestOperation.setValues(indexParam, paramValue, bestDeltaFitness);
					}
				}
			}
		}

		//Apply the operation and store it in the short term memory
		bestOperation.apply(*_solution);
		_shortTermMem.push(bestOperation.getParam());
		_shortTermMem_aux.insert(bestOperation.getParam());

		//If there are too many elements in the memory, according to the tabu tennure,
		//delete the eldest one (i.e. obtain the first element of the queue and eliminate it
		//from the queue and from the set)
		if (_shortTermMem.size() > _tabuTennure) {
			unsigned value = _shortTermMem.front();
			_shortTermMem.pop();
			_shortTermMem_aux.erase(value);
		}

		//Update the best solution
		if (RPOEvaluator::compare(_solution->getFitness(),
				_bestSolution->getFitness()) > 0) {
			_bestSolution->copy(*_solution);
		}

		numIterations++;
		_results.push_back(_solution->getFitness());

		stopCondition.notifyIteration();
	}
}
