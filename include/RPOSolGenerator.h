/**
 * RPOSolGenerator.h
 * 
 * File defining the class RPOSolGenerator.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef __RPOSOLGENERATOR_H__
#define __RPOSOLGENERATOR_H__

#include "RPOInstance.h"
#include "RPOSolution.h"
#include <time.h>
#include <stdlib.h>

/**
 * Class for generating a random solution of the RPO
 *
 * About the representation of the solutions:
 * The representation will be a vector of integer values: from 1 to M for the objects which are allocated
 * in one of the M knapsacks and 0 for those non included in any knapsack.
 */
class RPOSolGenerator {

public:

	/**
	 * Function which generates a random solution for the RPO
	 * @param[in] instance Reference to an object with the information of the RPO instance.
	 * @param[out] solution Reference to an object representing a solution to the problem.
     *             IMPORTANT NOTE: it has to be correctly initialize. Specifically, its internal
     *             vector has to be previously reserved.
	 */
	static void genRandomSol(RPOInstance &instance, RPOSolution &solution);
};

#endif
