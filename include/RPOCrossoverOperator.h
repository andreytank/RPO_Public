/**
 * RPOCrossoverOperator.h
 *
 * File defining the class RPOCrossoverOperator.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef INCLUDE_RPOCROSSOVEROPERATOR_H_
#define INCLUDE_RPOCROSSOVEROPERATOR_H_

#include <RPOSolution.h>
#include <Solution.h>
#include <RPOInstance.h>
#include <vector>

using namespace std;

/**
 * Class which implements a uniform crossover operator for the RPO. The basic idea is similar
 * to the uniform crossover for integer coding
 */
class RPOCrossoverOperator {

protected:
	/**
	 * Properties of the class:
	 *  - _instance Instance of the problem considered. It is used only for creating new objects RPOSolution
	 *  - _numObjs Stores the number of objects in the instance considered to reduce the number of queries
	 *             to the instance object
	 *  - _crossProb crossover probability
	 */
	RPOInstance *_instance;
	unsigned _numParam;
	double _crossProb;

	/**
	 * Function which cross two solutions according to the crossover probability. In case the crossover is not
	 * considered, the solution will be the first parent
	 * @param[in] s1 first parent
	 * @param[in] s2 second parent
	 * @return New object with the solution descendant after considering the crossover of s1 and s2.
	 *         The solution is dynamically allocated in memory. It is the responsibility of the one invoking
	 *         this method to manage this memory correctly.
	 */
	RPOSolution * cross(Solution *s1, Solution *s2) {
		RPOSolution * sol = new RPOSolution(*_instance);
		RPOSolution * sol1 = (RPOSolution *) s1;
		RPOSolution * sol2 = (RPOSolution *) s2;

		double randSample = (((double) rand()) / RAND_MAX);

		if (randSample < _crossProb) {

			//Uniform crossover of two parents, where the value of the parents
			//is randomly chosen for each gene
			for (unsigned i = 0; i < _numParam; i++) {
				unsigned paramValue = sol1->parameterValue(i);

				randSample = (((double) rand()) / RAND_MAX);

				if (randSample < 0.5)
					paramValue = sol2->parameterValue(i);

				sol->ChangeParameterTo(i, paramValue);
			}
		} else {
			//If there is no crossover, copy the first parent
			sol->copy(*sol1);
		}

		return sol;
	}

public:

	/**
	 * Constructor
	 * @param[in] crossProb Crossover probability
	 * @param[in] instance Instance of the problem considered
	 */
	RPOCrossoverOperator(double crossProb, RPOInstance &instance) {
		_instance = &instance;
		_numParam = instance.getNumParameters();
		_crossProb = crossProb;
	}

	/**
	 * Function which applies crossover to a population of parents
	 * @param[in] parents Parent vector. The crossover is applied to every two consecutive parents (1,2), (3,4)...
	 * @param[out] offspring Vector where the descendants are stored.
	 * IMPORTANT: this function allocates memory dynamically for the new solutions in offspring,
	 * so it is the responsibility of the one invoking this method to manage this memory correctly.
	 */
	void cross(vector<Solution*> &parents, vector<Solution*> &offspring) {

		unsigned numParents = (unsigned) parents.size();

		//Apply crossover considering every two consecutive pairs (1,2), (3,4)...
		for (unsigned i=0; i < numParents; i+=2) {
			RPOSolution *sol = cross(parents[i], parents[i+1]);
			offspring.push_back(sol);
		}
	}
};

#endif /* INCLUDE_RPOCROSSOVEROPERATOR_H_ */
