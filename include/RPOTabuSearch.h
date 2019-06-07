/**
 * RPOTabuSearch.h
 *
 * File defining the class RPOTabuSearch.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef INCLUDE_RPOTABUSEARCH_H_
#define INCLUDE_RPOTABUSEARCH_H_

#include <RPOSolution.h>
#include <RPOInstance.h>
#include <RPOMetaheuristic.h>
#include <RPOStopCondition.h>
#include <cstdlib>
#include <queue>
#include <set>

using namespace std;

/**
 * Class implementing the Tabu Search metaheuristic for RPO. It uses only the short
 * term memory which avoids any operation over an object over which an operation
 * was recently applied
 */

class RPOTabuSearch : public RPOMetaheuristic{
protected:

	/**
	 * Properties of the class
	 * _solution RPO solution over which the tabu search is working
	 * _instance is a pointer to the instance of the problem
	 * _shortTermMem this a queue implementing the short term memory of tabu search
	 * _shortTermMem_aux this is a auxiliary structure to make more efficient checking if
	 *                   an element is found in the tabu memory
	 * _tabuTennure Tabu tennure of the metaheuristic
	 */
	RPOSolution *_solution;
	RPOInstance *_instance;
	queue<unsigned> _shortTermMem;
	set<unsigned> _shortTermMem_aux;
	unsigned _tabuTennure;

	/**
	 * Vector of doubles with the quality of the last solution evaluated
	 */
	vector<double> _results;

public:
	/**
	 * Constructor
	 */
	RPOTabuSearch(){
		_solution = NULL;
		_instance = NULL;
		_tabuTennure = 0;
	}

	/**
	 * Destructor
	 */
	virtual ~RPOTabuSearch(){

		if (_bestSolution != NULL){
			delete _bestSolution;
			_bestSolution = NULL;
		}
	}

	/**
	 * Function which initializes some parameters of the metaheuristic.
	 * @param[in] instance Instance of the problem to be tackled
	 * @param[in] tabuTennure Tabu tennure of the metaheuristic
	 */
	void initialise(RPOInstance * instance, unsigned tabuTennure);

	/**
	 * Function which assigns the solution over which the tabu search will work
	 * @param[in] solution Initial solution over which the tabu search is applied
	 */
	void setSolution(RPOSolution* solution);

	/**
	 * Function which runs the metaheuristic until reaching the stop condition
	 * @param[in] stopCondition Stop condition of the metaheuristic
	 */
	virtual void run(RPOStopCondition &stopCondition);

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

#endif
