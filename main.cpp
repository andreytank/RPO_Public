/*
 * main.cpp
 *
 * File which defines the main fucntion for running experiments over the Multiple Quadratic Knapsack Problem.
 * Specifically, these experiments generate random solutions over the four instances and store the best solution
 * found up to the moment.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

//Definition of some parameters of the experimentation
#define MAX_SECONS_PER_RUN 5
#define MAX_SOLUTIONS_PER_RUN 100000
#define NUM_RUNS 5
#define MAX_INITIAL_SOLUTIONS 5



#include <RPOEvaluator.h>
#include <RPOInstance.h>
#include <RPOSolGenerator.h>
#include <RPOSolution.h>
#include <RPOLocalSearch.h>
#include <RPOSimpleFirstImprovementNO.h>
#include <RPOSimpleBestImprovementNO.h>
#include <Timer.h>
#include <RPOSimulatedAnnealing.h>
#include <RPOTabuSearch.h>
#include <RPOGrasp.h>
#include <RPOIteratedGreedy.h>
#include <RPOGeneticAlgorithm.h>
#include <RPOAntColonyOpt.h>
#include <RPOStopCondition.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cfloat>
#include "map"

using namespace std;


//External definition of the seeds for the random number generator (in seeds.cpp)
extern unsigned int numSeeds;
extern unsigned int seeds[];


/**
 * Function to free the memory of a 2D vector
 * @param[in] array 2D vector whose memory is going to be freed
 */
template <typename T>
void free2Darray(vector< vector<T>* > &array){

	int numVectors = array.size();

	for (int i = 0; i < numVectors; i++){
		array.at(i)->clear();
		delete (array.at(i));
	}

	array.clear();
}

/**
 * Function to free the memory of a 3D vector
 * @param[in] array 3D vector whose memory is going to be freed
 */
template <typename T>
void free3Darray(vector< vector <vector<T>*>*> &array){

	int numArrays = array.size();

	for (int i = 0; i < numArrays; i++){
		free2Darray(*(array.at(i)));
		delete (array.at(i));
	}

	array.clear();
}
/**
 * Function for the generation of random solutions, during MAX_SECONS_PER_RUN seconds and with a maximum of MAX_SOLUTIONS_PER_RUN,
 * for the instance passed as a parameter
 * @param[out] results array where we store the fitness values of the randomly generated solutions
 * @param[in] instance Instance of the RPO
 */
void runARandomSearchExperiment(vector<double> &results, RPOInstance &instance){

	RPOSolution currentSolution(instance);
	Timer t;
	unsigned int numSolutions = 0;

	//While there is time and not too many solutions have been generated, generate a random solution and store the result
	while (t.elapsed_time(Timer::VIRTUAL) <= MAX_SECONS_PER_RUN && numSolutions < MAX_SOLUTIONS_PER_RUN){
		RPOSolGenerator::genRandomSol(instance, currentSolution);
		double currentFitness = instance.getFitness(currentSolution);
		results.push_back(currentFitness);
		numSolutions++;
	}
}

