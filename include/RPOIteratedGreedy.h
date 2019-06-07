/**
 * RPOIteratedGreedy.h
 *
 * File defining the class RPOIteratedGreedy.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef INCLUDE_RPOITERATEDGREEDY_H_
#define INCLUDE_RPOITERATEDGREEDY_H_

#include <RPOMetaheuristic.h>
#include <RPOSolution.h>
#include <RPOObjectAssignmentOperation.h>

/**
 * Class implementing the Iterated Greedy metaheuristic for RPO, in the variant that the
 * destruction is always applies to the best solution and local search is not considered
 */
class RPOIteratedGreedy : public RPOMetaheuristic{

protected:

	/**
	 * Properties of the class
	 * _alpha stores the probability that the objects are removed from their corresponding knapsacks
	 * _instance is a pointer to the instance of the problem
	 * _sol stores the solution considered by Iterated Greedy, during the destructions and constructions
	 */
	double _alpha;
	RPOInstance *_instance;
	RPOSolution *_sol;

	/**
	 * Vector of doubles with the quality of the last solution evaluated
	 */
	vector<double> _results;

	/**
	 * Function returning the best assignment operation of a non-assigned object to a knapsack
	 * from a series of randomly selected alternatives
	 * @param[out] operation Assignment operation of an object to a knapsack
	 */
	void chooseOperation(RPOObjectAssignmentOperation &operation);

	/**
	 * Function which rebuilds the solution _sol. For this, it iteratively calls to the
	 * function chooseOperation until it does not find any other assignment
	 * operation improving the current solution. The changes are stored in the property
	 * _sol
	 */
	void rebuild();

	/**
	 * Function which partially destroys the solution _sol. For this, it removes objects
	 * from their knapsacks with probability _alpha
	 */
	void destroy();

public:
	/**
	 * Constructor
	 */
	RPOIteratedGreedy(){
		_alpha = 0.;
		_instance = NULL;
		_sol = NULL;
	}

	/**
	 * Destructor
	 */
	virtual ~RPOIteratedGreedy(){

		if (_bestSolution != NULL){
			delete _bestSolution;
			_bestSolution = NULL;
		}

		if (_sol != NULL){
			delete _sol;
			_sol = NULL;
		}
	}

	/**
	 * Function which initializes some parameters of the metaheuristic.
	 * Specifically, the properties _sol and _bestSolution and the probability  alpha
	 * @param[in] alpha Probability of removing an object from its knapsack during the destruction phase
	 * @param[in] instance Instance of the problem to be tackled
	 */
	void initialise(double alpha, RPOInstance &instance);

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



#endif /* INCLUDE_RPOITERATEDGREEDY_H_ */
