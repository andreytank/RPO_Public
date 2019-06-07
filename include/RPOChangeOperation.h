/**
 * RPOEvaluator.h
 *
 * File defining the class RPOChangeOperation.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef __RPOCHANGEOPERATION_H__
#define __RPOCHANGEOPERATION_H__

#include <RPOSolution.h>

/**
 * Abstract class for representing any operation modifying a solution
 */
class RPOChangeOperation{
public:

	/**
	 * Destructor
	 */
	virtual ~RPOChangeOperation(){
	}

	/**
	 * Function applying the change defined by the object over the solution received
	 * as a parameter.
	 * @param[in,out] solution Solution objection over which the change will be applied
	 */
	virtual void apply(RPOSolution &solution) = 0;
};

#endif
