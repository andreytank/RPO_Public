/**
 * RPOAntColonyOpt.h
 *
 * File defining the class RPOAntColonyOpt.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef INCLUDE_RPOANTCOLONYOPT_H_
#define INCLUDE_RPOANTCOLONYOPT_H_

#include <RPOMetaheuristic.h>
#include <RPOInstance.h>
#include <RPOSolution.h>
#include <RPOObjectAssignmentOperation.h>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <cmath>
#include <numeric>
#include <cstdlib>

using namespace std;

/**
 * Class implementing an ant colony system for the RPO.
 */

class RPOAntColonyOpt: public RPOMetaheuristic {

	/**
	 * Class which internally implements an ant for the RPO
	 */
	class RPOAnt {
	protected:

		/**
		 * Properties of the class
		 * Basic:
		 *  _colony pointer to the class which includes it (its colony)
		 *  _sol solution for which the ant is working
		 *
		 * Additional:
		 *  _objectsLeft Set of objects which have not been included in any knapsack
         *               This variable is used for accelerating the process of construction
         *               of solutions. Its objective is making the process more efficient
		 *  _candidateListSize Number of options to consider when including a new component in
		 *                     the solution. This variable also accelerate the construction process
		 *                     avoiding the consideration of all possible solutions.
		 */
		RPOAntColonyOpt *_colony;
		RPOSolution *_sol;
		unordered_set<unsigned> _paramsLeft;
		double _candidateListSize;

		/**
		 * Function which sums the values of a vector. It will be used to know the sum of the values
		 * of relevance from the different options the ant has
		 * @param[in] significances Vector with the relevance of the options of the ant
		 * @return sum of the values of the relevance vector
		 */
		double sumSignificances(vector<double> &significances) {
			int sum= std::accumulate(significances.begin(), significances.end(), 0);

			return sum;
		}

		/**
		 * Functions which returns the set of alternatives that an ant has when adding a new component
		 * to its solution.
		 * @param[out] alternatives Vector where the different alternative are stored
		 * @param[out] significances Vector with the relevance of the different alternatives created
		 */
		void createAlternatives(
				vector<RPOObjectAssignmentOperation*> &alternatives,
				vector<double> &significances) {

			//Obtain the information of the ant
			RPOInstance *instance = _colony->_instance;
			double alpha = _colony->_alpha;
			double beta = _colony->_beta;
			vector<vector<double>*> &phMatrix = _colony->_phMatrix;

			unsigned numTries = 0;

			//For each object which is not in a knapsack
			for (auto indexParam : _paramsLeft) {
				unsigned dLength=instance->getLengthD(indexParam);
				//For each possible knapsack and without reaching the maximum number of tries
				for (unsigned j = 0;
						j < dLength && numTries < _candidateListSize;
						j++) {
					int paramValue=j;
					//If the movement violates the capacity of the knapsack, discard it
					if(paramValue==_sol->parameterValue(indexParam))
						continue;

					//Obtain the deltaFitness and count this as a try (given that we have previously checked
					//that there is no violation, the deltaFitness will be DeltaSumProfits)
					double deltaFitness = RPOEvaluator::computeDeltaFitness(*instance,*_sol,
							indexParam, paramValue);
					numTries++;

					//Ignore those options with worse fitness or the same fitness
					//(they should not happen if the profits are always positive, but
					// just in case...)
					if (deltaFitness <= 0)
						continue;

					/**
					 * 1. Create the operation
					 * 2. Obtain its relevance as densityOfTheObject^beta * amountOfPheromona^alpha
					 * 3. Include the operation in the alternatives and the relevance in significances
					 */
					RPOObjectAssignmentOperation *al =
							new RPOObjectAssignmentOperation();
					double density = deltaFitness
							/ fabs(paramValue-_sol->parameterValue(indexParam));
					al->setValues(indexParam, paramValue, deltaFitness);
					alternatives.push_back(al);
					double significance = pow(density, beta)
							* pow(phMatrix.at(indexParam)->at(j), alpha);
					significances.push_back(significance);
				}
			}
		}

