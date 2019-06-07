/*
 * RPOGrasp.cpp
 *
 * File defining the methods of the class RPOGrasp.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#include <RPOGrasp.h>
#include <vector>
#include <RPOSimpleFirstImprovementNO.h>
#include <RPOLocalSearch.h>
#include <iostream>
#include "math.h"

using namespace std;

void RPOGrasp::chooseOperation(RPOObjectAssignmentOperation& operation) {

	int bestParam = 0;
	int bestValue = _sol->parameterValue(bestParam);
	double bestDensity = 0;
	double bestDeltaFitness = 0;
	bool initialisedBestDensity = false;
	unsigned numParam = _instance->getNumParameters();

	/**
	 * Obtain the number of tries as the percentage alpha_ of the number of possibilities,
	 * which is the number of objects multiplied by the number of knapsacks.
	 *
	 * You will not count the fact that the objects are already assigned to
	 * a knapsack, you will not count knapsack 0 (objects without a knapsack assigned), and
	 * you will not not count the fact that some knapsacks are already full.
	 */
	unsigned numTries = 0;

	for(int param=0;param<numParam;param++)
	 	numTries+=_instance->getLengthD(param);
	numTries*=_alpha;
	/**
	 *
	 * Generate random alternatives of object and knapsack (without knapsack 0) and choose
	 * the alternative leading to better density.
	 */
	for (unsigned i = 0; i < numTries; i++) {
		int indexParam = rand() % numParam;
		int paramValue;
		while(paramValue = (rand() % _instance->getLengthD(indexParam)) ==
		_sol->parameterValue(indexParam));

		double deltaFitness = RPOEvaluator::computeDeltaFitness(*_instance, *_sol, indexParam, paramValue); // obtain the fitness improvement
		double density = deltaFitness / fabs(_sol->parameterValue(indexParam)-paramValue); //obtain the density of the operation (difference of fitness divided by the weight)

		//update the best values if they are improved
		if (density > bestDensity || initialisedBestDensity == false) {
			initialisedBestDensity = true;
			bestDensity = density;
			bestParam = indexParam;
			bestValue = paramValue;
			bestDeltaFitness = deltaFitness;
		}
	}

	operation.setValues(bestParam, bestValue,
			bestDeltaFitness);
}

void RPOGrasp::buildInitialSolution() {

	/**
	 * Empty the solution _sol assigning a 0 fitness to it and establishing a 0
	 * for the knapsacks of all objects
	 */
	unsigned numParam = _instance->getNumParameters();

	delete _sol;
	_sol=new RPOSolution(*_instance);

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

void RPOGrasp::initialise(double alpha, RPOInstance& instance) {

	_sol = new RPOSolution(instance);
	_bestSolution = new RPOSolution(instance);
	_bestSolution->copy(*_sol);
	_instance = &instance;
	_alpha = alpha;
}

void RPOGrasp::run(RPOStopCondition& stopCondition) {

	if (_sol == NULL) {
		cerr << "GRASP was not initialised" << endl;
		exit(-1);
	}

	/**
	 *
	 * While the stop condition is not met
	 *   1. Generate a random solution calling the corresponding method (buildInitialSolution)
	 *   2. Store the fitness of the solution in _results
	 *   3. Optimize _sol using a local search and the neighbourhood operator of the metaheuristic
	 *   4. Update the best so-far solution
	 */
	while (! stopCondition.reached()) {
		buildInitialSolution();
		_results.push_back(_sol->getFitness());
		_ls.optimise(*_instance, _no, *_sol);

		vector<double> &auxResults = _ls.getResults();

		for (auto result : auxResults){
			_results.push_back(result);
		}

		if (RPOEvaluator::compare(_sol->getFitness(), _bestSolution->getFitness()) > 0)
			_bestSolution->copy(*_sol);

		stopCondition.notifyIteration();
	}
}
