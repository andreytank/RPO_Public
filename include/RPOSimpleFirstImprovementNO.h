/**
 * RPOSimpleFirstImprovementNO.h
 *
 * File defining the class RPOSimpleFirstImprovementNO.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef INCLUDE_RPOSIMPLEFIRSTIMPROVEMENTNO_H_
#define INCLUDE_RPOSIMPLEFIRSTIMPROVEMENTNO_H_

#include <RPONeighExplorer.h>
#include <RPOSolution.h>
#include <RPOChangeOperation.h>
/**
 * Class which explores the neighbourhood of a given solution, returning the first assignment operation of an
 * object to a knapsack which improves the current solution. It returns FALSE if there is not assignment
 * improving the solution.
 */
class RPOSimpleFirstImprovementNO : public RPONeighExplorer{

public:

	/**
	 * Destructor
	 */
	virtual ~RPOSimpleFirstImprovementNO(){

	}

	/**
	 * Function exploring the neighbourhood of a given solution, returning the first assignment operation of an object
	 * to a knapsack which improves the current solution. It returns FALSE if there is not assignment
	 * improving the solution.
	 * @param[in] instance Instance of the problem
	 * @param[in] solution Solution whose neighbourhood is going to be explored
	 * @param[out] operation Operation which, after being applied to the solution, obtains the first improvement.
	 *
	 * @return true if there is an operation which improves the solution; false otherwise
	 */
	virtual bool findOperation(RPOInstance &instance, RPOSolution &solution, RPOChangeOperation &operation);
};


#endif /* INCLUDE_RPOSIMPLEFIRSTIMPROVEMENTNO_H_ */