		/**
		 * Function returning the best alternative of the ant when including a new component
		 * in the considered solution, from a set of alternatives randomly evaluated.
		 * @param[out] op Alternative selected as the best one for the ant
		 */
		void selectBestAlternative(RPOObjectAssignmentOperation &op) {

			//Obtain the information of the colony
			RPOInstance *instance = _colony->_instance;
			vector<vector<double>*> &phMatrix = _colony->_phMatrix;
			double beta = _colony->_beta;
			double alpha = _colony->_alpha;

			double bestSignificance = -1;
			unsigned numTries = 0;

			//For each object which is not in the any knapsack
			for (auto indexParam : _paramsLeft) {
				unsigned dLength=instance->getLengthD(indexParam);
				//For each possible knapsack and without reaching the maximum number of tries
				for (unsigned j = 0;
					 j < dLength && numTries < _candidateListSize;
					 j++) {
					int paramValue=j;
					//If the movement violates the capacity of the knapsack, discard it
					if(paramValue==_sol->parameterValue(indexParam))
						continue;
                    numTries++;
					//Obtain the deltaFitness and count this as a try (given that we have previously checked
					//that there is no violation, the deltaFitness will be DeltaSumProfits)
					double deltaFitness = RPOEvaluator::computeDeltaFitness(*instance,*_sol,
																			 indexParam, paramValue);
					// Ignore those options with worse fitness or the same fitness
					// by using a "continue"
					//(they should not happen if the profits are always positive, but
					// just in case...)
					if (deltaFitness <= 0)continue;

					/**
					 * 1. Obtain its relevance as densityOfTheObject^beta * amountOfPheromone^alpha
					 * 2. If it is better than the best-so-far solution, store it in op
					 */
					double density = deltaFitness
									 / fabs(paramValue-_sol->parameterValue(indexParam));
					double significance = pow(density, beta)* pow(phMatrix.at(indexParam)->at(j), alpha);
					if (significance>bestSignificance)
					{
						bestSignificance=significance;
						op.setValues(indexParam,paramValue,deltaFitness);
					}
				}
			}
		}

		/**
		 * Function which frees the memory of the alternatives created by the ant
		 * @param[in,out] alt Vector with the alternatives to be freed
		 */
		void freeAlternatives(vector<RPOObjectAssignmentOperation*> &alt) {

			for (auto altOp : alt) {
				delete altOp;
			}

			alt.clear();
		}

	public:
        /**
		 * Constructor of an ant
		 * @param[in] candidateListSize Number of solutions to be evaluated in each step
		 * @param[in] colony Pointer to the colony which includes this ant
		 */
		RPOAnt(unsigned candidateListSize, RPOAntColonyOpt *colony) {
			_colony = colony;
			_sol = new RPOSolution(*(colony->_instance));
			_candidateListSize = candidateListSize;
		}

		/**
		 * Destructor
		 */
		~RPOAnt() {
		    _colony=NULL;
		}

		/**
		 * Function which rests the memory of the ant so that it can start generating
		 * a solution from the beginning
		 */
		void resetSolution() {

			/**
			 *
			 * 1. Assign all the objects to the knapsack 0 and insert them in the memory _objectsLeft
			 * 2. Assign a zero fitness
			 */
			 for(int i=0;i<_colony->_instance->getNumParameters();i++) {
				 _sol->ChangeParameterTo(i, 0);
			 	_paramsLeft.insert(i);
			 }_sol->setFitness(RPOEvaluator::computeFitness(*(_colony->_instance),*_sol));
		}

		/**
		 * Function which makes the ant select an alternative and include it in its solution.
		 * It also returns the option selected
		 * @param[out] operation Assignment operation of an object to a knapsack selected by the ant
		 */
		void chooseOperation(RPOObjectAssignmentOperation &operation) {

			//Decide between using the best alternative or selecting the alternative according to probabilities
			double randSample =(((double) rand()) / RAND_MAX);

			if (randSample < _colony->_q0) {
				selectBestAlternative(operation);
			} else {

				//Create the alternatives
				vector<RPOObjectAssignmentOperation*> alternatives;
				vector<double> significances;
				createAlternatives(alternatives, significances);

				//If the ant did not find alternatives, exit
				if (significances.size() <= 0) {
					return;
				}

				//Select one of the alternatives according to some probabilities which
				//are proportional to the relevances
				double v_sumSignificances = sumSignificances(significances);
				double randSample = (((double) rand()) / RAND_MAX)
						* v_sumSignificances;
				randSample -= significances.at(0);
				unsigned opSelected = 0;

				while (randSample>0) {
					opSelected+=1;
					randSample-=significances.at(opSelected);
				}

				//Assign the alternative selected in opSelected
				unsigned indexParam = alternatives.at(opSelected)->getParam();
				unsigned paramValue =
						alternatives.at(opSelected)->getparamValue();
				double deltaFitness =
						alternatives.at(opSelected)->getDeltaFitness();
				operation.setValues(indexParam, paramValue, deltaFitness);

				//Free the memory of the alternatives
				freeAlternatives(alternatives);
			}

			//If an alternative was selected, apply it to the solution
            //and delete the corresponding object from _objectsLeft
			if (operation.getParam() >= 0) {
				operation.apply(*_sol);
				_paramsLeft.erase(operation.getParam());
			}
		}

		/**
		 * Function which returns the solution constructed by the ant
		 * @return Solution constructed by the ant
		 */
		RPOSolution & getSolution() {
			return *_sol;
		}
	};

protected:

