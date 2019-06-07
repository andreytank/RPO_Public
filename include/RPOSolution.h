/**
 * RPOSolution.h
 * 
 * File defining the class RPOSolution.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef __RPOSOLUTION_H__
#define __RPOSOLUTION_H__

#ifndef __RPOINSTANCE_H__
#include "RPOInstance.h"
#else
class RPOInstance;
#endif

#ifndef __SOLUTION_H__
#include "Solution.h"
#else
class Solution;
#endif

#include <time.h>
#include <stdlib.h>
#include <vector>

/**
 * Class representing a solution of the problem.
 *
 * About the encoding:
 * The representation will be a vector of integer values: from 1 to M for the objects which are allocated
 * in one of the M knapsacks and 0 for those non included in any knapsack.
 */
class RPOSolution : public Solution {
protected:
	std::vector<int> _sol;
	int _numParametrs;
	double _fitness;
	bool _fitnessAssigned;
	/* Define the properties of the class
	 * _sol Integer vector, which will be the internal representation of the solution.
	 * _numObjs Integer with the number of objects of the problem.
	 * _fitness double value with quality of the solution.
	 */

public:
	/**
	 * Constructor
	 * @param[in] instance Reference to an object with the information of the RPO instance.
	 */
	RPOSolution(RPOInstance &instance);

	~RPOSolution();

	const std::vector<int> getSetofParameters(){
		return _sol;
	}

	void ChangeParameterTo(int parameter, int value);

	int parameterValue(int parameter);

	double getFitness() const;

	void setFitness(double fitness){ _fitness=fitness;}

	/**
	 * Function for knowing whether the fitness of the solution is valid (it will not be valid if an object is moved to other knapsack; and it is valid when the fitness is reasigned)
	 *
	 * READ This method is used to avoid the evaluation of solutions which are a copy of other solutions
	 */
	bool hasValidFitness(){
		return _fitnessAssigned;
	}
	/**
	 * Function for copying the information of the solution from another
	 * @param[in] solution Solution from which the information is taken
	 */
	virtual void copy(Solution &solution);
};

#endif