void runALSExperiment(vector<double> &currentResults, vector<double> &bestSoFarResults, RPOInstance &instance, RPONeighExplorer &explorer) {

	//Initialization
	RPOLocalSearch ls;
	RPOSolution initialSolution(instance);
	Timer t;
	RPOEvaluator::resetNumEvaluations();

	//Generate a first random solution to initialize bestFitness
	RPOSolGenerator::genRandomSol(instance, initialSolution);
	double currentFitness = RPOEvaluator::computeFitness(instance,
														  initialSolution);
	initialSolution.setFitness(currentFitness);
	currentResults.push_back(currentFitness);
	bestSoFarResults.push_back(currentFitness);
	int numInitialSolutions = 0;

	//While there is time and not too many solutions have been generated,
	//generate a random solution, apply the local search and store
	//the result
	while (t.elapsed_time(Timer::VIRTUAL) <= MAX_SECONS_PER_RUN
		   && RPOEvaluator::getNumEvaluations() < MAX_SOLUTIONS_PER_RUN &&
		   numInitialSolutions < MAX_INITIAL_SOLUTIONS) {

		RPOSolGenerator().genRandomSol(instance, initialSolution);
		currentFitness = RPOEvaluator::computeFitness(instance,
															  initialSolution);
		initialSolution.setFitness(currentFitness);
		/*
		 * 		- Generate a new random solution in initialSolution
		 *      - Obtain its current fitness
		 *      - Set currentFitness to the solution
		 */

		currentResults.push_back(currentFitness);
		bestSoFarResults.push_back(
				max(bestSoFarResults.back(), currentFitness));

		//Call to the method optimise of the object ls to optimise the solution

		ls.optimise(instance, explorer, initialSolution);

		//Store the results
		vector<double> &resultsLS = ls.getResults();

		for (auto aResult : resultsLS) {
			currentResults.push_back(aResult);
			bestSoFarResults.push_back(max(bestSoFarResults.back(), aResult));
		}

		numInitialSolutions++;
	}

}

/**
 * Function returning the number of solutions in the run resulting in more solutions.
 * @param[in] results 2D matrix with the results of the different runs. The first index is for the runs, the second is for the solutions in a given run.
 * @return Longest number of solutions.
 */
unsigned int getLongestRunLength(vector< vector< vector<double>* >* > &results){

	unsigned int maxLength = 0;
	unsigned int numRuns = results.size();

	for (unsigned int iRun = 0; iRun < numRuns; iRun++){

		unsigned long length = results.at(iRun)->at(0)->size();
		if (length > maxLength){
			maxLength = length;
		}
	}

	return maxLength;
}
void compute(vector<double>&best,vector<double>&these){
	best.push_back(these.at(0));
	unsigned int size=these.size();
	for(int i=1;i<size;i++)
		if(these.at(i)<best.back())
			best.push_back(best.back());
		else
			best.push_back(these.at(i));
}

/**
 * Function to apply simulated annealing for a maximum of MAX_SECONS_PER_RUN seconds or a maximum of MAX_SOLUTIONS_PER_RUN
 * for the instance provided
 * @param[out] currentResults Vector where we will store the fitness of the solutions that the simulated annealing
 *                            accepts in any moment.
 * @param[out] bestSoFarResults Vector where we will store the fitness of the best solutions found by the local
 *                              search up to the moment.
 * @param[in] instance Instance of the RPO
 */
void runASAExperiment(vector<double> &currentResults,
					  vector<double> &bestSoFarResults, RPOInstance &instance) {

	//Initialization
	RPOSolution initialSolution(instance);
	RPOSimulatedAnnealing sa;
	RPOStopCondition stopCond;
	RPOEvaluator::resetNumEvaluations();
	sa.initialise(0.9, 10, 0.9999, 50, instance);
	stopCond.setConditions(MAX_SOLUTIONS_PER_RUN, 0, MAX_SECONS_PER_RUN);

	//Generate a first random solution
	RPOSolGenerator::genRandomSol(instance, initialSolution);
	double currentFitness = initialSolution.getFitness();
	double bestFitness = currentFitness;
	currentResults.push_back(currentFitness);
	bestSoFarResults.push_back(bestFitness);
	sa.setSolution(&initialSolution);

	//Apply SA
	sa.run(stopCond);

	//Store the results
	vector<double> &resultsSA = sa.getResults();

	for (auto aResult : resultsSA) {
		currentResults.push_back(aResult);
		bestSoFarResults.push_back(max(bestSoFarResults.back(), aResult));
	}
}

/**
 * Function to apply tabu search for a maximum of MAX_SECONS_PER_RUN seconds or a maximum of MAX_SOLUTIONS_PER_RUN
 * for the instance provided
 * @param[out] currentResults Vector where we will store the fitness of the solutions that the tabu search
 *                            accepts in any moment.
 * @param[out] bestSoFarResults Vector where we will store the fitness of the best solutions found by the local
 *                              search up to the moment.
 * @param[in] instance Instance of the RPO
 */
