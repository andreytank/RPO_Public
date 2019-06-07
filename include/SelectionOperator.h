/**
 * SelectionOperator.h
 *
 * File defining the class SelectionOperator.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef INCLUDE_SELECTIONOPERATOR_H_
#define INCLUDE_SELECTIONOPERATOR_H_

#include <vector>
#include <Solution.h>

using namespace std;

/**
 * Abstract class which defines the operations that every selection operator should have
 * for an evolutionary algorithm
 */
class SelectionOperator {
public:
	/**
	 * Destructor
	 */
	virtual ~SelectionOperator(){}

	/**
	 * Function to select a set of solutions from a given vector
	 * @param[in] orig Vector of solutions over which the selection will be applied
	 * @param[out] result Vector where the selected solutions will be stored
	 */
	virtual void select(vector<Solution*> &orig, vector<Solution*> &result) = 0;
};



#endif /* INCLUDE_SELECTIONOPERATOR_H_ */
