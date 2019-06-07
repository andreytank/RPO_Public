/**
 * TournamentSelector.h
 *
 * File defining the class TournamentSelector.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef INCLUDE_TOURNAMENTSELECTOR_H_
#define INCLUDE_TOURNAMENTSELECTOR_H_

#include <Solution.h>
#include <SelectionOperator.h>
#include <vector>

using namespace std;

/**
 * Class implementing the tournament selection
 */
class TournamentSelector : public SelectionOperator{

protected:
	/**
	 * Properties of the class
	 * _k number of contestants in the tournament
	 */
	unsigned _k;

	/**
	 * Function which selects a solution from the set by applying a tournament
	 * @param[in] set Vector of solutions
	 * @result Solution selected
	 */
	Solution* selectOne (vector<Solution*> &set){

		/**
		 *
		 * 1. Select randomly a solution as the current winner
		 *
		 * 2. Repeat (_k-1) times: select randomly another solutions and make a tournament with the current winner (keep the best one)
		 */
		Solution* best=set[rand()%set.size()];
		for(unsigned i=0;i<_k-1;i++){
			Solution* aux=set[rand()%set.size()];
			if(aux->getFitness()>best->getFitness()){
				best=aux;
			}
		}
		return best;
	}

public:

	/**
	 * Constructor
	 * @param[in] k number of contestants in the tournament
	 */
	TournamentSelector(unsigned k){
		_k = k;
	}

	/**
	 * Destructor
	 */
	virtual ~TournamentSelector(){}

	/**
	 * Function which select as many pairs of parents from a vector as the number of elements
	 * in the corresponding vector
	 * @param[in] orig Vector of solutions from which apply the selection
	 * @param[out] result Vector where the pairs of parents selected will be stored
	 */
	virtual void select(vector<Solution*> &orig, vector<Solution*> &result){

		unsigned origSize = (unsigned) orig.size();

		for (unsigned i = 0; i < origSize; i++){
			result.push_back(selectOne(orig));
			result.push_back(selectOne(orig));
		}
	}
};



#endif /* INCLUDE_TOURNAMENTSELECTOR_H_ */