void runATSExperiment(vector<double> &currentResults,
					  vector<double> &bestSoFarResults, RPOInstance &instance) {

	//Initialization
	RPOSolution initialSolution(instance);
	RPOTabuSearch ts;
	RPOStopCondition stopCond;
	RPOEvaluator::resetNumEvaluations();
	ts.initialise(&instance, ((unsigned)(instance.getNumParameters() / 2.5)));
	stopCond.setConditions(MAX_SOLUTIONS_PER_RUN, 0, MAX_SECONS_PER_RUN);

	//Generate a first random solution
	RPOSolGenerator::genRandomSol(instance, initialSolution);
	double currentFitness = initialSolution.getFitness();
	double bestFitness = currentFitness;
	currentResults.push_back(currentFitness);
	bestSoFarResults.push_back(bestFitness);
	ts.setSolution(&initialSolution);

	//Apply TS
	ts.run(stopCond);

	//Store the results
	vector<double> &resultsTS = ts.getResults();

	for (auto aResult : resultsTS) {
		currentResults.push_back(aResult);
		bestSoFarResults.push_back(max(bestSoFarResults.back(), aResult));
	}
}

/**
 * Function to apply GRASP for a maximum of MAX_SECONS_PER_RUN seconds or a maximum of MAX_SOLUTIONS_PER_RUN
 * for the instance provided
 * @param[out] currentResults Vector where we will store the fitness of the solutions that GRASP
 *                            accepts in any moment.
 * @param[out] bestSoFarResults Vector where we will store the fitness of the best solutions found by the local
 *                              search up to the moment.
 * @param[in] instance Instance of the RPO
 */
void runAGraspExperiment(vector<double> &currentResults,
						 vector<double> &bestSoFarResults, RPOInstance &instance) {

	//Initialization
	RPOSolution initialSolution(instance);
	RPOGrasp grasp;
	RPOStopCondition stopCond;
	RPOEvaluator::resetNumEvaluations();
	grasp.initialise(0.25, instance);
	stopCond.setConditions(MAX_SOLUTIONS_PER_RUN, 0, MAX_SECONS_PER_RUN);

	//Generate a first random solution
	RPOSolGenerator::genRandomSol(instance, initialSolution);
	double currentFitness = initialSolution.getFitness();
	double bestFitness = currentFitness;
	currentResults.push_back(currentFitness);
	bestSoFarResults.push_back(bestFitness);

	//Apply GRASP
	grasp.run(stopCond);

	//Store the results
	vector<double> &resultsGRASP = grasp.getResults();

	for (auto aResult : resultsGRASP) {
		currentResults.push_back(aResult);
		bestSoFarResults.push_back(max(bestSoFarResults.back(), aResult));
	}
}

/**
 * Function to apply iterated greedy for a maximum of MAX_SECONS_PER_RUN seconds or a maximum of MAX_SOLUTIONS_PER_RUN
 * for the instance provided
 * @param[out] currentResults Vector where we will store the fitness of the solutions that iterated greedy
 *                            accepts in any moment.
 * @param[out] bestSoFarResults Vector where we will store the fitness of the best solutions found by the local
 *                              search up to the moment.
 * @param[in] instance Instance of the RPO
 */
void runAIGExperiment(vector<double> &currentResults,
					  vector<double> &bestSoFarResults, RPOInstance &instance) {

	//Initialization
	RPOSolution initialSolution(instance);
	RPOIteratedGreedy ig;
	RPOStopCondition stopCond;
	RPOEvaluator::resetNumEvaluations();
	ig.initialise(0.25, instance);
	stopCond.setConditions(MAX_SOLUTIONS_PER_RUN, 0, MAX_SECONS_PER_RUN);

	//Generate a first random solution
	RPOSolGenerator::genRandomSol(instance, initialSolution);
	double currentFitness = initialSolution.getFitness();
	double bestFitness = currentFitness;
	currentResults.push_back(currentFitness);
	bestSoFarResults.push_back(bestFitness);

	//Apply IG
	ig.run(stopCond);

	//Store the results
	vector<double> &resultsIG = ig.getResults();

	for (auto aResult : resultsIG) {
		currentResults.push_back(aResult);
		bestSoFarResults.push_back(max(bestSoFarResults.back(), aResult));
	}
}

