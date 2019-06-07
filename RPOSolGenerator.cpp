/*
 * RPOSolGenerator.cpp
 *
 * File defining the methods of the class RPOSolGenerator.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#include "RPOSolGenerator.h"
#include "RPOInstance.h"
#include "RPOSolution.h"
#include "RPOEvaluator.h"

void RPOSolGenerator::genRandomSol(RPOInstance &instance,RPOSolution &solution){

	int numParam = instance.getNumParameters();

	for (int i = 0; i < numParam; i++){
		int randomValue = (rand() % (instance.getLengthD(i)));
		solution.ChangeParameterTo(i, randomValue);
	}
	solution.setFitness(RPOEvaluator::computeFitness(instance,solution));
}

