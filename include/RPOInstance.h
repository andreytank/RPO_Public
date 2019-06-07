/**
 * RPOInstance.h
 * 
 * File defining the class RPOInstance.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */
#ifndef __RPOINSTANCE_H__
#define __RPOINSTANCE_H__

#ifndef __RPOSOLUTION_H__
#include "RPOSolution.h"
#else
class RPOSolution;
#endif

#include "Func.h"
#include <time.h>
#include <stdlib.h>
#include <vector>

/**
 * Class for storing the information of an instance of the problem RPO
 */
class RPOInstance {
protected:
	int _numParameters;
	Func func;

public:
	/**
	 * Default constructor
	 */
	RPOInstance(int size_param,int numfunc);

	/**
	 * Destructor
	 */
	~RPOInstance();

	/**
	 * Function for reading an input file.
	 * @param[in] filename Name of the file where the data of the instance can be found.
	 * @param[in] numKnapsacks Integer with the number of knapsacks to be considered. This is not read from the file,
	 *                         but established by the user.
	 */

	inline int getLengthD(int indexParameter){
		return func.getWidthOfParameter(indexParameter);
	}

	inline int getNumParameters(){
		return _numParameters;
	}

	double getFitness(RPOSolution &solution);
	void randomPermutation(int numParam, vector<int>& perm);
};

#endif