/**
Function to apply the evolutionary algorithm for a maximum of MAX_SECONS_PER_RUN seconds or a maximum of MAX_SOLUTIONS_PER_RUN
 * for the instance provided
 * @param[out] currentResults Vector where we will store the fitness of the solutions generated
 * @param[out] bestSoFarResults Vector where we will store the fitness of the best solutions found up to the moment
 * @param[out] bestPerIterations Vector where we will store the best fitness values of each generation
 * @param[out] popMean Vector where we will store the average fitness values of the population of each generation
 * @param[out] offMean Vector where we will store the average fitness values of the offspring population of each generation
 * @param[in] instance Instance of the RPO
 */
void runAGAExperiment(vector<double> &currentResults,
                      vector<double> &bestSoFarResults, vector<double> &bestPerIterations,
                      vector<double> &popMean, vector<double> &offMean,
                      RPOInstance &instance) {

    //Initialization
    RPOGeneticAlgorithm ga;
    RPOStopCondition stopCond;
    RPOEvaluator::resetNumEvaluations();
    ga.initialise(60, instance);
    stopCond.setConditions(MAX_SOLUTIONS_PER_RUN, 0, MAX_SECONS_PER_RUN);

    //Apply the GA
    ga.run(stopCond);

    //Store the results
    vector<double> &resultsGA = ga.getResults();

    for (double aResult : resultsGA) {
        currentResults.push_back(aResult);

        if (bestSoFarResults.size() > 0)
            bestSoFarResults.push_back(max(bestSoFarResults.back(), aResult));
        else
            bestSoFarResults.push_back(aResult);
    }

    bestPerIterations = ga.getBestsPerIterations();
    popMean = ga.getPopMeanResults();
    offMean = ga.getOffMeanResults();
}

/**
Function to apply the ACO algorithm for a maximum of MAX_SECONS_PER_RUN seconds or a maximum of MAX_SOLUTIONS_PER_RUN
 * for the instance provided
 * @param[out] currentResults Vector where we will store the fitness of the solutions generated
 * @param[out] bestSoFarResults Vector where we will store the fitness of the best solutions found up to the moment
 * @param[out] bestPerIterations Vector where we will store the best fitness values of each generation
 * @param[out] antsMean Vector where we will store the average fitness values of the solutions of each iteration
 * @param[in] instance Instance of the RPO
 */
void runAnACOExperiment(vector<double> &currentResults,
                        vector<double> &bestSoFarResults, vector<double> &bestPerIterations,
                        vector<double> &antsMean, RPOInstance &instance) {

    //Initialization
    RPOAntColonyOpt* aco= new RPOAntColonyOpt();
    RPOStopCondition stopCond;
    RPOEvaluator::resetNumEvaluations();
    aco->initialise(10, 0.7, 0.1, 0.5, 10, 0.0000001, 4000, instance);
    stopCond.setConditions(MAX_SOLUTIONS_PER_RUN, 0, MAX_SECONS_PER_RUN);
    //Run the ACO
    aco->run(stopCond);

    //Store the results
    vector<double> &resultsACO = aco->getResults();

    for (double aResult : resultsACO) {
        currentResults.push_back(aResult);

        if (bestSoFarResults.size() > 0)
            bestSoFarResults.push_back(max(bestSoFarResults.back(), aResult));
        else
            bestSoFarResults.push_back(aResult);
    }

    bestPerIterations = aco->getBestPerIteration();
    antsMean = aco->getAntsMeanResults();
	delete aco;
}

