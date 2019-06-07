/**
 * RPONeighExplorer.h
 *
 * File defining the class RPONeighExplorer.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef INCLUDE_RPONEIGHEXPLORER_H_
#define INCLUDE_RPONEIGHEXPLORER_H_

#include <RPOSolution.h>
#include <RPOInstance.h>
#include <RPOChangeOperation.h>

/**
 * Abstract class defining the operation of any operator which explores the neighbourhood of a given solution
 */
class RPONeighExplorer {
public:

	/**
	 * Destructor
	 */
	virtual ~RPONeighExplorer(){}

	/**
	 * Function for searching an operation which, applied to the solution, returns other neighbour solution.
	 * It will be used to search one neighbour solution which improves the current solution or to search
	 * the best of all the possible neighbour solutions.
	 *
	 * @param[in] instance Instance of the RPO
	 * @param[in] solution Reference to an object with a RPO solution, whose neighbourhood is going to be explored.
	 * @param[out] operation Operator selected after exploring the neighbourhood.
	 *
	 * @return It returns true if a valid operation is found, improving the current solution. False in the opposite case.
	 */
	virtual bool findOperation(RPOInstance &instance, RPOSolution &solution, RPOChangeOperation &operation) = 0;
};



#endif /* INCLUDE_RPONEIGHEXPLORER_H_ */
