/*
 * RPOSolution.cpp
 *
 * File defining the methods of the class RPOSolution.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#include "RPOSolution.h"
#include "RPOInstance.h"

RPOSolution::RPOSolution(RPOInstance &instance) {
	_numParametrs = instance.getNumParameters();
	_sol.clear();
	for (int i = 0; i < instance.getNumParameters(); i++)
		_sol.push_back(0);
	_fitness = instance.getFitness(*this);
    _fitnessAssigned=0;
}

RPOSolution::~RPOSolution() {
	_sol.clear();

}

void RPOSolution::ChangeParameterTo(int parameter, int value){
	_sol[parameter] = value;
}

int RPOSolution::parameterValue(int parameter){
	return _sol[parameter];
}

double RPOSolution::getFitness() const{
	return _fitness;
}

void RPOSolution::copy(Solution& solution) {
	RPOSolution &auxSol = (RPOSolution&) solution;
	_sol.clear();
    for (int i = 0; i < auxSol._sol.size(); i++)
        _sol.push_back(auxSol._sol.at(i));
    _fitnessAssigned=auxSol.hasValidFitness();
    _fitness = auxSol._fitness;
}