void runRandomExperiment(vector<double> &currentResults,
						 vector<double> &bestSoFarResults, RPOInstance &instance){
	vector< vector< vector< double>* >* >*auxres=new vector< vector< vector< double>* >* >;
	//Run NUM_RUNS executions over this instance
	for (unsigned int r = 1; r <= NUM_RUNS && r < numSeeds; r++){
		vector< vector < double >* >* resultsRand = new vector< vector< double >* >;
		srand(seeds[r]); //Initialization of the random number generator before each execution
		vector<double> *theseResults = new vector<double>;
		resultsRand->push_back(theseResults);
		runARandomSearchExperiment(*theseResults, instance);
		vector<double> *bestResults = new vector<double>;
		compute(*bestResults,*theseResults);
		resultsRand->push_back(bestResults);
		auxres->push_back(resultsRand);
	}
	unsigned int longestrun=getLongestRunLength(*auxres);

	for(unsigned int i=0;i<longestrun;i++){
		int counter=0;
		double sumCur=0, sumBest=0;
		for(int irun=0;irun<NUM_RUNS;irun++)
			if(auxres->at(irun)->at(0)->size()>i){
				sumCur+=auxres->at(irun)->at(0)->at(i);
				sumBest+=auxres->at(irun)->at(1)->at(i);
				counter++;
			}
		currentResults.push_back(sumCur/counter);
		bestSoFarResults.push_back(sumBest/counter);
	}

	free3Darray(*auxres);

}
/**
 * Function which runs all the experiments for arguments passed in the command line to the main program.
 * Specifically, NUM_RUNS experiments for each instance.
 * @param[out] results 3D matrix where the results will be saved. The first index will be the instance considered,
 *                     the second index will be the experiment and the third index will be the solution generated
 *                     in this experiment.
 */
