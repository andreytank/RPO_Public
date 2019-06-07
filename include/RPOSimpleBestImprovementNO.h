/**
 * RPOSimpleBestImprovementNO.h
 *
 * File defining the class RPOSimpleBestImprovementNO.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef INCLUDE_RPOSIMPLEBESTIMPROVEMENTNO_H_
#define INCLUDE_RPOSIMPLEBESTIMPROVEMENTNO_H_

#include <RPONeighExplorer.h>
#include <RPOSolution.h>
#include <RPOChangeOperation.h>
/**
 * Class which explores the neighbourhood of a given solution, returning the best assignment operation of an
 * object to a knapsack from all possible assignments. It returns FALSE if the assignment found does not
 * improve current solution.
 */
class RPOSimpleBestImprovementNO : public RPONeighExplorer{

public:

	/**
	 * Destructor
	 */
	virtual ~RPOSimpleBestImprovementNO(){

	}

	/**
	 * Function exploring the neighbourhood of a given solution, returning the best assignment operation of an object
	 * to a knapsack from all possible assignments. It returns FALSE if the assignment found does not
	 * improve current solution.
	 * @param[in] instance Instance of the problem
	 * @param[in] solution Solution whose neighbourhood is going to be explored
	 * @param[out] operation Operation which, after being applied to the solution, obtains the maximum improvement.
	 *
	 * @return true if the operation improves the solution; false, if there is no operation which improves the solution
	 */
	virtual bool findOperation(RPOInstance &instance, RPOSolution &solution, RPOChangeOperation &operation);

};


#endif /* INCLUDE_RPOSIMPLEBESTIMPROVEMENTNO_H_ */