	/**
	 * Properties of the ant colony
	 *  _q0 Probability that an ant select the best alternative instead of a random one
	 *  _alpha Importance of the amount of pheromone when calculating the relevance of each alternative
	 *  _beta Importance of the heuristic information when obtaining the relevance of each alternative
	 *  _initTau Amount of initial pheromone in the surrounding (it should not be 0)
	 *  _evaporation Percentage of pheromone which evaporates
	 *  _phMatrix 2D pheromone matrix. The first index is for the objects of the problems. The second
	 *            index is for the knapsacks
	 *  _ants Vector with the ants of the colony
	 *  _instance Instance of the problem considered
	 */
	double _q0;
	double _alpha;
	double _beta;
	double _initTau;
	double _evaporation;
	vector<vector<double>*> _phMatrix;
	vector<RPOAnt*> _ants;
	RPOInstance *_instance;

	/**
	 * Vectors where the results will be stored
	 *  _results Fitness values of the solutions generated
	 *  _bestPerIteration Best fitness obtained in each iteration
	 *  _currentItMeans Average fitness value of the solutions generated in each iteration
	 */
	vector<double> _results;
	vector<double> _bestPerIteration;
	vector<double> _currentItMeans;

	/**
	 * Function which performs a local update of the pheromone (when an ant walks, it takes
	 * part of the pheromone)
	 * @param[in] op Option which the ant selected, for which the update is going to be applied
	 */
	void localUpdate(RPOObjectAssignmentOperation &op) {
		// New pheremone value has to be stored in the matrix _phMatrix
		// The new value will be equal to ((1-_evaporation)*oldValue)+(evaporation*_initTau)
		double oldvalue = _phMatrix.at(op.getParam())->at(op.getparamValue());
		_phMatrix.at(op.getParam())->at(op.getparamValue())
		= (1 - _evaporation)*oldvalue + _evaporation*_initTau;
	}

	/**
	 * Function which releases ants for constructing their solutions
	 */
	void releaseAnts() {

		unordered_set<unsigned> movingAnts;
		unordered_set<unsigned> stoppedAnts;
		int i = 0;

		//Reset the solutions of each ant (using the corresponding method)
		// and insert the index in moving ants
		for (auto ant : _ants) {
			ant->resetSolution();
			movingAnts.insert(i);
			i++;
		}

		//While there are ants which are moving
		while (movingAnts.size() > 0) {
			stoppedAnts.clear();

			//Move each ant
			for (auto iAnt : movingAnts) {
				RPOAnt *ant = _ants[iAnt];
				RPOObjectAssignmentOperation op;
				op.setValues(-1, -1, 0);
				ant->chooseOperation(op); // Choose the operation calling to the method

				//If the ant has moved, then apply the local update of the pheromone.
				//If not, insert it in stoppedAnts to be removed from from movingAnts
				if (op.getParam() != -1) {
					localUpdate(op);
				} else {
					stoppedAnts.insert(iAnt);
				}
			}

			for (auto iAnt : stoppedAnts) {
				movingAnts.erase(iAnt);
			}
		}

		//Update the best solution
		double bestFitness = _bestSolution->getFitness();

		for (auto ant : _ants) {
			RPOSolution &sol = ant->getSolution();
			double currentFitness = ant->getSolution().getFitness();

			if (RPOEvaluator::compare(currentFitness, bestFitness) > 0) {
				_bestSolution->copy(sol);
				bestFitness = currentFitness;
			}
		}
	}

	/**
	 * Function which stores statistics about the solutions generated in the property vectors
	 */
	void saveStatistics() {

		RPOSolution &firstSol = _ants.at(0)->getSolution();
		double bestFitness = firstSol.getFitness();
		double meanFitness = 0.;
		unsigned numAnts = (unsigned) _ants.size();
		double inverseNumAnts = 1. / numAnts;

		for (auto ant : _ants) {
			RPOSolution &sol = ant->getSolution();
			double currentFitness = sol.getFitness();
			_results.push_back(currentFitness);
			meanFitness += (currentFitness * inverseNumAnts);

			if (RPOEvaluator::compare(currentFitness, bestFitness) > 0) {
				bestFitness = currentFitness;
			}
		}

		_bestPerIteration.push_back(bestFitness);
		_currentItMeans.push_back(meanFitness);
	}