void runExperiments(vector< vector< vector< double>* >* > &results, int numfunc, vector<string>&methanames){
	//Read the instance and create the data structures needed to store the results
	RPOInstance instance(50,numfunc);
	//Random
		methanames.push_back("RS");
        vector<double>*avCurr=new vector<double>;
		vector<double>*avBest=new vector<double>;
		vector< vector < double >* >* resultsRand = new vector< vector< double >* >;
		//resultsRand->push_back(avCurr);
		resultsRand->push_back(avBest);
		results.push_back(resultsRand);
		runRandomExperiment(*avCurr,*avBest,instance);
	//LS
		methanames.push_back("LS-F");
		methanames.push_back("LS-B");
		RPOSimpleFirstImprovementNO firstExplorer;
		RPOSimpleBestImprovementNO bestExplorer;
		vector< vector < double >* >* resultsLSF = new vector<vector<double>*>;
		vector< vector < double >* >* resultsLSB = new vector<vector<double>*>;
		results.push_back(resultsLSF);
		results.push_back(resultsLSB);
		//Run the first improvement local search
		vector<double> *theseFirstResults = new vector<double>;
		vector<double> *bestFirstResults = new vector<double>;
		//resultsLSF->push_back(theseFirstResults);
		resultsLSF->push_back(bestFirstResults);
		runALSExperiment(*theseFirstResults, *bestFirstResults, instance, firstExplorer);

		//Run the best improvement local search
		vector<double> *theseBestResults = new vector<double>;
		vector<double> *bestBestResults = new vector<double>;
		//resultsLSB->push_back(theseBestResults);
		resultsLSB->push_back(bestBestResults);
		runALSExperiment(*theseBestResults, *bestBestResults, instance, bestExplorer);

	//SA+TS+GRASP+Greedy
		methanames.push_back("SA");
		methanames.push_back("TS");
		methanames.push_back("GRASP");
		methanames.push_back("Gr");
        vector<vector<double>*>* resultsSA = new vector< vector<double>*>;
		vector<vector<double>*>* resultsTS = new vector< vector<double>*>;
		vector<vector<double>*>* resultsGRASP = new vector< vector<double>*>;
		vector<vector<double>*>* resultsGr = new vector< vector<double>*>;
		results.push_back(resultsSA);
		results.push_back(resultsTS);
		results.push_back(resultsGRASP);
		results.push_back(resultsGr);

		//Run the simulated annealing
		theseFirstResults = new vector<double>;
		bestFirstResults = new vector<double>;
		//resultsSA->push_back(theseFirstResults);
		resultsSA->push_back(bestFirstResults);
		runASAExperiment(*theseFirstResults, *bestFirstResults, instance);

		//Run the tabu search
		theseFirstResults = new vector<double>;
		bestFirstResults = new vector<double>;
		//resultsTS->push_back(theseFirstResults);
		resultsTS->push_back(bestFirstResults);
		runATSExperiment(*theseFirstResults, *bestFirstResults, instance);

		//Run GRASP
		theseFirstResults = new vector<double>;
		bestFirstResults = new vector<double>;
		//resultsGRASP->push_back(theseFirstResults);
		resultsGRASP->push_back(bestFirstResults);
		runAGraspExperiment(*theseFirstResults, *bestFirstResults, instance);

		//Run iterated greedy
    	theseFirstResults = new vector<double>;
		bestFirstResults = new vector<double>;
		//resultsGr->push_back(theseFirstResults);
		resultsGr->push_back(bestFirstResults);
		runAIGExperiment(*theseFirstResults, *bestFirstResults, instance);

    //GA+ACO
		methanames.push_back("GA");
		methanames.push_back("ACO");
		vector<vector<double>*>* resultsGA = new vector<vector<double>*>;
		results.push_back(resultsGA);
		//Run the evolutionary algorithm
        vector<double> *theseResults = new vector<double>;
        vector<double> *bestResults = new vector<double>;
        vector<double> *bestPerIterations = new vector<double>;
        vector<double> *popMeanResults = new vector<double>;
        vector<double> *offMeanResults = new vector<double>;
		//resultsGA->push_back(theseResults);
		resultsGA->push_back(bestResults);
		//resultsGA->push_back(bestPerIterations);
		//resultsGA->push_back(popMeanResults);
		//resultsGA->push_back(offMeanResults);

		runAGAExperiment(*theseResults, *bestResults, *bestPerIterations,
			 *popMeanResults, *offMeanResults, instance);

		//Run the ant colony optimization algorithm
        vector<vector<double>*>* resultsACO = new vector<vector<double>*>;
        results.push_back(resultsACO);
        theseResults = new vector<double>;
        bestResults = new vector<double>;
        bestPerIterations = new vector<double>;
        vector<double> *antsMean = new vector<double>;
        //resultsACO->push_back(theseResults);
        resultsACO->push_back(bestResults);
        //resultsACO->push_back(bestPerIterations);
        //resultsACO->push_back(antsMean);
        runAnACOExperiment(*theseResults, *bestResults, *bestPerIterations,*antsMean, instance);
    //cout << endl;
}

/**
 * Main function running the experimentation. Generation of random solutions for the instances.
 *
 * Finally, the results generated in matrix form are written to the standard output. For each instance, two columns are generated,
 * one with the average results of the solutions generated in each iteration and other with average results
 * of the best solution found from the beginning to one given iteration.
 */
