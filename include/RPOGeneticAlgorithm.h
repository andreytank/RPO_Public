/**
 * RPOGeneticAlgorithm.h
 *
 * File defining the class RPOGeneticAlgorithm.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef INCLUDE_RPOGENETICALGORITHM_H_
#define INCLUDE_RPOGENETICALGORITHM_H_

#include <RPOMetaheuristic.h>
#include <RPOSolution.h>
#include <SelectionOperator.h>
#include <RPOCrossoverOperator.h>
#include <RPOMutationOperator.h>
#include <TournamentSelector.h>
#include <vector>
#include <iostream>

using namespace std;

/**
 * Class which implements a Generational Genetic Algorithm with Elitism for the RPO
 */
class RPOGeneticAlgorithm: public RPOMetaheuristic {
protected:
	/**
	 * Properties of the class:
	 *  _popSize Size of the population
	 *  _population Set of individuals in the population
	 *  _selector Selection operator
	 *  _crossoverOp Crossover operator
	 *  _mutOp Mutation operator
	 *  _instancia Instance of the problem considered
	 */
	unsigned _popSize;
	vector<Solution*> _population;
	SelectionOperator *_selector = NULL;
	RPOCrossoverOperator *_crossoverOp = NULL;
	RPOMutationOperator *_mutOp = NULL;
	RPOInstance *_instance = NULL;

	/**
	 * Vectors where the results will be stored
	 *  _results Fitness values of the solutions generated
	 *  _popMeanResults Average fitness values of the solutions in the population
	 *  _offMeanResults Average fitness values of the new solutions generated
	 *  _bestPerIterations Best fitness value in each iteration
	 */
	vector<double> _results;
	vector<double> _popMeanResults;
	vector<double> _offMeanResults;
	vector<double> _bestPerIterations;

	/**
	 * Function which tries to find the index of the best solution in a vector
	 * @param[in] set Vector with solutions
	 * @return index of the best solution
	 */
	unsigned indexBest(vector<Solution*> &set) {

		//Search for the index of the best solution in set
		unsigned indexBestOne=0;
		double bestFitness = set[0]->getFitness();

		for(unsigned i=1; i<set.size(); i++){
			if(set[i]->getFitness() > bestFitness){
				bestFitness = set[i]->getFitness();
				indexBestOne = i;
			}
		}

		return indexBestOne;
	}

	/**
	 * Function which tries to find the index of the worst solution in a vector
	 * @param[in] set Vector with solutions
	 * @return index of the worst solution
	 */
	unsigned indexWorst(vector<Solution*> &set) {

		// Search for the index of the worst solution in set
		unsigned indexWorst=0;
		double worstFitness = set[0]->getFitness();
		for(unsigned i=1;i<set.size();i++){
			if(set[i]->getFitness() < worstFitness){
				worstFitness = set[i]->getFitness();
				indexWorst=i;
			}
		}

		return indexWorst;
	}

	/**
	 * Function which updates the new population, given the offspring generated
	 * @param[in] offspring Vector of the offspring generated
	 */
	void selectNewPopulation(vector<Solution*> &offspring) {

		/**
		 *
		 * The new population will be the offspring, but, in case that the current
		 * population has a better solution than the best one in offspring,
		 * the best current solution will replace the worst one in offspring.
		 *
		 * 1. Find the index of the best solution in _population
		 * 2. Find the index of the best solution in _offspring
		 * 3. If the best solution in _population is better than the best solution in _offspring
		 *   a. Find the index of the worst solution in _offspring
		 *   b. Make that solution copy the information of the best solution in _population
		 * 4. Delete the individuals in the current population (freeing memory)
		 * 5. Store the individuals of _offspring in current population
		 */
		unsigned int indexBestPop = indexBest(_population);
		unsigned int indexBestOff = indexBest(offspring);

		if(_population[indexBestPop]->getFitness() > offspring[indexBestOff]->getFitness()){
			unsigned worst = indexWorst(offspring);
			offspring[worst]->copy(*_population[indexBestPop]);
		}

		//Delete the individuals in current population
		for (unsigned i = 0; i < _popSize; i++) {
			delete (_population.back());
			_population.pop_back();
		}

		//Copy the offspring in the current population
		unsigned offSize = (unsigned) offspring.size();

		for (unsigned i = 0; i < offSize; i++) {
			_population.push_back(offspring.back());
			offspring.pop_back();
		}
	}

	/**
	 * Function which evaluates the solutions of a vector
	 * @param[in,out] set Set of solution to be evaluated. Once evaluated, their fitness
	 *                    should be assigned
	 */
	void evaluate(vector<Solution*> &set) {

		for (Solution *sol : set) {
			RPOSolution *s = (RPOSolution*) sol;

			/**
			 *
			 * A new functionality has been included in Solution to detect if its fitness was previously calculated.
			 * which is useful when the descendant is a copy of the parent.
             * In this way, only those solutions which has an invalid fitness will be evaluated
			 */
			if (!(s->hasValidFitness())) {

				double fitness= RPOEvaluator::computeFitness(*_instance, *s);

				_results.push_back(fitness);
				s->setFitness(fitness);


				if (RPOEvaluator::compare(fitness, _bestSolution->getFitness()) > 0){
					_bestSolution->copy(*s);
				}
			}
		}
	}