	/**
	 * Function which runs a iteration of the ACO algorithm, that is:
	 * 1. Release the ants to construct their solutions.
     * 2. Update the pheromone matrix based on the best solution.
	 */
	void iterate() {

		//Release the ants
		releaseAnts();
		saveStatistics();

		//Apply pheromone with the best solution
		unsigned numParams = _instance->getNumParameters();
		double fitness = _bestSolution->getFitness();

		//For each object, deposit the pheromone of the best solution
		// in the pair (object,knapsack) where the object is positioned.
		// The new value is equal to (1-_evaporation) * oldValue + (_evaporation * fitness)
		// The new value is assigned only if it is higher than _initTau
		for (unsigned i = 0; i < numParams; i++) {
			double oldvalue = _phMatrix.at(i)->at(_bestSolution->parameterValue(i));
			double newvalue = (1 - _evaporation)*oldvalue + _evaporation*fitness;

			if(newvalue > _initTau){
				_phMatrix.at(i)->at(_bestSolution->parameterValue(i)) = newvalue;
			}
		}
	}

public:
	/**
	 * Constructor
	 */
	RPOAntColonyOpt() {
		_bestSolution = NULL;
		_q0 = 0.8;
		_alpha = 1;
		_beta = 1;
		_initTau = 0.1;
		_evaporation = 0.1;
		_instance = NULL;
	}

	/**
	 * Destructor
	 */
	~RPOAntColonyOpt() {

		if (_bestSolution != NULL) {
			delete _bestSolution;
			_bestSolution = NULL;
		}

		for (auto ant : _ants) {
			delete ant;
		}
		_ants.clear();

		for (auto vector : _phMatrix) {
			vector->clear();
			delete vector;
		}

		_phMatrix.clear();
	}

	/**
	 * Function which initializes the algorithm
	 * @param[in] numAnts Number of ants in the colony
	 * @param[in] q0 Probability that an ant select the best alternative instead of a random one
	 * @param[in] alpha Importance of the amount of pheromone when calculating the relevance of each alternative
	 * @param[in] beta Importance of the heuristic information when obtaining the relevance of each alternative
	 * @param[in] initTau Amount of initial pheromone in the surrounding (it should not be 0)
	 * @param[in] evaporation Percentage of pheromone which evaporates
	 * @param[in] candidateListSize Number of alternative that each ant evaluates when selecting one option to
	 *                              be included in the solution
	 * @param[in] instance Instance of the problem considered
	 */
	void initialise(unsigned numAnts, double q0, double alpha, double beta,
			double initTau, double evaporation, unsigned candidateListSize,
			RPOInstance &instance) {
		_instance = &instance;
		_q0 = q0;
		_alpha = alpha;
		_beta = beta;
		_initTau = initTau;
		_evaporation = evaporation;

		if (numAnts <= 0) {
			cerr << "The number of ants must be greater than 0" << endl;
			exit(1);
		}

		if (_bestSolution != NULL) {
			delete _bestSolution;
			_bestSolution = NULL;
		}

		//Generation of an initial solution for _bestSolution
		_bestSolution = new RPOSolution(*_instance);
		RPOSolGenerator::genRandomSol(*_instance, *_bestSolution);
		double fitness = RPOEvaluator::computeFitness(*_instance,
				*_bestSolution);
		_bestSolution->setFitness(fitness);


		// Generation of the ants: create empty ants (using the constructor) and include them in _ants
		//RPOAnt *auxiliarAnt=new RPOAnt(candidateListSize, this);//RPOAnt(unsigned candidateListSize, RPOAntColonyOpt *colony)
		for (unsigned i = 0; i < numAnts; i++) {
			_ants.push_back(new RPOAnt(candidateListSize, this));
		}


		// Initialization of the pheromone matrix by using _initTau
		unsigned numParams = _instance->getNumParameters();

		for (unsigned i = 0; i < numParams; i++) {
			vector<double> *aVector = new vector<double>;
			_phMatrix.push_back(aVector);
			unsigned dLenght=_instance->getLengthD(i);
			for (unsigned j = 0; j < dLenght; j++) {
				aVector->push_back(_initTau); //Use _initTau
			}
		}
	}

	/**
	 * Function which runs the ACO algorithm
	 * @param[in] stopCondition Object which defines when the stop condition is reached
	 */
	virtual void run(RPOStopCondition &stopCondition) {

		if (_instance == NULL) {
			cerr << "The ACO algorithm has not been initialised" << endl;
			exit(1);
		}
		// While the stop condition is not met, call to the iterate method
		while (stopCondition.reached() == false) {
			iterate();
			stopCondition.notifyIteration();
		}
	}

	/**
	 * Function which returns the vector with the fitness of the best solutions generated per iteration
	 * @return Fitness of the best solutions generated per iteration
	 */
	const vector<double>& getBestPerIteration() const {
		return _bestPerIteration;
	}

	/**
	 * Function which returns the vector with the average fitness value of the solutions generated per iteration
	 * @return Average fitness value of the solutions generated per iteration
	 */
	const vector<double>& getAntsMeanResults() const {
		return _currentItMeans;
	}

	/**
	 * Function which returns the fitness of the solutions generated in each moment
	 * @return Fitness of the solutions generated in each moment
	 */
	vector<double>& getResults() {
		return _results;
	}
};

#endif /* INCLUDE_RPOANTCOLONYOPT_H_ */