int main() {
	for(int numfunc=1;numfunc<=26;numfunc++) {
		//////////////////////////
		//Experiment execution
		//////////////////////////
		vector<vector<vector<double> *> *> allTheResults;
		//All the fitness values generated will be saved in the matrix allTheResults
		//It is a 3D matrix
		//-> First index for the instance
		//-> Second index for the runs performed over the same instance
		//-> Third index for the solutions generated during the run
		vector<string> methanames;
		runExperiments(allTheResults, numfunc,methanames);

		//////////////////////
		// Printing the results
		//////////////////////
        char FileName[30];
        sprintf(FileName, "%dRanks50.txt", numfunc);
        ofstream out(FileName);
        int iterationsize[allTheResults.size()], largest;
        largest=iterationsize[0]=allTheResults.at(0)->at(0)->size();
        for(int meth=1; meth<allTheResults.size();meth++){
            iterationsize[meth] =allTheResults.at(meth)->at(0)->size();
            if(iterationsize[meth]>largest)
                largest=iterationsize[meth];
        }
        for(int iteration=0;iteration<largest;iteration++){
            std::map<double,int> count;
            std::map<double,unordered_set<int>> list;
            for(int meth=0;meth<9;meth++) {
                double value;
                if (allTheResults.at(meth)->at(0)->size() > iteration) {
                    value= allTheResults.at(meth)->at(0)->at(iteration);
                } else{
                    value = allTheResults.at(meth)->at(0)->back();
                }
                count[value]++;
                list[value].insert(meth);
            }
            double meth[9];
            int i=0;
            for(auto&[first,second]:list){
                double rank,sum=0;
                int size=count.at(first);
                for(int j=1;j<=size;j++)
                    sum+=i+j;
                sum/=size;
                rank=sum;
                i+=size;
                for(auto nummeth: second)
                    meth[nummeth]=rank;
            }
            for(int i=0;i<9;i++)
                out<<10-meth[i]<<" ";
            out<<endl;
        }
        out.close();
		/*for(int meth=0; meth<allTheResults.size();meth++){
			char FileName[30];
			sprintf(FileName, "_func%d_10D.txt", numfunc);
			methanames[meth].append(FileName);
			ofstream out(methanames[meth]);
			int typesize=2;//allTheResults.at(meth)->size();
			int iterationsize=allTheResults.at(meth)->at(0)->size();
			for(int type=1;type<typesize;type++)
				if(iterationsize<allTheResults.at(meth)->at(type)->size())
					iterationsize=allTheResults.at(meth)->at(type)->size();
			for(int iteration=0;iteration<iterationsize;iteration++) {
				for(int type=0;type<typesize;type++){
					if(iteration<allTheResults.at(meth)->at(type)->size())
						out << fabs(allTheResults.at(meth)->at(type)->at(iteration)) << "\t";
					else
						out << fabs(allTheResults.at(meth)->at(type)->back())<< "\t";
				}
				out<<endl;
			}
			out.close();
		}
		char FileName[30];
		sprintf(FileName, "ACO_Pop_func%d_10D.txt", numfunc);
		ofstream out(FileName);

		int itersize=allTheResults.at(8)->at(2)->size();
		if(itersize<allTheResults.at(8)->at(3)->size())
			itersize=allTheResults.at(8)->at(3)->size();
		for(int iter=0;iter<itersize;iter++) {
			for(int i=2;i<4;i++)
				if (iter < allTheResults.at(8)->at(i)->size())
					out << fabs(allTheResults.at(8)->at(i)->at(iter)) << "\t";
				else
					out << fabs(allTheResults.at(8)->at(i)->back()) << "\t";
			out<<endl;
		}
		out.close();
		sprintf(FileName, "GA_Pop_func%d_10D.txt", numfunc);
		out.open(FileName);
		itersize=allTheResults.at(7)->at(2)->size();
		for(int i=3; i<5;i++)
			if(itersize<allTheResults.at(7)->at(i)->size())
				itersize=allTheResults.at(7)->at(i)->size();
		for(int iter=0;iter<itersize;iter++) {
			for (int i = 2; i < 5; i++)
				if (iter < allTheResults.at(7)->at(i)->size())
					out << fabs(allTheResults.at(7)->at(i)->at(iter)) << "\t";
				else
					out << fabs(allTheResults.at(7)->at(i)->back()) << "\t";
			out<<endl;
		}*/


		//////////////////////
		// Freeing memory
		//////////////////////
		free3Darray(allTheResults);
	}
	return 0;
}
