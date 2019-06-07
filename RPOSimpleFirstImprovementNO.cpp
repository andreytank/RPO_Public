/*
 * RPOSimpleFirstImprovementNO.cpp
 *
 * File defining the methods of the class RPOSimpleFirstImprovementNO.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#include <RPOSimpleFirstImprovementNO.h>
#include <RPOInstance.h>
#include <RPOEvaluator.h>
#include <RPOObjectAssignmentOperation.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

bool RPOSimpleFirstImprovementNO::findOperation(RPOInstance &instance, RPOSolution &solution, RPOChangeOperation &operation) {

	RPOObjectAssignmentOperation *oaOperation = dynamic_cast<RPOObjectAssignmentOperation*>(&operation);
	if (oaOperation == NULL){
		cerr << "RPOSimpleBestImprovementNO::findOperation received an operation object of a class different from RPOObjectAssignmentOperation" << endl;
		exit(-1);
	}

	//Create a permutation of the indices of the objects and initialize some variables
	vector<int> perm;
	int numParam = instance.getNumParameters();
	instance.randomPermutation(numParam,perm);
	for(int i=0; i<numParam; i++){
		int paramIndex=perm[i];
		/*int paramValue=solution.parameterValue(paramIndex);
		if(paramValue+1<instance.getLengthD(paramIndex)){
			double deltaFitness = RPOEvaluator::computeDeltaFitness(instance, solution,paramIndex, paramValue+1);
			if(deltaFitness>0)
			{
				oaOperation->setValues(paramIndex,paramValue+1,deltaFitness);
				return true;
			}
		}
		if(paramValue-1>0){
			double deltaFitness = RPOEvaluator::computeDeltaFitness(instance, solution,paramIndex, paramValue-1);
			if(deltaFitness>0)
			{
				oaOperation->setValues(paramIndex,paramValue-1,deltaFitness);
				return true;
			}
		}*/
		int minValue=0;
		int maxValue=instance.getLengthD(paramIndex)-1;
		for(int j=minValue;j<=maxValue;j++){
			double deltaFitness=RPOEvaluator::computeDeltaFitness(instance,solution,paramIndex,j);
			if(deltaFitness>0){
				oaOperation->setValues(paramIndex,j,deltaFitness);
				return true;
			}
		}
	}
	return false;
	/*
	 * 1. For each object of the problem (iterating them in the order defined by perm)
	 *   a. For each knapsack of the problem (Note: do not forget any of them)
	 *     i. Obtain the value deltaFitness obtained from assign this object to the knapsack in the solution
	 *
	 *     ii. If deltaFitness is positive
	 *       . Update the values of the operation in oaOperatio
	 *       . Exit returning true
	 *
	 * 2. If you reach this point, no positive deltaFitness was found so you return false
	 *
	 */
}



