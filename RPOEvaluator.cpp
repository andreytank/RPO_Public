/*
 * RPOEvaluator.cpp
 *
 * File defining the methods of the class RPOEvaluator.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#include <RPOEvaluator.h>
#include <RPOInstance.h>
#include <RPOSolution.h>

unsigned RPOEvaluator::_numEvaluations = 0;

double RPOEvaluator::computeFitness(RPOInstance &instance, RPOSolution &solution){
	double fitness=instance.getFitness(solution);
	return fitness;
}

double RPOEvaluator::computeDeltaFitness(RPOInstance& instance,
										  RPOSolution& solution, int indexParam, int newParamValue) {

	_numEvaluations++;

	double oldFitness=solution.getFitness();
	double oldParamValue=solution.parameterValue(indexParam);
	solution.ChangeParameterTo(indexParam,newParamValue);
	double newFitness=instance.getFitness(solution);
	solution.ChangeParameterTo(indexParam,oldParamValue);
	return newFitness-oldFitness;
}

void RPOEvaluator::resetNumEvaluations() {
	_numEvaluations = 0;
}


