/*
 * RPOSimpleBestImprovementNO.cpp
 *
 * File defining the methods of the class RPOSimpleBestImprovementNO.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#include <RPOSimpleBestImprovementNO.h>
#include <RPOInstance.h>
#include <RPOSolution.h>
#include <RPOObjectAssignmentOperation.h>
#include <RPOEvaluator.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

bool RPOSimpleBestImprovementNO::findOperation(RPOInstance& instance,
		RPOSolution& solution, RPOChangeOperation& operation) {

	RPOObjectAssignmentOperation *oaOperation = dynamic_cast<RPOObjectAssignmentOperation*>(&operation);
	if (oaOperation == NULL){
		cerr << "RPOSimpleBestImprovementNO::findOperation received an operation object of a class different from RPOObjectAssignmentOperation" << endl;
		exit(1);
	}

	//Create a permutation of the indices of the objects and initialize some variables
	vector<int> perm;
	int numParam = instance.getNumParameters();
	instance.randomPermutation(numParam,perm);
	bool initialised = false;
	double bestDeltaFitness = 0;

	for( int i=0;i<numParam;i++)
	{
		int paramIndex=perm[i];
		/*int paramValue=solution.parameterValue(paramIndex);
		if(paramValue+1<instance.getLengthD(paramIndex)){
			double deltaFitness = RPOEvaluator::computeDeltaFitness(instance, solution,paramIndex, paramValue+1);
			if(deltaFitness>bestDeltaFitness)
			{
				bestDeltaFitness=deltaFitness;
				oaOperation->setValues(paramIndex,paramValue+1,deltaFitness);
			}
		}
		if(paramValue-1>0){
			double deltaFitness = RPOEvaluator::computeDeltaFitness(instance, solution,paramIndex, paramValue-1);
			if(deltaFitness>bestDeltaFitness)
			{
				bestDeltaFitness=deltaFitness;
				oaOperation->setValues(paramIndex,paramValue-1,deltaFitness);
			}
		}*/
		int minValue=0;
		int maxValue=instance.getLengthD(paramIndex)-1;
		for(int value=minValue;value<=maxValue;value++)
		{
			double deltaFitness = RPOEvaluator::computeDeltaFitness(instance, solution,paramIndex, value);
			if(deltaFitness>bestDeltaFitness)
			{
				bestDeltaFitness=deltaFitness;
				oaOperation->setValues(paramIndex,value,deltaFitness);
			}
		}
	}

	if (bestDeltaFitness>0)return true;
	else return false;
}
/*if(initialised==false )
			{
				initialised=true;
				bestDeltaFitness=deltaFitness;
				oaOperation->setValues(paramIndex,j,deltaFitness);
				oaOperation->apply(solution);
			}
			else */