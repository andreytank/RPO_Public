/**
 * RPOMutationOperator.h
 *
 * File defining the class RPOMutationOperator.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef INCLUDE_RPOMUTATIONOPERATOR_H_
#define INCLUDE_RPOMUTATIONOPERATOR_H_

#include <RPOSolution.h>
#include <vector>
#include <cstdlib>

using namespace std;

/**
 * Class which defines a mutation operator for the RPO. It is based on iterating over all the
 * genes in the solutions and changing by a value randomly generated according to a probability
 */
class RPOMutationOperator{

protected:
	/**
	 * Properties of the class:
	 * _mutProb Mutation probability
	 * _numObjs Number of objects. It is used to reduce the number of queries to the instance
	 * _numKnapsacks Number of knapsack of the problem. It is used to reduce the number of
	 *               queries to the instance
	 */
	double _mutProb;
	unsigned _numParam;
	RPOInstance *_instance;
	/**
	 * Function which mutates a solution
	 * @param[in,out] sol Solution to be mutated
	 */
	void mutate(Solution* sol){
		RPOSolution *s = (RPOSolution*) sol;

		//Iterate over the objects and, according to a mutation probability, assign them
		//to a random knapsack (0, 1 or more than 1 genes could be modified)
		for(unsigned i=0; i< this->_numParam ;i++){
			double randSample=(double)rand()/RAND_MAX;
			if(randSample < _mutProb){
				s->ChangeParameterTo(i, rand()%(_instance->getLengthD(i)));
			}
		}
	}

public:
	/**
	 * Constructor
	 * @param[in] mutProb Mutation probability
	 * @param[in] instance Instance of the problem to be considered
	 */
	RPOMutationOperator(double mutProb, RPOInstance &instance){
		_mutProb = mutProb;
		_numParam = instance.getNumParameters();
		_instance=&instance;
	}

	/**
	 * Function which mutates a set of solutions
	 * @param[in,out] sols Solutions to be mutated
	 */
	void mutate(vector<Solution*> &sols){

		for (Solution* sol : sols){
			mutate(sol);
		}
	}
};



#endif /* INCLUDE_RPOMUTATIONOPERATOR_H_ */