	/**
	 * Function which initializes the population of the GA
	 * @param[in] popSize Size of the population
	 */
	void initPopulation(unsigned popSize) {

		if (_instance == NULL) {
			cerr
					<< "The evolutionary algorithm has not been initialised. At least, its _instance is NULL"
					<< endl;
			exit(1);
		}

		/**
		 *
		 * For each iteration:
		 *  1. Generate a random solution
		 *  2. Evaluate it and assign its fitness
		 *  3. Update the _bestSolution (remember to use the function compare(,))
		 *  4. Include the solution in the population (done)
		 */
		for (unsigned i = 0; i < popSize; i++) {
			RPOSolution *sol = new RPOSolution(*_instance);
			RPOSolGenerator::genRandomSol(*_instance, *sol);
			double fitness = RPOEvaluator::computeFitness(*_instance, *sol);
			_results.push_back(fitness);
			_population.push_back(sol);
		}
	}

	/**
	 * Function which obtains the average fitness of a set of solutions
	 * @param[int] set Set of solutions from which obtaining the average fitness
	 * @return Average fitness of the solutions
	 */
	double computeMeanFitness(vector<Solution*>&set) {
		double mean = 0.;
		unsigned numElements = (unsigned) set.size();
		double i_numElements = 1. / numElements;

		for (auto sol : set) {
			double fitness = sol->getFitness();
			mean += (fitness * i_numElements);
		}

		return mean;
	}

public:

	/**
	 * Constructor
	 */
	RPOGeneticAlgorithm() {
		_popSize = 0;
		_bestSolution = NULL;
	}

	/**
	 * Destructor
	 */
	virtual ~RPOGeneticAlgorithm() {

		for (unsigned i = 0; i < _popSize; i++) {
			delete (_population.back());
			_population.pop_back();
		}

		if (_bestSolution != NULL){
			delete _bestSolution;
			_bestSolution = NULL;
		}
	}

	/**
	 * Function which runs the genetic algorithm
	 * @param[in] stopCondition Object which defines when the stop condition has been met
	 */
	virtual void run(RPOStopCondition &stopCondition) {

		/**
		 *
		 * 1. Initialize the population
		 * 2. While the stop condition is not met
		 *   a. Store the average of the current population and the best solution
		 *   b. Select the parents
		 *   c. Apply crossover to the parents
		 *   d. Mutate the descendants
		 *   f. Store the average of the descendants
		 *   g. Select the new population
		 *
		 * 3. Store the average of the final population and the best solution
		 */

		initPopulation(this->_popSize);
				while (stopCondition.reached() == false) {

					_popMeanResults.push_back(computeMeanFitness(_population));
					_bestPerIterations.push_back(
							_population.at(indexBest(_population))->getFitness());

					vector<Solution*> parents;
					_selector->select(_population,parents); // Selection of parents

					vector<Solution*> offspring;
					_crossoverOp->cross(parents, offspring); // Crossover
					_mutOp->mutate(offspring);// Mutation
					evaluate(offspring);// Evaluate
					_offMeanResults.push_back(computeMeanFitness(offspring));

					selectNewPopulation(offspring); // Selection of the offspring (replacement)
					stopCondition.notifyIteration();
				}

				_popMeanResults.push_back(computeMeanFitness(_population));
				_bestPerIterations.push_back(
				_population.at(indexBest(_population))->getFitness());
	}

	/**
	 * Function which initializes the algorithm
	 * @param[in] popSize Size of the population
	 * @param[in] instance Instance of the problem to be considered
	 */
	void initialise(unsigned popSize, RPOInstance &instance) {
		_instance = &instance;

		if (popSize <= 0) {
			cerr << "The population size must be greater than 0" << endl;
			exit(1);
		}

		if (_bestSolution != NULL) {
			delete _bestSolution;
			_bestSolution = NULL;
		}

		_bestSolution = new RPOSolution(*_instance);
		RPOSolGenerator::genRandomSol(*_instance, *_bestSolution);
		double fitness = RPOEvaluator::computeFitness(*_instance, *_bestSolution);
		_bestSolution->setFitness(fitness);

		_popSize = popSize;

		/**
		 * It is configured to use binary tournament selection, the genetic operators
		 * implemented and the corresponding probabilities. But other operators could be used
		 * by simply changing the object.
		 */
		if (_crossoverOp == NULL) {
			_crossoverOp = new RPOCrossoverOperator(0.8, *_instance);
		}

		if (_mutOp == NULL) {
			_mutOp = new RPOMutationOperator((0.25 / _instance->getNumParameters()),*_instance);
		}

		if (_selector == NULL) {
			_selector = new TournamentSelector(2); //You can try different values of selective pressure
		}
	}

	/**
	 * Function which returns the vector of fitness of the solutions generated
	 */
	vector<double>& getResults() {
		return _results;
	}

	/**
	 * Function which assigns a new crossover operator
	 */
	void setCrossoverOp(RPOCrossoverOperator* crossoverOp) {

		if (_crossoverOp != NULL)
			delete _crossoverOp;

		_crossoverOp = crossoverOp;
	}

	/**
	 * Function which assigns a new mutation operator
	 */
	void setMutOp(RPOMutationOperator* mutOp) {

		if (_mutOp != NULL)
			delete _mutOp;

		_mutOp = mutOp;
	}

	/**
	 * Function which assigns a new selection operator
	 */
	void setSelector(SelectionOperator* selector) {

		if (_selector != NULL)
			delete _selector;

		_selector = selector;
	}

	/**
	 * Function which returns the average of the offspring population in each iteration
	 */
	const vector<double>& getOffMeanResults() const {
		return _offMeanResults;
	}

	/**
	 * Function which returns the vector with the average of the current population in each iteration
	 */
	const vector<double>& getPopMeanResults() const {
		return _popMeanResults;
	}

	/**
	 * Function which returns the best solution of the population in each iteration
	 */
	const vector<double>& getBestsPerIterations() const {
		return _bestPerIterations;
	}
};

#endif /* INCLUDE_RPOGENETICALGORITHM_H_ */
