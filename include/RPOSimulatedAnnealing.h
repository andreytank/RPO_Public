/**
 * RPOSimulatedAnnealing.h
 *
 * File defining the class RPOSimulatedAnnealing.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef INCLUDE_RPOSIMULATEDANNEALING_H_
#define INCLUDE_RPOSIMULATEDANNEALING_H_

#include <RPOSolution.h>
#include <RPOInstance.h>
#include <RPOMetaheuristic.h>
#include <RPOStopCondition.h>
#include <cstdlib>

/**
 * Class implementing the Simulated Annealing metaheuristic for RPO.
 */
class RPOSimulatedAnnealing : public RPOMetaheuristic {

protected:
	/**
	 * Properties of the class
	 * _T stores the current temperature of the system
	 * _initialProb stores the initial probability that a change to a worse solution is accepted, in average
	 * _annealingFactor stores the factor used for decreasing the temperature
	 * _solution stores the solution considered by Simulated Annealing
	 * _instance is a pointer to the instance of the problem
	 */
	double _T;
	double _initialProb;
	double _annealingFactor;
	unsigned _itsPerAnnealing;
	RPOSolution *_solution;
	RPOInstance *_instance;

	/**
	 * Vector of doubles with the quality of the last solution evaluated
	 */
	vector<double> _results;

	/**
	 * Function which decides if a new solution is accepted according to its
	 * difference in fitness with respect to the current solution
	 * @param[in] deltaFitness Difference of fitness of the new solution with
	 *                         respect to the current one
	 * @return true if the new solution is accepted, false otherwise
	 */
	bool accept(double deltaFitness);

public:
	/**
	 * Constructor
	 */
	RPOSimulatedAnnealing(){
		_T = 0;
		_initialProb = 0;
		_annealingFactor = 0;
		_itsPerAnnealing = 0;
		_solution = NULL;
		_instance = NULL;
	}

	/**
	 * Destructor
	 */
	virtual ~RPOSimulatedAnnealing(){

		if (_bestSolution != NULL){
			delete _bestSolution;
			_bestSolution = NULL;
		}
	};

	/**
	 * Function which runs the metaheuristic until reaching the stop condition
	 * @param[in] stopCondition Stop condition of the metaheuristic
	 */
	virtual void run(RPOStopCondition &stopCondition);

	/**
	 * Function which assigns the initial solution for the metaheuristic
	 * @param[in] solution Initial solution over which the simulated annealing is applied
	 */
	void setSolution(RPOSolution* solution);

	/**
	 * Function which initializes some parameters of the metaheuristic.
	 * Specifically, it calculates the initial temperature of the system according to the
	 * probability with which the first changes to worse solutions want to be accepted.
	 * @param[in] initialProb Probability with which the changes to worse solutions want
	 *                        to be accepted in average
	 * @param[in] numInitialEstimates Number of initial solutions over which the temperature
	 *                                necessary to accept changes to worse solutions is estimated,
	 *                                using the probability initialProb
	 * @param[in] annealingFactor Factor used for decreasing the temperature
	 * @param[in] itersPerAnnealing Number of iterations between two temperature changes
	 * @param[in] instance Instance of the problem to be tackled
	 */
	void initialise(double initialProb, int numInitialEstimates, double annealingFactor, unsigned itsPerAnnealing, RPOInstance &instance);

	/**
	 * Function returning the array with the results of the solutions accepted, in each step,
	 * by the metaheuristic
	 * @return vector with the results of the solutions accepted, in each step,
	 *         by the metaheuristic
	 */
	vector<double>& getResults() {
		return _results;
	}
};


#endif /* INCLUDE_RPOSIMULATEDANNEALING_H_ */
