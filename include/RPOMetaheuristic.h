/**
 * RPOMetaheuristic.h
 *
 * File defining the class RPOMetaheuristic.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef INCLUDE_RPOMETAHEURISTIC_H_
#define INCLUDE_RPOMETAHEURISTIC_H_

#include <RPOSolution.h>
#include <RPOStopCondition.h>
#include <cstdlib>

/**
 * Abstract class defining the common methods for metaheuristics for the RPO.
 */
class RPOMetaheuristic {

protected:
	RPOSolution *_bestSolution;

public:
	/**
	 * Constructor
	 */
	RPOMetaheuristic(){
		_bestSolution = NULL;
	}

	/**
	 * Destructor
	 */
	virtual ~RPOMetaheuristic(){

		if (_bestSolution != NULL){
			delete _bestSolution;
			_bestSolution = NULL;
		}
	};

	/**
	 * Function which runs the metaheuristic until reaching the stop condition
	 * @param[in] stopCondition Stop condition of the metaheuristic
	 */
	virtual void run (RPOStopCondition &stopCondition) = 0;


	/**
	 * Function which returns the best solution found up to the moment.
	 * It is the responsability of each metaheuristic to keep udpate the
	 * property _bestSolution
	 * @return Best solution found up the moment
	 */
	RPOSolution*& getBestSolution(){
		return _bestSolution;
	}
};



#endif /* INCLUDE_RPOMETAHEURISTIC_H_ */
