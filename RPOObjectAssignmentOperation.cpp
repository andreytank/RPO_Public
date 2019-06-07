/*
 * RPOObjectAssignmentOperation.cpp
 *
 * File defining the methods of the class RPOObjectAssignmentOperation.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#include <RPOObjectAssignmentOperation.h>

RPOObjectAssignmentOperation::RPOObjectAssignmentOperation(){
	_paramValue = 0;
	_indexParam = 0;
	_deltaFitness = 0;
}

RPOObjectAssignmentOperation::~RPOObjectAssignmentOperation() {
}

void RPOObjectAssignmentOperation::apply(RPOSolution& solution) {
	/* 1. Assign the index _indexObj to the knapsack _indexKnapsack in solution
	 * 2. Update the fitness of the solution increasing it by _deltaFitness
	 */
	solution.ChangeParameterTo(_indexParam, _paramValue);
	double actualFitness = solution.getFitness();
	double newFitness = actualFitness + _deltaFitness;
	solution.setFitness(newFitness);
}

void RPOObjectAssignmentOperation::setValues(int indexParam,
		int paramValue, double deltaFitness) {
	/* Store the values passed as an argument in the properties of the class */
	_indexParam = indexParam;
	_paramValue = paramValue;
	_deltaFitness = deltaFitness;
}
