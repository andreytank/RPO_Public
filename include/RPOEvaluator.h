/**
 * RPOEvaluator.h
 * 
 * File defining the class RPOEvaluator.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef __RPOEvaluator_H__
#define __RPOEvaluator_H__

#include "RPOInstance.h"
#include "RPOSolution.h"

/**
 * Class for obtaining the fitness of a solution of the RPO
 */

class RPOEvaluator {

protected:
	/**
	 * Variable counting the number of solutions that have been evaluated using computeFitness or computeDeltaFitness
	 */
	static unsigned _numEvaluations;

public:
	/**
	 * Function obtaining the fitness of a solution
	 * @param[in] instance Reference to an object with the information of the RPO instance.
	 * @param[in] solution Reference to an object with a RPO solution.
	 *
	 * @return Solution fitness. It will be negative if the capicity constraint is violated for a
	 *                           given knapsack and positive if the solution is feasible.
	 *                           In this case, the value is the sum of the individual and shared profits.
	 */
	static double computeFitness(RPOInstance &instance, RPOSolution &solution);

	/**
	 * Function which obtains the difference of fitness when the solution passed as an argument is modified
	 * by assigning a given object to a given knapsack.
	 * @param[in] instance Reference to an object with information about the instance of the RPO
	 * @param[in] solution Reference to an object with the information of the current solution
	 * @param[in] indexObject Index of the object which would be included in other knapsack
	 * @param[in] indexKnapsack Index of the knapsack in which the object would be included.
	 *                          It can be 0 (the object is taken out from the current knapsack and
	 *                          not included in any other one
	 *
	 * @return Difference of fitness after including the object indexObject in the knapsack indexKnapsack
	 */
	static double computeDeltaFitness(RPOInstance &instance, RPOSolution &solution, int indexParam, int newParamValue);

	/**
	 * Function which resets the internal variable storing the number of evaluations.
	 */
	static void resetNumEvaluations();

	/**
	 * Function for observing the number of times any solution has been evaluated.
	 */
	static unsigned getNumEvaluations() {
		return _numEvaluations;
	}

	/**
	 * Function that compares two fitness values. Its purpose is to specify if the problem is a
	 * minimization problem or a maximization one.
	 * @param[in] f1 first fitness value to compare
	 * @param[in] f2 second fitness value to compare
	 * @return A positive value if the first value is better than the second, a negative if the second
	 *         is better than the first, 0 if they cannot be differentiated
	 */
	static double compare(double f1, double f2){
		return f1 - f2; //correct the value that has to be returned
	}
};

#endif
