/*
 * RPOInstance.cpp
 *
 * File defining the methods of the class RPOInstance.
 *
 *
 * @author Carlos García cgarcia@uco.es
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#include "RPOInstance.h"
#include "RPOSolution.h"

RPOInstance::RPOInstance(int size_param,int numfunc): func(size_param,numfunc) {
	_numParameters=func.getNumParameters();
}

RPOInstance::~RPOInstance() {
}

double RPOInstance::getFitness(RPOSolution &solution){
	return func.fitness(solution.getSetofParameters());
}
void RPOInstance::randomPermutation(int numParam, vector<int>& perm) {
	perm.clear();
	//we have to initialise it
	perm.resize(numParam);
	for( int i=0;i<numParam;i++)
	{
		perm[i]=i;
	}
	for( int i=0;i<numParam;i++)
	{
		int num = rand()%numParam;
		//if we put size-1 and we have five elements
		//that will be rand()%4, that will generate numbers from 0 to 3
		//so we ignore the index 4 and we can't do it
		int tmp= perm[i];
		perm[i]=perm[num];
		perm[num]=tmp;
	}
	return;

}
