/*
 * RPOSimulatedAnnealing.cpp
 *
 * File defining the methods of the class RPOSimulatedAnnealing.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#include "RPOSimulatedAnnealing.h"
#include "RPOSolution.h"
#include "RPOSolGenerator.h"
#include <RPOEvaluator.h>
#include <cmath>
#include <iostream>
#include <cstdlib>

using namespace std;

void RPOSimulatedAnnealing::setSolution(RPOSolution* solution) {
	if (_T <= 0 || _annealingFactor <= 0){
		cerr << "Simulated annealing has not been initialized" << endl;
		exit(-1);
	}

	if (_solution != NULL){
		cerr << "You should not call RPOSimulatedAnnealing::setSolution more than once" << endl;
		exit(1);
	}

	this->_solution = solution;

	if (_bestSolution == NULL){
		_bestSolution = new RPOSolution(*_instance);
	}

	_bestSolution->copy(*solution);
}

void RPOSimulatedAnnealing::run(RPOStopCondition& stopCondition) {

	if (_T <= 0 || _annealingFactor <= 0){
		cerr << "Simulated annealing has not been initialised" << endl;
		exit(-1);
	}

	if (_solution == NULL){
		cerr << "Simulated annealing has not been given an initial solution" << endl;
		exit(-1);
	}

	_results.clear();
	unsigned numParam = _instance->getNumParameters();
	unsigned numIterations = 0;

	/**
	 *
	 * While the stop condition is not met
	 *   1. Generate random indices for an object and a knapsack (including knapsack 0)
	 *   2. Obtain the difference of fitness when applying the change over _solution
	 *   3. Check if the change is accepted
	 *   4. Update the best so-far solution.
	 *   5. If _itsPerAnnealing iterations has passed since the last temperature decrease,
	 *      then you should decrease the temperature
	 */
	while (stopCondition.reached()==false){
		int indexParam = rand() % numParam;
		int paramValue = rand() % (_instance->getLengthD(indexParam));
		double deltaFitness =RPOEvaluator::computeDeltaFitness(*_instance,*_solution,indexParam,paramValue);
		if (accept(deltaFitness)){
			_solution->ChangeParameterTo(indexParam, paramValue);
			_solution->setFitness(_solution->getFitness() + deltaFitness);

			if (RPOEvaluator::compare(_solution->getFitness(), _bestSolution->getFitness()) > 0){
				_bestSolution->copy(*_solution);
			}
		}
		numIterations++;
		_results.push_back(_solution->getFitness());

		if (numIterations % _itsPerAnnealing == 0){
			_T *= _annealingFactor;
		}

		stopCondition.notifyIteration();
	}
}

bool RPOSimulatedAnnealing::accept(double deltaFitness) {
	/**
	 *
	 * Obtain the probability of accepting a change, which is the exponential of
	 * (difference of fitness divided by the temperature)
	 * Generate a random number between 0 and 1
	 * Return true if the random number is lower than the acceptance probability
	 *
	 * (Think about what happens when the difference of fitness is positive and when
	 *  the difference of fitness is negative. You should use the isToBeMinisedMethod
	 *  to interpret correctly the auxDeltaFitness, if the problem is minimization, then
	 *  a negative difference of fitness is good)
	 */

	double prob = exp(deltaFitness/_T);
	double randSample = (((double)rand()) /RAND_MAX);

	if(randSample<prob){
		return true;
	}
	else{
		return false;
	}

}

void RPOSimulatedAnnealing::initialise(double initialProb, int numInitialEstimates, double annealingFactor, unsigned itsPerAnnealing, RPOInstance &instance) {
	_initialProb = initialProb;
	_annealingFactor = annealingFactor;
	_instance = &instance;
	_itsPerAnnealing = itsPerAnnealing;
	int numParam = instance.getNumParameters();
	double averageFDiffs = 0.;

	/**
	 * Initialize the temperature:
	 * - For this, you will have to generate some random solutions and the indices for changes to
	 *   random neighboring solutions
	 * - You will have to obtain the average difference of fitness for changes to worse solutions
	 * - Then, you will have to derive the initial temperature from the equation used for accepting
	 *   worse solutions
	 */

	for (int i = 0; i < numInitialEstimates; i++){
		RPOSolution sol(instance);
		RPOSolGenerator::genRandomSol(instance, sol);
		int indexParam = rand() % numParam;
		int paramValue = rand() % (_instance->getLengthD(indexParam));
		double deltaFitness =RPOEvaluator::computeDeltaFitness(*_instance,sol,indexParam,paramValue);
		averageFDiffs += max(fabs(deltaFitness),10.);
		//There is a minimum difference of 10 to avoid fitness changes too small
		//(for example, when a knapsack is modified and it is not the one leading to
		// maximum violation) (this method could be improved)
	}

	averageFDiffs /= numInitialEstimates;

	_T = -1. * averageFDiffs / log(initialProb);
}
